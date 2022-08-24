#ifndef IMAGE_QRFINDER_PATTERN_H
#define IMAGE_QRFINDER_PATTERN_H

#include <imageresultpoint.h>
#include <cmath>

class ImageQRFinderPattern : public ImageResultPoint {
		private:
			float estimatedModuleSize_;
			int count_;
            ImageQRFinderPattern(float posX, float posY, float estimatedModuleSize, int count);

		public:
             ImageQRFinderPattern(float posX, float posY, float estimatedModuleSize);
			int getCount() const;
			float getEstimatedModuleSize() const;
			void incrementCount();
			bool aboutEquals(float moduleSize, float i, float j) const;
           ImageRef<ImageQRFinderPattern> combineEstimate(float i, float j, float newModuleSize) const;
		};

#endif // IMAGE_FINDER_PATTERN_H
