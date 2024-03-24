#include "graph.h"
#include "cli.h"
#include <unistd.h>
#include "thread_management.h"
#include "layer2/protocols/arp.h"

extern Graph *build_simple_topology();

extern void initialize_memory_manager();

extern void destroy_memory_manager();

Graph *topology;

ArpEntry create_arp_entry(char *ipv4, unsigned int mac_address, char *interface_name) {
    ArpEntry entry;
    strncpy(entry.ip_address.address, ipv4, IPV4_ADDRESS_STRING_SIZE);
    memcpy(entry.mac_address.address, (char *) &mac_address, MAC_ADDRESS_SIZE);
    strcpy(entry.out_interface_name, interface_name);
    return entry;
}

int main() {
    initialize_memory_manager();

//    topology = build_simple_topology();
//    sleep(2);
//    dump_graph(topology);
//
//    Node *sender_node = find_node_by_name(topology, "R0_router");
//    Interface *out_interface = find_interface_by_name(sender_node, "eth0/0");
//
//    char msg[] = "This Message For Testing Socket Between To Node!\0";
//
//    dump_graph(topology);
//    send_packet_out(msg, strlen(msg), out_interface);
//    start_tcp_ip_cli();

//    cancel_all_threads();
//    wait_for_all_threads();


    ArpTable *arp_table = NULL;
    create_arp_table(&arp_table);


    ArpEntry entry_a = create_arp_entry("192.168.0.1", 2147483647, "eth1/1");
    ArpEntry entry_b = create_arp_entry("172.16.0.10", 987654321, "eth2/2");
    ArpEntry entry_c = create_arp_entry("10.0.0.100", 1234567890, "eth3/3");
    ArpEntry entry_d = create_arp_entry("192.168.1.1", 715255425, "eth4/4");
    ArpEntry entry_e = create_arp_entry("172.31.10.5", 496979491, "eth5/5");

    arp_table_entry_add(arp_table, &entry_a);
    arp_table_entry_add(arp_table, &entry_b);
    arp_table_entry_add(arp_table, &entry_c);
    arp_table_entry_add(arp_table, &entry_d);
    arp_table_entry_add(arp_table, &entry_e);

    dump_arp_table(arp_table);

    delete_arp_table_entry(arp_table, "10.0.0.100");

    printf("\n\n\n");
    dump_arp_table(arp_table);

    ArpHeader arp_header = {
            .hardware_type = 1,  // Sample hardware type
            .protocol_type = 0x0800,  // Sample protocol type (IPv4)
            .hardware_address_length = 6,  // Assuming MAC addresses are 6 bytes
            .protocol_address_length = 4,  // Assuming IPv4 addresses are 4 bytes
            .operation_code = 1,  // Sample operation code (request)
            .source_mac_address = {{0x00, 0x11, 0x22, 0x33, 0x44, 0x55}},  // Sample source MAC address
            .source_ipv4_address = 0xC0A80101,  // Sample source IPv4 address (192.168.1.1)
            .destination_mac_address = {
                    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}},  // Sample destination MAC address (broadcast)
            .destination_ipv4_address = 0xC0A80102,  // Sample destination IPv4 address (192.168.1.2)
    };

    Interface interface;
    strcpy(interface.interface_name, "eth0/0");
    arp_table_update_from_arp_reply(arp_table, &arp_header, &interface);

    printf("\n\n\n");
    dump_arp_table(arp_table);

    clear_arp_table(arp_table);

    printf("\n\n\n");
    dump_arp_table(arp_table);

    destroy_memory_manager();
}