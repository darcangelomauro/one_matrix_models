#include "action.hpp"
#include <random>
#include <ctime>
#include <iostream>
#include <iomanip>

#define N 1024
#define G2 -3
#define REP 10000
#define SCALE_S 1

using namespace std;

int main()
{
    time_t seed = time(NULL);
    mt19937 gen(seed);
    uniform_real_distribution<double> dis(0., 1.);

    for(int j=0; j<REP; ++j)
    {
        double v[N];
        for(int i=0; i<N; ++i)
            v[i] = SCALE_S*(2*dis(gen)-1);
        
        double S1 = action_p1q0(v, N, G2);
        double S2 = action_p1q0_bruteforce(v, N, G2);

        if(abs(S1-S2) > 1e-8)
        {
            cout << setprecision(16) << "fast S: " << S1 << endl << "bruteforce S: " << S2 << endl;
            cout << setprecision(16) << "rel error: " << abs(S1-S2)/S1 << endl;
            cout << setprecision(16) << "abs error: " << abs(S1-S2) << endl << endl;
        }
    }

    return 0;
}