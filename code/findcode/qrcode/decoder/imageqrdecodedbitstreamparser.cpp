
#include <qrcode/decoder/imageqrdecodedbitstreamparser.h>
#include <common/imagecharacterseteci.h>
#include <imagexception.h>
#include <imagestringutils.h>
#include <imagebitsource.h>
#include <iostream>
#ifndef NO_ICONV
#include <iconv.h>
#endif

// Required for compatibility. TODO : test on Symbian
#ifdef ZXING_ICONV_CONST
#undef ICONV_CONST
#define ICONV_CONST const
#endif

#ifndef ICONV_CONST
#define ICONV_CONST /**/
#endif
#include <qglobal.h>


using namespace std;

const char ImageQRDecodedBitStreamParser::ALPHANUMERIC_CHARS[] =
{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B',
  'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
  'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
  'Y', 'Z', ' ', '$', '%', '*', '+', '-', '.', '/', ':'
};

namespace {int GB2312_SUBSET = 1;}

void ImageQRDecodedBitStreamParser::append(std::string &result,
                                    string const& in,
                                    const char *src) {
    append(result, (cx_byte const*)in.c_str(), in.length(), src);
}

void ImageQRDecodedBitStreamParser::append(std::string &result,
                                    const cx_byte *bufIn,
                                    size_t nIn,
                                    const char *src) {
#ifndef NO_ICONV
    if (nIn == 0) {
        return;
    }

    iconv_t cd = iconv_open(ImageStringUtils::UTF8, src);
    if (cd == (iconv_t)-1) {
        result.append((const char *)bufIn, nIn);
        return;
    }

    const int maxOut = 4 * int(nIn) + 1;
    char* bufOut = new char[maxOut];

    ICONV_CONST char *fromPtr = (ICONV_CONST char *)bufIn;
    size_t nFrom = nIn;
    char *toPtr = (char *)bufOut;
    size_t nTo = maxOut;

    while (nFrom > 0) {
#if defined(Q_OS_SYMBIAN)
    size_t oneway = iconv(cd,(const char**) &fromPtr, &nFrom, &toPtr, &nTo);
#else
    size_t oneway = iconv(cd,(char**) &fromPtr, &nFrom, &toPtr, &nTo);
#endif
        if (oneway == (size_t)(-1)) {
            iconv_close(cd);
            delete[] bufOut;
            throw ImageException("error converting characters");
        }
    }
    iconv_close(cd);

    int nResult = maxOut - int(nTo);
    bufOut[nResult] = '\0';
    result.append((const char *)bufOut);
    delete[] bufOut;
#else
    Q_UNUSED(src);
    result.append((const char *)bufIn, nIn);
#endif
}

void ImageQRDecodedBitStreamParser::decodeHanziSegment(ImageRef<ImageBitSource> bits_,
                                                string& result,
                                                int count) {
    ImageBitSource& bits (*bits_);
    // Don't crash trying to read more bits than we have available.
    if (count * 13 > bits.available()) {
        throw ImageException();
    }

    // Each character will require 2 bytes. Read the characters as 2-byte pairs
    // and decode as GB2312 afterwards
    size_t nBytes = 2 * count;
    cx_byte* buffer = new cx_byte[nBytes];
    int offset = 0;
    while (count > 0) {
        // Each 13 bits encodes a 2-byte character
        int twoBytes = bits.readBits(13);
        int assembledTwoBytes = ((twoBytes / 0x060) << 8) | (twoBytes % 0x060);
        if (assembledTwoBytes < 0x003BF) {
            // In the 0xA1A1 to 0xAAFE range
            assembledTwoBytes += 0x0A1A1;
        } else {
            // In the 0xB0A1 to 0xFAFE range
            assembledTwoBytes += 0x0A6A1;
        }
        buffer[offset] = (cx_byte) ((assembledTwoBytes >> 8) & 0xFF);
        buffer[offset + 1] = (cx_byte) (assembledTwoBytes & 0xFF);
        offset += 2;
        count--;
    }

    try {
        append(result, buffer, nBytes, ImageStringUtils::GB2312);
    } catch (ImageException const& ignored) {
        (void)ignored;
        delete [] buffer;
        throw ImageException();
    }

    delete [] buffer;
}

