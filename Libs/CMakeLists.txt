cmake_minimum_required(VERSION 2.8.4)

project(methods)

set(SOURCE_LIB
    factorisation.cpp
    hardcode.cpp
    crc32.c)

add_library(methods STATIC ${SOURCE_LIB})