

#include <iostream>
#include <common/reedsolomon/imagegenericgf.h>
#include <common/reedsolomon/imagegenericgfpoly.h>
#include <common/imagexception.h>


ImageRef<ImageGenericGF> ImageGenericGF::AZTEC_DATA_12(new ImageGenericGF(0x1069, 4096, 1));
ImageRef<ImageGenericGF> ImageGenericGF::AZTEC_DATA_10(new ImageGenericGF(0x409, 1024, 1));
ImageRef<ImageGenericGF> ImageGenericGF::AZTEC_DATA_6(new ImageGenericGF(0x43, 64, 1));
ImageRef<ImageGenericGF> ImageGenericGF::AZTEC_PARAM(new ImageGenericGF(0x13, 16, 1));
ImageRef<ImageGenericGF> ImageGenericGF::QR_CODE_FIELD_256(new ImageGenericGF(0x011D, 256, 0));
ImageRef<ImageGenericGF> ImageGenericGF::DATA_MATRIX_FIELD_256(new ImageGenericGF(0x012D, 256, 1));
ImageRef<ImageGenericGF> ImageGenericGF::AZTEC_DATA_8 = DATA_MATRIX_FIELD_256;
ImageRef<ImageGenericGF> ImageGenericGF::MAXICODE_FIELD_64 = AZTEC_DATA_6;

namespace {
  size_t INITIALIZATION_THRESHOLD = 0;
}

ImageGenericGF::ImageGenericGF(int primitive_, size_t size_, int b)
  : size(size_), primitive(primitive_), generatorBase(b), initialized(false) {
  if (size <= INITIALIZATION_THRESHOLD) {
    initialize();
  }
}

ImageGenericGF::~ImageGenericGF()
{
    return;
}

void ImageGenericGF::initialize() {
  expTable = std::vector<int>(size);
  logTable = std::vector<int>(size);

  size_t x = 1;

  for (size_t i = 0; i < size; i++) {
    expTable[i] = x;
    x <<= 1; // x = x * 2; we're assuming the generator alpha is 2
    if (x >= size) {
      x ^= primitive;
      x &= size-1;
    }
  }
  for (size_t i = 0; i < size-1; i++) {
    logTable.at(expTable.at(i)) = i;
  }
  //logTable[0] == 0 but this should never be used
  ImageArrayRef<int> coefficients_zero(1);
  ImageArrayRef<int> coefficients_one(1);

  coefficients_zero[0] = 0;
  coefficients_one[0] = 1;

  zero = ImageRef<ImageGenericGFPoly>(new ImageGenericGFPoly(this, coefficients_zero));
  one = ImageRef<ImageGenericGFPoly>(new ImageGenericGFPoly(this, coefficients_one));
  initialized = true;
}

void ImageGenericGF::checkInit() {
  if (!initialized) {
    initialize();
  }
}

ImageRef<ImageGenericGFPoly> ImageGenericGF::getZero() {
  checkInit();
  return zero;
}

ImageRef<ImageGenericGFPoly> ImageGenericGF::getOne() {
  checkInit();
  return one;
}

ImageRef<ImageGenericGFPoly> ImageGenericGF::buildMonomial(int degree, int coefficient) {
  checkInit();

  if (degree < 0) {
    throw ImageException("Degree must be non-negative");
  }
  if (coefficient == 0) {
    return zero;
  }
  ImageArrayRef<int> coefficients(degree + 1);
  coefficients[0] = coefficient;

  return ImageRef<ImageGenericGFPoly>(new ImageGenericGFPoly(this, coefficients));
}

int ImageGenericGF::addOrSubtract(int a, int b) {
  return a ^ b;
}

int ImageGenericGF::exp(int a) {
  checkInit();
  return expTable[a];
}

int ImageGenericGF::log(int a) {
  checkInit();
  if (a == 0) {
    throw ImageException("cannot give log(0)");
  }
  return logTable[a];
}

int ImageGenericGF::inverse(int a) {
  checkInit();
  if (a == 0) {
    throw ImageException("Cannot calculate the inverse of 0");
  }
  return expTable[size - logTable[a] - 1];
}

int ImageGenericGF::multiply(int a, int b) {
  checkInit();

  if (a == 0 || b == 0) {
    return 0;
  }

  return expTable[(logTable[a] + logTable[b]) % (size - 1)];
  }

size_t ImageGenericGF::getSize() {
  return size;
}

int ImageGenericGF::getGeneratorBase() {
  return generatorBase;
}
