#ifndef CX_EXCEPTION_H
#define CX_EXCEPTION_H

#include <string>
#include <exception>
#define CX_NULLPTR nullptr
#define CX_NOEXCEPT noexcept
class CXException : public std::exception {
private:
  char const* const message;

public:
  CXException() CX_NOEXCEPT;
  CXException(const char* msg) CX_NOEXCEPT;
  CXException(CXException const& that) CX_NOEXCEPT;
  ~CXException() CX_NOEXCEPT;
  char const* what() const CX_NOEXCEPT;

private:
  static char const* copy(char const*);
  void deleteMessage();
};


#endif // CX_EXCEPTION_H
