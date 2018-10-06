# Prepare "Catch" library for other executables
set(CATCH_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/libs/Catch2/single_include)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(test ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_link_libraries(test Threads::Threads)
target_link_libraries(test Catch)
