cmake_minimum_required(VERSION 3.21)
project(Client)

set(CMAKE_CXX_STANDARD 14)

add_executable(Client main.cpp Client.cpp Client.h)
target_link_libraries(Client ws2_32)
