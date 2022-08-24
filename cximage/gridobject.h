#ifndef GRIDOBJECT_H
#define GRIDOBJECT_H

#include "findobject.h"

class gridobject :public findobject
{
public:
    gridobject();
    ~gridobject();

    virtual void setshow(int ishow);
    virtual void setrect(int ix,int iy,int iw,int ih);
    virtual void drawshape(QPainter &painter,QPalette &pal);

    void Clear();
    void GridMeasure_L0(ImageBase &image);
    void GridMeasure_L1(ImageBase &image);
    void GridMeasure_L2(ImageBase &image);
    void GridMeasure_L3(ImageBase &image);

      void setshowspec(int inum);
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
    void gridmeasure_l0(void *pimage);
    void gridmeasure_l1(void *pimage);
    void gridmeasure_l2(void *pimage);
    void gridmeasure_l3(void *pimage);

    void gridopt(int i,int ix,int iy,double dx,double dy);

    void shapesetroi(void *pshape);
private:

    ImageBase * g_pbackobjectimage;
    vectorint m_izoomv;
    typedef vector<RectsShape> RectsMap;
    RectsMap m_rectsmap;
    int m_ishowspec;

};












#endif
