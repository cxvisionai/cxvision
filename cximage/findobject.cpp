
#include <QRect>

#include "findobject.h"
#include "grid.h"
#include "fastmatch.h"
#include "backimagemanager.h"

#define  PUSH_SCANOR(ix,iy) \
{ncurscan++; \
    m_objlistscanorA[ncurscan].x=ix;\
    m_objlistscanorA[ncurscan].y=iy;\
    nscansize++;}

#define CLEAR_SCANOR() \
{ncurscan = -1;\
    nscansize = 0;}

#define  PUSH_SEARCHSEEK(ix,iy) \
{ncursearchseek++; \
    m_objlistcollectorA[ncursearchseek].x=ix;\
    m_objlistcollectorA[ncursearchseek].y=iy;\
    nsearchseeksize++;}

#define  CLEAR_SEARCHSEEK() \
{ncursearchseek = -1; \
    nsearchseeksize = 0;}
typedef unsigned char  BYTE;

#define HI4bit(w)           ((BYTE)((qintptr)(w) >> 4)&0x0f)
#define LO4bit(w)           ((BYTE)((qintptr)(w) & 0x0f))
#define LOBYTE(w)           ((BYTE)((qintptr)(w) & 0xff))

//High bit<-[ 4bit 4bit|4bit 4bit|4bit 4bit|4bit 4bit]->Low bit
//High bit<-[  Service Value     |edge asis|--Pixel--]->Low bit
#define GetServiceValue(rgb)     ((rgb)>>16)
#define GetPixelValue(rgb)       (LOBYTE(rgb))
#define GetAnalysisValue(rgb)    (LO4bit((rgb)>>8))
#define GetEdgeValue(rgb)        (HI4bit((rgb)>>8))

#define SetServiceValue(rgb,ivalue)     (((ivalue)<<16)|(0x0ffff&rgb))
#define SetPixelValue(rgb,ivalue)       ((ivalue)|(0xffffff00&rgb))
#define SetAnalysisValue(rgb,ivalue)    (((ivalue)<<8)|(0xfffff0ff&rgb))
#define SetEdgeValue(rgb,ivalue)         (((ivalue)<<12)|(0xffff0fff&rgb))


#define MAPCLEAR() \
{g_pmapimage->SetROI(rect().x(),rect().y(),rect().width(),rect().height());\
g_pmapimage->ROIImageClear(0);}


#define MAP(ix,iy) g_pmapimage->pixel(ix,iy)
#define MAP_pixel(ix,iy) GetPixelValue(MAP(ix,iy))
#define MAP_service(ix,iy) GetServiceValue(MAP(ix,iy))
#define MAP_analysis(ix,iy) GetAnalysisValue(MAP(ix,iy))
#define MAP_edge(ix,iy) GetEdgeValue(MAP(ix,iy))

#define SetMAP(ix,iy,ivalue)  g_pmapimage->setPixel(ix,iy,ivalue)
#define SetMAP_pixel(ix,iy,ivalue) SetMAP(ix,iy,SetPixelValue(MAP(ix,iy),ivalue))
#define SetMAP_service(ix,iy,ivalue) SetMAP(ix,iy,SetServiceValue(MAP(ix,iy),ivalue))
#define SetMAP_analysis(ix,iy,ivalue) SetMAP(ix,iy,SetAnalysisValue(MAP(ix,iy),ivalue))
#define SetMAP_edge(ix,iy,ivalue) SetMAP(ix,iy,SetEdgeValue(MAP(ix,iy),ivalue))
static APOINT G_SearchPointGroup[224]=
{
//0
{1,0},{0,-1},{-1,0},{0,1},//4
                          {1,1},{-1,1},{-1,-1},{1,-1},//8
{2,-1},{2,0},{2,1},{2,2},{1,2},{0,2},{-1,2},{-2,2},{-2,1},{-2,0},{-2,-1},{-2,-2},{-1,-2},{0,-2},{1,-2},{2,-2},//24
{3,-2},{3,-1},{3,0},{3,1},{3,2},{3,3},{2,3},{1,3},{0,3},{-1,3},{-2,3},{-3,3},{-3,2},{-3,1},{-3,0},{-3,-1},{-3,-2},{-3,-3},{-2,-3},{-1,-3},{0,-3},{1,-3},{2,-3},{3,-3},//48
{4,-3},{4,-2},{4,-1},{4,0},{4,1},{4,2},{4,3},{4,4},{3,4},{2,4},{1,4},{0,4},{-1,4},{-2,4},{-3,4},{-4,4},{-4,3},{-4,2},{-4,1},{-4,0},{-4,-1},{-4,-2},{-4,-3},{-4,-4},{-3,-4},{-2,-4},{-1,-4},{0,-4},{1,-4},{2,-4},{3,-4},{4,-4},//80
{5,-4},{5,-3},{5,-2},{5,-1},{5,0},{5,1},{5,2},{5,3},{5,4},{5,5},{4,5},{3,5},{2,5},{1,5},{0,5},{-1,5},{-2,5},{-3,5},{-4,5},{-5,5},{-5,4},{-5,3},{-5,2},{-5,1},{-5,0},{-5,-1},{-5,-2},{-5,-3},{-5,-4},{-5,-5},{-4,-5},{-3,-5},{-2,-5},{-1,-5},{0,-5},{1,-5},{2,-5},{3,-5},{4,-5},{5,-5},//120
{6,-5},{6,-4},{6,-3},{6,-2},{6,-1},{6,0},{6,1},{6,2},{6,3},{6,4},{6,5},{6,6},{5,6},{4,6},{3,6},{2,6},{1,6},{0,6},{-1,6},{-2,6},{-3,6},{-4,6},{-5,6},{-6,6},{-6,5},{-6,4},{-6,3},{-6,2},{-6,1},{-6,0},{-6,-1},{-6,-2},{-6,-3},{-6,-4},{-6,-5},{-6,-6},{-5,-6},{-4,-6},{-3,-6},{-2,-6},{-1,-6},{0,-6},{1,-6},{2,-6},{3,-6},{4,-6},{5,-6},{6,-6},//168
{7,-6},{7,-5},{7,-4},{7,-3},{7,-2},{7,-1},{7,0},{7,1},{7,2},{7,3},{7,4},{7,5},{7,6},{7,7},{6,7},{5,7},{4,7},{3,7},{2,7},{1,7},{0,7},{-1,7},{-2,7},{-3,7},{-4,7},{-5,7},{-6,7},{-7,7},{-7,6},{-7,5},{-7,4},{-7,3},{-7,2},{-7,1},{-7,0},{-7,-1},{-7,-2},{-7,-3},{-7,-4},{-7,-5},{-7,-6},{-7,-7},{-6,-7},{-5,-7},{-4,-7},{-3,-7},{-2,-7},{-1,-7},{0,-7},{1,-7},{2,-7},{3,-7},{4,-7},{5,-7},{6,-7},{7,-7},//224
};
static APOINT G_SearchPointGroup_OOD[16]=
{
//0
{0,-1},{0,1},{1,0},{-1,0},{1,1},{-1,1},{-1,-1},{1,-1},
    {0,2},{0,3},{0,4},{0,5},//8
{0,8},{0,11},{0,15},{0,20}

};
static APOINT G_SearchPointGroup_OD[16]=
{
//0
{0,-1},{0,1},{1,0},{-1,0},{1,1},{-1,1},{-1,-1},{1,-1},
    {0,2},{0,3},{0,4},{0,5},//8
{0,6},{0,7},{0,8},{0,9}

};


static APOINT G_SearchPointGroup_ODD[16]=
{
//0
{0,-1},{0,1},{1,0},{-1,0},{1,1},{-1,1},{-1,-1},{1,-1},
    {0,3},{0,6},{0,9},{0,12},//8
{0,15},{0,18},{0,21},{0,24}

};

static APOINT G_SearchPointGroup_L[16]=
{
//0
{1,0},{0,1},{0,-1},{-1,0},{2,0},{3,0},{4,0},{5,0},//8
{6,0},{7,0},{8,0},{9,0},{10,0},{11,0},{12,0},{13,0}

};
static APOINT G_SearchPointGroup_LL[16]=
{
//0
{1,0},{0,1},{0,-1},{-1,0},{2,0},{4,0},{6,0},{8,0},//8
{10,0},{12,0},{14,0},{16,0},{18,0},{20,0},{22,0},{24,0}

};
static APOINT G_SearchPointGroup_LLL[16]=
{
//0
{1,0},{0,1},{0,-1},{-1,0},{3,0},{6,0},{9,0},{12,0},//8
{15,0},{18,0},{21,0},{24,0},{27,0},{30,0},{33,0},{36,0}

};
static APOINT G_SearchPointGroup_LMAX[16]=
{
//0
{1,0},{0,1},{0,-1},{-1,0},{5,0},{10,0},{15,0},{20,0},//8
{25,0},{30,0},{35,0},{40,0},{45,0},{50,0},{55,0},{60,0}

};
static APOINT G_SearchPointGroup_R[16]=
{
    //0
    {-1,0},{1,0},{0,1},{0,-1},{-2,0},{-3,0},{-4,0},{-5,0},//8
    {-6,0},{-7,0},{-8,0},{-9,0},{-10,0},{-11,0},{-12,0},{-13,0}

};
static APOINT G_SearchPointGroup_U[16]=
{
//0
{0,-1},{0,1},{1,0},{-1,0},{0,-2},{0,-3},{0,-4},{0,-5},//8
{0,-6},{0,-7},{0,-8},{0,-9},{0,-10},{0,-11},{0,-12},{0,-13}

};
static APOINT G_SearchPointGroup_D[16]=
{
//0
{0,-1},{0,1},{1,0},{-1,0},{0,2},{0,3},{0,4},{0,5},//8
{0,6},{0,7},{0,8},{0,9},{0,10},{0,11},{0,12},{0,13}

};
static APOINT G_SearchPointGroup_DD[16]=
{
//0
{0,-1},{0,1},{1,0},{-1,0},{0,2},{0,4},{0,6},{0,8},//8
{0,10},{0,12},{0,14},{0,16},{0,18},{0,20},{0,22},{0,24}

};
static APOINT G_SearchPointGroup_DDD[16]=
{
//0
{0,-1},{0,1},{1,0},{-1,0},{0,3},{0,6},{0,9},{0,12},//8
{0,15},{0,18},{0,21},{0,24},{0,27},{0,30},{0,33},{0,36}

};
static APOINT G_SearchPointGroup_X[16]=
{
//0
{0,-1},{0,1},{1,0},{-1,0},{0,2},{0,3},{0,4},{0,5},//8
{0,6},{0,7},{0,8},{0,9},{0,10},{0,11},{0,12},{0,13}

};

