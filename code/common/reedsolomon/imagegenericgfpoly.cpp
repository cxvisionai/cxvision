
#include <iostream>
#include <common/reedsolomon/imagegenericgfpoly.h>
#include <common/reedsolomon/imagegenericgf.h>
#include <common/imagexception.h>


ImageGenericGFPoly::ImageGenericGFPoly(ImageGenericGF *field,
                             ImageArrayRef<int> coefficients)
  :  field_(field) {
  if (coefficients->size() == 0) {
    throw ImageException("need coefficients");
  }
  int coefficientsLength = coefficients->size();
  if (coefficientsLength > 1 && coefficients[0] == 0) {
    // Leading term must be non-zero for anything except the constant polynomial "0"
    int firstNonZero = 1;
    while (firstNonZero < coefficientsLength && coefficients[firstNonZero] == 0) {
      firstNonZero++;
    }
    if (firstNonZero == coefficientsLength) {
      coefficients_ = field->getZero()->getCoefficients();
    } else {
      coefficients_ = ImageArrayRef<int>(coefficientsLength-firstNonZero);
      for (int i = 0; i < (int)coefficients_->size(); i++) {
        coefficients_[i] = coefficients[i + firstNonZero];
      }
    }
  } else {
    coefficients_ = coefficients;
  }
}
  
ImageArrayRef<int> ImageGenericGFPoly::getCoefficients() {
  return coefficients_;
}
  
int ImageGenericGFPoly::getDegree() {
  return coefficients_->size() - 1;
}
  
bool ImageGenericGFPoly::isZero() {
  return coefficients_[0] == 0;
}
  
int ImageGenericGFPoly::getCoefficient(int degree) {
  return coefficients_[coefficients_->size() - 1 - degree];
}
  
int ImageGenericGFPoly::evaluateAt(int a) {
  if (a == 0) {
    // Just return the x^0 coefficient
    return getCoefficient(0);
  }
    
  int size = coefficients_->size();
  if (a == 1) {
    // Just the sum of the coefficients
    int result = 0;
    for (int i = 0; i < size; i++) {
      result = ImageGenericGF::addOrSubtract(result, coefficients_[i]);
    }
    return result;
  }
  int result = coefficients_[0];
  for (int i = 1; i < size; i++) {
    result = ImageGenericGF::addOrSubtract(field_->multiply(a, result), coefficients_[i]);
  }
  return result;
}
  
ImageRef<ImageGenericGFPoly> ImageGenericGFPoly::addOrSubtract(ImageRef<ImageGenericGFPoly> other) {
  if (!(field_ == other->field_)) {
    throw ImageException("ImageGenericGFPolys do not have same GenericGF field");
  }
  if (isZero()) {
    return other;
  }
  if (other->isZero()) {
    return ImageRef<ImageGenericGFPoly>(this);
  }
    
  ImageArrayRef<int> smallerCoefficients = coefficients_;
  ImageArrayRef<int> largerCoefficients = other->getCoefficients();
  if (smallerCoefficients->size() > largerCoefficients->size()) {
    ImageArrayRef<int> temp = smallerCoefficients;
    smallerCoefficients = largerCoefficients;
    largerCoefficients = temp;
  }
    
  ImageArrayRef<int> sumDiff(largerCoefficients->size());
  int lengthDiff = largerCoefficients->size() - smallerCoefficients->size();
  // Copy high-order terms only found in higher-degree polynomial's coefficients
  for (int i = 0; i < lengthDiff; i++) {
    sumDiff[i] = largerCoefficients[i];
  }
    
  for (int i = lengthDiff; i < (int)largerCoefficients->size(); i++) {
    sumDiff[i] = ImageGenericGF::addOrSubtract(smallerCoefficients[i-lengthDiff],
                                          largerCoefficients[i]);
  }
    
  return ImageRef<ImageGenericGFPoly>(new ImageGenericGFPoly(field_, sumDiff));
}
  
