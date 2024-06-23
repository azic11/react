#ifndef NOISE_HPP
#define NOISE_HPP

#include <algorithm>
#include <random>

#include "global_types.hpp"


namespace noise
{
	constexpr double std = 1.5;

	template <std::size_t N, typename rng_t>
	nvec<N> generate_vector(rng_t& rng);
}

#include "noise.tpp"

#endif

