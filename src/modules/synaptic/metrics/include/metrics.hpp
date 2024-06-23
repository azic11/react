#ifndef SYNAPTIC_METRICS_HPP
#define SYNAPTIC_METRICS_HPP

#include <algorithm>
#include <numeric>

#include "global_types.hpp"
#include "types.hpp"


namespace synaptic
{
	template <std::size_t N>
	ncountmat<N> count(mat<N> weights);

	template <std::size_t N>
	nmat<N> calculate_accumulated_weights(mat<N> weights);
}

#include "metrics.tpp"

#endif