int findobject::m_curfindobjectnum = 0;
findobject::findobject():
    m_iborw(3),
    m_ifilterNedge(0),
    m_idistance(16),
    m_icurobj(0),
    m_iobjnum(0),
    m_iminarea(5),
    m_imaxarea(99999),
    m_iminobjw(0),
    m_iminobjh(0),
    m_imaxobjw(9999),
    m_imaxobjh(9999),
    m_ihgap(50),
    m_isgap(20),
    m_iogap(30),
    m_pgetimage(0),
    m_icopyw(30),
    m_icopyh(30),
    m_icopywgrid(20),
    m_background_edge(2),
    m_background_method(1),
    m_ioffsetx0(0),
    m_ioffsetx1(0),
    m_ioffsety0(0),
    m_ioffsety1(0),
    m_imagethre(18),
    m_imagethreincrease(0),
    m_imagecomparegap(2),
    m_imagefindBorW(0),
    m_imageedge_5o7(5)

{
    QString strname = QString("fobject%1").arg(m_curfindobjectnum);
     setname(strname.toStdString().c_str());
     m_curfindobjectnum = m_curfindobjectnum + 1;

    setcolor(0,255,120);
    int icurmodule = BackImageManager::GetCurMode();
    g_pbackobjectimage = BackImageManager::GetBackObjectImage(icurmodule);
    g_pmapimage = BackImageManager::GetMapImage(icurmodule);

    m_objlistscanorA = BackImageManager::GetListScan(icurmodule);
    m_objlistcollectorA = BackImageManager::GetListCollect(icurmodule);
    m_SearchPointGroup = G_SearchPointGroup;
    m_searchtype = ObjectSearchType::Search_O;
}
findobject::~findobject()
{

}
void findobject::setbrow(int iborw)
{
    m_iborw = iborw;
}
void findobject::setshow(int ishow)
{
    QShape::setshow(ishow);
}
void findobject::setrect(int ix, int iy, int iw, int ih)
{
    QShape::setrect(ix,iy,iw,ih);
}
void findobject::drawshape(QPainter &painter,QPalette &pal)
{
    if(show()&0x02)
    {
        m_rectresults.drawshape(painter);
    }
    if(show()&0x04)
    {
        m_curedge.drawshape(painter);
        m_curobject.drawshape(painter);
        //painter.setPen(QPen(Qt::green));
        //for(int i=0;i<m_curedge.size();i++)
        //{
        //   m_curedge[i].drawshape(painter);
        //}

    }
    if(show()&0x08)
    {
        int isize = m_cent_h_bw_points_v.size();
        for(int i=0;i<isize;i++)
            m_cent_h_bw_points_v[i].drawshape(painter);
        isize = m_cent_v_bw_points_v.size();
        for(int i=0;i<isize;i++)
            m_cent_v_bw_points_v[i].drawshape(painter);
        isize = m_cent_h_wb_points_v.size();
        for(int i=0;i<isize;i++)
            m_cent_h_wb_points_v[i].drawshape(painter);
        isize = m_cent_v_wb_points_v.size();
        for(int i=0;i<isize;i++)
            m_cent_v_wb_points_v[i].drawshape(painter);
    }
    QShape::drawshape(painter,pal);
}
int findobject::getresultcentx(int inum)
{
    if(inum<m_rectresults.size())
        return m_rectresults.getrect(inum).center().x();
    else
        return 0;
}
int findobject::getresultcenty(int inum)
{
    if(inum<m_rectresults.size())
        return m_rectresults.getrect(inum).center().y();
    else
        return 0;
}
int findobject::getresultx(int inum)
{
    if(inum<m_rectresults.size()&&inum>=0)
        return m_rectresults.getrect(inum).x();
    else
        return 0;
}
int findobject::getresulty(int inum)
{

    if(inum<m_rectresults.size()&&inum>=0)
        return m_rectresults.getrect(inum).y();
    else
        return 0;
}
int findobject::getresultw(int inum)
{

    if(inum<m_rectresults.size())
        return m_rectresults.getrect(inum).width();
    else
        return 0;
}
int findobject::getresulth(int inum)
{

    if(inum<m_rectresults.size())
        return m_rectresults.getrect(inum).height();
    else
        return 0;
}
int findobject::getresultsize(int inum)
{
    if(inum<m_rectresults.size())
        return m_vobjnum.at(inum);
    else
        return 0;
}
int findobject::getresultobjsnum()
{
    return m_rectresults.size();
}
void findobject::setdistance(int idist)
{
    switch (m_searchtype)
    {
    case ObjectSearchType::Search_O:
        if(idist>3&&idist<1520)
            m_idistance = idist;
        else if(idist<4)
            m_idistance = 4;
        else if(idist>1519)
            m_idistance = 1520;

        break;
    case ObjectSearchType::Search_OL:
    case ObjectSearchType::Search_OR:
    case ObjectSearchType::Search_OU:
    case ObjectSearchType::Search_OD:
    case ObjectSearchType::Search_OX:
        if(idist>0&&idist<16)
            m_idistance = idist;
        else if(idist<0)
            m_idistance = 4;
        else if(idist>16)
            m_idistance = 16;

        break;
    default:
        m_idistance = 16;
        break;
    }
}
void findobject::setoffset(int ix0,int ix1,int iy0,int iy1)
{
    m_ioffsetx0 = ix0;
    m_ioffsetx1 = ix1;
    m_ioffsety0 = iy0;
    m_ioffsety1 = iy1;
}
void findobject::setsearchtype(int itype)
{
    switch (itype) {
    case 0:
      m_searchtype = ObjectSearchType::Search_O;
      m_SearchPointGroup = G_SearchPointGroup;
        break;
    case 1:
        m_searchtype = ObjectSearchType::Search_OL;
        m_SearchPointGroup = G_SearchPointGroup_L;
        break;
    case 11:
        m_searchtype = ObjectSearchType::Search_OL;
        m_SearchPointGroup = G_SearchPointGroup_LL;
        break;
    case 111:
        m_searchtype = ObjectSearchType::Search_OL;
        m_SearchPointGroup = G_SearchPointGroup_LLL;
        break;
    case 1111:
        m_searchtype = ObjectSearchType::Search_OL;
        m_SearchPointGroup = G_SearchPointGroup_LMAX;
        break;

    case 2:
        m_searchtype = ObjectSearchType::Search_OR;
        m_SearchPointGroup = G_SearchPointGroup_R;
        break;
    case 3:
        m_searchtype = ObjectSearchType::Search_OU;
        m_SearchPointGroup = G_SearchPointGroup_U;

        break;
    case 4:
        m_searchtype = ObjectSearchType::Search_OD;
        m_SearchPointGroup = G_SearchPointGroup_D;

        break;
    case 44:
        m_searchtype = ObjectSearchType::Search_OD;
        m_SearchPointGroup = G_SearchPointGroup_DD;

        break;
    case 444:
        m_searchtype = ObjectSearchType::Search_OD;
        m_SearchPointGroup = G_SearchPointGroup_DDD;

        break;
        
    case 40:
        m_searchtype = ObjectSearchType::Search_OD;
        m_SearchPointGroup = G_SearchPointGroup_OD;

        break;
    case 440:
        m_searchtype = ObjectSearchType::Search_OD;
        m_SearchPointGroup = G_SearchPointGroup_ODD;

        break;
    case 441:
        m_searchtype = ObjectSearchType::Search_OD;
        m_SearchPointGroup = G_SearchPointGroup_OOD;

        break;

    case 5:
        m_searchtype = ObjectSearchType::Search_OX;
        m_SearchPointGroup = G_SearchPointGroup_X;

        break;
    case 6:

        break;

    default:
        m_searchtype = ObjectSearchType::Search_O;
        m_SearchPointGroup = G_SearchPointGroup;

        break;
    }
}
void findobject::Measure(ImageBase &image)
{

    m_pgetimage = &image;
    if(image.width()< rect().x() + rect().width()
       || image.height() < rect().y() + rect().height())
        return;//error process
    int iw = rect().width();
    int ih = rect().height();
    int ix = rect().x();
    int iy = rect().y();
    int ix1 = ix + iw;
    int iy1 = iy + ih;

    int m_isearchfirstx = rect().x();
    int m_isearchfirsty = rect().y();

    int nx0=0;
    int ny0=0;
    int nx=0;
    int ny=0;
    int nservice=0;
    QRgb abyte,bytenext;

    int nScanerID=1;
    int icurScanerNUM=0;
    bool boverflow =false;

    int ncurscan =-1;
    int nscansize =0;

    int ncursearchseek =-1;
    int nsearchseeksize =0;

    int ishowfont = 0;
    short mapservice = 0;
    char mapanalysis = 0;

    int iminx = 9999;
    int iminy = 9999;
    int imaxx = 0;
    int imaxy = 0;

    int iborw = 0;

    switch (m_iborw)
    {
        case 901:
        {
            int isize = m_cent_h_bw_points_v.size();
            for(int i=0;i<isize;i++)
                m_cent_h_bw_points_v[i].clear();
            m_cent_h_bw_points_v.clear();
         }
        break;
        case 902:
        {
            int isize = m_cent_v_bw_points_v.size();
            for(int i=0;i<isize;i++)
                m_cent_v_bw_points_v[i].clear();
            m_cent_v_bw_points_v.clear();
        }
        break;
        case 903:
        {
            int isize = m_cent_h_wb_points_v.size();
            for(int i=0;i<isize;i++)
                m_cent_h_wb_points_v[i].clear();
            m_cent_h_wb_points_v.clear();
         }
        break;
        case 904:
        {
            int isize = m_cent_v_wb_points_v.size();
            for(int i=0;i<isize;i++)
                m_cent_v_wb_points_v[i].clear();
            m_cent_v_wb_points_v.clear();
        }
        break;
    }

    m_icurobj = 0;
    m_totalarea = 0;

    m_scanid.clear();
    m_vborw.clear();
    m_vrow.clear();
    m_rectresults.clear();
    m_keypoint.clear();
    m_fitwh.clear();;
    MAPCLEAR();

    CLEAR_SEARCHSEEK();
    PUSH_SEARCHSEEK(m_isearchfirstx,m_isearchfirsty);

    for(int icurSeekNum=0;icurSeekNum<nsearchseeksize;)//searchseek.size();)
    {
FORBEGIN:
        mapservice = MAP_service(m_objlistcollectorA[icurSeekNum].x,m_objlistcollectorA[icurSeekNum].y);

        if(mapservice>0)
        {
            icurSeekNum++;

            if(icurSeekNum>=nsearchseeksize)
                break;
            else
                goto FORBEGIN;
        }
        CLEAR_SCANOR();
        PUSH_SCANOR(m_objlistcollectorA[icurSeekNum].x,m_objlistcollectorA[icurSeekNum].y);
        SetMAP_service(m_objlistcollectorA[icurSeekNum].x,m_objlistcollectorA[icurSeekNum].y,nScanerID);
        icurScanerNUM=0;

CURSCANERBEGIN:
        while(icurScanerNUM!=nscansize)
        {
            nx0= m_objlistscanorA[icurScanerNUM].x;
            ny0= m_objlistscanorA[icurScanerNUM].y;
            abyte =  image.pixel(nx0,ny0);
            abyte = abyte&0x0fff0;

            mapanalysis = MAP_analysis(nx0,ny0);

            if(mapanalysis==findobject::ANLAYSIS_OVER)
                goto NEXTFINDSTEP;
            for(int i= 0;i<m_idistance;i++)
            {
                nx=nx0+m_SearchPointGroup[i].x;
                ny=ny0+m_SearchPointGroup[i].y;
                if(nx<ix
                  ||ny<iy
                  ||nx>=ix1
                  ||ny>=iy1)
                    continue;
                mapanalysis = MAP_analysis(nx,ny);
                mapservice = MAP_service(nx,ny);
                if(mapanalysis==ANLAYSIS_OVER
                    ||mapservice>0
                    ||-nScanerID==nservice)
                    continue;
                bytenext = image.pixel(nx,ny) ;
                bytenext = bytenext&0x0fff0;

                if(abyte==bytenext)
                {
                    PUSH_SCANOR(nx,ny);
                    SetMAP_service(nx,ny,nScanerID);
                    iborw = (abyte==0)?0:1;
                    SetMAP_pixel(nx,ny,iborw);
                    if(iminx > nx)
                        iminx = nx ;
                    if(iminy > ny)
                        iminy = ny ;
                    if(imaxx < nx)
                        imaxx = nx ;
                    if(imaxy < ny)
                        imaxy = ny ;
                }
                else
                {
                    PUSH_SEARCHSEEK(nx,ny);
                    SetMAP_service(nx,ny,-nScanerID);
                }
            }
            SetMAP_analysis(nx0,ny0,ANLAYSIS_OVER);
NEXTFINDSTEP:
            icurScanerNUM++;
        }
CURSCANEREND:
        int iobjw = (imaxx - iminx<=0)?1:imaxx - iminx;
        int iobjh = (imaxy - iminy<=0)?1:imaxy - iminy;
        if((m_ifilterNedge>0
            &&(iminx>m_ifilterNedge
            &&imaxx<iw-m_ifilterNedge
            &&iminy>m_ifilterNedge
            &&imaxy<ih-m_ifilterNedge))
            ||m_ifilterNedge==0)
        {
            if(nscansize>m_iminarea
                &&nscansize<m_imaxarea
                &&iobjw<m_imaxobjw
                &&iobjw>=m_iminobjw
                &&iobjh<m_imaxobjh
                &&iobjh>=m_iminobjh)
            {
                m_vrow.push_back(nscansize);

                abyte= image.pixel(m_objlistscanorA[0].x, m_objlistscanorA[0].y);

                m_vborw.push_back(abyte&0x0fff0);

                boverflow=false;
                {
                    switch(m_iborw)
                    {
                    case 3://any white or black
                        {
                             QRect arectresult(iminx,iminy,imaxx-iminx,imaxy-iminy);
                            QPoint apoint(m_objlistscanorA[0].x,m_objlistscanorA[0].y);
                            m_keypoint.addpoint(apoint);

                            m_rectresults.addrect(arectresult);
                            m_scanid.push_back(nScanerID);
                            m_vobjnum.push_back(nscansize);
                            m_iobjnum++;
                        }
                        ishowfont++;
                        m_icurobj ++;

                        break;
                    case 0:break;// no
                    case 1://select w
                        {
                            {
                                 QRect arectresult(iminx,iminy,imaxx-iminx,imaxy-iminy);
                                if(m_vborw[m_icurobj] >0)
                                {
                                    QPoint apoint(m_objlistscanorA[0].x,m_objlistscanorA[0].y);
                                    m_keypoint.addpoint(apoint);
                                    m_rectresults.addrect(arectresult);
                                    m_scanid.push_back(nScanerID);
                                    m_vobjnum.push_back(nscansize);
                                    m_totalarea = m_totalarea + nscansize;
                                    m_iobjnum++;
                                }
                                ishowfont++;
                                m_icurobj ++;

                            }
                        }
                        break;
                    case 2://select black
                        {
                            {
                                QRect arectresult(iminx,iminy,imaxx-iminx,imaxy-iminy);
                                if(m_vborw[m_icurobj] <255)
                                {
                                    QPoint apoint(m_objlistscanorA[0].x,m_objlistscanorA[0].y);
                                    m_keypoint.addpoint(apoint);

                                    m_rectresults.addrect(arectresult);
                                    m_scanid.push_back(nScanerID);
                                    m_vobjnum.push_back(nscansize);

                                    m_totalarea = m_totalarea + nscansize;
                                    m_iobjnum++;
                                }
                                ishowfont++;
                                m_icurobj ++;

                            }
                        }
                        break;

                    case 11://test
                    {

                        QRect arectresult(iminx,iminy,imaxx-iminx,imaxy-iminy);
                        if(m_vborw[m_icurobj] >0)
                        {
                            for(int ir=0;ir<nscansize;ir++)
                            {
                               image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(0,0,0));
                            }
                            QPoint apoint(m_objlistscanorA[0].x,m_objlistscanorA[0].y);
                            m_keypoint.addpoint(apoint);
                            m_rectresults.addrect(arectresult);
                            m_scanid.push_back(nScanerID);
                            m_vobjnum.push_back(nscansize);

                            m_totalarea = m_totalarea + nscansize;
                            m_iobjnum++;
                        }
                        ishowfont++;
                        m_icurobj ++;

                    }
                    break;
                    case 101://test
                    {

                        QRect arectresult(iminx,iminy,imaxx-iminx,imaxy-iminy);
                        if(m_vborw[m_icurobj] >0)
                        {
                            for(int ir=0;ir<nscansize;ir++)
                            {
                               image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(255,255,255));
                            }
                            QPoint apoint(m_objlistscanorA[0].x,m_objlistscanorA[0].y);
                            m_keypoint.addpoint(apoint);
                            m_rectresults.addrect(arectresult);
                            m_scanid.push_back(nScanerID);
                            m_vobjnum.push_back(nscansize);

                            m_totalarea = m_totalarea + nscansize;
                            m_iobjnum++;
                        }
                        ishowfont++;
                        m_icurobj ++;

                    }
                    break;
                    case 12://test
                    {
                        {
                            QRect arectresult(iminx,iminy,imaxx-iminx,imaxy-iminy);
                            if(m_vborw[m_icurobj] <255)
                            {
                                for(int ir=0;ir<nscansize;ir++)
                                {
                                   image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(0,0,0));
                                }
                                QPoint apoint(m_objlistscanorA[0].x,m_objlistscanorA[0].y);
                                m_keypoint.addpoint(apoint);

                                m_rectresults.addrect(arectresult);
                                m_scanid.push_back(nScanerID);
                                m_vobjnum.push_back(nscansize);

                                m_totalarea = m_totalarea + nscansize;
                                m_iobjnum++;
                            }
                            ishowfont++;
                            m_icurobj ++;

                        }

                    }
                    break;
                    case 102://test
                    {
                        {
                            QRect arectresult(iminx,iminy,imaxx-iminx,imaxy-iminy);
                            if(m_vborw[m_icurobj] <255)
                            {
                                for(int ir=0;ir<nscansize;ir++)
                                {
                                   image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(255,255,255));
                                }
                                QPoint apoint(m_objlistscanorA[0].x,m_objlistscanorA[0].y);
                                m_keypoint.addpoint(apoint);

                                m_rectresults.addrect(arectresult);
                                m_scanid.push_back(nScanerID);
                                m_vobjnum.push_back(nscansize);

                                m_totalarea = m_totalarea + nscansize;
                                m_iobjnum++;
                            }
                            ishowfont++;
                            m_icurobj ++;

                        }
                    }
                    break;
                    case 13://test
                    {
                        {
                             if(m_vborw[m_icurobj] <255)
                             {
                                for(int ir=0;ir<nscansize;ir++)
                                {
                                   image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(255,0,0));
                                }
                            }
                             if(m_vborw[m_icurobj] >0)
                             {
                                for(int ir=0;ir<nscansize;ir++)
                                {
                                   image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(0,0,255));
                                }
                            }
                            ishowfont++;
                            m_icurobj ++;

                        }
                    }
                    break;
                    case 901:
                    {
                       if(m_vborw[m_icurobj] <255)
                       {
                            TwoPointsShape atpshape;
                            for(int ir=0;ir<nscansize;ir++)
                            {
                                atpshape.addpoint(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y);
                            }
                            m_cent_h_bw_points_v.push_back(atpshape);
                       }
                       if(m_vborw[m_icurobj] >0)
                       {
                            TwoPointsShape atpshape;
                            for(int ir=0;ir<nscansize;ir++)
                            {
                                atpshape.addpoint(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y);
                            }
                            m_cent_h_bw_points_v.push_back(atpshape);
                       }
                       ishowfont++;
                       m_icurobj ++;
                    }
                    break;
                    case 902:
                        {
                             if(m_vborw[m_icurobj] <255)
                             {
                                TwoPointsShape atpshape;
                                for(int ir=0;ir<nscansize;ir++)
                                {
                                    atpshape.addpoint(m_objlistscanorA[ir].y,m_objlistscanorA[ir].x);
                                }
                                 m_cent_v_bw_points_v.push_back(atpshape);
                            }
                             if(m_vborw[m_icurobj] >0)
                             {
                                 TwoPointsShape atpshape;
                                for(int ir=0;ir<nscansize;ir++)
                                {
                                    atpshape.addpoint(m_objlistscanorA[ir].y,m_objlistscanorA[ir].x);
                                }
                                m_cent_v_bw_points_v.push_back(atpshape);
                            }
                            ishowfont++;
                            m_icurobj ++;
                        }
                    break;
                    case 903:
                    {
                       if(m_vborw[m_icurobj] <255)
                       {
                            TwoPointsShape atpshape;
                            for(int ir=0;ir<nscansize;ir++)
                            {
                                atpshape.addpoint(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y);
                            }
                            m_cent_h_wb_points_v.push_back(atpshape);
                       }
                       if(m_vborw[m_icurobj] >0)
                       {
                            TwoPointsShape atpshape;
                            for(int ir=0;ir<nscansize;ir++)
                            {
                                atpshape.addpoint(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y);
                            }
                            m_cent_h_wb_points_v.push_back(atpshape);
                       }
                       ishowfont++;
                       m_icurobj ++;
                    }
                    break;
                    case 904:
                        {
                             if(m_vborw[m_icurobj] <255)
                             {
                                TwoPointsShape atpshape;
                                for(int ir=0;ir<nscansize;ir++)
                                {
                                    atpshape.addpoint(m_objlistscanorA[ir].y,m_objlistscanorA[ir].x);
                                }
                                 m_cent_v_wb_points_v.push_back(atpshape);
                            }
                             if(m_vborw[m_icurobj] >0)
                             {
                                 TwoPointsShape atpshape;
                                for(int ir=0;ir<nscansize;ir++)
                                {
                                    atpshape.addpoint(m_objlistscanorA[ir].y,m_objlistscanorA[ir].x);
                                }
                                m_cent_v_wb_points_v.push_back(atpshape);
                            }
                            ishowfont++;
                            m_icurobj ++;
                        }
                    break;
                    }
                }
            }
             else
            {
                //full color no selected object
                 switch(m_iborw)
                 {
                   // no
                   case 21://select w
                    {
                     for(int ir=0;ir<nscansize;ir++)
                     {
                        image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(0,0,0));
                     }
                    }
                    break;
                   case 22://select black
                   {
                     for(int ir=0;ir<nscansize;ir++)
                     {
                        image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(255,255,255));
                     }
                 }
                    break;
                   case 23://test
                 {
                   for(int ir=0;ir<nscansize;ir++)
                   {
                      image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(255,0,0));
                   }
                 }
                    break;

                 }
            }
        }
        else
        {
            //full color no selected object
             switch(m_iborw)
             {
             // no
               case 21://select w
                {
                 for(int ir=0;ir<nscansize;ir++)
                 {
                    image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(0,0,0));
                 }
                }
                break;
               case 22://select black
               {
                 for(int ir=0;ir<nscansize;ir++)
                 {
                    image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(255,255,255));
                 }
             }
             break;
             case 23://test
             {
               for(int ir=0;ir<nscansize;ir++)
               {
                  image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(255,0,0));
               }
             }
            break;
             }
        }

            iminx = 9999;
            iminy = 9999;
            imaxx = 0;
            imaxy = 0;

            nScanerID++;
            icurSeekNum++;
