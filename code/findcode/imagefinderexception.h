#ifndef IMAGEFINDER_EXCEPTION_H
#define IMAGEFINDER_EXCEPTION_H

#include <cxexception.h>


class ImageFinderException : public CXException {
 public:
  ImageFinderException() CX_NOEXCEPT;
  ImageFinderException(char const* msg) CX_NOEXCEPT;
  ~ImageFinderException() CX_NOEXCEPT;
};


#endif // IMAGEFINDER_EXCEPTION_H
