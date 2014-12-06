#include "classes.h"

tl_type::tl_type() {} // its abstract class
tl_int::tl_int(uint8_t v){
    value = v;
}

vector<unsigned char> const tl_int::serialize() const{
    return vectorInversion(intToVector(value));
}

tl_long::tl_long(uint16_t v):value(v) {}

vector<unsigned char> const tl_long::serialize() const {
    return vectorInversion(longToVector(value));
}

tl_string::tl_string(string s) :value(s) {}

vector<unsigned char> const tl_string::serialize() const {
    // lenght is devided by two because two symbosl == 1 byte
    vector<unsigned char> length_v; // which would be added before actual string
    if (value.length()/2<0xfe) // idk by the way how it works
    {
        length_v = vectorInversion(intToVector(value.length() / 2));
        length_v.pop_back();//cutting last 3 bytes which are unused
        length_v.pop_back();
        length_v.pop_back();
    }
    else
    {
        length_v = vectorInversion(intToVector(value.length() / 2));
        length_v.insert(length_v.begin(), 0xfe); // inserting additional 0xfe byte
        length_v.pop_back();
    };

    int padding_length; // we are thinking that string is always divisible by 2
    if (value.length()%8 == 0)
        padding_length = 0;
    else
        padding_length = (4-((value.length())/2 + length_v.size())%4);

    return length_v + hexStringToVector(value) + vector<unsigned char> (padding_length,0);
}

tl_int128::tl_int128(string v):value(v){}
tl_int256::tl_int256(string v):value(v){}

vector<unsigned char> const tl_int128::serialize() const {
    return hexStringToVector(value); // in big endian format
}

vector<unsigned char> const tl_int256::serialize() const {
    return hexStringToVector(value); // in big endian format
}

tl_int128::tl_int128(vector<unsigned char> v) {
    value = vectorToHexString(v);
}

tl_int256::tl_int256(vector<unsigned char> v) {
    value = vectorToHexString(v);
}
