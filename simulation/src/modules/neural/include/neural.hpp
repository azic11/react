#ifndef NEURAL_HPP
#define NEURAL_HPP

#include <algorithm>
#include <numeric>
#include <cmath>

#include "global_types.hpp"


namespace neural
{
	template <std::size_t N>
	using vec = std::array<double,N>;

	constexpr double time_constant = 0.155;

	double firing_rate(double potential);
	template<std::size_t N>
	nvec<N> firing_rates(nvec<N> potentials);

	double evolve_potential(double dt, double old_potential, double stimulus,
			double synaptic_transmission, double inhibitory, double random);
	template<std::size_t N>
	vec<N> evolve_potentials(double dt, vec<N> potentials, vec<N> stimuli,
			vec<N> synaptic_transmissions, vec<N> inhibitory, vec<N> random);
}

#include "neural.tpp"

#endif

