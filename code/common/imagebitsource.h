#ifndef IMAGE_BIT_SOURCE_H
#define IMAGE_BIT_SOURCE_H


#include <common/imagearray.h>
//#include <common/Types.h>


/**
 * <p>This provides an easy abstraction to read bits at a time from a sequence of bytes, where the
 * number of bits read is not often a multiple of 8.</p>
 *
 * <p>This class is not thread-safe.</p>
 *
 * @author srowen@google.com (Sean Owen)
 * @author christian.brunschen@gmail.com (Christian Brunschen)
 */
class ImageBitSource : public ImageCounted {
private:
  ImageArrayRef<cx_byte> bytes_;
  int byteOffset_;
  int bitOffset_;
public:
  /**
   * @param bytes bytes from which this will read bits. Bits will be read from the first byte first.
   * Bits are read within a byte from most-significant to least-significant bit.
   */
  ImageBitSource(ImageArrayRef<cx_byte> &bytes) :
      bytes_(bytes), byteOffset_(0), bitOffset_(0) {
  }

  int getBitOffset() {
    return bitOffset_;
  }

  int getByteOffset() {
    return byteOffset_;
  }

  /**
   * @param numBits number of bits to read
   * @return int representing the bits read. The bits will appear as the least-significant
   *         bits of the int
   * @throws IllegalArgumentException if numBits isn't in [1,32]
   */
  int readBits(int numBits);

  /**
   * @return number of bits that can be read successfully
   */
  int available();
};



#endif // IMAGE_BIT_SOURCE_H
