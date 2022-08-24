#ifndef IMAGEQR_DATA_BLOCK_H
#define IMAGEQR_DATA_BLOCK_H


#include <vector>
#include <common/imagecounted.h>
#include <common/imagearray.h>
#include <qrcode/imageqrversion.h>
#include <qrcode/imageqrerrorcorrectionlevel.h>


class ImageQRDataBlock : public ImageCounted {
private:
  int numDataCodewords_;
  ImageArrayRef<cx_byte> codewords_;

  ImageQRDataBlock(int numDataCodewords, ImageArrayRef<cx_byte> codewords);

public:
  static std::vector<ImageRef<ImageQRDataBlock> >
  getDataBlocks(ImageArrayRef<cx_byte> rawCodewords, ImageRef<ImageQRVersion>version, ImageQRErrorCorrectionLevel &ecLevel);

  int getNumDataCodewords();
  ImageArrayRef<cx_byte> getCodewords();
};


#endif // IMAGEQR_DATA_BLOCK_H
