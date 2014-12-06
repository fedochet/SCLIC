cmake_minimum_required(VERSION 2.8.4)

project(tl_creating)

set(SOURCE_LIB
    builder.cpp)

add_library(tl_builder_lib STATIC ${SOURCE_LIB})