#include <iostream>
#include <iomanip>
#include <pcap.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <linux/if_ether.h>


void print_packet_info(const u_char* packet, struct pcap_pkthdr* header)
{
    // Check if header is null
    if (header == NULL) {
        std::cout << "Invalid packet header" << std::endl;
        return;
    }

    // Get the timestamp of the packet
    char timestamp[26];
    strftime(timestamp, 26, "%Y-%m-%d %H:%M:%S.%06ld", localtime(&header->ts.tv_sec));
    std::cout << "timestamp: " << timestamp << std::endl;

    // Extract the Ethernet header
    if (header->caplen < sizeof(struct ethhdr)) {
        std::cout << "Packet is too small" << std::endl;
        return;
    }

    // Extract the Ethernet header
    struct ethhdr* ethernet_header = reinterpret_cast<struct ethhdr*>(const_cast<u_char*>(packet));

    if (ethernet_header != nullptr) {
        std::cout << "src MAC: " << std::hex << std::setw(2) << std::setfill('0') << (int)ethernet_header->h_source[0] << ":" 
            << std::hex << std::setw(2) << std::setfill('0') << (int)ethernet_header->h_source[1] << ":"
            << std::hex << std::setw(2) << std::setfill('0') << (int)ethernet_header->h_source[2] << ":"
            << std::hex << std::setw(2) << std::setfill('0') << (int)ethernet_header->h_source[3] << ":"
            << std::hex << std::setw(2) << std::setfill('0') << (int)ethernet_header->h_source[4] << ":"
            << std::hex << std::setw(2) << std::setfill('0') << (int)ethernet_header->h_source[5] << std::endl;
        std::cout << "dst MAC: " << std::hex << std::setw(2) << std::setfill('0') << (int)ethernet_header->h_dest[0] << ":" 
            << std::hex << std::setw(2) << std::setfill('0') << (int)ethernet_header->h_dest[1] << ":"
            << std::hex << std::setw(2) << std::setfill('0') << (int)ethernet_header->h_dest[2] << ":"
            << std::hex << std::setw(2) << std::setfill('0') << (int)ethernet_header->h_dest[3] << ":"
            << std::hex << std::setw(2) << std::setfill('0') << (int)ethernet_header->h_dest[4] << ":"
            << std::hex << std::setw(2) << std::setfill('0') << (int)ethernet_header->h_dest[5] << std::endl;
    } else {
        std::cout << "ethernet_header is null" << std::endl;
    }

    // Extract the IP header, if present
    if (ntohs(ethernet_header->h_proto) == ETH_P_IP) {
        struct iphdr* ip_header = (struct iphdr*)(packet + sizeof(struct ethhdr));

        // Print the source and destination IP addresses
        char src_ip[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET, &ip_header->saddr, src_ip, sizeof(src_ip));
        printf("src IP: %s\n", src_ip);

        char dst_ip[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET, &ip_header->daddr, dst_ip, sizeof(dst_ip));
        printf("dst IP: %s\n", dst_ip);

        // Extract the IP header, if present
        if (ntohs(ethernet_header->h_proto) == ETH_P_IP && header->caplen >= sizeof(struct ethhdr) + sizeof(struct iphdr)) {
            struct iphdr* ip_header = (struct iphdr*)(packet + sizeof(struct ethhdr));

            // Print the source and destination IP addresses
            char src_ip[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET, &ip_header->saddr, src_ip, sizeof(src_ip));
            std::cout << "src IP: " << src_ip << std::endl;

            char dst_ip[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET, &ip_header->daddr, dst_ip, sizeof(dst_ip));
            std::cout << "dst IP: " << dst_ip << std::endl;

            // Extract the TCP/UDP header, if present
            if (ip_header->protocol == IPPROTO_TCP || ip_header->protocol == IPPROTO_UDP) {
                void* transport_header = (void*)(packet + sizeof(struct ethhdr) + sizeof(struct iphdr));

                // Ensure the packet has enough length to contain the transport header
                if (header->caplen < (const u_char*)transport_header - packet + (ip_header->protocol == IPPROTO_TCP ? sizeof(struct tcphdr) : sizeof(struct udphdr))) {
                    std::cout << "Packet is too small" << std::endl;
                    return;
                }

                // Print the source and destination port numbers
                if (ip_header->protocol == IPPROTO_TCP) {
                    struct tcphdr* tcp_header = (struct tcphdr*)transport_header;
                    std::cout << "src port: " << ntohs(tcp_header->source) << std::endl;
                    std::cout << "dst port: " << ntohs(tcp_header->dest) << std::endl;
                } else {
                    struct udphdr* udp_header = (struct udphdr*)transport_header;
                    std::cout << "src port: " << ntohs(udp_header->source) << std::endl;
                    std::cout << "dst port: " << ntohs(udp_header->dest) << std::endl;
                }

                // Print the byte offset of the payload
                size_t byte_offset = (const u_char*)transport_header - packet;
                printf("byte_offset: %04lx ", byte_offset);

                // Print the hexadecimal and ASCII representations of the payload
                const u_char* payload = packet + byte_offset;
                size_t payload_length = header->caplen - byte_offset;

                std::cout << "payload:" << std::endl;
                const int bytes_per_line = 16;
                for (size_t i = 0; i < payload_length; i++) {
                    // Print the hexadecimal representation of the byte
                    printf("%02X ", payload[i]);

                    // Print the ASCII representation of the byte, or a dot if it's not printable
                    if (isprint(payload[i])) {
                        std::cout << payload[i];
                    } else {
                        std::cout << ".";
                    }

                    // Print a newline every `bytes_per_line` bytes
                    if ((i + 1) % bytes_per_line == 0) {
                        std::cout << std::endl;
                    }
                }

                // Print a final newline if necessary
                if (payload_length % bytes_per_line != 0) {
                    std::cout << std::endl;
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    char* dev = argv[1];
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        std::cerr << "Could not open device " << dev << ": " << errbuf << std::endl;
        return 1;
    }

    // Compile and apply the filter expression to the capture device
    struct bpf_program filter;
    char filter_exp[] = "ether";
    if (pcap_compile(handle, &filter, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1) {
        std::cerr << "Error compiling filter expression \"" << filter_exp << "\": " << pcap_geterr(handle) << std::endl;
        return 1;
    }
    if (pcap_setfilter(handle, &filter) == -1) {
        std::cerr << "Error applying filter expression \"" << filter_exp << "\": " << pcap_geterr(handle) << std::endl;
        return 1;
    }

    // Enter the loop to capture and process packets
    pcap_loop(handle, -1, (pcap_handler)print_packet_info, NULL);

    // Close the capture device and free resources
    pcap_freecode(&filter);
    pcap_close(handle);

    return 0;
}
