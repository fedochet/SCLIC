cmake_minimum_required(VERSION 2.8.4)
project(crc_32)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    crc32.c
    crc32.h
    main.cpp
    factorisation.cpp
    factorisation.h
    hardcode.cpp
    hardcode.hpp)

add_executable(crc_32 ${SOURCE_FILES})