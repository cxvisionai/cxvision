
#include <imagedecoderresult.h>

using namespace std;
ImageDecoderResult::ImageDecoderResult(ImageArrayRef<cx_byte> rawBytes,
                             ImageRef<ImageString> text,
                             ImageArrayRef< ImageArrayRef<cx_byte> >& byteSegments,
                             string const& ecLevel, string charSet) :
  rawBytes_(rawBytes),
  text_(text),
  byteSegments_(byteSegments),
  ecLevel_(ecLevel), charSet_(charSet) {}

ImageDecoderResult::ImageDecoderResult(ImageArrayRef<cx_byte> rawBytes,
                             ImageRef<ImageString> text)
  : rawBytes_(rawBytes), text_(text),charSet_("") {}

ImageArrayRef<cx_byte> ImageDecoderResult::getRawBytes() {
  return rawBytes_;
}

ImageRef<ImageString> ImageDecoderResult::getText() {
    return text_;
}

string ImageDecoderResult::charSet()
{
    return charSet_;
}
