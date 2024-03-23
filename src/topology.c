//
// Created by Bagheri on 3/17/24.
//

#include "graph.h"
#include "memory_manager.h"

extern void network_start_packet_receiver_thread(Graph *);

Graph *build_simple_topology() {
#if 0
    /*
                               +--------------+
                           0/4 |              |0/0
            +------------------+   R0_router  +----------------------+
            |       40.1.1.1/24|   122.1.1.0  |20.1.1.1/24           |
            |                  |              |                      |
            |                  +--------------+                      |
            |                                                        |
            |                                                        |
            |                                                        |
            |40.1.1.2/24                                             |20.1.1.2/24
            |0/5                                                     |0/1
      +-----+-----+                                           +------+------+
      |           |0/3                                     0/2|             |
      | R2_router +-------------------------------------------+  R1_router  |
      | 122.1.1.2 |30.1.1.2/24                     30.1.1.1/24|  122.1.1.1  |
      |           |                                           |             |
      +-----------+                                           +-------------+

    */

#endif

    // Graph
    Graph *topology = create_new_graph("simple Graph");

    // Nodes
    Node *R0_router = create_graph_node(topology, "R0_router");
    Node *R1_router = create_graph_node(topology, "R1_router");
    Node *R2_router = create_graph_node(topology, "R2_router");

    //Links
    insert_link_between_two_nodes(R0_router, R1_router,
                                  "eth0/0", "eth0/1", 1);

    insert_link_between_two_nodes(R0_router, R2_router,
                                  "eth0/4", "eth0/5", 1);

    insert_link_between_two_nodes(R1_router, R2_router,
                                  "eth0/2", "eth0/3", 1);


    // R0_router config
    node_set_loopback_address(R0_router, "122.1.1.0");
    node_set_interface_ip_address(R0_router, "eth0/0", "20.1.1.1", 24);
    node_set_interface_ip_address(R0_router, "eth0/4", "40.1.1.1", 24);

    // R1_router config

    node_set_loopback_address(R1_router, "122.1.1.1");
    node_set_interface_ip_address(R1_router, "eth0/1", "20.1.1.2", 24);
    node_set_interface_ip_address(R1_router, "eth0/2", "30.1.1.1", 24);

    // R2_router config
    node_set_loopback_address(R2_router, "122.1.1.2");
    node_set_interface_ip_address(R2_router, "eth0/5", "40.1.1.2", 24);
    node_set_interface_ip_address(R2_router, "eth0/3", "30.1.1.2", 24);

    network_start_packet_receiver_thread(topology);
    return topology;
}


Graph *build_liner_topology() {

#if 0
    /*
    +-----------+                              +-----------+                                  +-----------+
    |           |                              |           |                                  |           |
    |    R1     |eth0/1                  eth0/2|    R2     |eth0/3                      eth0/4|    R3     |
    | 122.1.1.1 +------------------------------+ 122.1.1.2 +----------------------------------+ 122.1.1.3 |
    |           |10.1.1.1/24        10.1.1.2/24|           |11.1.1.2/24            11.1.1.1/24|           |
    +-----------+                              +-----------+                                  +-----------+

    */
#endif

    Graph *topology = create_new_graph("liner_topology");

    // Create node
    Node *R1 = create_graph_node(topology, "R1");
    Node *R2 = create_graph_node(topology, "R2");
    Node *R3 = create_graph_node(topology, "R3");

    // Link Node
    insert_link_between_two_nodes(R1, R2, "eth0/1", "eth0/2", 1);
    insert_link_between_two_nodes(R2, R3, "eth0/3", "eth0/4", 1);

    // Config Node R1
    node_set_loopback_address(R1, "122.1.1.1");
    node_set_interface_ip_address(R1, "eth0/1", "10.1.1.1", 24);

    // Config Node R2
    node_set_loopback_address(R2, "122.1.1.2");
    node_set_interface_ip_address(R2, "eth0/2", "10.1.1.2", 24);
    node_set_interface_ip_address(R2, "eth0/3", "11.1.1.2", 24);

    // Config Node R3
    node_set_loopback_address(R3, "122.1.1.3");
    node_set_interface_ip_address(R3, "eth0/4", "11.1.1.1", 24);
}


void destroy_topology(Graph *topology) {
    deallocate_all_memory();
    g_list_free(topology->node_list); // Free the GList itself

}

