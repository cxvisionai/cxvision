
#include <imagedetectorresult.h>

ImageDetectorResult::ImageDetectorResult(ImageRef<ImageBitMatrix> bits,
                               ImageArrayRef< ImageRef<ImageResultPoint> > points)
  : bits_(bits), points_(points) {
}

ImageRef<ImageBitMatrix> ImageDetectorResult::getBits() {
  return bits_;
}

ImageArrayRef< ImageRef<ImageResultPoint> > ImageDetectorResult::getPoints() {
  return points_;
}
