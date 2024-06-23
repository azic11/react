project(inhibition)
add_library(inhibition INTERFACE)
target_include_directories(inhibition INTERFACE include)
target_link_libraries(inhibition INTERFACE global_shared)

