#include "tl.hpp"

tl_packet::tl_packet(){};

tl_packet::tl_packet(vector<unsigned char> v) {

    int read_position = 0;

    auth_id.clear();
    auth_id.resize(8);

    auth_id = vector<unsigned char> (&v[read_position], &v[read_position + auth_id.size()]);

    read_position += auth_id.size();

    timestamp.clear();
    timestamp.resize(8);

    timestamp = vector<unsigned char> (&v[read_position], &v[read_position + timestamp.size()]);
    vector<unsigned char> time_v = getUnixTimestamp();

    read_position += timestamp.size();

    length.clear();
    length.resize(4);

    length = vector<unsigned char> (&v[read_position], &v[read_position + length.size()]);

    read_position += length.size();
    int data_size = vectorToInt(vectorInversion(length)) - 4; // because of msg id

    msg_id.clear();
    msg_id.resize(4);

    msg_id = vector <unsigned char> (&v[read_position], &v[read_position + msg_id.size()]);

    read_position += msg_id.size();

    data.clear();
    data.resize(data_size);

    data = vector<unsigned char> (&v[read_position], &v[read_position + data.size()]);
}

req_pq_packet::req_pq_packet() {

    auth_id = vector<unsigned char> (8, 0);

    timestamp = vectorInversion(getUnixTimestamp());

    length = vectorInversion(intToVector(20));

    msg_id = vectorInversion(intToVector(0x60469778));

    nonce = generate_random(128);

    data = nonce;
}

vector<unsigned char> tl_packet::to_vector() {
    vector<unsigned char> result;
    result = mergeVectors(auth_id, timestamp);
    result = mergeVectors(result, length);
    result = mergeVectors(result, msg_id);
    result = mergeVectors(result, data);

    return result;
}

res_pq_packet::res_pq_packet(vector<unsigned char> v) {
    int read_position = 0;

    auth_id.clear();
    auth_id.resize(8);

    auth_id = vector<unsigned char> (&v[read_position], &v[read_position + auth_id.size()]);
    printVector(auth_id);

    read_position += auth_id.size();

    timestamp.clear();
    timestamp.resize(8);

    timestamp = vector<unsigned char> (&v[read_position], &v[read_position + timestamp.size()]);
    vector<unsigned char> time_v = getUnixTimestamp();

    read_position += timestamp.size();

    length.clear();
    length.resize(4);

    length = vector<unsigned char> (&v[read_position], &v[read_position + length.size()]);
    printVector(length);

    read_position += length.size();
    int data_size = vectorToInt(vectorInversion(length)) - 4; // because of msg id

    msg_id.clear();
    msg_id.resize(4);

    msg_id = vector <unsigned char> (&v[read_position], &v[read_position + msg_id.size()]);
    printVector(msg_id);

    read_position += msg_id.size();

    data = vector<unsigned char> (&v[read_position], &v[v.size()]);

    nonce = vector<unsigned char> (&v[read_position], &v[read_position + nonce_size]);
    printVector(nonce);

    read_position += nonce.size();

    server_nonce = vector<unsigned char>(&v[read_position], &v[read_position + server_nonce_size]);
    printVector(server_nonce);

    read_position += server_nonce.size();

    read_position++;// byte of pq length (should be read later)

    // big endian format
    pq = vector<unsigned char> (&v[read_position], &v[read_position + pq_size]);
    printVector(pq);

    read_position += pq.size();

    read_position += 3; // padding (to 4 deviding P.S. nothing do here)
    read_position += 4; // vector constructor id fucking shit
    fingerprints_count = vector<unsigned char> (&v[read_position], &v[read_position + fingerprints_count_size]);
    printVector(fingerprints_count);

    read_position += fingerprints_count.size();

    fingerprints = vector<unsigned char> (&v[read_position], &v[read_position + fingerprints_size]);
    cout << "Fingerprints: ";
    printVector(fingerprints);

    read_position += fingerprints.size();
}

long res_pq_packet::get_pq() {
    return vectorToLong(pq);
}

vector<unsigned char> res_pq_packet::get_server_nonce() {
    return server_nonce;
}

vector<unsigned char> res_pq_packet::get_nonce() {
    return nonce;
}

