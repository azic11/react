#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "synaptic.hpp"


TEST_CASE("correct computation of transmission current", "[synaptic][transmission_current]")
{
	constexpr std::size_t N = 3;
	nvec<N> potentials = {1., 2., 3.};
	nvec<N> util_factors = {4., 5., 6.};
	synaptic::vec<N> weights = {{
        {0.1, 0.3},
        {0.4, 0.6},
        {0.7, 0.9}
    }};

	double result = synaptic::transmission_current(potentials, util_factors, weights);

	double expected = 0.;
	for (std::size_t i = 0; i < N; i++)
	{
		double weight_sum = 0.;
		for (std::size_t j = 0; j < weights[i].size(); j++)
			weight_sum += weights[i][j];
		expected += potentials[i] * util_factors[i] * weight_sum;
	}

	REQUIRE_THAT(result, Catch::Matchers::WithinRel(expected, RELATIVE_EPSILON));
}

TEST_CASE("correct computation of evolved weight", "[synaptic][evolve_weight]")
{
	double dt = 0.1;
	double weight = 0.5;

	SECTION("decay")
	{
		double fr1 = 0.3;
		double fr2 = 0.4;
		double result = synaptic::evolve_weight(dt, weight, fr1, fr2);

		double expected = weight * (1. - synaptic::decay_rate * dt);
		REQUIRE_THAT(result, Catch::Matchers::WithinRel(expected, RELATIVE_EPSILON));
	}

	SECTION("potentiation")
	{
		double fr1 = 0.6;
		double fr2 = 0.7;
		double result = synaptic::evolve_weight(dt, weight, fr1, fr2);

		double expected = weight + synaptic::LTP_rate * dt * (synaptic::max_weight - weight);
		REQUIRE_THAT(result, Catch::Matchers::WithinRel(expected, RELATIVE_EPSILON));
	}

	SECTION("depression")
	{
		double fr1 = 0.6;
		double fr2 = 0.2;
		double result = synaptic::evolve_weight(dt, weight, fr1, fr2);

		double expected = weight - synaptic::LTD_rate * dt * weight;
		REQUIRE_THAT(result, Catch::Matchers::WithinRel(expected, RELATIVE_EPSILON));
	}
}

