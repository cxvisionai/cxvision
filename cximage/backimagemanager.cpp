#include "backimagemanager.h"
#include "findobject.h"

ImageBase* BackImageManager::m_pBackImage = 0;
ImageBase* BackImageManager::m_pBackObjectImage = 0;
ImageBase* BackImageManager::m_pMapImage = 0;
ImageBase *BackImageManager::m_pModelImage =0;
ImageBase *BackImageManager::m_pTransferImage =0;

findobject *BackImageManager::m_pfindobject =0;


int BackImageManager::m_imodulid = 0;

int BackImageManager::m_curmodul = 0;

SPoint * BackImageManager::_thelistscanorA = 0;

SPoint * BackImageManager::_thelistcollectorA = 0;

qint64 * BackImageManager::g_r_table = 0;
qint64 * BackImageManager::g_g_table = 0;
qint64 * BackImageManager::g_b_table = 0;

int BackImageManager::g_r_table_thre = 0;
int BackImageManager::g_g_table_thre = 0;
int BackImageManager::g_b_table_thre = 0;

int BackImageManager::g_table_rate = 0;

QRootGrid *BackImageManager::m_rootgridA = 0;
QRootGrid *BackImageManager::m_rootgridA_ = 0;
QRootGrid *BackImageManager::m_rootgridB = 0;
QRootGrid *BackImageManager::m_rootgridB_ = 0;

BackImageManager::~BackImageManager()
{
    if(NULL!=_thelistcollectorA)
    {
        delete []_thelistcollectorA;
        _thelistcollectorA = NULL;
    }
    if(NULL!=_thelistscanorA)
    {
        delete []_thelistscanorA;
        _thelistscanorA = NULL;
    }
    m_imodulid = 0;

    m_curmodul = 0;

}
void BackImageManager::setobjectshow(int ishow)
{
    m_iobjectshow = ishow;
    if(0!=m_pfindobject)
        m_pfindobject->setshow(m_iobjectshow);
}
void BackImageManager::draw(QPainter &painter)
{
    if(0x02==m_ishow)
    {
        if(0!=m_pBackImage)
        {
            m_pBackImage->draw(painter);
        }
    }
    else if(0x04==m_ishow)
    {
        if(0!=m_pMapImage)
        {
            m_pMapImage->draw(painter);
        }
    }
    else if(0x08==m_ishow)
    {
        if(0!=m_pModelImage)
        {
            m_pModelImage->draw(painter);
        }
    }
    else if(0x10==m_ishow)
    {
        if(0!=m_pBackObjectImage)
        {
            m_pBackObjectImage->draw(painter);
        }
    }
    else if(0x20==m_ishow)
    {
        drawtable(painter);
    }
    else if(0x40==m_ishow)
    {
        QPalette apalette;
        m_pfindobject->drawshape(painter,apalette);
    }
}
void BackImageManager::drawtable(QPainter &painter)
{
    if(g_r_table==0
        ||g_g_table==0
        ||g_b_table==0)
        return;
    int ibx = 0;
    int iby = 0;

    int irate = 1;
    if(g_table_rate>30000)
        irate = 10;
    else if(g_table_rate>10000)
        irate = 5;

    QPainterPath pathr;
    pathr.moveTo(0, 300);
    for(int i=0;i<=255;i++)
    {
        pathr.lineTo(i, 300-(g_r_table[i]/irate));
    }

    QPainterPath pathg;
    pathg.moveTo(0, 300);
    for(int i=0;i<=255;i++)
    {
        pathg.lineTo(i, 300-(g_g_table[i]/irate));
    }

    QPainterPath pathb;
    pathb.moveTo(0, 300);
    for(int i=0;i<=255;i++)
    {
        pathb.lineTo(i, 300-(g_b_table[i]/irate));
    }

painter.save();
    painter.setPen(Qt::red);
    painter.drawRect(ibx+0,iby+0,256,300);
    painter.drawPath(pathr);

    painter.drawLine(ibx+g_r_table_thre,iby+0,ibx+g_r_table_thre,iby+300);
painter.restore();


painter.save();
painter.translate(256, 0);
    painter.setPen(Qt::green);
    painter.drawRect(ibx+0,iby+0,256,300);
    painter.drawPath(pathg);
    painter.drawLine(ibx+g_g_table_thre,iby+0,ibx+g_g_table_thre,iby+300);

painter.restore();

painter.save();
painter.translate(512, 0);
    painter.setPen(Qt::blue);
    painter.drawRect(ibx+0,iby+0,256,300);
    painter.drawPath(pathb);
    painter.drawLine(ibx+g_b_table_thre,iby+0,ibx+g_b_table_thre,iby+300);

painter.restore();
}
void BackImageManager::setshow(int ishow)
{
    m_ishow = ishow;
}

