
#include <imageqrdetector.h>
#include <imageqrfinderpattern.h>
#include <imageqrfinderpatternfinder.h>
#include <imageqralignmentpattern.h>
#include <imageqralignmentpatternfinder.h>
#include <imageqrversion.h>
#include <imagegridsampler.h>
#include <imagecodetypes.h>
#include <detector/mathtools.h>
#include <sstream>
#include <cstdlib>
#include <algorithm>



ImageQRDetector::ImageQRDetector(ImageRef<ImageBitMatrix> image) :
  image_(image) {
}

ImageRef<ImageBitMatrix> ImageQRDetector::getImage() const {
  return image_;
}

ImageRef<ImageResultPointCallback> ImageQRDetector::getResultPointCallback() const {
  return callback_;
}

ImageRef<ImageDetectorResult> ImageQRDetector::detect(ImageCodeTypes const& hints) {
  callback_ = hints.getResultPointCallback();
  ImageQRFinderPatternFinder finder(image_, hints.getResultPointCallback());
  ImageRef<ImageQRFinderPatternInfo> info(finder.find(hints));
  return processFinderPatternInfo(info);
}

ImageRef<ImageDetectorResult> ImageQRDetector::processFinderPatternInfo(ImageRef<ImageQRFinderPatternInfo> info){
  ImageRef<ImageQRFinderPattern> topLeft(info->getTopLeft());
  ImageRef<ImageQRFinderPattern> topRight(info->getTopRight());
  ImageRef<ImageQRFinderPattern> bottomLeft(info->getBottomLeft());

  float moduleSize = calculateModuleSize(topLeft, topRight, bottomLeft);
  if (moduleSize < 1.0f) {
    throw ImageException("bad module size");
  }
  int dimension = computeDimension(topLeft, topRight, bottomLeft, moduleSize);
  ImageRef<ImageQRVersion>provisionalVersion = ImageQRVersion::getProvisionalVersionForDimension(dimension);
  int modulesBetweenFPCenters = provisionalVersion->getDimensionForVersion() - 7;

  ImageRef<ImageQRAlignmentPattern> alignmentPattern;
  // Anything above version 1 has an alignment pattern
  if (provisionalVersion->getAlignmentPatternCenters().size() > 0) {


    // Guess where a "bottom right" finder pattern would have been
    float bottomRightX = topRight->getX() - topLeft->getX() + bottomLeft->getX();
    float bottomRightY = topRight->getY() - topLeft->getY() + bottomLeft->getY();


    // Estimate that alignment pattern is closer by 3 modules
    // from "bottom right" to known top left location
    float correctionToTopLeft = 1.0f - 3.0f / (float)modulesBetweenFPCenters;
    int estAlignmentX = (int)(topLeft->getX() + correctionToTopLeft * (bottomRightX - topLeft->getX()));
    int estAlignmentY = (int)(topLeft->getY() + correctionToTopLeft * (bottomRightY - topLeft->getY()));


    // Kind of arbitrary -- expand search radius before giving up
    for (int i = 4; i <= 16; i <<= 1) {
      try {
        alignmentPattern = findAlignmentInRegion(moduleSize, estAlignmentX, estAlignmentY, (float)i);
        break;
      } catch (ImageException const& re) {
        (void)re;
        // try next round
      }
    }
    if (alignmentPattern == 0) {
      // Try anyway
    }

  }

  ImageRef<ImagePerspectiveTransform> transform = createTransform(topLeft, topRight, bottomLeft, alignmentPattern, dimension);
  ImageRef<ImageBitMatrix> bits(sampleGrid(image_, dimension, transform));
  ImageArrayRef< ImageRef<ImageResultPoint> > points(new ImageArray< ImageRef<ImageResultPoint> >(alignmentPattern == 0 ? 3 : 4));
  points[0].reset(bottomLeft);
  points[1].reset(topLeft);
  points[2].reset(topRight);
  if (alignmentPattern != 0) {
    points[3].reset(alignmentPattern);
  }

  ImageRef<ImageDetectorResult> result(new ImageDetectorResult(bits, points));
  return result;
}

