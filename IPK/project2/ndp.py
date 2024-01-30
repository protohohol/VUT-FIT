from scapy.all import *


base = IPv6(dst='fe80::1234')
router_solicitation = ICMPv6ND_RS()
src_ll_addr = ICMPv6NDOptSrcLLAddr(lladdr='01:23:45:67:89:ab')
packet = base/router_solicitation/src_ll_addr
send(packet)