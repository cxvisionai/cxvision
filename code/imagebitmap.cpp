
#include <imagebitmap.h>


ImageBitmap::ImageBitmap(ImageRef<ImageBinarizer> imagebinarizer) : imagebinarizer_(imagebinarizer) {
}

ImageBitmap::~ImageBitmap() {
}

ImageRef<ImageBitArray> ImageBitmap::getBlackRow(int y, ImageRef<ImageBitArray> row) {
    return imagebinarizer_->getBlackRow(y, row);
}

ImageRef<ImageBitMatrix> ImageBitmap::getBlackMatrix() {
    return imagebinarizer_->getBlackMatrix();
}

int ImageBitmap::getWidth() const {
    return getimagesource()->getWidth();
}

int ImageBitmap::getHeight() const {
    return getimagesource()->getHeight();
}

ImageRef<ImageSource> ImageBitmap::getimagesource() const {
    return imagebinarizer_->getimagesource();
}

bool ImageBitmap::isCropSupported() const {
    return getimagesource()->isCropSupported();
}

ImageRef<ImageBitmap> ImageBitmap::crop(int left, int top, int width, int height) {
    return ImageRef<ImageBitmap> (new ImageBitmap(imagebinarizer_->createImageBinarizer(getimagesource()->crop(left, top, width, height))));
}

bool ImageBitmap::isRotateSupported() const {
    return getimagesource()->isRotateSupported();
}

ImageRef<ImageBitmap> ImageBitmap::rotateCounterClockwise() {
    return ImageRef<ImageBitmap> (new ImageBitmap(imagebinarizer_->createImageBinarizer(getimagesource()->rotateCounterClockwise())));
}

ImageRef<ImageBitmap> ImageBitmap::rotateCounterClockwise45()
{
    return ImageRef<ImageBitmap> (new ImageBitmap(imagebinarizer_->createImageBinarizer(getimagesource()->rotateCounterClockwise45())));
}
