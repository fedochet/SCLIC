cmake_minimum_required(VERSION 2.8.4)

project(transport_library)

set(SOURCE_LIB
    tcp.cpp
    tl.cpp
    assets.hpp)

add_library(transport_library STATIC ${SOURCE_LIB})