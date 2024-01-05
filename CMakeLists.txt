# Operating Systems II - UNC FCEFyN 2023


# First, we need to specify the CMake version
cmake_minimum_required(VERSION 3.15.0)

# Then, we setup our project
project(
    Client-Server
    VERSION 1.0.0
    DESCRIPTION "A Basic Server-Client Arquitechture using IPC mechanisms"
    LANGUAGES C
)

# Set the C version
set(CMAKE_C_STANDARD 11)

add_compile_options(-Wall -Werror -pedantic -Wextra -Wconversion)

add_definitions(${GCC_COVERAGE_LINK_FLAGS})

add_link_options("-lrt")

# We create our executable
add_executable(server src/server.c)
add_executable(client1 src/client1.c)
add_executable(client2 src/client2.c)
add_executable(client3 src/client3.c)

target_link_libraries(server "-lrt")
target_link_libraries(client1 "-lrt")
target_link_libraries(client2 "-lrt")
target_link_libraries(client3 "-lrt")


# The we add the headers of our project
target_include_directories(server PRIVATE "include/")
target_include_directories(client1 PRIVATE "include/")
target_include_directories(client2 PRIVATE "include/")
target_include_directories(client3 PRIVATE "include/")


