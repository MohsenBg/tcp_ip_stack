//
// Created by Bagheri on 3/22/24.
//

#ifndef TCPIP_COMMUNICATION_H
#define TCPIP_COMMUNICATION_H

#include "graph.h"

#define  MAX_PACKET_BUFFER_SIZE 2048


void network_start_packet_receiver_thread(Graph *topology);

int packet_receive(Node *node, Interface *interface, char *packet, size_t length);

int send_packet_out(char *packet, unsigned int packet_size, Interface *interface);

void init_udp_socket(Node *node);

#endif //TCPIP_COMMUNICATION_H
