cmake_minimum_required(VERSION 2.8.4)

project(transport_library)

set(SOURCE_LIB
    tcp.cpp)

add_library(transport_library STATIC ${SOURCE_LIB})