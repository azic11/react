project(synaptic_plasticity_hebbian)

add_library(synaptic_plasticity_hebbian
	src/hebbian.cpp
	include/hebbian.tpp
	include/hebbian.hpp
)

target_include_directories(synaptic_plasticity_hebbian PUBLIC include)

target_link_libraries(synaptic_plasticity_hebbian PUBLIC
	global_shared
	synaptic_shared
	synaptic_plasticity_shared
)

