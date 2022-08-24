#ifndef IMAGEQR_MATRIX_PARSER_H
#define IMAGEQR_MATRIX_PARSER_H



#include <imagexception.h>
#include <common/imagebitmatrix.h>
#include <common/imagecounted.h>
#include <common/imagearray.h>
#include <qrcode/imageqrversion.h>
#include <qrcode/imageqrformatinformation.h>


class ImageQRBitMatrixParser : public ImageCounted {
private:
  ImageRef<ImageBitMatrix> bitMatrix_;
  ImageRef<ImageQRVersion>parsedVersion_;
  ImageRef<ImageQRFormatInformation> parsedFormatInfo_;
  bool mirror_;

  int copyBit(size_t x, size_t y, int versionBits);

public:
  ImageQRBitMatrixParser(ImageRef<ImageBitMatrix> bitMatrix);
  ImageRef<ImageQRFormatInformation> readFormatInformation();
  ImageRef<ImageQRVersion>readVersion();
  ImageArrayRef<cx_byte> readCodewords();
  void remask();
  void setMirror(cx_boolean mirror);
  void mirror();

private:
  ImageQRBitMatrixParser(const ImageQRBitMatrixParser&);
  ImageQRBitMatrixParser& operator =(const ImageQRBitMatrixParser&);
  
};


#endif // ZXING_MATRIX_PARSER_H
