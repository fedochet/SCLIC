#include "tcp_package.h"

tcp_package::tcp_package(vector<unsigned char> data_param){
    static int count = 0;
    header = vectorInversion(intToVector((data_param.size() + 4 + 4 + 4)));
    msg_id = vectorInversion(intToVector(count++));
    data = data_param;
    vector<unsigned char> to_compute_crc = header + msg_id + data_param;

    crc32 = vectorInversion(intToVector(compute_crc32(&to_compute_crc[0], to_compute_crc.size())));
}

int tcp_package::send(int connection) {
    vector<unsigned char> to_send = header + msg_id + data + crc32;
    printVector(to_send);
    return write(connection, &to_send, to_send.size()*sizeof(to_send[0]));
}

int tcp_package::receive(int connection) {
    int bytes_recieved = 0;

    header.clear();
    header.resize(4);
    msg_id.clear();
    msg_id.resize(4);
    crc32.clear();
    crc32.resize(4);

    vector<unsigned char>curr_header(4);
    cout<<curr_header.size()<<endl;
    bytes_recieved += read(connection, &curr_header[0], curr_header.size()*sizeof(curr_header[0]));
    printVector(curr_header);
//    unsigned int pkg_length = vectorToInt(vectorInversion(header));
//
//    pkg_length -= 4 + 4 + 4;
//    data.clear();
//    data.resize(pkg_length);
//
//    bytes_recieved += read(connection, &msg_id[0], msg_id.size()*sizeof(header[0]));
//    bytes_recieved += read(connection, &data[0], data.size()*sizeof(data[0]));
//    bytes_recieved += read(connection, &crc32[0], crc32.size()*sizeof(crc32[0]));

    return bytes_recieved;
}
