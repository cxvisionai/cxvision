#ifndef IMAGERESULT_POINT_H
#define IMAGERESULT_POINT_H


#include <common/imagecounted.h>
#include <vector>

class ImageResultPoint : public ImageCounted {
protected:
  const float posX_;
  const float posY_;
  
public:
  ImageResultPoint();
  ImageResultPoint(float x, float y);
  ImageResultPoint(int x, int y);
  virtual ~ImageResultPoint();

  virtual float getX() const;
  virtual float getY() const;

  bool equals(const ImageRef<ImageResultPoint> &other);

  static void orderBestPatterns(std::vector<ImageRef<ImageResultPoint> > &patterns);
  static float distance(ImageRef<ImageResultPoint> point1, ImageRef<ImageResultPoint> point2);
  static float distance(float x1, float x2, float y1, float y2);

private:
  static float crossProductZ(ImageRef<ImageResultPoint> pointA, ImageRef<ImageResultPoint> pointB, ImageRef<ImageResultPoint> pointC);
};


#endif // IMAGERESULT_POINT_H
