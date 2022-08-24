#ifndef IMAGE_QRDETECTOR_H
#define IMAGE_QRDETECTOR_H



#include <common/imagecounted.h>
#include <imagedetectorresult.h>
#include <common/imagebitmatrix.h>
#include <qrcode/detector/imageqralignmentpattern.h>
#include <common/imageperspectivetransform.h>
#include <imageresultpointcallback.h>
#include <qrcode/detector/imageqrfinderpatterninfo.h>


class ImageCodeTypes;

class ImageQRDetector : public ImageCounted {
private:
  ImageRef<ImageBitMatrix> image_;
  ImageRef<ImageResultPointCallback> callback_;

protected:
  ImageRef<ImageBitMatrix> getImage() const;
  ImageRef<ImageResultPointCallback> getResultPointCallback() const;

  static ImageRef<ImageBitMatrix> sampleGrid(ImageRef<ImageBitMatrix> image, int dimension, ImageRef<ImagePerspectiveTransform>);
  static int computeDimension(ImageRef<ImageResultPoint> topLeft, ImageRef<ImageResultPoint> topRight, ImageRef<ImageResultPoint> bottomLeft,
                              float moduleSize);
  float calculateModuleSize(ImageRef<ImageResultPoint> topLeft, ImageRef<ImageResultPoint> topRight, ImageRef<ImageResultPoint> bottomLeft);
  float calculateModuleSizeOneWay(ImageRef<ImageResultPoint> pattern, ImageRef<ImageResultPoint> otherPattern);
  float sizeOfBlackWhiteBlackRunBothWays(int fromX, int fromY, int toX, int toY);
  float sizeOfBlackWhiteBlackRun(int fromX, int fromY, int toX, int toY);
  ImageRef<ImageQRAlignmentPattern> findAlignmentInRegion(float overallEstModuleSize, int estAlignmentX, int estAlignmentY,
      float allowanceFactor);
  ImageRef<ImageDetectorResult> processFinderPatternInfo(ImageRef<ImageQRFinderPatternInfo> info);
public:
  virtual ImageRef<ImagePerspectiveTransform> createTransform(ImageRef<ImageResultPoint> topLeft, ImageRef<ImageResultPoint> topRight, ImageRef <
      ImageResultPoint > bottomLeft, ImageRef<ImageResultPoint> alignmentPattern, int dimension);

  ImageQRDetector(ImageRef<ImageBitMatrix> image);
  ImageRef<ImageDetectorResult> detect(ImageCodeTypes const& hints);


};

#endif // IMAGE_DETECTOR_H
