cmake_minimum_required(VERSION 3.9)
project(socket_chat_reborn C)

set(CMAKE_C_STANDARD 99)

add_executable(socket_chat_reborn server.c client.c)