from scapy.all import *

# Create a packet with an ICMPv6 Echo Request
ip6_icmp = IPv6()
ip6_icmp.src = "fe80::90ce:1827:6881:68fd"
ip6_icmp.dst = "fe80::90ce:1827:6881:6899" # Non-existing address
ip6_icmp.nh = 58 # ICMPv6
icmp6 = ICMPv6EchoRequest()
icmp6.id = 2222
icmp6.seq = 3333
icmp6.data = "Hello World"
send(ip6_icmp/icmp6)