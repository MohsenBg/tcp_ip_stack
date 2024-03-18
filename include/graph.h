
#ifndef TCPIP_GRAPH_H
#define TCPIP_GRAPH_H

#include "glib.h"

#define NODE_NAME_SIZE 32
#define TOPOLOGY_NAME_SIZE 32
#define INTERFACE_NAME_SIZE 32
#define  MAX_INTERFACE_PER_NODE 10

typedef struct Graph Graph;
typedef struct Interface Interface;
typedef struct Node Node;
typedef struct Link Link;

struct Node {
    char node_name[NODE_NAME_SIZE];
    Interface *interfaces[MAX_INTERFACE_PER_NODE];
};

struct Graph {
    char topology_name[TOPOLOGY_NAME_SIZE];
    GList *node_list;
};


struct Interface {
    char interface_name[INTERFACE_NAME_SIZE];
    struct Node *device_node;
    struct Link *link;
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

static Interface *get_neighbor_node(const Interface *interface) {
    const Link *link = interface->link;

    if (&link->interface_A == interface) {
        return &link->interface_B;
    } else {
        return &link->interface_A;
    }
}

#endif //TCPIP_GRAPH_H
