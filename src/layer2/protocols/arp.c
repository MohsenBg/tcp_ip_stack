#include "layer2/protocols/arp.h"
#include "memory_manager.h"
#include <stdlib.h>
#include <stdio.h>


void create_arp_table(ArpTable **arp_table) {
    // Allocate memory for ARP table
    *arp_table = allocate_memory_with_calloc(1, sizeof(ArpTable));

    // Check if memory allocation is successful
    if (*arp_table == NULL) {
        fprintf(stderr, "Failed to allocate memory for ARP table\n");
        return;
    }
}


ArpEntry *arp_table_lookup(ArpTable *arp_table, char *ip_address) {
    if (arp_table == NULL) {
        fprintf(stderr, "Invalid pointer to ARP table\n");
        return NULL;
    }

    for (GList *current_entry = arp_table->entries; current_entry != NULL; current_entry = g_list_next(current_entry)) {
        ArpEntry *entry = (ArpEntry *) current_entry->data;
        if (strncmp(entry->ip_address.address, ip_address, IPV4_ADDRESS_STRING_SIZE) == 0)
            return entry;
    }

    return NULL;
}

void clear_arp_table(ArpTable *arp_table) {
    if (arp_table == NULL) {
        fprintf(stderr, "Invalid pointer to ARP table\n");
        return;
    }
    deallocate_glist(arp_table->entries);
    arp_table->entries = NULL;
}

void delete_arp_table_entry(ArpTable *arp_table, char *ip_address) {
    if (arp_table == NULL) {
        fprintf(stderr, "Invalid ARP table or empty entries\n");
        return;
    }

    for (GList *current_entry = arp_table->entries; current_entry != NULL; current_entry = g_list_next(current_entry)) {
        ArpEntry *entry = (ArpEntry *) current_entry->data;
        if (strncmp(entry->ip_address.address, ip_address, IPV4_ADDRESS_STRING_SIZE) == 0) {
            arp_table->entries = g_list_remove(arp_table->entries, entry);
            break;
        }
    }
}

bool arp_table_entry_add(ArpTable *arp_table, ArpEntry *arp_entry) {
    if (arp_table == NULL || arp_entry == NULL) {
        fprintf(stderr, "Invalid ARP table or entry\n");
        return false;
    }

    arp_table->entries = g_list_append(arp_table->entries, arp_entry);
    add_glist_to_memory_manager(arp_table->entries);
    return true;
}

void dump_arp_table(ArpTable *arp_table) {
    if (arp_table == NULL) {
        fprintf(stderr, "Invalid ARP table or empty entries\n");
        return;
    }


    printf(" IP Address     Mac Address          Interface Out\n");
    for (GList *current_entry = arp_table->entries; current_entry != NULL; current_entry = g_list_next(current_entry)) {
        ArpEntry *entry = (ArpEntry *) current_entry->data;
        printf("==================================================\n");
        // Format the MAC address
        char formatted_mac[18];
        snprintf(formatted_mac, sizeof(formatted_mac), "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
                 (unsigned char) entry->mac_address.address[0], (unsigned char) entry->mac_address.address[1],
                 (unsigned char) entry->mac_address.address[2], (unsigned char) entry->mac_address.address[3],
                 (unsigned char) entry->mac_address.address[4], (unsigned char) entry->mac_address.address[5]);

        // Print ARP table entry
        printf(" %-14s %-20s %s\n", entry->ip_address.address, formatted_mac, entry->out_interface_name);
    }
}


void arp_table_update_from_arp_reply(ArpTable *arp_table, ArpHeader *arp_reply, Interface *interface_in) {
    if (arp_table == NULL || arp_reply == NULL || interface_in == NULL) {
        fprintf(stderr, "Invalid ARP table, ARP reply, or interface\n");
        return;
    }

    char ipv4[IPV4_ADDRESS_STRING_SIZE];
    convert_uint32_to_ipv4(arp_reply->source_ipv4_address, ipv4);

    ArpEntry *entry = arp_table_lookup(arp_table, ipv4);

    if (entry == NULL) {
        entry = (ArpEntry *) allocate_memory_with_calloc(1, sizeof(ArpEntry));
        if (entry == NULL) {
            fprintf(stderr, "Memory allocation failed for ARP entry\n");
            return;
        }

        // Convert source IPv4 address to string
        convert_uint32_to_ipv4(arp_reply->source_ipv4_address, entry->ip_address.address);

        // Add entry to ARP table
        arp_table_entry_add(arp_table, entry);
    }

    // Copy MAC address
    memcpy(entry->mac_address.address, arp_reply->source_mac_address.address, MAC_ADDRESS_SIZE - 1);
    entry->mac_address.address[MAC_ADDRESS_SIZE - 1] = '\0';

    // Copy interface name
    strncpy(entry->out_interface_name, interface_in->interface_name, INTERFACE_NAME_SIZE - 1);
    entry->out_interface_name[INTERFACE_NAME_SIZE - 1] = '\0';
}





