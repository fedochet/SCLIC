#include <vector>
#include <iostream>
#include "crc32.h"

#include <sstream>
#include <ctime>
#include <openssl/bn.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <sys/time.h>

#define TCP_SIZE 4
#define TCP_PKT_NUM 4


using namespace std;

string vectorToHexString(vector<unsigned char> vec);
vector<unsigned char> hexStringToVector(string& str);
vector<unsigned char> generate_random(int len);

unsigned char* longToChar(long int x);

vector<unsigned char> longToVector(long x);

void printVector(vector<unsigned char> v);

vector<unsigned char> getUnixTimestamp();

vector<unsigned char> vectorInversion(vector<unsigned char> v);

//vector has to be 4 length
unsigned long vectorToLong(vector<unsigned char> v);
unsigned int vectorToInt(vector<unsigned char> v);

vector<unsigned char> longToVector(unsigned long x);
vector<unsigned char> intToVector(unsigned int x);

vector<unsigned char> mergeVectors(vector<unsigned char> first, vector<unsigned char> second);

//TCP-packing
void makePacket(vector<unsigned char> &tl_packet, vector<unsigned char> &result);

vector<unsigned char> substring(vector<unsigned char>, int, int);

vector<unsigned char> SHA1V(vector<unsigned char>, vector<unsigned char>);
