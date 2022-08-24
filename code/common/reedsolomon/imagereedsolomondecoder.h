#ifndef CX_REED_SOLOMON_DECODER_H
#define CX_REED_SOLOMON_DECODER_H



#include <memory>
#include <vector>
#include <common/imagecounted.h>
#include <common/imagearray.h>
#include <common/reedsolomon/imagegenericgfpoly.h>
#include <common/reedsolomon/imagegenericgf.h>

class ImageGenericGFPoly;
class ImageGenericGF;

class ImageReedSolomonDecoder {
private:
  ImageRef<ImageGenericGF> field;
public:
  ImageReedSolomonDecoder(ImageRef<ImageGenericGF> fld);
  ~ImageReedSolomonDecoder();
  void decode(ImageArrayRef<int> received, int twoS);
  std::vector<ImageRef<ImageGenericGFPoly> > runEuclideanAlgorithm(ImageRef<ImageGenericGFPoly> a, ImageRef<ImageGenericGFPoly> b, int R);

private:
  ImageArrayRef<int> findErrorLocations(ImageRef<ImageGenericGFPoly> errorLocator);
  ImageArrayRef<int> findErrorMagnitudes(ImageRef<ImageGenericGFPoly> errorEvaluator, ImageArrayRef<int> errorLocations);
};

#endif // CX_REED_SOLOMON_DECODER_H
