#ifndef METROPOLIS_HPP
#define METROPOLIS_HPP

#include <ctime>
#include <random>
#include <functional>

double metropolis(
    std::function<double(double*, const int&, const double&)>,
    std::function<double(double*, const int&, const double&, const int&, const double&)>,
    const double&,
    const double&,
    const int&,
    double*,
    const int&,
    std::mt19937&
);

double metropolis_scale_tuning(
    std::function<double(double*, const int&, const double&)>,
    std::function<double(double*, const int&, const double&, const int&, const double&)>,
    const double&,
    const double&,
    const double&,
    const int&,
    double*,
    const int&,
    std::mt19937&
);

#endif