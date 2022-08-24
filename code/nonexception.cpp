#include <nonexception.h>

NoneException::NoneException()  CX_NOEXCEPT {
}

NoneException::NoneException(const char *msg) CX_NOEXCEPT
    : CXException(msg) {
}

NoneException::~NoneException()  CX_NOEXCEPT {
}
