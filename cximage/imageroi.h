#ifndef IMAGEROI_H
#define IMAGEROI_H

#include "shape.h"
#include "shapebase.h"
#include "imagebase.h"
class fastmatch;

typedef std::vector<int> ValueVector;
class ImageROI:public QShape
{
public:
    ImageROI();
    ~ImageROI();
    void setshow(int ishow);
    virtual void setrect(int ix,int iy,int iw,int ih);
    virtual void drawshape(QPainter &painter,QPalette &pal);
    void drawshapex(QPainter &painter,QPalette &pal,double dmovx,double dmovy,
                          double dangle,double dzoomx,double dzoomy);



    PointsShape m_points;
    ValueVector m_ivalues;
    RectsShape m_rectgrids;

    int m_isplitx = 1;
    int m_isplity = 1;
    int m_iselectx = 0;
    int m_iselecty = 0;

    int m_iexw=10;
    int m_iexh=10;
    int m_ioffset = 0;


    int m_irelationresultnum;
    double m_drelationzoomx;
    double m_drelationzoomy;
    QRect m_irelationrect;

    fastmatch * m_prelationmatch;
    ImageBase * m_pimage;
public:
    void getimage(void * pimage);
    void setrelationrectfromresultnum(int inum);
    void setrelationrectfrom_shape(void *pshape);
    void setrelationrectfrom_matchresult(void *pmatch);
    void setrelationrectfrom_objectresult(void *pobj);
    void setrelationpart(int igridw,int igridh,int igridx,int igridy);
    void setrelationxy(int iprex1,int iprey1,int iendx1,int iendy1);
    void setrelationzoom(double drelationzoomx,double drelationzoomy);
    void setrelationtorect();
    void setcolorstyle(int istyle);

    void ColorTable();
    void ColorTableBlur(int iGauss_Smoth,int ithre);
    void ColorTableEasyThre(int ioffset);
    int tablevalue(int icolor);
    void roithre(int ithre);

    void getfindlineresult(void *pfindline);
    void roiedge(int ithre,int ireserve,int igap,int ifindBorW);
    void roiedgeh(int ithre,int ireserve,int igap,int ifindBorW);

    void roiedge7(int ithre,int ireserve,int igap,int ifindBorW);
    void roiedgeh7(int ithre,int ireserve,int igap,int ifindBorW);

    void setsplitrect(int isplitx,int isplity,int iexw,int iexh);
    void selectrect(int iselectx,int iselecty);
    void findrect(void *pfindline);

    void shapesetroi(void *pshape);

    void roieasythre(int iandor,int ioffset);
};












#endif
