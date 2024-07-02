project(synaptic_metrics)
add_library(synaptic_metrics INTERFACE)
target_include_directories(synaptic_metrics INTERFACE include)
target_link_libraries(synaptic_metrics INTERFACE
	global_shared
	synaptic_shared
)

