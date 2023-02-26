#ifndef METROPOLIS_HPP
#define METROPOLIS_HPP

#include <ctime>
#include <random>

double metropolis(
    double action_function(double*, const int&, const double&),
    double action_difference_function(double*, const int&, const double&, const int&, const double&),
    const double&,
    const double&,
    const int&,
    double*,
    const int&,
    std::mt19937&
);

double metropolis_scale_tuning(
    double action_function(double*, const int&, const double&),
    double action_difference_function(double*, const int&, const double&, const int&, const double&),
    const double&,
    const double&,
    const double&,
    const int&,
    double*,
    const int&,
    std::mt19937&
);

#endif