cmake_minimum_required(VERSION 3.30)
project(microhttpd C)

set(CMAKE_C_STANDARD 11)

find_package(OpenSSL REQUIRED)
add_executable(microhttpd src/main.c
        src/sockets.c
        include/sockets.h
        src/error.c
        include/error.h
        src/http.c
        include/http.h
)

target_link_libraries(microhttpd OpenSSL::SSL)