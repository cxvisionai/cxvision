
#ifndef IMAGEDATAMATRIX_DETECTOREXCEPTION_H_
#define IMAGEDATAMATRIX_DETECTOREXCEPTION_H_

#include <cxexception.h>


class ImageDataMatrixDetectorException : public CXException {
  public:
    ImageDataMatrixDetectorException(const char *msg);
    virtual ~ImageDataMatrixDetectorException() CX_NOEXCEPT;
};

#endif // IMAGEDATAMATRIX_DETECTOREXCEPTION_H_
