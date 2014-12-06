#include <vector>
#include <stdint.h>
#include "methods.h"

struct tl_type;
struct tl_int;
struct tl_int128;
struct tl_int256;
struct tl_long;
struct tl_string;

struct tl_type{
public:
    tl_type();
    virtual vector<unsigned char> const serialize() const = 0;
};

struct tl_int:tl_type{
private:
    uint8_t value;
public:
    tl_int(uint8_t v);
    vector<unsigned char> const serialize() const;
};

struct tl_int128:tl_type{
private:
    string value;
public:
    tl_int128(string v);
    tl_int128(vector<unsigned char> v);
    vector<unsigned char> const serialize() const;
};

struct tl_int256:tl_type{
private:
    string value;
public:
    tl_int256(string v);
    tl_int256(vector<unsigned char> v);
    vector<unsigned char> const serialize() const;
};

struct tl_long:tl_type{
private:
    uint16_t value;
public:
    tl_long(uint16_t v);
    vector<unsigned char> const serialize() const;
};

struct tl_string:tl_type{
private:
    string value;
public:
    tl_string(string s);
    vector<unsigned char> const serialize() const;
};
