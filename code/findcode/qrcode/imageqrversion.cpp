
#include <qrcode/imageqrversion.h>
#include <qrcode/imageqrformatinformation.h>
#include <cxexception.h>
#include <limits>
#include <iostream>
#include <cstdarg>

using std::vector;
using std::numeric_limits;


QR_ECB::QR_ECB(int count, int dataCodewords) :
    count_(count), dataCodewords_(dataCodewords) {
}

int QR_ECB::getCount() {
  return count_;
}

int QR_ECB::getDataCodewords() {
  return dataCodewords_;
}

QR_ECBlocks::QR_ECBlocks(int ecCodewordsPerBloc, QR_ECB *ecBlocks) :
    ecCodewordsPerBloc_(ecCodewordsPerBloc), ecBlocks_() {
    ecBlocks_.push_back(ecBlocks);
}

QR_ECBlocks::QR_ECBlocks(int ecCodewordsPerBloc, QR_ECB *ecBlocks1, QR_ECB *ecBlocks2) :
    ecCodewordsPerBloc_(ecCodewordsPerBloc), ecBlocks_() {
  ecBlocks_.push_back(ecBlocks1);
  ecBlocks_.push_back(ecBlocks2);
}

int QR_ECBlocks::numBlocks() const
{
    int sumSizeOfBlocks = 0;
    for (size_t i=0; i<ecBlocks_.size(); ++i)
        sumSizeOfBlocks += ecBlocks_[i]->getCount();
    return sumSizeOfBlocks;
}

int QR_ECBlocks::getECCodewordsPerBloc()
{
  return ecCodewordsPerBloc_;
}

int QR_ECBlocks::getTotalECCodewords()
{
    return ecCodewordsPerBloc_ * numBlocks();//int(ecBlocks_.size());
}

std::vector<QR_ECB*>& QR_ECBlocks::getECBlocks() {
  return ecBlocks_;
}

QR_ECBlocks::~QR_ECBlocks() {
  for (size_t i = 0; i < ecBlocks_.size(); i++) {
    delete ecBlocks_[i];
  }
}

unsigned int ImageQRVersion::VERSION_DECODE_INFO[] = { 0x07C94, 0x085BC, 0x09A99, 0x0A4D3, 0x0BBF6, 0x0C762, 0x0D847, 0x0E60D,
    0x0F928, 0x10B78, 0x1145D, 0x12A17, 0x13532, 0x149A6, 0x15683, 0x168C9, 0x177EC, 0x18EC4, 0x191E1, 0x1AFAB,
    0x1B08E, 0x1CC1A, 0x1D33F, 0x1ED75, 0x1F250, 0x209D5, 0x216F0, 0x228BA, 0x2379F, 0x24B0B, 0x2542E, 0x26A64,
    0x27541, 0x28C69
                                              };
int ImageQRVersion::N_VERSION_DECODE_INFOS = 34;
vector<ImageRef<ImageQRVersion>> ImageQRVersion::IMAGEQRVERSIONS;
static int N_VERSIONS = ImageQRVersion::buildVersions();

int ImageQRVersion::getVersionNumber() const {
  return versionNumber_;
}

vector<int> &ImageQRVersion::getAlignmentPatternCenters() {
  return alignmentPatternCenters_;
}

int ImageQRVersion::getTotalCodewords() {
  return totalCodewords_;
}

int ImageQRVersion::getDimensionForVersion() {
  return 17 + 4 * versionNumber_;
}

QR_ECBlocks& ImageQRVersion::getECBlocksForLevel(const ImageQRErrorCorrectionLevel &ecLevel) const {
  return *ecBlocks_[ecLevel.ordinal()];
}

ImageRef<ImageQRVersion> ImageQRVersion::getProvisionalVersionForDimension(int dimension) {
  if (dimension % 4 != 1) {
    throw CXException();
  }
  try {
    return ImageQRVersion::getVersionForNumber((dimension - 17) >> 2);
  } catch (ImageException const& ignored) {
    (void)ignored;
    throw CXException();
  }
}

