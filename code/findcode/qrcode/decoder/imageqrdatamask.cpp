
#include <qrcode/decoder/imageqrdatamask.h>

#include <common/imagexception.h>


using namespace std;

ImageQRDataMask::ImageQRDataMask() {
}

ImageQRDataMask::~ImageQRDataMask() {
}

vector<ImageRef<ImageQRDataMask> > ImageQRDataMask::DATA_MASKS;
static int N_DATA_MASKS = ImageQRDataMask::buildDataMasks();

ImageQRDataMask &ImageQRDataMask::forReference(int reference) {
  if (reference < 0 || reference > 7) {
    throw ImageException("reference must be between 0 and 7");
  }
  return *DATA_MASKS[reference];
}

void ImageQRDataMask::unmaskBitMatrix(ImageBitMatrix& bits, size_t dimension) {
  for (size_t y = 0; y < dimension; y++) {
    for (size_t x = 0; x < dimension; x++) {
      // TODO: check why the coordinates have to be swapped
      if (isMasked(y, x)) {
        bits.flip(int(x), int(y));
      }
    }
  }
}

/**
 * 000: mask bits for which (x + y) mod 2 == 0
 */
class ImageQRDataMask000 : public ImageQRDataMask {
public:
  bool isMasked(size_t x, size_t y) {
    //		return ((x + y) & 0x01) == 0;
    return ((x + y) % 2) == 0;
  }
};

/**
 * 001: mask bits for which x mod 2 == 0
 */
class ImageQRDataMask001 : public ImageQRDataMask {
public:
  bool isMasked(size_t x, size_t) {
    //		return (x & 0x01) == 0;
    return (x % 2) == 0;
  }
};

/**
 * 010: mask bits for which y mod 3 == 0
 */
class ImageQRDataMask010 : public ImageQRDataMask {
public:
  bool isMasked(size_t, size_t y) {
    return y % 3 == 0;
  }
};

/**
 * 011: mask bits for which (x + y) mod 3 == 0
 */
class ImageQRDataMask011 : public ImageQRDataMask {
public:
  bool isMasked(size_t x, size_t y) {
    return (x + y) % 3 == 0;
  }
};

/**
 * 100: mask bits for which (x/2 + y/3) mod 2 == 0
 */
class ImageQRDataMask100 : public ImageQRDataMask {
public:
  bool isMasked(size_t x, size_t y) {
    //		return (((x >> 1) + (y / 3)) & 0x01) == 0;
    return (((x >> 1) + (y / 3)) % 2) == 0;
  }
};

/**
 * 101: mask bits for which xy mod 2 + xy mod 3 == 0
 */
class ImageQRDataMask101 : public ImageQRDataMask {
public:
  bool isMasked(size_t x, size_t y) {
    size_t temp = x * y;
    //		return (temp & 0x01) + (temp % 3) == 0;
    return (temp % 2) + (temp % 3) == 0;

  }
};

/**
 * 110: mask bits for which (xy mod 2 + xy mod 3) mod 2 == 0
 */
class ImageQRDataMask110 : public ImageQRDataMask {
public:
  bool isMasked(size_t x, size_t y) {
    size_t temp = x * y;
    //		return (((temp & 0x01) + (temp % 3)) & 0x01) == 0;
    return (((temp % 2) + (temp % 3)) % 2) == 0;
  }
};

/**
 * 111: mask bits for which ((x+y)mod 2 + xy mod 3) mod 2 == 0
 */
class ImageQRDataMask111 : public ImageQRDataMask {
public:
  bool isMasked(size_t x, size_t y) {
    //		return ((((x + y) & 0x01) + ((x * y) % 3)) & 0x01) == 0;
    return ((((x + y) % 2) + ((x * y) % 3)) % 2) == 0;
  }
};

int ImageQRDataMask::buildDataMasks() {
  DATA_MASKS.push_back(ImageRef<ImageQRDataMask> (new ImageQRDataMask000()));
  DATA_MASKS.push_back(ImageRef<ImageQRDataMask> (new ImageQRDataMask001()));
  DATA_MASKS.push_back(ImageRef<ImageQRDataMask> (new ImageQRDataMask010()));
  DATA_MASKS.push_back(ImageRef<ImageQRDataMask> (new ImageQRDataMask011()));
  DATA_MASKS.push_back(ImageRef<ImageQRDataMask> (new ImageQRDataMask100()));
  DATA_MASKS.push_back(ImageRef<ImageQRDataMask> (new ImageQRDataMask101()));
  DATA_MASKS.push_back(ImageRef<ImageQRDataMask> (new ImageQRDataMask110()));
  DATA_MASKS.push_back(ImageRef<ImageQRDataMask> (new ImageQRDataMask111()));
  return int(DATA_MASKS.size());
}

