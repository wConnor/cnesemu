cmake_minimum_required(VERSION 3.5)
project(cnesemu)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

include_directories(src)

add_subdirectory(src)
add_subdirectory(tests)
