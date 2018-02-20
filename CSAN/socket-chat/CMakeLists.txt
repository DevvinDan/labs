cmake_minimum_required(VERSION 3.9)
project(socket_chat)

set(CMAKE_CXX_STANDARD 11)

add_executable(socket_chat server.cpp client.cpp)