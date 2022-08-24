#ifndef IMAGECODE_FORMAT_H
#define IMAGECODE_FORMAT_H


class ImagecodeFormat {
public:

  enum Value {
    NONE,
    AZTEC,
    CODABAR,
    CODE_39,
    CODE_93,
    CODE_128,
    DATA_MATRIX,
    EAN_8,
    EAN_13,
    ITF,
    MAXICODE,
    PDF_417,
    QR_CODE,
    RSS_14,
    RSS_EXPANDED,
    UPC_A,
    UPC_E,
    UPC_EAN_EXTENSION,
    ASSUME_GS1
  };

  ImagecodeFormat(Value v) : value(v) {}
  const Value value;
  operator Value () const {return value;}

  static char const* imagecodeformatnames[];
};


#endif // IMAGECODE_FORMAT_H
