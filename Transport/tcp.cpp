#include "tcp.hpp"

tcp_packet::tcp_packet(vector<unsigned char> &tl_packet)
{
    data = tl_packet;
    size.clear();
    num.clear();
    crc32.clear();

    size.resize(TCP_SIZE);
    num.resize(TCP_PKT_NUM);
    crc32.resize(CRC_SIZE);

    size = intToVector(TCP_SIZE + TCP_PKT_NUM+tl_packet.size()+ CRC_SIZE);

    vector<unsigned char> to_compute_crc = size;
    to_compute_crc = mergeVectors(to_compute_crc, num);
    to_compute_crc = mergeVectors(to_compute_crc, data);

    long crc32_num = compute_crc32(&to_compute_crc[0], to_compute_crc.size());

    crc32 = intToVector(crc32_num);
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
