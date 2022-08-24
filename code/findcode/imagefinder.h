#ifndef IMAGEFINDER_H
#define IMAGEFINDER_H

#include <imagebitmap.h>
#include <imageresult.h>
#include <imagecodetypes.h>

 class ImageFinder : public ImageCounted {
  protected:
//   ImageFinder() {}
  public:
   virtual ImageRef<ImageResult> decode(ImageRef<ImageBitmap> image);
   virtual ImageRef<ImageResult> decode(ImageRef<ImageBitmap> image, ImageCodeTypes itypes) = 0;
   virtual ~ImageFinder();
};

#endif // IMAGEFINDER_H
