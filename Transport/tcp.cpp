#include "tcp.hpp"

tcp_packet::tcp_packet(){};
tcp_packet::tcp_packet(vector<unsigned char> &tl_packet, int packet_num)
{
    data = tl_packet;
    size.clear();
    num.clear();
    crc32.clear();

    size.resize(TCP_SIZE);
    num.resize(TCP_PKT_NUM);
    num = vectorInversion(intToVector(packet_num));
    crc32.resize(CRC_SIZE);

    size = intToVector(TCP_SIZE + TCP_PKT_NUM + tl_packet.size() + CRC_SIZE);
    size = vectorInversion(size);

    vector<unsigned char> to_compute_crc = size;
    to_compute_crc = mergeVectors(to_compute_crc, num);
    to_compute_crc = mergeVectors(to_compute_crc, data);

    long crc32_num = compute_crc32(&to_compute_crc[0], to_compute_crc.size());

    crc32 = intToVector(crc32_num);
    crc32 = vectorInversion(crc32);
}

void tcp_packet::print(void) {
    //printVector(packet_data);
    printVector(size);
    printVector(num);
    printVector(data);
    printVector(crc32);
}

size_t tcp_packet::send(int fg) {
    vector<unsigned char> packet_data = mergeVectors(size, num);
    packet_data = mergeVectors(packet_data, data);
    packet_data = mergeVectors(packet_data, crc32);
    printVector(packet_data);
    cout<<packet_data.size()<<endl;
    return write(fg, &packet_data[0], packet_data.size() * sizeof(packet_data[0]));
}

size_t tcp_packet::receive(int fg) {
    size_t packages_recieved = 0;
    int bytes_left = 0;
    //length is known
    size.clear();
    size.resize(4);

    //length recieving
    packages_recieved += read(fg, &size[0], size.size()*sizeof(size[0]));
    printVector(size);

    bytes_left = vectorToInt(vectorInversion(size));

    bytes_left -= TCP_SIZE;
    cout<<bytes_left<<endl;

    if (bytes_left<=0)
        return packages_recieved;

    num.clear();
    num.resize(4);

    packages_recieved += read(fg, &num[0], num.size()*sizeof(num[0]));
    printVector(num);

    bytes_left -= TCP_PKT_NUM;

    data.clear();
    data.resize(bytes_left - CRC_SIZE);

    packages_recieved += read(fg, &data[0], data.size()*sizeof(data[0]));
    printVector(data);

    bytes_left -= data.size();

    crc32.clear();
    crc32.resize(CRC_SIZE);

    packages_recieved += read(fg, &crc32[0], crc32.size()*sizeof(crc32[0]));
    printVector(crc32);

    bytes_left -= CRC_SIZE;

    return packages_recieved;
}

vector<unsigned char> tcp_packet::get_data() {
    return data;
}
