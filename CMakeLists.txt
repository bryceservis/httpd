cmake_minimum_required(VERSION 3.30)
project(httpd C)

set(CMAKE_C_STANDARD 11)

add_executable(httpd src/main.c
        src/config.c
        src/error.c
        include/schema/host.h
        include/config.h
        include/error.h
        src/socket.c
        include/socket.h
)

find_package(OpenSSL REQUIRED)
target_link_libraries(httpd OpenSSL::SSL)
