#ifndef IMAGEHISTOGRAMBINARIZER_H
#define IMAGEHISTOGRAMBINARIZER_H

#include <imagebinarizer.h>
#include <common/imagebitarray.h>
#include <common/imagebitmatrix.h>
#include <common/imagearray.h>

class ImageHistogramBinarizer : public ImageBinarizer {
private:
  ImageArrayRef<cx_byte> image_;
  ImageArrayRef<int> buckets;
public:
  ImageHistogramBinarizer(ImageRef<ImageSource> source);
  virtual ~ImageHistogramBinarizer();
		
  virtual ImageRef<ImageBitArray> getBlackRow(int y, ImageRef<ImageBitArray> row);
  virtual ImageRef<ImageBitMatrix> getBlackMatrix();
  static int estimateBlackPoint(ImageArrayRef<int> const& buckets);
  ImageRef<ImageBinarizer> createImageBinarizer(ImageRef<ImageSource> source);
private:
  void initArrays(int imageSize);
};


#endif //IMAGEHISTOGRAMBINARIZER_H
