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

/*
 * Function Declarations
 */

/*
 * Creates a new ARP table.
 *
 * Parameters:
 * - arp_table: Pointer to a pointer to the ARP table structure to be created.
 *
 * This function allocates memory for a new ARP table and assigns it to the specified pointer.
 * If memory allocation fails, an error message is printed to stderr.
 */
void create_arp_table(ArpTable **arp_table);

/*
 * Looks up an entry in the ARP table based on the specified IP address.
 *
 * Parameters:
 * - arp_table: Pointer to the ARP table structure.
 * - ip_address: Pointer to the string representing the IP address to look up.
 *
 * Returns:
 * - Pointer to the ARP entry if found.
 * - NULL if the ARP table is invalid or the entry is not found.
 *
 * This function searches the ARP table for an entry with the specified IP address.
 * If found, it returns a pointer to the corresponding ARP entry; otherwise, it returns NULL.
 */
ArpEntry *arp_table_lookup(ArpTable *arp_table, char *ip_address);

/*
 * Clears the ARP table by deallocating all entries.
 *
 * Parameters:
 * - arp_table: Pointer to the ARP table structure to be cleared.
 *
 * This function clears the ARP table by deallocating memory for all entries.
 * If the ARP table is invalid, an error message is printed to stderr.
 * After clearing, the ARP table's entries pointer is set to NULL.
 */
void clear_arp_table(ArpTable *arp_table);

/*
 * Deletes an entry from the ARP table based on the specified IP address.
 *
 * Parameters:
 * - arp_table: Pointer to the ARP table structure.
 * - ip_address: Pointer to the string representing the IP address of the entry to be deleted.
 *
 * This function removes the entry with the specified IP address from the ARP table.
 * If the ARP table is invalid or empty, an error message is printed to stderr.
 * Memory management for the entry is handled appropriately, ensuring proper cleanup.
 */
void delete_arp_table_entry(ArpTable *arp_table, char *ip_address);

/*
 * Adds an entry to the ARP table.
 *
 * Parameters:
 * - arp_table: Pointer to the ARP table structure.
 * - arp_entry: Pointer to the ARP entry structure to be added.
 *
 * Returns:
 * - true if the entry is successfully added to the ARP table.
 * - false if the ARP table or entry is invalid, or if the entry already exists in the table.
 *   If the entry is not added to the ARP table, you should deallocate it.
 *
 * This function adds the specified ARP entry to the ARP table if it is not already present.
 * If the entry already exists in the table, it is not added again, and false is returned.
 * If the entry is different from an existing entry in the table with the same IP address,
 * the existing entry is replaced with the new one.
 * Memory management for the ARP entry and ARP table is handled appropriately.
 */
bool arp_table_entry_add(ArpTable *arp_table, ArpEntry *arp_entry);

/*
 * Dumps the contents of the ARP table.
 *
 * Parameters:
 * - arp_table: Pointer to the ARP table structure to be dumped.
 *
 * This function prints the contents of the ARP table in a tabular format,
 * including IP addresses, MAC addresses, and outgoing interfaces.
 * If the ARP table is invalid, an appropriate error message is printed to stderr.
 */
void dump_arp_table(ArpTable *arp_table);

/*
 * Updates the ARP table with information obtained from an ARP reply.
 *
 * Parameters:
 * - arp_table: Pointer to the ARP table structure.
 * - arp_reply: Pointer to the ARP header structure containing the ARP reply.
 * - interface_in: Pointer to the interface structure through which the ARP reply was received.
 *
 * This function updates the ARP table with the IP-MAC mapping obtained from the ARP reply.
 * It checks if the operation code of the ARP reply is ARP_REPLY, allocates memory for a new
 * ARP entry, fills it with the necessary information from the ARP reply, and adds it to the ARP table.
 * If memory allocation fails or the ARP entry cannot be added to the table, appropriate error messages
 * are printed to stderr.
 */
void arp_table_update_from_arp_reply(ArpTable *arp_table, ArpHeader *arp_reply, Interface *interface_in);


#endif //TCPIP_ARP_H
