#include "tl.hpp"

tl_packet::tl_packet(){};

tl_packet::tl_packet(vector<unsigned char> v) {

    int read_position = 0;

    auth_id.clear();
    auth_id.resize(8);

    auth_id = vector<unsigned char> (&v[read_position], &v[read_position + auth_id.size()]);
    printVector(auth_id);

    read_position += auth_id.size();

    timestamp.clear();
    timestamp.resize(8);

    timestamp = vector<unsigned char> (&v[read_position], &v[read_position + timestamp.size()]);
    printVector(timestamp);
    cout<<dec<<"exact unixtime on server is "<<vectorToLong(vectorInversion(timestamp))/0x100000000<<endl;
    vector<unsigned char> time_v = getUnixTimestamp();
    printVector(time_v);

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

    data.clear();
    data.resize(data_size);

    data = vector<unsigned char> (&v[read_position], &v[read_position + data.size()]);
    printVector(data);
}

req_pq_packet::req_pq_packet() {

    auth_id = vector<unsigned char> (8, 0);
    printVector(auth_id);

    timestamp = vectorInversion(getUnixTimestamp());
    printVector(timestamp);

    length = vectorInversion(intToVector(20));
    printVector(length);

    msg_id = vectorInversion(intToVector(0x60469778));
    printVector(msg_id);

    data = random128();
    printVector(data);
}

vector<unsigned char> tl_packet::to_vector() {
    vector<unsigned char> result;
    result = mergeVectors(auth_id, timestamp);
    result = mergeVectors(result, length);
    result = mergeVectors(result, msg_id);
    result = mergeVectors(result, data);

    return result;
}
