#ifndef IMAGE_QRALIGNMENT_PATTERN_FINDER_H
#define IMAGE_QRALIGNMENT_PATTERN_FINDER_H


#include "imageqralignmentpattern.h"
#include <common/imagecounted.h>
#include <common/imagebitmatrix.h>
#include <imageresultpointcallback.h>
#include <vector>


class ImageQRAlignmentPatternFinder : public ImageCounted {
private:
  static int CENTER_QUORUM;
  static int MIN_SKIP;
  static int MAX_MODULES;

  ImageRef<ImageBitMatrix> image_;
  std::vector<ImageQRAlignmentPattern *> *possibleCenters_;
  int startX_;
  int startY_;
  int width_;
  int height_;
  float moduleSize_;

  static float centerFromEnd(std::vector<int> &stateCount, int end);
  bool foundPatternCross(std::vector<int> &stateCount);

  float crossCheckVertical(int startI, int centerJ, int maxCount, int originalStateCountTotal);

  ImageRef<ImageQRAlignmentPattern> handlePossibleCenter(std::vector<int> &stateCount, int i, int j);

public:
  ImageQRAlignmentPatternFinder(ImageRef<ImageBitMatrix> image, int startX, int startY, int width, int height,
                         float moduleSize, ImageRef<ImageResultPointCallback>const& callback);
  ~ImageQRAlignmentPatternFinder();
  ImageRef<ImageQRAlignmentPattern> find();
  
private:
  ImageQRAlignmentPatternFinder(const ImageQRAlignmentPatternFinder&);
  ImageQRAlignmentPatternFinder& operator =(const ImageQRAlignmentPatternFinder&);
  
  ImageRef<ImageResultPointCallback> callback_;
};

#endif // IMAGE_ALIGNMENT_PATTERN_FINDER_H
