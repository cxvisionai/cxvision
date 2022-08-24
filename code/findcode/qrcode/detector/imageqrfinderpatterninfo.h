#ifndef IMAGE_FINDER_PATTERN_INFO_H
#define IMAGE_FINDER_PATTERN_INFO_H

#include <detector/imageqrfinderpattern.h>
#include <common/imagecounted.h>
#include <common/imagearray.h>
#include <vector>


class ImageQRFinderPatternInfo : public ImageCounted {
private:
  ImageRef<ImageQRFinderPattern> bottomLeft_;
  ImageRef<ImageQRFinderPattern> topLeft_;
  ImageRef<ImageQRFinderPattern> topRight_;

public:
  ImageQRFinderPatternInfo(std::vector<ImageRef<ImageQRFinderPattern> > patternCenters);

  ImageRef<ImageQRFinderPattern> getBottomLeft();
  ImageRef<ImageQRFinderPattern> getTopLeft();
  ImageRef<ImageQRFinderPattern> getTopRight();
};

#endif // IMAGE_FINDER_PATTERN_INFO_H
