#ifndef DEPRESSION_HPP
#define DEPRESSION_HPP

#include <algorithm>

#include "global_types.hpp"


namespace depression
{
	constexpr double decay_time = 5.;
	constexpr double scale_factor = 1.;
	
	template<std::size_t N>
	nvec<N> evolve_utilisation_factors(double dt, nvec<N> old_factors, nvec<N>
			firing_rates);
}

#include "depression.tpp"

#endif
