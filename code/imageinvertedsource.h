#ifndef IMAGEINVERTEDSOURCE_H
#define IMAGEINVERTEDSOURCE_H

#include <imagesource.h>

class ImageInvertedSource : public ImageSource {
private:
  typedef ImageSource Super;
  const ImageRef<ImageSource> delegate;

public:
  ImageInvertedSource(ImageRef<ImageSource> const&);

  ImageArrayRef<cx_byte> getRow(int y, ImageArrayRef<cx_byte> row) const;
  ImageArrayRef<cx_byte> getMatrix() const;

  cx_boolean isCropSupported() const;
  ImageRef<ImageSource> crop(int left, int top, int width, int height) const;

  cx_boolean isRotateSupported() const;

  virtual ImageRef<ImageSource> invert() const;

  ImageRef<ImageSource> rotateCounterClockwise() const;
};

#endif // IMAGEINVERTEDSOURCE_H
