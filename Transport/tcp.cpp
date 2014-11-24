#include "tcp.hpp"

tcp_packet::tcp_packet(vector<unsigned char> &tl_packet)
{
    // size of package
    vector<unsigned char> tl_header(4);
    vector<unsigned char> crc32_v(4);

    vector<unsigned char> packet_num(4,0);

    tl_header = intToVector(TCP_SIZE+TCP_PKT_NUM+tl_packet.size()+ CRC_SIZE);

    tl_header = mergeVectors(tl_header, packet_num);
    packet_data = mergeVectors(tl_header, tl_packet);


    long crc32 = compute_crc32(&packet_data[0], packet_data.size());

    crc32_v = intToVector(crc32);
    //printVector(crc32_v);

    packet_data = mergeVectors(packet_data, crc32_v);
}

void tcp_packet::printPacket()
{
    printVector(packet_data);
}

bool tcp_packet::sendPacket(int fg)
{
    if (write(fg, &packet_data[0], packet_data.size()) == packet_data.size());
        return 1;
    return 0;
}