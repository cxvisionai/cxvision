
#ifndef IMAGEQR_DECODED_BIT_STREAM_PARSER_H
#define IMAGEQR_DECODED_BIT_STREAM_PARSER_H


#include <string>
#include <sstream>
#include <map>
#include <imageqrmode.h>
#include <common/imagebitsource.h>
#include <common/imagecounted.h>
#include <common/imagearray.h>
#include <imagedecoderresult.h>
#include <common/imagecharacterseteci.h>
#include <imagecodetypes.h>
#include <imageqrversion.h>
class ImageQRDecodedBitStreamParser {
public:
  typedef std::map<ImageCodeType, std::string> Hashtable;

private:
  static char const ALPHANUMERIC_CHARS[];
  static char toAlphaNumericChar(size_t value);

  static void decodeHanziSegment(ImageRef<ImageBitSource> bits, std::string &result, int count);
  static void decodeKanjiSegment(ImageRef<ImageBitSource> bits, std::string &result, int count);
  static void decodeByteSegment(ImageRef<ImageBitSource> bits, std::string &result, int count);
  static std::string decodeByteSegment(ImageRef<ImageBitSource> bits_,
                                std::string& result,
                                int count,
                                ImageCharacterSetECI const *currentCharacterSetECI,
                                ImageArrayRef<ImageArrayRef<cx_byte> > &byteSegments,
                                Hashtable const& hints);
  static void decodeAlphanumericSegment(ImageRef<ImageBitSource> bits, std::string &result, int count, bool fc1InEffect);
  static void decodeNumericSegment(ImageRef<ImageBitSource> bits, std::string &result, int count);

  static void append(std::string &ost, const cx_byte *bufIn, size_t nIn, const char *src);
  static void append(std::string &ost, std::string const& in, const char *src);

public:
  static ImageRef<ImageDecoderResult> decode(ImageArrayRef<cx_byte> bytes,
                                   ImageRef<ImageQRVersion>version,
                                   ImageQRErrorCorrectionLevel const& ecLevel,
                                   Hashtable const& hints);
};


#endif // IMAGEQR_DECODED_BIT_STREAM_PARSER_H
