
#include <onedcode128finder.h>
#include <onedcodefinder.h>
#include <imagearray.h>
#include <imagefinderexception.h>
#include <nonexception.h>
#include <imagecodetypes.h>
#include <imagecodeformat.h>
#include <onedcoderesultpoint.h>
#include <math.h>
#include <string.h>
#include <sstream>
#include <algorithm>
#include <vector>



const int OneDCode128Finder::MAX_AVG_VARIANCE = int(PATTERN_MATCH_RESULT_SCALE_FACTOR * 250/1000);
const int OneDCode128Finder::MAX_INDIVIDUAL_VARIANCE = int(PATTERN_MATCH_RESULT_SCALE_FACTOR * 700/1000);

const int CODE_SHIFT = 98;
	
const int CODE_CODE_C = 99;
const int CODE_CODE_B = 100;
const int CODE_CODE_A = 101;
	
const int CODE_FNC_1 = 102;
const int CODE_FNC_2 = 97;
const int CODE_FNC_3 = 96;
const int CODE_FNC_4_A = 101;
const int CODE_FNC_4_B = 100;
	
const int CODE_START_A = 103;
const int CODE_START_B = 104;
const int CODE_START_C = 105;
const int CODE_STOP = 106;
			
const int CODE_PATTERNS_LENGTH = 107;
const int CODE_PATTERNS[CODE_PATTERNS_LENGTH][6] = {
  {2, 1, 2, 2, 2, 2}, /* 0 */
  {2, 2, 2, 1, 2, 2},
  {2, 2, 2, 2, 2, 1},
  {1, 2, 1, 2, 2, 3},
  {1, 2, 1, 3, 2, 2},
  {1, 3, 1, 2, 2, 2}, /* 5 */
  {1, 2, 2, 2, 1, 3},
  {1, 2, 2, 3, 1, 2},
  {1, 3, 2, 2, 1, 2},
  {2, 2, 1, 2, 1, 3},
  {2, 2, 1, 3, 1, 2}, /* 10 */
  {2, 3, 1, 2, 1, 2},
  {1, 1, 2, 2, 3, 2},
  {1, 2, 2, 1, 3, 2},
  {1, 2, 2, 2, 3, 1},
  {1, 1, 3, 2, 2, 2}, /* 15 */
  {1, 2, 3, 1, 2, 2},
  {1, 2, 3, 2, 2, 1},
  {2, 2, 3, 2, 1, 1},
  {2, 2, 1, 1, 3, 2},
  {2, 2, 1, 2, 3, 1}, /* 20 */
  {2, 1, 3, 2, 1, 2},
  {2, 2, 3, 1, 1, 2},
  {3, 1, 2, 1, 3, 1},
  {3, 1, 1, 2, 2, 2},
  {3, 2, 1, 1, 2, 2}, /* 25 */
  {3, 2, 1, 2, 2, 1},
  {3, 1, 2, 2, 1, 2},
  {3, 2, 2, 1, 1, 2},
  {3, 2, 2, 2, 1, 1},
  {2, 1, 2, 1, 2, 3}, /* 30 */
  {2, 1, 2, 3, 2, 1},
  {2, 3, 2, 1, 2, 1},
  {1, 1, 1, 3, 2, 3},
  {1, 3, 1, 1, 2, 3},
  {1, 3, 1, 3, 2, 1}, /* 35 */
  {1, 1, 2, 3, 1, 3},
  {1, 3, 2, 1, 1, 3},
  {1, 3, 2, 3, 1, 1},
  {2, 1, 1, 3, 1, 3},
  {2, 3, 1, 1, 1, 3}, /* 40 */
  {2, 3, 1, 3, 1, 1},
  {1, 1, 2, 1, 3, 3},
  {1, 1, 2, 3, 3, 1},
  {1, 3, 2, 1, 3, 1},
  {1, 1, 3, 1, 2, 3}, /* 45 */
  {1, 1, 3, 3, 2, 1},
  {1, 3, 3, 1, 2, 1},
  {3, 1, 3, 1, 2, 1},
  {2, 1, 1, 3, 3, 1},
  {2, 3, 1, 1, 3, 1}, /* 50 */
  {2, 1, 3, 1, 1, 3},
  {2, 1, 3, 3, 1, 1},
  {2, 1, 3, 1, 3, 1},
  {3, 1, 1, 1, 2, 3},
  {3, 1, 1, 3, 2, 1}, /* 55 */
  {3, 3, 1, 1, 2, 1},
  {3, 1, 2, 1, 1, 3},
  {3, 1, 2, 3, 1, 1},
  {3, 3, 2, 1, 1, 1},
  {3, 1, 4, 1, 1, 1}, /* 60 */
  {2, 2, 1, 4, 1, 1},
  {4, 3, 1, 1, 1, 1},
  {1, 1, 1, 2, 2, 4},
  {1, 1, 1, 4, 2, 2},
  {1, 2, 1, 1, 2, 4}, /* 65 */
  {1, 2, 1, 4, 2, 1},
  {1, 4, 1, 1, 2, 2},
  {1, 4, 1, 2, 2, 1},
  {1, 1, 2, 2, 1, 4},
  {1, 1, 2, 4, 1, 2}, /* 70 */
  {1, 2, 2, 1, 1, 4},
  {1, 2, 2, 4, 1, 1},
  {1, 4, 2, 1, 1, 2},
  {1, 4, 2, 2, 1, 1},
  {2, 4, 1, 2, 1, 1}, /* 75 */
  {2, 2, 1, 1, 1, 4},
  {4, 1, 3, 1, 1, 1},
  {2, 4, 1, 1, 1, 2},
  {1, 3, 4, 1, 1, 1},
  {1, 1, 1, 2, 4, 2}, /* 80 */
  {1, 2, 1, 1, 4, 2},
  {1, 2, 1, 2, 4, 1},
  {1, 1, 4, 2, 1, 2},
  {1, 2, 4, 1, 1, 2},
  {1, 2, 4, 2, 1, 1}, /* 85 */
  {4, 1, 1, 2, 1, 2},
  {4, 2, 1, 1, 1, 2},
  {4, 2, 1, 2, 1, 1},
  {2, 1, 2, 1, 4, 1},
  {2, 1, 4, 1, 2, 1}, /* 90 */
  {4, 1, 2, 1, 2, 1},
  {1, 1, 1, 1, 4, 3},
  {1, 1, 1, 3, 4, 1},
  {1, 3, 1, 1, 4, 1},
  {1, 1, 4, 1, 1, 3}, /* 95 */
  {1, 1, 4, 3, 1, 1},
  {4, 1, 1, 1, 1, 3},
  {4, 1, 1, 3, 1, 1},
  {1, 1, 3, 1, 4, 1},
  {1, 1, 4, 1, 3, 1}, /* 100 */
  {3, 1, 1, 1, 4, 1},
  {4, 1, 1, 1, 3, 1},
  {2, 1, 1, 4, 1, 2},
  {2, 1, 1, 2, 1, 4},
  {2, 1, 1, 2, 3, 2}, /* 105 */
  {2, 3, 3, 1, 1, 1}
};


