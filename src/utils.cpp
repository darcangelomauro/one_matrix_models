#include <sstream>
#include <iostream>
#include <algorithm>
#include <ctime>
#include "utils.hpp"

using namespace std;

string data_to_name(const string& geom, const int& N, const double& g2)
{
    string sN = to_string(N);
    
    ostringstream osg2;
    osg2 << g2;
    string sg2 = osg2.str();
    replace(sg2.begin(), sg2.end(), '.', 'd');
    replace(sg2.begin(), sg2.end(), '-', 'n');
    
    return geom + "N" + sN + "g" + sg2;
}

string cc_to_name(const double& g2)
{
    ostringstream osg2;
    osg2 << g2;
    string sg2 = osg2.str();
    replace(sg2.begin(), sg2.end(), '.', 'd');
    replace(sg2.begin(), sg2.end(), '-', 'n');
    
    return sg2;
}