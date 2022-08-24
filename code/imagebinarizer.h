#ifndef IMAGEBINARIZER_H_
#define IMAGEBINARIZER_H_


#include <imagesource.h>
#include <imagebitarray.h>
#include <common/imagebitmatrix.h>
#include <common/imagecounted.h>


class ImageBinarizer : public ImageCounted {
 private:
  ImageRef<ImageSource> source_;

 public:
  ImageBinarizer(ImageRef<ImageSource> source);
  virtual ~ImageBinarizer();

  virtual ImageRef<ImageBitArray> getBlackRow(int y, ImageRef<ImageBitArray> row) = 0;
  virtual ImageRef<ImageBitMatrix> getBlackMatrix() = 0;

  ImageRef<ImageSource> getimagesource() const ;
  virtual ImageRef<ImageBinarizer> createImageBinarizer(ImageRef<ImageSource> source) = 0;

  int getWidth() const;
  int getHeight() const;

};

#endif /* IMAGEBINARIZER_H_ */
