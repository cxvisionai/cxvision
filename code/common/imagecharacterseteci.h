#ifndef IMAGE_CHARACTERSET_ECI
#define IMAGE_CHARACTERSET_ECI


#include <map>
//#include <CodeType.h>
#include <imagecounted.h>
#include <vector>

class ImageCharacterSetECI : public ImageCounted {
private:
  static std::map<int, ImageCharacterSetECI*> VALUE_TO_ECI;
  static std::map<std::string, ImageCharacterSetECI*> NAME_TO_ECI;
  static std::vector<ImageCharacterSetECI*> ECItables;
  static bool inited;
  static bool init_tables();

  std::vector<int> values_;
  std::vector<std::string> names_;

  ImageCharacterSetECI(const std::vector<int> values, const std::vector<const char*> names);

  static void addCharacterSet(const std::vector<int> &value, const std::vector<const char*> &encodingNames);

public:
  char const* name() const;
  int getValue() const;

  static ImageCharacterSetECI* getCharacterSetECIByValue(int value);
  static ImageCharacterSetECI* getCharacterSetECIByName(std::string const& name);
  static void removeAllCharacterSets();
};



#endif // ZXING_CHARACTERSET_ECI

