cmake_minimum_required(VERSION 3.10)
project(libpkt_tests LANGUAGES C)

# Check for pkg-config tool
find_package(PkgConfig REQUIRED)

# Check for the Check library using pkg-config
pkg_check_modules(CHECK REQUIRED check)

# Add the test source files
add_executable(test_libpkt test_libpkt.c)

# Link the test executable with the library and Check
target_link_libraries(test_libpkt PRIVATE pkt ${CHECK_LIBRARIES})

# Include directories for Check
target_include_directories(test_libpkt PRIVATE ${CHECK_INCLUDE_DIRS})

# Add a test target
add_test(NAME test_libpkt COMMAND test_libpkt)
