#ifndef FINDLINE_H
#define FINDLINE_H


#include "shape.h"

#include "shapebase.h"
#include "imagebase.h"

#include<vector>

using namespace std;

class findobject;
class findline:public QShape
{
public:
    findline();
    ~findline();
    int wgap(){return m_iwgap;}
    int hgap(){return m_ihgap;}
    int thre();

    void clear();
    void setshow(int ishow);
    virtual void setrect(int ix,int iy,int iw,int ih);
    virtual void drawshape(QPainter &painter,QPalette &pal);
    void drawshapex(QPainter &painter,QPalette &pal,double dmovx,double dmovy,
                    double dangle,double dzoomx,double dzoomy);

    void drawpattern(QPainter &painter);
    void drawpatternx(QPainter &painter,
                      double dmovx,double dmovy,
                      double dangle,
                      double dzoomx,double dzoomy);
    void edgepattern(ImageBase &image);
    void setcomparegap(int igap);
    void patternzeroposition();
    void savepatternfile(const char * pchar);
    void loadpatternfile(const char * pchar);
    QRectF patternboundingrect();
    void patternrootgrid(double itype,double drate,double ilevel);
    void patternzoom(double dx,double dy,double igap,double itype);
    void patterntranform(int igap,int itype,int isgap,int iline);
    void patternrotate(double dangle);
    void modelzoom(double dx,double dy);
    QPainterPath &getpatternpath();
    PointsShape &getpattern();
    void setpattern(PointsShape &apattern){m_modelpoints=apattern;}

    void SetWHgap(int wgap=2,int hgap=2);
    void measure(void *pimage);
    void findpattern(void *pimage);

    void setlinesamplerate(double dsamplerate);
    void setlinegap(int igap);
    void setmethod(int imethod);
    void setthre(int ithre);
    void setgamarate(int igama);
    void setfindsetting(int ifindset);
    void setfilter(int ifilterborw,int ifiltermin,int ifiltermax);//21 w ,22 b
    void setselectedgenum(int iedgenum);

    void setshowlines(int ilines){m_ishowlines=ilines;}
    PointsShape &getresultpointsw();
    PointsShape &getresultpointsh();
    void Measure(ImageBase &image);

    void shapesetroi(void *pshape);

    int getconparegap(){return m_icomparegap;}
private:
    int m_icomparegap;
    PointsShape m_modelpoints;    //red(white 1) gap blue(black 0) model

    PointsShape m_measurepointsA;
    PointsShape m_measurepointsB;

    PointsShape m_measurepointsA_;
    PointsShape m_measurepointsB_;

    ImageBase * g_pbackimage;

    findobject * g_pbackfindobject;

    PointsShape m_measurepoints_w;
    PointsShape m_measurepoints_h;

    LineShape m_LineA;
    LineShape m_LineB;

    LineVector m_lines_w;
    LineVector m_lines_h;

    int m_ihgap;
    int m_iwgap;

    QPoint* m_listscanorA;
    QPoint* m_listcollectorA;

    int m_iSelectPointGap;
    int m_iMethod;
    int m_iThreshold;
    int m_igamarate;
    double m_dsamplerate;

    int m_ifindset;
    int m_ifilterborw;//21 w ,22 b ,23
    qint64 m_ifiltermax;
    qint64 m_ifiltermin;

    int m_iselectedgenum;
    int m_ineedfixs;

    int m_ncurscan;
    int m_nscansize;

    int resultsize();


    //20201222
    static int m_curfindlinenum;

    int m_ishowlines;
};


#endif
