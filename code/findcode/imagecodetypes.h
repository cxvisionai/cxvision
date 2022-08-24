#ifndef IMAGEDECODETYPES_H
#define IMAGEDECODETYPES_H

#include <imagecodeformat.h>
#include <imageresultpointcallback.h>

#include <set>

typedef unsigned int ImageCodeType;
class ImageCodeTypes;
ImageCodeTypes operator | (ImageCodeTypes const&, ImageCodeTypes const&);
ImageCodeTypes operator & (ImageCodeTypes const&, ImageCodeTypes const&);

class ImageCodeTypes {
 private:
  ImageCodeType itypes;
  ImageRef<ImageResultPointCallback> callback;
  std::set<int> allowedEanExtensions;

 public:
  static const ImageCodeType AZTEC_HINT;
  static const ImageCodeType CODABAR_HINT;
  static const ImageCodeType CODE_39_HINT;
  static const ImageCodeType CODE_93_HINT;
  static const ImageCodeType CODE_128_HINT;
  static const ImageCodeType DATA_MATRIX_HINT;
  static const ImageCodeType EAN_8_HINT;
  static const ImageCodeType EAN_13_HINT;
  static const ImageCodeType ITF_HINT;
  static const ImageCodeType MAXICODE_HINT;
  static const ImageCodeType PDF_417_HINT;
  static const ImageCodeType QR_CODE_HINT;
  static const ImageCodeType RSS_14_HINT;
  static const ImageCodeType RSS_EXPANDED_HINT;
  static const ImageCodeType UPC_A_HINT;
  static const ImageCodeType UPC_E_HINT;
  static const ImageCodeType UPC_EAN_EXTENSION_HINT;
  static const ImageCodeType ASSUME_GS1;

  static const ImageCodeType TRYHARDER_HINT;
  static const ImageCodeType CHARACTER_SET;
  // static const ImageCodeType ALLOWED_LENGTHS = 1 << 29;
  // static const ImageCodeType ASSUME_CODE_39_CHECK_DIGIT = 1 << 28;
  // static const ImageCodeType NEED_RESULT_POINT_CALLBACK = 1 << 26;
  
  static const ImageCodeTypes PRODUCT_HINT;
  static const ImageCodeTypes ONED_HINT;
  static const ImageCodeTypes DEFAULT_HINT;

  ImageCodeTypes();
  ImageCodeTypes(const ImageCodeType &init);
  ImageCodeTypes(const ImageCodeTypes &other);

  void addFormat(ImagecodeFormat toadd);
  bool containsFormat(ImagecodeFormat tocheck) const;
  bool isEmpty() const {return (itypes==0);}
  void clear() {itypes=0;}
  void setTryHarder(bool toset);
  bool getTryHarder() const;

  void setAllowedEanExtensions(std::set<int> toset);
  std::set<int> getAllowedEanExtensions() const;

  void setResultPointCallback(ImageRef<ImageResultPointCallback> const&);
  ImageRef<ImageResultPointCallback> getResultPointCallback() const;

  ImageCodeTypes& operator =(ImageCodeTypes const &other);

  friend ImageCodeTypes operator| (ImageCodeTypes const&, ImageCodeTypes const&);
  friend ImageCodeTypes operator& (ImageCodeTypes const&, ImageCodeTypes const&);
};


#endif // IMAGEDECODETYPES_H

