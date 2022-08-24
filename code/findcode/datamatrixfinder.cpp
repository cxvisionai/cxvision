
#include <datamatrixfinder.h>
#include <datamatrix/detector/imagedatamatrixdetector.h>
#include <iostream>


using namespace std;

DataMatrixFinder::DataMatrixFinder() :
    decoder_() {
}

ImageRef<ImageResult> DataMatrixFinder::decode(ImageRef<ImageBitmap> image, DecodeHints hints) {
  (void)hints;
  ImageDetector detector(image->getBlackMatrix());
  Ref<DetectorResult> detectorResult(detector.detect());
  ArrayRef< Ref<ResultPoint> > points(detectorResult->getPoints());


  Ref<DecoderResult> decoderResult(decoder_.decode(detectorResult->getBits()));

  Ref<Result> result(
    new Result(decoderResult->getText(), decoderResult->getRawBytes(), points, BarcodeFormat::DATA_MATRIX));

  return result;
}

DataMatrixFinder::~DataMatrixFinder() {
}

