using namespace std;

#include <iostream>
#include <iomanip>
#include <string.h>
#include <arpa/inet.h>
#include <pcap.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <getopt.h>
#include <ctime>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/igmp.h>
#include <netinet/icmp6.h>
#include <netinet/ip6.h>
#include <netinet/ip_icmp.h>

#define SNAP_LEN 65535
#define SIZE_ETHERNET 14

struct packet_data {
    timeval timestamp;
    u_int8_t src_mac[6];
    u_int8_t dst_mac[6];
    u_int16_t frame_length;
    u_int8_t src_ip[16];
    u_int8_t dst_ip[16];
    u_int16_t src_port;
    u_int16_t dst_port;
    u_int8_t *payload;
    u_int16_t payload_size;
};

void print_usage() {
    cerr << "Usage: ipk-sniffer [-i interface | --interface interface] {-p port [--tcp|-t] [--udp|-u]} [--arp] [--icmp4] [--icmp6] [--igmp] [--mld] {-n num}" << endl;
}

void print_packet_data(const packet_data &data, const u_char *packet) {
    char time_buffer[40];
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%dT%H:%M:%S", localtime(&data.timestamp.tv_sec));
    sprintf(time_buffer + strlen(time_buffer), ".%06ld", data.timestamp.tv_usec);
    char tz_sign = timezone > 0 ? '-' : '+';
    int tz_hours = abs(timezone) / 3600;
    int tz_minutes = abs(timezone % 3600) / 60;
    sprintf(time_buffer + strlen(time_buffer), "%c%02d:%02d", tz_sign, tz_hours, tz_minutes);

    cout << "timestamp: " << time_buffer << endl;

    cout << "src MAC: ";
    for (int i = 0; i < 6; i++) {
        cout << hex << setw(2) << setfill('0') << (int)data.src_mac[i];
        if (i != 5) cout << ":";
    }
    cout << dec << endl;

    cout << "dst MAC: ";
    for (int i = 0; i < 6; i++) {
        cout << hex << setw(2) << setfill('0') << (int)data.dst_mac[i];
        if (i != 5) cout << ":";
    }
    cout << dec << endl;

    cout << "frame length: " << data.frame_length << " bytes" << endl;

    char src_ip_str[INET6_ADDRSTRLEN];
    if (data.src_ip[0] == 4) {
        inet_ntop(AF_INET, data.src_ip + 1, src_ip_str, INET_ADDRSTRLEN);
        cout << "src IP: " << src_ip_str << endl;
    } else if (data.src_ip[0] == 6) {
        inet_ntop(AF_INET6, data.src_ip + 1, src_ip_str, INET6_ADDRSTRLEN);
        cout << "src IP: " << src_ip_str << endl;
    } else {
        cout << "src IP: " << src_ip_str << endl;
    }

    char dst_ip_str[INET6_ADDRSTRLEN];
    if (data.dst_ip[0] == 4) {
        inet_ntop(AF_INET, data.dst_ip + 1, dst_ip_str, INET_ADDRSTRLEN);
        cout << "dst IP: " << dst_ip_str << endl;
    } else if (data.dst_ip[0] == 6) {
        inet_ntop(AF_INET6, data.src_ip + 1, dst_ip_str, INET6_ADDRSTRLEN);
        cout << "dst IP: " << dst_ip_str << endl;
    } else {
        cout << "dst IP: " << dst_ip_str << endl;
    }

    if (data.src_port != 0) {
        cout << "src port: " << ntohs(data.src_port) << endl;
    }

    if (data.dst_port != 0) {
        cout << "dst port: " << ntohs(data.dst_port) << endl;
    }

    if (data.payload_size != 0) {
        cout << endl;
        const u_char *payload = data.payload;
        int payload_remaining = data.payload_size;
        int line_count = 0;
        while (payload_remaining > 0) {
            printf("0x%04x: ", line_count);
            int bytes_to_print = min(16, payload_remaining);
            for (int i = 0; i < bytes_to_print; i++) {
                printf("%02x ", payload[i]);
            }
            for (int i = bytes_to_print; i < 16; i++) {
                printf("   ");
            }
            for (int i = 0; i < bytes_to_print; i++) {
                char c = payload[i];
                if (c < 32 || c > 126) {
                    c = '.';
                }
                printf("%c", c);
            }
            printf("\n");
            payload_remaining -= bytes_to_print;
            payload += bytes_to_print;
            line_count += 16;
        }
    }

}



