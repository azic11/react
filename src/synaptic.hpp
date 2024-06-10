#ifndef SYNAPTIC_HPP
#define SYNAPTIC_HPP

#include <array>
#include <vector>
#include <algorithm>

#include "global_defs.hpp"


namespace synaptic
{
	// type definitions
	template <std::size_t N>
	using vec = std::array<std::vector<double>, N>;
	
	template <std::size_t N>
	using mat = std::array<vec<N>, N>;

	// transmission
	template<std::size_t N>
	nvec<N> transmission_currents(nvec<N> potentials, nvec<N> util_factors,
			mat<N> weights);

	// synaptic plasticity
	const double threshold = 0.5;
	const double max_weight = 0.7;
	const double decay_rate = 1. / (2 * 24 * 60 * 60);
	const double LTP_rate = 0.1;
	const double LTD_rate = 0.01;
	double evolve_weight(double dt, double weight, double pot1, double pot2);

	template<std::size_t N>
	mat<N> evolve_weights(double dt, mat<N> weights, nvec<N> potentials);

	// structural plasticity
	// TODO: implement
}

template<std::size_t N>
nvec<N> synaptic::transmission_currents(nvec<N> potentials, nvec<N> util_factors,
		mat<N> weights)
{
	nvec<N> currents;
	// TODO: optimise!!!
	std::transform(weights.begin(), weights.end(), currents.begin(), [&](vec <N> w) -> double
	{
		double sum = 0.0;
		for (std::size_t i = 0; i < N; i++)
		{
			double f = util_factors[i] * potentials[i];
			for (std::size_t j = 0; j < N; j++)
				sum += f * w[i][j];
		}
		return sum;
	});
	return currents;
}

double synaptic::evolve_weight(double dt, double weight, double pot1, double pot2)
{
	if ((pot1 < threshold) && (pot2 < threshold))
		return weight * (1 - decay_rate * dt);
	else if ((pot1 > threshold) && (pot2 > threshold))
		return weight + LTP_rate * (max_weight - weight) * dt;
	else
		return weight * (1 - LTD_rate * dt);
}

template<std::size_t N>
synaptic::mat<N> synaptic::evolve_weights(double dt, synaptic::mat<N> weights, nvec<N> potentials)
{
	mat<N> evolved_weights;
	// TODO: optimise!!!
	for (std::size_t i = 0; i < N; i++)
	{
		double pot_i = potentials[i];
		for (std::size_t j = 0; j < N; j++)
		{
			double pot_j = potentials[j];
			for (std::size_t k = 0; k < weights[i][j].size(); k++)

			evolved_weights[i][j][k] = evolve_weight(dt, weights[i][j][k], pot_i,
					pot_j);
		}
	}
	return evolved_weights;
}

#endif

