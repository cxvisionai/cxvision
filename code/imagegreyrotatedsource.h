#ifndef IMAGE_GREYSCALE_ROTATED_SOURCE
#define IMAGE_GREYSCALE_ROTATED_SOURCE

#include <imagesource.h>

class ImageGreyRotatedSource : public ImageSource {
 private:
  typedef ImageSource Super;
  ImageArrayRef<cx_byte> greyData_;
  const int dataWidth_;
  const int left_;
  const int top_;

public:
  ImageGreyRotatedSource(ImageArrayRef<cx_byte> greyData, int dataWidth, int dataHeight,
      int left, int top, int width, int height);

  ImageArrayRef<cx_byte> getRow(int y, ImageArrayRef<cx_byte> row) const;
  ImageArrayRef<cx_byte> getMatrix() const;
};

#endif // ZXING_GREYSCALE_ROTATED_LUMINANCE_SOURCE
