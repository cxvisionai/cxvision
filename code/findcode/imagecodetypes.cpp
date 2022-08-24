
#include <imagecodetypes.h>
#include <imagexception.h>
#include <qglobal.h>
#include <iterator>


const ImageCodeType ImageCodeTypes::AZTEC_HINT = 1 << ImagecodeFormat::AZTEC;
const ImageCodeType ImageCodeTypes::CODABAR_HINT = 1 << ImagecodeFormat::CODABAR;
const ImageCodeType ImageCodeTypes::CODE_39_HINT = 1 << ImagecodeFormat::CODE_39;
const ImageCodeType ImageCodeTypes::CODE_93_HINT = 1 << ImagecodeFormat::CODE_93;
const ImageCodeType ImageCodeTypes::CODE_128_HINT = 1 << ImagecodeFormat::CODE_128;
const ImageCodeType ImageCodeTypes::DATA_MATRIX_HINT = 1 << ImagecodeFormat::DATA_MATRIX;
const ImageCodeType ImageCodeTypes::EAN_8_HINT = 1 << ImagecodeFormat::EAN_8;
const ImageCodeType ImageCodeTypes::EAN_13_HINT = 1 << ImagecodeFormat::EAN_13;
const ImageCodeType ImageCodeTypes::ITF_HINT = 1 << ImagecodeFormat::ITF;
const ImageCodeType ImageCodeTypes::MAXICODE_HINT = 1 << ImagecodeFormat::MAXICODE;
const ImageCodeType ImageCodeTypes::PDF_417_HINT = 1 << ImagecodeFormat::PDF_417;
const ImageCodeType ImageCodeTypes::QR_CODE_HINT = 1 << ImagecodeFormat::QR_CODE;
const ImageCodeType ImageCodeTypes::RSS_14_HINT = 1 << ImagecodeFormat::RSS_14;
const ImageCodeType ImageCodeTypes::RSS_EXPANDED_HINT = 1 << ImagecodeFormat::RSS_EXPANDED;
const ImageCodeType ImageCodeTypes::UPC_A_HINT = 1 << ImagecodeFormat::UPC_A;
const ImageCodeType ImageCodeTypes::UPC_E_HINT = 1 << ImagecodeFormat::UPC_E;
const ImageCodeType ImageCodeTypes::UPC_EAN_EXTENSION_HINT = 1 << ImagecodeFormat::UPC_EAN_EXTENSION;
const ImageCodeType ImageCodeTypes::ASSUME_GS1 = 1 << ImagecodeFormat::ASSUME_GS1;
const ImageCodeType ImageCodeTypes::TRYHARDER_HINT = 1 << 31;
const ImageCodeType ImageCodeTypes::CHARACTER_SET = 1 << 30;

const ImageCodeTypes ImageCodeTypes::PRODUCT_HINT(
  ImageCodeTypes::UPC_A_HINT |
  ImageCodeTypes::UPC_E_HINT |
  ImageCodeTypes::EAN_13_HINT |
  ImageCodeTypes::EAN_8_HINT |
  ImageCodeTypes::RSS_14_HINT
  );

const ImageCodeTypes ImageCodeTypes::ONED_HINT(
  ImageCodeTypes::CODE_39_HINT |
  ImageCodeTypes::CODE_93_HINT |
  ImageCodeTypes::CODE_128_HINT |
  ImageCodeTypes::ITF_HINT |
  ImageCodeTypes::CODABAR_HINT |
  ImageCodeTypes::PRODUCT_HINT
  );

const ImageCodeTypes ImageCodeTypes::DEFAULT_HINT(
  ImageCodeTypes::ONED_HINT |
  ImageCodeTypes::QR_CODE_HINT |
  ImageCodeTypes::DATA_MATRIX_HINT |
  ImageCodeTypes::AZTEC_HINT |
  ImageCodeTypes::PDF_417_HINT
  );

ImageCodeTypes::ImageCodeTypes() {
  itypes = 0;
  allowedEanExtensions = {};
}

ImageCodeTypes::ImageCodeTypes(const ImageCodeType &init) {
    itypes = init;
}

ImageCodeTypes::ImageCodeTypes(const ImageCodeTypes &other) {
    itypes = other.itypes;
    callback = other.callback;
    allowedEanExtensions = other.allowedEanExtensions;
}

