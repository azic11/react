project(noise)
add_library(noise INTERFACE)
target_include_directories(noise INTERFACE include)
target_link_libraries(noise INTERFACE global_shared)

