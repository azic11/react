#include "hebbian.hpp"

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

