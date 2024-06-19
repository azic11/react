#ifndef DEPRESSION_HPP
#define DEPRESSION_HPP

#include <algorithm>

#include "global_defs.hpp"


namespace depression
{
	constexpr double decay_time = 5.;
	constexpr double scale_factor = 1.;
	
	template<std::size_t N>
	nvec<N> evolve_utilisation_factors(double dt, nvec<N> old_factors, nvec<N>
			firing_rates);
}

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

#endif
