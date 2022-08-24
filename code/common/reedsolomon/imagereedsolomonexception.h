#ifndef IMAGEREED_SOLOMON_EXCEPTION_H
#define IMAGEREED_SOLOMON_EXCEPTION_H


#include <cxexception.h>
class ImageReedSolomonException : public CXException {
public:
  ImageReedSolomonException(const char *msg) CX_NOEXCEPT;
  ~ImageReedSolomonException() CX_NOEXCEPT;
};
#endif // IMAGEREED_SOLOMON_EXCEPTION_H
