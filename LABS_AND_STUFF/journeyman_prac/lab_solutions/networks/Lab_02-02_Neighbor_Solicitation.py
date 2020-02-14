#!/usr/bin/python3.7
import cno_net_helper as cno
import struct

# Global Variables
dev = "ens33"

# Multicast address
ipv6_multicast_mac = cno.mac_str_to_bin(
    "33:33:00:00:00:01")  # IPv6 Multicast MAC

# Destination
dst_mac = ipv6_multicast_mac
dst_ipv6 = cno.ipv6_str_to_bin("a:c:7:9::1")
dst_ipv6_mcast = cno.ipv6_str_to_bin("ff02::1:ff00:1")

# Source
src_mac = cno.mac_str_to_bin("00:0c:29:f2:89:71")          # Host MAC
src_ipv6 = cno.ipv6_str_to_bin(
    "a:c:7:9:725:bca6:2140:bbd")  # Host Ipv6 address


# ICMPv6 and ICMPv6 Options
icmpv6_hdr = b"\x87"     # Type: 135 (neighbor solicitation)
icmpv6_hdr += b"\x00"     # Code: 0
icmpv6_hdr += b"\x00" * 2  # Checksum : Set to 0.  Calculated in send rawsend_cksum
icmpv6_hdr += b"\x00" * 4  # Reserved
icmpv6_hdr += dst_ipv6    # Target IPv6 for solicitation

# ICMPv6 header options
icmpv6_option = b"\x01"  # Type: Source Link-Layer address (1)
icmpv6_option += b"\x01"  # Len: 8 binasciiryasciit
icmpv6_option += src_mac  # Source MAC

# Calculate ICMPv6 length
icmpv6_hdr += icmpv6_option
icmpv6_hdr_len = struct.pack('>H', len(icmpv6_hdr))

# ICMPv6
ipv6_hdr = b"\x60\x00\x00\x00"  # Version (4 bit): 6 (0x6)
# Traffic class (8 bit): 0 (0x00)
# Flow label (20 bit): 0  (0x00000)
# Payload len (2 bytes): ICMPv6 header + ICMPv6 options
ipv6_hdr += icmpv6_hdr_len
ipv6_hdr += b"\x3a"            # Next header: 54 (ICMPv6)
ipv6_hdr += b"\xff"            # Max hop limit: 255
ipv6_hdr += src_ipv6           # Source IPv6 adresses
ipv6_hdr += dst_ipv6           # Destination ICMPv6

# Ethernet
eth_hdr = ipv6_multicast_mac  # Destination MAC : IPv6 multicast MAC
eth_hdr += src_mac            # Source MAC
eth_hdr += struct.pack(">H", cno.ETH_TYPES['IPv6'])        # Type: IPv6

# Contruct Ethernet frame
eth_frame = eth_hdr + ipv6_hdr + icmpv6_hdr

# Send the packet with checksum
cno.rawsend_cksum_ipv6(eth_frame, dev)
