project(depression)
add_library(depression INTERFACE)
target_include_directories(depression INTERFACE include)
target_link_libraries(depression INTERFACE global_shared)

