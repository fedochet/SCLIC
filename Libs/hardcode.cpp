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
void printVector(vector<unsigned char> v)
{
    for (int i=0; i<v.size(); i++) {
        
        if (i % 0x10 == 0)
            cout<<endl;
        
        if ((int) v[i] / 0x10 == 0)
            cout<<"0";
        
        cout << hex << (int) v[i]<<" ";
    }
    
    cout<<endl;
}

vector<unsigned char> getUnixTimestamp() {
    
    timeval t;
    gettimeofday(&t, 0);
    cout<<t.tv_sec * 0x100000000 + t.tv_usec * 0x100<<endl;
    
    return longToVector(t.tv_sec * 0x100000000 + t.tv_usec * 0x1000);
    cout<<time(0)<<endl;
}

//vector inverter
vector<unsigned char> vectorInversion(vector<unsigned char> v)
{
    vector<unsigned char> result(v.size());
    
    for (int i = 0; i<v.size(); i++)
        result[(v.size()-1)-i] = v[i];
    
    return result;
}

unsigned long vectorToLong(vector<unsigned char> v) {
    unsigned long result = 0;
    unsigned long mult = 0x100000000000000;
    for(int i = 0; i<v.size(); i++)
    {
        result = result + (unsigned long)v[i] * mult;
        mult = mult / 0x100;
    }
    
    return result;
}

unsigned int vectorToInt(vector<unsigned char> v){
    unsigned long result = 0;
    unsigned long mult = 0x1000000;
    for(int i = 0; i<v.size(); i++)
    {
        result = result + (unsigned long)v[i] * mult;
        mult = mult / 0x100;
    }
    
    return result;
}

vector<unsigned char> longToVector(long x) {
    vector<unsigned char> result(8);
    
    for(int i = result.size()-1; i>=0; i--)
    {
        result[i] = x % 0x100;
        x = x / 0x100;
    }
    
    return result;
}

vector<unsigned char> intToVector(unsigned int x) {
    vector<unsigned char> result(4);
    
    
    for(int i = result.size()-1; i>=0; i--)
    {
        result[i] = x % 0x100;
        x = x / 0x100;
    }
    
    return result;
}

vector<unsigned char> mergeVectors(vector<unsigned char> first, vector<unsigned char> second) {
    vector<unsigned char> result(first.size() + second.size());
    
    for (int i = 0; i<result.size(); i++)
    {
        if (i < first.size())
            result[i] = first[i];
        else
            result[i] = second[i - first.size()];
    }
    
    return result;
}
/*
void makePacket(vector<unsigned char> &tl_packet, vector<unsigned char> &result)
{
    // size of package
    vector<unsigned char> tl_header(4);
    vector<unsigned char> crc32_v(4);
    
    vector<unsigned char> packet_num(4,0);
    
    tl_header = intToVector(TCP_SIZE+TCP_PKT_NUM+tl_packet.size()+ CRC_SIZE);
    
    tl_header = mergeVectors(tl_header, packet_num);
    result = mergeVectors(tl_header, tl_packet);
    
    
    long crc32 = compute_crc32(&result[0], result.size());
    
    crc32_v = intToVector(crc32);
    printVector(crc32_v);
    
    result = mergeVectors(result, crc32_v);
}*/


vector<unsigned char> hexStringToVector(string &str)
{
    vector<unsigned char> result;
    unsigned int temp;
    stringstream ss;
    for (int i = 0; i < str.size(); i+=2)
    {
        ss.clear();
        ss << hex << str.substr(i, 2);
        ss >> temp;
        result.push_back(static_cast<unsigned char>(temp));
    }
    
    return result;
}

string vectorToHexString(vector<unsigned char> vec)
{
    string result;
    string temp;
    stringstream ss;
    for (int i = 0; i < vec.size(); i++)
    {
        ss.clear();
        if ((int)vec[i]/0x10 == 0)
            ss << "0";
        ss << hex << (int)vec[i];
        ss >> temp;
        result+=temp;
    }
    return result;
}

vector<unsigned char> generate_random(int len)
{
    BIGNUM * bn = BN_new();
    BN_rand(bn, len, 0x00, 0xFF);
    string convert = BN_bn2hex(bn);
    free(bn);
    
    return hexStringToVector(convert);
}

vector<unsigned char> substring(vector<unsigned char> src, int start, int len)
{
    vector<unsigned char> res(len);
    for (int i = start; i < len+start; ++i) {
        res[i-start] = src[i];
    }
    return res;
}

vector<unsigned char> SHA1V(vector<unsigned char> v1, vector<unsigned char> v2)
{
    vector<unsigned char> result_hash(20,0);
    vector<unsigned char> pass_to_sha1 = mergeVectors(v1, v2);
    
    SHA1(&pass_to_sha1[0], pass_to_sha1.size(), &result_hash[0]);
    
    return result_hash;
}