OneDCode128Finder::OneDCode128Finder(){}

std::vector<int> OneDCode128Finder::findStartPattern(ImageRef<ImageBitArray> row){
  int width = row->getSize();
  int rowOffset = row->getNextSet(0);

  int counterPosition = 0;
  std::vector<int> counters (6, 0);
  int patternStart = rowOffset;
  bool isWhite = false;
  int patternLength = int(counters.size());

  for (int i = rowOffset; i < width; i++) {
    if (row->get(i) ^ isWhite) {
      counters[counterPosition]++;
    } else {
      if (counterPosition == patternLength - 1) {
        int bestVariance = MAX_AVG_VARIANCE;
        int bestMatch = -1;
        for (int startCode = CODE_START_A; startCode <= CODE_START_C; startCode++) {
          int variance = patternMatchVariance(counters, CODE_PATTERNS[startCode], MAX_INDIVIDUAL_VARIANCE);
          if (variance < bestVariance) {
            bestVariance = variance;
            bestMatch = startCode;
          }
        }
        // Look for whitespace before start pattern, >= 50% of width of start pattern
        if (bestMatch >= 0 &&
            row->isRange(std::max(0, patternStart - (i - patternStart) / 2), patternStart, false)) {
          std::vector<int> resultValue (3, 0);
          resultValue[0] = patternStart;
          resultValue[1] = i;
          resultValue[2] = bestMatch;
          return resultValue;
        }
        patternStart += counters[0] + counters[1];
        for (int y = 2; y < patternLength; y++) {
          counters[y - 2] = counters[y];
        }
        counters[patternLength - 2] = 0;
        counters[patternLength - 1] = 0;
        counterPosition--;
      } else {
        counterPosition++;
      }
      counters[counterPosition] = 1;
      isWhite = !isWhite;
    }
  }
  throw NoneException();
}

