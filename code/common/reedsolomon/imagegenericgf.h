
#ifndef IMAGEGENERICGF_H
#define IMAGEGENERICGF_H

#include <vector>
#include <common/imagecounted.h>

 class ImageGenericGFPoly;
  
  class ImageGenericGF : public ImageCounted {
    
  private:
    std::vector<int> expTable;
    std::vector<int> logTable;
    ImageRef<ImageGenericGFPoly> zero;
    ImageRef<ImageGenericGFPoly> one;
    size_t size;
    int primitive;
    int generatorBase;
    bool initialized;

    ImageGenericGF(int primitive, size_t size, int b);
    ~ImageGenericGF();
    
    void initialize();
    void checkInit();
    
  public:
    static ImageRef<ImageGenericGF> AZTEC_DATA_12;
    static ImageRef<ImageGenericGF> AZTEC_DATA_10;
    static ImageRef<ImageGenericGF> AZTEC_DATA_8;
    static ImageRef<ImageGenericGF> AZTEC_DATA_6;
    static ImageRef<ImageGenericGF> AZTEC_PARAM;
    static ImageRef<ImageGenericGF> QR_CODE_FIELD_256;
    static ImageRef<ImageGenericGF> DATA_MATRIX_FIELD_256;
    static ImageRef<ImageGenericGF> MAXICODE_FIELD_64;
    
    ImageRef<ImageGenericGFPoly> getZero();
    ImageRef<ImageGenericGFPoly> getOne();
    size_t getSize();
    int getGeneratorBase();
    ImageRef<ImageGenericGFPoly> buildMonomial(int degree, int coefficient);
    
    static int addOrSubtract(int a, int b);
    int exp(int a);
    int log(int a);
    int inverse(int a);
    int multiply(int a, int b);
  };


#endif //IMAGEGENERICGF_H

