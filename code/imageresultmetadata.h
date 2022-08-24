#ifndef IMAGE_RESULT_METADATA_H
#define IMAGE_RESULT_METADATA_H


#include <string>
#include <map>
#include <memory>
#include <list>

class ImageByteArray;

class ImageResultMetadata
{
public:

    enum Key {
        OTHER,
        ORIENTATION,
        BYTE_SEGMENTS,
        ERROR_CORRECTION_LEVEL,
        ISSUE_NUMBER,
        SUGGESTED_PRICE,
        POSSIBLE_COUNTRY,
        UPC_EAN_EXTENSION,
        PDF417_EXTRA_METADATA,
        STRUCTURED_APPEND_SEQUENCE,
        STRUCTURED_APPEND_CODE_COUNT,
        STRUCTURED_APPEND_PARITY
    };

    int getInt(Key key, int fallbackValue = 0) const;
    std::string getString(Key key) const;
	
    void put(Key key, int value);
    void put(Key key, const std::string& value);

    void putAll(const ImageResultMetadata& other);

    std::list<Key> keys() const;
    std::string keyToString(Key key) const;

    bool empty() const;

private:
    struct Value;
    struct IntegerValue;
    struct StringValue;

    std::map<Key, std::shared_ptr<Value>> _contents;
};


#endif
