cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(channel++)

# Prepare "Catch" library for other executables
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/Catch2)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

# Make test executable
set(TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/main.cpp)
add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests Catch)
