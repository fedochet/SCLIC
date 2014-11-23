#include "hardcode.hpp"

unsigned char* longToChar(long int x)
{
    unsigned char *result = new unsigned char[8];

    for (int i = 0; i<8; i++)
    {
        result[7-i] = x % 0x10;
        cout<<(int)result[7-i]<<endl;
        x = x / 0x10;
    }

    return result;
}

//hex print of
void printVector(vector<unsigned char> &v)
{
    for (int i=0; i<v.size(); i++)
        cout<<hex<<(int)v[i]<<" ";

    cout<<endl;
}

//vector inverter
vector<unsigned char> vectorInversion(vector<unsigned char> &v)
{
    vector<unsigned char> result(v.size());

    for (int i = 0; i<v.size(); i++)
        result[(v.size()-1)-i] = v[i];

    return result;
}

unsigned long vectorToLong(vector<unsigned char> &v) {
    unsigned long result = 0;
    unsigned long mult = 0x100000000000000;
    for(int i = 0; i<v.size(); i++)
    {
        result = result + (unsigned long)v[i] * mult;
        mult = mult / 0x100;
    }

    return result;
}

vector<unsigned char> longToVector(unsigned long x) {
    vector<unsigned char> result(8);

    for(int i = 0; i<result.size(); i++)
    {
        result[i] = x % 0x100;
        x = x / 100;
    }
}
