
#ifndef IMAGEGENERICGFPOLY_H
#define IMAGEGENERICGFPOLY_H

#include <vector>
#include <imagearray.h>
#include <imagecounted.h>


class ImageGenericGF;
  
class ImageGenericGFPoly : public ImageCounted {
private:
  ImageGenericGF *field_;
  ImageArrayRef<int> coefficients_;
    
public:
  ImageGenericGFPoly(ImageGenericGF *field, ImageArrayRef<int> coefficients);
  ImageArrayRef<int> getCoefficients();
  int getDegree();
  bool isZero();
  int getCoefficient(int degree);
  int evaluateAt(int a);
  ImageRef<ImageGenericGFPoly> addOrSubtract(ImageRef<ImageGenericGFPoly> other);
  ImageRef<ImageGenericGFPoly> multiply(ImageRef<ImageGenericGFPoly> other);
  ImageRef<ImageGenericGFPoly> multiply(int scalar);
  ImageRef<ImageGenericGFPoly> multiplyByMonomial(int degree, int coefficient);
  std::vector<ImageRef<ImageGenericGFPoly>> divide(ImageRef<ImageGenericGFPoly> other);
    

};

#endif //IMAGEGENERICGFPOLY_H
