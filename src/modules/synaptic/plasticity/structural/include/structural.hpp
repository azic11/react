#ifndef SYNAPTIC_PLASTICITY_STRUCTURAL_HPP
#define SYNAPTIC_PLASTICITY_STRUCTURAL_HPP

#include <cmath>
#include <random>

#include "types.hpp"
#include "constants.hpp"


namespace synaptic
{
    double generalised_logistic_function(double x);

	constexpr double init_weight = 0.001;
	constexpr double creation_rate = 1. / 3600. / 24.;
	constexpr double removal_rate = 24. * creation_rate;
	constexpr double min_removal_rate = 0.03 * creation_rate;
	constexpr double beta = 20. / max_weight;
	constexpr double epsilon = 0.35 * max_weight;

	template <typename rng_t>
	double generate_creation_time_interval(rng_t& rng, std::size_t free_synapses);
	template <typename rng_t>
	double generate_lifetime_threshold(rng_t& rng);

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
}

#include "structural.tpp"

#endif

