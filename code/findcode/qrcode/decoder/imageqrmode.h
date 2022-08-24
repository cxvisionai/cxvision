#ifndef IMAGEQR_MODE_H
#define IMAGEQR_MODE_H


#include <qrcode/imageqrversion.h>


class ImageQRMode : public ImageCounted
{
private:
  int characterCountBitsForVersions0To9_;
  int characterCountBitsForVersions10To26_;
  int characterCountBitsForVersions27AndHigher_;
  int bits_;
  std::string name_;

  ImageQRMode(int cbv0_9, int cbv10_26, int cbv27, int bits, const std::string &name);

public:
  ImageQRMode(const ImageQRMode& ImageQRMode);
  ImageQRMode();

  static ImageQRMode TERMINATOR;
  static ImageQRMode NUMERIC;
  static ImageQRMode ALPHANUMERIC;
  static ImageQRMode STRUCTURED_APPEND;
  static ImageQRMode BYTE;
  static ImageQRMode ECI;
  static ImageQRMode KANJI;
  static ImageQRMode FNC1_FIRST_POSITION;
  static ImageQRMode FNC1_SECOND_POSITION;
  static ImageQRMode HANZI;

  static ImageQRMode& forBits(int bits);
  int getCharacterCountBits(ImageRef<ImageQRVersion> version) const;
  int getBits() const { return bits_; }

  ImageQRMode& operator=(const ImageQRMode& other);
  bool operator==(const ImageQRMode& other);
  bool operator!=(const ImageQRMode& other);

  std::string getName() const {
      if(name_ == "")
          return "null";
      else
          return name_;
  }
};


#endif // IMAGEQR_MODEL_H
