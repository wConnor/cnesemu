set(PROJECT_NAME_TEST ${CMAKE_PROJECT_NAME}_test)

set(CMAKE_OUTPUT_ON_FAILURE TRUE)

set(TEST_SOURCES
  test_main.cpp

  basic_cpu_test.cpp

  functional_test.cpp
  )

find_package(GTest REQUIRED)
find_package(spdlog REQUIRED)
find_package(SFML COMPONENTS system window graphics audio REQUIRED)
add_executable(${PROJECT_NAME_TEST} ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME_TEST} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest spdlog::spdlog_header_only sfml-graphics sfml-audio)
gtest_discover_tests(${PROJECT_NAME_TEST})
