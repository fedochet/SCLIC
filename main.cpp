#include <iostream>

// methods
#include "factorisation.h"
#include "hardcode.hpp"
#include "crc32.h"
#include "AES.h"

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
    
    vector<unsigned char> answer = AES256_ige_decrypt(encrypted_answer, tmp_aes_iv, tmp_aes_key);
    printVector(resPQ_tl.get_nonce());
    printVector(resPQ_tl.get_server_nonce());
    printVector(answer);

    
    
// COMMENTED SOME SHITTY TELEGRAM.ORG EXAMPLE
// RULEV SEXY LOVE HIM

    /* string enans = "28A92FE20173B347A8BB324B5FAB2667C9A8BBCE6468D5B509A4CBDDC186240AC912CF7006AF8926DE606A2E74C0493CAA57741E6C82451F54D3E068F5CCC49B4444124B9666FFB405AAB564A3D01E67F6E912867C8D20D9882707DC330B17B4E0DD57CB53BFAAFA9EF5BE76AE6C1B9B6C51E2D6502A47C883095C46C81E3BE25F62427B585488BB3BF239213BF48EB8FE34C9A026CC8413934043974DB03556633038392CECB51F94824E140B98637730A4BE79A8F9DAFA39BAE81E1095849EA4C83467C92A3A17D997817C8A7AC61C3FF414DA37B7D66E949C0AEC858F048224210FCC61F11C3A910B431CCBD104CCCC8DC6D29D4A5D133BE639A4C32BBFF153E63ACA3AC52F2E4709B8AE01844B142C1EE89D075D64F69A399FEB04E656FE3675A6F8F412078F3D0B58DA15311C1A9F8E53B3CD6BB5572C294904B726D0BE337E2E21977DA26DD6E33270251C2CA29DFCC70227F0755F84CFDA9AC4B8DD5F84F1D1EB36BA45CDDC70444D8C213E4BD8F63B8AB95A2D0B4180DC91283DC063ACFB92D6A4E407CDE7C8C69689F77A007441D4A6A8384B666502D9B77FC68B5B43CC607E60A146223E110FCB43BC3C942EF981930CDC4A1D310C0B64D5E55D308D863251AB90502C3E46CC599E886A927CDA963B9EB16CE62603B68529EE98F9F5206419E03FB458EC4BD9454AA8F6BA777573CC54B328895B1DF25EAD9FB4CD5198EE022B2B81F388D281D5E5BC580107CA01A50665C32B552715F335FD76264FAD00DDD5AE45B94832AC79CE7C511D194BC42B70EFA850BB15C2012C5215CABFE97CE66B8D8734D0EE759A638AF013";
    
    vector<unsigned char> encrans = hexStringToVector(enans);
    printVector(encrans);
    string nonce = "311C85DB234AA2640AFC4A76A735CF5B1F0FD68BD17FA181E1229AD867CC024D";
    string servnonce = "A5CF4D33F4A11EA877BA4AA573907330";
    vector<unsigned char> nnonce = hexStringToVector(nonce);
    vector<unsigned char> snonce = hexStringToVector(servnonce);
    
    vector<unsigned char> tmp_aes_key = mergeVectors(SHA1V(nnonce, snonce), substring(SHA1V(snonce, nnonce), 0, 12));
    printVector(tmp_aes_key);
    vector<unsigned char> tmp_aes_iv = mergeVectors(mergeVectors(substring(SHA1V(snonce, nnonce), 12, 8), SHA1V(nnonce, nnonce)),substring(nnonce,0,4));
    printVector(tmp_aes_iv);
    vector<unsigned char> answer = AES256_ige_decrypt(encrans, tmp_aes_iv, tmp_aes_key);
    printVector(answer);*/
}
