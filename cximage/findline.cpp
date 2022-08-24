#include "findline.h"
#include "findobject.h"

#include "backimagemanager.h"

int findline::m_curfindlinenum = 0;
findline::findline():QShape(),
    m_ihgap(2),
    m_iwgap(2),
    m_iSelectPointGap(3),
    m_iMethod(1),
    m_iThreshold(8),
    m_igamarate(0),
    m_dsamplerate(0.004),
    m_ifindset(1),
    m_ifilterborw(21),
    m_ifiltermin(50),
    m_ifiltermax(100000),
    m_iselectedgenum(0),//any
    m_ineedfixs(2),
    m_icomparegap(2),
    m_ishowlines(1)
{
    QString strname = QString("fline%1").arg(m_curfindlinenum);
    setname(strname.toStdString().c_str());
    m_curfindlinenum = m_curfindlinenum + 1;

    int icurmodule = BackImageManager::GetCurMode();
    g_pbackimage = BackImageManager::GetBackImage(icurmodule);
    g_pbackfindobject = BackImageManager::Getbackfindobject(icurmodule);

}
findline::~findline()
{

}
void findline::setcomparegap(int igap)
{
    m_icomparegap = igap;
}
void findline::setshow(int ishow)
{
    if(ishow&0x02)
    {
        m_measurepoints_w.setshow(2);
        m_measurepoints_h.setshow(2);
    }
    if(1==ishow)
    {
        m_measurepoints_w.setshow(1);
        m_measurepoints_h.setshow(1);

    }
    QShape::setshow(ishow);

}
void findline::setselectedgenum(int iedgenum)
{
    m_iselectedgenum = iedgenum;
}
void findline::clear()
{
    m_lines_w.clear();
    m_lines_h.clear();

}
void findline::SetWHgap(int wgap,int hgap)
{
    m_iwgap = wgap;
    m_ihgap = hgap;
    m_lines_w.clear();
    m_lines_h.clear();
    int iplinewsize = m_iwgap>0?(rect().width()/m_iwgap):0 ;
    int iplinehsize = m_ihgap>0?(rect().height()/m_ihgap):0 ;
    LineShape aline1,aline2;
    for(int i=0;i<iplinewsize;i++)
    {
        aline1.copy(m_LineB);
        m_lines_w.push_back(aline1);
        m_lines_w[i].Move(m_iwgap*i,0);
        m_lines_w[i].setPercent(m_dsamplerate);
    }
    for(int i=0;i<iplinehsize;i++)
    {
        aline2.copy(m_LineA);
        m_lines_h.push_back(aline2);
        m_lines_h[i].Move(0,m_ihgap*i);
        m_lines_h[i].setPercent(m_dsamplerate);
    }

}
void findline::setrect(int ix, int iy, int iw, int ih)
{
    m_LineA.setline(ix,iy,ix+iw,iy);
    m_LineB.setline(ix,iy,ix,iy+ih);
    m_lines_w.clear();
    m_lines_h.clear();
    int iplinewsize = m_iwgap>0?(iw/m_iwgap):0 ;
    int iplinehsize = m_ihgap>0?(ih/m_ihgap):0 ;
    LineShape aline1,aline2;
    for(int i=0;i<iplinewsize;i++)
    {
        aline1.copy(m_LineB);
        m_lines_w.push_back(aline1);
        m_lines_w[i].Move(m_iwgap*i,0);
        m_lines_w[i].setPercent(m_dsamplerate);
    }
    for(int i=0;i<iplinehsize;i++)
    {
        aline2.copy(m_LineA);
        m_lines_h.push_back(aline2);
        m_lines_h[i].Move(0,m_ihgap*i);
        m_lines_h[i].setPercent(m_dsamplerate);
    }

    QShape::setrect(ix,iy,iw,ih);
}
void findline::drawpattern(QPainter &painter)
{

    m_modelpoints.setshow(8);
    m_modelpoints.drawshape(painter);
    m_measurepointsA.drawshape(painter);
    m_measurepointsB.drawshape(painter);
    m_measurepointsA_.drawshape(painter);
    m_measurepointsB_.drawshape(painter);

}
void findline::drawpatternx(QPainter &painter,
                            double dmovx,double dmovy,
                            double dangle,
                            double dzoomx,double dzoomy)
{
    m_modelpoints.setshow(8);
    m_modelpoints.drawshapex(painter,dmovx,dmovy,
                            dangle,dzoomx, dzoomy);
    m_measurepointsA.drawshapex(painter,dmovx,dmovy,
                               dangle,dzoomx, dzoomy);
    m_measurepointsB.drawshapex(painter,dmovx,dmovy,
                               dangle,dzoomx, dzoomy);
    m_measurepointsA_.drawshapex(painter,dmovx,dmovy,
                                dangle,dzoomx, dzoomy);
    m_measurepointsB_.drawshapex(painter,dmovx,dmovy,
                                dangle,dzoomx, dzoomy);

}


