#include "depression.hpp"

template <std::size_t N>
nvec<N> depression::evolve_utilisation_factors(double dt, nvec<N> old_factors,
		nvec<N> firing_rates)
{
	nvec<N> evolved_factors;
	std::transform(old_factors.begin(), old_factors.end(), firing_rates.begin(), evolved_factors.begin(),
						   [dt](double f, double r) {
							   return f + dt * (1 - f) / decay_time - scale_factor * r;
						   });
	return evolved_factors;
}


