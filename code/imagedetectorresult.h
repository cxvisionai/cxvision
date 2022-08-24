#ifndef IMAGE_DETECTOR_RESULT_H
#define IMAGE_DETECTOR_RESULT_H


#include <common/imagecounted.h>
#include <common/imagearray.h>
#include <common/imagebitmatrix.h>
#include <imageresultpoint.h>

class ImageDetectorResult : public ImageCounted {
private:
  ImageRef<ImageBitMatrix> bits_;
  ImageArrayRef< ImageRef<ImageResultPoint> > points_;

public:
  ImageDetectorResult(ImageRef<ImageBitMatrix> bits, ImageArrayRef< ImageRef<ImageResultPoint> > points);
  ImageRef<ImageBitMatrix> getBits();
  ImageArrayRef< ImageRef<ImageResultPoint> > getPoints();
};


#endif // IMAGE_DETECTOR_RESULT_H
