add_executable(main main.cpp
					Network.hpp
					ParadigmFile.hpp
					logging.hpp
					stimulation.hpp
)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(modules)
target_link_libraries(main global_shared
						   io
						   neural
						   synaptic
						   inhibition
						   depression
						   noise
)

