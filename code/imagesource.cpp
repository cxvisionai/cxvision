#include <sstream>
#include <imagesource.h>
#include <imageinvertedsource.h>
#include <common/imagexception.h>

ImageSource::ImageSource(int width_, int height_) :width(width_), height(height_) {}

ImageSource::~ImageSource() {}

bool ImageSource::isCropSupported() const {
  return false;
}

ImageRef<ImageSource> ImageSource::crop(int, int, int, int) const {
  throw ImageException("This image source does not support cropping.");
}

bool ImageSource::isRotateSupported() const {
  return false;
}

ImageRef<ImageSource> ImageSource::rotateCounterClockwise() const {
    throw ImageException("This luminance source does not support rotation.");
}

ImageRef<ImageSource> ImageSource::rotateCounterClockwise45() const
{
    throw ImageException("This luminance source does not support rotation 45.");
}

ImageSource::operator std::string() const {
  ImageArrayRef<cx_byte> row;
  std::ostringstream oss;
  for (int y = 0; y < getHeight(); y++) {
    row = getRow(y, row);
    for (int x = 0; x < getWidth(); x++) {
      int luminance = row[x];// & 0xFF;
      char c;
      if (luminance < 0x40) {
        c = '#';
      } else if (luminance < 0x80) {
        c = '+';
      } else if (luminance < 0xC0) {
        c = '.';
      } else {
        c = ' ';
      }
      oss << c;
    }
    oss << '\n';
  }
  return oss.str();
}

ImageRef<ImageSource> ImageSource::invert() const {

  // N.B.: this only works because we use counted objects with the
  // count in the object. This is _not_ how things like shared_ptr
  // work. They do not allow you to make a smart pointer from a native
  // pointer more than once. If we ever switch to (something like)
  // shared_ptr's, the luminace source is going to have keep a weak
  // pointer to itself from which it can create a strong pointer as
  // needed. And, FWIW, that has nasty semantics in the face of
  // exceptions during construction.

  return ImageRef<ImageSource>
      (new ImageInvertedSource(ImageRef<ImageSource>(const_cast<ImageSource*>(this))));
}