ImageRef<ImageQRVersion> ImageQRVersion::getVersionForNumber(int versionNumber) {
  if (versionNumber < 1 || versionNumber > N_VERSIONS) {
    throw CXException("versionNumber must be between 1 and 40");
  }

  return IMAGEQRVERSIONS[versionNumber - 1];
}

ImageQRVersion::ImageQRVersion(int versionNumber, vector<int> *alignmentPatternCenters, QR_ECBlocks *ecBlocks1, QR_ECBlocks *ecBlocks2,
                 QR_ECBlocks *ecBlocks3, QR_ECBlocks *ecBlocks4) :
    versionNumber_(versionNumber), alignmentPatternCenters_(*alignmentPatternCenters), ecBlocks_(4), totalCodewords_(0) {
  ecBlocks_[0] = ecBlocks1;
  ecBlocks_[1] = ecBlocks2;
  ecBlocks_[2] = ecBlocks3;
  ecBlocks_[3] = ecBlocks4;

  int total = 0;
  int ecCodewords = ecBlocks1->getECCodewordsPerBloc();
  vector<QR_ECB*> &ecbArray = ecBlocks1->getECBlocks();
  for (size_t i = 0; i < ecbArray.size(); i++) {
    QR_ECB *ecBlock = ecbArray[i];
    total += ecBlock->getCount() * (ecBlock->getDataCodewords() + ecCodewords);
  }
  totalCodewords_ = total;
}

ImageQRVersion::~ImageQRVersion() {
  delete &alignmentPatternCenters_;
  for (size_t i = 0; i < ecBlocks_.size(); i++) {
    delete ecBlocks_[i];
  }
}

ImageRef<ImageQRVersion> ImageQRVersion::decodeVersionInformation(unsigned int versionBits) {
  int bestDifference = numeric_limits<int>::max();
  size_t bestVersion = 0;
  for (int i = 0; i < N_VERSION_DECODE_INFOS; i++) {
    unsigned targetVersion = VERSION_DECODE_INFO[i];
    // Do the version info bits match exactly? done.
    if (targetVersion == versionBits) {
      return getVersionForNumber(i + 7);
    }
    // Otherwise see if this is the closest to a real version info bit
    // string we have seen so far
    int bitsDifference = ImageQRFormatInformation::numBitsDiffering(versionBits, targetVersion);
    if (bitsDifference < bestDifference) {
      bestVersion = i + 7;
      bestDifference = bitsDifference;
    }
  }
  // We can tolerate up to 3 bits of error since no two version info codewords will
  // differ in less than 4 bits.
  if (bestDifference <= 3) {
    return getVersionForNumber(int(bestVersion));
  }
  // If we didn't find a close enough match, fail
  return ImageRef<ImageQRVersion>();
}

ImageRef<ImageBitMatrix> ImageQRVersion::buildFunctionPattern() {
  int dimension = getDimensionForVersion();
  ImageRef<ImageBitMatrix> functionPattern(new ImageBitMatrix(dimension));


  // Top left finder pattern + separator + format
  functionPattern->setRegion(0, 0, 9, 9);
  // Top right finder pattern + separator + format
  functionPattern->setRegion(dimension - 8, 0, 8, 9);
  // Bottom left finder pattern + separator + format
  functionPattern->setRegion(0, dimension - 8, 9, 8);


  // Alignment patterns
  size_t max = alignmentPatternCenters_.size();
  for (size_t x = 0; x < max; x++) {
    int i = alignmentPatternCenters_[x] - 2;
    for (size_t y = 0; y < max; y++) {
      if ((x == 0 && (y == 0 || y == max - 1)) || (x == max - 1 && y == 0)) {
        // No alignment patterns near the three finder patterns
        continue;
      }
      functionPattern->setRegion(alignmentPatternCenters_[y] - 2, i, 5, 5);
    }
  }

  // Vertical timing pattern
  functionPattern->setRegion(6, 9, 1, dimension - 17);
  // Horizontal timing pattern
  functionPattern->setRegion(9, 6, dimension - 17, 1);

  if (versionNumber_ > 6) {
    // Version info, top right
    functionPattern->setRegion(dimension - 11, 0, 3, 6);
    // Version info, bottom left
    functionPattern->setRegion(0, dimension - 11, 6, 3);
  }

  return functionPattern;
}