req_DH_params_packet::req_DH_params_packet(
        vector<unsigned char> nonce_param,
        vector<unsigned char> server_nonce_param,
        vector<unsigned char> fingerprints_param,
        vector<unsigned char> encrypted_data_param,
        long p_param,
        long q_param)
{
    p = intToVector(p_param);
    q = intToVector(q_param);

    auth_id = vector<unsigned char> (8, 0);
    printVector(auth_id);

    timestamp = vectorInversion(getUnixTimestamp());
    printVector(timestamp);

    length = vectorInversion(intToVector(320));
    printVector(length);

    msg_id = vectorInversion(intToVector(0xd712e4be));
    printVector(msg_id);

    cout<<"nonce is:"<<endl;
    nonce = nonce_param;
    data = nonce;
    printVector(nonce);

    cout<<endl<<"data now is:"<<endl;
    printVector(data);


    cout<<"server_nonce is:"<<endl;
    server_nonce = server_nonce_param;
    data = mergeVectors(data, server_nonce);
    printVector(server_nonce);


    // p and q inserting with padding
    data.push_back(0x04);
    data = mergeVectors(data, p);
    data.push_back(0x0);
    data.push_back(0x0);
    data.push_back(0x0);

    data.push_back(0x04);
    data = mergeVectors(data, q);
    data.push_back(0x0);
    data.push_back(0x0);
    data.push_back(0x0);

    fingerprints = fingerprints_param;
    printVector(fingerprints);
    data = mergeVectors(data, fingerprints);

    data.push_back(0xfe);
    data.push_back(0x00);
    data.push_back(0x01);
    data.push_back(0x00);
    //data.push_back(0x0);
    encrypted_data = encrypted_data_param;
    data = mergeVectors(data, encrypted_data);
}

vector<unsigned char> p_q_inner_data_packet::get_new_nonce() {
    return new_nonce;
}

p_q_inner_data_packet::p_q_inner_data_packet(
        vector<unsigned char> nonce_param,
        vector<unsigned char> server_nonce_param,
        long pq_param,
        long p_param,
        long q_param)
{
    msg_id = vectorInversion(intToVector(0x83c95aec));
    nonce = nonce_param;
    server_nonce = server_nonce_param;
    pq = longToVector(pq_param);
    p = intToVector(p_param);
    q = intToVector(q_param);
    new_nonce = generate_random(256);

    data = msg_id;

    data.push_back(0x08);
    data = mergeVectors(data, pq);
    data.push_back(0x0);
    data.push_back(0x0);
    data.push_back(0x0);

    data.push_back(0x04);
    data = mergeVectors(data, p);
    data.push_back(0x0);
    data.push_back(0x0);
    data.push_back(0x0);

    data.push_back(0x04);
    data = mergeVectors(data, q);
    data.push_back(0x0);
    data.push_back(0x0);
    data.push_back(0x0);

    data = mergeVectors(data, nonce);
    data = mergeVectors(data, server_nonce);
    data = mergeVectors(data, new_nonce);

    vector<unsigned char> hash(20,0);
    SHA1(&data[0], data.size(), &hash[0]);
    printVector(hash);

    data_with_hash = mergeVectors(hash, data);
    vector<unsigned char> rand_padding (data_with_hash_size - data_with_hash.size(), 0x0);
    data_with_hash = mergeVectors(data_with_hash, rand_padding);

    string data_with_hash_str = vectorToHexString(data_with_hash);
    BIGNUM * a = BN_new();
    BN_hex2bn(&a, data_with_hash_str.c_str());
    BIGNUM * result = BN_new();
    BIGNUM * e = BN_new();
    BIGNUM * mod = BN_new();

    BN_hex2bn(&mod, MODULUS);
    BN_hex2bn(&e,EXPONENT);

    BN_mod_exp(result, a, e, mod, BN_CTX_new());
    string res = BN_bn2hex(result);
    encrypted_data = hexStringToVector(res);
    cout <<data_with_hash.size()<<"   "<<encrypted_data.size() << endl;
    cout<<"DATA WITH HASH"<<endl;
    printVector(data_with_hash);
    cout<<"////DATA WITH HASH"<<endl<<endl;

    cout<<"ENCRYPTED DATA"<<endl;

    printVector(encrypted_data);
    cout<<"////ENCRYPTED DATA"<<endl;


}

vector<unsigned char> p_q_inner_data_packet::get_encrypted_data() {
    return encrypted_data;
}

vector<unsigned char> res_pq_packet::get_fingerprints() {
    return fingerprints;
}
