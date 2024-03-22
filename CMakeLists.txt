cmake_minimum_required(VERSION 3.2.0)
project(tcpip VERSION 1.0.0)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

# Set source files
set(SOURCE_FILES
        app.c
        src/graph.c
        src/topology.c
        src/net.c
        src/communication.c
        src/cli/show_command.c
        src/cli/run_command.c
        src/cli/config_command.c
        src/cli/debug_command.c
        src/cli/clear_command.c
        src/cli.c
)

# Add subdirectory for command parser library
add_subdirectory(lib/CommandParser)

# Find GLib
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET glib-2.0)

# Executable
add_executable(tcp_ip ${SOURCE_FILES})
target_link_libraries(tcp_ip PRIVATE PkgConfig::GLIB CommandParser)

# Include directories
target_include_directories(tcp_ip PUBLIC include)

# Check the operating system
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(tcp_ip PRIVATE Threads::Threads)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # Mac-specific settings can be added here if needed
else ()
    # Windows - you may use a different threading library, like Windows API
    message("Pthread not supported on your operating system")
endif ()
