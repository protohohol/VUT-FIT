from scapy.all import *

# Create IP header
ip = IP(src="10.0.0.1", dst="10.0.0.2")

# Create TCP header
tcp = TCP(sport=1234, dport=80, flags="S", seq=1000)

# Create payload
payload = "Hello, World!"

# Combine headers and payload into a packet
packet = ip/tcp/payload

# Send the packet
send(packet)