#include "transmission.hpp"

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


