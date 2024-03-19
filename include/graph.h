
#ifndef TCPIP_GRAPH_H
#define TCPIP_GRAPH_H

#include "glib.h"
#include "net.h"
#include "common_type_def.h"

#define NODE_NAME_SIZE 32
#define TOPOLOGY_NAME_SIZE 32
#define INTERFACE_NAME_SIZE 32
#define  MAX_INTERFACE_PER_NODE 10


struct Node {
    char node_name[NODE_NAME_SIZE];
    Interface *interfaces[MAX_INTERFACE_PER_NODE];
    NodeNetworkProperty nodeNetworkProperty;
};

struct Graph {
    char topology_name[TOPOLOGY_NAME_SIZE];
    GList *node_list;
};


struct Interface {
    char interface_name[INTERFACE_NAME_SIZE];
    struct Node *device_node;
    struct Link *link;
    InterfaceNetworkProperty interfaceNetworkProperty;
};

struct Link {
    Interface interface_A;
    Interface interface_B;
    unsigned int cost;
};

Graph *create_new_graph(const char *topology_name);

Node *create_graph_node(Graph *graph, const char *node_name);

void insert_link_between_two_nodes(Node *node_A, Node *node_B, char *interface_name_A,
                                   char *interface_name_B, unsigned int cost);

void dump_graph(const Graph *graph);

// Static Methode
static int get_empty_interface_node_slot(const Node *node) {
    for (int i = 0; i < MAX_INTERFACE_PER_NODE; i++) {
        if (node->interfaces[i] == NULL) {
            return i;
        }
    }
    return -1;
}

static inline Interface *get_neighbor_node(const Interface *interface) {
    Link *link = interface->link;

    if (&link->interface_A == interface) {
        return &link->interface_B;
    } else {
        return &link->interface_A;
    }
}

static inline Interface *find_interface_by_name(const Node *node, const char *interface_name) {

    if (!node)
        return NULL;


    for (int i = 0; i < MAX_INTERFACE_PER_NODE; ++i) {
        const Interface *current_interface = node->interfaces[i];
        if (current_interface && strcmp(current_interface->interface_name, interface_name) == 0) {
            return (Interface *) current_interface;
        }
    }

    return NULL;
}


static inline char *get_node_loopback_address(Node *node) {
    return node->nodeNetworkProperty.ip_address_loopback.address;
}

static inline char *get_interface_mac_address(Interface *interface) {
    return interface->interfaceNetworkProperty.mac_address.address;
}

static inline char *get_interface_ip_address(Interface *interface) {
    return interface->interfaceNetworkProperty.ip_address.address;
}


#endif //TCPIP_GRAPH_H
