cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(channel++)

include_directories("${PROJECT_SOURCE_DIR}/libs")
find_package(Catch2 REQUIRED)
target_link_libraries(tests Catch2::Catch2)
