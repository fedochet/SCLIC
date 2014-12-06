cmake_minimum_required(VERSION 2.8.4)

project(methods_library)

set(SOURCE_LIB
    methods.cpp
    crc32.c)

add_library(methods_lib STATIC ${SOURCE_LIB})
