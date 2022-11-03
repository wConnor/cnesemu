cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME cnesemu)

project(${PROJECT_NAME} VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

set(PROJECT_SOURCES
  main.cpp

  nes.hpp
  nes.cpp

  cpu6502.hpp
  cpu6502.cpp

  video.hpp
  video.cpp
  )

find_package(Threads REQUIRED)
find_package(SDL2 REQUIRED)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} SDL2::SDL2 SDL2::SDL2main Threads::Threads)
