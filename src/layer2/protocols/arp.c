#include "layer2/protocols/arp.h"
#include "memory_manager.h"
#include <arpa/inet.h>
#include <stdio.h>


void create_arp_table(ArpTable **arp_table) {
    *arp_table = allocate_memory_with_calloc(1, sizeof(ArpTable));

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
            deallocate_memory(entry);

            break;
        }
    }
}


bool arp_table_entry_add(ArpTable *arp_table, ArpEntry *arp_entry) {
    if (arp_table == NULL || arp_entry == NULL) {
        fprintf(stderr, "Invalid ARP table or entry\n");
        return false;
    }

    ArpEntry *old_entry = arp_table_lookup(arp_table, arp_entry->ip_address.address);
    bool found_old_entry = old_entry != NULL;

    bool same_entry_address = found_old_entry && arp_entry == old_entry;
    if (same_entry_address)
        return true;

    bool same_entry_value = found_old_entry && memcmp(old_entry, arp_entry, sizeof(ArpEntry)) == 0;
    if (same_entry_value)
        return false;

    if (found_old_entry)
        delete_arp_table_entry(arp_table, old_entry->ip_address.address);

    arp_table->entries = g_list_append(arp_table->entries, arp_entry);

    add_glist_to_memory_manager(arp_table->entries);

    return true;
}


void dump_arp_table(ArpTable *arp_table) {
    if (arp_table == NULL) {
        fprintf(stderr, "Invalid ARP table or empty entries\n");
        return;
    }

    // Print table header
    printf(" IP Address     Mac Address          Interface Out\n");

    // Iterate over ARP table entries
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

    bool is_operation_arp_reply = arp_reply->operation_code == ARP_OPERATION_REPLY;
    if (!is_operation_arp_reply)
        return;

    ArpEntry *entry = (ArpEntry *) allocate_memory_with_calloc(1, sizeof(ArpEntry));
    if (entry == NULL) {
        fprintf(stderr, "Memory allocation failed for ARP entry\n");
        return;
    }

    uint32_t src_ip = htonl(arp_reply->source_ipv4_address);
    inet_ntop(AF_INET, &src_ip, entry->ip_address.address, IPV4_ADDRESS_STRING_SIZE);
    entry->ip_address.address[IPV4_ADDRESS_STRING_SIZE - 1] = '\0';

    memcpy(entry->mac_address.address, arp_reply->source_mac_address.address, MAC_ADDRESS_SIZE - 1);

    // Copy interface name
    strncpy(entry->out_interface_name, interface_in->interface_name, INTERFACE_NAME_SIZE - 1);
    entry->out_interface_name[INTERFACE_NAME_SIZE - 1] = '\0';

    bool is_entry_add_to_table = arp_table_entry_add(arp_table, entry);
    if (!is_entry_add_to_table)
        deallocate_memory(entry);
}






