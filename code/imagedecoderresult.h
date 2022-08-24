#ifndef IMAGE_DECODER_RESULT_H
#define IMAGE_DECODER_RESULT_H


#include <common/imagecounted.h>
#include <common/imagearray.h>
#include <string>
#include <common/imagestr.h>


class ImageDecoderResult : public ImageCounted {
private:
  ImageArrayRef<cx_byte> rawBytes_;
  ImageRef<ImageString> text_;
  ImageArrayRef< ImageArrayRef<cx_byte> > byteSegments_;
  std::string ecLevel_;
  std::string charSet_;

public:
  ImageDecoderResult(ImageArrayRef<cx_byte> rawBytes,
                ImageRef<ImageString> text,
                ImageArrayRef< ImageArrayRef<cx_byte> >& byteSegments,
                std::string const& ecLevel,
                std::string charSet = "");

  ImageDecoderResult(ImageArrayRef<cx_byte> rawBytes, ImageRef<ImageString> text);

  ImageArrayRef<cx_byte> getRawBytes();
  ImageRef<ImageString> getText();
  std::string charSet();
};



#endif // IMAGE_DECODER_RESULT_H