ImageRef<ImagePerspectiveTransform> ImageQRDetector::createTransform(ImageRef<ImageResultPoint> topLeft, ImageRef<ImageResultPoint> topRight, ImageRef <
                                                    ImageResultPoint > bottomLeft, ImageRef<ImageResultPoint> alignmentPattern, int dimension) {

  float dimMinusThree = (float)dimension - 3.5f;
  float bottomRightX;
  float bottomRightY;
  float sourceBottomRightX;
  float sourceBottomRightY;
  if (alignmentPattern != 0) {
    bottomRightX = alignmentPattern->getX();
    bottomRightY = alignmentPattern->getY();
    sourceBottomRightX = dimMinusThree - 3.0f;
    sourceBottomRightY = sourceBottomRightX;
  } else {
    // Don't have an alignment pattern, just make up the bottom-right point
    bottomRightX = (topRight->getX() - topLeft->getX()) + bottomLeft->getX();
    bottomRightY = (topRight->getY() - topLeft->getY()) + bottomLeft->getY();
    sourceBottomRightX = dimMinusThree;
    sourceBottomRightY = dimMinusThree;
  }

  ImageRef<ImagePerspectiveTransform> transform(ImagePerspectiveTransform::quadrilateralToQuadrilateral(3.5f, 3.5f, dimMinusThree, 3.5f, sourceBottomRightX,
                                                                                         sourceBottomRightY, 3.5f, dimMinusThree, topLeft->getX(), topLeft->getY(), topRight->getX(),
                                                                                         topRight->getY(), bottomRightX, bottomRightY, bottomLeft->getX(), bottomLeft->getY()));

  return transform;
}

ImageRef<ImageBitMatrix> ImageQRDetector::sampleGrid(ImageRef<ImageBitMatrix> image, int dimension, ImageRef<ImagePerspectiveTransform> transform) {
  ImageGridSampler &sampler = ImageGridSampler::getInstance();
  return sampler.sampleGrid(image, dimension, transform);
}

int ImageQRDetector::computeDimension(ImageRef<ImageResultPoint> topLeft, ImageRef<ImageResultPoint> topRight, ImageRef<ImageResultPoint> bottomLeft,
                               float moduleSize) {
  int tltrCentersDimension =
    MathTools::round(ImageResultPoint::distance(topLeft, topRight) / moduleSize);
  int tlblCentersDimension =
    MathTools::round(ImageResultPoint::distance(topLeft, bottomLeft) / moduleSize);
  int dimension = ((tltrCentersDimension + tlblCentersDimension) >> 1) + 7;
  switch (dimension & 0x03) { // mod 4
  case 0:
    dimension++;
    break;
    // 1? do nothing
  case 2:
    dimension--;
    break;
  case 3:
    std::ostringstream s;
    s << "Bad dimension: " << dimension;
    throw ImageException(s.str().c_str());
  }
  return dimension;
}

float ImageQRDetector::calculateModuleSize(ImageRef<ImageResultPoint> topLeft, ImageRef<ImageResultPoint> topRight, ImageRef<ImageResultPoint> bottomLeft) {
  // Take the average
  return (calculateModuleSizeOneWay(topLeft, topRight) + calculateModuleSizeOneWay(topLeft, bottomLeft)) / 2.0f;
}

float ImageQRDetector::calculateModuleSizeOneWay(ImageRef<ImageResultPoint> pattern, ImageRef<ImageResultPoint> otherPattern) {
  float moduleSizeEst1 = sizeOfBlackWhiteBlackRunBothWays((int)pattern->getX(), (int)pattern->getY(),
                                                          (int)otherPattern->getX(), (int)otherPattern->getY());
  float moduleSizeEst2 = sizeOfBlackWhiteBlackRunBothWays((int)otherPattern->getX(), (int)otherPattern->getY(),
                                                          (int)pattern->getX(), (int)pattern->getY());
  if (isnan_z(moduleSizeEst1)) {
    return moduleSizeEst2;
  }
  if (isnan_z(moduleSizeEst2)) {
    return moduleSizeEst1;
  }
  // Average them, and divide by 7 since we've counted the width of 3 black modules,
  // and 1 white and 1 black module on either side. Ergo, divide sum by 14.
  return (moduleSizeEst1 + moduleSizeEst2) / 14.0f;
}

