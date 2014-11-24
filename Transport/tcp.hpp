#include <vector>
#include "hardcode.hpp"

using std::vector;

struct tcp_packet {
    tcp_packet(vector<unsigned char> &tl_packet);

private:
    vector<unsigned char> packet_data;
};