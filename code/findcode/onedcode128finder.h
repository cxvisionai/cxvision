#ifndef ZXING_CODE_128_READER_H
#define ZXING_CODE_128_READER_H

#include <onedcodefinder.h>
#include <imagebitarray.h>
#include <imageresult.h>
#include <imagegreysource.h>
#include <QImage>

class OneDCode128Finder : public OneDCodeFinder {
private:
  static const int MAX_AVG_VARIANCE;
  static const int MAX_INDIVIDUAL_VARIANCE;

  static std::vector<int> findStartPattern(ImageRef<ImageBitArray> row);
  static int decodeCode(ImageRef<ImageBitArray> row,
                        std::vector<int>& counters,
                        int rowOffset);
			
public:
  ImageRef<ImageResult> doDecode(ImageRef<ImageBitmap> image, int rowStep=5,int maxLines=15);
  ImageRef<ImageResult> decodeRow(int rowNumber, ImageRef<ImageBitArray> row);

  ImageRef<ImageResult> doDecode(ImageRef<ImageBitmap> image, ImageCodeTypes itypes);
  ImageRef<ImageResult> decodeRow(int rowNumber, ImageRef<ImageBitArray> row, ImageCodeTypes itypes);
  OneDCode128Finder();
  ~OneDCode128Finder();

  ImageCodeType getImagecodeFormat();

  void QImagetoImage(const QImage &origin);
  ImageRef<ImageGreySource> delegate;
  ImageArrayRef<ImageArrayRef<cx_byte>> imageBytesPerRow;
  ImageArrayRef<cx_byte> imageBytes;

};


#endif // ZXING_CODE_128_READER_H

