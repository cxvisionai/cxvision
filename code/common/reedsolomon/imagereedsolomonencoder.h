#ifndef ImageReedSolomonEncoder_H
#define ImageReedSolomonEncoder_H

#include <common/reedsolomon/imagegenericgfpoly.h>
#include <common/reedsolomon/imagegenericgf.h>
#include <common/imagearray.h>


class ImageReedSolomonEncoder
{
private:
    ImageRef<ImageGenericGF> field_;
    std::vector< ImageRef< ImageGenericGFPoly > >cachedGenerators_;

    ImageRef<ImageGenericGFPoly> buildGenerator(int degree);

public:
    ImageReedSolomonEncoder(ImageRef<ImageGenericGF> field);

    void encode(std::vector<cx_byte> &toEncode, int ecBytes);
};


#endif // ImageReedSolomonEncoder_H
