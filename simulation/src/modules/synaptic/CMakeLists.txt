project(synaptic)

add_library(synaptic INTERFACE synaptic.hpp)
target_include_directories(synaptic INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(shared)
target_link_libraries(synaptic INTERFACE synaptic_shared)

add_subdirectory(metrics)
target_link_libraries(synaptic INTERFACE synaptic_metrics)

add_subdirectory(transmission)
target_link_libraries(synaptic INTERFACE synaptic_transmission)

add_subdirectory(plasticity)
target_link_libraries(synaptic INTERFACE synaptic_plasticity)

