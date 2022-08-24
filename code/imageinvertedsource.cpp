
#include <imageinvertedsource.h>


ImageInvertedSource::ImageInvertedSource(ImageRef<ImageSource> const& delegate_)
    : Super(delegate_->getWidth(), delegate_->getHeight()), delegate(delegate_) {}  

ImageArrayRef<cx_byte> ImageInvertedSource::getRow(int y, ImageArrayRef<cx_byte> row) const {
  row = delegate->getRow(y, row);
  int width = getWidth();
  for (int i = 0; i < width; i++) {
    row[i] = 0xFF - row[i];
  }
  return row;
}

ImageArrayRef<cx_byte> ImageInvertedSource::getMatrix() const {
  ImageArrayRef<cx_byte> matrix = delegate->getMatrix();
  int length = getWidth() * getHeight();
  ImageArrayRef<cx_byte> invertedMatrix(length);
  for (int i = 0; i < length; i++) {
    invertedMatrix[i] = 0xFF - matrix[i];
  }
  return invertedMatrix;
}

cx_boolean ImageInvertedSource::isCropSupported() const {
  return delegate->isCropSupported();
}

ImageRef<ImageSource> ImageInvertedSource::crop(int left, int top, int width, int height) const {
  return ImageRef<ImageSource>(new ImageInvertedSource(delegate->crop(left, top, width, height)));
}

cx_boolean ImageInvertedSource::isRotateSupported() const {
  return delegate->isRotateSupported();
}

ImageRef<ImageSource> ImageInvertedSource::invert() const {
  return delegate;
}

ImageRef<ImageSource> ImageInvertedSource::rotateCounterClockwise() const {
  return ImageRef<ImageSource>(new ImageInvertedSource(delegate->rotateCounterClockwise()));
}
