cmake_minimum_required(VERSION 3.18.4)
project(libpkt C)

set(CMAKE_C_STANDARD 11)

# Create a static library from your source file
add_library(libpkt STATIC libpkt.c)

# Enable testing (assuming you have tests in a subdirectory)
enable_testing()
add_subdirectory(tests)

# Specify the installation locations
install(TARGETS libpkt DESTINATION lib)
install(FILES libpkt.h DESTINATION include)

# Set the version and description for the package
set(CPACK_PACKAGE_NAME "libpkt")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION "A simple C library for packet processing")
set(CPACK_PACKAGE_VENDOR "Torben Conto")
set(CPACK_PACKAGE_CONTACT "torbenmconto@gmail.com")
set(CPACK_PACKAGE_LICENSE "MIT")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")

# Specify where the package should be created
set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_BINARY_DIR}/package")

# Use CPack to generate a package
include(CPack)
