#ifndef ZXING_BINARYBITMAP_H
#define ZXING_BINARYBITMAP_H

#include <zxing/common/Counted.h>
#include <zxing/common/BitMatrix.h>
#include <zxing/common/BitArray.h>
#include <zxing/Binarizer.h>

namespace zxing {
	
	class BinaryBitmap : public Counted {
	private:
		Ref<Binarizer> binarizer_;
		
	public:
		BinaryBitmap(Ref<Binarizer> binarizer);
		virtual ~BinaryBitmap();
		
		Ref<BitArray> getBlackRow(int y, Ref<BitArray> row);
		Ref<BitMatrix> getBlackMatrix();
		
		Ref<LuminanceSource> getLuminanceSource() const;

		int getWidth() const;
		int getHeight() const;

		bool isRotateSupported() const;
		Ref<BinaryBitmap> rotateCounterClockwise();
        Ref<BinaryBitmap> rotateCounterClockwise45();

		bool isCropSupported() const;
		Ref<BinaryBitmap> crop(int left, int top, int width, int height);
	};
	
}

#endif /* ZXING_BINARYBITMAP_H */
