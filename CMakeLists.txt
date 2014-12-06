cmake_minimum_required(VERSION 2.8.4)
project(packets_organization)

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

include_directories(
    tl_scheme
    methods
    tl_creating
    connection
    tcp_creating)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp)

add_subdirectory(methods)
add_subdirectory(tl_scheme)
add_subdirectory(tl_creating)
add_subdirectory(connection)
add_subdirectory(tcp_creating)

add_executable(packets_organization ${SOURCE_FILES})
target_link_libraries(
    packets_organization
    tl_lib
    tl_builder_lib
    methods_lib
    tcp_lib
    connection_lib)
target_link_libraries(packets_organization ${OPENSSL_LIB} ${OPENSSL_CRYPTO_LIBRARY})

