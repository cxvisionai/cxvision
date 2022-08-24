#ifndef IMAGE_WHITERECTANGLEDETECTOR_H
#define IMAGE_WHITERECTANGLEDETECTOR_H


#include <vector>
#include <imagexception.h>
#include <imageresultpoint.h>
#include <imagebitarray.h>
#include <imagecounted.h>
#include <imageresultpoint.h>
#include <imagebitmatrix.h>

class ImageWhiteRectangleDetector : public ImageCounted {
  private:
    static int INIT_SIZE;
    static int CORR;
    ImageRef<ImageBitMatrix> image_;
    int width_;
    int height_;
    int leftInit_;
    int rightInit_;
    int downInit_;
    int upInit_;

  public:
    ImageWhiteRectangleDetector(ImageRef<ImageBitMatrix> image);
    ImageWhiteRectangleDetector(ImageRef<ImageBitMatrix> image, int initSize, int x, int y);
    std::vector<ImageRef<ImageResultPoint> > detect();

  private: 
    ImageRef<ImageResultPoint> getBlackPointOnSegment(int aX, int aY, int bX, int bY);
    std::vector<ImageRef<ImageResultPoint> > centerEdges(ImageRef<ImageResultPoint> y, ImageRef<ImageResultPoint> z,
                                    ImageRef<ImageResultPoint> x, ImageRef<ImageResultPoint> t);
    bool containsBlackPoint(int a, int b, int fixed, bool horizontal);
};


#endif // IMAGE_WHITERECTANGLEDETECTOR_H