void process_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    packet_data data;
    
    // Get timestamp
    data.timestamp = header->ts;

    // Get ethernet header
    const struct ether_header *ethernet_header = (const struct ether_header *) packet;

    // Get source MAC
    memcpy(data.src_mac, ethernet_header->ether_shost, 6);

    // Get destination MAC
    memcpy(data.dst_mac, ethernet_header->ether_dhost, 6);

    // Get frame length
    data.frame_length = ntohs(ethernet_header->ether_type);

    // Get IP header
    const struct ip *ip_header = (const struct ip *)(packet + SIZE_ETHERNET);

    // IGMP + TCP + UDP + ICMP
    if (ntohs(ethernet_header->ether_type) == ETHERTYPE_IP) {

        if (ip_header->ip_p == IPPROTO_IGMP) {
            struct igmp *igmp_header = (struct igmp *)(packet + SIZE_ETHERNET + sizeof(struct ip));

            // Get source IP
            if (ip_header->ip_v == 4) {
                data.src_ip[0] = 4;
                memcpy(data.src_ip + 1, &igmp_header->igmp_group, 4);
            } else {
                data.src_ip[0] = 0;
            }
        } else {
            // Get source IP
            if (ip_header->ip_v == 4) {
                data.src_ip[0] = 4;
                memcpy(data.src_ip + 1, &ip_header->ip_src, 4);
            } else {
                data.src_ip[0] = 0;
            }
        }

        // Get destination IP
        if (ip_header->ip_v == 4) {
            data.dst_ip[0] = 4;
            memcpy(data.dst_ip + 1, &ip_header->ip_dst, 4);
        } else {
            data.dst_ip[0] = 0;
        }

        // Get payload
        const u_char *payload = packet + SIZE_ETHERNET + (ip_header->ip_hl * 4);
        if (ip_header->ip_p == IPPROTO_TCP) {
            struct tcphdr *tcp_header = (struct tcphdr *)(payload);
            data.payload = (u_int8_t *)(payload + tcp_header->th_off * 4);
            data.payload_size = ntohs(ip_header->ip_len) - (ip_header->ip_hl * 4) - (tcp_header->th_off * 4);
        } else if (ip_header->ip_p == IPPROTO_UDP) {
            struct udphdr *udp_header = (struct udphdr *)(payload);
            data.payload = (u_int8_t *)(payload + sizeof(udphdr));
            data.payload_size = ntohs(udp_header->uh_ulen) - sizeof(struct udphdr);
        } else if (ip_header->ip_p == IPPROTO_ICMP) {
            struct icmphdr *icmp_header = (struct icmphdr *)(payload);

            // Get payload
            data.payload = (u_int8_t *)(icmp_header + 1);
            data.payload_size = ntohs(ip_header->ip_len) - (ip_header->ip_hl * 4) - sizeof(struct icmphdr);
        } else {
            // other protocols
            data.payload = NULL;
            data.payload_size = 0;
        }


    } else if (ntohs(ethernet_header->ether_type) == ETHERTYPE_IPV6) {
        // MLD + NDP + TCP + UDP + ICMP6

        const struct ip6_hdr *ip_6_header = (const struct ip6_hdr *)(packet + SIZE_ETHERNET);
        struct icmp6_hdr *icmp6_header = (struct icmp6_hdr *)(packet + SIZE_ETHERNET + sizeof(struct ip));

        // Get source IP
        data.src_ip[0] = 6;
        memcpy(data.src_ip + 1, &ip_6_header->ip6_src, 16);

        if (icmp6_header->icmp6_type == ICMP6_MEMBERSHIP_QUERY || icmp6_header->icmp6_type == ICMP6_MEMBERSHIP_REPORT || icmp6_header->icmp6_type == ICMP6_MEMBERSHIP_REDUCTION) {
            // This is an MLD packet, handle it accordingly
            struct in6_addr *mcast_addr = (struct in6_addr *)(packet + SIZE_ETHERNET + sizeof(struct ip) + sizeof(struct icmp6_hdr) + 4);

            // Get destination IP
            data.dst_ip[0] = 6;
            memcpy(data.dst_ip + 1, mcast_addr, 16);
        } else {
            // Get destination IP
            data.dst_ip[0] = 6;
            memcpy(data.dst_ip + 1, &ip_6_header->ip6_dst, 16);
        }

        // Determine protocol
        int protocol = ip_6_header->ip6_nxt;
        const u_char *payload = packet + sizeof(struct ip6_hdr);

        // Skip over extension headers, if any
        while (protocol != IPPROTO_TCP && protocol != IPPROTO_UDP && protocol != IPPROTO_ICMPV6) {
            switch (protocol) {
                case IPPROTO_HOPOPTS:
                case IPPROTO_ROUTING:
                case IPPROTO_FRAGMENT:
                case IPPROTO_DSTOPTS:
                    payload += (payload[1] + 1) * 8; // Extension header length is in units of 8 bytes
                    protocol = payload[0];
                    break;
                default:
                    // Unknown extension header, stop processing
                    data.payload = NULL;
                    data.payload_size = 0;
                    return;
            }
        }

        // Get payload
        if (protocol == IPPROTO_TCP) {
            struct tcphdr *tcp_header = (struct tcphdr *)(payload);
            data.payload = (u_int8_t *)(payload + tcp_header->th_off * 4);
            data.payload_size = ntohs(ip_6_header->ip6_plen) - (payload - (u_char*)ip_6_header) - (tcp_header->th_off * 4);
        } else if (protocol == IPPROTO_UDP) {
            struct udphdr *udp_header = (struct udphdr *)(payload);
            data.payload = (u_int8_t *)(payload + sizeof(struct udphdr));
            data.payload_size = ntohs(udp_header->uh_ulen) - sizeof(struct udphdr);
        } else if (protocol == IPPROTO_ICMPV6) {
            struct icmp6_hdr *icmp_header = (struct icmp6_hdr *)(payload);
            data.payload = (u_int8_t *)(payload + sizeof(struct icmp6_hdr));
            if (icmp_header->icmp6_type == ICMP6_ECHO_REQUEST || icmp_header->icmp6_type == ICMP6_ECHO_REPLY) {
                data.payload_size = ntohs(ip_6_header->ip6_plen) - (payload - (u_char*)ip_6_header) - sizeof(struct icmp6_hdr);
            } else {
                data.payload_size = ntohs(ip_6_header->ip6_plen) - (payload - (u_char*)ip_6_header) - sizeof(struct icmp6_hdr) - 8;
            }
        } else {
            // Unknown protocol
            data.payload = NULL;
            data.payload_size = 0;
        }




    } else if (ntohs(ethernet_header->ether_type) == ETHERTYPE_ARP) {

    }
        
    // Get transport layer header
    const u_char *transport_header = packet + SIZE_ETHERNET + (ip_header->ip_hl * 4);

    // Get source and destination ports
    if (ip_header->ip_p == IPPROTO_TCP) {
        const struct tcphdr *tcp_header = (const struct tcphdr *)transport_header;
        tcp_header.sou
        data.src_port = ntohs(tcp_header->th_sport);
        data.dst_port = ntohs(tcp_header->th_dport);
    } else if (ip_header->ip_p == IPPROTO_UDP) {
        const struct udphdr *udp_header = (const struct udphdr *)transport_header;
        data.src_port = ntohs(udp_header->uh_sport);
        data.dst_port = ntohs(udp_header->uh_dport);
    } else {
        data.src_port = 0;
        data.dst_port = 0;
    }

    print_packet_data(data, packet);
}

