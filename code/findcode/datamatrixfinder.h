#ifndef DATA_MATRIX_FINDER_H
#define DATA_MATRIX_FINDER_H


#include <imagefinder.h>
#include <imagecodetypes.h>
#include <datamatrix/decoder/imagedatamatrixdecoder.h>


class DataMatrixFinder : public ImageFinder {
private:
  ImageDataMatrixDecoder decoder_;

public:
  DataMatrixFinder();
  virtual ImageRef<ImageResult> decode(ImageRef<ImageBitmap> image, ImageCodeTypes hints);
  virtual ~DataMatrixFinder();

};


#endif // DATA_MATRIX_FINDER_H
