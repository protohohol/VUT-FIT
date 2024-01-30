from scapy.all import *

ipv6_packet = IPv6(dst="ff02::16")
mld_packet = ICMPv6MLQuery(mladdr="ff02::1:2")
packet = ipv6_packet/mld_packet
send(packet)