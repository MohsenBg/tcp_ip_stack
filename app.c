#include "graph.h"
#include "cli.h"

extern Graph *build_simple_topology();

Graph *topology;

int main() {
    topology = build_simple_topology();
    start_tcp_ip_cli();

    //    dump_graph(topology);
    return 0;
}