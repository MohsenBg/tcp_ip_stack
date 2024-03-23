#include <layer2/layer2.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for strncmp
#include <graph.h>  // assuming get_interface_mac_address() is declared here

static bool layer2_interface_frame_acceptance_check(
        Interface *interface,
        EthernetHeader *ethernetHeader) {

    if (!interface) {
        fprintf(stderr, "Error: Interface pointer is NULL\n");
        return false;
    }

    if (!ethernetHeader) {
        fprintf(stderr, "Error: Ethernet header pointer is NULL\n");
        return false;
    }

    char *destination_mac_address = ethernetHeader->destination_mac_address.address;
    char *interface_mac_address = get_interface_mac_address(interface);

    if (!destination_mac_address || !interface_mac_address) {
        fprintf(stderr, "Error: MAC addresses not available\n");
        return false;
    }

    if (strncmp(destination_mac_address, interface_mac_address, MAC_ADDRESS_SIZE) != 0) {
        return false;
    }

    return true;
}

void layer2_interface_frame_receive(Node *node, Interface *interface, char *packet, size_t packet_size) {

}
