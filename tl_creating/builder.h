#include "methods.h"
#include "classes.h"

struct tl_fabric{
private:
    map<string, vector<string> > schemes;
public:
    tl_fabric();
    void addScheme(string scheme);
    vector<unsigned char> serialize(string id, map<string ,tl_type *> data, bool header_on); //header = timestamp (i guess)
};

struct encryptor{
private:
    vector<unsigned char> current_nonce;
    vector<unsigned char> auth_key;
public:
    encryptor();
    vector<unsigned char> encrypt(vector<unsigned char> tl_package, bool no_encryption);
};