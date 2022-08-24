#ifndef IMAGEQR_DATA_MASK_H
#define IMAGEQR_DATA_MASK_H


#include <common/imagearray.h>
#include <common/imagecounted.h>
#include <common/imagebitmatrix.h>

#include <vector>


class ImageQRDataMask : public ImageCounted {
private:
  static std::vector<ImageRef<ImageQRDataMask> > DATA_MASKS;

protected:

public:
  static int buildDataMasks();
  ImageQRDataMask();
  virtual ~ImageQRDataMask();
  void unmaskBitMatrix(ImageBitMatrix& matrix, size_t dimension);
  virtual bool isMasked(size_t x, size_t y) = 0;
  static ImageQRDataMask& forReference(int reference);
};



#endif // IMAGEQR_DATA_MASK_H
