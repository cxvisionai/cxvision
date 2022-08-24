#ifndef IMAGE_ONED_FINDER_H
#define IMAGE_ONED_FINDER_H


#include <imagefinder.h>
#include <imagecodetypes.h>

class OneDCodeFinder : public ImageFinder {
private:
  ImageRef<ImageResult> doDecode(ImageRef<ImageBitmap> image, ImageCodeTypes itypes);

protected:
  static const int INTEGER_MATH_SHIFT = 8;

  static int patternMatchVariance(std::vector<int>& counters,
                                  std::vector<int> const& pattern,
                                  int maxIndividualVariance);
  static int patternMatchVariance(std::vector<int>& counters,
                                  int const pattern[],
                                  int maxIndividualVariance);

protected:
  static const int PATTERN_MATCH_RESULT_SCALE_FACTOR = 1 << INTEGER_MATH_SHIFT;

public:
  struct Range {
  private:
    int data[2];
  public:
    Range() {}
    Range(int zero, int one) {
      data[0] = zero;
      data[1] = one;
    }
    int& operator [] (int index) {
      return data[index];
    }
    int const& operator [] (int index) const {
      return data[index];
    }
  };

  virtual ImageRef<ImageResult> decode(ImageRef<ImageBitmap> image, ImageCodeTypes itypes);

  // Implementations must not throw any exceptions. If a barcode is not found on this row,
  // a empty ref should be returned e.g. return Ref<Result>();
  virtual ImageRef<ImageResult> decodeRow(int rowNumber, ImageRef<ImageBitArray> row, ImageCodeTypes itypes) = 0;

  static void recordPattern(ImageRef<ImageBitArray> row,
                            int start,
                            std::vector<int>& counters);

  static void recordPatternInReverse(ImageRef<ImageBitArray> row,
                                     int start,
                                     std::vector<int>& counters);
  virtual ~OneDCodeFinder();
};

#endif // IMAGE_ONED_FINDER_H

