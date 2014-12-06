cmake_minimum_required(VERSION 2.8.4)

project(connection)

set(SOURCE_LIB
    connect.cpp)

add_library(connection_lib STATIC ${SOURCE_LIB})