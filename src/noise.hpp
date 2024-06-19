#ifndef NOISE_HPP
#define NOISE_HPP

#include <algorithm>
#include <random>

#include "global_defs.hpp"


namespace noise
{
	constexpr double std = 1.5;

	template <std::size_t N, typename rng_t>
	nvec<N> generate_vector(rng_t& rng);
}

template <std::size_t N, typename rng_t>
nvec<N> noise::generate_vector(rng_t& rng)
{
	std::normal_distribution<double> dist(0.0, std);
	nvec<N> noise;
	// maybe use parallel execution policy
	std::generate(noise.begin(), noise.end(), [&dist, &rng](){ return dist(rng); });
	return noise;
}

#endif

