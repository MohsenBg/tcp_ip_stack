#include "graph.h"
#include "cli.h"
#include <unistd.h>
#include "communication.h"

extern Graph *build_simple_topology();

Graph *topology;

int main() {
    topology = build_simple_topology();
    sleep(2);

    Node *sender_node = find_node_by_name(topology, "R0_router");
    Interface *out_interface = find_interface_by_name(sender_node, "eth0/0");

    char msg[] = "This Message For Testing Socket Between To Node!\0";

    //    dump_graph(topology);
    send_packet_out(msg, strlen(msg), out_interface);
    start_tcp_ip_cli();

    return 0;
}