#include "graph.h"

extern Graph *build_simple_topology();

int main() {
    Graph *topology = build_simple_topology();
    dump_graph(topology);
    return 0;
}