int OneDCode128Finder::decodeCode(ImageRef<ImageBitArray> row, std::vector<int>& counters, int rowOffset) {
  recordPattern(row, rowOffset, counters);
  int bestVariance = MAX_AVG_VARIANCE; // worst variance we'll accept
  int bestMatch = -1;
  for (int d = 0; d < CODE_PATTERNS_LENGTH; d++) {
    int const* const pattern = CODE_PATTERNS[d];
    int variance = patternMatchVariance(counters, pattern, MAX_INDIVIDUAL_VARIANCE);
    if (variance < bestVariance) {
      bestVariance = variance;
      bestMatch = d;
    }
  }
  // TODO We're overlooking the fact that the STOP pattern has 7 values, not 6.
  if (bestMatch >= 0) {
    return bestMatch;
  } else {
    throw NoneException();
  }
}

ImageRef<ImageResult> OneDCode128Finder::decodeRow(int rowNumber, ImageRef<ImageBitArray> row, ImageCodeTypes hints)
{
  bool convertFNC1 = hints.containsFormat(ImagecodeFormat(ImagecodeFormat::Value::ASSUME_GS1));

  std::vector<int> startPatternInfo (findStartPattern(row));
  int startCode = startPatternInfo[2];
  int codeSet;
  switch (startCode) {
    case CODE_START_A:
      codeSet = CODE_CODE_A;
      break;
    case CODE_START_B:
      codeSet = CODE_CODE_B;
      break;
    case CODE_START_C:
      codeSet = CODE_CODE_C;
      break;
    default:
      throw CXException();
  }

  bool done = false;
  bool isNextShifted = false;

  std::string result;
  std::vector<cx_byte> rawCodes(20, 0);

  int lastStart = startPatternInfo[0];
  int nextStart = startPatternInfo[1];
  std::vector<int> counters (6, 0);

  int lastCode = 0;
  int code = 0;
  int checksumTotal = startCode;
  int multiplier = 0;
  bool lastCharacterWasPrintable = true;
  bool upperMode = false;
  bool shiftUpperMode = false;

  std::ostringstream oss;

  while (!done) {

    bool unshift = isNextShifted;
    isNextShifted = false;

    // Save off last code
    lastCode = code;

    code = decodeCode(row, counters, nextStart);

    // Remember whether the last code was printable or not (excluding CODE_STOP)
    if (code != CODE_STOP) {
      lastCharacterWasPrintable = true;
    }

    // Add to checksum computation (if not CODE_STOP of course)
    if (code != CODE_STOP) {
      multiplier++;
      checksumTotal += multiplier * code;
    }

    // Advance to where the next code will to start
    lastStart = nextStart;
    for (int i = 0, e = int(counters.size()); i < e; i++) {
      nextStart += counters[i];
    }

    // Take care of illegal start codes
    switch (code) {
      case CODE_START_A:
      case CODE_START_B:
      case CODE_START_C:
        throw CXException();
    }

    switch (codeSet) {

      case CODE_CODE_A:
        if (code < 64) {
            if (shiftUpperMode == upperMode) {
              result.append(1,(cx_byte) (' ' + code));
            } else {
              result.append(1,(cx_byte) (' ' + code + 128));
            }
            shiftUpperMode = false;
        } else if (code < 96) {
            if (shiftUpperMode == upperMode) {
              result.append(1, (cx_byte) (code - 64));
            } else {
              result.append(1, (cx_byte) (code + 64));
            }
            shiftUpperMode = false;
        } else {
          // Don't let CODE_STOP, which always appears, affect whether whether we think the
          // last code was printable or not.
          if (code != CODE_STOP) {
            lastCharacterWasPrintable = false;
          }
          switch (code) {
            case CODE_FNC_1:
              if (convertFNC1) {
                if (result.length() == 0){
                  // GS1 specification 5.4.3.7. and 5.4.6.4. If the first char after the start code
                  // is FNC1 then this is GS1-128. We add the symbology identifier.
                  result.append("]C1");
                } else {
                  // GS1 specification 5.4.7.5. Every subsequent FNC1 is returned as ASCII 29 (GS)
                  result.append(1, (cx_byte) 29);
                }
              }
              break;
            case CODE_FNC_2:
            case CODE_FNC_3:
              break;
            case CODE_FNC_4_A:
              if (!upperMode && shiftUpperMode) {
                upperMode = true;
                shiftUpperMode = false;
              } else if (upperMode && shiftUpperMode) {
                upperMode = false;
                shiftUpperMode = false;
              } else {
                shiftUpperMode = true;
              }
              break;
            case CODE_SHIFT:
              isNextShifted = true;
              codeSet = CODE_CODE_B;
              break;
            case CODE_CODE_B:
              codeSet = CODE_CODE_B;
              break;
            case CODE_CODE_C:
              codeSet = CODE_CODE_C;
              break;
            case CODE_STOP:
              done = true;
              break;
          }
        }
        break;
      case CODE_CODE_B:
        if (code < 96) {
            if (shiftUpperMode == upperMode) {
              result.append(1, (cx_byte) (' ' + code));
            } else {
              result.append(1, (cx_byte) (' ' + code + 128));
            }
            shiftUpperMode = false;
        } else {
          if (code != CODE_STOP) {
            lastCharacterWasPrintable = false;
          }
          switch (code) {
          case CODE_FNC_1:
              if (convertFNC1) {
                if (result.length() == 0) {
                  // GS1 specification 5.4.3.7. and 5.4.6.4. If the first char after the start code
                  // is FNC1 then this is GS1-128. We add the symbology identifier.
                  result.append("]C1");
                } else {
                  // GS1 specification 5.4.7.5. Every subsequent FNC1 is returned as ASCII 29 (GS)
                  result.append(1, (cx_byte) 29);
                }
              }
              break;
            case CODE_FNC_2:
            case CODE_FNC_3:
              break;
            case CODE_FNC_4_B:
              if (!upperMode && shiftUpperMode) {
                upperMode = true;
                shiftUpperMode = false;
              } else if (upperMode && shiftUpperMode) {
                upperMode = false;
                shiftUpperMode = false;
              } else {
                shiftUpperMode = true;
              }
              break;
            case CODE_SHIFT:
              isNextShifted = true;
              codeSet = CODE_CODE_A;
              break;
            case CODE_CODE_A:
              codeSet = CODE_CODE_A;
              break;
            case CODE_CODE_C:
              codeSet = CODE_CODE_C;
              break;
            case CODE_STOP:
              done = true;
              break;
          }
        }
        break;
      case CODE_CODE_C:
        if (code < 100) {
          if (code < 10) {
            result.append(1, '0');
          }
          oss.clear();
          oss.str("");
          oss << code;
          result.append(oss.str());
        } else {
          if (code != CODE_STOP) {
            lastCharacterWasPrintable = false;
          }
          switch (code) {
            case CODE_FNC_1:
              if (convertFNC1) {
                if (result.length() == 0) {
                  // GS1 specification 5.4.3.7. and 5.4.6.4. If the first char after the start code
                  // is FNC1 then this is GS1-128. We add the symbology identifier.
                  result.append("]C1");
                } else {
                  // GS1 specification 5.4.7.5. Every subsequent FNC1 is returned as ASCII 29 (GS)
                  result.append(1, (cx_byte) 29);
                }
              }
              break;
            case CODE_CODE_A:
              codeSet = CODE_CODE_A;
              break;
            case CODE_CODE_B:
              codeSet = CODE_CODE_B;
              break;
            case CODE_STOP:
              done = true;
              break;
          }
        }
        break;
    }

    // Unshift back to another code set if we were shifted
    if (unshift) {
      codeSet = codeSet == CODE_CODE_A ? CODE_CODE_B : CODE_CODE_A;
    }
    
  }

  int lastPatternSize = nextStart - lastStart;

  // Check for ample whitespace following pattern, but, to do this we first need to remember that
  // we fudged decoding CODE_STOP since it actually has 7 bars, not 6. There is a black bar left
  // to read off. Would be slightly better to properly read. Here we just skip it:
  nextStart = row->getNextUnset(nextStart);
  if (!row->isRange(nextStart,
                    std::min(row->getSize(), nextStart + (nextStart - lastStart) / 2),
                    false)) {
    throw NoneException();
  }

  // Pull out from sum the value of the penultimate check code
  checksumTotal -= multiplier * lastCode;
  // lastCode is the checksum then:
  if (checksumTotal % 103 != lastCode) {
    throw CXException();
  }

  // Need to pull out the check digits from string
  int resultLength = result.length();
  if (resultLength == 0) {
    // false positive
    throw NoneException();
  }

  // Only bother if the result had at least one character, and if the checksum digit happened to
  // be a printable character. If it was just interpreted as a control code, nothing to remove.
  if (resultLength > 0 && lastCharacterWasPrintable) {
    if (codeSet == CODE_CODE_C) {
      result.erase(resultLength - 2, resultLength);
    } else {
      result.erase(resultLength - 1, resultLength);
    }
  }

  float left = (float) (startPatternInfo[1] + startPatternInfo[0]) / 2.0f;
  float right = lastStart + lastPatternSize / 2.0f;

  int rawCodesSize = rawCodes.size();
  ImageArrayRef<cx_byte> rawBytes (rawCodesSize);
  for (int i = 0; i < rawCodesSize; i++) {
    rawBytes[i] = rawCodes[i];
  }

  ImageArrayRef< ImageRef<ImageResultPoint> > resultPoints(2);
  resultPoints[0] =
      ImageRef<OneDCodeResultPoint>(new OneDCodeResultPoint(left, (float) rowNumber));
  resultPoints[1] =
      ImageRef<OneDCodeResultPoint>(new OneDCodeResultPoint(right, (float) rowNumber));

  return ImageRef<ImageResult>(new ImageResult(ImageRef<ImageString>(new ImageString(result)), rawBytes, resultPoints,
                                ImagecodeFormat::CODE_128));
}

