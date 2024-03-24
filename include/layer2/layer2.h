#ifndef TCPIP_LAYER2_H
#define TCPIP_LAYER2_H

#include <stdint.h>
#include "net.h"
#include "network_constants.h"

typedef struct EthernetHeader {
    MacAddress destination_mac_address;
    MacAddress source_mac_address;
    uint16_t type;
    char payload[ETHERNET_MAX_PAYLOAD_SIZE];
    uint16_t fcs;
} __attribute__((packed)) EthernetHeader;

void layer2_interface_frame_receive(Node *node, Interface *interface, char *packet, size_t packet_size);

#endif // TCPIP_LAYER2_H
