#ifndef IMAGERESULT_H
#define IMAGERESULT_H


#include <string>
#include <common/imagearray.h>
#include <common/imagecounted.h>
#include <common/imagestr.h>
#include <imageresultpoint.h>
#include <imageresultmetadata.h>
#include <imagecodeformat.h>

class ImageResult : public ImageCounted {
private:
  ImageRef<ImageString> text_;
  ImageArrayRef<uint8_t> rawBytes_;
  ImageArrayRef< ImageRef<ImageResultPoint> > imageresultpoints_;
  ImagecodeFormat format_;
  std::string charSet_;
  ImageResultMetadata metadata_;

public:
  ImageResult(ImageRef<ImageString> text,
         ImageArrayRef<uint8_t> rawBytes,
         ImageArrayRef< ImageRef<ImageResultPoint> > imageresultpoints,
         ImagecodeFormat format, const std::string &charSet = "",
         ImageResultMetadata metadata = ImageResultMetadata());
  ~ImageResult();
  ImageRef<ImageString> getText();
  ImageArrayRef<uint8_t> getRawBytes();
  ImageArrayRef< ImageRef<ImageResultPoint> > const& getImageResultPoints() const;
  ImageArrayRef< ImageRef<ImageResultPoint> >& getImageResultPoints();
  ImagecodeFormat getImagecodeFormat() const;
  std::string getCharSet() const;
  ImageResultMetadata& getMetadata();

  friend std::ostream& operator<<(std::ostream &out, ImageResult& result);
};

#endif // ZXING_RESULT_H
