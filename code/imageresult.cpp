#include <imageresult.h>

ImageResult::ImageResult(ImageRef<ImageString> text,
               ImageArrayRef<uint8_t> rawBytes,
               ImageArrayRef< ImageRef<ImageResultPoint> > imageresultpoints,
               ImagecodeFormat format, const std::string &charSet,
               ImageResultMetadata metadata) :
  text_(text), rawBytes_(rawBytes), imageresultpoints_(imageresultpoints), format_(format), charSet_(charSet), metadata_(metadata) {
}

ImageResult::~ImageResult() {
}

ImageRef<ImageString> ImageResult::getText() {
  return text_;
}

ImageArrayRef<uint8_t> ImageResult::getRawBytes() {
  return rawBytes_;
}

ImageArrayRef< ImageRef<ImageResultPoint> > const& ImageResult::getImageResultPoints() const {
  return imageresultpoints_;
}

ImageArrayRef< ImageRef<ImageResultPoint> >& ImageResult::getImageResultPoints() {
  return imageresultpoints_;
}

ImagecodeFormat ImageResult::getImagecodeFormat() const {
    return format_;
}

std::string ImageResult::getCharSet() const
{
    return charSet_;
}

ImageResultMetadata &ImageResult::getMetadata()
{
    return metadata_;
}
