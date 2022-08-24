
#include <common/imagecharacterseteci.h>
#include <imagexception.h>
#include <cstdlib>

using std::string;

std::map<int, ImageCharacterSetECI*> ImageCharacterSetECI::VALUE_TO_ECI;
std::map<std::string, ImageCharacterSetECI*> ImageCharacterSetECI::NAME_TO_ECI;
std::vector<ImageCharacterSetECI*> ImageCharacterSetECI::ECItables;

bool ImageCharacterSetECI::inited = ImageCharacterSetECI::init_tables();

#define COMMA ,

#define ADD_CHARACTER_SET(VALUES, STRINGS) \
  { int values[] = {VALUES}; \
    const char *strings[] = {STRINGS}; \
    addCharacterSet(std::vector<int>(values, values + sizeof(values) / sizeof(values[0])), \
                    std::vector<const char*>(strings, strings + sizeof(strings) / sizeof(strings[0]))); }

bool ImageCharacterSetECI::init_tables() {
  ADD_CHARACTER_SET(1 COMMA 3, "ISO8859_1" COMMA "ISO-8859-1")
  ADD_CHARACTER_SET(0 COMMA 2, "Cp437");
  ADD_CHARACTER_SET(4, "ISO8859_2" COMMA "ISO-8859-2");
  ADD_CHARACTER_SET(5, "ISO8859_3" COMMA "ISO-8859-3");
  ADD_CHARACTER_SET(6, "ISO8859_4" COMMA "ISO-8859-4");
  ADD_CHARACTER_SET(7, "ISO8859_5" COMMA "ISO-8859-5");
  ADD_CHARACTER_SET(8, "ISO8859_6" COMMA "ISO-8859-6");
  ADD_CHARACTER_SET(9, "ISO8859_7" COMMA "ISO-8859-7");
  ADD_CHARACTER_SET(10, "ISO8859_8" COMMA "ISO-8859-8");
  ADD_CHARACTER_SET(11, "ISO8859_9" COMMA "ISO-8859-9");
  ADD_CHARACTER_SET(12, "ISO8859_10" COMMA "ISO-8859-10");
  ADD_CHARACTER_SET(13, "ISO8859_11" COMMA "ISO-8859-11");
  ADD_CHARACTER_SET(15, "ISO8859_13" COMMA "ISO-8859-13");
  ADD_CHARACTER_SET(16, "ISO8859_14" COMMA "ISO-8859-14");
  ADD_CHARACTER_SET(17, "ISO8859_15" COMMA "ISO-8859-15");
  ADD_CHARACTER_SET(18, "ISO8859_16" COMMA "ISO-8859-16");
  ADD_CHARACTER_SET(20, "SJIS" COMMA "Shift_JIS");
  ADD_CHARACTER_SET(21, "Cp1250" COMMA "windows-1250");
  ADD_CHARACTER_SET(22, "Cp1251" COMMA "windows-1251");
  ADD_CHARACTER_SET(23, "Cp1252" COMMA "windows-1252");
  ADD_CHARACTER_SET(24, "Cp1256" COMMA "windows-1256");
  ADD_CHARACTER_SET(25, "UnicodeBigUnmarked" COMMA "UTF-16BE" COMMA "UnicodeBig");
  ADD_CHARACTER_SET(26, "UTF8" COMMA "UTF-8");
  ADD_CHARACTER_SET(27 COMMA 170, "ASCII" COMMA "US-ASCII");
  ADD_CHARACTER_SET(28, "Big5");
  ADD_CHARACTER_SET(29, "GB18030" COMMA "GB2312" COMMA "EUC_CN" COMMA "GBK");
  ADD_CHARACTER_SET(30, "EUC_KR" COMMA "EUC-KR");

  std::atexit(removeAllCharacterSets);

  return true;
}

ImageCharacterSetECI::ImageCharacterSetECI(const std::vector<int> values, const std::vector<const char*> names)
  : ImageCounted(), values_(values)
{
    for(size_t i=0; i<names.size(); i++)
        names_.push_back(std::string(names[i]));
}

char const* ImageCharacterSetECI::name() const {
  return names_[0].c_str();
}

int ImageCharacterSetECI::getValue() const {
  return values_[0];
}

void ImageCharacterSetECI::addCharacterSet(const std::vector<int> &values, const std::vector<const char *> &names) {
  ImageCharacterSetECI* charSet = new ImageCharacterSetECI(values, names);
  for(size_t i=0; i<values.size(); i++) {
    VALUE_TO_ECI[values[i]] = charSet;
  }
  for(size_t i=0; i<names.size(); i++) {
    NAME_TO_ECI[std::string(names[i])] = charSet;
  }

  ECItables.push_back(charSet);
}

ImageCharacterSetECI* ImageCharacterSetECI::getCharacterSetECIByValue(int value) {
  if (value < 0 || value >= 900) {
    throw ImageException();
  }
  return VALUE_TO_ECI[value];
}

ImageCharacterSetECI* ImageCharacterSetECI::getCharacterSetECIByName(string const& name) {
  return NAME_TO_ECI[name];
}

void ImageCharacterSetECI::removeAllCharacterSets()
{
    VALUE_TO_ECI.clear();
    NAME_TO_ECI.clear();

    for (std::vector<ImageCharacterSetECI*>::iterator i=ECItables.begin();i!=ECItables.end();i++)
    {
        delete *i;
    }
    ECItables.clear();

    inited=false;
 }


