
#include <qrcode/decoder/imageqrdecoder.h>
#include <qrcode/decoder/imageqrbitmatrixparser.h>
#include <qrcode/imageqrerrorcorrectionlevel.h>
#include <qrcode/imageqrversion.h>
#include <qrcode/decoder/imageqrdatablock.h>
#include <qrcode/decoder/imageqrdecodedbitstreamparser.h>
#include <imagexception.h>
//#include <ChecksumException.h>
#include <common/reedsolomon/imagereedsolomonexception.h>


ImageQRDecoder::ImageQRDecoder() :
  rsDecoder_(ImageGenericGF::QR_CODE_FIELD_256) {
}

void ImageQRDecoder::correctErrors(ImageArrayRef<cx_byte> codewordBytes, int numDataCodewords) {
  int numCodewords = codewordBytes->size();
  ImageArrayRef<int> codewordInts(numCodewords);
  for (int i = 0; i < numCodewords; i++) {
    codewordInts[i] = codewordBytes[i] & 0xff;
  }
  int numECCodewords = numCodewords - numDataCodewords;

  try {
    rsDecoder_.decode(codewordInts, numECCodewords);
  } catch (ImageReedSolomonException const& ignored) {
    (void)ignored;
    throw ImageException();
  }

  for (int i = 0; i < numDataCodewords; i++) {
    codewordBytes[i] = (cx_byte)codewordInts[i];
  }
}

ImageRef<ImageDecoderResult> ImageQRDecoder::decode(ImageRef<ImageBitMatrix> bits) {
  // Construct a parser and read version, error-correction level
  ImageQRBitMatrixParser parser(bits);

  // std::cerr << *bits << std::endl;

  ImageRef<ImageQRVersion>version = parser.readVersion();
  ImageQRErrorCorrectionLevel &ecLevel = parser.readFormatInformation()->getErrorCorrectionLevel();


  // Read codewords
  ImageArrayRef<cx_byte> codewords(parser.readCodewords());


  // Separate into data blocks
  std::vector<ImageRef<ImageQRDataBlock> > dataBlocks(ImageQRDataBlock::getDataBlocks(codewords, version, ecLevel));


  // Count total number of data bytes
  int totalBytes = 0;
  for (size_t i = 0; i < dataBlocks.size(); i++) {
    totalBytes += dataBlocks[i]->getNumDataCodewords();
  }
  ImageArrayRef<cx_byte> resultBytes(totalBytes);
  int resultOffset = 0;


  // Error-correct and copy data blocks together into a stream of bytes
  for (size_t j = 0; j < dataBlocks.size(); j++) {
    ImageRef<ImageQRDataBlock> dataBlock(dataBlocks[j]);
    ImageArrayRef<cx_byte> codewordBytes = dataBlock->getCodewords();
    int numDataCodewords = dataBlock->getNumDataCodewords();
    correctErrors(codewordBytes, numDataCodewords);
    for (int i = 0; i < numDataCodewords; i++) {
      resultBytes[resultOffset++] = (cx_byte)codewordBytes[i];
    }
  }

  return ImageQRDecodedBitStreamParser::decode(resultBytes,
                                        version,
                                        ecLevel,
                                        ImageQRDecodedBitStreamParser::Hashtable());
}

