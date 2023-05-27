#include "action.hpp"
#include "metropolis.hpp"
#include <random>
#include <ctime>
#include <iostream>
#include <fstream>

#define N 100
#define G2 -3
#define NDA 100000 // number of dual averaging iterations
#define NTH 100000 // number of thermalization iterations
#define NMEAS 1000 // number of measurements
#define NSWP 100 // number of sweeps in-between measurements
#define TGTAR 0.8 // target acceptance rate

using namespace std;

int main()
{
    // random number generator
    time_t seed = time(NULL);
    mt19937 gen(seed);
    uniform_real_distribution<double> dis_real(0., 1.);

    double eigvals[N]; 
    for(int i=0; i<N; ++i)
        eigvals[i] = dis_real(gen);

    double scale = 0.1;

    // scale tuning
    scale = metropolis_scale_tuning(action_p1q0, action_diff_p1q0, G2, scale, TGTAR, NDA, eigvals, N, gen);
    cout << "tuned scale: " << scale << endl;

    // thermalization
    double ar_th = metropolis(action_p1q0, action_diff_p1q0, G2, scale, NTH, eigvals, N, gen);
    cout << "thermalization acceptance rate: " << ar_th << endl;

    // simulation
    ofstream out_S;
    ofstream out_V;
    out_S.open("S.txt");
    out_V.open("V.txt");
    double ar = 0;
    for(int i=0; i<NMEAS; ++i)
    {
        ar += metropolis(action_p1q0, action_diff_p1q0, G2, scale, NSWP, eigvals, N, gen);
        out_S << action_p1q0(eigvals, N, G2) << endl;
        for(int j=0; j<N; ++j)
        {
            if(j==N-1)
                out_V << eigvals[j] << endl;
            else
                out_V << eigvals[j] << " ";
        }
    }

    cout << "simulation acceptance rate: " << ar/NMEAS << endl;

    return 0;
}