int main(int argc, char *argv[]) {
    string interface;
    string port;
    bool arp_flag = false;
    bool icmp4_flag = false;
    bool icmp6_flag = false;
    bool igmp_flag = false;
    bool mld_flag = false;
    bool ndp_flag = false;
    bool port_flag = false;
    bool protocol_t_flag = false;
    bool protocol_u_flag = false;
    int num_packets = 1;

    struct option long_options[] = {
        {"interface", required_argument, 0, 'i'},
        {"udp", no_argument, 0, 'u'},
        {"tcp", no_argument, 0, 't'},
        {"arp", no_argument, 0, 'a'},
        {"icmp4", no_argument, 0, '4'},
        {"icmp6", no_argument, 0, '6'},
        {"igmp", no_argument, 0, 'g'},
        {"mld", no_argument, 0, 'm'},
        {"ndp", no_argument, 0, 'd'},
        {0, 0, 0, 0}
    };

    // Parse command line arguments
    int opt;
    int long_index = 0;
    while ((opt = getopt_long(argc, argv, "i:p:tun:", long_options, &long_index)) != -1) {
        switch (opt) {
            case 'i':
                if (optarg != NULL) {
                    interface = optarg;
                }
                break;
            case 'p':
                port_flag = true;
                if (optarg != NULL) {
                    port = optarg;
                }
                break;
            case 't':
                protocol_t_flag = true;
                break;
            case 'u':
                protocol_u_flag = true;
                break;
            case 'n':
                if (optarg != NULL) {
                    num_packets = stoi(optarg);
                }
                break;
            case 'a':
                arp_flag = true;
                break;
            case '4':
                icmp4_flag = true;
                break;
            case '6':
                icmp6_flag = true;
                break;
            case 'g':
                igmp_flag = true;
                break;
            case 'm':
                mld_flag = true;
                break;
            case 'd':
                ndp_flag = true;
                break;
            default:
                if (optopt == 'i' || optopt == 'n') {
                    continue;
                }
                print_usage();
                return 1;
        }
    }

    if (interface.empty()) {
        pcap_if_t *alldevs;
        char errbuf[PCAP_ERRBUF_SIZE];

        // Get list of interfaces
        if (pcap_findalldevs(&alldevs, errbuf) == -1) {
            fprintf(stderr, "Error finding devices: %s\n", errbuf);
            return 1;
        }

        // Print interface list
        for (pcap_if_t *d = alldevs; d != NULL; d = d->next) {
            printf("  %s\n", d->name);
            if (d->description) {
                printf("    (%s)\n", d->description);
            }
        }

        // Free interface list
        pcap_freealldevs(alldevs);

        return 0;
    }

    // Check for required arguments
    if (port.empty() && port_flag) {
        cerr << "Argument for option -p is required.\n";
        print_usage();
        return 1;
    }

    // Determine filter string
    string filter_str;

    if (protocol_t_flag) {
        filter_str = "(tcp and ";
        if (port_flag) {
            filter_str += "port " + port + ")";
        } else {
            filter_str += "portrange 0-65535)";
        }
    }

    if (protocol_u_flag) {
        if (filter_str.empty()) {
            filter_str = "(udp and ";
        } else {
            filter_str += " or (udp and ";
        }
        if (port_flag) {
            filter_str += "port " + port + ")";
        } else {
            filter_str += "portrange 0-65535)";
        }
    }

    if (arp_flag) {
        if (!filter_str.empty()) filter_str += " or ";
        filter_str += "(arp)";
    }

    if (icmp4_flag) {
        if (!filter_str.empty()) filter_str += " or ";
        filter_str += "(icmp)";
    }

    if (icmp6_flag) {
        if (!filter_str.empty()) filter_str += " or ";
        filter_str += "(icmp6[icmp6type] == icmp6-echo or icmp6[icmp6type] == icmp6-echoreply)";
    }

    if (igmp_flag) {
        if (!filter_str.empty()) filter_str += " or ";
        filter_str += "(igmp)";
    }

    if (mld_flag) {
        if (!filter_str.empty()) filter_str += " or ";
        filter_str += "(icmp6[icmp6type] == icmp6-multicastlistenerquery or icmp6[icmp6type] == icmp6-multicastlistenerreportv1 or icmp6[icmp6type] == icmp6-multicastlistenerdone)";
    }

    if (ndp_flag) {
        if (!filter_str.empty()) filter_str += " or ";
        filter_str += "(icmp6[icmp6type] == icmp6-routersolicit or icmp6[icmp6type] == icmp6-routeradvert or icmp6[icmp6type] == icmp6-routerrenum or icmp6[icmp6type] == icmp6-neighborsolicit or icmp6[icmp6type] == icmp6-neighboradvert)";
    }

    cout << filter_str << endl;


    // Compile filter string
    bpf_program filter;
    pcap_t *handle;
    char error_buffer[PCAP_ERRBUF_SIZE];
    struct packet_data data_struct;

    handle = pcap_open_live(interface.c_str(), SNAP_LEN, 1, 1000, error_buffer);
    if (handle == NULL) {
        cerr << "Could not open device " << interface << ": " << error_buffer << endl;
        return 1;
    }

    if(pcap_datalink(handle) != DLT_EN10MB)
    {
        cerr << "Unsupported device" << endl;
        exit(2);
    }

    if (pcap_compile(handle, &filter, filter_str.c_str(), 1, PCAP_NETMASK_UNKNOWN) == -1) {
        cerr << "Could not compile filter string: " << pcap_geterr(handle) << endl;
        return 1;
    }

    if (pcap_setfilter(handle, &filter) == -1) {
        cerr << "Could not set filter: " << pcap_geterr(handle) << endl;
        return 1;
    }

    pcap_loop(handle, num_packets, process_packet, (u_char *)&data_struct);

    pcap_close(handle);

    return 0;
}