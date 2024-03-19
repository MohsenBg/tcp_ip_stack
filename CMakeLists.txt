cmake_minimum_required(VERSION 3.6.0)
project(TcpIp VERSION 1.0.0)

# Source Files
set(SOURCE_FILES
        src/graph.c
        src/topology.c
        app.c
        src/net.c
)

# Find GLib 2
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET glib-2.0)

# Executable
add_executable(tcp_ip ${SOURCE_FILES})
target_link_libraries(tcp_ip PkgConfig::GLIB)

# Include directories
target_include_directories(tcp_ip PUBLIC include)
