
#include <imagegreyrotatedsource.h>
#include <common/imagexception.h>

// Note that dataWidth and dataHeight are not reversed, as we need to
// be able to traverse the greyData correctly, which does not get
// rotated.
ImageGreyRotatedSource::
ImageGreyRotatedSource(ImageArrayRef<cx_byte> greyData,
                                int dataWidth, int dataHeight,
                                int left, int top,
                                int width, int height)
    : Super(width, height),
      greyData_(greyData),
      dataWidth_(dataWidth),
      left_(left), top_(top) {
  // Intentionally comparing to the opposite dimension since we're rotated.
  if (left + width > dataHeight || top + height > dataWidth) {
    throw ImageException("Crop rectangle does not fit within image data.");
  }
}

// The API asks for rows, but we're rotated, so we return columns.
ImageArrayRef<cx_byte>
ImageGreyRotatedSource::getRow(int y, ImageArrayRef<cx_byte> row) const {
  if (y < 0 || y >= getHeight()) {
    throw ImageException("Requested row is outside the image.");
  }
  if (!row || row->size() < getWidth()) {
    row = ImageArrayRef<cx_byte>(getWidth());
  }
  int offset = (left_ * dataWidth_) + (dataWidth_ - 1 - (y + top_));
  using namespace std;
  if (false) {
    cerr << offset << " = "
         << top_ << " " << left_ << " "
         << getHeight() << " " << getWidth() << " "
         << y << endl;
  }
  for (int x = 0; x < getWidth(); x++) {
    row[x] = greyData_[offset];
    offset += dataWidth_;
  }
  return row;
}

ImageArrayRef<cx_byte> ImageGreyRotatedSource::getMatrix() const {
  ImageArrayRef<cx_byte> result (getWidth() * getHeight());
  for (int y = 0; y < getHeight(); y++) {
    cx_byte* row = &result[y * getWidth()];
    int offset = (left_ * dataWidth_) + (dataWidth_ - 1 - (y + top_));
    for (int x = 0; x < getWidth(); x++) {
      row[x] = greyData_[offset];
      offset += dataWidth_;
    }
  }
  return result;
}
