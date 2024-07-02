#include "neural.hpp"

double neural::firing_rate(double potential)
{
	// return 1.0 / (1.0 + std::exp(-potential)));
	return std::pow(1 + std::exp(-potential), -1);
}

template<std::size_t N>
neural::vec<N> neural::firing_rates(neural::vec<N> potentials)
{
	neural::vec<N> firing_rates;
	std::transform(potentials.begin(), potentials.end(), firing_rates.begin(), neural::firing_rate);
	return firing_rates;
}

double neural::evolve_potential(double dt, double old_potential, double
		stimulus, double synaptic_transmission, double inhibitory, double
		random)
{
	double evolve_potential = old_potential
						 + (dt / time_constant) * (- old_potential
												   + stimulus
												   + synaptic_transmission
												   + inhibitory
												   + random);
	return evolve_potential;
}

template<std::size_t N>
neural::vec<N> neural::evolve_potentials(double dt, vec<N> potentials, nvec<N>
		stimuli, nvec<N> synaptic_transmissions, nvec<N> inhibitory, nvec<N> random)
{
	vec<N> evolved_potentials;
	// perhaps use parallel execution policy
	std::array<std::size_t, N> indices;
	std::iota(indices.begin(), indices.end(), 0);
	std::transform(indices.begin(), indices.end(), evolved_potentials.begin(),
			[&](std::size_t i) {
				return evolve_potential(dt, potentials[i], stimuli[i], synaptic_transmissions[i], inhibitory[i], random[i]);
			});
	return evolved_potentials;
}


