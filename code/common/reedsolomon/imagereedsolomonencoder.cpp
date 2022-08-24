#include "imagereedsolomonencoder.h"

#include <cxexception.h>
#include <stdio.h>
#include <string.h>


ImageReedSolomonEncoder::ImageReedSolomonEncoder(ImageRef<ImageGenericGF> field) :
    field_(field), cachedGenerators_()
{
    ImageArrayRef<int> arrayRef(1); //will this work?
    arrayRef[0] = 1;
    ImageRef< ImageGenericGFPoly > tmpGeneratorRef(new ImageGenericGFPoly(field, arrayRef));
    cachedGenerators_.push_back(tmpGeneratorRef);
}

ImageRef<ImageGenericGFPoly> ImageReedSolomonEncoder::buildGenerator(int degree)
{
    if (degree >= int(cachedGenerators_.size())) {
        ImageRef<ImageGenericGFPoly> lastGenerator = cachedGenerators_.at(cachedGenerators_.size() - 1);
        for (int d = int(cachedGenerators_.size()); d <= degree; d++)
        {
            ImageArrayRef<int> arrayRef(2); //will this work?
            arrayRef[0] = 1;
            arrayRef[1] = field_->exp(d - 1 + field_->getGeneratorBase());
            ImageRef<ImageGenericGFPoly> tmpGFRef(new ImageGenericGFPoly(field_, arrayRef));
            ImageRef<ImageGenericGFPoly> nextGenerator = (*lastGenerator).multiply(tmpGFRef);
            cachedGenerators_.push_back(nextGenerator);
            lastGenerator = nextGenerator;
        }
    }

    // ??? wont this through exception?
    // No the elements up to index degree are added above
    return cachedGenerators_.at(size_t(degree));
}

void ImageReedSolomonEncoder::encode(std::vector<cx_byte> &toEncode, int ecBytes)
{
    if (ecBytes == 0) {
        throw CXException("No error correction bytes");
    }

    int dataBytes = int(toEncode.size());// - ecBytes;
    toEncode.resize(toEncode.size() + size_t(ecBytes));
    if (dataBytes <= 0) {
        throw CXException("No data bytes provided");
    }
    ImageRef<ImageGenericGFPoly> generator = buildGenerator(ecBytes);
    ImageArrayRef<int> infoCoefficients(dataBytes);

    //to-do optimize the following loop
    for(int i=0; i< dataBytes; i++)
        infoCoefficients[i] = toEncode[size_t(i)];

    ImageRef<ImageGenericGFPoly> info(new ImageGenericGFPoly(field_, infoCoefficients));
    info = info->multiplyByMonomial(ecBytes, 1);
    ImageRef<ImageGenericGFPoly> remainder = info->divide(generator)[1];
    ImageArrayRef<int> coefficients = remainder->getCoefficients();
    int numZeroCoefficients = ecBytes - coefficients->size();
    for (int i = 0; i < numZeroCoefficients; i++) {
        toEncode[size_t(dataBytes + i)] = 0;
    }

    for (int i = 0; i < coefficients->size(); i++)
      toEncode[size_t(dataBytes + numZeroCoefficients + i)] = cx_byte(coefficients[i]);
}

