from scapy.all import *
import scapy.contrib.igmp

igmp_packet = IP(dst="224.0.0.1")/scapy.contrib.igmp.IGMP()


send(igmp_packet)
