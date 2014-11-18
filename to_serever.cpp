#include <iostream>
// включаем файлы, необходимые для tcp и сокетов
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // inet_pton()
#include <errno.h>

#include <openssl/bn.h>

#define SERVER "149.154.167.40" // telegram test-server ip
#define PORT 80 // port

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
            return 0;
        }
    }


    char *query = "7E 21 45 00 00 4B 57 49 40 00 FA 06 85 77 C7 B6 78 0E CE D6 95 50 00 6E 04 9F 74 5B EE A2 59 9A 00 0E 50 18 24 00 E3 2A 00 00 2B 4F 4B 20 50 61 73 73 77 6F 72 64 20 72 65 71 75 69 72 65 64 20 66 6F 72 20 61 6C 65 78 75 72 2E 0D 0A 67 B2 7E";

    // for (int i = 0; i<sizeof(mass)/sizeof(uint16_t); i++)
    // {
    //     mass[i] = htons(mass[i]);
    //     cout<<std::hex<<mass[i]<<endl;
    // }

    char buffer[256];

    bzero(buffer, 256);

    cout<<strlen(buffer)<<endl;
    cout<<buffer<<endl;


    int n = write(fd,query, sizeof(query));
    cout<<"n is "<<n<<endl;
    cout<<buffer<<" "<<endl;
    cout<<read(fd,buffer,strlen(buffer))<<endl;
    cout<<buffer<<endl;

    for (int i = 0; i<12; i++)
    {
        cout<<buffer[i];
    }

    return 1;
}

int main(int argc, char *argv[])
{
    cout << connect(SERVER, PORT)<<"  "<<sizeof(uint32_t(100))<<endl;

    return 0;
}