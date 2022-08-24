
#include <qrcode/detector/imageqrfinderpatterninfo.h>


ImageQRFinderPatternInfo::ImageQRFinderPatternInfo(std::vector<ImageRef<ImageQRFinderPattern> > patternCenters) :
    bottomLeft_(patternCenters[0]), topLeft_(patternCenters[1]), topRight_(patternCenters[2]) {
}

ImageRef<ImageQRFinderPattern> ImageQRFinderPatternInfo::getBottomLeft() {
  return bottomLeft_;
}
ImageRef<ImageQRFinderPattern> ImageQRFinderPatternInfo::getTopLeft() {
  return topLeft_;
}
ImageRef<ImageQRFinderPattern> ImageQRFinderPatternInfo::getTopRight() {
  return topRight_;
}

