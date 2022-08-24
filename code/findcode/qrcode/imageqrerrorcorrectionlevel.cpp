#include <qrcode/imageqrerrorcorrectionlevel.h>
#include <imagexception.h>

using std::string;

ImageQRErrorCorrectionLevel::ImageQRErrorCorrectionLevel(int inOrdinal,
                                           int bits,
                                           char const* name) :
    ordinal_(inOrdinal), bits_(bits), name_(name)
{}

ImageQRErrorCorrectionLevel::ImageQRErrorCorrectionLevel(const ImageQRErrorCorrectionLevel &other) :
    ordinal_(other.ordinal()), bits_(other.bits()), name_(other.name())
{}

int ImageQRErrorCorrectionLevel::ordinal() const {
  return ordinal_;
}

int ImageQRErrorCorrectionLevel::bits() const {
  return bits_;
}

string const& ImageQRErrorCorrectionLevel::name() const {
  return name_;
}

ImageQRErrorCorrectionLevel::operator string const& () const {
  return name_;
}

ImageQRErrorCorrectionLevel& ImageQRErrorCorrectionLevel::operator=(const ImageQRErrorCorrectionLevel &other)
{
    ordinal_ = other.ordinal();
    bits_ = other.bits();
    name_ = other.name();

    return *this;
}

ImageQRErrorCorrectionLevel& ImageQRErrorCorrectionLevel::forBits(int bits) {
  if (bits < 0 || bits >= N_LEVELS) {
    throw ImageException("Ellegal error correction level bits");
  }
  return *FOR_BITS[bits];
}

  ImageQRErrorCorrectionLevel ImageQRErrorCorrectionLevel::L(0, 0x01, "L");
  ImageQRErrorCorrectionLevel ImageQRErrorCorrectionLevel::M(1, 0x00, "M");
  ImageQRErrorCorrectionLevel ImageQRErrorCorrectionLevel::Q(2, 0x03, "Q");
  ImageQRErrorCorrectionLevel ImageQRErrorCorrectionLevel::H(3, 0x02, "H");
ImageQRErrorCorrectionLevel *ImageQRErrorCorrectionLevel::FOR_BITS[] = { &M, &L, &H, &Q };
int ImageQRErrorCorrectionLevel::N_LEVELS = 4;


