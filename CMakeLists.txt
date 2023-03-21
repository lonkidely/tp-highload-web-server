cmake_minimum_required(VERSION 3.16.3)
project(tp_highload_web_server C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(project/include)

add_executable(tp_highload_web_server
        project/main.c project/src/config.c project/src/server.c project/src/http.c project/src/utils.c project/src/handler.c project/src/thread_pool.c)

TARGET_LINK_LIBRARIES(tp_highload_web_server pthread)
