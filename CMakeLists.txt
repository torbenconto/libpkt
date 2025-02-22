cmake_minimum_required(VERSION 3.18.4)
project(libpkt C)

set(CMAKE_C_STANDARD 11)

add_library(libpkt STATIC libpkt.c)

enable_testing()
add_subdirectory(tests)
