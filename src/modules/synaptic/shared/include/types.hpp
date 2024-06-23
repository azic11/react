#ifndef SYNAPTIC_TYPES_HPP
#define SYNAPTIC_TYPES_HPP

#include <array>
#include <vector>


namespace synaptic
{
	template <std::size_t N>
	using vec = std::array<std::vector<double>, N>;
	
	template <std::size_t N>
	using mat = std::array<vec<N>, N>;
}

#endif

