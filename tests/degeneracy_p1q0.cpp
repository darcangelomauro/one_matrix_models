#include "action.hpp"
#include <random>
#include <ctime>
#include <iostream>
#include <iomanip>

#define N 100
#define G2 -3
#define REP 10000
#define SCALE 1

using namespace std;

int main()
{
    double v[N];
    for(int i=0; i<N; ++i)
        v[i] = i;

    v[0] = 1;
    double delta = 0.001;
    double S = action_p1q0(v, N, G2);
    cout << "Degenerate action: " << S << endl;
    double dS = action_diff_p1q0(v, N, G2, 0, delta);
    cout << "Action difference: " << dS << endl;
    v[0] += delta;
    double Sp = action_p1q0(v, N, G2);
    cout << "Non-degenerate action: " << Sp << endl;
    
    return 0;
}