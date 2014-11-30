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
#include <openssl/aes.h>

#include "ctime"
#include <vector>

// ports
#define SERVER "149.154.167.40" // telegram test-server ip
#define PORT 80 // port

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

int fd = connect(SERVER, PORT);

int main() {
    cout<<"fd is "<<fd<<endl;
    req_pq_packet Req_pq_packet;
    
    vector<unsigned char> tl_vector  = Req_pq_packet.to_vector();
    printVector(tl_vector);
    
    tcp_packet req_pq(tl_vector, 0x0);
    tcp_packet resPQ_tcp;
    
    req_pq.print();
    cout << "bytes sended:"<<req_pq.send(fd)<<endl;
    cout << "bytes recieved:"<<resPQ_tcp.receive(fd)<<endl;
    
    resPQ_tcp.print();
    
    cout<<"------------------resPQ is recieved!!!----------------"<<endl;
    
    res_pq_packet resPQ_tl(resPQ_tcp.get_data());
    cout<<"pq is "<<resPQ_tl.get_pq()<<endl;
    
    long int p = factorize(resPQ_tl.get_pq());
    
    cout<<hex<<"p and q is "<< p << " and " << resPQ_tl.get_pq()/p <<endl;
    
    cout<<"------------------resPQ is decomposed!!!----------------"<<endl;
    
    p_q_inner_data_packet p_q_inner_data_packet1(resPQ_tl.get_nonce(),resPQ_tl.get_server_nonce(), resPQ_tl.get_pq(), p, resPQ_tl.get_pq()/p);
    
    cout<<"------------------pq_inner is done!!!-------------------"<<endl;
    
    req_DH_params_packet req_dh_params_packet(
                                              resPQ_tl.get_nonce(),
                                              resPQ_tl.get_server_nonce(),
                                              resPQ_tl.get_fingerprints(),
                                              p_q_inner_data_packet1.get_encrypted_data(),
                                              p, resPQ_tl.get_pq()/p);
    
    cout<<"------------------req_DH is done!!!-------------------"<<endl;
    
    vector<unsigned char> req_dh_v = req_dh_params_packet.to_vector();
    printVector(req_dh_v);
    tcp_packet req_DH(req_dh_v, 0x01);
    cout<<"------------------req_DH with TCP is-------------------"<<endl;
    req_DH.print();
    
    cout<<dec<<req_DH.send(fd)<<endl;
    
    tcp_packet res_DH;
    cout<<"bytes recieved"<<endl;
    cout<<res_DH.receive(fd)<<endl;
    
// start DH-auth
// TODO
// 1) do some fucking AESIGEDECRYPT
// 2)
    
    server_DH_params_packet server_dh_params_packet(res_DH.get_data());
    
    vector<unsigned char> encrypted_answer = server_dh_params_packet.get_encrypted_answer();
    
    vector<unsigned char> new_nonce = p_q_inner_data_packet1.get_new_nonce();
    vector<unsigned char> server_nonce = resPQ_tl.get_server_nonce();
    
    vector<unsigned char> tmp_aes_key = mergeVectors(SHA1V(new_nonce, server_nonce), substring(SHA1V(server_nonce, new_nonce), 0, 12));
    printVector(tmp_aes_key);
    
    vector<unsigned char> tmp_aes_iv = mergeVectors(mergeVectors(substring(SHA1V(server_nonce, new_nonce), 12, 8), SHA1V(new_nonce, new_nonce)),substring(new_nonce,0,4));
    printVector(tmp_aes_iv);
    
//    vector<unsigned char> answer = AES256_ige_decrypt(encrypted_answer, tmp_aes_iv, tmp_aes_key);
    
}
