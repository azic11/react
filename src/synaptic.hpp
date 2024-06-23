#ifndef SYNAPTIC_HPP
#define SYNAPTIC_HPP

#include <array>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>

#include "global_defs.hpp"
#include "logist.hpp"


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

	template <std::size_t N>
	mat<N> evolve_weights(double dt, mat<N> weights, nvec<N> firing_rates);

	// structural plasticity
	constexpr double init_weight = 0.001;
	constexpr double creation_rate = 1. / 3600. / 24.;
	constexpr double removal_rate = 24. * creation_rate;
	constexpr double min_removal_rate = 0.03 * creation_rate;
	constexpr double beta = 20. / max_weight;
	constexpr double epsilon = 0.35 * max_weight;
	template <typename rng_t>
	double generate_creation_time_interval(rng_t& rng, std::size_t free_synapses)
	{
		static const double base_creation_rate = 1. / (60 * 60 * 24);
		std::exponential_distribution<double> distr(base_creation_rate * free_synapses);
		double dt = distr(rng);
		return dt;
	}
	template <typename rng_t>
	double generate_lifetime_threshold(rng_t& rng)
	{
		std::exponential_distribution<double> distr;
		double thresh = distr(rng);
		return thresh;
	}
	template <std::size_t N>
	class StructureArtist
	{
	private:
		std::size_t max_synapses;
		mat<N> creation_times;
		mat<N> lifetimes;
	public:
		template <class rng_t>
		StructureArtist(rng_t& rng, std::size_t max_synapses);
		template<class rng_t>
		mat<N> remove_synapses(double dt, rng_t& rng, mat<N> weights);
		template<class rng_t>
		mat<N> add_synapses(double time, rng_t& rng, mat<N> weights);
		template <class rng_t>
		mat<N> update_structure(double time, double dt, rng_t& rng, mat<N> weights);
	};
	template <std::size_t N>
	template <class rng_t>
	synaptic::StructureArtist<N>::StructureArtist(rng_t& rng, std::size_t max_synapses) : max_synapses(max_synapses)
	{
		for (std::size_t i = 0; i < N; i++)
		{
			for (std::size_t j = 0; j < N; j++)
			{
				for (std::size_t k = 0; k < max_synapses; k++)
				{
					creation_times[i][j].push_back(generate_creation_time_interval(rng, max_synapses));
				}
			}
		}
	}
	template <std::size_t N>
	template<class rng_t>
	synaptic::mat<N> synaptic::StructureArtist<N>::remove_synapses(double dt, rng_t& rng, synaptic::mat<N> weights)
	{
		// TODO: optimise
		for (std::size_t i = 0; i < N; i++)
		{
			for (std::size_t j = 0; j < N; j++)
			{
				for (std::size_t k = 0; k < lifetimes[i][j].size();)
				{
					lifetimes[i][j][k] -= dt * (min_removal_rate / removal_rate + (1 - min_removal_rate / removal_rate) * generalised_logistic_function(beta * (epsilon - weights[i][j][k] / max_weight)));
					if (lifetimes[i][j][k] <= 0.)
					{
						weights[i][j].erase(weights[i][j].begin() + k);
						lifetimes[i][j].erase(lifetimes[i][j].begin() + k);
						std::size_t free_synapses = max_synapses - lifetimes[i][j].size();
						creation_times[i][j].push_back(generate_creation_time_interval(rng, free_synapses));
					}
					else
						k++;
				}
			}
		}
		return weights;
	}
	template <std::size_t N>
	template <class rng_t>
	synaptic::mat<N> synaptic::StructureArtist<N>::add_synapses(double time, rng_t& rng, synaptic::mat<N> weights)
	{
		// TODO: optimise
		for (std::size_t i = 0; i < N; i++)
		{
			for (std::size_t j = 0; j < N; j++)
			{
				for (std::size_t k = 0; k < creation_times[i][j].size();)
				{
					if (creation_times[i][j][k] <= time)
					{
						weights[i][j].push_back(init_weight);
						creation_times[i][j].erase(creation_times[i][j].begin() + k);
						lifetimes[i][j].push_back(generate_lifetime_threshold(rng) / removal_rate);	
					}
					else
						k++;
				}
			}
		}
		return weights;
	}
	template <std::size_t N>
	template <class rng_t>
	synaptic::mat<N> synaptic::StructureArtist<N>::update_structure(double time, double dt, rng_t& rng, synaptic::mat<N> weights)
	{
		weights = remove_synapses(dt, rng, weights);
		weights = add_synapses(time, rng, weights);
		return weights;
	}
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

