#ifndef TCPIP_LAYER2_H
#define TCPIP_LAYER2_H

#include "net.h"

// Maximum Ethernet frame size including preamble, SFD, and CRC
#define MAX_ETHERNET_SIZE 1526
#define ETHERNET_PREAMBLE_SIZE 7
#define ETHERNET_SFD_SIZE 1
#define ETHERNET_TYPE_SIZE 2
#define ETHERNET_CRC_SIZE 4
#define ETHERNET_MAX_PAYLOAD_SIZE 1500  // Maximum payload size

typedef struct EthernetHeader EthernetHeader;

struct EthernetHeader {
    char preamble[ETHERNET_PREAMBLE_SIZE];
    char sfd[ETHERNET_SFD_SIZE];
    MacAddress destination_mac_address;
    MacAddress source_mac_address;
    char type[ETHERNET_TYPE_SIZE];
    char payload[ETHERNET_MAX_PAYLOAD_SIZE];
    char fcs[ETHERNET_CRC_SIZE];
} __attribute__((packed));


void layer2_interface_frame_receive(Node *node, Interface *interface, char *packet, size_t packet_size);

#endif //TCPIP_LAYER2_H