FOREND:
            ;
    }


    switch (m_iborw)
    {
        case 901:
        {
            int isize = m_cent_h_bw_points_v.size();
            for(int i=0;i<isize;i++)
                m_cent_h_bw_points_v[i].makepath(0);

        }
        break;
        case 902:
        {
            int isize = m_cent_v_bw_points_v.size();
            for(int i=0;i<isize;i++)
                m_cent_v_bw_points_v[i].makepath(1);
        }
        break;
        case 903:
        {
            int isize = m_cent_h_wb_points_v.size();
            for(int i=0;i<isize;i++)
                m_cent_h_wb_points_v[i].makepath(0);

        }
        break;
        case 904:
        {
            int isize = m_cent_v_wb_points_v.size();
            for(int i=0;i<isize;i++)
                m_cent_v_wb_points_v[i].makepath(1);
        }
        break;
     }


    CLEAR_SCANOR();
    CLEAR_SEARCHSEEK();

}
void findobject::MeasureGrid(QGrid *pgrid)
{
    setrect(0,0,pgrid->getfastmodelw(),pgrid->getfastmodelh());
    int iw = rect().width();
    int ih = rect().height();
    int ix = rect().x();
    int iy = rect().y();
    int ix1 = ix + iw;
    int iy1 = iy + ih;

    int m_isearchfirstx = rect().x();
    int m_isearchfirsty = rect().y();

    int nx0=0;
    int ny0=0;
    int nx=0;
    int ny=0;
    int nservice=0;
    int ivalue,ivaluenext;

    int nScanerID=1;
    int icurScanerNUM=0;
    bool boverflow =false;

    int ncurscan =-1;
    int nscansize =0;

    int ncursearchseek =-1;
    int nsearchseeksize =0;

    int ishowfont = 0;
    short mapservice = 0;
    char mapanalysis = 0;

    int iminx = 9999;
    int iminy = 9999;
    int imaxx = 0;
    int imaxy = 0;

    int iborw = 0;
    int itotalvalue = 0;

    switch (m_iborw)
    {
        case 901:
        {
            int isize = m_cent_h_bw_points_v.size();
            for(int i=0;i<isize;i++)
                m_cent_h_bw_points_v[i].clear();
            m_cent_h_bw_points_v.clear();
         }
        break;
        case 902:
        {
            int isize = m_cent_v_bw_points_v.size();
            for(int i=0;i<isize;i++)
                m_cent_v_bw_points_v[i].clear();
            m_cent_v_bw_points_v.clear();
        }
        break;
        case 903:
        {
            int isize = m_cent_h_wb_points_v.size();
            for(int i=0;i<isize;i++)
                m_cent_h_wb_points_v[i].clear();
            m_cent_h_wb_points_v.clear();
         }
        break;
        case 904:
        {
            int isize = m_cent_v_wb_points_v.size();
            for(int i=0;i<isize;i++)
                m_cent_v_wb_points_v[i].clear();
            m_cent_v_wb_points_v.clear();
        }
        break;
    }

    m_icurobj = 0;
    m_totalarea = 0;

    m_scanid.clear();
    m_vborw.clear();
    m_vrow.clear();
    m_rectresults.clear();
    m_keypoint.clear();
    m_fitwh.clear();;
    MAPCLEAR();

    CLEAR_SEARCHSEEK();
    PUSH_SEARCHSEEK(m_isearchfirstx,m_isearchfirsty);

    for(int icurSeekNum=0;icurSeekNum<nsearchseeksize;)//searchseek.size();)
    {
FORBEGIN:
        mapservice = MAP_service(m_objlistcollectorA[icurSeekNum].x,m_objlistcollectorA[icurSeekNum].y);

        if(mapservice>0)
        {
            icurSeekNum++;

            if(icurSeekNum>=nsearchseeksize)
                break;
            else
                goto FORBEGIN;
        }
        CLEAR_SCANOR();
        PUSH_SCANOR(m_objlistcollectorA[icurSeekNum].x,m_objlistcollectorA[icurSeekNum].y);
        SetMAP_service(m_objlistcollectorA[icurSeekNum].x,m_objlistcollectorA[icurSeekNum].y,nScanerID);
        icurScanerNUM=0;
        itotalvalue = 0 + pgrid->getfastmodelvalue(m_objlistcollectorA[icurSeekNum].x,m_objlistcollectorA[icurSeekNum].y);
CURSCANERBEGIN:
        while(icurScanerNUM!=nscansize)
        {
            nx0= m_objlistscanorA[icurScanerNUM].x;
            ny0= m_objlistscanorA[icurScanerNUM].y;
            ivalue =  pgrid->getfastmodelvalue(nx0,ny0);
            //abyte = abyte&0x0fff0;

            mapanalysis = MAP_analysis(nx0,ny0);

            if(mapanalysis==findobject::ANLAYSIS_OVER)
                goto NEXTFINDSTEP;
            for(int i= 0;i<m_idistance;i++)
            {
                nx=nx0+m_SearchPointGroup[i].x;
                ny=ny0+m_SearchPointGroup[i].y;
                if(nx<ix
                  ||ny<iy
                  ||nx>=ix1
                  ||ny>=iy1)
                    continue;
                mapanalysis = MAP_analysis(nx,ny);
                mapservice = MAP_service(nx,ny);
                if(mapanalysis==ANLAYSIS_OVER
                    ||mapservice>0
                    ||-nScanerID==nservice)
                    continue;
                ivaluenext = pgrid->getfastmodelvalue(nx,ny) ;
                //bytenext = bytenext&0x0fff0;

                if((ivalue==ivaluenext
                  &&ivalue==0)
                    ||(ivalue!=0
                       &&ivaluenext!=0))
                {
                    PUSH_SCANOR(nx,ny);
                    SetMAP_service(nx,ny,nScanerID);
                    iborw = (ivalue==0)?0:1;
                    SetMAP_pixel(nx,ny,iborw);
                    if(iminx > nx)
                        iminx = nx ;
                    if(iminy > ny)
                        iminy = ny ;
                    if(imaxx < nx)
                        imaxx = nx ;
                    if(imaxy < ny)
                        imaxy = ny ;
                    itotalvalue = itotalvalue + ivaluenext;
                }
                else
                {
                    PUSH_SEARCHSEEK(nx,ny);
                    SetMAP_service(nx,ny,-nScanerID);
                }
            }
            SetMAP_analysis(nx0,ny0,ANLAYSIS_OVER);
NEXTFINDSTEP:
            icurScanerNUM++;
        }
CURSCANEREND:
        int iobjw = (imaxx - iminx<=0)?1:imaxx - iminx;
        int iobjh = (imaxy - iminy<=0)?1:imaxy - iminy;
        if((m_ifilterNedge>0
            &&(iminx>m_ifilterNedge
            &&imaxx<iw-m_ifilterNedge
            &&iminy>m_ifilterNedge
            &&imaxy<ih-m_ifilterNedge))
            ||m_ifilterNedge==0)
        {
            if(nscansize>m_iminarea
                &&nscansize<m_imaxarea
                &&iobjw<m_imaxobjw
                &&iobjw>=m_iminobjw
                &&iobjh<m_imaxobjh
                &&iobjh>=m_iminobjh)
            {
                m_vrow.push_back(nscansize);

                ivalue = pgrid->getfastmodelvalue(m_objlistscanorA[0].x, m_objlistscanorA[0].y);

                m_vborw.push_back(itotalvalue);

                boverflow=false;
                {
                    switch(m_iborw)
                    {
                    case 3://any white or black
                        {
                            QRect arectresult(iminx,iminy,imaxx-iminx,imaxy-iminy);
                            QPoint apoint(m_objlistscanorA[0].x,m_objlistscanorA[0].y);
                            m_keypoint.addpoint(apoint);

                            m_rectresults.addrect(arectresult);
                            m_scanid.push_back(nScanerID);
                            m_vobjnum.push_back(nscansize);
                            m_iobjnum++;
                        }
                        ishowfont++;
                        m_icurobj ++;

                        break;
                    case 0:break;// no
                    case 1://select w
                        {
                            {
                                 QRect arectresult(iminx,iminy,imaxx-iminx,imaxy-iminy);
                                if(m_vborw[m_icurobj] ==0)
                                {
                                    QPoint apoint(m_objlistscanorA[0].x,m_objlistscanorA[0].y);
                                    m_keypoint.addpoint(apoint);
                                    m_rectresults.addrect(arectresult);
                                    m_scanid.push_back(nScanerID);
                                    m_vobjnum.push_back(nscansize);
                                    m_totalarea = m_totalarea + nscansize;
                                    m_iobjnum++;
                                }
                                ishowfont++;
                                m_icurobj ++;

                            }
                        }
                        break;
                    case 2://select black
                        {
                            {
                                QRect arectresult(iminx,iminy,imaxx-iminx,imaxy-iminy);
                                if(m_vborw[m_icurobj] <0)
                                {
                                    QPoint apoint(m_objlistscanorA[0].x,m_objlistscanorA[0].y);
                                    m_keypoint.addpoint(apoint);

                                    m_rectresults.addrect(arectresult);
                                    m_scanid.push_back(nScanerID);
                                    m_vobjnum.push_back(nscansize);

                                    m_totalarea = m_totalarea + nscansize;
                                    m_iobjnum++;
                                }
                                ishowfont++;
                                m_icurobj ++;

                            }
                        }
                        break;

                    case 11://test
                    {

                        QRect arectresult(iminx,iminy,imaxx-iminx,imaxy-iminy);
                        if(m_vborw[m_icurobj] >0)
                        {
                            for(int ir=0;ir<nscansize;ir++)
                            {
                               //image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(0,0,0));
                            }
                            QPoint apoint(m_objlistscanorA[0].x,m_objlistscanorA[0].y);
                            m_keypoint.addpoint(apoint);
                            m_rectresults.addrect(arectresult);
                            m_scanid.push_back(nScanerID);
                            m_vobjnum.push_back(nscansize);

                            m_totalarea = m_totalarea + nscansize;
                            m_iobjnum++;
                        }
                        ishowfont++;
                        m_icurobj ++;

                    }
                    break;
                    case 101://test
                    {

                        QRect arectresult(iminx,iminy,imaxx-iminx,imaxy-iminy);
                        if(m_vborw[m_icurobj] >0)
                        {
                            for(int ir=0;ir<nscansize;ir++)
                            {
                               //image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(255,255,255));
                            }
                            QPoint apoint(m_objlistscanorA[0].x,m_objlistscanorA[0].y);
                            m_keypoint.addpoint(apoint);
                            m_rectresults.addrect(arectresult);
                            m_scanid.push_back(nScanerID);
                            m_vobjnum.push_back(nscansize);

                            m_totalarea = m_totalarea + nscansize;
                            m_iobjnum++;
                        }
                        ishowfont++;
                        m_icurobj ++;

                    }
                    break;
                    case 12://test
                    {
                        {
                            QRect arectresult(iminx,iminy,imaxx-iminx,imaxy-iminy);
                            if(m_vborw[m_icurobj] <255)
                            {
                                for(int ir=0;ir<nscansize;ir++)
                                {
                                   //image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(0,0,0));
                                }
                                QPoint apoint(m_objlistscanorA[0].x,m_objlistscanorA[0].y);
                                m_keypoint.addpoint(apoint);

                                m_rectresults.addrect(arectresult);
                                m_scanid.push_back(nScanerID);
                                m_vobjnum.push_back(nscansize);

                                m_totalarea = m_totalarea + nscansize;
                                m_iobjnum++;
                            }
                            ishowfont++;
                            m_icurobj ++;

                        }

                    }
                    break;
                    case 102://test
                    {
                        {
                            QRect arectresult(iminx,iminy,imaxx-iminx,imaxy-iminy);
                            if(m_vborw[m_icurobj] <255)
                            {
                                for(int ir=0;ir<nscansize;ir++)
                                {
                                   //image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(255,255,255));
                                }
                                QPoint apoint(m_objlistscanorA[0].x,m_objlistscanorA[0].y);
                                m_keypoint.addpoint(apoint);

                                m_rectresults.addrect(arectresult);
                                m_scanid.push_back(nScanerID);
                                m_vobjnum.push_back(nscansize);

                                m_totalarea = m_totalarea + nscansize;
                                m_iobjnum++;
                            }
                            ishowfont++;
                            m_icurobj ++;

                        }
                    }
                    break;
                    case 13://test
                    {
                        {
                             if(m_vborw[m_icurobj] <255)
                             {
                                for(int ir=0;ir<nscansize;ir++)
                                {
                                   //image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(255,0,0));
                                }
                            }
                             if(m_vborw[m_icurobj] >0)
                             {
                                for(int ir=0;ir<nscansize;ir++)
                                {
                                   //image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(0,0,255));
                                }
                            }
                            ishowfont++;
                            m_icurobj ++;

                        }
                    }
                    break;
                    case 901:
                    {
                       if(m_vborw[m_icurobj] <255)
                       {
                            TwoPointsShape atpshape;
                            for(int ir=0;ir<nscansize;ir++)
                            {
                                atpshape.addpoint(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y);
                            }
                            m_cent_h_bw_points_v.push_back(atpshape);
                       }
                       if(m_vborw[m_icurobj] >0)
                       {
                            TwoPointsShape atpshape;
                            for(int ir=0;ir<nscansize;ir++)
                            {
                                atpshape.addpoint(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y);
                            }
                            m_cent_h_bw_points_v.push_back(atpshape);
                       }
                       ishowfont++;
                       m_icurobj ++;
                    }
                    break;
                    case 902:
                        {
                             if(m_vborw[m_icurobj] <255)
                             {
                                TwoPointsShape atpshape;
                                for(int ir=0;ir<nscansize;ir++)
                                {
                                    atpshape.addpoint(m_objlistscanorA[ir].y,m_objlistscanorA[ir].x);
                                }
                                 m_cent_v_bw_points_v.push_back(atpshape);
                            }
                             if(m_vborw[m_icurobj] >0)
                             {
                                 TwoPointsShape atpshape;
                                for(int ir=0;ir<nscansize;ir++)
                                {
                                    atpshape.addpoint(m_objlistscanorA[ir].y,m_objlistscanorA[ir].x);
                                }
                                m_cent_v_bw_points_v.push_back(atpshape);
                            }
                            ishowfont++;
                            m_icurobj ++;
                        }
                    break;
                    case 903:
                    {
                       if(m_vborw[m_icurobj] <255)
                       {
                            TwoPointsShape atpshape;
                            for(int ir=0;ir<nscansize;ir++)
                            {
                                atpshape.addpoint(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y);
                            }
                            m_cent_h_wb_points_v.push_back(atpshape);
                       }
                       if(m_vborw[m_icurobj] >0)
                       {
                            TwoPointsShape atpshape;
                            for(int ir=0;ir<nscansize;ir++)
                            {
                                atpshape.addpoint(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y);
                            }
                            m_cent_h_wb_points_v.push_back(atpshape);
                       }
                       ishowfont++;
                       m_icurobj ++;
                    }
                    break;
                    case 904:
                        {
                             if(m_vborw[m_icurobj] <255)
                             {
                                TwoPointsShape atpshape;
                                for(int ir=0;ir<nscansize;ir++)
                                {
                                    atpshape.addpoint(m_objlistscanorA[ir].y,m_objlistscanorA[ir].x);
                                }
                                 m_cent_v_wb_points_v.push_back(atpshape);
                            }
                             if(m_vborw[m_icurobj] >0)
                             {
                                 TwoPointsShape atpshape;
                                for(int ir=0;ir<nscansize;ir++)
                                {
                                    atpshape.addpoint(m_objlistscanorA[ir].y,m_objlistscanorA[ir].x);
                                }
                                m_cent_v_wb_points_v.push_back(atpshape);
                            }
                            ishowfont++;
                            m_icurobj ++;
                        }
                    break;
                    }
                }
            }
             else
            {
                //full color no selected object
                 switch(m_iborw)
                 {
                   // no
                   case 21://select w
                    {
                     for(int ir=0;ir<nscansize;ir++)
                     {
                        //image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(0,0,0));
                     }
                    }
                    break;
                   case 22://select black
                   {
                     for(int ir=0;ir<nscansize;ir++)
                     {
                        //image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(255,255,255));
                     }
                 }
                    break;
                   case 23://test
                 {
                   for(int ir=0;ir<nscansize;ir++)
                   {
                      //image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(255,0,0));
                   }
                 }
                    break;

                 }
            }
        }
        else
        {
            //full color no selected object
             switch(m_iborw)
             {
             // no
               case 21://select w
                {
                 for(int ir=0;ir<nscansize;ir++)
                 {
                    //image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(0,0,0));
                 }
                }
                break;
               case 22://select black
               {
                 for(int ir=0;ir<nscansize;ir++)
                 {
                    //image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(255,255,255));
                 }
             }
             break;
             case 23://test
             {
               for(int ir=0;ir<nscansize;ir++)
               {
                  //image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(255,0,0));
               }
             }
            break;
             }
        }

            iminx = 9999;
            iminy = 9999;
            imaxx = 0;
            imaxy = 0;

            nScanerID++;
            icurSeekNum++;
FOREND:
            ;
    }


    switch (m_iborw)
    {
        case 901:
        {
            int isize = m_cent_h_bw_points_v.size();
            for(int i=0;i<isize;i++)
                m_cent_h_bw_points_v[i].makepath(0);

        }
        break;
        case 902:
        {
            int isize = m_cent_v_bw_points_v.size();
            for(int i=0;i<isize;i++)
                m_cent_v_bw_points_v[i].makepath(1);
        }
        break;
        case 903:
        {
            int isize = m_cent_h_wb_points_v.size();
            for(int i=0;i<isize;i++)
                m_cent_h_wb_points_v[i].makepath(0);

        }
        break;
        case 904:
        {
            int isize = m_cent_v_wb_points_v.size();
            for(int i=0;i<isize;i++)
                m_cent_v_wb_points_v[i].makepath(1);
        }
        break;
     }


    CLEAR_SCANOR();
    CLEAR_SEARCHSEEK();

}

