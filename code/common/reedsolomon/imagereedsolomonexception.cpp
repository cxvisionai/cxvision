
#include <common/reedsolomon/imagereedsolomonexception.h>

ImageReedSolomonException::ImageReedSolomonException(const char *msg) CX_NOEXCEPT :
    CXException(msg) {
}
ImageReedSolomonException::~ImageReedSolomonException() CX_NOEXCEPT {
}
