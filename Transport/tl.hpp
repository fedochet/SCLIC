#include "hardcode.hpp"
#include <vector>

struct tl_packet
{
    vector<unsigned char> auth_id;
    vector<unsigned char> timestamp;

    vector<unsigned char> length;
    vector<unsigned char> msg_id;   // as example 60 46 97 78 is req_pq

    vector<unsigned char> data;     //real data

    tl_packet();
    tl_packet(vector<unsigned char> v); //taking package from string
};