#include "hebbian.hpp"


double synaptic::evolve_weight(double dt, double weight, double fr1, double fr2)
{
	if ((fr1 < threshold) && (fr2 < threshold))
		return weight * (1 - decay_rate * dt);
	else if ((fr1 > threshold) && (fr2 > threshold))
		return weight + LTP_rate * (max_weight - weight) * dt;
	else
		return weight * (1 - LTD_rate * dt);
}