float ImageQRDetector::sizeOfBlackWhiteBlackRunBothWays(int fromX, int fromY, int toX, int toY) {

  float result = sizeOfBlackWhiteBlackRun(fromX, fromY, toX, toY);

  // Now count other way -- don't run off image though of course
  float scale = 1.0f;
  int otherToX = fromX - (toX - fromX);
  if (otherToX < 0) {
    scale = (float) fromX / (float) (fromX - otherToX);
    otherToX = 0;
  } else if (otherToX >= (int)image_->getWidth()) {
    scale = (float) (image_->getWidth() - 1 - fromX) / (float) (otherToX - fromX);
    otherToX = image_->getWidth() - 1;
  }
  int otherToY = (int) (fromY - (toY - fromY) * scale);

  scale = 1.0f;
  if (otherToY < 0) {
    scale = (float) fromY / (float) (fromY - otherToY);
    otherToY = 0;
  } else if (otherToY >= (int)image_->getHeight()) {
    scale = (float) (image_->getHeight() - 1 - fromY) / (float) (otherToY - fromY);
    otherToY = image_->getHeight() - 1;
  }
  otherToX = (int) (fromX + (otherToX - fromX) * scale);

  result += sizeOfBlackWhiteBlackRun(fromX, fromY, otherToX, otherToY);

  // Middle pixel is double-counted this way; subtract 1
  return result - 1.0f;
}

float ImageQRDetector::sizeOfBlackWhiteBlackRun(int fromX, int fromY, int toX, int toY) {
  // Mild variant of Bresenham's algorithm;
  // see http://en.wikipedia.org/wiki/Bresenham's_line_algorithm
  bool steep = ::abs(toY - fromY) > ::abs(toX - fromX);
  if (steep) {
    int temp = fromX;
    fromX = fromY;
    fromY = temp;
    temp = toX;
    toX = toY;
    toY = temp;
  }

  int dx = ::abs(toX - fromX);
  int dy = ::abs(toY - fromY);
  int error = -dx >> 1;
  int xstep = fromX < toX ? 1 : -1;
  int ystep = fromY < toY ? 1 : -1;

  // In black pixels, looking for white, first or second time.
  int state = 0;
  // Loop up until x == toX, but not beyond
  int xLimit = toX + xstep;
  for (int x = fromX, y = fromY; x != xLimit; x += xstep) {
    int realX = steep ? y : x;
    int realY = steep ? x : y;

    // Does current pixel mean we have moved white to black or vice versa?
    if (!((state == 1) ^ image_->get(realX, realY))) {
      if (state == 2) {
        return MathTools::distance(x, y, fromX, fromY);
      }
      state++;
    }

    error += dy;
    if (error > 0) {
      if (y == toY) {
        break;
      }
      y += ystep;
      error -= dx;
    }
  }
  // Found black-white-black; give the benefit of the doubt that the next pixel outside the image
  // is "white" so this last point at (toX+xStep,toY) is the right ending. This is really a
  // small approximation; (toX+xStep,toY+yStep) might be really correct. Ignore this.
  if (state == 2) {
    return MathTools::distance(toX + xstep, toY, fromX, fromY);
  }
  // else we didn't find even black-white-black; no estimate is really possible
  return nan();
}

ImageRef<ImageQRAlignmentPattern> ImageQRDetector::findAlignmentInRegion(float overallEstModuleSize, int estAlignmentX, int estAlignmentY,
                                                      float allowanceFactor) {
  // Look for an alignment pattern (3 modules in size) around where it
  // should be
  int allowance = (int)(allowanceFactor * overallEstModuleSize);
  int alignmentAreaLeftX = std::max(0, estAlignmentX - allowance);
  int alignmentAreaRightX = std::min((int)(image_->getWidth() - 1), estAlignmentX + allowance);
  if (alignmentAreaRightX - alignmentAreaLeftX < overallEstModuleSize * 3) {
    throw ImageException("region too small to hold alignment pattern");
  }
  int alignmentAreaTopY = std::max(0, estAlignmentY - allowance);
  int alignmentAreaBottomY = std::min((int)(image_->getHeight() - 1), estAlignmentY + allowance);
  if (alignmentAreaBottomY - alignmentAreaTopY < overallEstModuleSize * 3) {
    throw ImageException("region too small to hold alignment pattern");
  }

  ImageQRAlignmentPatternFinder alignmentFinder(image_, alignmentAreaLeftX, alignmentAreaTopY, alignmentAreaRightX
                                         - alignmentAreaLeftX, alignmentAreaBottomY - alignmentAreaTopY, overallEstModuleSize, callback_);
  return alignmentFinder.find();
}
