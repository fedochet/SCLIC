#include <vector>
#include "hardcode.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using std::vector;

struct tcp_packet{
    tcp_packet(vector<unsigned char> &tl_packet);
    void print(void);

    //sending package to port (port has to be initialize)
    size_t send(int fg);
private:
    vector<unsigned char> data;
    vector<unsigned char> size;
    vector<unsigned char> num;
    vector<unsigned char> crc32;
};