find_package(PkgConfig REQUIRED)
pkg_check_modules(Check REQUIRED check)

include_directories(${Check_INCLUDE_DIRS})
link_directories(${Check_LIBRARY_DIRS})

# Add pthread to link libraries
add_executable(test_runner test_libpkt.c)
target_link_libraries(test_runner PRIVATE libpkt ${Check_LIBRARIES} pthread)

# Register the test
add_test(NAME LibPktTests COMMAND test_runner)
