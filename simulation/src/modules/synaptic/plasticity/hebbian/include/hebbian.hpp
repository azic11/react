#ifndef SYNAPTIC_PLASTICITY_HEBBIAN_HPP
#define SYNAPTIC_PLASTICITY_HEBBIAN_HPP

#include "global_types.hpp"
#include "types.hpp"
#include "constants.hpp"


namespace synaptic
{
	constexpr double threshold = 0.5;
	constexpr double decay_rate = 1. / (2 * 24 * 60 * 60);
	constexpr double LTP_rate = 0.1;
	constexpr double LTD_rate = 0.01;

	double evolve_weight(double dt, double weight, double fr1, double fr2);

	template <std::size_t N>
	mat<N> evolve_weights(double dt, mat<N> weights, nvec<N> firing_rates);
}

#include "hebbian.tpp"

#endif

