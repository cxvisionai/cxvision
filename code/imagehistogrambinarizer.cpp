

#include <imagehistogrambinarizer.h>
#include <nonexception.h>
#include <common/imagearray.h>
#include <common/imagebitmatrix.h>

const int IMAGE_BITS = 5;
const int IMAGE_SHIFT = 8 - IMAGE_BITS;
const int IMAGE_BUCKETS = 1 << IMAGE_BITS;
const ImageArrayRef<cx_byte> EMPTY (0);

ImageHistogramBinarizer::ImageHistogramBinarizer(ImageRef<ImageSource> source)
    : ImageBinarizer(source), image_(EMPTY), buckets(IMAGE_BUCKETS) {}

ImageHistogramBinarizer::~ImageHistogramBinarizer() {}

void ImageHistogramBinarizer::initArrays(int imageSize) {
    if (image_->size() < imageSize) {
        image_ = ImageArrayRef<cx_byte>(imageSize);
    }
//    for (int x = 0; x < IMAGE_BUCKETS; x++) {
//        buckets[x] = 0;
//    }
    memset(&buckets[0], 0, sizeof(int) * IMAGE_BUCKETS);
}

ImageRef<ImageBitArray> ImageHistogramBinarizer::getBlackRow(int y, ImageRef<ImageBitArray> row) {
    // std::cerr << "gbr " << y << std::endl;
    ImageSource& source = *getimagesource();
    int width = source.getWidth();
    if (row == NULL || static_cast<int>(row->getSize()) < width) {
        row = new ImageBitArray(width);
    } else {
        row->clear();
    }

    initArrays(width);
    ImageArrayRef<cx_byte> localimage_ = source.getRow(y, image_);
    if (false) {
        std::cerr << "gbr " << y << " r ";
        for(int i=0, e=localimage_->size(); i < e; ++i) {
            std::cerr << 0+localimage_[i] << " ";
        }
        std::cerr << std::endl;
    }
    ImageArrayRef<int> localBuckets = buckets;
    for (int x = 0; x < width; x++) {
        int pixel = localimage_[x] & 0xff;
        localBuckets[pixel >> IMAGE_SHIFT]++;
    }
    int blackPoint = estimateBlackPoint(localBuckets);
    // std::cerr << "gbr bp " << y << " " << blackPoint << std::endl;

    int left = localimage_[0] & 0xff;
    int center = localimage_[1] & 0xff;
    for (int x = 1; x < width - 1; x++) {
        int right = localimage_[x + 1] & 0xff;
        // A simple -1 4 -1 box filter with a weight of 2.
        int luminance = ((center << 2) - left - right) >> 1;
        if (luminance < blackPoint) {
            row->set(x);
        }
        left = center;
        center = right;
    }
    return row;
}

ImageRef<ImageBitMatrix> ImageHistogramBinarizer::getBlackMatrix() {
    ImageSource& source = *getimagesource();
    int width = source.getWidth();
    int height = source.getHeight();
    ImageRef<ImageBitMatrix> matrix(new ImageBitMatrix(width, height));

    // Quickly calculates the histogram by sampling four rows from the image.
    // This proved to be more robust on the blackbox tests than sampling a
    // diagonal as we used to do.
    initArrays(width);
    ImageArrayRef<int> localBuckets = buckets;
    for (int y = 1; y < 5; y++) {
        int row = height * y / 5;
        ImageArrayRef<cx_byte> localimage_ = source.getRow(row, image_);
        int right = (width << 2) / 5;
        for (int x = width / 5; x < right; x++) {
            int pixel = localimage_[x] & 0xff;
            localBuckets[pixel >> IMAGE_SHIFT]++;
        }
    }

    int blackPoint = estimateBlackPoint(localBuckets);

    ImageArrayRef<cx_byte> localimage_ = source.getMatrix();
    for (int y = 0; y < height; y++) {
        int offset = y * width;
        for (int x = 0; x < width; x++) {
            int pixel = localimage_[offset + x] & 0xff;
            if (pixel < blackPoint) {
                matrix->set(x, y);
            }
        }
    }

    return matrix;
}

using namespace std;

int ImageHistogramBinarizer::estimateBlackPoint(ImageArrayRef<int> const& buckets) {
    // Find tallest peak in histogram
    int numBuckets = buckets->size();
    int maxBucketCount = 0;
    int firstPeak = 0;
    int firstPeakSize = 0;
    if (false) {
        for (int x = 0; x < numBuckets; x++) {
            cerr << buckets[x] << " ";
        }
        cerr << endl;
    }
    for (int x = 0; x < numBuckets; x++) {
        if (buckets[x] > firstPeakSize) {
            firstPeak = x;
            firstPeakSize = buckets[x];
        }
        if (buckets[x] > maxBucketCount) {
            maxBucketCount = buckets[x];
        }
    }

    // Find second-tallest peak -- well, another peak that is tall and not
    // so close to the first one
    int secondPeak = 0;
    int secondPeakScore = 0;
    for (int x = 0; x < numBuckets; x++) {
        int distanceToBiggest = x - firstPeak;
        // Encourage more distant second peaks by multiplying by square of distance
        int score = buckets[x] * distanceToBiggest * distanceToBiggest;
        if (score > secondPeakScore) {
            secondPeak = x;
            secondPeakScore = score;
        }
    }

    if (firstPeak > secondPeak) {
        int temp = firstPeak;
        firstPeak = secondPeak;
        secondPeak = temp;
    }

    // Kind of arbitrary; if the two peaks are very close, then we figure there is
    // so little dynamic range in the image, that discriminating black and white
    // is too error-prone.
    // Decoding the image/line is either pointless, or may in some cases lead to
    // a false positive for 1D formats, which are relatively lenient.
    // We arbitrarily say "close" is
    // "<= 1/16 of the total histogram buckets apart"
    // std::cerr << "! " << secondPeak << " " << firstPeak << " " << numBuckets << std::endl;
    if (secondPeak - firstPeak <= numBuckets >> 4) {
        throw NoneException();
    }

    // Find a valley between them that is low and closer to the white peak
    int bestValley = secondPeak - 1;
    int bestValleyScore = -1;
    for (int x = secondPeak - 1; x > firstPeak; x--) {
        int fromFirst = x - firstPeak;
        // Favor a "valley" that is not too close to either peak -- especially not
        // the black peak -- and that has a low value of course
        int score = fromFirst * fromFirst * (secondPeak - x) *
                (maxBucketCount - buckets[x]);
        if (score > bestValleyScore) {
            bestValley = x;
            bestValleyScore = score;
        }
    }

    // std::cerr << "bps " << (bestValley << IMAGE_SHIFT) << std::endl;
    return bestValley << IMAGE_SHIFT;
}

ImageRef<ImageBinarizer> ImageHistogramBinarizer::createImageBinarizer(ImageRef<ImageSource> image) {
    return ImageRef<ImageBinarizer> (new ImageHistogramBinarizer(image));
}


