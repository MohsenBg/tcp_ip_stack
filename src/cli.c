//
// Created by Bagheri on 3/20/24.
//
#include "CommandParser/libcli.h"
#include "CommandParser/cmdtlv.h"
#include "cli.h"

void start_tcp_ip_cli() {
    init_libcli();

    Param *show = libcli_get_show_hook();
    Param *debug = libcli_get_debug_hook();
    Param *config = libcli_get_config_hook();
    Param *clear = libcli_get_clear_hook();
    Param *run = libcli_get_run_hook();


    build_show_command(show);
    build_debug_command(debug);
    build_config_command(config);
    build_clear_command(clear);
    build_run_command(run);

    support_cmd_negation(config);

    start_shell();
}
