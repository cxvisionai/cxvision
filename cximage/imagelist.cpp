
#include"imagelist.h"
#include <QDir>


ImageList::ImageList()
    :m_ishow(-1)
    ,m_imagnum(30)
    ,m_icurimgnum(0)
{
    for(int i=0;i<30;i++)
    {
        ImageBase aimage;
        m_imagelist.append(aimage);

    }
}
ImageList::~ImageList()
{
    int isize=m_imagelist.size();


}
void ImageList::setimagelist(int inum)
{
    m_imagelist.clear();
    for(int i=0;i<inum;i++)
    {
        ImageBase aimage;
        m_imagelist.append(aimage);

    }
}

ImageBase * ImageList::getimage(int inum)
{
    int isize = m_imagelist.size();
    if(inum>=0&&inum<isize)
    {
        return &m_imagelist[inum];
    }

    return nullptr;
}
int ImageList::nextcurimgenum()
{
    int inum= m_icurimgnum;
       int isize = m_imagelist.size();
      if(inum>=0&&inum<isize-1)
            m_icurimgnum=m_icurimgnum+1;
      else
          m_icurimgnum =0;
    return inum;
}

ImageBase * ImageList::nextimage()
{

        return &m_imagelist[nextcurimgenum()];
}


void mkdir(const QString &astr)
{
    const QDir cxDir(astr);
    if (!cxDir.exists())
    {
       if (!cxDir.mkpath(astr))
       {
          return ;
       }
    }
}

void ImageList::save()
{
#if defined( Q_OS_WIN )
     int isize = m_imagelist.size();
        mkdir(QString("C://CxVision"));
        mkdir(QString("C://CxVision//img"));
        for(int i=0;i<isize;i++)
        {
            QString strname =  QString("C://CxVision//img//%1.bmp").arg(i);
            m_imagelist[i].save(strname);
        }
 #else
    int isize = m_imagelist.size();
       mkdir(QString("/media/cxy/Q/img"));
       for(int i=0;i<isize;i++)
       {
           QString strname =  QString("/media/cxy/Q/img/%1.bmp").arg(i);
           m_imagelist[i].save(strname);
       }
 #endif
}
void ImageList::load()
{
#if defined( Q_OS_WIN )
     int isize = m_imagelist.size();
        for(int i=0;i<isize;i++)
        {
            QString strname =  QString("C://CxVision//img//%1.bmp").arg(i);
            m_imagelist[i].load(strname);
        }
#else
    int isize = m_imagelist.size();
       for(int i=0;i<isize;i++)
       {
           QString strname =  QString("/media/cxy/Q/img/%1.bmp").arg(i);
           m_imagelist[i].load(strname);
       }
#endif
}
void ImageList::settoimage(void *pimage)
{
    ImageBase *paimage=(ImageBase *)pimage;
    ImageBase *pbimage=getimage(m_icurimgnum);
    if(nullptr!=pbimage&&nullptr!=paimage)
    {
        paimage->SetMode(0);
        paimage->ImageAnd((void *)pbimage);
    }
}
void ImageList::clearimage()
{
    int isize=m_imagelist.size();
    for(int i=0;i<isize;i++)
    {
        m_imagelist[i].ImageClear();
    }
}
