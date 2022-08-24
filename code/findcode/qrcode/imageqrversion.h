#ifndef IMAGEQR_VERSION_H
#define IMAGEQR_VERSION_H


#include <imagecounted.h>
#include <imageqrerrorcorrectionlevel.h>
#include <cxexception.h>
#include <imagebitmatrix.h>
#include <vector>


class QR_ECB {
private:
  int count_;
  int dataCodewords_;
public:
  QR_ECB(int count, int dataCodewords);
  int getCount();
  int getDataCodewords();
};

class QR_ECBlocks {
private:
  int ecCodewordsPerBloc_;
  std::vector<QR_ECB*> ecBlocks_;
public:
  QR_ECBlocks(int ecCodewordsPerBloc, QR_ECB *ecBlocks);
  QR_ECBlocks(int ecCodewordsPerBloc, QR_ECB *ecBlocks1, QR_ECB *ecBlocks2);
  int numBlocks() const;
  int getECCodewordsPerBloc();
  int getTotalECCodewords();
  std::vector<QR_ECB*>& getECBlocks();
  ~QR_ECBlocks();
};

class ImageQRVersion : public ImageCounted {

private:
  int versionNumber_;
  std::vector<int> &alignmentPatternCenters_;
  std::vector<QR_ECBlocks*> ecBlocks_;
  int totalCodewords_;
  ImageQRVersion(int versionNumber, std::vector<int> *alignmentPatternCenters, QR_ECBlocks *ecBlocks1, QR_ECBlocks *ecBlocks2,
          QR_ECBlocks *ecBlocks3, QR_ECBlocks *ecBlocks4);

public:
  static unsigned int VERSION_DECODE_INFO[];
  static int N_VERSION_DECODE_INFOS;
  static std::vector<ImageRef<ImageQRVersion> > IMAGEQRVERSIONS;

  ~ImageQRVersion();
  int getVersionNumber() const;
  std::vector<int> &getAlignmentPatternCenters();
  int getTotalCodewords();
  int getDimensionForVersion();
  QR_ECBlocks &getECBlocksForLevel(const ImageQRErrorCorrectionLevel &ecLevel) const;
  static ImageRef<ImageQRVersion> getProvisionalVersionForDimension(int dimension);
  static ImageRef<ImageQRVersion> getVersionForNumber(int versionNumber);
  static ImageRef<ImageQRVersion> decodeVersionInformation(unsigned int versionBits);
  ImageRef<ImageBitMatrix> buildFunctionPattern();
  static int buildVersions();
};


#endif // IMAGEQR_VERSION_H