void findline::edgepattern(ImageBase &image)
{
    m_measurepointsA.clear();
    m_measurepointsB.clear();
    m_measurepointsA_.clear();
    m_measurepointsB_.clear();
    m_modelpoints.clear();

    SetWHgap(wgap(),hgap());

    setselectedgenum(0);
    setmethod(0);
    Measure(image);
    m_measurepointsA.addpoints(getresultpointsw());
    m_measurepointsB.addpoints(getresultpointsh());

    m_measurepointsA.doublepattern(m_icomparegap,12,m_modelpoints);
    m_measurepointsB.doublepattern(m_icomparegap,9,m_modelpoints);

    setselectedgenum(0);
    setmethod(1);
    Measure(image);
    m_measurepointsA_.addpoints(getresultpointsw());
    m_measurepointsB_.addpoints(getresultpointsh());

    m_measurepointsA_.doublepattern(m_icomparegap,6,m_modelpoints);
    m_measurepointsB_.doublepattern(m_icomparegap,3,m_modelpoints);

}
void findline::patternzeroposition()
{
    QRectF arect1 = m_modelpoints.boundingRect();
    m_modelpoints.Move(-arect1.x(),-arect1.y());
}
void findline::savepatternfile(const char * pchar)
{
    m_modelpoints.save(pchar);
}
void findline::loadpatternfile(const char * pchar)
{
    m_modelpoints.load(pchar);
}
QRectF findline::patternboundingrect()
{
    return m_modelpoints.boundingRect();
}
void findline::patternrootgrid(double itype,double drate,double ilevel)
{
    m_modelpoints.keysrootgrid(itype,drate,ilevel);
}
void findline::patterntranform(int igap,int itype,int isgap,int iline)
{
     m_modelpoints.patterntranform(igap,itype,isgap, iline);
}
void findline::patternzoom(double dx,double dy,double igap,double itype)
{
     m_modelpoints.patternzoom(dx,dy,igap,itype);
}
void findline::patternrotate(double dangle)
{
     m_modelpoints.Rotate(dangle);
}
void findline::modelzoom(double dx,double dy)
{
     m_modelpoints.Zoom(dx,dy);
}
QPainterPath &findline::getpatternpath()
{
    return m_modelpoints.getpath();
}
PointsShape &findline::getpattern()
{
     return m_modelpoints;
}
void findline::findpattern(void *pimage)
{
    ImageBase *pgetimage = (ImageBase*)pimage;
    edgepattern(*pgetimage);
}
void findline::drawshape(QPainter &painter,QPalette &pal)
{
    if(show()&0x04)
    {
        m_LineA.setPen(255,0,0);
        m_LineB.setPen(255,0,0);
        m_LineA.drawshape(painter);
        m_LineB.drawshape(painter);
        painter.setPen(QPen(QColor(0,255,0)));
        for(int i=0;i<m_lines_w.size();i++)
        {
            m_lines_w[i].drawshape(painter);
        }
        for(int i=0;i<m_lines_h.size();i++)
        {
            m_lines_h[i].drawshape(painter);
        }
    }
    if(show()&0x02)
    {
        m_measurepoints_w.drawshape(painter);
        m_measurepoints_h.drawshape(painter);
    }
    if(show()&0x08)
    {
        drawpattern(painter);
    }

    QShape::drawshape(painter,pal);
}
void findline::drawshapex(QPainter &painter,QPalette &pal,
                      double dmovx,double dmovy,
                      double dangle,
                      double dzoomx,double dzoomy)
{
    if(show()&0x04)
    {
        m_LineA.setPen(255,0,0);
        m_LineB.setPen(255,0,0);
        m_LineA.drawshape(painter);
        m_LineB.drawshape(painter);
        painter.setPen(QPen(QColor(0,255,0)));
        for(int i=0;i<m_lines_w.size();i++)
        {
            m_lines_w[i].drawshape(painter);
        }
        for(int i=0;i<m_lines_h.size();i++)
        {
            m_lines_h[i].drawshape(painter);
        }
    }
    if(show()&0x02)
    {
        m_measurepoints_w.drawshapex(painter,dmovx,dmovy,
                                     dangle,dzoomx, dzoomy);
        m_measurepoints_h.drawshapex(painter,dmovx,dmovy,
                                    dangle,dzoomx, dzoomy);
    }
    if(show()&0x08)
    {
        drawpatternx(painter,dmovx,dmovy,
                    dangle,dzoomx, dzoomy);
    }

   QShape::drawshapex(painter,pal,dmovx,dmovy,
                             dangle,dzoomx, dzoomy);
}