int BackImageManager::show()
{
    return m_ishow;
}

void BackImageManager::CurMode()
{
    m_imodulid = m_imodulid + 1;
}

int BackImageManager::GetCurMode()
{
    return m_imodulid;
}

ImageBase* BackImageManager::GetBackImage(int curmodul)
{
    if(1==curmodul)
    {
        if(0==m_pBackImage)
        {
            CreateBackImage(BACKIMAGEWITH,BACKIMAGEHIGH);
        }
        return m_pBackImage;
    }
    else if(2==curmodul)
    {

    }
}
void BackImageManager::CreateBackImage(int iw,int ih)
{
    if(0!=m_pBackImage)
        return;
    ImageBase &backimage = BackImage();
    m_pBackImage = &backimage;
    ImageBase aimage(QSize(iw,ih),QImage::Format_ARGB32_Premultiplied);
    *m_pBackImage = aimage;
}
ImageBase& BackImageManager::BackImage()
{
    static ImageBase _thebackimage;
    return _thebackimage;
}


ImageBase* BackImageManager::GetBackObjectImage(int curmodul)
{
    if(1==curmodul)
    {
        if(0==m_pBackObjectImage)
        {
            CreateBackObjectImage(BACKIMAGEWITH,BACKIMAGEHIGH);
        }
        return m_pBackObjectImage;
    }
    else if(2==curmodul)
    {

    }
    return 0;
}
void BackImageManager::CreateBackObjectImage(int iw,int ih)
{
    if(0!=m_pBackObjectImage)
        return;
    ImageBase &backobjectimage = BackObjectImage();
    m_pBackObjectImage = &backobjectimage;
    ImageBase aimage(QSize(iw,ih),QImage::Format_ARGB32_Premultiplied);
    *m_pBackObjectImage = aimage;
}
ImageBase& BackImageManager::BackObjectImage()
{
    static ImageBase _thebackobjectimage;
    return _thebackobjectimage;
}


ImageBase* BackImageManager::GetMapImage(int curmodul)
{
    if(1==curmodul)
    {
        if(0==m_pMapImage)
        {
            CreateMapImage(BACKIMAGEWITH,BACKIMAGEHIGH);
        }
        return m_pMapImage;
    }
    else if(2==curmodul)
    {

    }
    return 0;
}
void BackImageManager::CreateMapImage(int iw,int ih)
{
    if(0!=m_pMapImage)
        return;
    ImageBase &mapimage = MapImage();
    m_pMapImage = &mapimage;
    ImageBase aimage(QSize(iw,ih),QImage::Format_ARGB32_Premultiplied);
    *m_pMapImage = aimage;
}
ImageBase& BackImageManager::MapImage()
{
    static ImageBase _themapimage;
    return _themapimage;

}

ImageBase* BackImageManager::GetModelImage(int curmodul)
{
    if(1==curmodul)
    {
        if(0==m_pModelImage)
        {
            CreateModelImage(BACKIMAGEWITH,BACKIMAGEHIGH);
        }
        return m_pModelImage;
    }
    else if(2==curmodul)
    {

    }
    return 0;
}
void BackImageManager::CreateModelImage(int iw,int ih)
{
    if(0!=m_pModelImage)
        return;
    ImageBase &modelimage = ModelImage();
    m_pModelImage = &modelimage;
    ImageBase aimage(QSize(iw,ih),QImage::Format_ARGB32_Premultiplied);
    *m_pModelImage = aimage;
}
ImageBase& BackImageManager::ModelImage()
{
    static ImageBase _themodelimage;
    return _themodelimage;

}
ImageBase* BackImageManager::GetTransferImage(int curmodul)
{
    if(1==curmodul)
    {
        if(0==m_pTransferImage)
        {
            CreateTransferImage(BACKIMAGEWITH,BACKIMAGEHIGH);
        }
        return m_pTransferImage;
    }
    else if(2==curmodul)
    {

    }
    return 0;
}
void BackImageManager::CreateTransferImage(int iw,int ih)
{
    if(0!=m_pTransferImage)
        return;
    ImageBase &transferimage = TransferImage();
    m_pTransferImage = &transferimage;
    ImageBase aimage(QSize(iw,ih),QImage::Format_ARGB32_Premultiplied);
    *m_pTransferImage = aimage;
}
ImageBase& BackImageManager::TransferImage()
{
    static ImageBase _thetransferimage;
    return _thetransferimage;

}
SPoint * BackImageManager::GetListScan(int curmodul)
{
    switch(curmodul)
    {
        default:
        case 1:
             if(NULL==_thelistscanorA)
             {
                _thelistscanorA = new SPoint[OBJSCANNNUM];
             }
            return _thelistscanorA;
            break;
    }
}
SPoint * BackImageManager::GetListCollect(int curmodul)
{
    switch(curmodul)
    {
    default:
    case 1:
        if(NULL==_thelistcollectorA)
        {
            _thelistcollectorA = new SPoint[OBJCOLLECTIONNUM];
        }
        return _thelistcollectorA;
        break;
    }
}
findobject* BackImageManager::Getbackfindobject(int curmodul)
{
    if(1==curmodul)
    {
        if(0==m_pfindobject)
        {

            static findobject _thefindobject;

            m_pfindobject = &_thefindobject;
        }
        return m_pfindobject;
    }

    return 0;
}

