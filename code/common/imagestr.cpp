
#include <common/imagestr.h>

using std::string;

ImageString::ImageString(const std::string &text) :
  text_(text) {
}

ImageString::ImageString(int capacity) {
  text_.reserve(capacity);
}

const std::string& ImageString::getText() const {
  return text_;
}

char ImageString::charAt(int i) const { return text_[i]; }

int ImageString::size() const { return int(text_.size()); }

int ImageString::length() const { return int(text_.size()); }

ImageRef<ImageString> ImageString::substring(int i, int j) const {
  return ImageRef<ImageString>(new ImageString(text_.substr(i, j)));
}

void ImageString::append(const std::string &tail) {
  text_.append(tail);
}

void ImageString::append(char c) {
  text_.append(1,c);
}

std::ostream& operator << (std::ostream& out, ImageString const& s) {
  out << s.text_;
  return out;
}
