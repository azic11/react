#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <random>

#include "neural.hpp"
#include "synaptic.hpp"
#include "depression.hpp"
#include "inhibition.hpp"
#include "noise.hpp"


template <std::size_t N>
class Network
{
private:
	neural::vec<N> potentials;
	synaptic::mat<N> weights;
	nvec<N> utilisation_factors;
	double global_inhibition_current;

	std::mt19937 rng;

public:
	Network();
	void update(double dt, nvec<N> stimulus_current);
};


template <std::size_t N>
Network<N>::Network()
{
	potentials.fill(0.0);
	//weights.fill(0.0);
	utilisation_factors.fill(1.0);
	global_inhibition_current = 0.0;

	std::random_device rd;
	rng = std::mt19937(rd());
}

template <std::size_t N>
void Network<N>::update(double dt, nvec<N> stimulus_current)
{
	nvec<N> firing_rates = neural::firing_rates(potentials);
	global_inhibition_current = inhibition::evolve_global_current(dt, global_inhibition_current, firing_rates);
	utilisation_factors = depression::evolve_utilisation_factors(dt, utilisation_factors, firing_rates);

	// update connectome
	weights = synaptic::evolve_weights(dt, weights, firing_rates);

	// update potentials
	nvec<N> random_currents = noise::generate_vector<N,std::mt19937>(rng);
	nvec<N> synaptic_transmissions = synaptic::transmission_currents(potentials, utilisation_factors, weights);
	nvec<N> inhibitory_currents;
	inhibitory_currents.fill(global_inhibition_current);
	potentials = neural::evolve_potentials(dt, potentials, stimulus_current, synaptic_transmissions, inhibitory_currents, random_currents);
}

#endif
