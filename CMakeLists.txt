cmake_minimum_required(VERSION 3.16.3)
project(tp_highload_web_server C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-g -O0 -Wall -Wextra -Wpedantic -Wno-unused-variable -Wno-unknown-pragmas -Wno-sign-compare -Wwrite-strings -Wno-unused")

include_directories(project/include)

add_executable(tp_highload_web_server
        project/main.c project/src/config.c project/src/server.c project/src/http.c project/src/utils.c project/src/handler.c)
