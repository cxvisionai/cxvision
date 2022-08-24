#ifndef IMAGE_SOURCE_H
#define IMAGE_SOURCE_H

#include <common/imagecounted.h>
#include <common/imagearray.h>
#include <string.h>

class ImageSource : public ImageCounted {
 private:
  const int width;
  const int height;

 public:
  ImageSource(int width, int height);
  virtual ~ImageSource();

  int getWidth() const { return width; }
  int getHeight() const { return height; }

  // Callers take ownership of the returned memory and must call delete [] on it themselves.
  virtual ImageArrayRef<cx_byte> getRow(int y, ImageArrayRef<cx_byte> row) const = 0;
  virtual ImageArrayRef<cx_byte> getMatrix() const = 0;

  virtual bool isCropSupported() const;
  virtual ImageRef<ImageSource> crop(int left, int top, int width, int height) const;

  virtual bool isRotateSupported() const;

  virtual ImageRef<ImageSource> invert() const;
  
  virtual ImageRef<ImageSource> rotateCounterClockwise() const;

  virtual ImageRef<ImageSource> rotateCounterClockwise45() const;

  operator std::string () const;
};

#endif // ImageSource_H
