
#include <imageresultmetadata.h>
#include <common/imagebytearray.h>


struct ImageResultMetadata::Value
{
    virtual ~Value() {}
    virtual int toInteger(int fallback) const {
        return fallback;
    }
    virtual std::string toString() const {
        return std::string();
    }
};

struct ImageResultMetadata::IntegerValue : public Value
{
    int value;
    explicit IntegerValue(int v) : value(v) {}
    int toInteger(int) const override {
        return value;
    }
};

struct ImageResultMetadata::StringValue : public Value
{
    std::string value;
    explicit StringValue(std::string v) : value(std::move(v)) {}
    std::string toString() const override {
        return value;
    }
};

int ImageResultMetadata::getInt(Key key, int fallbackValue) const
{
    std::map<Key, std::shared_ptr<Value>>::const_iterator it = _contents.find(key);
    return it != _contents.end() ? it->second->toInteger(fallbackValue) : fallbackValue;
}

std::string ImageResultMetadata::getString(Key key) const {
    std::map<Key, std::shared_ptr<Value>>::const_iterator it = _contents.find(key);
    return it != _contents.end() ? it->second->toString() : std::string();
}

void ImageResultMetadata::put(Key key, int value) {
    _contents[key] = std::make_shared<IntegerValue>(value);
}

void ImageResultMetadata::put(Key key, const std::string &value) {
    _contents[key] = std::make_shared<StringValue>(value);
}

void ImageResultMetadata::putAll(const ImageResultMetadata& other) {
    _contents.insert(other._contents.begin(), other._contents.end());
}

std::list<ImageResultMetadata::Key> ImageResultMetadata::keys() const
{
    std::list<Key> keys;
    for(std::map<Key, std::shared_ptr<Value>>::const_iterator it = _contents.begin(); it != _contents.end(); ++it) {
        keys.push_back(it->first);
    }

    return keys;
}

bool ImageResultMetadata::empty() const
{
    return _contents.empty();
}

std::string ImageResultMetadata::keyToString(Key key) const
{
    switch (key)
    {
    case OTHER:                         return "OTHER";
    case ORIENTATION:                   return "ORIENTATION";
    case BYTE_SEGMENTS:                 return "BYTE_SEGMENTS";
    case ERROR_CORRECTION_LEVEL:        return "ERROR_CORRECTION_LEVEL";
    case ISSUE_NUMBER:                  return "ISSUE_NUMBER";
    case SUGGESTED_PRICE:               return "SUGGESTED_PRICE";
    case POSSIBLE_COUNTRY:              return "POSSIBLE_COUNTRY";
    case UPC_EAN_EXTENSION:             return "UPC_EAN_EXTENSION";
    case PDF417_EXTRA_METADATA:         return "PDF417_EXTRA_METADATA";
    case STRUCTURED_APPEND_SEQUENCE:    return "STRUCTURED_APPEND_SEQUENCE";
    case STRUCTURED_APPEND_CODE_COUNT:  return "STRUCTURED_APPEND_CODE_COUNT";
    case STRUCTURED_APPEND_PARITY:      return "STRUCTURED_APPEND_PARITY";
    }
    return "UNKNOWN";
}

