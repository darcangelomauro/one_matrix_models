#include "action.hpp"
#include <random>
#include <ctime>
#include <iostream>
#include <iomanip>

#define N 100
#define G2 -3
#define REP 10000
#define SCALE_S 100
#define SCALE_D 1

using namespace std;

int main()
{
    time_t seed = time(NULL);
    mt19937 gen(seed);
    uniform_real_distribution<double> dis(0., 1.);
    uniform_int_distribution<int> dis_int(0, N-1);

    for(int j=0; j<REP; ++j)
    {
        double v[N];
        for(int i=0; i<N; ++i)
            v[i] = SCALE_S*dis(gen);
        
        double S = action_p1q0(v, N, G2);
        double delta = SCALE_D*dis(gen);
        int k = dis_int(gen); 
        double dS_fast = action_diff_p1q0(v, N, G2, k, delta);
        v[k] += delta;
        double Sp = action_p1q0(v, N, G2);
        double dS_slow = Sp-S;

        cout << setprecision(16) << "fast dS: " << dS_fast << endl << "slow dS: " << dS_slow << endl << "S: " << S << endl;
        cout << setprecision(16) << "rel error: " << abs(dS_fast-dS_slow)/dS_fast << endl << endl;
    }

    return 0;
}