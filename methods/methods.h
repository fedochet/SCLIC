#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <sys/time.h>
#include <openssl/bn.h>
#include <stdint.h>
#include <elf.h>
#include <stdint-gcc.h>
#include <crc32.h>

using namespace std;

void printVector(vector<unsigned char> v);
vector<unsigned char> vectorInversion(vector<unsigned char> v);

vector<unsigned char> intToVector(uint32_t x);
uint32_t vectorToInt(vector<unsigned char> v);
vector<unsigned char> longToVector(uint64_t x);

vector<unsigned char> hexStringToVector(string str);
string vectorToHexString(vector<unsigned char> vec);

vector<unsigned char> const operator+(vector<unsigned char> v1, vector<unsigned char> v2);
vector<unsigned char> generate_random(int len);

vector<unsigned char> getUnixTimestamp();