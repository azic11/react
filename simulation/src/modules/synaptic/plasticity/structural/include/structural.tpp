#include "structural.hpp"

template <typename rng_t>
double synaptic::generate_creation_time_interval(rng_t& rng, std::size_t free_synapses)
{
	static const double base_creation_rate = 1. / (60 * 60 * 24);
	std::exponential_distribution<double> distr(base_creation_rate * free_synapses);
	double dt = distr(rng);
	return dt;
}

template <typename rng_t>
double synaptic::generate_lifetime_threshold(rng_t& rng)
{
	std::exponential_distribution<double> distr;
	double thresh = distr(rng);
	return thresh;
}

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

