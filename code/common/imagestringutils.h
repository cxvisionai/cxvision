
#ifndef IMAGE_STRING_UTILS_H
#define IMAGE_STRING_UTILS_H


#include <string>
#include <map>
#include <imagecodetypes.h>
//#include <common/Types.h>


class ImageStringUtils {
private:
  static char const* const PLATFORM_DEFAULT_ENCODING;

  ImageStringUtils() {}

public:
  static char const* const ASCII;
  static char const* const SHIFT_JIS;
  static char const* const GB2312;
  static char const* const EUC_JP;
  static char const* const UTF8;
  static char const* const ISO88591;
  static const bool ASSUME_SHIFT_JIS;

  typedef std::map<ImageCodeType, std::string> Hashtable;

  static std::string guessEncoding(cx_byte *bytes, int length, Hashtable const& hints);

  static std::string intToStr(int number);
};


#endif // IMAGE_STRING_UTILS_H

