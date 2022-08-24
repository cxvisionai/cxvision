#ifndef QR_CODE_FINDER_H
#define QR_CODE_FINDER_H


#include <imagefinder.h>
#include <decoder/imageqrdecoder.h>
#include <imagecodetypes.h>

class QRCodeFinder : public ImageFinder {
 private:
  ImageQRDecoder decoder_;
			
 protected:
  ImageQRDecoder& getImageDecoder();

 public:
  QRCodeFinder();
  virtual ~QRCodeFinder();
			
  ImageRef<ImageResult> decode(ImageRef<ImageBitmap> image, ImageCodeTypes itypes);
};


#endif // QR_CODE_FINDER_H
