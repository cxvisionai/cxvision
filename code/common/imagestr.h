#ifndef IMAGESTR_H
#define IMAGESTR_H

#include <string>
#include <iostream>
#include <common/imagecounted.h>

class ImageString;
std::ostream& operator << (std::ostream& out, ImageString const& s);

class ImageString : public ImageCounted {
private:
  std::string text_;
public:
  explicit ImageString(const std::string &text);
  explicit ImageString(int);
  char charAt(int) const;
  ImageRef<ImageString> substring(int, int = -1) const;
  const std::string& getText() const;
  int size() const;
  void append(std::string const& tail);
  void append(char c);
  int length() const;
  friend std::ostream& operator << (std::ostream& out, ImageString const& s);
};


#endif // imageSTR
