#ifndef IMAGEQR_FORMAT_INFORMATION_H
#define IMAGEQR_FORMAT_INFORMATION_H


#include <imageqrerrorcorrectionlevel.h>
#include <imagecounted.h>
#include <iostream>


class ImageQRFormatInformation : public ImageCounted {
private:
  static int FORMAT_INFO_MASK_QR;
  static int FORMAT_INFO_DECODE_LOOKUP[][2];
  static int N_FORMAT_INFO_DECODE_LOOKUPS;
  static int BITS_SET_IN_HALF_BYTE[];

  ImageQRErrorCorrectionLevel &errorCorrectionLevel_;
  char dataMask_;

  ImageQRFormatInformation(int formatInfo);

public:
  static int numBitsDiffering(int a, int b);
  static ImageRef<ImageQRFormatInformation> decodeFormatInformation(int maskedFormatInfo1, int maskedFormatInfo2);
  static ImageRef<ImageQRFormatInformation> doDecodeFormatInformation(int maskedFormatInfo1, int maskedFormatInfo2);
  ImageQRErrorCorrectionLevel &getErrorCorrectionLevel();
  char getDataMask();
  friend bool operator==(const ImageQRFormatInformation &a, const ImageQRFormatInformation &b);
  friend std::ostream& operator<<(std::ostream& out, const ImageQRFormatInformation& fi);
};

#endif // IMAGEQR_FORMAT_INFORMATION_H
