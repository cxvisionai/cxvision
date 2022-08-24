#ifndef IMAGE_GREYSCALE_LUMINANCE_SOURCE
#define IMAGE_GREYSCALE_LUMINANCE_SOURCE

#include <imagesource.h>

class ImageGreySource : public ImageSource {

private:
  typedef ImageSource Super;
  ImageArrayRef<cx_byte> greyData_;
  const int dataWidth_;
  const int dataHeight_;
  const int left_;
  const int top_;

public:
  ImageGreySource(ImageArrayRef<cx_byte> greyData, int dataWidth, int dataHeight, int left,
                           int top, int width, int height);

  ImageArrayRef<cx_byte> getRow(int y, ImageArrayRef<cx_byte> row) const;
  ImageArrayRef<cx_byte> getMatrix() const;

  bool isRotateSupported() const {
    return true;
  }

  ImageRef<ImageSource> rotateCounterClockwise() const;
};


#endif // IMAGE_GREYSCALE_LUMINANCE_SOURCE
