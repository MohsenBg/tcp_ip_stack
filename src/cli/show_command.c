//
// Created by Bagheri on 3/20/24.
//

#include "cli/show_command.h"
#include "CommandParser/libcli.h"
#include "CommandParser/cmdtlv.h"
#include "graph.h"

extern Graph *topology;

// show topology
int topology_callback_handler(
        __attribute__((unused))  Param *param,
        __attribute__((unused)) SerializedBuffer *buffer,
        __attribute__((unused)) OpMode enable_or_disable) {
    dump_graph(topology);
    return 0;
}

// show node <node-name>
int node_name_callback_handler(
        __attribute__((unused)) Param *param,
        SerializedBuffer *buffer,
        __attribute__((unused)) OpMode enable_or_disable) {

    TLV *tlv = NULL;

    char *node_name;
    TLV_LOOP_BEGIN(buffer, tlv) {
            if (strncmp(tlv->leaf_id, LEAF_ID_SHOW_NODE, strlen(LEAF_ID_SHOW_NODE)) == 0) {
                node_name = tlv->value;
            }
        }
    TLV_LOOP_END


    Node *node = find_node_by_name(topology, node_name);
    dump_node(node);
    return 0;
}

int validate_node_name(char *value) {
    Node *node = find_node_by_name(topology, value);
    if (!node)
        return VALIDATION_FAILED;
    return VALIDATION_SUCCESS;
}


void build_show_command(Param *show) {
/*
init_param

Get the param_t variable, either a static memory or heap memory, not stack memory

Args:
    Address of the current param
    CMD for command param, LEAF for leaf param
    Name of the param, this string is what is displayed in command line
    callback : pointer to application function. Null in this case since 'show node' is not a complete command
    Applicable only for LEAF params. Always NULL for CMD param
    Always INVALID for CMD params
    Always NULL for CMD params
    Help String

 */


    {
        // show topology
        static Param _topology;

        init_param(
                &_topology,
                CMD,
                "topology",
                topology_callback_handler,
                NULL,
                INVALID,
                NULL,
                "Help : show topology"
        );

        libcli_register_param(show, &_topology);
    }


    {
        // show node
        static Param node;

        init_param(
                &node,
                CMD,
                "node",
                0,
                NULL,
                INVALID,
                NULL,
                "Help : show node"
        );

        libcli_register_param(show, &node);


        {
            static Param node_name;

            // show node <node-name>
            init_param(
                    &node_name,
                    LEAF,
                    "node",
                    node_name_callback_handler,
                    validate_node_name,
                    STRING,
                    LEAF_ID_SHOW_NODE,
                    "Help : show node"
            );

            libcli_register_param(&node, &node_name);
        }


    }
}

