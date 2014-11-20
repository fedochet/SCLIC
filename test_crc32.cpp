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
#include "crc32.h"

#define SERVER "149.154.167.40" // telegram test-server ip
#define PORT 443 // port

using namespace std;
using std::cout;

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
    size_t n = write(fd, buffer, sizeof(buffer));
    //cout << n << "\n";
    cout << "\n";
    unsigned int crc32;
    
    crc32 = compute_crc32(test, 48);
    cout << "crc32 (от 48 байт первого пакета):\n" << hex << crc32 << "\n";
    cout << "\n\n\n";
    unsigned char buffer_receive[96];
    size_t k = read(fd, buffer_receive, sizeof(buffer_receive));
    
//  unsigned crc32_partial (const void *data, int len, unsigned crc);

    crc32 = compute_crc32(buffer_receive, 96);
    
    for (int i = 0; i < sizeof(buffer_receive); ++i) {
        cout << hex << (int)buffer_receive[i] << " ";
    }
    
    cout << "\n\n";
    
    cout << hex << crc32 << "\n";

    close(fd);

    return 1;
}

int main(int argc, char *argv[])
{
    connect(SERVER, PORT);
    
    return 0;
}
