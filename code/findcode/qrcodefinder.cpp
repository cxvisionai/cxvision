

#include <qrcodefinder.h>
#include <imageqrdetector.h>
#include <imagedecoderresult.h>
#include <iostream>

        using namespace std;

        QRCodeFinder::QRCodeFinder() :decoder_() {
        }
        //TODO : see if any of the other files in the qrcode tree need tryHarder
        ImageRef<ImageResult> QRCodeFinder::decode(ImageRef<ImageBitmap> image, ImageCodeTypes hints) {
            ImageQRDetector detector(image->getBlackMatrix());
            ImageRef<ImageDetectorResult> detectorResult(detector.detect(hints));
            ImageArrayRef< ImageRef<ImageResultPoint> > points (detectorResult->getPoints());
            ImageRef<ImageDecoderResult> decoderResult(decoder_.decode(detectorResult->getBits()));
            ImageRef<ImageResult> result(new ImageResult(decoderResult->getText(), decoderResult->getRawBytes(), points, ImagecodeFormat::QR_CODE, decoderResult->charSet()));
            return result;
        }

        QRCodeFinder::~QRCodeFinder() {
        }

    ImageQRDecoder& QRCodeFinder::getImageDecoder() {
        return decoder_;
    }


