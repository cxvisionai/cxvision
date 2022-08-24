
#include <qrcode/detector/imageqralignmentpatternfinder.h>
#include <imagexception.h>
#include <common/imagebitarray.h>
#include <mathtools.h>

#include <vector>
#include <cmath>
#include <cstdlib>

// VC++

float ImageQRAlignmentPatternFinder::centerFromEnd(std::vector<int>& stateCount, int end) {
  return (float)(end - stateCount[2]) - stateCount[1] / 2.0f;
}

bool ImageQRAlignmentPatternFinder::foundPatternCross(std::vector<int> &stateCount) {
  float maxVariance = moduleSize_ / 2.0f;
  for (int i = 0; i < 3; i++) {
    if (abs(moduleSize_ - stateCount[i]) >= maxVariance) {
      return false;
    }
  }
  return true;
}

float ImageQRAlignmentPatternFinder::crossCheckVertical(int startI, int centerJ, int maxCount,
                                                 int originalStateCountTotal) {
  int maxI = image_->getHeight();
  std::vector<int> stateCount(3, 0);


  // Start counting up from center
  int i = startI;
  while (i >= 0 && image_->get(centerJ, i) && stateCount[1] <= maxCount) {
    stateCount[1]++;
    i--;
  }
  // If already too many modules in this state or ran off the edge:
  if (i < 0 || stateCount[1] > maxCount) {
    return nan();
  }
  while (i >= 0 && !image_->get(centerJ, i) && stateCount[0] <= maxCount) {
    stateCount[0]++;
    i--;
  }
  if (stateCount[0] > maxCount) {
    return nan();
  }

  // Now also count down from center
  i = startI + 1;
  while (i < maxI && image_->get(centerJ, i) && stateCount[1] <= maxCount) {
    stateCount[1]++;
    i++;
  }
  if (i == maxI || stateCount[1] > maxCount) {
    return nan();
  }
  while (i < maxI && !image_->get(centerJ, i) && stateCount[2] <= maxCount) {
    stateCount[2]++;
    i++;
  }
  if (stateCount[2] > maxCount) {
    return nan();
  }

  int stateCountTotal = stateCount[0] + stateCount[1] + stateCount[2];
  if (5 * ::abs(stateCountTotal - originalStateCountTotal) >= 2 * originalStateCountTotal) {
    return nan();
  }

  return foundPatternCross(stateCount) ? centerFromEnd(stateCount, i) : nan();
}

ImageRef<ImageQRAlignmentPattern> ImageQRAlignmentPatternFinder::handlePossibleCenter(std::vector<int> &stateCount, int i, int j) {
  int stateCountTotal = stateCount[0] + stateCount[1] + stateCount[2];
  float centerJ = centerFromEnd(stateCount, j);
  float centerI = crossCheckVertical(i, (int)centerJ, 2 * stateCount[1], stateCountTotal);
  if (!isnan_z(centerI)) {
    float estimatedModuleSize = (float)(stateCount[0] + stateCount[1] + stateCount[2]) / 3.0f;
    int max = int(possibleCenters_->size());
    for (int index = 0; index < max; index++) {
      ImageRef<ImageQRAlignmentPattern> center((*possibleCenters_)[index]);
      // Look for about the same center and module size:
      if (center->aboutEquals(estimatedModuleSize, centerI, centerJ)) {
        return center->combineEstimate(centerI, centerJ, estimatedModuleSize);
      }
    }
    ImageQRAlignmentPattern *tmp = new ImageQRAlignmentPattern(centerJ, centerI, estimatedModuleSize);
    // Hadn't found this before; save it
    tmp->retain();
    possibleCenters_->push_back(tmp);
    if (callback_ != 0) {
      callback_->foundPossibleResultPoint(*tmp);
    }
  }
  ImageRef<ImageQRAlignmentPattern> result;
  return result;
}

ImageQRAlignmentPatternFinder::ImageQRAlignmentPatternFinder(ImageRef<ImageBitMatrix> image, int startX, int startY, int width,
                                               int height, float moduleSize, 
                                               ImageRef<ImageResultPointCallback>const& callback) :
    image_(image), possibleCenters_(new std::vector<ImageQRAlignmentPattern *> ()), startX_(startX), startY_(startY),
    width_(width), height_(height), moduleSize_(moduleSize), callback_(callback) {
}

ImageQRAlignmentPatternFinder::~ImageQRAlignmentPatternFinder() {
  for (int i = 0; i < int(possibleCenters_->size()); i++) {
    (*possibleCenters_)[i]->release();
    (*possibleCenters_)[i] = nullptr;
  }
  delete possibleCenters_;
}

ImageRef<ImageQRAlignmentPattern> ImageQRAlignmentPatternFinder::find() {
  int maxJ = startX_ + width_;
  int middleI = startY_ + (height_ >> 1);
  //      Ref<BitArray> luminanceRow(new BitArray(width_));
  // We are looking for black/white/black modules in 1:1:1 ratio;
  // this tracks the number of black/white/black modules seen so far
  std::vector<int> stateCount(3, 0);
  for (int iGen = 0; iGen < height_; iGen++) {
    // Search from middle outwards
    int i = middleI + ((iGen & 0x01) == 0 ? ((iGen + 1) >> 1) : -((iGen + 1) >> 1));
    //        image_->getBlackRow(i, luminanceRow, startX_, width_);
    stateCount[0] = 0;
    stateCount[1] = 0;
    stateCount[2] = 0;
    int j = startX_;
    // Burn off leading white pixels before anything else; if we start in the middle of
    // a white run, it doesn't make sense to count its length, since we don't know if the
    // white run continued to the left of the start point
    while (j < maxJ && !image_->get(j, i)) {
      j++;
    }
    int currentState = 0;
    while (j < maxJ) {
      if (image_->get(j, i)) {
        // Black pixel
        if (currentState == 1) { // Counting black pixels
          stateCount[currentState]++;
        } else { // Counting white pixels
          if (currentState == 2) { // A winner?
            if (foundPatternCross(stateCount)) { // Yes
              ImageRef<ImageQRAlignmentPattern> confirmed(handlePossibleCenter(stateCount, i, j));
              if (confirmed != nullptr) {
                return confirmed;
              }
            }
            stateCount[0] = stateCount[2];
            stateCount[1] = 1;
            stateCount[2] = 0;
            currentState = 1;
          } else {
            stateCount[++currentState]++;
          }
        }
      } else { // White pixel
        if (currentState == 1) { // Counting black pixels
          currentState++;
        }
        stateCount[currentState]++;
      }
      j++;
    }
    if (foundPatternCross(stateCount)) {
      ImageRef<ImageQRAlignmentPattern> confirmed(handlePossibleCenter(stateCount, i, maxJ));
      if (confirmed != 0) {
        return confirmed;
      }
    }

  }

  // Hmm, nothing we saw was observed and confirmed twice. If we had
  // any guess at all, return it.
  if (possibleCenters_->size() > 0) {
    ImageRef<ImageQRAlignmentPattern> center((*possibleCenters_)[0]);
    return center;
  }

  throw ImageException("Could not find alignment pattern");
}
