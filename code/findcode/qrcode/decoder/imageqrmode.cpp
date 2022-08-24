
#include <imageqrmode.h>
#include <imagexception.h>
#include <sstream>

using std::ostringstream;


ImageQRMode ImageQRMode::TERMINATOR(0, 0, 0, 0x00, "TERMINATOR");
ImageQRMode ImageQRMode::NUMERIC(10, 12, 14, 0x01, "NUMERIC");
ImageQRMode ImageQRMode::ALPHANUMERIC(9, 11, 13, 0x02, "ALPHANUMERIC");
ImageQRMode ImageQRMode::STRUCTURED_APPEND(0, 0, 0, 0x03, "STRUCTURED_APPEND");
ImageQRMode ImageQRMode::BYTE(8, 16, 16, 0x04, "BYTE");
ImageQRMode ImageQRMode::ECI(0, 0, 0, 0x07, "ECI");
ImageQRMode ImageQRMode::KANJI(8, 10, 12, 0x08, "KANJI");
ImageQRMode ImageQRMode::FNC1_FIRST_POSITION(0, 0, 0, 0x05, "FNC1_FIRST_POSITION");
ImageQRMode ImageQRMode::FNC1_SECOND_POSITION(0, 0, 0, 0x09, "FNC1_SECOND_POSITION");
ImageQRMode ImageQRMode::HANZI(8, 10, 12, 0x0D, "HANZI");

ImageQRMode::ImageQRMode() :
    characterCountBitsForVersions0To9_(0),
    characterCountBitsForVersions10To26_(0),
    characterCountBitsForVersions27AndHigher_(0),
    bits_(0),
    name_("")
{}

ImageQRMode::ImageQRMode(int cbv0_9, int cbv10_26, int cbv27, int bits, const std::string &name) :
    characterCountBitsForVersions0To9_(cbv0_9), characterCountBitsForVersions10To26_(cbv10_26),
    characterCountBitsForVersions27AndHigher_(cbv27), bits_(bits), name_(name)
{
}

ImageQRMode::ImageQRMode(const ImageQRMode &ImageQRMode)
{
    characterCountBitsForVersions0To9_ = ImageQRMode.characterCountBitsForVersions0To9_;
    characterCountBitsForVersions10To26_ = ImageQRMode.characterCountBitsForVersions10To26_;
    characterCountBitsForVersions27AndHigher_ = ImageQRMode.characterCountBitsForVersions27AndHigher_;
    bits_ = ImageQRMode.bits_;
    name_ = ImageQRMode.name_;
}

ImageQRMode& ImageQRMode::forBits(int bits) {
    switch (bits) {
    case 0x0:
        return TERMINATOR;
    case 0x1:
        return NUMERIC;
    case 0x2:
        return ALPHANUMERIC;
    case 0x3:
        return STRUCTURED_APPEND;
    case 0x4:
        return BYTE;
    case 0x5:
        return FNC1_FIRST_POSITION;
    case 0x7:
        return ECI;
    case 0x8:
        return KANJI;
    case 0x9:
        return FNC1_SECOND_POSITION;
    case 0xD:
        // 0xD is defined in GBT 18284-2000, may not be supported in foreign country
        return HANZI;
    default:
        ostringstream s;
        s << "Illegal ImageQRMode bits: " << bits;
        throw ImageException(s.str().c_str());
    }
}

int ImageQRMode::getCharacterCountBits(ImageRef<ImageQRVersion> version) const
{
    int number = version->getVersionNumber();
    if (number <= 9) {
        return characterCountBitsForVersions0To9_;
    } else if (number <= 26) {
        return characterCountBitsForVersions10To26_;
    } else {
        return characterCountBitsForVersions27AndHigher_;
    }
}

ImageQRMode& ImageQRMode::operator=(const ImageQRMode& other)
{
    characterCountBitsForVersions0To9_ = other.characterCountBitsForVersions0To9_;
    characterCountBitsForVersions10To26_ = other.characterCountBitsForVersions10To26_;
    characterCountBitsForVersions27AndHigher_ = other.characterCountBitsForVersions27AndHigher_;
    bits_ = other.bits_;
    name_ = other.name_;

    return *this;
}

bool ImageQRMode::operator==(const ImageQRMode& other)
{
    return ( characterCountBitsForVersions0To9_ == other.characterCountBitsForVersions0To9_
            && characterCountBitsForVersions10To26_ == other.characterCountBitsForVersions10To26_
            && characterCountBitsForVersions27AndHigher_ == other.characterCountBitsForVersions27AndHigher_
            && name_ == other.name_
             && bits_ == other.bits_ );
}

bool ImageQRMode::operator!=(const ImageQRMode &other)
{
    return !(operator==(other));
}
