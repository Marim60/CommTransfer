cmake_minimum_required(VERSION 3.21)
project(Server)

set(CMAKE_CXX_STANDARD 14)

add_executable(Server main.cpp Server.cpp Server.h Command.h DownloadCommand.h BroadcastCommand.h UploadCommand.h)
target_link_libraries(Server ws2_32.lib)