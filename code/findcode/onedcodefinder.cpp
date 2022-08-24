
#include <onedcodefinder.h>
#include <nonexception.h>
#include <onedcoderesultpoint.h>
#include <nonexception.h>

#include <math.h>
#include <limits.h>
#include <algorithm>


ImageRef<ImageResult> OneDCodeFinder::decode(ImageRef<ImageBitmap> image, ImageCodeTypes hints) {
  try {
    return doDecode(image, hints);
  } catch (NoneException const& nfe) {
    // std::cerr << "trying harder" << std::endl;
    bool tryHarder = hints.getTryHarder();
    if (tryHarder && image->isRotateSupported()) {
      // std::cerr << "v rotate" << std::endl;
      ImageRef<ImageBitmap> rotatedImage(image->rotateCounterClockwise());
      // std::cerr << "^ rotate" << std::endl;
      ImageRef<ImageResult> result = doDecode(rotatedImage, hints);
      // Doesn't have java metadata stuff
      ImageArrayRef< ImageRef<ImageResultPoint> >& points (result->getImageResultPoints());
      if (points && !points->empty()) {
        int height = rotatedImage->getHeight();
        for (int i = 0; i < points->size(); i++) {
          points[i].reset(new OneDCodeResultPoint(height - points[i]->getY() - 1, points[i]->getX()));
        }
      }
      // std::cerr << "tried harder" << std::endl;
      return result;
    } else {
      // std::cerr << "tried harder nfe" << std::endl;
      throw nfe;
    }
  }
}

#include <typeinfo>

ImageRef<ImageResult> OneDCodeFinder::doDecode(ImageRef<ImageBitmap> image, ImageCodeTypes hints) {
  int width = image->getWidth();
  int height = image->getHeight();
  ImageRef<ImageBitArray> row(new ImageBitArray(width));

  int middle = height >> 1;
  bool tryHarder = hints.getTryHarder();
  int rowStep = std::max(1, height >> (tryHarder ? 8 : 5));
  using namespace std;
  // cerr << "rS " << rowStep << " " << height << " " << tryHarder << endl;
  int maxLines;
  if (tryHarder) {
    maxLines = height; // Look at the whole image, not just the center
  } else {
    maxLines = 15; // 15 rows spaced 1/32 apart is roughly the middle half of the image
  }

  for (int x = 0; x < maxLines; x++) {

    // Scanning from the middle out. Determine which row we're looking at next:
    int rowStepsAboveOrBelow = (x + 1) >> 1;
    bool isAbove = (x & 0x01) == 0; // i.e. is x even?
    int rowNumber = middle + rowStep * (isAbove ? rowStepsAboveOrBelow : -rowStepsAboveOrBelow);
    if (false) {
      std::cerr << "rN "
                << rowNumber << " "
                << height << " "
                << middle << " "
                << rowStep << " "
                << isAbove << " "
                << rowStepsAboveOrBelow
                << std::endl;
    }
    if (rowNumber < 0 || rowNumber >= height) {
      // Oops, if we run off the top or bottom, stop
      break;
    }

    // Estimate black point for this row and load it:
    try {
      row = image->getBlackRow(rowNumber, row);
    } catch (NoneException const& ignored) {
      (void)ignored;
      continue;
    }

    // While we have the image data in a BitArray, it's fairly cheap to reverse it in place to
    // handle decoding upside down barcodes.
    for (int attempt = 0; attempt < 2; attempt++) {
      if (attempt == 1) {
        row->reverse(); // reverse the row and continue
      }

      try {
        // Look for a barcode
        // std::cerr << "rn " << rowNumber << " " << typeid(*this).name() << std::endl;
        ImageRef<ImageResult> result = decodeRow(rowNumber, row, hints);
        // We found our barcode
        if (attempt == 1) {
          // But it was upside down, so note that
          // result.putMetadata(ResultMetadataType.ORIENTATION, new Integer(180));
          // And remember to flip the result points horizontally.
          ImageArrayRef< ImageRef<ImageResultPoint> > points(result->getImageResultPoints());
          if (points) {
            points[0] = ImageRef<ImageResultPoint>(new OneDCodeResultPoint(width - points[0]->getX() - 1,
                                                             points[0]->getY()));
            points[1] = ImageRef<ImageResultPoint>(new OneDCodeResultPoint(width - points[1]->getX() - 1,
                                                             points[1]->getY()));
            
          }
        }
        return result;
      } catch (CXException const& re) {
        (void)re;
        continue;
      }
    }
  }
  throw NoneException();
}

