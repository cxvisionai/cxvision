
#include <imageqrfinderpattern.h>

using std::abs;

ImageQRFinderPattern::ImageQRFinderPattern(float posX, float posY, float estimatedModuleSize)
    : ImageResultPoint(posX,posY), estimatedModuleSize_(estimatedModuleSize), count_(1) {}
		
ImageQRFinderPattern::ImageQRFinderPattern(float posX, float posY, float estimatedModuleSize, int count)
    : ImageResultPoint(posX,posY), estimatedModuleSize_(estimatedModuleSize), count_(count) {}
		
int ImageQRFinderPattern::getCount() const {
  return count_;
}
		
float ImageQRFinderPattern::getEstimatedModuleSize() const {
  return estimatedModuleSize_;
}
		
void ImageQRFinderPattern::incrementCount() {
  count_++;
  // cerr << "ic " << getX() << " " << getY() << " " << count_ << endl;
}
		
/*
  bool ImageQRFinderPattern::aboutEquals(float moduleSize, float i, float j) const {
  return abs(i - posY_) <= moduleSize && abs(j - posX_) <= moduleSize && (abs(moduleSize - estimatedModuleSize_)
  <= 1.0f || abs(moduleSize - estimatedModuleSize_) / estimatedModuleSize_ <= 0.1f);
  }
*/
bool ImageQRFinderPattern::aboutEquals(float moduleSize, float i, float j) const {
  if (abs(i - getY()) <= moduleSize && abs(j - getX()) <= moduleSize) {
    float moduleSizeDiff = abs(moduleSize - estimatedModuleSize_);
    return moduleSizeDiff <= 1.0f || moduleSizeDiff <= estimatedModuleSize_;
  }
  return false;
}
		
ImageRef<ImageQRFinderPattern> ImageQRFinderPattern::combineEstimate(float i, float j, float newModuleSize) const {
  // fprintf(stderr, "ce %f %f %f\n", i, j, newModuleSize);

  int combinedCount = count_ + 1;
  float combinedX = (count_ * getX() + j) / combinedCount;
  float combinedY = (count_ * getY() + i) / combinedCount;
  float combinedModuleSize = (count_ * getEstimatedModuleSize() + newModuleSize) / combinedCount;
  return ImageRef<ImageQRFinderPattern>(new ImageQRFinderPattern(combinedX, combinedY, combinedModuleSize, combinedCount));
}
