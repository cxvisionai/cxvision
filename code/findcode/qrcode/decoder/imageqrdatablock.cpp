
#include <qrcode/decoder/imageqrdatablock.h>
#include <imagexception.h>


using namespace std;

ImageQRDataBlock::ImageQRDataBlock(int numDataCodewords, ImageArrayRef<cx_byte> codewords) :
    numDataCodewords_(numDataCodewords), codewords_(codewords) {
}

int ImageQRDataBlock::getNumDataCodewords() {
  return numDataCodewords_;
}

ImageArrayRef<cx_byte> ImageQRDataBlock::getCodewords() {
  return codewords_;
}


std::vector<ImageRef<ImageQRDataBlock> > ImageQRDataBlock::getDataBlocks(ImageArrayRef<cx_byte> rawCodewords, ImageRef<ImageQRVersion>version,
    ImageQRErrorCorrectionLevel &ecLevel) {


  // Figure out the number and size of data blocks used by this version and
  // error correction level
  QR_ECBlocks &ecBlocks = version->getECBlocksForLevel(ecLevel);


  // First count the total number of data blocks
  int totalBlocks = ecBlocks.numBlocks();
  vector<QR_ECB*> ecBlockArray = ecBlocks.getECBlocks();

  // Now establish ImageQRDataBlocks of the appropriate size and number of data codewords
  std::vector<ImageRef<ImageQRDataBlock> > result(totalBlocks);
  int numResultBlocks = 0;
  for (size_t j = 0; j < ecBlockArray.size(); j++) {
    QR_ECB *ecBlock = ecBlockArray[j];
    for (int i = 0; i < ecBlock->getCount(); i++) {
      int numDataCodewords = ecBlock->getDataCodewords();
      int numBlockCodewords = ecBlocks.getECCodewordsPerBloc() + numDataCodewords;
      ImageArrayRef<cx_byte> buffer(numBlockCodewords);
      ImageRef<ImageQRDataBlock> blockRef(new ImageQRDataBlock(numDataCodewords, buffer));
      result[numResultBlocks++] = blockRef;
    }
  }

  // All blocks have the same amount of data, except that the last n
  // (where n may be 0) have 1 more byte. Figure out where these start.
  int shorterBlocksTotalCodewords = result[0]->codewords_->size();
  int longerBlocksStartAt = int(result.size()) - 1;
  while (longerBlocksStartAt >= 0) {
    int numCodewords = result[longerBlocksStartAt]->codewords_->size();
    if (numCodewords == shorterBlocksTotalCodewords) {
      break;
    }
    if (numCodewords != shorterBlocksTotalCodewords + 1) {
      throw ImageException("Data block sizes differ by more than 1");
    }
    longerBlocksStartAt--;
  }
  longerBlocksStartAt++;

  int shorterBlocksNumDataCodewords = shorterBlocksTotalCodewords - ecBlocks.getECCodewordsPerBloc();
  // The last elements of result may be 1 element longer;
  // first fill out as many elements as all of them have
  int rawCodewordsOffset = 0;
  for (int i = 0; i < shorterBlocksNumDataCodewords; i++) {
    for (int j = 0; j < numResultBlocks; j++) {
      result[j]->codewords_[i] = rawCodewords[rawCodewordsOffset++];
    }
  }
  // Fill out the last data block in the longer ones
  for (int j = longerBlocksStartAt; j < numResultBlocks; j++) {
    result[j]->codewords_[shorterBlocksNumDataCodewords] = rawCodewords[rawCodewordsOffset++];
  }
  // Now add in error correction blocks
  int max = result[0]->codewords_->size();
  for (int i = shorterBlocksNumDataCodewords; i < max; i++) {
    for (int j = 0; j < numResultBlocks; j++) {
      int iOffset = j < longerBlocksStartAt ? i : i + 1;
      result[j]->codewords_[iOffset] = rawCodewords[rawCodewordsOffset++];
    }
  }

  if (rawCodewordsOffset != rawCodewords->size()) {
    throw ImageException("rawCodewordsOffset != rawCodewords.length");
  }

  return result;
}

