cmake_minimum_required(VERSION 3.16)
project(StrategyPattern)

set(CMAKE_CXX_STANDARD 17)
find_package(Boost COMPONENTS util)

include_directories(include)
include_directories(third-party)

add_executable(type_erasure_benchmark benchmark.cc)
target_link_libraries(type_erasure_benchmark ${Boost_LIBRARIES})