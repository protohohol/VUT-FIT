from scapy.all import *

# create IP header
ip = IP(dst="192.168.0.1")

# create ICMP layer (Echo Request)
icmp = ICMP()

# combine the IP and ICMP layers
packet = ip/icmp/"Hello, World!"

# send the packet
send(packet)