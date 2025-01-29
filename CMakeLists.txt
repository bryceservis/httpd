cmake_minimum_required(VERSION 3.30)
project(httpd C)

set(CMAKE_C_STANDARD 11)

add_executable(httpd src/main.c
        src/tcp/socket.c
        src/tcp/socket.h)
