#include "imagecounted.h"

ImageCounted::ImageCounted() :
    count_(0)
{
}

ImageCounted::~ImageCounted()
{
}

ImageCounted *ImageCounted::retain()
{
    count_++;
    return this;
}

void ImageCounted::release()
{
    count_--;
    if (count_ == 0) {
      count_ = 0x0F001;
      delete this;
    }
}

size_t ImageCounted::count() const
{
    return count_;
}