ImageRef<ImageGenericGFPoly> ImageGenericGFPoly::multiply(ImageRef<ImageGenericGFPoly> other) {
  if (!(field_ == other->field_)) {
    throw ImageException("ImageGenericGFPolys do not have same GenericGF field");
  }
    
  if (isZero() || other->isZero()) {
    return field_->getZero();
  }
    
  ImageArrayRef<int> aCoefficients = coefficients_;
  int aLength = aCoefficients->size();
    
  ImageArrayRef<int> bCoefficients = other->getCoefficients();
  int bLength = bCoefficients->size();
    
  ImageArrayRef<int> product(aLength + bLength - 1);
  for (int i = 0; i < aLength; i++) {
    int aCoeff = aCoefficients[i];
    for (int j = 0; j < bLength; j++) {
      product[i+j] = ImageGenericGF::addOrSubtract(product[i+j],
                                              field_->multiply(aCoeff, bCoefficients[j]));
    }
  }
    
  return ImageRef<ImageGenericGFPoly>(new ImageGenericGFPoly(field_, product));
}
  
ImageRef<ImageGenericGFPoly> ImageGenericGFPoly::multiply(int scalar) {
  if (scalar == 0) {
    return field_->getZero();
  }
  if (scalar == 1) {
    return ImageRef<ImageGenericGFPoly>(this);
  }
  int size = coefficients_->size();
  ImageArrayRef<int> product(size);
  for (int i = 0; i < size; i++) {
    product[i] = field_->multiply(coefficients_[i], scalar);
  }
  return ImageRef<ImageGenericGFPoly>(new ImageGenericGFPoly(field_, product));
}
  
ImageRef<ImageGenericGFPoly> ImageGenericGFPoly::multiplyByMonomial(int degree, int coefficient) {
  if (degree < 0) {
    throw ImageException("degree must not be less then 0");
  }
  if (coefficient == 0) {
    return field_->getZero();
  }
  int size = coefficients_->size();
  ImageArrayRef<int> product(size+degree);
  for (int i = 0; i < size; i++) {
    product[i] = field_->multiply(coefficients_[i], coefficient);
  }
  return ImageRef<ImageGenericGFPoly>(new ImageGenericGFPoly(field_, product));
}
  
std::vector<ImageRef<ImageGenericGFPoly>> ImageGenericGFPoly::divide(ImageRef<ImageGenericGFPoly> other) {
  if (!(field_ == other->field_)) {
    throw ImageException("ImageGenericGFPolys do not have same GenericGF field");
  }
  if (other->isZero()) {
    throw ImageException("divide by 0");
  }
    
  ImageRef<ImageGenericGFPoly> quotient = field_->getZero();
  ImageRef<ImageGenericGFPoly> remainder = ImageRef<ImageGenericGFPoly>(this);
    
  int denominatorLeadingTerm = other->getCoefficient(other->getDegree());
  int inverseDenominatorLeadingTerm = field_->inverse(denominatorLeadingTerm);
    
  while (remainder->getDegree() >= other->getDegree() && !remainder->isZero()) {
    int degreeDifference = remainder->getDegree() - other->getDegree();
    int scale = field_->multiply(remainder->getCoefficient(remainder->getDegree()),
                                 inverseDenominatorLeadingTerm);
    ImageRef<ImageGenericGFPoly> term = other->multiplyByMonomial(degreeDifference, scale);
    ImageRef<ImageGenericGFPoly> iterationQuotiont = field_->buildMonomial(degreeDifference,
                                                                 scale);
    quotient = quotient->addOrSubtract(iterationQuotiont);
    remainder = remainder->addOrSubtract(term);
  }
    
  std::vector<ImageRef<ImageGenericGFPoly> > returnValue;
  returnValue.push_back(quotient);
  returnValue.push_back(remainder);
  return returnValue;
}
