#include <elf.h>
#include <stdint-gcc.h>
#include <stdint.h>
#include "methods.h"

void printVector(vector<unsigned char> v) {
    for (int i=0; i<v.size(); i++) {

        if (i % 0x10 == 0)
            cout<<endl;

        if ((int) v[i] / 0x10 == 0)
            cout<<"0";

        cout << hex << (int) v[i]<<" ";
    }

    cout<<endl;
}

vector<unsigned char> vectorInversion(vector<unsigned char> v) {
    vector<unsigned char> result(v.size());

    for (int i = 0; i<v.size(); i++)
        result[(v.size()-1)-i] = v[i];

    return result;
}

vector<unsigned char> intToVector(uint32_t x) {
    vector<unsigned char> result(4);
    for(int i = result.size()-1; i>=0; i--){
        result[i] = x % 0x100;
        x = x / 0x100;
    }
    return result;
}

uint32_t vectorToInt(vector<unsigned char> v){
    uint32_t result = 0;
    unsigned long mult = 0x1000000;
    for(int i = 0; i<v.size(); i++)
    {
        result = result + (unsigned long)v[i] * mult;
        mult = mult / 0x100;
    }

    return result;
}

vector<unsigned char> longToVector(uint64_t x) {
    vector<unsigned char> result(8);
    for(int i = result.size()-1; i>=0; i--){
        result[i] = x % 0x100;
        x = x / 0x100;
    }
    return result;
}

vector<unsigned char> hexStringToVector(string str) {
    vector<unsigned char> result;
    unsigned int temp;
    stringstream ss;
    for (int i = 0; i < str.size(); i+=2) {
        ss.clear();
        ss << hex << str.substr(i, 2);
        ss >> temp;
        result.push_back(static_cast<unsigned char>(temp));
    }
    return result;
}

string vectorToHexString(vector<unsigned char> vec) {
    string result;
    string temp;
    stringstream ss;
    for (int i = 0; i < vec.size(); i++){
        ss.clear();
        if ((int)vec[i]/0x10 == 0)
            ss << "0";
        ss << hex << (int)vec[i];
        ss >> temp;
        result+=temp;
    }
    return result;
}

vector<unsigned char> const operator+(vector<unsigned char> v1, vector<unsigned char> v2) {
    vector<unsigned char> result = v1;
    for(auto i = v2.begin(); i!=v2.end(); i++)
        result.push_back(*i);
    return result;
};

vector<unsigned char> generate_random(int len)
{
    BIGNUM * bn = BN_new();
    BN_rand(bn, len, 0x00, 0xFF);
    string convert = BN_bn2hex(bn);
    free(bn);

    return hexStringToVector(convert);
}

vector<unsigned char> getUnixTimestamp() {

    timeval t;
    gettimeofday(&t, 0);
    return longToVector(t.tv_sec * 0x100000000 + t.tv_usec * 0x1000);
}