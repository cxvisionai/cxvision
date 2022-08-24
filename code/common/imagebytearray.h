#ifndef IMAGE_BYTE_ARRAY_H
#define IMAGE_BYTE_ARRAY_H

#include <vector>
#include <cstdint>

class ByteArray : public std::vector<uint8_t>
{
public:
    ByteArray() {}
    ByteArray(std::initializer_list<uint8_t> list) : std::vector<uint8_t>(list) {}
    explicit ByteArray(int len) : std::vector<uint8_t>(len, 0) {}
    int length() const { return static_cast<int>(size()); }
    const uint8_t* bytePtr() const { return data(); }
    uint8_t* bytePtr() { return data(); }
};


#endif
