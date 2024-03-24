#ifndef TCPIP_ARP_H
#define TCPIP_ARP_H

#include <stdint.h>
#include "graph.h"
#include "network_constants.h"
#include <glib.h>

typedef struct ArpHeader {
    uint16_t hardware_type;
    uint16_t protocol_type;
    uint8_t hardware_address_length;
    uint8_t protocol_address_length;
    uint16_t operation_code;
    MacAddress source_mac_address;
    uint32_t source_ipv4_address;
    MacAddress destination_mac_address;
    uint32_t destination_ipv4_address;
} __attribute__((packed)) ArpHeader;

typedef struct ArpTable {
    GList *entries;
} ArpTable;

typedef struct ArpEntry {
    IPV4Address ip_address;
    MacAddress mac_address;
    char out_interface_name[INTERFACE_NAME_SIZE];
} ArpEntry;

// create arp table
void create_arp_table(ArpTable **arp_table);

// find arp entry with ip address
ArpEntry *arp_table_lookup(ArpTable *arp_table, char *ip_address);

// clear all entries in arp table
void clear_arp_table(ArpTable *arp_table);

void delete_arp_table_entry(ArpTable *arp_table, char *ip_address);

bool arp_table_entry_add(ArpTable *arp_table, ArpEntry *rp_entry);

void dump_arp_table(ArpTable *arp_table);

void arp_table_update_from_arp_reply(ArpTable *arp_table, ArpHeader *arp_reply, Interface *interface_in);

#endif //TCPIP_ARP_H
