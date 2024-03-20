//
// Created by Bagheri on 3/19/24.
//
#include "net.h"
#include "graph.h"
#include "stdio.h"

int generate_hash_code(const char *str, size_t size) {
    int hash = 0;

    for (int i = 0; i < size; i++) {
        hash = 31 * hash + str[i];
    }

    return hash;
}

bool node_set_loopback_address(Node *node, const char *ip_address) {
    if (!node || !ip_address) {
        fprintf(stderr, "Invalid arguments: node (%p), ip_address (%p)\n", node, ip_address);
        return false;
    }

    node->nodeNetworkProperty.is_loopback_address_config = true;

    strncpy(get_node_loopback_address(node), ip_address, IPV4_ADDRESS_SIZE - 1);
    get_node_loopback_address(node)[IPV4_ADDRESS_SIZE - 1] = '\0';
    return true;
}

bool node_unset_loopback_address(Node *node) {
    if (!node) {
        fprintf(stderr, "Invalid arguments: node (%p)\n", node);
        return false;
    }

    memset(get_node_loopback_address(node), 0, IPV4_ADDRESS_SIZE);
    get_node_loopback_address(node)[IPV4_ADDRESS_SIZE - 1] = '\0';

    node->nodeNetworkProperty.is_loopback_address_config = false;

    return true;
}

bool
node_set_interface_ip_address(Node *node, const char *local_interface, const char *ip_address, unsigned char mask) {
    if (!node || !ip_address) {
        fprintf(stderr, "Invalid arguments: node (%p), ip_address (%p)\n", node, ip_address);
        return false;
    }

    Interface *config_interface = find_interface_by_name(node, local_interface);
    if (!config_interface) {
        fprintf(stderr, "Interface '%s' not found\n", local_interface);
        return false;
    }

    config_interface->interfaceNetworkProperty.is_ip_address_config = true;

    strncpy(get_interface_ip_address(config_interface), ip_address, IPV4_ADDRESS_SIZE - 1);
    get_interface_ip_address(config_interface)[IPV4_ADDRESS_SIZE - 1] = '\0';
    config_interface->interfaceNetworkProperty.mask = mask;

    return true;
}

bool node_unset_interface_ip_address(Node *node, const char *local_interface) {
    if (!node || !local_interface) {
        fprintf(stderr, "Invalid arguments: node (%p), local_interface (%p)\n", node, local_interface);
        return false;
    }

    Interface *config_interface = find_interface_by_name(node, local_interface);
    if (!config_interface) {
        fprintf(stderr, "Interface '%s' not found\n", local_interface);
        return false;
    }


    memset(get_interface_ip_address(config_interface), 0, IPV4_ADDRESS_SIZE);
    get_interface_ip_address(config_interface)[IPV4_ADDRESS_SIZE - 1] = '\0';
    config_interface->interfaceNetworkProperty.mask = 0;

    config_interface->interfaceNetworkProperty.is_ip_address_config = false;

    return true;
}

void interface_assign_mac_address(Interface *interface) {
    Node *node = interface->device_node;

    if (!node)
        return;

    unsigned int hash_value = generate_hash_code(node->node_name, NODE_NAME_SIZE);
    hash_value *= generate_hash_code(interface->interface_name, INTERFACE_NAME_SIZE);

    memset(get_interface_mac_address(interface), 0, MAC_ADDRESS_SIZE);

    memcpy(get_interface_mac_address(interface), (char *) &hash_value, sizeof(unsigned int));
}