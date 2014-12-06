#include <iostream>
#include <methods.h>
#include <builder.h>
#include <tcp_package.h>

using namespace std;


int main()
{
    int fd = connect(SERVER, PORT);
    cout<<fd<<endl;
    vector<unsigned char> to_activate_crc(10,0);
    compute_crc32(&to_activate_crc[0], 10);

    tl_fabric TL;
    encryptor Encryptor;
    map<string ,tl_type*> req_pq;
    req_pq.insert({"nonce",new tl_int128(generate_random(128))});

    TL.addScheme("req_pq#60469778 nonce:int128 = ResPQ");
    //tcp_package TCP(Encryptor.encrypt(TL.serialize("req_pq#60469778", req_pq, true),false));
    //tcp_package TCP(hexStringToVector("0000000000000000000000000f35e8511400000078974660153ea9bec54ffc1666234ad031c0f13d"));
    tcp_package TCP(hexStringToVector("000000000000000000000000b1cce9511400000078974660c93938baf652aff7c9ba60b04295aa7c"));

    cout<<dec<<TCP.send(fd)<<endl;
    cout<<TCP.receive(fd)<<endl;

    return 0;
};
