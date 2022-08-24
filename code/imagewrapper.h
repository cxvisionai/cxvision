#ifndef IMAGEWARPPER_H
#define IMAGEWARPPER_H

#include <QImage>
#include <QString>
#include <imagegreysource.h>


class ImageWrapper : public ImageSource
{
public:
    ImageWrapper();
    ImageWrapper(const QImage& sourceImage);
    ImageWrapper(ImageWrapper& otherInstance);
    ~ImageWrapper();

    static ImageWrapper* Factory(const QImage& image, int maxWidth=-1, int maxHeight=-1, bool smoothTransformation=false);
    
    ImageArrayRef<ImageArrayRef<cx_byte> > getOriginalImage();
    ImageRef<ImageGreySource> getDelegate() { return delegate; }

    ImageArrayRef<cx_byte> getRow(int y, ImageArrayRef<cx_byte> row) const;
    ImageArrayRef<cx_byte> getMatrix() const;

    bool isCropSupported() const;
    ImageRef<ImageSource> crop(int left, int top, int width, int height) const;
    bool isRotateSupported() const;
    ImageRef<ImageSource> invert() const;
    ImageRef<ImageSource> rotateCounterClockwise() const;

    inline cx_byte gray(const unsigned int r, const unsigned int g, const unsigned int b);
  
private:
    ImageArrayRef<cx_byte> getRowP(int y, ImageArrayRef<cx_byte> row) const;
    ImageArrayRef<cx_byte> getMatrixP() const;
    void updateImageAsGrayscale(const QImage &origin);

    ImageRef<ImageGreySource> delegate;
    ImageArrayRef<ImageArrayRef<cx_byte>> imageBytesPerRow;
    ImageArrayRef<cx_byte> imageBytes;

    static const cx_byte B_TO_GREYSCALE[256];
    static const cx_byte G_TO_GREYSCALE[256];
    static const cx_byte R_TO_GREYSCALE[256];
};

#endif //IMAGEWARPPER_H
