#include <iostream.h>
// включаем файлы, необходимые для tcp и сокетов
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // inet_pton()

#define SERVER "149.154.167.40" // telegram test-server ip
#define PORT 443 // port

using std::cout;

// немного спиздил у чмыря
bool connect(const char *host, int port)
{
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
    return 1;
}

int main(int argc, char *argv[])
{
    cout << connect(SERVER, PORT);
    return 0;
}