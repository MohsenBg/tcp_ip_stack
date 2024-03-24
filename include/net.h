#ifndef TCPIP_NET_H
#define TCPIP_NET_H

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <memory.h>
#include <stdint.h>
#include "common_type_def.h"
#include "network_constants.h"

#define MAC_ADDRESS_SIZE 6

typedef struct IPV4Address {
    char address[IPV4_ADDRESS_STRING_SIZE];
} IPV4Address;

typedef struct MacAddress {
    char address[MAC_ADDRESS_SIZE];
} MacAddress;

typedef struct NodeNetworkProperty {
    bool is_loopback_address_config;
    IPV4Address ip_address_loopback;
} NodeNetworkProperty;

static inline void create_node_network_property(NodeNetworkProperty *nodeNetworkProperty) {
    nodeNetworkProperty->is_loopback_address_config = false;
    memset(nodeNetworkProperty->ip_address_loopback.address, 0, 16);
}

typedef struct InterfaceNetworkProperty {
    // L2
    MacAddress mac_address;

    // L3
    bool is_ip_address_config;
    IPV4Address ip_address;
    unsigned char mask;
} InterfaceNetworkProperty;


static inline void create_interface_network_property(InterfaceNetworkProperty *interfaceNetworkProperty) {
    memset(interfaceNetworkProperty->mac_address.address, 0, MAC_ADDRESS_SIZE);
    interfaceNetworkProperty->is_ip_address_config = false;
    memset(interfaceNetworkProperty->ip_address.address, 0, IPV4_ADDRESS_STRING_SIZE);
    interfaceNetworkProperty->mask = 0;
}

void interface_assign_mac_address(Interface *interface);

bool node_set_loopback_address(Node *node, const char *ip_address);

bool node_unset_loopback_address(Node *node);

bool
node_set_interface_ip_address(Node *node, const char *local_interface, const char *ip_address, unsigned char mask);

bool node_unset_interface_ip_address(Node *node, const char *local_interface);

char *packer_buffer_shift_right(char *packet, size_t packet_size, size_t total_buffer);

char *convert_uint32_to_ipv4(uint32_t ip_address, char *ipv4_str);

#endif //TCPIP_NET_H
