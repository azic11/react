#include "structural.hpp"


double synaptic::generalised_logistic_function(double x)
{
    constexpr double rho       = 1.;		// Skewness of generalised logistic function (1=normal sigmoid)
    constexpr double v_off     = 0.0;		// Rate offset v = v_off + (1-v_off)*sigmoid( potential )
	return v_off + (1. - v_off) * std::pow((1. + std::exp(-x)), -rho);
}