static vector<int> *intArray(size_t n...) {
  va_list ap;
  va_start(ap, n);
  vector<int> *result = new vector<int>(n);
  for (size_t i = 0; i < n; i++) {
    (*result)[i] = va_arg(ap, int);
  }
  va_end(ap);
  return result;
}

int ImageQRVersion::buildVersions() {
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(1, intArray(0),
                                  new QR_ECBlocks(7, new QR_ECB(1, 19)),
                                  new QR_ECBlocks(10, new QR_ECB(1, 16)),
                                  new QR_ECBlocks(13, new QR_ECB(1, 13)),
                                  new QR_ECBlocks(17, new QR_ECB(1, 9)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(2, intArray(2, 6, 18),
                                  new QR_ECBlocks(10, new QR_ECB(1, 34)),
                                  new QR_ECBlocks(16, new QR_ECB(1, 28)),
                                  new QR_ECBlocks(22, new QR_ECB(1, 22)),
                                  new QR_ECBlocks(28, new QR_ECB(1, 16)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(3, intArray(2, 6, 22),
                                  new QR_ECBlocks(15, new QR_ECB(1, 55)),
                                  new QR_ECBlocks(26, new QR_ECB(1, 44)),
                                  new QR_ECBlocks(18, new QR_ECB(2, 17)),
                                  new QR_ECBlocks(22, new QR_ECB(2, 13)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(4, intArray(2, 6, 26),
                                  new QR_ECBlocks(20, new QR_ECB(1, 80)),
                                  new QR_ECBlocks(18, new QR_ECB(2, 32)),
                                  new QR_ECBlocks(26, new QR_ECB(2, 24)),
                                  new QR_ECBlocks(16, new QR_ECB(4, 9)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(5, intArray(2, 6, 30),
                                  new QR_ECBlocks(26, new QR_ECB(1, 108)),
                                  new QR_ECBlocks(24, new QR_ECB(2, 43)),
                                  new QR_ECBlocks(18, new QR_ECB(2, 15),
                                               new QR_ECB(2, 16)),
                                  new QR_ECBlocks(22, new QR_ECB(2, 11),
                                               new QR_ECB(2, 12)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(6, intArray(2, 6, 34),
                                  new QR_ECBlocks(18, new QR_ECB(2, 68)),
                                  new QR_ECBlocks(16, new QR_ECB(4, 27)),
                                  new QR_ECBlocks(24, new QR_ECB(4, 19)),
                                  new QR_ECBlocks(28, new QR_ECB(4, 15)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(7, intArray(3, 6, 22, 38),
                                  new QR_ECBlocks(20, new QR_ECB(2, 78)),
                                  new QR_ECBlocks(18, new QR_ECB(4, 31)),
                                  new QR_ECBlocks(18, new QR_ECB(2, 14),
                                               new QR_ECB(4, 15)),
                                  new QR_ECBlocks(26, new QR_ECB(4, 13),
                                               new QR_ECB(1, 14)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(8, intArray(3, 6, 24, 42),
                                  new QR_ECBlocks(24, new QR_ECB(2, 97)),
                                  new QR_ECBlocks(22, new QR_ECB(2, 38),
                                               new QR_ECB(2, 39)),
                                  new QR_ECBlocks(22, new QR_ECB(4, 18),
                                               new QR_ECB(2, 19)),
                                  new QR_ECBlocks(26, new QR_ECB(4, 14),
                                               new QR_ECB(2, 15)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(9, intArray(3, 6, 26, 46),
                                  new QR_ECBlocks(30, new QR_ECB(2, 116)),
                                  new QR_ECBlocks(22, new QR_ECB(3, 36),
                                               new QR_ECB(2, 37)),
                                  new QR_ECBlocks(20, new QR_ECB(4, 16),
                                               new QR_ECB(4, 17)),
                                  new QR_ECBlocks(24, new QR_ECB(4, 12),
                                               new QR_ECB(4, 13)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(10, intArray(3, 6, 28, 50),
                                  new QR_ECBlocks(18, new QR_ECB(2, 68),
                                               new QR_ECB(2, 69)),
                                  new QR_ECBlocks(26, new QR_ECB(4, 43),
                                               new QR_ECB(1, 44)),
                                  new QR_ECBlocks(24, new QR_ECB(6, 19),
                                               new QR_ECB(2, 20)),
                                  new QR_ECBlocks(28, new QR_ECB(6, 15),
                                               new QR_ECB(2, 16)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(11, intArray(3, 6, 30, 54),
                                  new QR_ECBlocks(20, new QR_ECB(4, 81)),
                                  new QR_ECBlocks(30, new QR_ECB(1, 50),
                                               new QR_ECB(4, 51)),
                                  new QR_ECBlocks(28, new QR_ECB(4, 22),
                                               new QR_ECB(4, 23)),
                                  new QR_ECBlocks(24, new QR_ECB(3, 12),
                                               new QR_ECB(8, 13)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(12, intArray(3, 6, 32, 58),
                                  new QR_ECBlocks(24, new QR_ECB(2, 92),
                                               new QR_ECB(2, 93)),
                                  new QR_ECBlocks(22, new QR_ECB(6, 36),
                                               new QR_ECB(2, 37)),
                                  new QR_ECBlocks(26, new QR_ECB(4, 20),
                                               new QR_ECB(6, 21)),
                                  new QR_ECBlocks(28, new QR_ECB(7, 14),
                                               new QR_ECB(4, 15)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(13, intArray(3, 6, 34, 62),
                                  new QR_ECBlocks(26, new QR_ECB(4, 107)),
                                  new QR_ECBlocks(22, new QR_ECB(8, 37),
                                               new QR_ECB(1, 38)),
                                  new QR_ECBlocks(24, new QR_ECB(8, 20),
                                               new QR_ECB(4, 21)),
                                  new QR_ECBlocks(22, new QR_ECB(12, 11),
                                               new QR_ECB(4, 12)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(14, intArray(4, 6, 26, 46, 66),
                                  new QR_ECBlocks(30, new QR_ECB(3, 115),
                                               new QR_ECB(1, 116)),
                                  new QR_ECBlocks(24, new QR_ECB(4, 40),
                                               new QR_ECB(5, 41)),
                                  new QR_ECBlocks(20, new QR_ECB(11, 16),
                                               new QR_ECB(5, 17)),
                                  new QR_ECBlocks(24, new QR_ECB(11, 12),
                                               new QR_ECB(5, 13)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(15, intArray(4, 6, 26, 48, 70),
                                  new QR_ECBlocks(22, new QR_ECB(5, 87),
                                               new QR_ECB(1, 88)),
                                  new QR_ECBlocks(24, new QR_ECB(5, 41),
                                               new QR_ECB(5, 42)),
                                  new QR_ECBlocks(30, new QR_ECB(5, 24),
                                               new QR_ECB(7, 25)),
                                  new QR_ECBlocks(24, new QR_ECB(11, 12),
                                               new QR_ECB(7, 13)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(16, intArray(4, 6, 26, 50, 74),
                                  new QR_ECBlocks(24, new QR_ECB(5, 98),
                                               new QR_ECB(1, 99)),
                                  new QR_ECBlocks(28, new QR_ECB(7, 45),
                                               new QR_ECB(3, 46)),
                                  new QR_ECBlocks(24, new QR_ECB(15, 19),
                                               new QR_ECB(2, 20)),
                                  new QR_ECBlocks(30, new QR_ECB(3, 15),
                                               new QR_ECB(13, 16)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(17, intArray(4, 6, 30, 54, 78),
                                  new QR_ECBlocks(28, new QR_ECB(1, 107),
                                               new QR_ECB(5, 108)),
                                  new QR_ECBlocks(28, new QR_ECB(10, 46),
                                               new QR_ECB(1, 47)),
                                  new QR_ECBlocks(28, new QR_ECB(1, 22),
                                               new QR_ECB(15, 23)),
                                  new QR_ECBlocks(28, new QR_ECB(2, 14),
                                               new QR_ECB(17, 15)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(18, intArray(4, 6, 30, 56, 82),
                                  new QR_ECBlocks(30, new QR_ECB(5, 120),
                                               new QR_ECB(1, 121)),
                                  new QR_ECBlocks(26, new QR_ECB(9, 43),
                                               new QR_ECB(4, 44)),
                                  new QR_ECBlocks(28, new QR_ECB(17, 22),
                                               new QR_ECB(1, 23)),
                                  new QR_ECBlocks(28, new QR_ECB(2, 14),
                                               new QR_ECB(19, 15)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(19, intArray(4, 6, 30, 58, 86),
                                  new QR_ECBlocks(28, new QR_ECB(3, 113),
                                               new QR_ECB(4, 114)),
                                  new QR_ECBlocks(26, new QR_ECB(3, 44),
                                               new QR_ECB(11, 45)),
                                  new QR_ECBlocks(26, new QR_ECB(17, 21),
                                               new QR_ECB(4, 22)),
                                  new QR_ECBlocks(26, new QR_ECB(9, 13),
                                               new QR_ECB(16, 14)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(20, intArray(4, 6, 34, 62, 90),
                                  new QR_ECBlocks(28, new QR_ECB(3, 107),
                                               new QR_ECB(5, 108)),
                                  new QR_ECBlocks(26, new QR_ECB(3, 41),
                                               new QR_ECB(13, 42)),
                                  new QR_ECBlocks(30, new QR_ECB(15, 24),
                                               new QR_ECB(5, 25)),
                                  new QR_ECBlocks(28, new QR_ECB(15, 15),
                                               new QR_ECB(10, 16)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(21, intArray(5, 6, 28, 50, 72, 94),
                                  new QR_ECBlocks(28, new QR_ECB(4, 116),
                                               new QR_ECB(4, 117)),
                                  new QR_ECBlocks(26, new QR_ECB(17, 42)),
                                  new QR_ECBlocks(28, new QR_ECB(17, 22),
                                               new QR_ECB(6, 23)),
                                  new QR_ECBlocks(30, new QR_ECB(19, 16),
                                               new QR_ECB(6, 17)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(22, intArray(5, 6, 26, 50, 74, 98),
                                  new QR_ECBlocks(28, new QR_ECB(2, 111),
                                               new QR_ECB(7, 112)),
                                  new QR_ECBlocks(28, new QR_ECB(17, 46)),
                                  new QR_ECBlocks(30, new QR_ECB(7, 24),
                                               new QR_ECB(16, 25)),
                                  new QR_ECBlocks(24, new QR_ECB(34, 13)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(23, intArray(5, 6, 30, 54, 78, 102),
                                  new QR_ECBlocks(30, new QR_ECB(4, 121),
                                               new QR_ECB(5, 122)),
                                  new QR_ECBlocks(28, new QR_ECB(4, 47),
                                               new QR_ECB(14, 48)),
                                  new QR_ECBlocks(30, new QR_ECB(11, 24),
                                               new QR_ECB(14, 25)),
                                  new QR_ECBlocks(30, new QR_ECB(16, 15),
                                               new QR_ECB(14, 16)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(24, intArray(5, 6, 28, 54, 80, 106),
                                  new QR_ECBlocks(30, new QR_ECB(6, 117),
                                               new QR_ECB(4, 118)),
                                  new QR_ECBlocks(28, new QR_ECB(6, 45),
                                               new QR_ECB(14, 46)),
                                  new QR_ECBlocks(30, new QR_ECB(11, 24),
                                               new QR_ECB(16, 25)),
                                  new QR_ECBlocks(30, new QR_ECB(30, 16),
                                               new QR_ECB(2, 17)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(25, intArray(5, 6, 32, 58, 84, 110),
                                  new QR_ECBlocks(26, new QR_ECB(8, 106),
                                               new QR_ECB(4, 107)),
                                  new QR_ECBlocks(28, new QR_ECB(8, 47),
                                               new QR_ECB(13, 48)),
                                  new QR_ECBlocks(30, new QR_ECB(7, 24),
                                               new QR_ECB(22, 25)),
                                  new QR_ECBlocks(30, new QR_ECB(22, 15),
                                               new QR_ECB(13, 16)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(26, intArray(5, 6, 30, 58, 86, 114),
                                  new QR_ECBlocks(28, new QR_ECB(10, 114),
                                               new QR_ECB(2, 115)),
                                  new QR_ECBlocks(28, new QR_ECB(19, 46),
                                               new QR_ECB(4, 47)),
                                  new QR_ECBlocks(28, new QR_ECB(28, 22),
                                               new QR_ECB(6, 23)),
                                  new QR_ECBlocks(30, new QR_ECB(33, 16),
                                               new QR_ECB(4, 17)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(27, intArray(5, 6, 34, 62, 90, 118),
                                  new QR_ECBlocks(30, new QR_ECB(8, 122),
                                               new QR_ECB(4, 123)),
                                  new QR_ECBlocks(28, new QR_ECB(22, 45),
                                               new QR_ECB(3, 46)),
                                  new QR_ECBlocks(30, new QR_ECB(8, 23),
                                               new QR_ECB(26, 24)),
                                  new QR_ECBlocks(30, new QR_ECB(12, 15),
                                               new QR_ECB(28, 16)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(28, intArray(6, 6, 26, 50, 74, 98, 122),
                                  new QR_ECBlocks(30, new QR_ECB(3, 117),
                                               new QR_ECB(10, 118)),
                                  new QR_ECBlocks(28, new QR_ECB(3, 45),
                                               new QR_ECB(23, 46)),
                                  new QR_ECBlocks(30, new QR_ECB(4, 24),
                                               new QR_ECB(31, 25)),
                                  new QR_ECBlocks(30, new QR_ECB(11, 15),
                                               new QR_ECB(31, 16)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(29, intArray(6, 6, 30, 54, 78, 102, 126),
                                  new QR_ECBlocks(30, new QR_ECB(7, 116),
                                               new QR_ECB(7, 117)),
                                  new QR_ECBlocks(28, new QR_ECB(21, 45),
                                               new QR_ECB(7, 46)),
                                  new QR_ECBlocks(30, new QR_ECB(1, 23),
                                               new QR_ECB(37, 24)),
                                  new QR_ECBlocks(30, new QR_ECB(19, 15),
                                               new QR_ECB(26, 16)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(30, intArray(6, 6, 26, 52, 78, 104, 130),
                                  new QR_ECBlocks(30, new QR_ECB(5, 115),
                                               new QR_ECB(10, 116)),
                                  new QR_ECBlocks(28, new QR_ECB(19, 47),
                                               new QR_ECB(10, 48)),
                                  new QR_ECBlocks(30, new QR_ECB(15, 24),
                                               new QR_ECB(25, 25)),
                                  new QR_ECBlocks(30, new QR_ECB(23, 15),
                                               new QR_ECB(25, 16)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(31, intArray(6, 6, 30, 56, 82, 108, 134),
                                  new QR_ECBlocks(30, new QR_ECB(13, 115),
                                               new QR_ECB(3, 116)),
                                  new QR_ECBlocks(28, new QR_ECB(2, 46),
                                               new QR_ECB(29, 47)),
                                  new QR_ECBlocks(30, new QR_ECB(42, 24),
                                               new QR_ECB(1, 25)),
                                  new QR_ECBlocks(30, new QR_ECB(23, 15),
                                               new QR_ECB(28, 16)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(32, intArray(6, 6, 34, 60, 86, 112, 138),
                                  new QR_ECBlocks(30, new QR_ECB(17, 115)),
                                  new QR_ECBlocks(28, new QR_ECB(10, 46),
                                               new QR_ECB(23, 47)),
                                  new QR_ECBlocks(30, new QR_ECB(10, 24),
                                               new QR_ECB(35, 25)),
                                  new QR_ECBlocks(30, new QR_ECB(19, 15),
                                               new QR_ECB(35, 16)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(33, intArray(6, 6, 30, 58, 86, 114, 142),
                                  new QR_ECBlocks(30, new QR_ECB(17, 115),
                                               new QR_ECB(1, 116)),
                                  new QR_ECBlocks(28, new QR_ECB(14, 46),
                                               new QR_ECB(21, 47)),
                                  new QR_ECBlocks(30, new QR_ECB(29, 24),
                                               new QR_ECB(19, 25)),
                                  new QR_ECBlocks(30, new QR_ECB(11, 15),
                                               new QR_ECB(46, 16)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(34, intArray(6, 6, 34, 62, 90, 118, 146),
                                  new QR_ECBlocks(30, new QR_ECB(13, 115),
                                               new QR_ECB(6, 116)),
                                  new QR_ECBlocks(28, new QR_ECB(14, 46),
                                               new QR_ECB(23, 47)),
                                  new QR_ECBlocks(30, new QR_ECB(44, 24),
                                               new QR_ECB(7, 25)),
                                  new QR_ECBlocks(30, new QR_ECB(59, 16),
                                               new QR_ECB(1, 17)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(35, intArray(7, 6, 30, 54, 78,
                                  102, 126, 150),
                                  new QR_ECBlocks(30, new QR_ECB(12, 121),
                                               new QR_ECB(7, 122)),
                                  new QR_ECBlocks(28, new QR_ECB(12, 47),
                                               new QR_ECB(26, 48)),
                                  new QR_ECBlocks(30, new QR_ECB(39, 24),
                                               new QR_ECB(14, 25)),
                                  new QR_ECBlocks(30, new QR_ECB(22, 15),
                                               new QR_ECB(41, 16)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(36, intArray(7, 6, 24, 50, 76,
                                  102, 128, 154),
                                  new QR_ECBlocks(30, new QR_ECB(6, 121),
                                               new QR_ECB(14, 122)),
                                  new QR_ECBlocks(28, new QR_ECB(6, 47),
                                               new QR_ECB(34, 48)),
                                  new QR_ECBlocks(30, new QR_ECB(46, 24),
                                               new QR_ECB(10, 25)),
                                  new QR_ECBlocks(30, new QR_ECB(2, 15),
                                               new QR_ECB(64, 16)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(37, intArray(7, 6, 28, 54, 80,
                                  106, 132, 158),
                                  new QR_ECBlocks(30, new QR_ECB(17, 122),
                                               new QR_ECB(4, 123)),
                                  new QR_ECBlocks(28, new QR_ECB(29, 46),
                                               new QR_ECB(14, 47)),
                                  new QR_ECBlocks(30, new QR_ECB(49, 24),
                                               new QR_ECB(10, 25)),
                                  new QR_ECBlocks(30, new QR_ECB(24, 15),
                                               new QR_ECB(46, 16)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(38, intArray(7, 6, 32, 58, 84,
                                  110, 136, 162),
                                  new QR_ECBlocks(30, new QR_ECB(4, 122),
                                               new QR_ECB(18, 123)),
                                  new QR_ECBlocks(28, new QR_ECB(13, 46),
                                               new QR_ECB(32, 47)),
                                  new QR_ECBlocks(30, new QR_ECB(48, 24),
                                               new QR_ECB(14, 25)),
                                  new QR_ECBlocks(30, new QR_ECB(42, 15),
                                               new QR_ECB(32, 16)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(39, intArray(7, 6, 26, 54, 82,
                                  110, 138, 166),
                                  new QR_ECBlocks(30, new QR_ECB(20, 117),
                                               new QR_ECB(4, 118)),
                                  new QR_ECBlocks(28, new QR_ECB(40, 47),
                                               new QR_ECB(7, 48)),
                                  new QR_ECBlocks(30, new QR_ECB(43, 24),
                                               new QR_ECB(22, 25)),
                                  new QR_ECBlocks(30, new QR_ECB(10, 15),
                                               new QR_ECB(67, 16)))));
  IMAGEQRVERSIONS.push_back(ImageRef<ImageQRVersion>( new ImageQRVersion(40, intArray(7, 6, 30, 58, 86,
                                  114, 142, 170),
                                  new QR_ECBlocks(30, new QR_ECB(19, 118),
                                               new QR_ECB(6, 119)),
                                  new QR_ECBlocks(28, new QR_ECB(18, 47),
                                               new QR_ECB(31, 48)),
                                  new QR_ECBlocks(30, new QR_ECB(34, 24),
                                               new QR_ECB(34, 25)),
                                  new QR_ECBlocks(30, new QR_ECB(20, 15),
                                               new QR_ECB(61, 16)))));
  return int(IMAGEQRVERSIONS.size());
}


