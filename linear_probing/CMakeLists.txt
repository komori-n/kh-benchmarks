cmake_minimum_required(VERSION 3.13)

project(linear_probing_benchmark)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(../third-party/benchmark benchmark)
add_executable(
  linear_probing_benchmark
  src/main.cpp
)

target_include_directories(
  linear_probing_benchmark
  PRIVATE
  src
)
target_link_libraries(linear_probing_benchmark benchmark::benchmark)

