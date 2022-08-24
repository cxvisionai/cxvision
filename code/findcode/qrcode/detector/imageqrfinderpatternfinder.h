#ifndef IMAGEQR_FINDER_PATTERN_FINDER_H
#define IMAGEQR_FINDER_PATTERN_FINDER_H


#include <imageqrfinderpattern.h>
#include <imageqrfinderpatterninfo.h>
#include <imagecounted.h>
#include <imagebitmatrix.h>
#include <imageresultpointcallback.h>
#include <vector>


class ImageCodeTypes;


class ImageQRFinderPatternFinder {
private:
  static int CENTER_QUORUM;

protected:
  static int MIN_SKIP;
  static int MAX_MODULES;

  ImageRef<ImageBitMatrix> image_;
  std::vector<ImageRef<ImageQRFinderPattern> > possibleCenters_;
  bool hasSkipped_;

  ImageRef<ImageResultPointCallback> callback_;
  mutable int crossCheckStateCount[5];

  /** stateCount must be int[5] */
  static float centerFromEnd(int* stateCount, int end);
  static bool foundPatternCross(int* stateCount);

  float crossCheckVertical(size_t startI, size_t centerJ, int maxCount, int originalStateCountTotal);
  float crossCheckHorizontal(size_t startJ, size_t centerI, int maxCount, int originalStateCountTotal);

  /** stateCount must be int[5] */
  bool handlePossibleCenter(int* stateCount, size_t i, size_t j);
  int findRowSkip();
  bool haveMultiplyConfirmedCenters();
  std::vector<ImageRef<ImageQRFinderPattern> > selectBestPatterns();
  static std::vector<ImageRef<ImageQRFinderPattern> > orderBestPatterns(std::vector<ImageRef<ImageQRFinderPattern> > patterns);

  ImageRef<ImageBitMatrix> getImage();
  std::vector<ImageRef<ImageQRFinderPattern> >& getPossibleCenters();

  bool crossCheckDiagonal(int startI, int centerJ, int maxCount, int originalStateCountTotal) const;
  int *getCrossCheckStateCount() const;

  static double squaredDistance(ImageRef<ImageQRFinderPattern> a, ImageRef<ImageQRFinderPattern> b);

public:
  static float distance(ImageRef<ImageResultPoint> p1, ImageRef<ImageResultPoint> p2);
  ImageQRFinderPatternFinder(ImageRef<ImageBitMatrix> image, ImageRef<ImageResultPointCallback>const&);
  ImageRef<ImageQRFinderPatternInfo> find(ImageCodeTypes const& hints);
};


#endif // IMAGEQR_FINDER_PATTERN_FINDER_H
