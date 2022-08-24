#ifndef FINDOBJECT_H
#define FINDOBJECT_H

#include "shape.h"

#include "shapebase.h"
#include "imagebase.h"


#include<vector>
typedef struct tagAPOINT{short x;short y;}APOINT;
/*G_SearchPointGroup Search Matrix */

typedef vector<int> vectorint;
typedef vectorint::iterator vectintitor;

class QGrid;
class fastmatch;
class findobject:public QShape
{
    enum ObjectAnalysisType
    {
        ANALYSIS_NO=0,
        ANLAYSIS_COLLECTION=1,
        ANLAYSIS_OVER=2,
        ANLAYSIS_OK=3,
        ANLAYSIS_READY=4,
        ANLAYSIS_NOVALID=5
    };
    enum ObjectSearchType
    {
        Search_O=0,
        Search_OL=1,
        Search_OR=2,
        Search_OU=3,
        Search_OD=4,
        Search_OX=5
    };

public:
    findobject();
    ~findobject();

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
    void MeasureGrid(QGrid *grid);
    void MeasureX(ImageBase &image);
    void measure(void *pimage);
    void measurex(void *pimage);

    void edgeimage(void *pimage);
    void setedgeoi(int iw,int ioffset,int iheadtail);

    void Edge(int inum);
    void Object(int inum);
    void setfilteredge(int iw);
    void setbrow(int iborw);
    void setdistance(int idist);
    void sethsogap(int ihgap,int isgap,int iogap);
    void setminmaxarea(int imin,int imax);
    void setminmaxwh(int iminw,int imaxw,int iminh,int imaxh);
    int getresultcentx(int inum);
    int getresultcenty(int inum);
    int getresultx(int inum);
    int getresulty(int inum);
    int getresultw(int inum);
    int getresulth(int inum);
    int getresultsize(int inum);
    int getresultobjsnum();
    void setsearchtype(int itype);
    void setoffset(int ix0,int ix1,int iy0,int iy1);
    RectsShape &getresultrects(){return m_rectresults;}
    void setobjectgrid(int iw,int ih,int ixgrid);
    int getobjectgridw();
    int getobjectgridh();
    void setbackground(int iedge,int ibackgroundmethod=1);
    void objectgrid(void *pimage);
    void resultsrectfilter();
    void objectsort();
    QRect getgrid(int inum);
    QRect getgridex(int inum);

    typedef vector<TwoPointsShape> tpvect;
    typedef tpvect::iterator tpvectitor;

    tpvect m_cent_h_bw_points_v;
    tpvect m_cent_v_bw_points_v;

    tpvect m_cent_h_wb_points_v;
    tpvect m_cent_v_wb_points_v;
 //   void B2Wedge_h();
 //   void W2Bedge_h();
 //   void B2Wedge_h();
 //   void W2Bedge_h();

private:
    int m_istyle;
    ImageBase * g_pmapimage;
    ImageBase * g_pbackobjectimage;
    ImageBase * m_pgetimage;

    SPoint* m_objlistscanorA;
    SPoint* m_objlistcollectorA;

    RectsShape m_rectresults;
    RectsShape m_rectgrids;

    PointsShape m_keypoint;
    PointsShape m_fitwh;

    PointsShape m_curedge;
    PointsShape m_curobject;

    vectorint m_scanid;
    vectorint m_vrow;
    vectorint m_vborw;
    vectorint m_vobjnum;

    int m_icurobj;
    int m_iobjnum;

    int m_ifilterNedge;
    int m_iborw;

    int m_idistance;
    qint64 m_iminarea;
    qint64 m_imaxarea;
    qint64 m_iminobjw;
    qint64 m_iminobjh;
    qint64 m_imaxobjw;
    qint64 m_imaxobjh;
    qint64 m_totalarea;

    int m_ioffsetx0;
    int m_ioffsetx1;
    int m_ioffsety0;
    int m_ioffsety1;

    int m_ihgap;
    int m_isgap;
    int m_iogap;

    APOINT *m_SearchPointGroup;
    ObjectSearchType m_searchtype;

    int m_icopyw;
    int m_icopyh;
    int m_icopywgrid;

    int m_background_edge;
    int m_background_method;

    //20201222
    static int m_curfindobjectnum;
//20210222
    fastmatch * m_prelationmatch;
    int m_irelationresultnum;

    double m_drelationzoomx;
    double m_drelationzoomy;
    QRect m_irelationrect;
    int m_imagethre;
    int m_imagethreincrease;
    int m_imagecomparegap;
    int m_imagefindBorW;
    int m_imageedge_5o7;
public:
    void setrelationrectfromresultnum(int inum);
    void setrelationrectfrom_matchresult(void *pmatch);
    void setrelationrectfrom_objectresult(void *pmatch);
    void setrelationxy(int iprex1,int iprey1,int iendx1,int iendy1);
    void setrelationzoom(double drelationzoomx,double drelationzoomy);
    void setrelationtorect();
    void setcolorstyle(int istyle);


    void SetImageROIthre(int ithre);
    void SetImageROIincrease(int increase);
    void SetImageROIcomparegap(int icomparegap);
    void SetImageROIfindBorW(int ifindBorW);
    void SetImageROIedge_5o7(int i5o7);

    void ImageROIthre(void * pimage);
    void ImageROIedge(void * pimage);
    void ImageROIedgeH(void * pimage);

    void shapesetroi(void *pshape);
};












#endif
