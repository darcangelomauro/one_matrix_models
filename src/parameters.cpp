#include <iostream>
#include <string>
#include "parameters.hpp"

using namespace std;

bool read_init_stream(istream& in, struct Simul_params& sm)
{
    bool success = false;
    if(in)
    {
        string temp;
        
        while(in >> temp)
        {
            if(temp == "GEOM:")
            {
                sm.control += temp;
                in >> sm.GEOM;
            }
            else if(temp == "N:")
            {
                sm.control += temp;
                in >> sm.N;
            }
            else if(temp == "TGTAR:")
            {
                sm.control += temp;
                in >> sm.TGTAR;
            }
            else if(temp == "NDA:")
            {
                sm.control += temp;
                in >> sm.NDA;
            }
            else if(temp == "NTH:")
            {
                sm.control += temp;
                in >> sm.NTH;
            }
            else if(temp == "NSWP:")
            {
                sm.control += temp;
                in >> sm.NSWP;
            }
            else if(temp == "NMEAS:")
            {
                sm.control += temp;
                in >> sm.NMEAS;
            }
        }

        success = true;

    }

    return success;
}

bool params_validity(const struct Simul_params& sm)
{
    if(sm.control.find("GEOM") == std::string::npos)
    {
        cerr << "geometry not found" << endl;
        return 0;
    }
    if(sm.control.find("N:") == std::string::npos)
    {
        cerr << "N not found" << endl;
        return 0;
    }
    
    if(sm.control.find("NDA:") == std::string::npos)
    {
        cerr << "NDA not found" << endl;
        return 0;
    }
    if(sm.control.find("NTH:") == std::string::npos)
    {
        cerr << "NTH not found" << endl;
        return 0;
    }
    if(sm.control.find("NSWP:") == std::string::npos)
    {
        cerr << "NSWP not found" << endl;
        return 0;
    }

    if(sm.control.find("TGTAR:") == std::string::npos)
    {
        cerr << "TGTAR not found" << endl;
        return 0;
    }

    return 1;
}