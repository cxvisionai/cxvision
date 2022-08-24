
#include <imagefinder.h>

ImageFinder::~ImageFinder() { }

ImageRef<ImageResult> ImageFinder::decode(ImageRef<ImageBitmap> image) {
  return decode(image, ImageCodeTypes::DEFAULT_HINT);
}
