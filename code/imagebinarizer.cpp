#include <imagebinarizer.h>

    ImageBinarizer::ImageBinarizer(ImageRef<ImageSource> source) : source_(source) {
  }
	
    ImageBinarizer::~ImageBinarizer() {
	}
	
    ImageRef<ImageSource> ImageBinarizer::getimagesource() const {
		return source_;
	}

  int ImageBinarizer::getWidth() const {
    return source_->getWidth();
  }
	
  int ImageBinarizer::getHeight() const {
    return source_->getHeight();
  }
	

