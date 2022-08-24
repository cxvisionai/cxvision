#ifndef COMMON_DETECTOR_MATHTOOLS_H
#define COMMON_DETECTOR_MATHTOOLS_H

#include <cmath>
#include <vector>

#include <limits>

#include <float.h>
inline float nan() {return std::numeric_limits<float>::quiet_NaN();}
inline bool isnan_z(float v) {return std::isnan(v) != 0;}
inline bool isnan_z(double v) {return std::isnan(v) != 0;}

class MathTools {
 private:
  MathTools();
  ~MathTools();
 public:

  static inline int round(float d) {
    return (int) (d + 0.5f);
  }

  static inline float distance(float aX, float aY, float bX, float bY) {
    float xDiff = aX - bX;
    float yDiff = aY - bY;
    return sqrt(xDiff * xDiff + yDiff * yDiff);
  }

  static inline float distance(int aX, int aY, int bX, int bY) {
    int xDiff = aX - bX;
    int yDiff = aY - bY;
    return sqrt(float(xDiff * xDiff + yDiff * yDiff));
  }

  static inline int sum(std::vector<int> array) {
    int count = 0;
    for (int a : array) {
      count += a;
    }
    return count;
  }
};



#endif // COMMON_DETECTOR_MATHTOOLS_H

