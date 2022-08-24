#include <imagefinderexception.h>

ImageFinderException::ImageFinderException()  CX_NOEXCEPT {
}

ImageFinderException::ImageFinderException(const char *msg) CX_NOEXCEPT
    : CXException(msg) {
}

ImageFinderException::~ImageFinderException()  CX_NOEXCEPT {
}
