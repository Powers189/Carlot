

#include "Random.h"

Random::Random(std::string filename)
{
    file.open(filename);
    if (file.is_open())
    {
        // fprintf(stdout, "opened file\n");
    }
    else
    {
        fprintf(stdout, "nuts\n");
    }
}

double Random::GetRandom()
{
    if (file.eof())
    {
        return -1;
        file.clear();
        file.seekg(0);
    }

    string line;
    getline(file, line);

    if (!isnumber(line))
        return -1;

    return stod(line);
}

bool Random::isOpen()
{
    return file.is_open();
}

bool Random::isnumber(const string &s)
{
    char *end = nullptr;
    double val = strtod(s.c_str(), &end);
    return end != s.c_str() && *end == '\0' && val != HUGE_VAL;
}