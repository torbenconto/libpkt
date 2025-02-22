cmake_minimum_required(VERSION 3.10)
project(libpkt VERSION 1.0.0 LANGUAGES C)

# Set the library name
set(LIBRARY_NAME pkt)

# Add the library source files
add_library(${LIBRARY_NAME} STATIC
    libpkt.c
)

# Include the header files
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Set the version information
set_target_properties(${LIBRARY_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

# Install the library and headers
install(TARGETS ${LIBRARY_NAME} DESTINATION lib)
install(FILES libpkt.h DESTINATION include)

# Export the library for use with find_package
export(TARGETS ${LIBRARY_NAME} FILE ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}Config.cmake)
install(EXPORT ${LIBRARY_NAME}Config DESTINATION share/${LIBRARY_NAME}/cmake)

enable_testing()
add_subdirectory(tests)
