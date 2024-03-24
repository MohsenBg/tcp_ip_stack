#include "glib.h"
#include "graph.h"
#include "string.h"
#include <stdio.h>
#include "communication.h"
#include "memory_manager.h"

Graph *create_new_graph(const char *topology_name) {
    Graph *graph = (Graph *) allocate_memory_with_calloc(1, sizeof(Graph));

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

    Node *node = (Node *) allocate_memory_with_calloc(1, sizeof(Node));

    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed for Node\n");
        return NULL;
    }

    create_node_network_property(&node->nodeNetworkProperty);

    for (int i = 0; i < MAX_INTERFACE_PER_NODE; ++i) {
        node->interfaces[i] = NULL;
    }

    strncpy(node->node_name, node_name, NODE_NAME_SIZE - 1);
    node->node_name[NODE_NAME_SIZE - 1] = '\0';
    init_udp_socket(node);
    graph->node_list = g_list_append(graph->node_list, node);
    
    // Add the head of the graph's node list to the memory manager.
    // This function handles duplicates automatically.
    add_glist_to_memory_manager(graph->node_list);

    return node;
}

void insert_link_between_two_nodes(
        Node *node_A,
        Node *node_B,
        char *interface_name_A,
        char *interface_name_B,
        unsigned int cost) {

    Link *link = (Link *) allocate_memory_with_calloc(1, sizeof(Link));

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

    create_interface_network_property(&node_A->interfaces[index_interfaces_node_A]->interfaceNetworkProperty);
    create_interface_network_property(&node_B->interfaces[index_interfaces_node_B]->interfaceNetworkProperty);

    interface_assign_mac_address(&link->interface_A);
    interface_assign_mac_address(&link->interface_B);

}


void dump_interface(Interface *interface) {
    if (interface == NULL) {
//        printf("Error: Interface is NULL\n");
        return;
    }

    printf("  Local Node: %s, Interface Name: %s, Neighbor Node: %s, Cost: %u\n",
           interface->device_node->node_name,
           interface->interface_name,
           interface->link ? get_neighbor_node(interface)->node_name : "(not connected)",
           interface->link ? interface->link->cost : 0);

    char *ipv4 = get_interface_ip_address(interface);
    char *mac = get_interface_mac_address(interface);
    unsigned char mask = interface->interfaceNetworkProperty.mask;
    printf("\t IPV4 : %s/%u", ipv4 ? ipv4 : "(None)", mask);
    printf("\t MAC : %02x:%02x:%02x:%02x:%02x:%02x\n",
           mac[0], mac[1],
           mac[2], mac[3],
           mac[4], mac[5]);
}

void dump_node(Node *node) {
    if (node == NULL) {
        printf("Error: Node is NULL\n");
        return;
    }

    printf("Node Name: %s\n", node->node_name);

    bool is_ip_config = node->nodeNetworkProperty.is_loopback_address_config;

    printf("LoopBack Address: %s\n", is_ip_config ? get_node_loopback_address(node) : "(None)");
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

    printf("Topology Name: %s\n", graph->topology_name);

    for (GList *node_list = graph->node_list; node_list != NULL; node_list = g_list_next(node_list)) {
        Node *node = node_list->data;
        dump_node(node);
    }
}

Node *find_node_by_name(Graph *graph, char *node_name) {
    if (graph == NULL) {
        printf("Error: Graph is NULL\n");
        return NULL;
    }

    for (GList *iter = graph->node_list; iter != NULL; iter = g_list_next(iter)) {
        Node *current_node = (Node *) iter->data;
        if (strcmp(current_node->node_name, node_name) == 0)
            return current_node;
    }

    return NULL;
}