void findobject::Edge(int inum)
{
    if(inum>=m_scanid.size()
            ||inum<0)
        return;
    int imapservice = m_scanid[inum];
    int ix = getresultx(inum);
    int iy = getresulty(inum);
    int iw = getresultw(inum);
    int ih = getresulth(inum);

    m_curedge.setshow(3);
    m_curedge.clear();

    for(int iy0=0;iy0<ih;iy0++)
    {
        int ibeginx = 0;
        int iendx = iw-1;
        for(int ix0=0;ix0<iw;ix0++)
        {
           int imapservicecur = MAP_service(ix0+ix,iy0+iy);
           if(imapservicecur == imapservice)
           {

               ibeginx = ix0;
                break;
           }
        }
        for(int ix0=iw-1;ix0>=0;ix0--)
        {
           int imapservicecur = MAP_service(ix0+ix,iy0+iy);
           if(imapservicecur == imapservice)
           {
                iendx = ix0;
                break;
           }
        }
        qreal qrx1 = ibeginx+ix;
        qreal qry1 = iy0+iy;
        qreal qrx2 = iendx+ix;
        qreal qry2 = iy0+iy;

        //LineShape aline;
        //aline.setline(ibeginx+ix,iy0+iy,iendx+ix,iy0+iy);
        //m_curedge.push_back(aline);
        //int inum = m_curedge.size() - 1;
        //m_curedge[inum].setPercent(m_dsamplerate);

        m_curedge.addpointa(qrx1,qry1);
        m_curedge.addpointb(qrx2,qry2);
    }
}
void findobject::Object(int inum)
{
    if(inum>=m_scanid.size()
            ||inum<0)
        return;
    int imapservice = m_scanid[inum];
    int ix = getresultx(inum);
    int iy = getresulty(inum);
    int iw = getresultw(inum);
    int ih = getresulth(inum);

    m_curobject.setshow(3);
    m_curobject.clear();

    for(int iy0=0;iy0<ih;iy0++)
    {
        for(int ix0=0;ix0<iw;ix0++)
        {
           int imapservicecur = MAP_service(ix0+ix,iy0+iy);
           if(imapservicecur == imapservice)
           {
              qreal qrx1 = ix0;
              qreal qry1 = iy0;

              m_curobject.addpoint(qrx1,qry1);

           }
        }


        //LineShape aline;
        //aline.setline(ibeginx+ix,iy0+iy,iendx+ix,iy0+iy);
        //m_curedge.push_back(aline);
        //int inum = m_curedge.size() - 1;
        //m_curedge[inum].setPercent(m_dsamplerate);

    }
}

