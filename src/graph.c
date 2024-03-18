#include "glib.h"
#include "graph.h"
#include "string.h"
#include <stdio.h>

Graph *create_new_graph(const char *topology_name) {
    Graph *graph = calloc(1, sizeof(Graph));
    if (graph == NULL) {
        fprintf(stderr, "Memory allocation failed for Graph\n");
        return NULL;
    }
    strncpy(graph->topology_name, topology_name, TOPOLOGY_NAME_SIZE - 1);
    graph->topology_name[TOPOLOGY_NAME_SIZE - 1] = '\0';
    return graph;
}

Node *create_graph_node(Graph *graph, const char *node_name) {
    if (graph == NULL) {
        fprintf(stderr, "Invalid graph pointer\n");
        return NULL;
    }
    Node *node = calloc(1, sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed for Node\n");
        return NULL;
    }

    for (int i = 0; i < MAX_INTERFACE_PER_NODE; ++i) {
        node->interfaces[i] = NULL;
    }

    strncpy(node->node_name, node_name, NODE_NAME_SIZE - 1);
    node->node_name[NODE_NAME_SIZE - 1] = '\0';
    graph->node_list = g_list_append(graph->node_list, node);
    return node;
}


void insert_link_between_two_nodes(
        Node *node_A,
        Node *node_B,
        char *interface_name_A,
        char *interface_name_B,
        unsigned int cost) {

    Link *link = calloc(1, sizeof(Link));

    strncpy(link->interface_A.interface_name, interface_name_A, INTERFACE_NAME_SIZE);
    link->interface_A.interface_name[INTERFACE_NAME_SIZE - 1] = '\0';

    strncpy(link->interface_B.interface_name, interface_name_B, INTERFACE_NAME_SIZE);
    link->interface_B.interface_name[INTERFACE_NAME_SIZE - 1] = '\0';

    link->interface_A.link = link;
    link->interface_B.link = link;

    link->interface_A.device_node = node_A;
    link->interface_B.device_node = node_B;

    link->cost = cost;

    int index_interfaces_node_A = get_empty_interface_node_slot(node_A);
    int index_interfaces_node_B = get_empty_interface_node_slot(node_B);


    node_A->interfaces[index_interfaces_node_A] = &link->interface_A;
    node_B->interfaces[index_interfaces_node_B] = &link->interface_B;
};


void dump_interface(const Interface *interface) {
    if (interface == NULL) {
//        printf("Error: Interface is NULL\n");
        return;
    }

    printf("  Local Node: %s, Interface Name: %s, Neighbor Node: %s, Cost: %u\n",
           interface->device_node->node_name,
           interface->interface_name,
           interface->link ? get_neighbor_node(interface)->device_node : "(not connected)",
           interface->link ? interface->link->cost : 0);
}

void dump_node(const Node *node) {
    if (node == NULL) {
        printf("Error: Node is NULL\n");
        return;
    }

    printf("Node Name: %s\n", node->node_name);

    for (int i = 0; i < MAX_INTERFACE_PER_NODE; i++) {
        dump_interface(node->interfaces[i]);
    }

    printf("\n");
}

void dump_graph(const Graph *graph) {
    if (graph == NULL) {
        printf("Error: Graph is NULL\n");
        return;
    }

    printf("Topology Name: %s\n",
           graph->topology_name ? graph->topology_name : "(not set)");

    for (GList *node_list = graph->node_list; node_list != NULL; node_list = g_list_next(node_list)) {
        Node *node = node_list->data;
        dump_node(node);
    }
}
