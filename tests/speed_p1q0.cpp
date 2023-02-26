#include "action.hpp"
#include <random>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <chrono>

#define N 10000
#define G2 -3
#define REP 10
#define SCALE 1

using namespace std;
using namespace std::chrono;

int main()
{
    time_t seed = time(NULL);
    mt19937 gen(seed);
    uniform_real_distribution<double> dis(0., 1.);
    uniform_int_distribution<int> dis_int(0, N-1);

    auto start_slow = high_resolution_clock::now();
    for(int j=0; j<REP; ++j)
    {
        double v[N];
        for(int i=0; i<N; ++i)
            v[i] = SCALE*dis(gen);
        
        double S = action_p1q0(v, N, G2);
        int k = dis_int(gen); 
        double delta = SCALE*dis(gen);
        v[k] += delta;
        double Sp = action_p1q0(v, N, G2);
        double dS_slow = Sp-S;
    }
    auto end_slow = high_resolution_clock::now();
    
    auto start_fast = high_resolution_clock::now();
    for(int j=0; j<REP; ++j)
    {
        double v[N];
        for(int i=0; i<N; ++i)
            v[i] = SCALE*dis(gen);
        
        int k = dis_int(gen); 
        double delta = SCALE*dis(gen);
        double dS_fast = action_diff_p1q0(v, N, G2, k, delta);
    }
    auto end_fast = high_resolution_clock::now();

    auto duration_slow = duration_cast<milliseconds>(end_slow - start_slow);
    auto duration_fast = duration_cast<milliseconds>(end_fast - start_fast);

    cout << "slow: " << duration_slow.count() << " millisec" << endl; 
    cout << "fast: " << duration_fast.count() << " millisec" << endl; 

    return 0;
}