#include "communication.h"
#include "thread_management.h"
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/select.h>


static unsigned int port_number = 35000;
static char send_buffer[MAX_PACKET_BUFFER_SIZE];
static char receive_buffer[MAX_PACKET_BUFFER_SIZE];

// prototypes
int send_packet_upd_socket(int socket_fd, char buffer[2048], size_t packet_size, unsigned int port);

static void *thread_start_packet_receiver_thread(void *_topology);

static void deliver_packet_receive_to_interface(Node *receive_node, char packet[2048], size_t length);


// Definitions
unsigned int get_unused_port_number() {
    return port_number++;
}

void init_udp_socket(Node *node) {

    node->udp_port_number = get_unused_port_number();

    int udp_socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udp_socket_fd == -1) {
        perror("Error: socket creation failed");
        return;
    }

    struct sockaddr_in node_address;
    node_address.sin_family = AF_INET;
    node_address.sin_port = htons(node->udp_port_number);
    node_address.sin_addr.s_addr = INADDR_ANY;

    int bind_status = bind(udp_socket_fd, (struct sockaddr *) &node_address, sizeof(struct sockaddr));
    if (bind_status == -1) {
        fprintf(stderr, "Error: socket bind failed for Node %s\n", node->node_name);
        close(udp_socket_fd);
        return;
    }

    node->udp_socket_fd = udp_socket_fd;
}


// Receive
void network_start_packet_receiver_thread(Graph *topology) {
    pthread_t thread;
//    pthread_attr_t attr;
//    pthread_attr_init(&attr);
//    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&thread, NULL, thread_start_packet_receiver_thread, (void *) topology);
    add_thread(thread);
}

_Noreturn static void *thread_start_packet_receiver_thread(void *_topology) {
    Graph *topology = (Graph *) _topology;

    fd_set active_socket_fd_set, backup_socket_fd_set;
    FD_ZERO(&active_socket_fd_set);
    FD_ZERO(&backup_socket_fd_set);

    int socket_max_fd = 0;
    int bytes_received;
    unsigned int address_len = sizeof(struct sockaddr);
    struct sockaddr sender_address;

    // Populate the fd_set and determine max fd
    GList *current_node;
    for (current_node = topology->node_list; current_node != NULL; current_node = g_list_next(current_node)) {
        Node *node = (Node *) current_node->data;
        if (node->udp_socket_fd) {
            FD_SET(node->udp_socket_fd, &backup_socket_fd_set);
            socket_max_fd = (node->udp_socket_fd > socket_max_fd) ? node->udp_socket_fd : socket_max_fd;
        }
    }
    // Main event loop
    while (1) {
        // Reset active_socket_fd_set with backup_socket_fd_set
        memcpy(&active_socket_fd_set, &backup_socket_fd_set, sizeof(fd_set));

        // Wait for activity on sockets
        select(socket_max_fd + 1, &active_socket_fd_set, NULL, NULL, NULL);

        // Process active sockets
        for (current_node = topology->node_list; current_node != NULL; current_node = g_list_next(current_node)) {
            Node *node = (Node *) current_node->data;
            if (FD_ISSET(node->udp_socket_fd, &active_socket_fd_set)) {
                memset(receive_buffer, 0, MAX_PACKET_BUFFER_SIZE);
                bytes_received = recvfrom(node->udp_socket_fd, (char *) receive_buffer, MAX_PACKET_BUFFER_SIZE, 0,
                                          (struct sockaddr *) &sender_address, &address_len);
                deliver_packet_receive_to_interface(node, receive_buffer, bytes_received);
            }
        }
        pthread_testcancel();
    }
    pthread_exit(NULL);
}

static void
deliver_packet_receive_to_interface(Node *receive_node, char packet[MAX_PACKET_BUFFER_SIZE], size_t length) {
    char interface_name[INTERFACE_NAME_SIZE];
    strncpy(interface_name, packet, INTERFACE_NAME_SIZE);
    interface_name[INTERFACE_NAME_SIZE - 1] = '\0';
    size_t packet_data_size = length - INTERFACE_NAME_SIZE;
    char packet_data[packet_data_size];
    memcpy(packet_data, packet + INTERFACE_NAME_SIZE, packet_data_size);

    Interface *receive_interface = find_interface_by_name(receive_node, interface_name);
    if (!receive_interface) {
        fprintf(stderr, "Error: receive interface '%s' Invalid ", interface_name);
        return;
    }

    packet_receive(receive_node, receive_interface, packet_data, packet_data_size);
}

int packet_receive(Node *node, Interface *interface, char *packet, size_t length) {
    printf("packet data: %s \n"
           "Node : %s \n"
           "interface_name : %s \n",
           packet, node->node_name, interface->interface_name);
    return 0;
}

// Send
int send_packet_out(char *packet, unsigned int packet_size, Interface *interface) {
    Node *from = interface->device_node;
    Node *to = get_neighbor_node(interface);

    if (!from || !to)
        return -1;

    Interface *destination_interface = (interface == &interface->link->interface_A) ? &interface->link->interface_B
                                                                                    : &interface->link->interface_A;


    memset(send_buffer, 0, MAX_PACKET_BUFFER_SIZE);
    memcpy(send_buffer, destination_interface->interface_name, INTERFACE_NAME_SIZE - 1);
    memcpy(send_buffer + INTERFACE_NAME_SIZE, packet, packet_size);

    int socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_fd == -1) {
        perror("Error: Sending socket creation failed");
        return -1;
    }

    int rc = send_packet_upd_socket(socket_fd, send_buffer, packet_size + INTERFACE_NAME_SIZE, to->udp_port_number);
    close(socket_fd);
    return rc;
}

int send_packet_upd_socket(int socket_fd, char buffer[2048], size_t packet_size, unsigned int port) {
    struct sockaddr_in destination_address;
    struct hostent *host = gethostbyname("127.0.0.1");
    if (!host) {
        perror("Error: Failed to get host by name");
        return -1;
    }

    memset(&destination_address, 0, sizeof(destination_address));
    destination_address.sin_family = AF_INET;
    destination_address.sin_port = htons(port);
    destination_address.sin_addr = *((struct in_addr *) host->h_addr);

    int rc = sendto(socket_fd, buffer, packet_size, 0, (const struct sockaddr *) &destination_address,
                    sizeof(destination_address));
    if (rc == -1) {
        perror("Error: sendto failed");
    }
    return rc;
}


