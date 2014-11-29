cmake_minimum_required(VERSION 2.8.4)
SET(CMAKE_CXX_FLAGS "-std=c++0x")
project(request_types)

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

include_directories(Libs Transport)

set(SOURCE_FILES
    main.cpp)

add_executable(request_types ${SOURCE_FILES})
add_subdirectory(Libs)
add_subdirectory(Transport)

target_link_libraries(request_types methods transport_library)
target_link_libraries(request_types ${OPENSSL_LIB} ${OPENSSL_CRYPTO_LIBRARY})