void ImageQRDecodedBitStreamParser::decodeKanjiSegment(ImageRef<ImageBitSource> bits, std::string &result, int count) {
    // Each character will require 2 bytes. Read the characters as 2-byte pairs
    // and decode as Shift_JIS afterwards
    size_t nBytes = 2 * count;
    cx_byte* buffer = new cx_byte[nBytes];
    int offset = 0;
    while (count > 0) {
        // Each 13 bits encodes a 2-byte character

        int twoBytes = bits->readBits(13);
        int assembledTwoBytes = ((twoBytes / 0x0C0) << 8) | (twoBytes % 0x0C0);
        if (assembledTwoBytes < 0x01F00) {
            // In the 0x8140 to 0x9FFC range
            assembledTwoBytes += 0x08140;
        } else {
            // In the 0xE040 to 0xEBBF range
            assembledTwoBytes += 0x0C140;
        }
        buffer[offset] = (cx_byte)(assembledTwoBytes >> 8);
        buffer[offset + 1] = (cx_byte)assembledTwoBytes;
        offset += 2;
        count--;
    }
    try {
        append(result, buffer, nBytes, ImageStringUtils::SHIFT_JIS);
    } catch (ImageException const& ignored) {
        (void)ignored;
        delete [] buffer;
        throw ImageException();
    }
    delete[] buffer;
}

std::string ImageQRDecodedBitStreamParser::decodeByteSegment(ImageRef<ImageBitSource> bits_,
                                                      string& result,
                                                      int count,
                                                      ImageCharacterSetECI const * currentCharacterSetECI,
                                                      ImageArrayRef< ImageArrayRef<cx_byte> >& byteSegments,
                                                      Hashtable const& hints) {
    int nBytes = count;
    ImageBitSource& bits (*bits_);
    // Don't crash trying to read more bits than we have available.
    if (count << 3 > bits.available()) {
        throw ImageException();
    }

    ImageArrayRef<cx_byte> bytes_ (count);
    cx_byte* readBytes = &(*bytes_)[0];
    for (int i = 0; i < count; i++) {
        readBytes[i] = (cx_byte) bits.readBits(8);
    }
    string encoding;
    if (currentCharacterSetECI == 0) {
        // The spec isn't clear on this mode; see
        // section 6.4.5: t does not say which encoding to assuming
        // upon decoding. I have seen ISO-8859-1 used as well as
        // Shift_JIS -- without anything like an ECI designator to
        // give a hint.
        encoding = ImageStringUtils::guessEncoding(readBytes, count, hints);
    } else {
        encoding = currentCharacterSetECI->name();
    }
    try {
        append(result, readBytes, nBytes, encoding.c_str());
    } catch (ImageException const& ignored) {
        (void)ignored;
        throw ImageException();
    }
    byteSegments->values().push_back(bytes_);
    return encoding;
}

