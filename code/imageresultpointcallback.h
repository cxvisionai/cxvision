#ifndef IMAGE_RESULT_POINT_CALLBACK_H
#define IMAGE_RESULT_POINT_CALLBACK_H


#include <common/imagecounted.h>

class ImageResultPoint;

class ImageResultPointCallback : public ImageCounted {
protected:
  ImageResultPointCallback() {}
public:
  virtual void foundPossibleResultPoint(ImageResultPoint const& point) = 0;
  virtual ~ImageResultPointCallback();
};


#endif // IMAGE_RESULT_POINT_CALLBACK_H
