import os
"""
Helper functions for CNO Dev Course Networks Series
"""
import binascii
import socket
import struct

ETH_TYPES = {'ARP': 0x0806,
             'IP': 0x0800,
             'IPv6': 0x86dd}


def hex_dumps(frame):
    output = "      0  1  2  3  4  5  6  7 |  8  9  a  b  c  d  e  f\n"
    output += "_" * 54 + "\n"
    line_index = 0
    for i, b in enumerate(frame):
        if i == 0:
            output += "%04x %02x " % (line_index, b)
            line_index += 16
            continue

        if ((i % 8) == 0) and ((i % 16) != 0):
            output += "| "
        if i % 16 == 0:
            output += "\n%04x %02x " % (line_index, b)
            line_index += 16
        else:
            output += "%02x " % b

    return output


def mac_str_to_bin(mac):
    return binascii.unhexlify(mac.replace(':', ''))


def ipv4_str_to_bin(addr):
    return socket.inet_pton(socket.AF_INET, addr)


def ipv6_str_to_bin(addr):
    return socket.inet_pton(socket.AF_INET6, addr)


class err(Exception):
    def __str__(self):
        return "Malformed packet"


def checksum(data):
    length = len(data)
    if length % 2:
        raise err
    sum = 0
    for i in range(0, length//2):
        sum += struct.unpack(">H", data[i*2:(i+1)*2])[0]
    sum = (((sum >> 16) + sum) & 0xffff) ^ 0xffff
    return sum


def rawsend(frame, dev="ens33"):
    """
    Send raw ethernet frame.
    """

    if len(frame) < 14:
        raise Exception("Your frame is not big enough")

    sock = socket.socket(socket.PF_PACKET, socket.SOCK_RAW)
    sock.bind((dev, 0))
    sock.send(frame)
    sock.close()


def rawsend_cksum(frame, dev="ens33"):
    """
    Perform IP, ICMP, UDP, and/or TCP checksum
    calculations and send raw frame.
    """

    try:
        type = struct.unpack(">H", frame[12:14])[0]
    except:
        raise err
    if type != 0x0800:
        raise "Unable to checksum non-IP ethertype 0x%04x" % type
    try:
        ippkt = frame[14:]
        iphlen = (ippkt[0] & 0x0f) * 4
        ipsum = checksum(ippkt[:iphlen])
        ippkt = ippkt[:10] + struct.pack(">H", ipsum) + ippkt[12:]
        ipproto = ippkt[9]
        iptotlen = struct.unpack(">H", ippkt[2:4])[0]
    except:
        raise err

    if ipproto == 1:
        try:
            icmpsum = checksum(ippkt[iphlen:iptotlen])
            ippkt = ippkt[:iphlen+2] + struct.pack(">H", icmpsum) + \
                ippkt[iphlen+4:]
        except:
            raise err
    elif ipproto == 6 or ipproto == 17:
        try:
            pseudohdr = ippkt[12:20] + b"\x00" + \
                ippkt[9:10] + struct.pack(">H", iptotlen - iphlen)
            sum = checksum(pseudohdr + ippkt[iphlen:iptotlen])
            sumoff = iphlen + {6: 16, 17: 6}[ipproto]
            ippkt = ippkt[:sumoff] + struct.pack(">H", sum) + ippkt[sumoff+2:]
        except:
            raise err
    else:
        raise "Unable to checksum IP proto 0x%02x" % ipproto

    rawsend(frame[:14] + ippkt, dev)


def rawsend_cksum_ipv6(frame, dev="ens33"):
    """
    Perform IP, ICMP, UDP, and/or TCP checksum
    calculations and send raw frame.
    """
    # next header fields for all cept tcp, udp, icmp and a couple of other ones...
    lst_hdrs = [b"\x00", b"\x2b", b"\x2c", b"\x2e",
                b"\x32", b"\x33", b"\x3b", b"\x3c"]

    try:
        type = struct.unpack(">H", frame[12:14])[0]
    except:
        raise err
    if type != 0x0800 and type != 0x86dd:
        raise Exception("Unable to checksum non-IP ethertype 0x%04x" % type)

    try:
        ippkt = frame[14:]
        ippayload = struct.unpack(">H", ippkt[4:6])[0]
        if ippayload == 0:
            # calculate payload:
            ippayload = len(ippkt[40:])
            ippkt = ippkt[:4] + struct.pack(">H", ippayload) + ippkt[6:]
        ipnexthdr = ippkt[6]

        # Upper-Layer Packet Length is 32-bit in pseudo header vice 16 bit in IPv6 header
        # If upper layer protocol includes length field, use that, if not, take payload length from ipv6 header, minus length of any extension headers

    except:
        raise err

    if ipnexthdr == 58:
        try:
            # Calculate payload without extension headers:
            ip_pseudohdr = ippkt[8:24] + ippkt[24:40] + b"\x00\x00" + \
                struct.pack(">H", ippayload) + b"\x00\x00\x00" + ippkt[6:7]
            icmpsum = checksum(ip_pseudohdr+ippkt[40:])
            ippkt = ippkt[:40+2] + struct.pack(">H", icmpsum) + ippkt[40+4:]
        except:
            raise err
    elif ipnexthdr == 17:
        # UDP
        try:
            udp_payload = struct.unpack(">H", ippkt[40+4:40+6])
            ip_pseudohdr = ippkt[8:24] + ippkt[24:40] + b"\x00\x00" + \
                struct.pack(">H", udp_payload[0]) + \
                b"\x00\x00\x00" + ippkt[6:7]
            sum = checksum(ip_pseudohdr + ippkt[40:])
            sumoff = 40 + {6: 16, 17: 6}[ipnexthdr]
            udp_len = len(ippkt[40:])-1
            ippkt = ippkt[:sumoff] + struct.pack(">H", sum) + ippkt[sumoff+2:]
            # length auto-calc:
            ippkt = ippkt[:44] + struct.pack(">H", udp_len) + ippkt[46:]
        except:
            raise err
    elif ipnexthdr == 6:
        # TCP
        try:
            ip_pseudohdr = ippkt[8:24] + ippkt[24:40] + b"\x00\x00" + \
                struct.pack(">H", ippayload) + b"\x00\x00\x00" + ippkt[6:7]
            sum = checksum(ip_pseudohdr + ippkt[40:])
            sumoff = 40 + {6: 16, 17: 6}[ipnexthdr]
            ippkt = ippkt[:sumoff] + struct.pack(">H", sum) + ippkt[sumoff+2:]
        except:
            raise err
    else:
        raise Exception("Unable to checksum IP proto 0x%02x" % ipnexthdr)

    rawsend(frame[:14] + ippkt, dev)


try:
    if os.geteuid() != 0:
        print("WARNING:\
                You are not root, you probably should be.\
                ...especially if this is somebody else's box ;-)\
                ")
except AttributeError:
    print("It is suggested that you use a linux machine for this")
