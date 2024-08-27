cmake_minimum_required(VERSION 3.10)

project(self_organised_reactivation_network)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -pedantic)

add_subdirectory(src)
add_subdirectory(tests)

