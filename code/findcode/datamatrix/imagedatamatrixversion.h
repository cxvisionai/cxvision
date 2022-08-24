#ifndef IMAGEDATAMATRIX_VERSION_H
#define IMAGEDATAMATRIX_VERSION_H


#include <imagexception.h>
#include <common/imagebitmatrix.h>
#include <common/imagecounted.h>
#include <vector>


class ECB {
private:
  int count_;
  int dataCodewords_;
public:
  ECB(int count, int dataCodewords);
  int getCount();
  int getDataCodewords();
};

class ECBlocks {
private:
  int ecCodewords_;
  std::vector<ECB*> ecBlocks_;
public:
  ECBlocks(int ecCodewords, ECB *ecBlocks);
  ECBlocks(int ecCodewords, ECB *ecBlocks1, ECB *ecBlocks2);
  int getECCodewords();
  std::vector<ECB*>& getECBlocks();
  ~ECBlocks();
};

class ImageDataMatrixVersion : public ImageCounted {
private:
  int versionNumber_;
  int symbolSizeRows_;
  int symbolSizeColumns_;
  int dataRegionSizeRows_;
  int dataRegionSizeColumns_;
  ECBlocks* ecBlocks_;
  int totalCodewords_;
  ImageDataMatrixVersion(int versionNumber, int symbolSizeRows, int symbolSizeColumns, int dataRegionSizeRows,
		  int dataRegionSizeColumns, ECBlocks *ecBlocks);

public:
  static std::vector<ImageRef<ImageDataMatrixVersion>> VERSIONS;
  
  ~Version();
  int getVersionNumber() const;
  int getSymbolSizeRows() const;
  int getSymbolSizeColumns() const;
  int getDataRegionSizeRows() const;
  int getDataRegionSizeColumns() const;
  int getTotalCodewords() const;
  ECBlocks* getECBlocks() const;
  static int  buildVersions();  
  Ref<Version>getVersionForDimensions(int numRows, int numColumns);
  
private:
  Version(const Version&);
  Version & operator=(const Version&);
};


#endif // IMAGEDATAMATRIX_VERSION_H
