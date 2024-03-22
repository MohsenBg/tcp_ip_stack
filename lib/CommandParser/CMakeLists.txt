cmake_minimum_required(VERSION 3.2.0)

# Source Files
set(SOURCE_FILES
        ./src/gluethread/glthread.c
        ./src/clicbext.c
        ./src/clistd.c
        ./src/cmd_hier.c
        ./src/parser.c
        ./src/string_util.c
        ./src/serialize.c
        ./src/ut/utinfra/ut_parser.c
)

# Library
add_library(CommandParser ${SOURCE_FILES})
target_include_directories(CommandParser PUBLIC include)

# Executables
add_executable(main1 main1.c)
target_link_libraries(main1 CommandParser)

add_executable(main2 main2.c)
target_link_libraries(main2 CommandParser)

add_executable(main3 main3.c)
target_link_libraries(main3 CommandParser)

add_executable(main4 main4.c)
target_link_libraries(main4 CommandParser)

add_executable(testapp testapp.c)
target_link_libraries(testapp CommandParser)
