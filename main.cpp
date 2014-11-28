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
    req_pq_packet Req_pq_packet;

    vector<unsigned char> tl_vector  = Req_pq_packet.to_vector();
    printVector(tl_vector);

    tcp_packet req_pq(tl_vector);
    tcp_packet resPQ_tcp;

    req_pq.print();
    cout << "bytes sended:"<<req_pq.send(fd)<<endl;
    cout << "bytes recieved:"<<resPQ_tcp.receive(fd)<<endl;

    resPQ_tcp.print();

    cout<<"------------------resPQ is recieved!!!----------------"<<endl;

    res_pq_packet resPQ_tl(resPQ_tcp.get_data());
    cout<<dec<<"pq is "<<resPQ_tl.get_pq()<<endl;

    long int p = factorize(resPQ_tl.get_pq());

    cout<<hex<<"p and q is "<< p << " and " << resPQ_tl.get_pq()/p <<endl;

    cout<<"------------------resPQ is decomposed!!!----------------"<<endl;

    p_q_inner_data_packet p_q_inner_data_packet1(resPQ_tl.get_nonce(),resPQ_tl.get_server_nonce(), resPQ_tl.get_pq(), p, resPQ_tl.get_pq()/p);
    req_DH_params_packet req_dh_params_packet(
            resPQ_tl.get_nonce(),
            resPQ_tl.get_server_nonce(),
            resPQ_tl.get_fingerprints(),
            p_q_inner_data_packet1.get_encrypted_data(),
            p, resPQ_tl.get_pq()/p);

    vector<unsigned char> req_dh_v = req_dh_params_packet.to_vector();
    printVector(req_dh_v);
    tcp_packet req_DH(req_dh_v);
    cout<<dec<<req_DH.send(fd)<<endl;

    tcp_packet res_DH;
    cout<<res_DH.receive(fd)<<endl;


//    cout<<"random 128 int"<<endl;
//    printVector(generate_random());
//
//    BIGNUM * e = BN_new();
//    BIGNUM * mod = BN_new();
//    BIGNUM * p = BN_new();
//    BIGNUM * q = BN_new();
//    BN_hex2bn(&p, "50629AFB");
//    BN_hex2bn(&q, "545EBA49");
//    BN_hex2bn(&e, "010001");
//    BN_hex2bn(&mod,
//            "C150023E2F70DB7985DED064759CFECF0AF328E69A41DAF4D6F01B538135A6F91F8F8B2A0EC9BA9720CE352EFCF6C5680FFC424BD634864902DE0B4BD6D49F4E580230E3AE97D95C8B19442B3C0A10D8F5633FECEDD6926A7F6DAB0DDB7D457F9EA81B8465FCD6FFFEED114011DF91C059CAEDAF97625F6C96ECC74725556934EF781D866B34F011FCE4D835A090196E9A5F0E4449AF7EB697DDB9076494CA5F81104A305B6DD27665722C46B60E5DF680FB16B210607EF217652E60236C255F6A28315F4083A96791D7214BF64C1DF4FD0DB1944FB26A2A57031B32EEE64AD15A8BA68885CDE74A5BFC920F6ABF59BA5C75506373E7130F9042DA922179251F");
//
//    RSA *rsa_pub = RSA_new();
//    rsa_pub->e = e;
//    rsa_pub->n = mod;
//    rsa_pub->p = p;
//    rsa_pub->q = q;



//    unsigned char hash[20];
//    //SHA1(test_sha1, (size_t)96, hash);
//   // cout<< SHA_LBLOCK<<endl;
//
//    for (int i = 0; i<20; i++) {
//        cout << hex << (int) hash[i] << " ";
//    }
//
//    unsigned char from_1[] = "70129E08BA7913FE24ED40A1962C11B9F7178DEBEC5AC983081A7E198DD4E28F930000000450629AFB00000004545EBA4900000015C21DCE2F8575BEF8B00544BDC4EF332C5F5ADA17B08350CA0D41E23D86E6E94428DA0E6F4EC9F8353C398DE6C15ED01911EA0F0C5D387AA87BBD3678B4ECBC72556D67CC8BF786DD491CC60864CC8A6F54BB1D8FEF4DD589291A0CB73C7C80C1D3D0295D8F74DB82EF8DC3D202D77AF1489F590FE10B810796DCA370EBCEA1CB9B053493A32CDFDFDE9A9F62880287509F1A6E9B2370DEC74F21C5B1D06A9520EAF737FBA2E64E1BBF792F466B1E7E935552D0D06006A82A266C4382652D0E74047E7690CA8AADB7CA52";
//    BIGNUM *a = BN_new();
//    BN_hex2bn(&a, (char*)from_1);
//
//    cout<<endl<<endl;
//    SHA1(from, 235, hash);
//
//    for (int i = 0; i<20; i++) {
//        cout << hex << (int) hash[i] << " ";
//    }
//
//    unsigned char to[255];
//
//
//    RSA_public_encrypt(255, from, to, rsa_pub, RSA_NO_PADDING);
//    //cout<<(int *)to<<endl;
//
//    BIGNUM * result = BN_new();
//
//    BN_mod_exp(result, a, e, mod, BN_CTX_new());
//    cout<<BN_bn2hex(result)<<endl;


//    for(int i = 0; i<255; i++)
//        cout<<(int)to[i];


}