
#include <imagegreysource.h>
#include <imagegreyrotatedsource.h>
#include <common/imagexception.h>

ImageGreySource::
ImageGreySource(ImageArrayRef<cx_byte> greyData,
                         int dataWidth, int dataHeight,
                         int left, int top,
                         int width, int height) 
    : Super(width, height),
      greyData_(greyData),
      dataWidth_(dataWidth), dataHeight_(dataHeight),
      left_(left), top_(top) {

  if (left + width > dataWidth || top + height > dataHeight || top < 0 || left < 0) {
    throw ImageException("Crop rectangle does not fit within image data.");
  }
}

ImageArrayRef<cx_byte> ImageGreySource::getRow(int y, ImageArrayRef<cx_byte> row) const {
  if (y < 0 || y >= this->getHeight()) {
    throw ImageException("Requested row is outside the image.");
  }
  int width = getWidth();
  if (!row || row->size() < width) {
    ImageArrayRef<cx_byte> temp (width);
    row = temp;
  }
  int offset = (y + top_) * dataWidth_ + left_;
  memcpy(&row[0], &greyData_[offset], width);
  return row;
}

ImageArrayRef<cx_byte> ImageGreySource::getMatrix() const {
  if (left_ == 0 && top_ == 0 && dataWidth_ == getWidth() && dataHeight_ == getHeight())
    return greyData_;

  int size = getWidth() * getHeight();
  ImageArrayRef<cx_byte> result (size);
  for (int row = 0; row < getHeight(); row++) {
    memcpy(&result[row * getWidth()], &greyData_[(top_ + row) * dataWidth_ + left_], getWidth());
  }
  return result;
}

ImageRef<ImageSource> ImageGreySource::rotateCounterClockwise() const {
  // Intentionally flip the left, top, width, and height arguments as
  // needed. dataWidth and dataHeight are always kept unrotated.
  ImageRef<ImageSource> result (
      new ImageGreyRotatedSource(greyData_,
                                          dataWidth_, dataHeight_,
                                          top_, left_, getHeight(), getWidth()));
  return result;
}


