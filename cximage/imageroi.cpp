#include <QRect>
#include"imagetype.h"
#include "imageroi.h"
#include "fastmatch.h"
#include"backimagemanager.h"

ImageROI::ImageROI()
{

}
ImageROI::~ImageROI()
{

}
void ImageROI::setshow(int ishow)
{
    QShape::setshow(ishow);
}
void ImageROI::setrect(int ix,int iy,int iw,int ih)
{
    QShape::setrect(ix,iy,iw,ih);
}
void ImageROI::drawshape(QPainter &painter,QPalette &pal)
{
    QShape::drawshape(painter,pal);
}
void ImageROI::drawshapex(QPainter &painter,QPalette &pal,
                      double dmovx,double dmovy,
                      double dangle,
                      double dzoomx,double dzoomy)
{
    QShape::drawshapex(painter,pal,dmovx,dmovy,
                       dangle,dzoomx, dzoomy);
    if(m_rectgrids.size()>0)
    {
        m_rectgrids.setshow(2);
        m_rectgrids.drawshapex(painter,dmovx,dmovy,
                                dangle,dzoomx,dzoomy);
    }
}

void ImageROI::setrelationrectfrom_matchresult(void *pmatch)
{
    m_prelationmatch = (fastmatch*)pmatch;
    if(nullptr!=m_prelationmatch)
    {
        int inum = m_prelationmatch->getresultrects()->size();
        if(m_irelationresultnum<inum)
        {
            m_irelationrect = m_prelationmatch->getresultrects()->getrect(m_irelationresultnum);
        }
    }
}
void ImageROI::setrelationxy(int iprex1,int iprey1,int iendx1,int iendy1)
{
    m_irelationrect.setLeft(m_irelationrect.left() + iprex1);
    m_irelationrect.setTop(m_irelationrect.top() + iprey1);
    m_irelationrect.setRight(m_irelationrect.right() + iendx1);
    m_irelationrect.setBottom(m_irelationrect.bottom() + iendy1);


}
void ImageROI::setrelationpart(int igridw,int igridh,int igridx,int igridy)
{
    int igw0 = m_irelationrect.width()/igridw;
    int igh0 = m_irelationrect.height()/igridh;
    int ix0 = m_irelationrect.x() + igw0*igridx;
    int iy0 = m_irelationrect.y() + igh0*igridy;

    m_irelationrect.setLeft(ix0);
    m_irelationrect.setTop(iy0);
    m_irelationrect.setRight(ix0+igw0);
    m_irelationrect.setBottom(iy0+igh0);
}
void ImageROI::setrelationzoom(double drelationzoomx,double drelationzoomy)
{
    m_irelationrect.setLeft((double)m_irelationrect.left() * drelationzoomx);
    m_irelationrect.setTop((double)m_irelationrect.top() * drelationzoomy);
    m_irelationrect.setRight((double)m_irelationrect.right() * drelationzoomx);
    m_irelationrect.setBottom((double)m_irelationrect.bottom() * drelationzoomy);
}
void ImageROI::setrelationtorect()
{
    if(m_irelationrect.x()>=0
            &&m_irelationrect.y()>=0
            &&m_irelationrect.width()>0
            &&m_irelationrect.height()>0)
     QShape::setrect(m_irelationrect.x(),
                     m_irelationrect.y(),
                     m_irelationrect.width(),
                     m_irelationrect.height());
}
void ImageROI::getfindlineresult(void *pfindline)
{
    findline *afindl=(findline*)pfindline;
    m_points.clear();
    if(nullptr!=afindl)
    {
        m_points.addpoints(afindl->getresultpointsw());
        m_points.addpoints(afindl->getresultpointsh());
    }
}
void ImageROI::setrelationrectfrom_shape(void *pshape)
{
    QShape *ashp=(QShape*)pshape;
    if(nullptr!=ashp)
   {
      m_irelationrect = ashp->rect();
    }
}
void ImageROI::getimage(void * pimage)
{
    m_pimage = (ImageBase*)pimage;
}
int ImageROI::tablevalue(int icolor)
{
    if(nullptr!=m_pimage)
        return m_pimage->tablevalue(icolor);
    else
        return 0;
}
void ImageROI::ColorTable()
{
    if(nullptr!=m_pimage)
    {
        m_pimage->SetROI(rect().x(),rect().y(),rect().width(),rect().height());
        return m_pimage->ROIColorTable();
    }
    else
        return ;
}
void ImageROI::setrelationrectfromresultnum(int inum)
{
    m_irelationresultnum = inum;
}
void ImageROI::ColorTableBlur(int iGauss_Smoth,int ithre)
{
    if(nullptr!=m_pimage)
    {
        m_pimage->SetROI(rect().x(),rect().y(),rect().width(),rect().height());

        return m_pimage->ROIColorTableBlur(iGauss_Smoth,ithre);
    }
    else
        return ;
}
void ImageROI::ColorTableEasyThre(int ioffset)
{
    if(nullptr!=m_pimage)
    {
        m_pimage->SetROI(rect().x(),rect().y(),rect().width(),rect().height());

        return m_pimage->ROIColorTableEasyThre(0,ioffset);
    }
    else
        return ;
}
void ImageROI::roiedge(int ithre,int ireserve,int igap,int ifindBorW)
{
    if(nullptr!=m_pimage)
    {
        m_pimage->SetROI(rect().x(),rect().y(),rect().width(),rect().height());

        return m_pimage->ROIImage_5Blur_Gap_mud_thre_BW(ithre,ireserve,igap,ifindBorW);
    }
    else
        return ;
}
void ImageROI::roiedgeh(int ithre,int ireserve,int igap,int ifindBorW)
{
    if(nullptr!=m_pimage)
    {
        m_pimage->SetROI(rect().x(),rect().y(),rect().width(),rect().height());

        return m_pimage->ROIImage_5Blur_Gap_mud_thre_BW_H(ithre,ireserve,igap,ifindBorW);
    }
    else
        return ;
}
void ImageROI::roiedge7(int ithre,int ireserve,int igap,int ifindBorW)
{
    if(nullptr!=m_pimage)
    {
        m_pimage->SetROI(rect().x(),rect().y(),rect().width(),rect().height());

        return m_pimage->ROIImage_7Blur_Gap_mud_thre_BW(ithre,ireserve,igap,ifindBorW);
    }
    else
        return ;
}
void ImageROI::roiedgeh7(int ithre,int ireserve,int igap,int ifindBorW)
{
    if(nullptr!=m_pimage)
    {
        m_pimage->SetROI(rect().x(),rect().y(),rect().width(),rect().height());

        return m_pimage->ROIImage_7Blur_Gap_mud_thre_BW_H(ithre,ireserve,igap,ifindBorW);
    }
    else
        return ;
}
void ImageROI::setsplitrect(int isplitx,int isplity,int iexw,int iexh)
{
    m_rectgrids.clear();
    if(isplitx>0)
        m_isplitx = isplitx;
    if(isplity>0)
        m_isplity = isplity;

    m_iexw=iexw;
    m_iexh=iexh;

        QRectF arect = rect();
        int ix0 = arect.x()-m_iexw;
        int iy0 = arect.y()-m_iexh;
        int iw0 = arect.width()+2*m_iexw;
        int ih0 = arect.height()+2*m_iexh;
        if(ih0<=0||iw0<=0)
            return;
        int isw = iw0/m_isplitx;
        int ish = ih0/m_isplity;
        int ix1=0,iy1=0;
        for(int iy=0;iy<m_isplity;iy++)
        {
            for(int ix=0;ix<m_isplitx;ix++)
            {
                ix1 = ix0+ix*isw;
                iy1 = iy0+iy*ish;
                if(ix==m_iselectx
                 &&iy==m_iselecty)
                {
                    setrect(ix1,iy1,isw,ish);
                }
                QRect arect(ix1,iy1,isw,ish);
                m_rectgrids.addrect(arect);
            }
        }

}
void ImageROI::selectrect(int iselectx,int iselecty)
{
    m_iselectx = iselectx;
    m_iselecty = iselecty;
}
void ImageROI::findrect(void *pfindline)
{
    findline *afindl = (findline*)pfindline;
    if(nullptr!=afindl)
    {
        QRectF arect = afindl->patternboundingrect();
        int ix0 = arect.x();
        int iy0 = arect.y();
        int iw0 = arect.width();
        int ih0 = arect.height();
        if(ih0<=0||iw0<=0)
            return;
        setrect(ix0,iy0,iw0,ih0);
     }
}
void ImageROI::shapesetroi(void *pshape)
{
    QShape::shapesetroi(pshape);
}
void ImageROI::roieasythre(int iandor,int ioffset)
{
    if(nullptr!=m_pimage)
    {
        m_pimage->ROIColorTableEasyThre(iandor,ioffset);
        return ;
    }
    else
        return ;
}
void ImageROI::roithre(int ithre)
{
    if(nullptr!=m_pimage)
    {
        m_pimage->ROIImageThre(ithre);
        return ;
    }
    else
        return ;
}