void ImageQRDecodedBitStreamParser::decodeNumericSegment(ImageRef<ImageBitSource> bits, std::string &result, int count) {
    int nBytes = count;
    cx_byte* bytes = new cx_byte[nBytes];
    int i = 0;
    // Read three digits at a time
    while (count >= 3) {
        // Each 10 bits encodes three digits
        if (bits->available() < 10) {
            delete[] bytes;
            throw ImageException("format exception");
        }
        int threeDigitsBits = bits->readBits(10);
        if (threeDigitsBits >= 1000) {
            ostringstream s;
            s << "Illegal value for 3-digit unit: " << threeDigitsBits;
            delete[] bytes;
            throw ImageException(s.str().c_str());
        }
        bytes[i++] = ALPHANUMERIC_CHARS[threeDigitsBits / 100];
        bytes[i++] = ALPHANUMERIC_CHARS[(threeDigitsBits / 10) % 10];
        bytes[i++] = ALPHANUMERIC_CHARS[threeDigitsBits % 10];
        count -= 3;
    }
    if (count == 2) {
        if (bits->available() < 7) {
            throw ImageException("format exception");
        }
        // Two digits left over to read, encoded in 7 bits
        int twoDigitsBits = bits->readBits(7);
        if (twoDigitsBits >= 100) {
            ostringstream s;
            s << "Illegal value for 2-digit unit: " << twoDigitsBits;
            delete[] bytes;
            throw ImageException(s.str().c_str());
        }
        bytes[i++] = ALPHANUMERIC_CHARS[twoDigitsBits / 10];
        bytes[i++] = ALPHANUMERIC_CHARS[twoDigitsBits % 10];
    } else if (count == 1) {
        if (bits->available() < 4) {
            throw ImageException("format exception");
        }
        // One digit left over to read
        int digitBits = bits->readBits(4);
        if (digitBits >= 10) {
            ostringstream s;
            s << "Illegal value for digit unit: " << digitBits;
            delete[] bytes;
            throw ImageException(s.str().c_str());
        }
        bytes[i++] = ALPHANUMERIC_CHARS[digitBits];
    }
    append(result, bytes, nBytes, ImageStringUtils::ASCII);
    delete[] bytes;
}

char ImageQRDecodedBitStreamParser::toAlphaNumericChar(size_t value) {
    if (value >= sizeof(ImageQRDecodedBitStreamParser::ALPHANUMERIC_CHARS)) {
        throw ImageException();
    }
    return ALPHANUMERIC_CHARS[value];
}

void ImageQRDecodedBitStreamParser::decodeAlphanumericSegment(ImageRef<ImageBitSource> bits_,
                                                       string& result,
                                                       int count,
                                                       bool fc1InEffect) {
    ImageBitSource& bits (*bits_);
    ostringstream bytes;
    // Read two characters at a time
    while (count > 1) {
        if (bits.available() < 11) {
            throw ImageException();
        }
        int nextTwoCharsBits = bits.readBits(11);
        bytes << toAlphaNumericChar(nextTwoCharsBits / 45);
        bytes << toAlphaNumericChar(nextTwoCharsBits % 45);
        count -= 2;
    }
    if (count == 1) {
        // special case: one character left
        if (bits.available() < 6) {
            throw ImageException();
        }
        bytes << toAlphaNumericChar(bits.readBits(6));
    }
    // See section 6.4.8.1, 6.4.8.2
    string s = bytes.str();
    if (fc1InEffect) {
        // We need to massage the result a bit if in an FNC1 mode:
        ostringstream r;
        for (size_t i = 0; i < s.length(); i++) {
            if (s[i] != '%') {
                r << s[i];
            } else {
                if (i < s.length() - 1 && s[i + 1] == '%') {
                    // %% is rendered as %
                    r << s[i++];
                } else {
                    // In alpha mode, % should be converted to FNC1 separator 0x1D
                    r << (cx_byte)0x1D;
                }
            }
        }
        s = r.str();
    }
    append(result, s, ImageStringUtils::ASCII);
}

namespace {
int parseECIValue(ImageBitSource& bits) {
    int firstByte = bits.readBits(8);
    if ((firstByte & 0x80) == 0) {
        // just one byte
        return firstByte & 0x7F;
    }
    if ((firstByte & 0xC0) == 0x80) {
        // two bytes
        int secondByte = bits.readBits(8);
        return ((firstByte & 0x3F) << 8) | secondByte;
    }
    if ((firstByte & 0xE0) == 0xC0) {
        // three bytes
        int secondThirdBytes = bits.readBits(16);
        return ((firstByte & 0x1F) << 16) | secondThirdBytes;
    }
    throw ImageException();
}
}