void findobject::measure(void *pimage)
{
    ImageBase *pgetimage = (ImageBase*)pimage;
    Measure(*pgetimage);
}

void findobject::sethsogap(int ihgap,int isgap,int iogap)
{
    m_ihgap = ihgap;
    m_isgap = isgap;
    m_iogap = iogap;
}
void findobject::setminmaxarea(int imin,int imax)
{
    m_iminarea = imin;
    m_imaxarea = imax;
}
void findobject::MeasureX(ImageBase &image)
{
    m_pgetimage = &image;
    if(image.width()< rect().x() + rect().width()
       || image.height() < rect().y() + rect().height())
        return;//error process
    int iw = rect().width();
    int ih = rect().height();
    int ix = rect().x();
    int iy = rect().y();
    int ix1 = ix + iw;
    int iy1 = iy + ih;

    int m_isearchfirstx = rect().x();
    int m_isearchfirsty = rect().y();

    int nx0=0;
    int ny0=0;
    int nx=0;
    int ny=0;
    int nservice=0;
    QRgb abyte,bytenext;

    int nScanerID=1;
    int icurScanerNUM=0;
    bool boverflow =false;

    int ncurscan =-1;
    int nscansize =0;

    int ncursearchseek =-1;
    int nsearchseeksize =0;

    int ishowfont = 0;
    short mapservice = 0;
    char mapanalysis = 0;

    int iminx = 9999;
    int iminy = 9999;
    int imaxx = 0;
    int imaxy = 0;

    int iborw = 0;

    m_icurobj = 0;
    m_iobjnum = 0;
    m_totalarea = 0;

    m_scanid.clear();
    m_vborw.clear();
    m_vrow.clear();
    m_vobjnum.clear();
    m_rectresults.clear();
    m_keypoint.clear();
    m_fitwh.clear();;
    MAPCLEAR();

    CLEAR_SEARCHSEEK();
    PUSH_SEARCHSEEK(m_isearchfirstx,m_isearchfirsty);

    for(int icurSeekNum=0;icurSeekNum<nsearchseeksize;)//searchseek.size();)
    {
FORBEGIN:
        mapservice = MAP_service(m_objlistcollectorA[icurSeekNum].x,m_objlistcollectorA[icurSeekNum].y);

        if(mapservice>0)
        {
            icurSeekNum++;

            if(icurSeekNum>=nsearchseeksize)
                break;
            else
                goto FORBEGIN;
        }
        CLEAR_SCANOR();
        PUSH_SCANOR(m_objlistcollectorA[icurSeekNum].x,m_objlistcollectorA[icurSeekNum].y);
        SetMAP_service(m_objlistcollectorA[icurSeekNum].x,m_objlistcollectorA[icurSeekNum].y,nScanerID);
        icurScanerNUM=0;

CURSCANERBEGIN:
        while(icurScanerNUM!=nscansize)
        {
            nx0= m_objlistscanorA[icurScanerNUM].x;
            ny0= m_objlistscanorA[icurScanerNUM].y;
            abyte =  image.pixel(nx0,ny0);


            mapanalysis = MAP_analysis(nx0,ny0);

            if(mapanalysis==findobject::ANLAYSIS_OVER)
                goto NEXTFINDSTEP;
            for(int i= 0;i<m_idistance;i++)
            {
                nx=nx0+m_SearchPointGroup[i].x;
                ny=ny0+m_SearchPointGroup[i].y;
                if(nx<=ix
                  ||ny<=iy
                  ||nx>=ix1
                  ||ny>=iy1)
                    continue;
                mapanalysis = MAP_analysis(nx,ny);
                mapservice = MAP_service(nx,ny);
                if(mapanalysis==ANLAYSIS_OVER
                    ||mapservice>0
                    ||-nScanerID==nservice)
                    continue;
                bytenext = image.pixel(nx,ny) ;


                if(image.PixelCompare(abyte,bytenext,m_ihgap,m_isgap,m_iogap))
                {
                    PUSH_SCANOR(nx,ny);
                    SetMAP_service(nx,ny,nScanerID);
                    iborw = (abyte==0)?0:1;
                    SetMAP_pixel(nx,ny,iborw);
                    if(iminx > nx)
                        iminx = nx ;
                    if(iminy > ny)
                        iminy = ny ;
                    if(imaxx < nx)
                        imaxx = nx ;
                    if(imaxy < ny)
                        imaxy = ny ;
                }
                else
                {
                    PUSH_SEARCHSEEK(nx,ny);
                    SetMAP_service(nx,ny,-nScanerID);
                }
            }
            SetMAP_analysis(nx0,ny0,ANLAYSIS_OVER);
NEXTFINDSTEP:
            icurScanerNUM++;
        }
CURSCANEREND:
        int iobjw = imaxx - iminx;
        int iobjh = imaxy - iminy;
        if((m_ifilterNedge>0
            &&(iminx>m_ifilterNedge
            &&imaxx<iw-m_ifilterNedge
            &&iminy>m_ifilterNedge
            &&imaxy<ih-m_ifilterNedge))
            ||m_ifilterNedge==0)
        {
            if(nscansize>m_iminarea
                &&nscansize<m_imaxarea
                &&iobjw<m_imaxobjw
                &&iobjw>=m_iminobjw
                &&iobjh<m_imaxobjh
                &&iobjh>=m_iminobjh)
            {
                m_vrow.push_back(nscansize);

                abyte= image.pixel(m_objlistscanorA[0].x, m_objlistscanorA[0].y);

                //m_vborw.push_back(abyte&0x0fff0);

                boverflow=false;
                {
                    switch(m_iborw)
                    {
                    case 3://any white or black
                        {
                            QRect arectresult(iminx,iminy,imaxx-iminx,imaxy-iminy);
                            QPoint apoint(m_objlistscanorA[0].x,m_objlistscanorA[0].y);
                            m_keypoint.addpoint(apoint);

                            m_rectresults.addrect(arectresult);
                            m_scanid.push_back(nScanerID);
                            m_iobjnum++;
                        }
                        ishowfont++;
                        m_icurobj ++;


                        break;
                    case 13://test
                    {
                        {
                             if(m_vborw[m_icurobj] <255)
                             {
                                for(int ir=0;ir<nscansize;ir++)
                                {
                                   image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(255,0,0));
                                }
                            }
                             if(m_vborw[m_icurobj] >0)
                             {
                                for(int ir=0;ir<nscansize;ir++)
                                {
                                   image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(0,0,255));
                                }
                            }
                            ishowfont++;
                            m_icurobj ++;

                        }
                    }
                    break;

                    }
                }
            }
             else
            {
                //full color no selected object
                 switch(m_iborw)
                 {
                 // no
                   case 21://select w
                    {
                     for(int ir=0;ir<nscansize;ir++)
                     {
                        image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(0,0,0));
                     }
                    }
                    break;
                   case 22://select black
                   {
                     for(int ir=0;ir<nscansize;ir++)
                     {
                        image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(255,255,255));
                     }
                 }
                 break;
                 case 23://test
                 {
                   for(int ir=0;ir<nscansize;ir++)
                   {
                      image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(255,0,0));
                   }
                 }
                break;

                 }
            }
        }
        else
        {
            //full color no selected object
             switch(m_iborw)
             {
             // no
               case 21://select w
                {
                 for(int ir=0;ir<nscansize;ir++)
                 {
                    image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(0,0,0));
                 }
                }
                break;
               case 22://select black
               {
                 for(int ir=0;ir<nscansize;ir++)
                 {
                    image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(255,255,255));
                 }
             }
             break;
             case 23://test
             {
               for(int ir=0;ir<nscansize;ir++)
               {
                  image.setPixel(m_objlistscanorA[ir].x,m_objlistscanorA[ir].y,qRgb(255,0,0));
               }
             }
            break;
             }
        }
            iminx = 9999;
            iminy = 9999;
            imaxx = 0;
            imaxy = 0;

            nScanerID++;
            icurSeekNum++;
