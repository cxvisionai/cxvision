
#include <qrcode/detector/imageqralignmentpattern.h>

using std::abs;

ImageQRAlignmentPattern::ImageQRAlignmentPattern(float posX, float posY, float estimatedModuleSize) :
    ImageResultPoint(posX,posY), estimatedModuleSize_(estimatedModuleSize) {
}

bool ImageQRAlignmentPattern::aboutEquals(float moduleSize, float i, float j) const {
  if (abs(i - getY()) <= moduleSize && abs(j - getX()) <= moduleSize) {
    float moduleSizeDiff = abs(moduleSize - estimatedModuleSize_);
    return moduleSizeDiff <= 1.0f || moduleSizeDiff <= estimatedModuleSize_;
  }
  return false;
}

ImageRef<ImageQRAlignmentPattern> ImageQRAlignmentPattern::combineEstimate(float i, float j, float newModuleSize) const {
  float combinedX = (getX() + j) / 2.0f;
  float combinedY = (getY() + i) / 2.0f;
  float combinedModuleSize = (estimatedModuleSize_ + newModuleSize) / 2.0f;
  ImageRef<ImageQRAlignmentPattern> result
      (new ImageQRAlignmentPattern(combinedX, combinedY, combinedModuleSize));
  return result;
}
