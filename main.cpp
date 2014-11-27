#include <iostream>

// methods
#include "factorisation.h"
#include "hardcode.hpp"
#include "crc32.h"

#include <openssl/sha.h>
#include <openssl/rsa.h>

// packages
#include "tcp.hpp"

// for connection
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <openssl/bn.h>

#include "ctime"
#include <vector>

// ports
#define SERVER "149.154.167.40" // telegram test-server ip
#define PORT 443 // port

using namespace std;

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

static int fd = connect(SERVER, PORT);

int main() {
    cout<<"fd is "<<fd<<endl;

    unsigned char buffer[] = {
            0x00, 0x00, 0x00, 0x00,    //auth_key
            0x00, 0x00, 0x00, 0x00,    //auth_key
            0x00, 0x00, 0x00, 0x00,    //unixstamp « 32
            0xB1, 0xCC, 0xE9, 0x51,    //unixstamp
            0x14, 0x00, 0x00, 0x00,    //длина тела сообщения
            0x78, 0x97, 0x46, 0x60,    //req_pq
            0xC9, 0x39, 0x38, 0xBA,    //nonce
            0xF6, 0x52, 0xAF, 0xF7,    //nonce
            0xC9, 0xBA, 0x60, 0xB0,    //nonce
            0x42, 0x95, 0xAA, 0x7C
    };

    vector<unsigned char> tl_vector(buffer, buffer + sizeof(buffer)/sizeof(unsigned char));
    printVector(tl_vector);

    tcp_packet req_pq(tl_vector);
    tcp_packet resPQ_tcp;

    req_pq.print();
    cout << "bytes sended:"<<req_pq.send(fd)<<endl;
    cout << "bytes recieved:"<<resPQ_tcp.receive(fd)<<endl;

    resPQ_tcp.print();

    cout<<"------------------resPQ is recieved!!!----------------"<<endl;

    tl_packet resPQ_tl = resPQ_tcp.create_tl();

    cout<<"random 128 int"<<endl;
    printVector(random128());

    BIGNUM * e = BN_new();
    BIGNUM * mod = BN_new();
    BIGNUM * p = BN_new();
    BIGNUM * q = BN_new();
    BN_hex2bn(&p, "50629AFB");
    BN_hex2bn(&q, "545EBA49");
    BN_hex2bn(&e, "010001");
    BN_hex2bn(&mod,
            "C150023E2F70DB7985DED064759CFECF0AF328E69A41DAF4D6F01B538135A6F91F8F8B2A0EC9BA9720CE352EFCF6C5680FFC424BD634864902DE0B4BD6D49F4E580230E3AE97D95C8B19442B3C0A10D8F5633FECEDD6926A7F6DAB0DDB7D457F9EA81B8465FCD6FFFEED114011DF91C059CAEDAF97625F6C96ECC74725556934EF781D866B34F011FCE4D835A090196E9A5F0E4449AF7EB697DDB9076494CA5F81104A305B6DD27665722C46B60E5DF680FB16B210607EF217652E60236C255F6A28315F4083A96791D7214BF64C1DF4FD0DB1944FB26A2A57031B32EEE64AD15A8BA68885CDE74A5BFC920F6ABF59BA5C75506373E7130F9042DA922179251F");

    RSA *rsa_pub = RSA_new();
    rsa_pub->e = e;
    rsa_pub->n = mod;
    rsa_pub->p = p;
    rsa_pub->q = q;



    unsigned char hash[20];
    //SHA1(test_sha1, (size_t)96, hash);
   // cout<< SHA_LBLOCK<<endl;

    for (int i = 0; i<20; i++) {
        cout << hex << (int) hash[i] << " ";
    }

    unsigned char from_1[] = "70129E08BA7913FE24ED40A1962C11B9F7178DEBEC5AC983081A7E198DD4E28F930000000450629AFB00000004545EBA4900000015C21DCE2F8575BEF8B00544BDC4EF332C5F5ADA17B08350CA0D41E23D86E6E94428DA0E6F4EC9F8353C398DE6C15ED01911EA0F0C5D387AA87BBD3678B4ECBC72556D67CC8BF786DD491CC60864CC8A6F54BB1D8FEF4DD589291A0CB73C7C80C1D3D0295D8F74DB82EF8DC3D202D77AF1489F590FE10B810796DCA370EBCEA1CB9B053493A32CDFDFDE9A9F62880287509F1A6E9B2370DEC74F21C5B1D06A9520EAF737FBA2E64E1BBF792F466B1E7E935552D0D06006A82A266C4382652D0E74047E7690CA8AADB7CA52";
    BIGNUM *a = BN_new();
    BN_hex2bn(&a, (char*)from_1);

    unsigned char from[] = {
            0xEC,
            0x5A,
            0xC9,
            0x83,
            0x08,
            0x1A,
            0x7E,
            0x19,
            0x8D,
            0xD4,
            0xE2,
            0x8F,
            0x93,
            0x00,
            0x00,
            0x00,
            0x04,
            0x50,
            0x62,
            0x9A,
            0xFB,
            0x00,
            0x00,
            0x00,
            0x04,
            0x54,
            0x5E,
            0xBA,
            0x49,
            0x00,
            0x00,
            0x00,
            0x15,
            0xC2,
            0x1D,
            0xCE,
            0x2F,
            0x85,
            0x75,
            0xBE,
            0xF8,
            0xB0,
            0x05,
            0x44,
            0xBD,
            0xC4,
            0xEF,
            0x33,
            0x2C,
            0x5F,
            0x5A,
            0xDA,
            0x17,
            0xB0,
            0x83,
            0x50,
            0xCA,
            0x0D,
            0x41,
            0xE2,
            0x3D,
            0x86,
            0xE6,
            0xE9,
            0x44,
            0x28,
            0xDA,
            0x0E,
            0x6F,
            0x4E,
            0xC9,
            0xF8,
            0x35,
            0x3C,
            0x39,
            0x8D,
            0xE6,
            0xC1,
            0x5E,
            0xD0,
            0x19,
            0x11,
            0xEA,
            0x0F,
            0x0C,
            0x5D,
            0x38,
            0x7A,
            0xA8,
            0x7B,
            0xBD,
            0x36,
            0x78,
            0xB4,
            0xEC,
            0xBC,
            0x72,
            0x55,
            0x6D,
            0x67,
            0xCC,
            0x8B,
            0xF7,
            0x86,
            0xDD,
            0x49,
            0x1C,
            0xC6,
            0x08,
            0x64,
            0xCC,
            0x8A,
            0x6F,
            0x54,
            0xBB,
            0x1D,
            0x8F,
            0xEF,
            0x4D,
            0xD5,
            0x89,
            0x29,
            0x1A,
            0x0C,
            0xB7,
            0x3C,
            0x7C,
            0x80,
            0xC1,
            0xD3,
            0xD0,
            0x29,
            0x5D,
            0x8F,
            0x74,
            0xDB,
            0x82,
            0xEF,
            0x8D,
            0xC3,
            0xD2,
            0x02,
            0xD7,
            0x7A,
            0xF1,
            0x48,
            0x9F,
            0x59,
            0x0F,
            0xE1,
            0x0B,
            0x81,
            0x07,
            0x96,
            0xDC,
            0xA3,
            0x70,
            0xEB,
            0xCE,
            0xA1,
            0xCB,
            0x9B,
            0x05,
            0x34,
            0x93,
            0xA3,
            0x2C,
            0xDF,
            0xDF,
            0xDE,
            0x9A,
            0x9F,
            0x62,
            0x88,
            0x02,
            0x87,
            0x50,
            0x9F,
            0x1A,
            0x6E,
            0x9B,
            0x23,
            0x70,
            0xDE,
            0xC7,
            0x4F,
            0x21,
            0xC5,
            0xB1,
            0xD0,
            0x6A,
            0x95,
            0x20,
            0xEA,
            0xF7,
            0x37,
            0xFB,
            0xA2,
            0xE6,
            0x4E,
            0x1B,
            0xBF,
            0x79,
            0x2F,
            0x46,
            0x6B,
            0x1E,
            0x7E,
            0x93,
            0x55,
            0x52,
            0xD0,
            0xD0,
            0x60,
            0x06,
            0xA8,
            0x2A,
            0x26,
            0x6C,
            0x43,
            0x82,
            0x65,
            0x2D,
            0x0E,
            0x74,
            0x04,
            0x7E,
            0x76,
            0x90,
            0xCA,
            0x8A,
            0xAD,
            0xB7,
            0xCA,
            0x52};

    cout<<endl<<endl;
    SHA1(from, 235, hash);

    for (int i = 0; i<20; i++) {
        cout << hex << (int) hash[i] << " ";
    }

    unsigned char to[255];


    RSA_public_encrypt(255, from, to, rsa_pub, RSA_NO_PADDING);
    //cout<<(int *)to<<endl;

    BIGNUM * result = BN_new();

    BN_mod_exp(result, a, e, mod, BN_CTX_new());
    cout<<BN_bn2hex(result)<<endl;


//    for(int i = 0; i<255; i++)
//        cout<<(int)to[i];


}