int OneDCodeFinder::patternMatchVariance(std::vector<int>& counters,
                                     std::vector<int> const& pattern,
                                     int maxIndividualVariance) {
  return patternMatchVariance(counters, &pattern[0], maxIndividualVariance);
}

int OneDCodeFinder::patternMatchVariance(std::vector<int>& counters,
                                     int const pattern[],
                                     int maxIndividualVariance) {
  int numCounters = int(counters.size());
  unsigned int total = 0;
  unsigned int patternLength = 0;
  for (int i = 0; i < numCounters; i++) {
    total += counters[i];
    patternLength += pattern[i];
  }
  if (total < patternLength) {
    // If we don't even have one pixel per unit of bar width, assume this is too small
    // to reliably match, so fail:
    return INT_MAX;
  }
  // We're going to fake floating-point math in integers. We just need to use more bits.
  // Scale up patternLength so that intermediate values below like scaledCounter will have
  // more "significant digits"
  int unitBarWidth = (total << INTEGER_MATH_SHIFT) / patternLength;
  maxIndividualVariance = (maxIndividualVariance * unitBarWidth) >> INTEGER_MATH_SHIFT;

  int totalVariance = 0;
  for (int x = 0; x < numCounters; x++) {
    int counter = counters[x] << INTEGER_MATH_SHIFT;
    int scaledPattern = pattern[x] * unitBarWidth;
    int variance = counter > scaledPattern ? counter - scaledPattern : scaledPattern - counter;
    if (variance > maxIndividualVariance) {
      return INT_MAX;
    }
    totalVariance += variance;
  }
  return totalVariance / total;
}

void OneDCodeFinder::recordPattern(ImageRef<ImageBitArray> row,
                               int start,
                               std::vector<int>& counters) {
  int numCounters = int(counters.size());
  for (int i = 0; i < numCounters; i++) {
    counters[i] = 0;
  }
  int end = row->getSize();
  if (start >= end) {
    throw NoneException();
  }
  bool isWhite = !row->get(start);
  int counterPosition = 0;
  int i = start;
  while (i < end) {
    if (row->get(i) ^ isWhite) { // that is, exactly one is true
      counters[counterPosition]++;
    } else {
      counterPosition++;
      if (counterPosition == numCounters) {
        break;
      } else {
        counters[counterPosition] = 1;
        isWhite = !isWhite;
      }
    }
    i++;
  }
  // If we read fully the last section of pixels and filled up our counters -- or filled
  // the last counter but ran off the side of the image, OK. Otherwise, a problem.
  if (!(counterPosition == numCounters || (counterPosition == numCounters - 1 && i == end))) {
    throw NoneException();
  }
}

void OneDCodeFinder::recordPatternInReverse(ImageRef<ImageBitArray> row,
                                        int start,
                                        std::vector<int>& counters)
{
    // This could be more efficient I guess
    int numTransitionsLeft = int(counters.size());
    bool last = row->get(start);
    while (start > 0 && numTransitionsLeft >= 0) {
        if (row->get(--start) != last) {
            numTransitionsLeft--;
            last = !last;
        }
    }
    if (numTransitionsLeft >= 0)
    {
        throw NoneException();
    }
    recordPattern(row, start + 1, counters);
}

OneDCodeFinder::~OneDCodeFinder() {}
