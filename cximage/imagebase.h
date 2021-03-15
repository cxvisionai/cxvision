#ifndef IMAGEBASE_H
#define IMAGEBASE_H

#include <QPainter>
#include <QWidget>

typedef struct sPoint
{
    short x;
    short y;
}SPoint;


class ImageBase :public QImage
{

public:
    ImageBase();

    ImageBase(const QImage &);

    ImageBase(const QSize &size,Format format);

    void ImageCopy(QImage &image);
    void ImageClear(int ibit=0);
    void ImageBar(int itimes,int ifade);
    void ImageThre(int ithre);
    void Image_5Blur_Gap_mud_thre_BW(int ithre,int increase,int igap,int ifindBorW);
    void Image_HSOThreshold(int iHbegin,int iHend,int iSbegin,int iSend,int iObegin,int iOend);
    void Image_ColorThreshold(int ir,int ig,int ib,int ihgap,int isgap,int iogap);
    void ImageMulDiv(double dvalue);
    void ImageAnd(ImageBase*pimage);
    void ImageAnd(void *paimage);
    void ImageZoomAnd(ImageBase *pimage,int ix,int iy,double dscalex,double dscaley);

    void ImageMoveAndX(int inum);
    void ImageMoveAndY(int inum);
    void ImageMoveOrX(int inum);
    void ImageMoveOrY(int inum);

    void Image_And(int inum);
    void Image_Or(int inum);

    //    void ImageXOR(ImageBase*pimage);
//    void ImageAND(ImageBase*pimage);
//    void ImageNOT(ImageBase*pimage);
    void ImageMove(int ix,int iy);
    void ImageIncrease(int inum);
    void ImageDecrease(int inum);
    void ImageFilp();
    void ImageMirror();
    void ImageZoom(double dscalex,double dscaley);
    void ImageZoomA(int ix,int iy,double dscalex,double dscaley);
    void ImagePen(int ix,int iy);
    void ImageGray();
    QPainter::CompositionMode m_mode;
    void SetMode(int imode);

    int m_ishow;
    void setshow(int ishow){m_ishow = ishow;}
    int getshow(){return m_ishow;}
    void draw(QPainter &painter);


    int m_ix0;
    int m_iy0;
    int m_iw;
    int m_ih;
    int ix0(){return m_ix0;}
    int iy0(){return m_iy0;}
    int iw(){return m_iw;}
    int ih(){return m_ih;}

    int PixelCompare(QRgb &pixel1,QRgb &pixel2,
                     int ihgap,int isgap,int iogap);

    void SetROI(int ix,int iy,int iw,int ih);
    void SaveROI(const char *pfilename);

    void ROIImage();
    void SavePath(const char *pfilename);
    void LoadPath(const char *pfilename);
    void ROIImageColor(QRgb pixel0);
    void ROIImageClear(int ibit);
    void ROIImageBar(int itimes,int ifade);
    void ROIImageThre(int ithre);
    void ROIImageColorGray();
    void ROIImageRGBThre(int iRthre,int iGthre,int iBthre,int iandor);
    void ROIImage_5Blur_Gap_mud_thre_BW(int ithre,int increase,int igap,int ifindBorW);
    void ROIImage_7Blur_Gap_mud_thre_BW(int ithre,int increase,int igap,int ifindBorW);

    void ROIImage_5Blur_Gap_mud_thre_BW_H(int ithre,int increase,int igap,int ifindBorW);
    void ROIImage_7Blur_Gap_mud_thre_BW_H(int ithre,int increase,int igap,int ifindBorW);

    void ROIImage_HSOThreshold(int iHbegin,int iHend,int iSbegin,int iSend,int iObegin,int iOend);
    void ROIImage_ColorThreshold(int ir,int ig,int ib,int ihgap,int isgap,int iogap);
    void ROIImageMulDiv(double dvalue);
    void ROIImageOR(ImageBase*pimage,QPainter::CompositionMode mode);
    void ROIImageXOR(ImageBase*pimage);
    void ROIImageAND(ImageBase*pimage);

    void ROIImageNOT(ImageBase*pimage);
    void ROIImageMove(int ix,int iy);
    void ROIImageIncrease(int inum);
    void ROIImageDecrease(int inum);
    void ROIImageFilp();
    void ROIImageMirror();
    void ROIImageGray();

    void ROIImageMoveAndX(int inum);
    void ROIImageMoveAndY(int inum);
    void ROIImageMoveOrX(int inum);
    void ROIImageMoveOrY(int inum);

    void ROIImageAnd(int inum);
    void ROIImageOr(int inum);

    void roitoroi(void *pimage);

    QPainterPath ImageTransfer(QPainterPath &path,int ix,int iy,double dangle=0.0,double dscalex=1.0,double dscaley=1.0);
    void ROIImageCopy(int ix1,int iy1,int iw1,int ih1);
    void ROItoROI(ImageBase *pimage);

    void ROIColorTable();
    void ROIColorTableBlur(int iGauss_Smoth,int ithre);
    void ROIColorTableEasyThre(int iandor);
    QRgb ROIBackground(int ileftgap,int irightgap,
                       int iupgap,int idowngap,int imethod=1);

    void ROIImageZoom(ImageBase *pimage,double dratex,double dratey);
    void ROIImageZoomEx(ImageBase *pimage,double dratex,double dratey);

    void ImageDrawPath(QPainterPath &path,int ix0,int iy0,QColor acolor);

    void clipboardROIimage();
};

#endif
