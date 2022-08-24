#ifndef IMAGE_PERSPECTIVE_TANSFORM_H
#define IMAGE_PERSPECTIVE_TANSFORM_H

#include <common/imagecounted.h>
#include <vector>

class ImagePerspectiveTransform : public ImageCounted {
private:
  float a11, a12, a13, a21, a22, a23, a31, a32, a33;
  ImagePerspectiveTransform(float a11, float a21, float a31, float a12, float a22, float a32, float a13, float a23,
                       float a33);

public:
  static ImageRef<ImagePerspectiveTransform>
  quadrilateralToQuadrilateral(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3,
                               float x0p, float y0p, float x1p, float y1p, float x2p, float y2p, float x3p, float y3p);
  static ImageRef<ImagePerspectiveTransform> squareToQuadrilateral(float x0, float y0, float x1, float y1, float x2, float y2,
      float x3, float y3);
  static ImageRef<ImagePerspectiveTransform> quadrilateralToSquare(float x0, float y0, float x1, float y1, float x2, float y2,
      float x3, float y3);
  ImageRef<ImagePerspectiveTransform> buildAdjoint();
  ImageRef<ImagePerspectiveTransform> times(ImageRef<ImagePerspectiveTransform> other);
  void transformPoints(std::vector<float> &points);

  friend std::ostream& operator<<(std::ostream& out, const ImagePerspectiveTransform &pt);
};


#endif // IMAGE_PERSPECTIVE_TANSFORM_H
