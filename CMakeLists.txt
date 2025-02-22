cmake_minimum_required(VERSION 3.18.4)
project(libpkt C)

set(CMAKE_C_STANDARD 11)

add_library(libpkt STATIC libpkt.c)

target_include_directories(libpkt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS libpkt DESTINATION lib)
install(FILES libpkt.h DESTINATION include)

#enable_testing()
#add_subdirectory(tests)