cmake_minimum_required(VERSION 3.16.3)
project(tp_highload_web_server C)

set(CMAKE_C_STANDARD 11)

include_directories(project/include)

add_executable(tp_highload_web_server
        project/main.c project/src/config.c project/src/server.c)