FOREND:
            ;
    }

    CLEAR_SCANOR();
    CLEAR_SEARCHSEEK();

}
void findobject::measurex(void *pimage)
{
    ImageBase *pgetimage = (ImageBase*)pimage;

    MeasureX(*pgetimage);
}

void findobject::setminmaxwh(int iminw,int imaxw,int iminh,int imaxh)
{
    m_imaxobjw = imaxw;
    m_iminobjw = iminw;
    m_imaxobjh = imaxh;
    m_iminobjh = iminh;
}
void findobject::setobjectgrid(int iw,int ih,int ixgrid)
{
    m_icopyw = iw;
    m_icopyh = ih;
    m_icopywgrid = ixgrid;
}
int findobject::getobjectgridw()
{
    return m_icopyw;
}
int findobject::getobjectgridh()
{
    return m_icopyh;
}

void findobject::setbackground(int iedge,int ibackgroundmethod)
{
    m_background_edge = iedge;
    m_background_method = ibackgroundmethod;
}
void findobject::resultsrectfilter()
{
    m_rectresults.removecontains_c();
}
void findobject::objectgrid(void *pimage)
{
    ImageBase *pgetimage =(ImageBase *)pimage;
    m_rectgrids.clear();
    int isize = m_rectresults.size();
    int inumw = 0;
    int inumh = 0;
    int ibeginx = 0;
    int ibeginy = 0;


    for(int inum =0;inum<isize;inum++)
    {
        if(inumw>=m_icopywgrid)
        {
            inumw =0;
            inumh++;
        }

        int iobjx0 =  getresultx(inum);
        int iobjy0 =  getresulty(inum);
        int iobjw =  getresultw(inum);
        int iobjh =  getresulth(inum);

        iobjx0 = iobjx0 +m_ioffsetx0;
        iobjy0 = iobjy0 +m_ioffsety0;
        iobjw = iobjw +m_ioffsetx1;
        iobjh = iobjh +m_ioffsety1;



        ibeginx = inumw * m_icopyw;
        ibeginy = inumh * m_icopyh;

        int igridsum = (iobjw+10)/m_icopyw +((iobjw+10)%m_icopyw>0?1:0) ;
        pgetimage->SetROI(iobjx0,iobjy0,iobjw+1,iobjh+1);
        QRgb acolor = pgetimage->ROIBackground(m_background_edge,m_background_edge,m_background_edge,m_background_edge,m_background_method);
        g_pbackobjectimage->SetROI(ibeginx,ibeginy,m_icopyw*igridsum,m_icopyh);
        g_pbackobjectimage->ROIImageColor(acolor);

        g_pbackobjectimage->SetROI(ibeginx+10,ibeginy+10,iobjw+1,iobjh+1);

        pgetimage->SetMode(3);
        pgetimage->ROItoROI(g_pbackobjectimage);

        QRect arect(ibeginx,ibeginy,m_icopyw * igridsum,m_icopyh);
        m_rectgrids.addrect(arect);

        //int ilx0 = ibeginx + 10;
        //int ily0 = ibeginy + 10;

        //g_pbackobjectimage->SetROI(ilx0,ily0,iobjw,iobjh);
        //g_pbackobjectimage->ROIImageClear(0);

        //Object(inum);
       // g_pbackobjectimage->ImageDrawPath(m_curobject.getpath(),ilx0,ily0,Qt::red);

        //
        //int icsize = m_curedge.size();
        //for(int ic=0;ic<icsize;ic++)
        //{
        //    m_curedge[ic].linecopyex(*pgetimage,*g_pbackobjectimage,ilx0,ily0);
        //}
        inumw = inumw + igridsum;
    }
}
void findobject::objectsort()
{
    m_rectresults.sort();
}
QRect findobject::getgrid(int inum)
{
    //m_rectgrids
    int igridh = inum/m_icopywgrid;
    int igridw = inum%m_icopywgrid;

    int ibeginx = igridw * m_icopyw;
    int ibeginy = igridh * m_icopyh;

    QRect arect(ibeginx,ibeginy,m_icopyw,m_icopyh);

    return arect;

}
QRect findobject::getgridex(int inum)
{
    if(inum<m_rectgrids.size()
     &&inum>=0)
        return m_rectgrids.getrect(inum);
    else
        return QRect(0,0,0,0);
}
void findobject::setedgeoi(int iw,int ioffset,int iheadtail)
{
    switch (m_iborw)
    {
        case 901:
        {
            int isize = m_cent_h_bw_points_v.size();
            for(int i=0;i<isize;i++)
                m_cent_h_bw_points_v[i].setedgeoi(iw,ioffset,iheadtail);
        }
        break;
        case 902:
        {
            int isize = m_cent_v_bw_points_v.size();
            for(int i=0;i<isize;i++)
                m_cent_v_bw_points_v[i].setedgeoi(iw,ioffset,iheadtail);
        }
        break;
        case 903:
        {
            int isize = m_cent_h_wb_points_v.size();
            for(int i=0;i<isize;i++)
               m_cent_h_wb_points_v[i].setedgeoi(iw,ioffset,iheadtail);
        }
        break;
        case 904:
        {
           int isize = m_cent_v_wb_points_v.size();
           for(int i=0;i<isize;i++)
               m_cent_v_wb_points_v[i].setedgeoi(iw,ioffset,iheadtail);
        }
        break;
    }
}
void findobject::edgeimage(void *pimage)
{
    if(0==pimage)
        return;
    ImageBase *paimage = (ImageBase*)pimage;
    switch (m_iborw)
    {
        case 901:
        {
            int isize = m_cent_h_bw_points_v.size();
            for(int i=0;i<isize;i++)
                m_cent_h_bw_points_v[i].edgeimage(*paimage,0);
         }
         break;
        case 902:
         {
            int isize = m_cent_v_bw_points_v.size();
            for(int i=0;i<isize;i++)
                m_cent_v_bw_points_v[i].edgeimage(*paimage,1);
         }
         break;
        case 903:
        {
            int isize = m_cent_h_wb_points_v.size();
            for(int i=0;i<isize;i++)
                m_cent_h_wb_points_v[i].edgeimage(*paimage,2);
         }
        break;
        case 904:
        {
           int isize = m_cent_v_wb_points_v.size();
           for(int i=0;i<isize;i++)
                m_cent_v_wb_points_v[i].edgeimage(*paimage,3);
        }
         break;
    }
}
void findobject::setcolorstyle(int istyle)
{
    m_istyle=istyle;
}
void findobject::drawshapex(QPainter &painter,
                       QPalette &pal,
                       double dmovx,
                       double dmovy,
                       double dangle,
                       double dzoomx,
                       double dzoomy)
{
    if(show()&0x02)
    {
        m_rectresults.drawshapex(painter,
                                 dmovx,
                                 dmovy,
                                 dangle,
                                 dzoomx,
                                 dzoomy);
    }
    if(show()&0x04)
    {
        m_curedge.drawshape(painter);
        m_curobject.drawshape(painter);
    }
    if(show()&0x08)
    {
        int isize = m_cent_h_bw_points_v.size();
        for(int i=0;i<isize;i++)
            m_cent_h_bw_points_v[i].drawshape(painter);
        isize = m_cent_v_bw_points_v.size();
        for(int i=0;i<isize;i++)
            m_cent_v_bw_points_v[i].drawshape(painter);
        isize = m_cent_h_wb_points_v.size();
        for(int i=0;i<isize;i++)
            m_cent_h_wb_points_v[i].drawshape(painter);
        isize = m_cent_v_wb_points_v.size();
        for(int i=0;i<isize;i++)
            m_cent_v_wb_points_v[i].drawshape(painter);
    }
   painter.save();
    if(0==m_istyle)
        painter.setPen(Qt::darkBlue);
    else if(1==m_istyle)
        painter.setPen(Qt::red);
    else if(2==m_istyle)
        painter.setPen(Qt::green);
    else
        painter.setPen(Qt::blue);
        //);
    QBrush abrush(Qt::transparent, Qt::NoBrush);
    painter.setBrush(abrush);
    QRect arect;
    arect.setX(rect().x()*dzoomx+dmovx);
    arect.setY(rect().y()*dzoomy+dmovy);
    arect.setWidth(rect().width()*dzoomx+dmovx);
    arect.setHeight(rect().height()*dzoomy+dmovy);

    painter.drawRect(arect);
  painter.restore();



}

