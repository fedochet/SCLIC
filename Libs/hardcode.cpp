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

vector<unsigned char> mergeVectors(vector<unsigned char> &first, vector<unsigned char> &second) {
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
}



