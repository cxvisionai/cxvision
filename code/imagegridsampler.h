#ifndef IMAGE_GRID_SAMPLER_H
#define IMAGE_GRID_SAMPLER_H

#include <common/imagecounted.h>
#include <common/imagebitmatrix.h>
#include <common/imageperspectivetransform.h>

class ImageGridSampler {
private:
  static ImageGridSampler imagegridsampler;
  ImageGridSampler();

public:
  ImageRef<ImageBitMatrix> sampleGrid(ImageRef<ImageBitMatrix> image, int dimension, ImageRef<ImagePerspectiveTransform> transform);
  ImageRef<ImageBitMatrix> sampleGrid(ImageRef<ImageBitMatrix> image, int dimensionX, int dimensionY, ImageRef<ImagePerspectiveTransform> transform);

  ImageRef<ImageBitMatrix> sampleGrid(ImageRef<ImageBitMatrix> image, int dimension, float p1ToX, float p1ToY, float p2ToX, float p2ToY,
                            float p3ToX, float p3ToY, float p4ToX, float p4ToY, float p1FromX, float p1FromY, float p2FromX,
                            float p2FromY, float p3FromX, float p3FromY, float p4FromX, float p4FromY);
  static void checkAndNudgePoints(ImageRef<ImageBitMatrix> image, std::vector<float> &points);
  static ImageGridSampler &getInstance();
};


#endif // IMAGE_GRID_SAMPLER_H
