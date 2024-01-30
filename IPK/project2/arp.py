from scapy.all import *

arp_pkt = Ether(dst="ff:ff:ff:ff:ff:ff")/ARP(op=1, hwsrc="FF:FF:FF:FF:FF:FF", psrc="192.168.1.1", pdst="192.168.1.2")

# Send the packet
sendp(arp_pkt)