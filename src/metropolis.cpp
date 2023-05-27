#include "action.hpp"
#include "metropolis.hpp"
#include <cmath>

using namespace std;

double metropolis(
    function<double(double*, const int&, const double&)> action_function,
    function<double(double*, const int&, const double&, const int&, const double&)> action_difference_function,
    const double& g2,
    const double& scale,
    const int& Nsweeps,
    double* eigvals,
    const int& n,
    mt19937& gen
)
{
    // random distributions
    uniform_real_distribution<double> dis_real(0., 1.);
    uniform_int_distribution<int> dis_int(0, n-1);

    // acceptance rate
    double ar = 0;

    // initial value of the action
    double S = action_function(eigvals, n, g2);

    // sweep the system Nsweeps times
    for(int i=0; i<Nsweeps; ++i)
    {
        // each sweep consists of n attempted moves
        for(int j=0; j<n; ++j)
        {
            int k = dis_int(gen); 
            double delta = scale*(2*dis_real(gen)-1);
            double dS = action_difference_function(eigvals, n, g2, k, delta);
    
            if(dS<0)
            {
                // update eigenvalues
                eigvals[k] += delta;
                
                // update action
                S += dS;

                // update acceptance rate
                ar += 1;
            }

            else
            {
                double e = exp(-dS);
                double p = dis_real(gen);

                if(e>p)
                {
                    // update eigenvalues
                    eigvals[k] += delta;
                    
                    // update action
                    S += dS;

                    // update acceptance rate
                    ar += 1;
                }
            }
        }
    }
    
    return ar/(Nsweeps*n);
}

double metropolis_scale_tuning(
    function<double(double*, const int&, const double&)> action_function,
    function<double(double*, const int&, const double&, const int&, const double&)> action_difference_function,
    const double& g2,
    const double& initial_scale,
    const double& target_ar,
    const int& Niters,
    double* eigvals,
    const int& n,
    mt19937& gen
)
{
    // random distributions
    uniform_real_distribution<double> dis_real(0., 1.);
    uniform_int_distribution<int> dis_int(0, n-1);
    
    // statistic to minimize
    double stat = 0;

    // scale factor
    double scale = initial_scale;

    // dual averaging variables
    const double shr = 0.05;
    const double kappa = 0.75;
    const int i0 = 10;
    double mu = log(10*scale);
    double log_scale_avg = log(scale);


    // initial value of the action
    double S = action_function(eigvals, n, g2);

    // propose Niters moves
    for(int i=0; i<Niters; ++i)
    {
        int k = dis_int(gen); 
        double delta = scale*(2*dis_real(gen)-1);
        double dS = action_difference_function(eigvals, n, g2, k, delta);

        if(dS<0)
        {
            // update eigenvalues
            eigvals[k] += delta;
            
            // update action
            S += dS;

            // update statistic
            stat += target_ar - 1;
        }

        else
        {
            double e = exp(-dS);
            double p = dis_real(gen);

            if(e>p)
            {
                // update eigenvalues
                eigvals[k] += delta;
                
                // update action
                S += dS;
            }
            
            // update statistic
            stat += target_ar - e;
        }

        // perform dual averaging
        double log_scale = mu - stat*sqrt(i+1)/(shr*(i+1+i0));
        scale = exp(log_scale);
        double eta = pow(i+1, -kappa);
        log_scale_avg = eta*log_scale + (1-eta)*log_scale_avg;
    }
    
    return scale;
}
