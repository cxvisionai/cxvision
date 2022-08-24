#ifndef QHDCIMAGE_H
#define QHDCIMAGE_H

#include <QtGui/QImage>
#include <QtCore/qt_windows.h>

class QHDCImage
{
public:
    QHDCImage(int width, int height, QImage::Format format, bool isTextBuffer, HDC drawDC);
    ~QHDCImage();

    inline int width() const;
    inline int height() const;

    QImage image;

    static QImage::Format systemFormat();

    HDC hdc;
    HBITMAP bitmap;
    HBITMAP null_bitmap;

private:
    Q_DISABLE_COPY(QHDCImage)
};

inline int QHDCImage::width() const { return image.width(); }
inline int QHDCImage::height() const { return image.height(); }
#endif // QHDCIMAGE_H