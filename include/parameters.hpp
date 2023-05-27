#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <iostream>
#include <string>

// Struct that packs the simulation
// parameters all together.
//
// GEOM     =   string, either p1q0 or p0q1     
//
// N        =   # of eigenvalues
//
// TGTAR    =   target acceptance rate
// 
// NDA      =   # of dual averaging iterations
//
// NTH      =   # of thermalization iterations
// 
// NSWP     =   # of iterations per sample
//
// NMEAS    =   # of samples to be collected
//
struct Simul_params
{
    // Geometric parameters
    std::string GEOM;
    int N;

    // Acceptance rate
    double TGTAR;

    // Simulation parameters
    int NDA;
    int NTH;
    int NSWP;
    int NMEAS;

    // Control string
    std::string control;
};

// Function to read simulation parameters from stream
bool read_init_stream(std::istream&, struct Simul_params&);

// Checks that the necessary parameters are there
bool params_validity(const struct Simul_params&);

#endif
