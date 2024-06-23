#ifndef LOGIST_HPP
#define LOGIST_HPP

#include <cmath>

constexpr float rho       = 1.;			// Skewness of generalized logistic function (1=normal sigmoid)
constexpr double v_off    = 0.0;		// Rate offset v = v_off + (1-v_off)*sigmoid( potential )

double generalised_logistic_function(double x){
	return v_off + (1. - v_off) * std::pow((1. + std::exp(-x)), -rho);
}

#endif

