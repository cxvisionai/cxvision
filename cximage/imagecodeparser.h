#ifndef IMAGECODEPARSER_H
#define IMAGECODEPARSER_H

#include "shape.h"
#include "shapebase.h"
#include "imagebase.h"

#include "QZXing.h"


class fastmatch;
class imagecodeparser:public QShape
{

public:
    imagecodeparser();
    ~imagecodeparser();
    void setdecoder(int hint);
    void setsmoothtransformation(int ismooth);
    void setshow(int ishow);
    virtual void setrect(int ix,int iy,int iw,int ih);
    virtual void drawshape(QPainter &painter,QPalette &pal);
    void drawshapex(QPainter &painter,
                          QPalette &pal,
                          double dmovx,
                          double dmovy,
                          double dangle,
                          double dzoomx,
                          double dzoomy);
    void Measure(ImageBase &image);
    void Measure0(ImageBase &image);

    void measure(void *pimage);
    void measure0(void *pimage);
    void setrotate(int irotate){m_irotate=irotate;}
    int findcode(){return m_ifindcode;}
    int findcodex();
    int coderule(int isize);
public:
    int m_iovergap;

    QZXing m_decoder;
    bool m_bsmoothtransformation;
    QString m_result;

    QString m_decodedFormat;
    QString m_charSet_;
    QRectF m_cxrect;

    int m_irotate;
    int m_ifindcode;
    static int m_curimagecodeparsernum;
    fastmatch * m_prelationmatch;
    int m_irelationresultnum;

    double m_drelationzoomx;
    double m_drelationzoomy;
    QRect m_irelationrect;


    void setrelationrectfromresultnum(int inum);
    void setrelationrectfrom_matchresult(void *pmatch);
    void setrelationrectfrom_objectresult(void *pmatch);
    void setrelationxy(int iprex1,int iprey1,int iendx1,int iendy1);
    void setrelationzoom(double drelationzoomx,double drelationzoomy);
    void setrelationtorect();
    void setcolorstyle(int istyle);
    QString result(){return m_result;}


    void shapesetroi(void *pshape);
};












#endif
