

#ifndef ELEVATORUP_RANDOM_H
#define ELEVATORUP_RANDOM_H


#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;
class Random {
public:
    Random(string fileName);
    double GetRandom();
    bool isOpen();
private:
    ifstream file;
    bool isnumber( const string & s);
};


#endif //ELEVATORUP_RANDOM_H