void ImageCodeTypes::addFormat(ImagecodeFormat toadd) {
  switch (toadd) {
  case ImagecodeFormat::AZTEC: itypes |= AZTEC_HINT; break;
  case ImagecodeFormat::CODABAR: itypes |= CODABAR_HINT; break;
  case ImagecodeFormat::CODE_39: itypes |= CODE_39_HINT; break;
  case ImagecodeFormat::CODE_93: itypes |= CODE_93_HINT; break;
  case ImagecodeFormat::CODE_128: itypes |= CODE_128_HINT; break;
  case ImagecodeFormat::DATA_MATRIX: itypes |= DATA_MATRIX_HINT; break;
  case ImagecodeFormat::EAN_8: itypes |= EAN_8_HINT; break;
  case ImagecodeFormat::EAN_13: itypes |= EAN_13_HINT; break;
  case ImagecodeFormat::ITF: itypes |= ITF_HINT; break;
  case ImagecodeFormat::MAXICODE: itypes |= MAXICODE_HINT; break;
  case ImagecodeFormat::PDF_417: itypes |= PDF_417_HINT; break;
  case ImagecodeFormat::QR_CODE: itypes |= QR_CODE_HINT; break;
  case ImagecodeFormat::RSS_14: itypes |= RSS_14_HINT; break;
  case ImagecodeFormat::RSS_EXPANDED: itypes |= RSS_EXPANDED_HINT; break;
  case ImagecodeFormat::UPC_A: itypes |= UPC_A_HINT; break;
  case ImagecodeFormat::UPC_E: itypes |= UPC_E_HINT; break;
  case ImagecodeFormat::UPC_EAN_EXTENSION: itypes |= UPC_EAN_EXTENSION_HINT; break;
  case ImagecodeFormat::ASSUME_GS1: itypes |= ASSUME_GS1; break;
  default: throw ImageException("Unrecognizd barcode format");
  }
}

bool ImageCodeTypes::containsFormat(ImagecodeFormat tocheck) const {
  ImageCodeType checkAgainst = 0;
  switch (tocheck) {
  case ImagecodeFormat::AZTEC: checkAgainst |= AZTEC_HINT; break;
  case ImagecodeFormat::CODABAR: checkAgainst |= CODABAR_HINT; break;
  case ImagecodeFormat::CODE_39: checkAgainst |= CODE_39_HINT; break;
  case ImagecodeFormat::CODE_93: checkAgainst |= CODE_93_HINT; break;
  case ImagecodeFormat::CODE_128: checkAgainst |= CODE_128_HINT; break;
  case ImagecodeFormat::DATA_MATRIX: checkAgainst |= DATA_MATRIX_HINT; break;
  case ImagecodeFormat::EAN_8: checkAgainst |= EAN_8_HINT; break;
  case ImagecodeFormat::EAN_13: checkAgainst |= EAN_13_HINT; break;
  case ImagecodeFormat::ITF: checkAgainst |= ITF_HINT; break;
  case ImagecodeFormat::MAXICODE: checkAgainst |= MAXICODE_HINT; break;
  case ImagecodeFormat::PDF_417: checkAgainst |= PDF_417_HINT; break;
  case ImagecodeFormat::QR_CODE: checkAgainst |= QR_CODE_HINT; break;
  case ImagecodeFormat::RSS_14: checkAgainst |= RSS_14_HINT; break;
  case ImagecodeFormat::RSS_EXPANDED: checkAgainst |= RSS_EXPANDED_HINT; break;
  case ImagecodeFormat::UPC_A: checkAgainst |= UPC_A_HINT; break;
  case ImagecodeFormat::UPC_E: checkAgainst |= UPC_E_HINT; break;
  case ImagecodeFormat::UPC_EAN_EXTENSION: checkAgainst |= UPC_EAN_EXTENSION_HINT; break;
  case ImagecodeFormat::ASSUME_GS1: checkAgainst |= ASSUME_GS1; break;
  default: throw ImageException("Unrecognizd barcode format");
  }
  return (itypes & checkAgainst) != 0;
}

void ImageCodeTypes::setTryHarder(bool toset) {
  if (toset) {
    itypes |= TRYHARDER_HINT;
  } else {
    itypes &= ~TRYHARDER_HINT;
  }
}

bool ImageCodeTypes::getTryHarder() const {
  return (itypes & TRYHARDER_HINT) != 0;
}

void ImageCodeTypes::setAllowedEanExtensions(std::set<int> toset) {
  allowedEanExtensions = toset;
}

std::set<int> ImageCodeTypes::getAllowedEanExtensions() const {
  return allowedEanExtensions;
}

void ImageCodeTypes::setResultPointCallback(ImageRef<ImageResultPointCallback> const& _callback) {
  callback = _callback;
}

ImageRef<ImageResultPointCallback> ImageCodeTypes::getResultPointCallback() const {
    return callback;
}

ImageCodeTypes &ImageCodeTypes::operator =(const ImageCodeTypes &other)
{
    itypes = other.itypes;
    callback = other.callback;
    allowedEanExtensions = other.allowedEanExtensions;
    return *this;
}

ImageCodeTypes operator | (ImageCodeTypes const& l, ImageCodeTypes const& r) {
  ImageCodeTypes result (l);
  result.itypes |= r.itypes;
  if (!result.callback) {
    result.callback = r.callback;
  }

  result.allowedEanExtensions = l.allowedEanExtensions;
  result.allowedEanExtensions.insert(r.allowedEanExtensions.begin(),
                                     r.allowedEanExtensions.end());

  return result;
}

ImageCodeTypes operator & (ImageCodeTypes const& l, ImageCodeTypes const& r) {
  ImageCodeTypes result (l);
  result.itypes &= r.itypes;
  if (!result.callback) {
    result.callback = r.callback;
  }

  std::set<int> intersect;
  std::set_intersection(l.allowedEanExtensions.begin(), l.allowedEanExtensions.end(),
                        r.allowedEanExtensions.begin(), r.allowedEanExtensions.end(),
                        std::inserter(intersect, intersect.begin()));

  result.allowedEanExtensions = intersect;

  return result;
}
