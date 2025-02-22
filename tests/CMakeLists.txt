find_package(PkgConfig REQUIRED)
pkg_check_modules(Check REQUIRED check)

include_directories(${Check_INCLUDE_DIRS})

add_executable(test_runner test_libpkt.c)
target_link_libraries(test_runner PRIVATE libpkt ${Check_LIBRARIES} pthread)

add_test(NAME LibPktTests COMMAND test_runner)