void findobject::setrelationrectfromresultnum(int inum)
{
    m_irelationresultnum = inum;
}
void findobject::setrelationrectfrom_matchresult(void *pmatch)
{
    m_prelationmatch = (fastmatch*)pmatch;
    if(0!=m_prelationmatch)
    {
        int inum = m_prelationmatch->getresultrects()->size();
        if(m_irelationresultnum<inum)
        {
            m_irelationrect = m_prelationmatch->getresultrects()->getrect(m_irelationresultnum);
        }
    }
}
void findobject::setrelationxy(int iprex1,int iprey1,int iendx1,int iendy1)
{
    m_irelationrect.setLeft(m_irelationrect.left() + iprex1);
    m_irelationrect.setTop(m_irelationrect.top() + iprey1);
    m_irelationrect.setRight(m_irelationrect.right() + iendx1);
    m_irelationrect.setBottom(m_irelationrect.bottom() + iendy1);


}
void findobject::setrelationzoom(double drelationzoomx,double drelationzoomy)
{
    m_irelationrect.setLeft((double)m_irelationrect.left() * drelationzoomx);
    m_irelationrect.setTop((double)m_irelationrect.top() * drelationzoomy);
    m_irelationrect.setRight((double)m_irelationrect.right() * drelationzoomx);
    m_irelationrect.setBottom((double)m_irelationrect.bottom() * drelationzoomy);
}
void findobject::setrelationtorect()
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
void findobject::SetImageROIthre(int ithre)
{
    m_imagethre = ithre;
}
void findobject::SetImageROIincrease(int increase)
{
  m_imagethreincrease=  increase;
}
void findobject::SetImageROIcomparegap(int icomparegap)
{
  m_imagecomparegap=icomparegap;
}
void findobject::SetImageROIfindBorW(int ifindBorW)
{
   m_imagefindBorW=ifindBorW;
}
void findobject::SetImageROIedge_5o7(int i5o7)
{
    m_imageedge_5o7=i5o7;
}
void findobject::ImageROIthre(void * pimage)
{
    ImageBase *pgetimage =(ImageBase *)pimage;
    pgetimage->SetROI(rect().x(),rect().y(),rect().width(),rect().height());
    pgetimage->ROIImageThre(m_imagethre);
}
void findobject::ImageROIedge(void * pimage)
{
    ImageBase *pgetimage =(ImageBase *)pimage;
    pgetimage->SetROI(rect().x(),rect().y(),rect().width(),rect().height());
    if(5==m_imageedge_5o7)
        pgetimage->ROIImage_5Blur_Gap_mud_thre_BW(m_imagethre,m_imagethreincrease,m_imagecomparegap,m_imagefindBorW);
    else if(7==m_imageedge_5o7)
        pgetimage->ROIImage_7Blur_Gap_mud_thre_BW(m_imagethre,m_imagethreincrease,m_imagecomparegap,m_imagefindBorW);

}
void findobject::ImageROIedgeH(void * pimage)
{
    ImageBase *pgetimage =(ImageBase *)pimage;
    pgetimage->SetROI(rect().x(),rect().y(),rect().width(),rect().height());
    if(5==m_imageedge_5o7)
        pgetimage->ROIImage_5Blur_Gap_mud_thre_BW_H(m_imagethre,m_imagethreincrease,m_imagecomparegap,m_imagefindBorW);
    else if(7==m_imageedge_5o7)
        pgetimage->ROIImage_7Blur_Gap_mud_thre_BW_H(m_imagethre,m_imagethreincrease,m_imagecomparegap,m_imagefindBorW);

}
void findobject::shapesetroi(void *pshape)
{
    QShape::shapesetroi(pshape);
}
