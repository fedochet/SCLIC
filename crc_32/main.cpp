#include <iostream>
#include <string>
// включаем файлы, необходимые для tcp и сокетов
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // inet_pton()
#include <errno.h> // errno
// VK-com kpdb files
#include "ctime"

#include "crc32.h"
#include "factorisation.h"
#include "hardcode.hpp"

#include <vector>

#define SERVER "149.154.167.40" // telegram test-server ip
#define PORT 443 // port

using namespace std;



// немного спиздил у чмыря
bool connect(const char *host, int port)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
        return 0;


    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons (port);
    inet_pton(AF_INET,host, &addr.sin_addr.s_addr);

    if(connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        cout << "SOCKET ERROR\n";
        if(errno != EINPROGRESS)
        {
            close (fd);
            return 0;
        }
        return 0;
    }

    unsigned char packet_length[4] = {
            0x34, 0x00, 0x00, 0x00
    };

    vector<unsigned char> vector_try(packet_length, packet_length + sizeof(packet_length)/sizeof(unsigned char));
    vector_try = vectorInversion(vector_try);
    printVector(vector_try);
    vector_try = vectorInversion(vector_try);

    unsigned char number[4] = {
            0x00,0x00,0x00,0x00
    };

    unsigned char auth_key[8] = {
            0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00
    };

    unsigned char unixstamp[8] = {
            0x00,0x00,0x00,0x00,
            0xB1,0xCC,0xE9,0x51
    };

    //vector<unsigned char> unixstamp(8);
    //unixstamp = longToChar(<#(long int)x#>)

    unsigned char message_length[4] = {
            0x14,0x00,0x00,0x00
    };

    unsigned char method_id[4] = {
            0x78,0x97,0x46,0x60
    };

    //unsigned char unixstamp[8] = longToChar(time(0));

    unsigned char buffer[52] = {
            0x34,0x00,0x00,0x00,    //длина сообщения
            0x00,0x00,0x00,0x00,    //порядковый номер
            0x00,0x00,0x00,0x00,    //auth_key
            0x00,0x00,0x00,0x00,    //auth_key
            0x00,0x00,0x00,0x00,    //unixstamp « 32
            0xB1,0xCC,0xE9,0x51,    //unixstamp
            0x14,0x00,0x00,0x00,    //длина тела сообщения
            0x78,0x97,0x46,0x60,    //req_pq
            0xC9,0x39,0x38,0xBA,    //nonce
            0xF6,0x52,0xAF,0xF7,    //nonce
            0xC9,0xBA,0x60,0xB0,    //nonce
            0x42,0x95,0xAA,0x7C,    //crc32 от всего, что выше
            0xC3,0x6C,0xFC,0x79     //crc32
    };

    unsigned char buffer_1[20] = {
            0x00,0x00,0x00,0x00,    //порядковый номер
            0x00,0x00,0x00,0x00,    //auth_key
            0x00,0x00,0x00,0x00,    //auth_key
            0x00,0x00,0x00,0x00,    //unixstamp « 32
            0xB1,0xCC,0xE9,0x51    //unixstamp
    };

    unsigned char buffer_2[28] = {
            0x14,0x00,0x00,0x00,    //длина тела сообщения
            0x78,0x97,0x46,0x60,    //req_pq
            0xC9,0x39,0x38,0xBA,    //nonce
            0xF6,0x52,0xAF,0xF7,    //nonce
            0xC9,0xBA,0x60,0xB0,    //nonce
            0x42,0x95,0xAA,0x7C,    //crc32 от всего, что выше
            0xC3,0x6C,0xFC,0x79     //crc32
    };

    unsigned char test[48] = {
            0x34,0x00,0x00,0x00,    //длина сообщения
            0x00,0x00,0x00,0x00,    //порядковый номер
            0x00,0x00,0x00,0x00,    //auth_key
            0x00,0x00,0x00,0x00,    //auth_key
            0x00,0x00,0x00,0x00,    //unixstamp « 32
            0xB1,0xCC,0xE9,0x51,    //unixstamp
            0x14,0x00,0x00,0x00,    //длина тела сообщения
            0x78,0x97,0x46,0x60,    //req_pq
            0xC9,0x39,0x38,0xBA,    //nonce
            0xF6,0x52,0xAF,0xF7,    //nonce
            0xC9,0xBA,0x60,0xB0,    //nonce
            0x42,0x95,0xAA,0x7C
    };

    for (int i = 0; i < sizeof(buffer); ++i) {
        cout << hex << (int)buffer[i] << " ";
    }

    cout << "\n";
    size_t n = write(fd, &vector_try[0], vector_try.size()*sizeof(vector_try[0]));
    n = write(fd, buffer_1, sizeof(buffer_1));
    n = write(fd, buffer_2, sizeof(buffer_2));
    cout << "\n";
    unsigned int crc32;

    crc32 = compute_crc32(test, 48);

    cout << "crc32 (от 48 байт первого пакета):\n" << hex << crc32 << "\n";
    cout << "\n\n\n";
    unsigned char buffer_receive[96];
    vector<unsigned char> vector_receive(96);

    vector<unsigned char> vector_length(4);
    vector<unsigned char> vector_auth_key(16); //lol
    vector<unsigned char> vector_time(4);
    vector<unsigned char> some_data(8); //lol(2)
    vector<unsigned char> nonce(16);
    vector<unsigned char> server_nonce(16);
    vector<unsigned char> pq_length(1);
    vector<unsigned char> pq(8);


    size_t k = read(fd, &vector_length[0],  vector_length.size()*sizeof(vector_length[0]));
    k = read(fd, &vector_auth_key[0],  vector_auth_key.size()*sizeof(vector_auth_key[0]));
    k = read(fd, &vector_time[0],  vector_time.size()*sizeof(vector_time[0]));
    k = read(fd, &some_data[0],  some_data.size()*sizeof(some_data[0]));
    k = read(fd, &nonce[0],  nonce.size()*sizeof(nonce[0]));
    k = read(fd, &server_nonce[0],  server_nonce.size()*sizeof(server_nonce[0]));
    k = read(fd, &pq_length[0], pq_length.size()*sizeof(pq_length[0]));
    k = read(fd, &pq[0], pq.size()*sizeof(pq[0]));

    //cout<<hex<<vectorToLong(pq)<<endl;
    printVector(vector_length);
    printVector(vector_auth_key);
    printVector(vector_time);
    printVector(some_data);
    printVector(nonce);
    //server_nonce = vectorInversion(server_nonce);
    printVector(server_nonce);
    printVector(pq_length);
    printVector(pq);


    unsigned long pq_num = vectorToLong(pq);
    cout<<dec<<pq_num<<endl;
//  unsigned crc32_partial (const void *data, int len, unsigned crc);

    crc32 = compute_crc32(buffer_receive, 96);

//    for (int i = 0; i < sizeof(buffer_receive); ++i) {
//        cout << hex << (int)buffer_receive[i] << " ";
//    }

    printVector(vector_receive);

    cout << endl << time(0) / 16 <<"+"<<time(0) % 16 <<endl;
    cout << longToChar(time(0))<<endl;

    cout << hex << crc32 << "\n";

    close(fd);

    return 1;
}

int main(int argc, char *argv[])
{
    connect(SERVER, PORT);

    return 0;
}