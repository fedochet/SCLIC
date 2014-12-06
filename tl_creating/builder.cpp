#include<builder.h>

tl_fabric::tl_fabric() {}
//package, wich is sended, is imidiately removed and cannot be used furthermore (nothingdohere)
vector<unsigned char> tl_fabric::serialize(string id, map<string, tl_type *> data, bool header_on) {
    vector<unsigned char> result;

    for(auto i:schemes[id])
    {
        cout<<i<<endl;
        result = result + data[i]->serialize();
        delete(data[i]);
    }

    id = id.substr(id.find_first_of('#',0)+1);
    cout<<id<<endl;
    auto header = vectorInversion(hexStringToVector(id));
    printVector(header);

    if (header_on)
    {
        header = vectorInversion(getUnixTimestamp()) + vectorInversion(intToVector(result.size() + header.size())) + header;
    }

    return header + result;
}

void tl_fabric::addScheme(string scheme) {
    string tl_id = scheme.substr(0, scheme.find_first_of(' ', 0));
    vector <string> fields(0);
    cout<<tl_id<<endl;

    scheme = scheme.substr(scheme.find_first_of(' ', 0)+1);

    //now we should take names of fields; maybe (MYBE!) later we are going to take typenames too
    while (scheme.find_first_of(':', 0)!=string::npos)
    {
        fields.push_back(scheme.substr(0, scheme.find_first_of(':', 0)));
        scheme = scheme.substr(scheme.find_first_of(' ', 0)+1);
    }

    schemes.insert({tl_id, fields});
}

vector<unsigned char> encryptor::encrypt(vector<unsigned char> tl_package, bool encryption_on) {
    if (!encryption_on)
    {
        return vector<unsigned char>(8, 0x0) + tl_package;
    }
}

encryptor::encryptor() {}
