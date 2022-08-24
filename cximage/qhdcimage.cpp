#include "qhdcimage.h"

typedef struct 
{
    BITMAPINFOHEADER bmiHeader;
    DWORD redMask;
    DWORD greenMask;
    DWORD blueMask;
} BITMAPINFO_MASK;


QHDCImage::QHDCImage(int width, int height, QImage::Format format, bool isTextBuffer, HDC drawDC)
{
    BITMAPINFO_MASK bmi;
    memset(&bmi, 0, sizeof(bmi));
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       = width;
    bmi.bmiHeader.biHeight      = -height;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biSizeImage   = 0;

    bmi.bmiHeader.biBitCount    = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.redMask = 0;
    bmi.greenMask = 0;
    bmi.blueMask = 0;

    hdc = CreateCompatibleDC(drawDC);

    uchar *bits = 0;
    bitmap = CreateDIBSection(hdc, reinterpret_cast<BITMAPINFO *>(&bmi), DIB_RGB_COLORS, (void**) &bits, 0, 0);

    null_bitmap = (HBITMAP)SelectObject(hdc, bitmap);
    image = QImage(bits, width, height, format);//Format_RGB32
    GdiFlush();
}

QHDCImage::~QHDCImage()
{
    if (bitmap || hdc) {
        if (null_bitmap)
            SelectObject(hdc, null_bitmap);
        DeleteDC(hdc);
        DeleteObject(bitmap);
    }
}

QImage::Format QHDCImage::systemFormat()
{
    return QImage::Format_RGB32;
}