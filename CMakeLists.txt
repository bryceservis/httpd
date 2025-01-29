cmake_minimum_required(VERSION 3.30)
project(httpd C)

set(CMAKE_C_STANDARD 11)

add_executable(httpd main.c
        networking/socket.c
        networking/socket.h)
