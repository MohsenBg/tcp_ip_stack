//
// Created by Bagheri on 3/20/24.
//

#ifndef TCPIP_COMMAND_CODE_H
#define TCPIP_COMMAND_CODE_H

#include "CommandParser/cmdtlv.h"

typedef param_t Param;
typedef ser_buff_t SerializedBuffer;
typedef op_mode OpMode;
typedef tlv_struct_t TLV;

// Leaf id
#define LEAF_ID_SHOW_NODE "node_name"
#define LEAF_ID_SHOW_NODE_LOOPBACK_ADDRESS "node_loopback_address"

// Command code
#define SHOW_TOPOLOGY              1
#define SHOW_NODE                  2
#define SHOW_NODE_LOOPBACK_ADDRESS 3
#define SHOW_INTERFACE_IP_ADDRESS  4
#define SHOW_INTERFACE_MAC_ADDRESS 5

#endif //TCPIP_COMMAND_CODE_H
