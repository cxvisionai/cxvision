
#include <common/imagexception.h>


ImageException::ImageException() : CXException() {}
ImageException::ImageException(const char *msg) : CXException(msg) {}
ImageException::~ImageException() CX_NOEXCEPT {}
