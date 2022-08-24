
#ifndef IMAGEQR_ALIGNMENT_PATTERN_H
#define IMAGEQR_ALIGNMENT_PATTERN_H


#include <imageresultpoint.h>
#include <cmath>

class ImageQRAlignmentPattern : public ImageResultPoint {
		private:
			float estimatedModuleSize_;
			
		public:
            ImageQRAlignmentPattern(float posX, float posY, float estimatedModuleSize);
			bool aboutEquals(float moduleSize, float i, float j) const;
      ImageRef<ImageQRAlignmentPattern> combineEstimate(float i, float j,
                                            float newModuleSize) const;
		};
		


#endif // IMAGEQR_ALIGNMENT_PATTERN_H
