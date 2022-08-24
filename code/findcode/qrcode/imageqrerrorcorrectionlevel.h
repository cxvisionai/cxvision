#ifndef IMAGE_ERROR_CORRECTION_LEVEL_H
#define IMAGE_ERROR_CORRECTION_LEVEL_H


#include <cxexception.h>
#include <imagecounted.h>

class ImageQRErrorCorrectionLevel : public ImageCounted {
private:
  int ordinal_;
  int bits_;
  std::string name_;

  ImageQRErrorCorrectionLevel(int inOrdinal, int bits, char const* name);
  static ImageQRErrorCorrectionLevel *FOR_BITS[];
  static int N_LEVELS;
public:
  static ImageQRErrorCorrectionLevel L;
  static ImageQRErrorCorrectionLevel M;
  static ImageQRErrorCorrectionLevel Q;
  static ImageQRErrorCorrectionLevel H;

  ImageQRErrorCorrectionLevel(const ImageQRErrorCorrectionLevel& other);
  ImageQRErrorCorrectionLevel& operator=(const ImageQRErrorCorrectionLevel &other);

  int ordinal() const;
  int bits() const;
  std::string const& name() const;
  operator std::string const& () const;

  static ImageQRErrorCorrectionLevel& forBits(int bits);
};


#endif // IMAGE_ERROR_CORRECTION_LEVEL_H
