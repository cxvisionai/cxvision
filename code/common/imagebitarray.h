#ifndef IMAGEBIT_ARRAY_H
#define IMAGEBIT_ARRAY_H

#include <common/imagecounted.h>
#include <common/imagexception.h>
#include <common/imagearray.h>
#include <vector>
#include <limits>
#include <iostream>

#define CX_ARRAY_LEN(v) ((int)(sizeof(v)/sizeof(v[0])))
#define CX_LOG_DIGITS(digits) \
    ((digits == 8) ? 3 : \
     ((digits == 16) ? 4 : \
      ((digits == 32) ? 5 : \
       ((digits == 64) ? 6 : \
        ((digits == 128) ? 7 : \
         (-1))))))
class ImageBitArray : public ImageCounted {
public:
    static const int bitsPerWord = std::numeric_limits<unsigned int>::digits;

private:
    int size;
    ImageArrayRef<int> bits;
    static const int logBits = CX_LOG_DIGITS(bitsPerWord);
    static const int bitsMask = (1 << logBits) - 1;

public:
    ImageBitArray();
    ImageBitArray(int size);
    ImageBitArray(std::vector<int> other);
    ~ImageBitArray();
    int getSize() const;
    int getSizeInBytes() const;

    bool get(int i) const {
        return (bits[i / 32] & (1 << (i & 0x1F))) != 0;
    }

    void set(int i) {
        bits[i / 32] |= 1 << (i & 0x1F);
    }

    void flip(int i) {
        bits[i / 32] ^= 1 << (i & 0x1F);
      }

    int getNextSet(int from);
    int getNextUnset(int from);

    void setBulk(int i, int newBits);
    void setRange(int start, int end);
    void clear();
    bool isRange(int start, int end, bool value);
    std::vector<int>& getBitArray();

    void appendBit(bool bit);
    void appendBits(int value, int numBits);
    void appendBitArray(const ImageBitArray& other);
    void ensureCapacity(int size);

    void xor_(const ImageBitArray& other);

    void toBytes(int bitOffset, std::vector<cx_byte>& array, int offset, int numBytes) const;

    std::string toString() const;

    static ImageArrayRef<int> makeArray(int size) {
        return ImageArrayRef<int>((size + 31) / 32);
      }

    void reverse();

    class Reverse {
    private:
        ImageRef<ImageBitArray> array;
    public:
        Reverse(ImageRef<ImageBitArray> array);
        ~Reverse();
    };

private:
    static int makeArraySize(int size);
};

std::ostream& operator << (std::ostream&, ImageBitArray const&);



#endif // IMAGEBIT_ARRAY_H
