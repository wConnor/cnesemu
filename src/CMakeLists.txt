cmake_minimum_required(VERSION 3.5)

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

add_custom_target(run
    COMMAND ${CMAKE_PROJECT_NAME}
    DEPENDS ${CMAKE_PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

find_package(Threads REQUIRED)
find_package(SFML COMPONENTS system window graphics audio REQUIRED)
find_package(spdlog REQUIRED)
add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME} sfml-graphics sfml-audio Threads::Threads spdlog::spdlog_header_only)
add_library(${CMAKE_PROJECT_NAME}_lib STATIC ${PROJECT_SOURCES})

