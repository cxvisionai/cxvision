
#include <cxexception.h>
#include <string.h>


CXException::CXException() CX_NOEXCEPT
    : message(CX_NULLPTR) {
}

CXException::CXException(const char *msg) CX_NOEXCEPT
    : message(copy(msg)) {
}

CXException::CXException(const CXException &that) CX_NOEXCEPT
    : std::exception(that),
      message(copy(that.message)) {
}

CXException::~CXException() CX_NOEXCEPT {
    if(message) {
        deleteMessage();
    }
}

const char *CXException::what() const CX_NOEXCEPT {
    return message ? message : "";
}

void CXException::deleteMessage() {
    delete [] message;
}

char const* CXException::copy(char const* msg) {
    char* message = CX_NULLPTR;
    if (msg) {
        size_t l = strlen(msg)+1;
        if (l) {
            message = new char[l];
            strcpy(message, msg);
        }
    }
    return message;
}