ImageRef<ImageDecoderResult>
ImageQRDecodedBitStreamParser::decode(ImageArrayRef<cx_byte> bytes,
                               ImageRef<ImageQRVersion> version,
                               ImageQRErrorCorrectionLevel const& ecLevel,
                               Hashtable const& hints) {
    ImageRef<ImageBitSource> bits_ (new ImageBitSource(bytes));
    ImageBitSource& bits (*bits_);
    string result;
    result.reserve(50);
    ImageArrayRef< ImageArrayRef<cx_byte> > byteSegments (0);
    const ImageCharacterSetECI* currentCharacterSetECI = 0;
    string charSet = "";
    try {
        bool fc1InEffect = false;
        ImageQRMode* mode = 0;
        do {
            // While still another segment to read...
            if (bits.available() < 4) {
                // OK, assume we're done. Really, a TERMINATOR mode should have been recorded here
                mode = &ImageQRMode::TERMINATOR;
            } else {
                try {
                    mode = &ImageQRMode::forBits(bits.readBits(4)); // mode is encoded by 4 bits
                } catch (ImageException const& iae) {
                    throw iae;
                    // throw FormatException.getFormatInstance();
                }
            }
            if (mode != &ImageQRMode::TERMINATOR) {
                if ((mode == &ImageQRMode::FNC1_FIRST_POSITION) || (mode == &ImageQRMode::FNC1_SECOND_POSITION)) {
                    // We do little with FNC1 except alter the parsed result a bit according to the spec
                    fc1InEffect = true;
                } else if (mode == &ImageQRMode::STRUCTURED_APPEND) {
                    if (bits.available() < 16) {
                        throw ImageException();
                    }
                    // not really supported; all we do is ignore it
                    // Read next 8 bits (symbol sequence #) and 8 bits (parity data), then continue
                    bits.readBits(16);
                } else if (mode == &ImageQRMode::ECI) {
                    // Count doesn't apply to ECI
                    int value = parseECIValue(bits);
                    currentCharacterSetECI = ImageCharacterSetECI::getCharacterSetECIByValue(value);
                    if (currentCharacterSetECI == 0) {
                        throw ImageException();
                    }
                } else {
                    // First handle Hanzi mode which does not start with character count
                    if (mode == &ImageQRMode::HANZI) {
                        //chinese mode contains a sub set indicator right after mode indicator
                        int subset = bits.readBits(4);
                        int countHanzi = bits.readBits(mode->getCharacterCountBits(version));
                        if (subset == GB2312_SUBSET) {
                            decodeHanziSegment(bits_, result, countHanzi);
                        }
                    } else {
                        // "Normal" QR code modes:
                        // How many characters will follow, encoded in this mode?
                        int count = bits.readBits(mode->getCharacterCountBits(version));
                        if (mode == &ImageQRMode::NUMERIC) {
                            decodeNumericSegment(bits_, result, count);
                        } else if (mode == &ImageQRMode::ALPHANUMERIC) {
                            decodeAlphanumericSegment(bits_, result, count, fc1InEffect);
                        } else if (mode == &ImageQRMode::BYTE) {
                            charSet = decodeByteSegment(bits_, result, count, currentCharacterSetECI, byteSegments, hints);
                        } else if (mode == &ImageQRMode::KANJI) {
                            decodeKanjiSegment(bits_, result, count);
                        } else {
                            throw ImageException();
                        }
                    }
                }
            }
        } while (mode != &ImageQRMode::TERMINATOR);
    } catch (ImageException const& iae) {
        (void)iae;
        // from readBits() calls
        throw ImageException();
    }
    return ImageRef<ImageDecoderResult>(new ImageDecoderResult(bytes, ImageRef<ImageString>(new ImageString(result)), byteSegments, (string)ecLevel, charSet));
}

