#ifndef IMAGEDATAMATRIX_DECODER_DM_H
#define IMAGEDATAMATRIX_DECODER_DM_H



#include <common/reedsolomon/imagereedsolomondecoder.h>
#include <common/imagecounted.h>
#include <common/imagearray.h>
#include <imagedecoderresult.h>
#include <common/imagebitmatrix.h>


class ImageDataMatrixDecoder {
private:
  ImageReedSolomonDecoder rsDecoder_;

  void correctErrors(ImageArrayRef<cx_byte> bytes, int numDataCodewords);

public:
  ImageDataMatrixDecoder();

  ImageRef<ImageDecoderResult> decode(ImageRef<ImageBitMatrix> bits);
};

#endif // IMAGEDATAMATRIX_DECODER_DM_H
