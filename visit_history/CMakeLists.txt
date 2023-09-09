cmake_minimum_required(VERSION 3.13)

project(visit_history_benchmark)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(../third-party/benchmark benchmark)
add_executable(
  visit_history_benchmark
  src/main.cpp
  src/old_visit_history.hpp
  src/open_address_history.hpp
)

target_include_directories(
  visit_history_benchmark
  PRIVATE
  src
)
target_link_libraries(visit_history_benchmark benchmark::benchmark)

