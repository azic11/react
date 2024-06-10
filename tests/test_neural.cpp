#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "neural.hpp"


TEST_CASE("correct computation of firing rates", "[neural]") {
	double potential = -1.7;
	double expected = 1.0 / (1.0 + std::exp(-potential));
	double result = neural::firing_rate(potential);
	
	REQUIRE_THAT(result, Catch::Matchers::WithinRel(expected, RELATIVE_EPSILON));
}

TEST_CASE("correct computation of evolved potentials", "[neural]") {
    double dt = 0.1;
    double old_potential = -1.7;
    double stimulus = 0.6;
    double synaptic_transmission = 0.2;
    double inhibitory = -0.3;
    double random = 0.1;
    
    double expected = old_potential + (dt / neural::time_constant) * (-old_potential + stimulus + synaptic_transmission + inhibitory + random);
    double result = neural::evolve_potential(dt, old_potential, stimulus, synaptic_transmission, inhibitory, random);
    
    REQUIRE_THAT(result, Catch::Matchers::WithinRel(expected, RELATIVE_EPSILON));
}

