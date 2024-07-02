project(synaptic_plasticity_structural)

add_library(synaptic_plasticity_structural
	src/structural.cpp
	include/structural.tpp
	include/structural.hpp
)

target_include_directories(synaptic_plasticity_structural PUBLIC include)

target_link_libraries(synaptic_plasticity_structural PUBLIC
	global_shared
	synaptic_shared
	synaptic_plasticity_shared
)

