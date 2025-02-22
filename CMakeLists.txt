cmake_minimum_required(VERSION 3.18.4)
project(libpkt C)

set(CMAKE_C_STANDARD 11)

# Create static library from the source file
add_library(libpkt STATIC libpkt.c)
# Create static library from the source file
add_library(libpkt STATIC libpkt.c)

# Include the current source directory for headers
target_include_directories(libpkt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Install the target library
install(TARGETS libpkt DESTINATION lib)

# Install only the header file
install(FILES libpkt.h DESTINATION include)