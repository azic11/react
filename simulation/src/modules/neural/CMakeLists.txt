project(neural)
add_library(neural INTERFACE)
target_include_directories(neural INTERFACE include)
target_link_libraries(neural INTERFACE global_shared)

