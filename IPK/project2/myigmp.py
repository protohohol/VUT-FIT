from scapy.all import *
from scapy.layers.inet6 import IPv6, ICMPv6EchoRequest, ICMPv6MLQuery

pkt = IPv6(dst="ff02::1", hlim=1)/ICMPv6MLQuery()
send(pkt)
