#include "hardcode.hpp"
#include <vector>

#include <openssl/sha.h>

#include "assets.hpp"

struct tl_packet
{
    vector<unsigned char> auth_id;
    vector<unsigned char> timestamp;

    vector<unsigned char> length;

    static const int msg_size = 4;
    vector<unsigned char> msg_id;   // as example 60 46 97 78 is req_pq_packet

    vector<unsigned char> data;     //real data

    tl_packet();
    tl_packet(vector<unsigned char> v); //taking package from string

    vector<unsigned char> to_vector();
};

struct req_pq_packet :tl_packet
{
    //individual fields
    vector<unsigned char> nonce;
    static const int nonce_size = 16;

    req_pq_packet();
};

struct res_pq_packet :tl_packet
{

    vector<unsigned char> nonce;
    static const int nonce_size = 16;

    vector<unsigned char> server_nonce;
    static const int server_nonce_size = 16;

    vector<unsigned char> pq;
    static const int pq_size = 8; //need to be defined

    vector<unsigned char> fingerprints_count;
    static const int fingerprints_count_size = 4;

    vector<unsigned char> fingerprints;
    static const int fingerprints_size = 8;

    long get_pq();

    vector<unsigned char> get_server_nonce();
    vector<unsigned char> get_nonce();
    vector<unsigned char> get_fingerprints();

    res_pq_packet(vector<unsigned char> v);
};

struct p_q_inner_data_packet:tl_packet
{
    vector<unsigned char> pq;
    static const int pq_size = 8; //need to be defined

    vector<unsigned char> p;
    static const int p_size = 4; //need to be defined

    vector<unsigned char> q;
    static const int q_size = 4; //need to be defined

    vector<unsigned char> nonce;
    static const int nonce_size = 16;

    vector<unsigned char> server_nonce;
    static const int server_nonce_size = 16;

    vector<unsigned char> data_with_hash;
    static const int data_with_hash_size = 255;

    vector<unsigned char> encrypted_data;
    static const int encrypted_data_size = 256; //why blyat why

    vector<unsigned char> new_nonce;
    static const int new_nonce_size = 32;

    vector<unsigned char> get_new_nonce();

    p_q_inner_data_packet(
            vector<unsigned char> nonce,
            vector<unsigned char> server_nonce,
            long pq,
            long p,
            long q);

    vector<unsigned char> get_encrypted_data();

};

struct req_DH_params_packet:tl_packet {
    vector<unsigned char> nonce;
    static const int nonce_size = 16;

    vector<unsigned char> server_nonce;
    static const int server_nonce_size = 16;

    vector<unsigned char> p;
    static const int p_size = 4; //need to be defined

    vector<unsigned char> q;
    static const int q_size = 4; //need to be defined

    vector<unsigned char> fingerprints;
    static const int fingerprints_size = 8;

    vector<unsigned char> encrypted_data; // data to be generated in constructor
    static const int encrytped_data_size = 256; // need to be 260 cause we need to add length bytes

    req_DH_params_packet(
            vector<unsigned char> nonce_param,
            vector<unsigned char> server_nonce_param,
            vector<unsigned char> fingerprints_param,
            vector<unsigned char> encrypted_data_param,
            long p_param,
            long q_param);
};