#include "connect.h"

// connection to server
int connect(const char *host, int port) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        return fd;


    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, host, &addr.sin_addr.s_addr);

    if (connect(fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
        cout << "SOCKET ERROR\n";
        if (errno != EINPROGRESS) {
            close(fd);
            return 0;
        }
        return 0;
    }
    return fd;
}