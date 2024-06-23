#ifndef SYNAPTIC_TRANSMISSION_HPP
#define SYNAPTIC_TRANSMISSION_HPP

#include <algorithm>
#include <numeric>

#include "global_types.hpp"
#include "types.hpp"


namespace synaptic
{
	template<std::size_t N>
	double transmission_current(nvec<N> potentials, nvec<N> util_factors,
			vec<N> weights);
	template<std::size_t N>
	nvec<N> transmission_currents(nvec<N> potentials, nvec<N> util_factors,
			mat<N> weights);
}

#include "transmission.tpp"

#endif

