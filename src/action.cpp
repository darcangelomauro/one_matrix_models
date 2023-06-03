#include "action.hpp"
#include <cmath>
#include <limits>

using namespace std;

double action_p1q0(double* eigvals, const int& n, const double& g2)
{
    double S = 0;

    for(int i=0; i<n; ++i)
    {
        double eig2_i = eigvals[i]*eigvals[i]; 
        S += g2*(n+1)*eig2_i + (n+7)*eig2_i*eig2_i;

        for(int j=i+1; j<n; ++j)
        {
            double diff = abs(eigvals[i]-eigvals[j]);
            if(diff < 1e-10)
                return numeric_limits<double>::max();
            
            double eig2_j = eigvals[j]*eigvals[j]; 
            S += 2*g2*eigvals[i]*eigvals[j] + 4*(eigvals[i]*eigvals[j]*eig2_j + eigvals[j]*eigvals[i]*eig2_i) + 6*(eig2_i*eig2_j) -log(diff);
        }
    }

    return 2*S;
}

double action_p1q0_bruteforce(double* eigvals, const int& n, const double& g2)
{
    double e = 0;
    double e2 = 0;
    double e3 = 0;
    double e4 = 0;
    double vdm = 0;

    for(int i=0; i<n; ++i)
    {
        e += eigvals[i];
        e2 += eigvals[i]*eigvals[i];
        e3 += eigvals[i]*eigvals[i]*eigvals[i];
        e4 += eigvals[i]*eigvals[i]*eigvals[i]*eigvals[i];
        for(int j=i+1; j<n; ++j)
        {
            vdm += -2*log(abs(eigvals[i]-eigvals[j]));
        }
    }

    return 2*n*(g2*e2 + e4) + 2*g2*e*e + 8*e*e3 + 6*e2*e2 + vdm;
}

double action_p0q1(double* eigvals, const int& n, const double& g2)
{
    double S = 0;

    for(int i=0; i<n; ++i)
    {
        double eig2_i = eigvals[i]*eigvals[i]; 
        S += (n-1)*(g2*eig2_i + eig2_i*eig2_i);

        for(int j=i+1; j<n; ++j)
        {
            double diff = abs(eigvals[i]-eigvals[j]);
            if(diff < 1e-10)
                return numeric_limits<double>::max();
            
            double eig2_j = eigvals[j]*eigvals[j]; 
            S += -2*g2*eigvals[i]*eigvals[j] - 4*(eigvals[i]*eigvals[j]*eig2_j + eigvals[j]*eigvals[i]*eig2_i) + 6*(eig2_i*eig2_j) -log(diff);
        }
    }

    return 2*S;
}

double action_diff_p1q0(double* eigvals, const int& n, const double& g2, const int& k, const double& delta)
{
    double delta2 = delta*delta;
    double delta3 = delta2*delta;
    double eig2_k = eigvals[k]*eigvals[k];

    double diff2 = 2*eigvals[k]*delta + delta2;
    double diff3 = 3*(eig2_k*delta + eigvals[k]*delta2) + delta3;
    double diff4 = 4*(eig2_k*eigvals[k]*delta + eigvals[k]*delta3) + 6*eig2_k*delta2 + delta2*delta2;

    double dS = g2*(n+1)*diff2 + (n+7)*diff4;

    for(int i=0; i<n; ++i)
    {
        if(i!=k)
        {
            double diff_ik = abs(eigvals[k]-eigvals[i]);
            if(diff_ik < 1e-10)
                return -numeric_limits<double>::max();
            
            double diff_ikp = abs(eigvals[k]+delta-eigvals[i]);
            if(diff_ikp < 1e-10)
                return numeric_limits<double>::max();
            
            double eig2_i = eigvals[i]*eigvals[i]; 
            dS += log(diff_ik) - log(diff_ikp);
            dS += 2*g2*delta*eigvals[i] + 4*(delta*eigvals[i]*eig2_i + eigvals[i]*diff3) + 6*(diff2*eig2_i);
        }
    }

    return 2*dS;
}

double action_diff_p1q0_bruteforce(double* eigvals, const int& n, const double& g2, const int& k, const double& delta)
{
    double S1 = action_p1q0(eigvals, n, g2);
    double* temp_eigvals = new double[n];
    for(int i=0; i<n; ++i)
        temp_eigvals[i] = eigvals[i];
    temp_eigvals[k] += delta; 
    double S2 = action_p1q0(temp_eigvals, n, g2);
    delete [] temp_eigvals;
    return S2-S1;
}

double action_diff_p0q1(double* eigvals, const int& n, const double& g2, const int& k, const double& delta)
{
    double delta2 = delta*delta;
    double delta3 = delta2*delta;
    double eig2_k = eigvals[k]*eigvals[k];

    double diff2 = 2*eigvals[k]*delta + delta2;
    double diff3 = 3*(eig2_k*delta + eigvals[k]*delta2) + delta3;
    double diff4 = 4*(eig2_k*eigvals[k]*delta + eigvals[k]*delta3) + 6*eig2_k*delta2 + delta2*delta2;

    double dS = (n-1)*(g2*diff2 + diff4);

    for(int i=0; i<n; ++i)
    {
        if(i!=k)
        {
            double diff_ik = abs(eigvals[k]-eigvals[i]);
            if(diff_ik < 1e-10)
                return -numeric_limits<double>::max();
            
            double diff_ikp = abs(eigvals[k]+delta-eigvals[i]);
            if(diff_ikp < 1e-10)
                return numeric_limits<double>::max();
            
            double eig2_i = eigvals[i]*eigvals[i]; 
            dS += log(diff_ik) - log(diff_ikp);
            dS += -2*g2*delta*eigvals[i] - 4*(delta*eigvals[i]*eig2_i + eigvals[i]*diff3) + 6*(diff2*eig2_i);
        }
    }

    return 2*dS;
}
