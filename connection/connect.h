#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include <iostream>

#define SERVER "149.154.167.40" // telegram test-server ip
#define PORT 80 // port

using namespace std;

int connect(const char *host, int port);