#include "noise.hpp"

template <std::size_t N, typename rng_t>
nvec<N> noise::generate_vector(rng_t& rng)
{
	std::normal_distribution<double> dist(0.0, std);
	nvec<N> noise;
	// maybe use parallel execution policy
	std::generate(noise.begin(), noise.end(), [&dist, &rng](){ return dist(rng); });
	return noise;
}


