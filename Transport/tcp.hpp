#include <vector>
#include "hardcode.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

using std::vector;

struct tcp_packet {
    tcp_packet(vector<unsigned char> &tl_packet);
    void printPacket();
    bool sendPacket(int);
private:
    vector<unsigned char> packet_data;
};