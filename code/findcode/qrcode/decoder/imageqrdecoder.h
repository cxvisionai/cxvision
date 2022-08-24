#ifndef IMAGE_DECODER_H
#define IMAGE_DECODER_H

#include <common/reedsolomon/imagereedsolomondecoder.h>
#include <common/imagecounted.h>
#include <common/imagearray.h>
#include <imagedecoderresult.h>
#include <common/imagebitmatrix.h>


class ImageQRDecoder {
private:
  ImageReedSolomonDecoder rsDecoder_;

  void correctErrors(ImageArrayRef<cx_byte> bytes, int numDataCodewords);

public:
  ImageQRDecoder();
  ImageRef<ImageDecoderResult> decode(ImageRef<ImageBitMatrix> bits);
};


#endif //IMAGE_DECODER_H
