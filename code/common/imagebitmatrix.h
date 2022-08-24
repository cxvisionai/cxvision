#ifndef IMAGE_BIT_MATRIX_H
#define IMAGE_BIT_MATRIX_H


#include <common/imagecounted.h>
#include <common/imagebitarray.h>
#include <common/imagearray.h>
#include <limits>

class ImageBitMatrix : public ImageCounted {
public:
  static const int bitsPerWord = std::numeric_limits<unsigned int>::digits;

private:
  int width;
  int height;
  int rowSize;
  ImageArrayRef<int> bits;

public:
  ImageBitMatrix(int dimension);
  ImageBitMatrix(int width, int height);

  ~ImageBitMatrix();

  bool get(int x, int y) const {
    int offset = y * rowSize + (x >> 5);
    return ((((unsigned)bits[offset]) >> (x & 0x1f)) & 1) != 0;
  }

  void set(int x, int y) {
    int offset = y * rowSize + (x >> 5);
    bits[offset] |= 1 << (x & 0x1f);
  }

  void flip(int x, int y);
  void rotate180();

  void clear();
  void setRegion(int left, int top, int width, int height);
  ImageRef<ImageBitArray> getRow(int y, ImageRef<ImageBitArray> row);
  void setRow(int y, ImageRef<ImageBitArray> row);

  int getWidth() const;
  int getHeight() const;

  ImageArrayRef<int> getTopLeftOnBit() const;
  ImageArrayRef<int> getBottomRightOnBit() const;
  ImageArrayRef<int> getEnclosingRectangle() const;

  friend std::ostream& operator<<(std::ostream &out, const ImageBitMatrix &bm);
  const char *description();

private:
  inline void init(int, int);

  ImageBitMatrix(const ImageBitMatrix&);
  ImageBitMatrix& operator =(const ImageBitMatrix&);
};


#endif // IMAGE_BIT_MATRIX_H
