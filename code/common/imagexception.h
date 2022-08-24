#ifndef IMAGE_EXCEPTION_H
#define IMAGE_EXCEPTION_H


#include <cxexception.h>


class ImageException : public CXException {
public:
  ImageException();
  ImageException(const char *msg);
  ~ImageException() CX_NOEXCEPT;
};


#endif // IMAGE_EXCEPTION_H
