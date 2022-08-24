#ifndef NONE_EXCEPTION_H
#define NONE_EXCEPTION_H


#include <cxexception.h>

class NoneException : public CXException {
public:
  NoneException() CX_NOEXCEPT;
  NoneException(const char *msg) CX_NOEXCEPT;
  ~NoneException() CX_NOEXCEPT;
};

#endif // NONE_EXCEPTION_H
