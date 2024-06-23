#ifndef SYNAPTIC_HPP
#define SYNAPTIC_HPP

#include <array>
#include <vector>
#include <algorithm>
#include <numeric>

#include "global_defs.hpp"


namespace synaptic
{
	// type definitions
	template <std::size_t N>
	using vec = std::array<std::vector<double>, N>;
	
	template <std::size_t N>
	using mat = std::array<vec<N>, N>;

	// calculation / transformation
	// TODO generalise using function template synaptic::transform
	template <std::size_t N>
	ncountmat<N> count(mat<N> weights);
	template <std::size_t N>
	nmat<N> calculate_accumulated_weights(mat<N> weights);

	// transmission
	template<std::size_t N>
	double transmission_current(nvec<N> potentials, nvec<N> util_factors,
			vec<N> weights);
	template<std::size_t N>
	nvec<N> transmission_currents(nvec<N> potentials, nvec<N> util_factors,
			mat<N> weights);

	// synaptic plasticity
	constexpr double threshold = 0.5;
	constexpr double max_weight = 0.7;
	constexpr double decay_rate = 1. / (2 * 24 * 60 * 60);
	constexpr double LTP_rate = 0.1;
	constexpr double LTD_rate = 0.01;
	double evolve_weight(double dt, double weight, double fr1, double fr2);

	template<std::size_t N>
	mat<N> evolve_weights(double dt, mat<N> weights, nvec<N> firing_rates);

	// structural plasticity
	// TODO: implement
}


template <std::size_t N>
ncountmat<N> synaptic::count(mat<N> weights)
{
	ncountmat<N> count;
	std::transform(weights.begin(), weights.end(), count.begin(), [](synaptic::vec<N> all_weights_one_neuron) -> ncountvec<N>
	{
		ncountvec<N> counts_one_neuron;
		std::transform(all_weights_one_neuron.begin(), all_weights_one_neuron.end(), counts_one_neuron.begin(), [](std::vector<double> weights_between_neuron_pair) -> std::size_t
		{
			return weights_between_neuron_pair.size();
		});
		return counts_one_neuron;
	});
	return count;
}

template<std::size_t N>
nmat<N> synaptic::calculate_accumulated_weights(synaptic::mat<N> weights)
{
	nmat<N> accumulated_weights;
	std::transform(weights.begin(), weights.end(), accumulated_weights.begin(), [](synaptic::vec<N> all_weights_one_neuron) -> nvec<N>
	{
		nvec<N> accumulated_weights_one_neuron;
		std::transform(all_weights_one_neuron.begin(), all_weights_one_neuron.end(), accumulated_weights_one_neuron.begin(), [](std::vector<double> weights_between_neuron_pair) -> double
		{
			return std::reduce(weights_between_neuron_pair.begin(), weights_between_neuron_pair.end());
		});
		return accumulated_weights_one_neuron;
	});
	return accumulated_weights;
}

template<std::size_t N>
double synaptic::transmission_current(nvec<N> potentials, nvec<N> util_factors,
		vec<N> weights)
{
	nvec<N> weight_sums;
	std::transform(weights.begin(), weights.end(), weight_sums.begin(),
			[](std::vector<double> w) { return std::reduce(w.begin(), w.end(), 0.); });
	nvec<N> factors;
	std::transform(potentials.begin(), potentials.end(), util_factors.begin(), factors.begin(),
			[](double p, double u) { return p * u; });
	return std::transform_reduce(factors.begin(), factors.end(), weight_sums.begin(), 0.);
}

template<std::size_t N>
nvec<N> synaptic::transmission_currents(nvec<N> potentials, nvec<N> util_factors,
		mat<N> weights)
{
	nvec<N> currents;
	std::transform(weights.begin(), weights.end(), currents.begin(), 
			[&](vec<N> w) { return transmission_current(potentials, util_factors, w); });
	return currents;
}

double synaptic::evolve_weight(double dt, double weight, double fr1, double fr2)
{
	if ((fr1 < threshold) && (fr2 < threshold))
		return weight * (1 - decay_rate * dt);
	else if ((fr1 > threshold) && (fr2 > threshold))
		return weight + LTP_rate * (max_weight - weight) * dt;
	else
		return weight * (1 - LTD_rate * dt);
}

template<std::size_t N>
synaptic::mat<N> synaptic::evolve_weights(double dt, synaptic::mat<N> weights,
		nvec<N> firing_rates)
{
	// TODO: optimise!!!
	for (std::size_t i = 0; i < N; i++)
	{
		double fr_i = firing_rates[i];
		for (std::size_t j = 0; j < N; j++)
		{
			double fr_j = firing_rates[j];
			for (std::size_t k = 0; k < weights[i][j].size(); k++)
				weights[i][j][k] = evolve_weight(dt, weights[i][j][k], fr_i, fr_j);
		}
	}
	return weights;
}

#endif