OneDCode128Finder::~OneDCode128Finder(){}

ImageCodeType OneDCode128Finder::getImagecodeFormat(){
  return ImagecodeFormat::CODE_128;
}
ImageRef<ImageResult> OneDCode128Finder::doDecode(ImageRef<ImageBitmap> image, ImageCodeTypes itypes)
{
  int width = image->getWidth();
  int height = image->getHeight();
  ImageRef<ImageBitArray> row(new ImageBitArray(width));

  int middle = height >> 1;
  bool tryHarder = itypes.getTryHarder();
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
        ImageRef<ImageResult> result = decodeRow(rowNumber, row, itypes);
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
void OneDCode128Finder::QImagetoImage(const QImage &origin)
{
    QImage aimage1= origin.convertToFormat(QImage::Format_Grayscale8);

    QRgb pixel;
    cx_byte pixelGrayscale;

    const int width = origin.width();
    const int height = origin.height();

    imageBytes = ImageArrayRef<cx_byte>(height*width);
    imageBytesPerRow = ImageArrayRef<ImageArrayRef<cx_byte>>(height);
    cx_byte* m = &imageBytes[0];

    for(int j=0; j<height; j++)
    {
        ImageArrayRef<cx_byte> line(width);
        for(int i=0; i<width; i++)
        {
            pixel = origin.pixel(i,j);
            pixelGrayscale = pixel & 0xFF;
            line[i] = pixelGrayscale;
        }
        imageBytesPerRow[j] = line;

        memcpy(m, line->values().data(), width);

        m += width * sizeof(cx_byte);
    }

/*
    ImageWrapper *ciw = nullptr;

    if ((maxWidth > 0) || (maxHeight > 0))
        ciw = ImageWrapper::Factory(image, maxWidth, maxHeight, smoothTransformation);
    else
        ciw = ImageWrapper::Factory(image, 999, 999, true);
    ImageRef<ImageSource> imageRefOriginal = ImageRef<ImageSource>(ciw);
    ImageCodeTypes ictypes(static_cast<ImageCodeType>(ImageCodeTypes::CODE_128_HINT));

    ImageRef<ImageHistogramBinarizer> binz;
    ImageRef<ImageBitmap> bb;

    binz = ImageRef<ImageHistogramBinarizer>( new ImageHistogramBinarizer(imageRefOriginal) );
    bb = ImageRef<ImageBitmap>( new ImageBitmap(binz) );
*/
}

ImageRef<ImageResult> OneDCode128Finder::decodeRow(int rowNumber, ImageRef<ImageBitArray> row)
{
 // bool convertFNC1 = hints.containsFormat(ImagecodeFormat(ImagecodeFormat::Value::ASSUME_GS1));

  std::vector<int> startPatternInfo (findStartPattern(row));
  int startCode = startPatternInfo[2];
  int codeSet;
  switch (startCode) {
    case CODE_START_A:
      codeSet = CODE_CODE_A;
      break;
    case CODE_START_B:
      codeSet = CODE_CODE_B;
      break;
    case CODE_START_C:
      codeSet = CODE_CODE_C;
      break;
    default:
      throw CXException();
  }

  bool done = false;
  bool isNextShifted = false;

  std::string result;
  std::vector<cx_byte> rawCodes(20, 0);

  int lastStart = startPatternInfo[0];
  int nextStart = startPatternInfo[1];
  std::vector<int> counters (6, 0);

  int lastCode = 0;
  int code = 0;
  int checksumTotal = startCode;
  int multiplier = 0;
  bool lastCharacterWasPrintable = true;
  bool upperMode = false;
  bool shiftUpperMode = false;

  std::ostringstream oss;

  while (!done) {

    bool unshift = isNextShifted;
    isNextShifted = false;

    // Save off last code
    lastCode = code;

    code = decodeCode(row, counters, nextStart);

    // Remember whether the last code was printable or not (excluding CODE_STOP)
    if (code != CODE_STOP) {
      lastCharacterWasPrintable = true;
    }

    // Add to checksum computation (if not CODE_STOP of course)
    if (code != CODE_STOP) {
      multiplier++;
      checksumTotal += multiplier * code;
    }

    // Advance to where the next code will to start
    lastStart = nextStart;
    for (int i = 0, e = int(counters.size()); i < e; i++) {
      nextStart += counters[i];
    }

    // Take care of illegal start codes
    switch (code) {
      case CODE_START_A:
      case CODE_START_B:
      case CODE_START_C:
        throw CXException();
    }

    switch (codeSet) {

      case CODE_CODE_A:
        if (code < 64) {
            if (shiftUpperMode == upperMode) {
              result.append(1,(cx_byte) (' ' + code));
            } else {
              result.append(1,(cx_byte) (' ' + code + 128));
            }
            shiftUpperMode = false;
        } else if (code < 96) {
            if (shiftUpperMode == upperMode) {
              result.append(1, (cx_byte) (code - 64));
            } else {
              result.append(1, (cx_byte) (code + 64));
            }
            shiftUpperMode = false;
        } else {
          // Don't let CODE_STOP, which always appears, affect whether whether we think the
          // last code was printable or not.
          if (code != CODE_STOP) {
            lastCharacterWasPrintable = false;
          }
          switch (code) {
           /* case CODE_FNC_1:
              if (convertFNC1) {
                if (result.length() == 0){
                  // GS1 specification 5.4.3.7. and 5.4.6.4. If the first char after the start code
                  // is FNC1 then this is GS1-128. We add the symbology identifier.
                  result.append("]C1");
                } else {
                  // GS1 specification 5.4.7.5. Every subsequent FNC1 is returned as ASCII 29 (GS)
                  result.append(1, (cx_byte) 29);
                }
              }
              break;
              */
            case CODE_FNC_2:
            case CODE_FNC_3:
              break;
            case CODE_FNC_4_A:
              if (!upperMode && shiftUpperMode) {
                upperMode = true;
                shiftUpperMode = false;
              } else if (upperMode && shiftUpperMode) {
                upperMode = false;
                shiftUpperMode = false;
              } else {
                shiftUpperMode = true;
              }
              break;
            case CODE_SHIFT:
              isNextShifted = true;
              codeSet = CODE_CODE_B;
              break;
            case CODE_CODE_B:
              codeSet = CODE_CODE_B;
              break;
            case CODE_CODE_C:
              codeSet = CODE_CODE_C;
              break;
            case CODE_STOP:
              done = true;
              break;
          }
        }
        break;
      case CODE_CODE_B:
        if (code < 96) {
            if (shiftUpperMode == upperMode) {
              result.append(1, (cx_byte) (' ' + code));
            } else {
              result.append(1, (cx_byte) (' ' + code + 128));
            }
            shiftUpperMode = false;
        } else {
          if (code != CODE_STOP) {
            lastCharacterWasPrintable = false;
          }
          switch (code) {
          /*case CODE_FNC_1:
              if (convertFNC1) {
                if (result.length() == 0) {
                  // GS1 specification 5.4.3.7. and 5.4.6.4. If the first char after the start code
                  // is FNC1 then this is GS1-128. We add the symbology identifier.
                  result.append("]C1");
                } else {
                  // GS1 specification 5.4.7.5. Every subsequent FNC1 is returned as ASCII 29 (GS)
                  result.append(1, (cx_byte) 29);
                }
              }
              break;*/
            case CODE_FNC_2:
            case CODE_FNC_3:
              break;
            case CODE_FNC_4_B:
              if (!upperMode && shiftUpperMode) {
                upperMode = true;
                shiftUpperMode = false;
              } else if (upperMode && shiftUpperMode) {
                upperMode = false;
                shiftUpperMode = false;
              } else {
                shiftUpperMode = true;
              }
              break;
            case CODE_SHIFT:
              isNextShifted = true;
              codeSet = CODE_CODE_A;
              break;
            case CODE_CODE_A:
              codeSet = CODE_CODE_A;
              break;
            case CODE_CODE_C:
              codeSet = CODE_CODE_C;
              break;
            case CODE_STOP:
              done = true;
              break;
          }
        }
        break;
      case CODE_CODE_C:
        if (code < 100) {
          if (code < 10) {
            result.append(1, '0');
          }
          oss.clear();
          oss.str("");
          oss << code;
          result.append(oss.str());
        } else {
          if (code != CODE_STOP) {
            lastCharacterWasPrintable = false;
          }
          switch (code) {
            /*case CODE_FNC_1:
              if (convertFNC1) {
                if (result.length() == 0) {
                  // GS1 specification 5.4.3.7. and 5.4.6.4. If the first char after the start code
                  // is FNC1 then this is GS1-128. We add the symbology identifier.
                  result.append("]C1");
                } else {
                  // GS1 specification 5.4.7.5. Every subsequent FNC1 is returned as ASCII 29 (GS)
                  result.append(1, (cx_byte) 29);
                }
              }
              break;*/
            case CODE_CODE_A:
              codeSet = CODE_CODE_A;
              break;
            case CODE_CODE_B:
              codeSet = CODE_CODE_B;
              break;
            case CODE_STOP:
              done = true;
              break;
          }
        }
        break;
    }

    // Unshift back to another code set if we were shifted
    if (unshift) {
      codeSet = codeSet == CODE_CODE_A ? CODE_CODE_B : CODE_CODE_A;
    }

  }

  int lastPatternSize = nextStart - lastStart;

  // Check for ample whitespace following pattern, but, to do this we first need to remember that
  // we fudged decoding CODE_STOP since it actually has 7 bars, not 6. There is a black bar left
  // to read off. Would be slightly better to properly read. Here we just skip it:
  nextStart = row->getNextUnset(nextStart);
  if (!row->isRange(nextStart,
                    std::min(row->getSize(), nextStart + (nextStart - lastStart) / 2),
                    false)) {
    throw NoneException();
  }

  // Pull out from sum the value of the penultimate check code
  checksumTotal -= multiplier * lastCode;
  // lastCode is the checksum then:
  if (checksumTotal % 103 != lastCode) {
    throw CXException();
  }

  // Need to pull out the check digits from string
  int resultLength = result.length();
  if (resultLength == 0) {
    // false positive
    throw NoneException();
  }

  // Only bother if the result had at least one character, and if the checksum digit happened to
  // be a printable character. If it was just interpreted as a control code, nothing to remove.
  if (resultLength > 0 && lastCharacterWasPrintable) {
    if (codeSet == CODE_CODE_C) {
      result.erase(resultLength - 2, resultLength);
    } else {
      result.erase(resultLength - 1, resultLength);
    }
  }

  float left = (float) (startPatternInfo[1] + startPatternInfo[0]) / 2.0f;
  float right = lastStart + lastPatternSize / 2.0f;

  int rawCodesSize = rawCodes.size();
  ImageArrayRef<cx_byte> rawBytes (rawCodesSize);
  for (int i = 0; i < rawCodesSize; i++) {
    rawBytes[i] = rawCodes[i];
  }

  ImageArrayRef< ImageRef<ImageResultPoint> > resultPoints(2);
  resultPoints[0] =
      ImageRef<OneDCodeResultPoint>(new OneDCodeResultPoint(left, (float) rowNumber));
  resultPoints[1] =
      ImageRef<OneDCodeResultPoint>(new OneDCodeResultPoint(right, (float) rowNumber));

  return ImageRef<ImageResult>(new ImageResult(ImageRef<ImageString>(new ImageString(result)), rawBytes, resultPoints,
                                ImagecodeFormat::CODE_128));
}

ImageRef<ImageResult> OneDCode128Finder::doDecode(ImageRef<ImageBitmap> image, int rowStep,int maxLines)
{
  int width = image->getWidth();
  int height = image->getHeight();
  ImageRef<ImageBitArray> row(new ImageBitArray(width));

  int middle = height >> 1;
  //int rowStep = std::max(1, height >> (tryHarder ? 8 : 5));
  using namespace std;


  for (int x = 0; x < maxLines; x++)
  {
    // Scanning from the middle out. Determine which row we're looking at next:
    int rowStepsAboveOrBelow = (x + 1) >> 1;
    bool isAbove = (x & 0x01) == 0; // i.e. is x even?
    int rowNumber = middle + rowStep * (isAbove ? rowStepsAboveOrBelow : -rowStepsAboveOrBelow);
    if (false)
    {
        std::cerr << "rN "
                << rowNumber << " "
                << height << " "
                << middle << " "
                << rowStep << " "
                << isAbove << " "
                << rowStepsAboveOrBelow
                << std::endl;
    }
    if (rowNumber < 0 || rowNumber >= height)
    {
      // Oops, if we run off the top or bottom, stop
      break;
    }

    // Estimate black point for this row and load it:
    try
    {
      row = image->getBlackRow(rowNumber, row);
    }
    catch (NoneException const& ignored)
    {
      (void)ignored;
      continue;
    }

    // While we have the image data in a BitArray, it's fairly cheap to reverse it in place to
    // handle decoding upside down barcodes.
    for (int attempt = 0; attempt < 2; attempt++)
    {
      if (attempt == 1)
      {
        row->reverse(); // reverse the row and continue
      }

      try
      {
        // Look for a barcode
        ImageRef<ImageResult> result = decodeRow(rowNumber, row);
        // We found our barcode
        if (attempt == 1)
        {
              // But it was upside down, so note that
              // result.putMetadata(ResultMetadataType.ORIENTATION, new Integer(180));
              // And remember to flip the result points horizontally.
              ImageArrayRef< ImageRef<ImageResultPoint> > points(result->getImageResultPoints());
              if (points)
              {
                  points[0] = ImageRef<ImageResultPoint>(new OneDCodeResultPoint(width - points[0]->getX() - 1,
                                                             points[0]->getY()));
                  points[1] = ImageRef<ImageResultPoint>(new OneDCodeResultPoint(width - points[1]->getX() - 1,
                                                             points[1]->getY()));
              }
        }
        return result;
      }
      catch (CXException const& re)
      {
        (void)re;
        continue;
      }
    }
  }
  throw NoneException();
}
