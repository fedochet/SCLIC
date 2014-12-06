#include <methods.h>
#include <connect.h>

struct tcp_package{
private:
    vector<unsigned char> header;
    vector<unsigned char> msg_id;
    vector<unsigned char> data;
    vector<unsigned char> crc32;
public:
    tcp_package(vector<unsigned char> data_param);
    int send(int connection);
    int receive(int connection);
};