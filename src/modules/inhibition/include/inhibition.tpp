#include "inhibition.hpp"

template<std::size_t N>
double inhibition::evolve_global_current(double dt, double old_current, nvec<N>
		firing_rates)
{
	double total_firing_rate = std::accumulate(firing_rates.begin(), firing_rates.end(), 0.0);
	double new_current = old_current - dt / decay_time * (old_current + weight * total_firing_rate);
	return new_current;
}


