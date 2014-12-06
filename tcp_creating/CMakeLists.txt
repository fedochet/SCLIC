cmake_minimum_required(VERSION 2.8.4)

project(tcp_pkg)

set(SOURCE_LIB
    tcp_package.cpp)

add_library(tcp_lib STATIC ${SOURCE_LIB})
