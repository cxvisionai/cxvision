#include "imageresultpoint.h"
#include <common/detector/mathtools.h>

ImageResultPoint::ImageResultPoint() : posX_(0), posY_(0) {}

ImageResultPoint::ImageResultPoint(float x, float y) : posX_(x), posY_(y) {}

ImageResultPoint::ImageResultPoint(int x, int y) : posX_(float(x)), posY_(float(y)) {}
  
ImageResultPoint::~ImageResultPoint() {}

float ImageResultPoint::getX() const {
  return posX_;
}
    
float ImageResultPoint::getY() const {
  return posY_;
}

bool ImageResultPoint::equals(const ImageRef<ImageResultPoint> &other) {
  return posX_ == other->getX() && posY_ == other->getY();
}


void ImageResultPoint::orderBestPatterns(std::vector<ImageRef<ImageResultPoint> > &patterns) {
    if(patterns.size() != 3)
        return;

    // Find distances between pattern centers
    float zeroOneDistance = distance(patterns[0]->getX(), patterns[1]->getX(),patterns[0]->getY(), patterns[1]->getY());
    float oneTwoDistance = distance(patterns[1]->getX(), patterns[2]->getX(),patterns[1]->getY(), patterns[2]->getY());
    float zeroTwoDistance = distance(patterns[0]->getX(), patterns[2]->getX(),patterns[0]->getY(), patterns[2]->getY());

    ImageRef<ImageResultPoint> pointA, pointB, pointC;
    // Assume one closest to other two is B; A and C will just be guesses at first
    if (oneTwoDistance >= zeroOneDistance && oneTwoDistance >= zeroTwoDistance) {
      pointB = patterns[0];
      pointA = patterns[1];
      pointC = patterns[2];
    } else if (zeroTwoDistance >= oneTwoDistance && zeroTwoDistance >= zeroOneDistance) {
      pointB = patterns[1];
      pointA = patterns[0];
      pointC = patterns[2];
    } else {
      pointB = patterns[2];
      pointA = patterns[0];
      pointC = patterns[1];
    }

    // Use cross product to figure out whether A and C are correct or flipped.
    // This asks whether BC x BA has a positive z component, which is the arrangement
    // we want for A, B, C. If it's negative, then we've got it flipped around and
    // should swap A and C.
    if (crossProductZ(pointA, pointB, pointC) < 0.0f) {
      ImageRef<ImageResultPoint> temp = pointA;
      pointA = pointC;
      pointC = temp;
    }

    patterns[0] = pointA;
    patterns[1] = pointB;
    patterns[2] = pointC;
}

  float ImageResultPoint::distance(ImageRef<ImageResultPoint> pattern1, ImageRef<ImageResultPoint> pattern2) {
  return MathTools::distance(pattern1->posX_,
                             pattern1->posY_,
                             pattern2->posX_,
                             pattern2->posY_);
}

float ImageResultPoint::distance(float x1, float x2, float y1, float y2) {
  float xDiff = x1 - x2;
  float yDiff = y1 - y2;
  return (float) sqrt((double) (xDiff * xDiff + yDiff * yDiff));
}

float ImageResultPoint::crossProductZ(ImageRef<ImageResultPoint> pointA, ImageRef<ImageResultPoint> pointB, ImageRef<ImageResultPoint> pointC) {
  float bX = pointB->getX();
  float bY = pointB->getY();
  return ((pointC->getX() - bX) * (pointA->getY() - bY)) - ((pointC->getY() - bY) * (pointA->getX() - bX));
}

