#include <iostream>
// включаем файлы, необходимые для tcp и сокетов
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // inet_pton()
#include <errno.h>

#include <openssl/bn.h>

#define SERVER "149.154.167.40" // telegram test-server ip
#define PORT 443 // port

using std::cout;
using std::endl;

// немного спиздил у чмыря
bool connect(const char *host, int port)
{
    struct connection *c;
    int fd = socket (AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        return 0;

    int flags = -1;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons (port);
    inet_pton(AF_INET,host, &addr.sin_addr.s_addr);

    if (connect (fd, (struct sockaddr *) &addr, sizeof (addr)) == -1)
    {
        if (errno != EINPROGRESS)
        {
            close (fd);
            return 0;
        }
    }


    short int mass[] = {
        0x2d,
        0x84,
        0xd5,
        0xf5,
        0x1c,
        0xb5,
        0xc4,
        0x15, 
        0x3, 
        0x2,
        0x3,
        0x4
    };

    char *buffer = new char[52];

    for (int i = 0; i<12; i++)
    {
        buffer[i] = (char)mass[i];
        cout<<buffer[i];
    }

    cout<<strlen(buffer)<<endl;
    cout<<buffer<<endl;


    int n = write(fd,buffer,strlen(buffer));
    cout<<buffer<<" "<<endl;
    cout<<read(fd,buffer,strlen(buffer))<<endl;

    for (int i = 0; i<12; i++)
    {
        cout<<buffer[i];
    }

    return 1;
}

int main(int argc, char *argv[])
{
    cout << connect(SERVER, PORT)<<endl;

    return 0;
}