#include <vector>
#include <iostream>

using namespace std;

unsigned char* longToChar(long int x);

vector<unsigned char> longToVector(long int x);

void printVector(vector<unsigned char> &v);

vector<unsigned char> vectorInversion(vector<unsigned char> &v);

//vector has to be 4 length
unsigned long vectorToLong(vector<unsigned char> &v);