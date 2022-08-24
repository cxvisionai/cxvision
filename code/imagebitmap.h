#ifndef IMAGEBITMAP_H
#define IMAGEBITMAP_H

#include <common/imagecounted.h>
#include <common/imagebitmatrix.h>
#include <common/imagebitarray.h>
#include <imagebinarizer.h>

	
    class ImageBitmap : public ImageCounted {
	private:
        ImageRef<ImageBinarizer> imagebinarizer_;
		
	public:
        ImageBitmap(ImageRef<ImageBinarizer> imagebinarizer);
        virtual ~ImageBitmap();
		
        ImageRef<ImageBitArray> getBlackRow(int y, ImageRef<ImageBitArray> row);
        ImageRef<ImageBitMatrix> getBlackMatrix();
		
        ImageRef<ImageSource> getimagesource() const;

		int getWidth() const;
		int getHeight() const;

		bool isRotateSupported() const;
        ImageRef<ImageBitmap> rotateCounterClockwise();
        ImageRef<ImageBitmap> rotateCounterClockwise45();

		bool isCropSupported() const;
        ImageRef<ImageBitmap> crop(int left, int top, int width, int height);
	};


#endif /* IMAGEBITMAP_H */