qint64* BackImageManager::GetRtable(int curmodul)
{
    switch(curmodul)
    {
        default:
        case 1:
             if(NULL==g_r_table)
             {
                g_r_table = new qint64[COLORNUMSUM];
             }
            return g_r_table;
            break;
    }
}
qint64* BackImageManager::GetGtable(int curmodul)
{
    switch(curmodul)
    {
        default:
        case 1:
             if(NULL==g_g_table)
             {
                g_g_table = new qint64[COLORNUMSUM];
             }
            return g_g_table;
            break;
    }
}
qint64* BackImageManager::GetBtable(int curmodul)
{
    switch(curmodul)
    {
        default:
        case 1:
             if(NULL==g_b_table)
             {
                g_b_table = new qint64[COLORNUMSUM];
             }
            return g_b_table;
            break;
    }
}

QRootGrid* BackImageManager::GetRootGridA(int curmodul)
{
    if(1==curmodul)
    {
        if(0==m_rootgridA)
        {
            CreateRootGridA();
        }
        return m_rootgridA;
    }
    else if(2==curmodul)
    {

    }
    return 0;
}
void BackImageManager::CreateRootGridA()
{
    if(0!=m_rootgridA)
        return;
    QRootGrid &rootgrid = RootGridA();
    m_rootgridA = &rootgrid;
}
QRootGrid& BackImageManager::RootGridA()
{
    static QRootGrid _therootgridA;
    return _therootgridA;

}

QRootGrid* BackImageManager::GetRootGridA_(int curmodul)
{
    if(1==curmodul)
    {
        if(0==m_rootgridA_)
        {
            CreateRootGridA_();
        }
        return m_rootgridA_;
    }
    else if(2==curmodul)
    {

    }
    return 0;
}
void BackImageManager::CreateRootGridA_()
{
    if(0!=m_rootgridA_)
        return;
    QRootGrid &rootgrid = RootGridA_();
    m_rootgridA_ = &rootgrid;
}
QRootGrid& BackImageManager::RootGridA_()
{
    static QRootGrid _therootgridA_;
    return _therootgridA_;

}

QRootGrid* BackImageManager::GetRootGridB(int curmodul)
{
    if(1==curmodul)
    {
        if(0==m_rootgridB)
        {
            CreateRootGridB();
        }
        return m_rootgridB;
    }
    else if(2==curmodul)
    {

    }
    return 0;
}
void BackImageManager::CreateRootGridB()
{
    if(0!=m_rootgridB)
        return;
    QRootGrid &rootgrid = RootGridB();
    m_rootgridB = &rootgrid;
}
QRootGrid& BackImageManager::RootGridB()
{
    static QRootGrid _therootgridB;
    return _therootgridB;

}


QRootGrid* BackImageManager::GetRootGridB_(int curmodul)
{
    if(1==curmodul)
    {
        if(0==m_rootgridB_)
        {
            CreateRootGridB_();
        }
        return m_rootgridB_;
    }
    else if(2==curmodul)
    {

    }
    return 0;
}
void BackImageManager::CreateRootGridB_()
{
    if(0!=m_rootgridB_)
        return;
    QRootGrid &rootgrid = RootGridB_();
    m_rootgridB_ = &rootgrid;
}
QRootGrid& BackImageManager::RootGridB_()
{
    static QRootGrid _therootgridB_;
    return _therootgridB_;

}





