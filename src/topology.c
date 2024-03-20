//
// Created by Bagheri on 3/17/24.
//

#include "graph.h"

Graph *build_simple_topology() {

/*
                          +----------+
                      0/4 |          |0/0
         +----------------+   R0_re  +---------------------------+
         |     40.1.1.1/24| 122.1.1.0|20.1.1.1/24                |
         |                +----------+                           |
         |                                                       |
         |                                                       |
         |                                                       |
         |40.1.1.2/24                                            |20.1.1.2/24
         |0/5                                                    |0/1
     +---+---+                                              +----+-----+
     |       |0/3                                        0/2|          |
     | R2_re +----------------------------------------------+    R1_re |
     |       |30.1.1.2/24                        30.1.1.1/24|          |
     +-------+                                              +----------+

*/
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

    return topology;
}