void findline::setlinesamplerate(double dsamplerate)
{
    m_dsamplerate=dsamplerate;
}
void findline::setlinegap(int igap)
{
    m_iSelectPointGap = igap;
}
void findline::setmethod(int imethod)
{
    m_iMethod = imethod;
}
void findline::setthre(int ithre)
{
    m_iThreshold = ithre;
}
int findline::thre()
{
    return m_iThreshold;
}
void findline::setgamarate(int igama)
{
    m_igamarate = igama;
}

void findline::setfindsetting(int ifindset)
{
    m_ifindset = ifindset;
}
void findline::setfilter(int ifilterborw,int ifiltermin,int ifiltermax)
{
    m_ifilterborw = ifilterborw;
    m_ifiltermin = ifiltermin;
    m_ifiltermax = ifiltermax;
}

void findline::Measure(ImageBase &image)
{

    if(image.width()< rect().x() + rect().width()
       || image.height() < rect().y() + rect().height())
        return;//error process
if(rect().x()<0||rect().y()<0)
     return;//error process
    m_measurepoints_w.clear();
    m_measurepoints_h.clear();
    int iwsize = m_lines_w.size();
    int ihsize = m_lines_h.size();
    for(int i=0;i<iwsize;i++)
    {
        m_lines_w[i].linecopyex(image,*g_pbackimage,0,i);
    }
    for(int i=0;i<ihsize;i++)
    {
        m_lines_h[i].linecopyex(image,*g_pbackimage,0,i+iwsize);
    }
    int ilineslen1 = 0;
    int ilineslen2 = 0;

    if(iwsize>0)
        ilineslen1 = m_lines_w[0].getlinesize();
    if(ihsize>0)
        ilineslen2 = m_lines_h[0].getlinesize();

    int iprocessw = ilineslen1>ilineslen2?ilineslen1:ilineslen2;

    g_pbackimage->SetROI(0,0,iprocessw,iwsize+ihsize);

    g_pbackimage->ROIImage_7Blur_Gap_mud_thre_BW(m_iThreshold,m_igamarate,m_iSelectPointGap,m_iMethod);

    if(m_ifindset&0x01)
    {
        g_pbackfindobject->setrect(0,0,iprocessw,iwsize+ihsize);
        g_pbackfindobject->setbrow(m_ifilterborw);//21 22
        g_pbackfindobject->setminmaxarea(m_ifiltermin,m_ifiltermax);
        g_pbackfindobject->Measure(*g_pbackimage);

    }


    int irecordpoint[100];
    int irecordnum =0;
    bool bcollectBegin=false;




    int icurlinenum = 0;
    int icurlineposition = 0;

    int ifixvalue = 3;

    unsigned int icolor = 0;
    for(int inumy=0+ifixvalue ;inumy<iwsize -ifixvalue;inumy++)
    {
        irecordnum =0;
        icurlinenum = 0;
        bcollectBegin=false;
        for(int inumx=0;inumx<ilineslen1;inumx++)
        {
            icolor = g_pbackimage->pixel(inumx,inumy);
            if((icolor&0x0FFFF) >0)
            {
                if(irecordnum<100)
                {
                    irecordpoint[irecordnum] = inumx;
                    irecordnum++;
                }
                else
                {
                    irecordnum =0;
                    break;
                }
                bcollectBegin=true;
            }
            else
            {
                if(true==bcollectBegin
                   &&irecordnum>0
                   &&irecordnum<=70)
                {
                        icurlineposition = m_ineedfixs+irecordpoint[(irecordnum>>1)];
                        //icurlineposition = icurlineposition>ilineslen1?ilineslen1-1:icurlineposition;

                        icurlinenum++;
                        if(icurlinenum==m_iselectedgenum
                            ||m_iselectedgenum==0)//0 any
                        {
                            if(icurlineposition<(ilineslen1-m_iSelectPointGap-3)//2021 before 1
                               &&icurlineposition>m_iSelectPointGap+3)//2021 before 1
                            {
                                QPointF apoint = m_lines_w[inumy].getlinepoint(icurlineposition);
                                m_measurepoints_w.addpoint(apoint);
                                if(icurlinenum==m_iselectedgenum)
                                    break;
                            }
                        }
                }
                irecordnum =0;
                bcollectBegin=false;
            }
        }
        if(true==bcollectBegin
           &&irecordnum>0)
        {
                icurlineposition = m_ineedfixs+irecordpoint[(irecordnum>>1)];
                //icurlineposition = icurlineposition>ilineslen1?ilineslen1-1:icurlineposition;
                icurlinenum++;
                if(icurlinenum==m_iselectedgenum
                    ||m_iselectedgenum==0)
                {
                    if(icurlineposition<(ilineslen1-m_iSelectPointGap-3)//2021 before 1
                            &&icurlineposition>m_iSelectPointGap+3)//2021 before 1
                    {
                        QPointF apoint = m_lines_w[inumy].getlinepoint(icurlineposition);
                        m_measurepoints_w.addpoint(apoint);
                        if(icurlinenum==m_iselectedgenum)
                            break;
                    }
                }
            irecordnum =0;
            bcollectBegin=false;
        }

    }

    bcollectBegin=false;
    for(int inumy=iwsize+ifixvalue ;inumy<iwsize+ihsize -ifixvalue;inumy++)
    {
        irecordnum =0;
        icurlinenum = 0;
        bcollectBegin=false;
        for(int inumx=0;inumx<ilineslen2;inumx++)
        {
            icolor = g_pbackimage->pixel(inumx,inumy);
            if((icolor&0x0FFFF) >0)
            {
                if(irecordnum<100)
                {
                    irecordpoint[irecordnum] = inumx;
                    irecordnum++;
                }
                else
                {
                    irecordnum =0;
                    break;
                }
                bcollectBegin=true;
            }
            else
            {
                if(true==bcollectBegin
                   &&irecordnum>0
                   &&irecordnum<=70)
                {
                        icurlineposition = m_ineedfixs + irecordpoint[(irecordnum>>1)];
                        //icurlineposition = icurlineposition>ilineslen2?ilineslen2-1:icurlineposition;
                        icurlinenum++;
                        if(icurlinenum==m_iselectedgenum
                            ||m_iselectedgenum==0)
                        {
                            if(icurlineposition<(ilineslen2 -m_iSelectPointGap-3)//2021 before 1
                                    &&icurlineposition>m_iSelectPointGap+3)//2021 before 1
                            {
                                QPointF apoint = m_lines_h[inumy-iwsize].getlinepoint(icurlineposition);
                                m_measurepoints_h.addpoint(apoint);
                                if(icurlinenum==m_iselectedgenum)
                                    break;
                            }
                        }
                 }
                irecordnum =0;
                bcollectBegin=false;
            }
        }
        if(true==bcollectBegin
           &&irecordnum>0)
        {
               icurlineposition = m_ineedfixs + irecordpoint[(irecordnum>>1)];
              // icurlineposition = icurlineposition>ilineslen2?ilineslen2-1:icurlineposition;

                icurlinenum++;
                if(icurlinenum==m_iselectedgenum
                    ||m_iselectedgenum==0)
                {
                    if(icurlineposition<(ilineslen2-m_iSelectPointGap-3)//2021 before 1
                            &&icurlineposition>m_iSelectPointGap+3)//2021 before 1
                    {
                        QPointF apoint = m_lines_h[inumy-iwsize].getlinepoint(icurlineposition);
                        m_measurepoints_h.addpoint(apoint);
                        if(icurlinenum==m_iselectedgenum)
                            break;
                    }
                }
            irecordnum =0;
            bcollectBegin=false;
        }
    }
}

PointsShape &findline::getresultpointsw()
{
    return m_measurepoints_w;
}
PointsShape &findline::getresultpointsh()
{
    return m_measurepoints_h;
}

void findline::measure(void *pimage)
{
    ImageBase *pgetimage = (ImageBase*)pimage;
    Measure(*pgetimage);
}
void findline::shapesetroi(void *pshape)
{
    QShape::shapesetroi(pshape);
}
