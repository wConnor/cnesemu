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

  bus.hpp
  bus.cpp

  cpu6502.hpp
  cpu6502.cpp

  cartridge.hpp
  cartridge.cpp

  video.hpp
  video.cpp

  instruction.hpp
  )

find_package(Threads REQUIRED)
find_package(SFML COMPONENTS system window graphics audio REQUIRED)
find_package(spdlog REQUIRED)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio Threads::Threads spdlog::spdlog_header_only)
