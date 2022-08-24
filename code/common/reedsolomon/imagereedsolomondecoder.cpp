
#include <iostream>

#include <memory>
#include <common/reedsolomon/imagereedsolomondecoder.h>
#include <common/reedsolomon/imagereedsolomonexception.h>
#include <common/imagexception.h>
#include <cxexception.h>


ImageReedSolomonDecoder::ImageReedSolomonDecoder(ImageRef<ImageGenericGF> field_) : field(field_) {}

ImageReedSolomonDecoder::~ImageReedSolomonDecoder() {
}

void ImageReedSolomonDecoder::decode(ImageArrayRef<int> received, int twoS) {
  ImageRef<ImageGenericGFPoly> poly(new ImageGenericGFPoly(field, received));
  ImageArrayRef<int> syndromeCoefficients(twoS);
  bool noError = true;
  for (int i = 0; i < twoS; i++) {
    int eval = poly->evaluateAt(field->exp(i + field->getGeneratorBase()));
    syndromeCoefficients[syndromeCoefficients->size() - 1 - i] = eval;
    if (eval != 0) {
      noError = false;
    }
  }
  if (noError) {
    return;
  }
  ImageRef<ImageGenericGFPoly> syndrome(new ImageGenericGFPoly(field, syndromeCoefficients));
  std::vector<ImageRef<ImageGenericGFPoly> > sigmaOmega =
    runEuclideanAlgorithm(field->buildMonomial(twoS, 1), syndrome, twoS);
  ImageRef<ImageGenericGFPoly> sigma = sigmaOmega[0];
  ImageRef<ImageGenericGFPoly> omega = sigmaOmega[1];
  ImageArrayRef<int> errorLocations = findErrorLocations(sigma);
  ImageArrayRef<int> errorMagitudes = findErrorMagnitudes(omega, errorLocations);
  for (int i = 0; i < errorLocations->size(); i++) {
    int position = received->size() - 1 - field->log(errorLocations[i]);
    if (position < 0) {
      throw ImageReedSolomonException("Bad error location");
    }
    received[position] = ImageGenericGF::addOrSubtract(received[position], errorMagitudes[i]);
  }
}

std::vector<ImageRef<ImageGenericGFPoly> > ImageReedSolomonDecoder::runEuclideanAlgorithm(ImageRef<ImageGenericGFPoly> a,
                                                                      ImageRef<ImageGenericGFPoly> b,
                                                                      int R) {
  // Assume a's degree is >= b's
  if (a->getDegree() < b->getDegree()) {
    ImageRef<ImageGenericGFPoly> tmp = a;
    a = b;
    b = tmp;
  }

  ImageRef<ImageGenericGFPoly> rLast(a);
  ImageRef<ImageGenericGFPoly> r(b);
  ImageRef<ImageGenericGFPoly> tLast(field->getZero());
  ImageRef<ImageGenericGFPoly> t(field->getOne());

  // Run Euclidean algorithm until r's degree is less than R/2
  while (r->getDegree() >= R / 2) {
    ImageRef<ImageGenericGFPoly> rLastLast(rLast);
    ImageRef<ImageGenericGFPoly> tLastLast(tLast);
    rLast = r;
    tLast = t;

    // Divide rLastLast by rLast, with quotient q and remainder r
    if (rLast->isZero()) {
      // Oops, Euclidean algorithm already terminated?
      throw ImageReedSolomonException("r_{i-1} was zero");
    }
    r = rLastLast;
    ImageRef<ImageGenericGFPoly> q = field->getZero();
    int denominatorLeadingTerm = rLast->getCoefficient(rLast->getDegree());
    int dltInverse = field->inverse(denominatorLeadingTerm);
    while (r->getDegree() >= rLast->getDegree() && !r->isZero()) {
      int degreeDiff = r->getDegree() - rLast->getDegree();
      int scale = field->multiply(r->getCoefficient(r->getDegree()), dltInverse);
      q = q->addOrSubtract(field->buildMonomial(degreeDiff, scale));
      r = r->addOrSubtract(rLast->multiplyByMonomial(degreeDiff, scale));
    }

    t = q->multiply(tLast)->addOrSubtract(tLastLast);

    if (r->getDegree() >= rLast->getDegree()) {
      throw ImageException("Division algorithm failed to reduce polynomial?");
    }
  }

  int sigmaTildeAtZero = t->getCoefficient(0);
  if (sigmaTildeAtZero == 0) {
    throw ImageReedSolomonException("sigmaTilde(0) was zero");
  }

  int inverse = field->inverse(sigmaTildeAtZero);
  ImageRef<ImageGenericGFPoly> sigma(t->multiply(inverse));
  ImageRef<ImageGenericGFPoly> omega(r->multiply(inverse));
  std::vector<ImageRef<ImageGenericGFPoly> > result(2);
  result[0] = sigma;
  result[1] = omega;
  return result;
}

ImageArrayRef<int> ImageReedSolomonDecoder::findErrorLocations(ImageRef<ImageGenericGFPoly> errorLocator) {
  // This is a direct application of Chien's search
  int numErrors = errorLocator->getDegree();
  if (numErrors == 1) { // shortcut
    ImageArrayRef<int> result(new ImageArray<int>(1));
    result[0] = errorLocator->getCoefficient(1);
    return result;
  }
  ImageArrayRef<int> result(new ImageArray<int>(numErrors));
  int e = 0;
  for (size_t i = 1; i < field->getSize() && e < numErrors; i++) {
    if (errorLocator->evaluateAt(i) == 0) {
      result[e] = field->inverse(i);
      e++;
    }
  }
  if (e != numErrors) {
    throw ImageReedSolomonException("Error locator degree does not match number of roots");
  }
  return result;
}

ImageArrayRef<int> ImageReedSolomonDecoder::findErrorMagnitudes(ImageRef<ImageGenericGFPoly> errorEvaluator, ImageArrayRef<int> errorLocations) {
  // This is directly applying Forney's Formula
  int s = errorLocations->size();
  ImageArrayRef<int> result(new ImageArray<int>(s));
  for (int i = 0; i < s; i++) {
    int xiInverse = field->inverse(errorLocations[i]);
    int denominator = 1;
    for (int j = 0; j < s; j++) {
      if (i != j) {
        int term = field->multiply(errorLocations[j], xiInverse);
        int termPlus1 = (term & 0x1) == 0 ? term | 1 : term & ~1;
        denominator = field->multiply(denominator, termPlus1);
      }
    }
    result[i] = field->multiply(errorEvaluator->evaluateAt(xiInverse),
                                field->inverse(denominator));
    if (field->getGeneratorBase() != 0) {
      result[i] = field->multiply(result[i], xiInverse);
    }
  }
  return result;
}
