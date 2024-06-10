cmake_minimum_required(VERSION 3.10)

project(self_organised_reactivation_network)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra -pedantic -Werror)

add_subdirectory(src)
add_subdirectory(tests)

