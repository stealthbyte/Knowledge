import socket
import argparse

def simple_fuzz(target, port, header, count):
    for i in range(count):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((target, port))
        x = "GET HTTP/1.1 / \r\n"
        x += header + ":" + 'A' * i + "\r\n\r\n"
        print("Sending with count = " + str(i) + " \n" + x)
        s.send(x.encode('utf-8'))
        print(s.recv(1024))


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("target")
    parser.add_argument("port")
    parser.add_argument("header")
    parser.add_argument("count")
    args = parser.parse_args()
    simple_fuzz(args.target, int(args.port), args.header, int(args.count))
