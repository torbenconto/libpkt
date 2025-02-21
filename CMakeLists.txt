cmake_minimum_required(VERSION 3.18.4)
project(pkt C)

set(CMAKE_C_STANDARD 11)

add_library(pkt STATIC pkt.c)
