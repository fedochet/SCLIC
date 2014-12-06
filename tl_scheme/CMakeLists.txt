cmake_minimum_required(VERSION 2.8.4)

project(classes)

set(SOURCE_LIB
    classes.cpp)

add_library(tl_lib STATIC ${SOURCE_LIB})