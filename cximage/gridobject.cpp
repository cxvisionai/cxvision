
#include <QRect>

#include "gridobject.h"
#include "backimagemanager.h"


gridobject::gridobject():findobject(),
    m_ishowspec(-1)
{
    setname("fgrid");
    setcolor(0,255,120);
    int icurmodule = BackImageManager::GetCurMode();
    g_pbackobjectimage = BackImageManager::GetBackObjectImage(icurmodule);

}
gridobject::~gridobject()
{

}
void gridobject::setbrow(int iborw)
{
    findobject::setbrow(iborw);
}
void gridobject::setshow(int ishow)
{
    QShape::setshow(ishow);
}
void gridobject::setshowspec(int ispec)
{
    m_ishowspec = ispec;
}
void gridobject::setrect(int ix, int iy, int iw, int ih)
{
    QShape::setrect(ix,iy,iw,ih);
}
void gridobject::drawshape(QPainter &painter,QPalette &pal)
{
            if(show()&0x02)
            {
                //m_rectresults.drawshape(painter);
                int isize = m_rectsmap.size();
                for(int i=0;i<isize;i++)
                {
                    if(i<16)
                    m_rectsmap[i].setPen(g_color[i]);
                    if(i==m_ishowspec||m_ishowspec==-1)
                    m_rectsmap[i].drawshape(painter);
                }
            }
            if(show()&0x04)
            findobject::drawshape(painter,pal);
}
int gridobject::getresultcentx(int inum)
{
    return findobject::getresultcentx(inum);
}
int gridobject::getresultcenty(int inum)
{
    return findobject::getresultcenty(inum);
}
int gridobject::getresultx(int inum)
{
    return findobject::getresultx(inum);
}
int gridobject::getresulty(int inum)
{
    return findobject::getresulty(inum);
}
int gridobject::getresultw(int inum)
{
    return findobject::getresultw(inum);
}
int gridobject::getresulth(int inum)
{
    return findobject::getresulth(inum);
}
int gridobject::getresultsize(int inum)
{
    return findobject::getresultsize(inum);
}
int gridobject::getresultobjsnum()
{
    return findobject::getresultobjsnum();
}
void gridobject::setdistance(int idist)
{
    findobject::setdistance(idist);
}
void gridobject::gridmeasure_l0(void *pimage)
{
    ImageBase *pgetimage = (ImageBase*)pimage;
    GridMeasure_L0(*pgetimage);
}
void gridobject::gridopt(int i,int ix,int iy,double dx,double dy)
{
    int isize = m_rectsmap.size();
    if(isize <=i)
        return;
   // for(int i=0;i<isize;i++)
   // {
        m_rectsmap[i].Zoom(dx,dy);
        m_rectsmap[i].Move(ix,iy);
   // }
}
void gridobject::Clear()
{
    m_rectsmap.clear();
}
void gridobject::GridMeasure_L0(ImageBase &image)
{
   // m_rectsmap.clear();
    //g_pbackobjectimage->SetMode(0);
    //g_pbackobjectimage->ImageAnd((void *)&image);
    //g_pbackobjectimage->ImageZoomA(0,0,0.25,0.25);

    //g_pbackobjectimage->ImageZoomAnd(&image,0,0,0.25,0.25);

    int iw = image.width();
    int ih = image.height();

   // setrect(0,0,iw*0.25,ih*0.25);

  //  Measure(*g_pbackobjectimage);

  //  m_rectsmap.push_back(getresultrects());

    //gridopt(0,iw*0.25,ih*0.25,2,2);
    //level 2
    //g_pbackobjectimage->ImageZoomAnd(&image,iw*0.25,ih*0.25,0.5,0.5);
   // g_pbackobjectimage->ImageZoomAnd(&image,0,0,0.5,0.5);

   // setrect(0,0,iw*0.5,ih*0.5);

   // Measure(*g_pbackobjectimage);

  //  m_rectsmap.push_back(getresultrects());
  //  gridopt(0,0,0,4,4);
  //  gridopt(1,0,0,2,2);

   // setrect(0,0,iw,ih);
    Measure(image);

    m_rectsmap.push_back(getresultrects());

}
void gridobject::GridMeasure_L1(ImageBase &image)
{
    m_rectsmap.clear();
    //g_pbackobjectimage->SetMode(0);
    //g_pbackobjectimage->ImageAnd((void *)&image);
    //g_pbackobjectimage->ImageZoomA(0,0,0.25,0.25);

    g_pbackobjectimage->ImageZoomAnd(&image,0,0,0.25,0.25);

    int iw = image.width();
    int ih = image.height();

    setrect(0,0,iw*0.25,ih*0.25);

    Measure(*g_pbackobjectimage);

    m_rectsmap.push_back(getresultrects());

    //gridopt(0,iw*0.25,ih*0.25,2,2);
    //level 2
    //g_pbackobjectimage->ImageZoomAnd(&image,iw*0.25,ih*0.25,0.5,0.5);
    g_pbackobjectimage->ImageZoomAnd(&image,0,0,0.5,0.5);
    //setrect(iw*0.25,ih*0.25,iw*0.5,ih*0.5);
    setrect(0,0,iw*0.5,ih*0.5);

    Measure(*g_pbackobjectimage);

    m_rectsmap.push_back(getresultrects());
    gridopt(0,0,0,4,4);
    gridopt(1,0,0,2,2);

    setrect(0,0,iw,ih);
    Measure(image);

    m_rectsmap.push_back(getresultrects());

}
void gridobject::GridMeasure_L2(ImageBase &image)
{
    m_rectsmap.clear();
    //g_pbackobjectimage->SetMode(0);
    //g_pbackobjectimage->ImageAnd((void *)&image);
    //g_pbackobjectimage->ImageZoomA(0,0,0.25,0.25);

    g_pbackobjectimage->ImageZoomAnd(&image,0,0,0.25,0.25);

    int iw = image.width();
    int ih = image.height();

    setrect(0,0,iw*0.25,ih*0.25);

    Measure(*g_pbackobjectimage);

    m_rectsmap.push_back(getresultrects());

    //gridopt(0,iw*0.25,ih*0.25,2,2);
    //level 2
    //g_pbackobjectimage->ImageZoomAnd(&image,iw*0.25,ih*0.25,0.5,0.5);
    g_pbackobjectimage->ImageZoomAnd(&image,0,0,0.5,0.5);
    //setrect(iw*0.25,ih*0.25,iw*0.5,ih*0.5);
    setrect(0,0,iw*0.5,ih*0.5);

    Measure(*g_pbackobjectimage);

    m_rectsmap.push_back(getresultrects());
    gridopt(0,0,0,4,4);
    gridopt(1,0,0,2,2);

    setrect(0,0,iw,ih);
    Measure(image);

    m_rectsmap.push_back(getresultrects());

}
void gridobject::GridMeasure_L3(ImageBase &image)
{
    m_rectsmap.clear();
    //g_pbackobjectimage->SetMode(0);
    //g_pbackobjectimage->ImageAnd((void *)&image);
    //g_pbackobjectimage->ImageZoomA(0,0,0.25,0.25);

    g_pbackobjectimage->ImageZoomAnd(&image,0,0,0.25,0.25);

    int iw = image.width();
    int ih = image.height();

    setrect(0,0,iw*0.25,ih*0.25);

    Measure(*g_pbackobjectimage);

    m_rectsmap.push_back(getresultrects());

    //gridopt(0,iw*0.25,ih*0.25,2,2);
    //level 2
    //g_pbackobjectimage->ImageZoomAnd(&image,iw*0.25,ih*0.25,0.5,0.5);
    g_pbackobjectimage->ImageZoomAnd(&image,0,0,0.5,0.5);
    //setrect(iw*0.25,ih*0.25,iw*0.5,ih*0.5);
    setrect(0,0,iw*0.5,ih*0.5);

    Measure(*g_pbackobjectimage);

    m_rectsmap.push_back(getresultrects());
    gridopt(0,0,0,4,4);
    gridopt(1,0,0,2,2);

    setrect(0,0,iw,ih);
    Measure(image);

    m_rectsmap.push_back(getresultrects());

}

void gridobject::setsearchtype(int itype)
{
    findobject::setsearchtype(itype);
}
void gridobject::sethsogap(int ihgap,int isgap,int iogap)
{
    findobject::sethsogap(ihgap,isgap,iogap);
}
void gridobject::setminmaxarea(int imin,int imax)
{
    findobject::setminmaxarea(imin,imax);
}
void gridobject::setminmaxwh(int iminw,int imaxw,int iminh,int imaxh)
{
    findobject::setminmaxwh(iminw,imaxw,iminh,imaxh);
}
void gridobject::shapesetroi(void *pshape)
{
    QShape::shapesetroi(pshape);
}
