cmake_minimum_required(VERSION 3.18.4)
project(libpkt C)

set(CMAKE_C_STANDARD 11)

if (APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "Architectures to build for" FORCE)
endif()

add_library(libpkt STATIC libpkt.c)

target_include_directories(libpkt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS libpkt DESTINATION lib)
install(FILES libpkt.h DESTINATION include)

# Optionally enable testing if needed
# enable_testing()
# add_subdirectory(tests)
