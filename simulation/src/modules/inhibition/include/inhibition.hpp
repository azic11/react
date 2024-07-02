#ifndef INHIBITION_HPP
#define INHIBITION_HPP

#include "global_types.hpp"
#include "neural.hpp"
#include "synaptic.hpp"

namespace inhibition
{
	constexpr double decay_time = neural::time_constant;
	constexpr double weight = 3.5 * synaptic::max_weight;

	template<std::size_t N>
	double evolve_global_current(double dt, double old_current, nvec<N>
			firing_rates);
}

#include "inhibition.tpp"

#endif

