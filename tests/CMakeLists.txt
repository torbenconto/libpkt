cmake_minimum_required(VERSION 3.10)
project(libpkt_tests LANGUAGES C)

# Add the Check library
find_package(Check REQUIRED)

# Add the test source files
add_executable(test_libpkt test_libpkt.c)

# Link the test executable with the library and Check
target_link_libraries(test_libpkt PRIVATE pkt Check::Check)

# Add a test target
add_test(NAME test_libpkt COMMAND test_libpkt)
