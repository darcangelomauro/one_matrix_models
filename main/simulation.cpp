#include "action.hpp"
#include "metropolis.hpp"
#include "parameters.hpp"
#include "utils.hpp"
#include <random>
#include <ctime>
#include <iostream>
#include <fstream>
#include <functional>


using namespace std;

int main(int argc, char** argv)
{
    // ARGUMENT LIST:
    // The first argument is the seed.
    // The second argument is the g2 value.
    // The third argument is the path to the work directory
    if(argc != 4)
    {
        cerr << "Error: need to pass seed, g2 value, and path to work directory as arguments." << endl;
        return 0;
    }
    // First argument (seed) is converted to unsigned long, the second (g2) to double.
    unsigned long seed = stoul(argv[1]);
    double g2 = stod(argv[2]);


    // STRING OPERATIONS FOR FILE NAMES
    string path_lvl0 = argv[3];
    string path_lvl1 = path_lvl0 + "/" + cc_to_name(g2);
    string init_filename = path_lvl0 + "/init.txt";


    // RNG:
    // Initialize random number generator
    mt19937 gen(seed);
    uniform_real_distribution<double> dis_real(0., 1.);
    clog << "RNG seed: " << seed << endl << endl;


    //********* BEGIN PARAMETER INITIALIZATION *******
    struct Simul_params sm;
    ifstream in_init;

    // Check that parameter reading is successful
    in_init.open(init_filename);
    if(!read_init_stream(in_init, sm))
    {
        cerr << "Error: couldn't read file " + init_filename << endl;
        return 1;
    }
    in_init.close();

    // Log what has been read
    clog << "File " + init_filename + " contains the following parameters:" << endl;
    clog << sm.control << endl;

    // Check that parameters are consistent with simulation type
    if(!params_validity(sm))
    {
        cerr << "Error: file " + init_filename + " does not contain the necessary parameters." << endl;
        return 1;
    }
    //********* END PARAMETER INITIALIZATION **********



    //********* BEGIN MONTE CARLO **********

    // ASSIGN FUNCTION FOR ACTION AND ACTION DIFFERENCE
    function<double(double*, const int&, const double&)> action_function;
    function<double(double*, const int&, const double&, const int&, const double&)> action_difference;
    if(sm.GEOM == "p1q0")
    {
        action_function = action_p1q0;
        action_difference = action_diff_p1q0;
    }
    else if(sm.GEOM == "p0q1")
    {
        action_function = action_p0q1;
        action_difference = action_diff_p0q1;
    }
    else
    {
        cerr << "Error: geometry " + sm.GEOM + " is not recognized" << endl;
        return 1;
    }

    // OPEN OUTPUT FILES
    ofstream out_S, out_V;
    string s_filename = path_lvl1 + "/" + data_to_name(sm.GEOM, sm.N, g2) + "_S.txt";
    string v_filename = path_lvl1 + "/" + data_to_name(sm.GEOM, sm.N, g2) + "_V.txt";
    out_S.open(s_filename);
    out_V.open(v_filename);


    // RANDOM INITIALIZATION
    double* eigvals = new double[sm.N]; 
    for(int i=0; i<sm.N; ++i)
        eigvals[i] = dis_real(gen);
    
    double scale = 0.01;

    // Start dual averaging
    clog << "Duav averaging start timestamp: " << time(NULL) << endl;
    scale = metropolis_scale_tuning(action_function, action_difference, g2, scale, sm.TGTAR, sm.NDA, eigvals, sm.N, gen);
    clog << "tuned scale: " << scale << endl;
    clog << "Dual averaging end timestamp: " << time(NULL) << endl << endl;

    // Start thermalization
    clog << "Thermalization start timestamp: " << time(NULL) << endl;
    double ar_th = metropolis(action_function, action_difference, g2, scale, sm.NTH, eigvals, sm.N, gen);
    clog << "thermalization acceptance rate: " << ar_th << endl;
    clog << "Thermalization end timestamp: " << time(NULL) << endl << endl;

    // Start simulation
    clog << "Simulation start timestamp: " << time(NULL) << endl;
    double ar = 0;
    for(int i=0; i<sm.NMEAS; ++i)
    {
        ar += metropolis(action_function, action_difference, g2, scale, sm.NSWP, eigvals, sm.N, gen);
        out_S << action_function(eigvals, sm.N, g2) << endl;
        for(int j=0; j<sm.N; ++j)
        {
            if(j==sm.N-1)
                out_V << eigvals[j] << endl;
            else
                out_V << eigvals[j] << " ";
        }
    }
    clog << "Acceptance rate: " << ar/sm.NMEAS << endl;
    clog << "Simulation end timestamp: " << time(NULL) << endl;

    // Close files
    out_S.close();
    out_V.close();

    //********* END MONTE CARLO **********


    // Free memory
    delete [] eigvals;

    return 0;
}
