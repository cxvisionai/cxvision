#include "plist.h"
#include "CONST_float_vect.H"
#include "CONST_PointDistancetable.H"
#include <string.h>
#include <math.h>
#include <QMessageBox>
#include <QFile>



#define PI 3.1415926
#define _DISTANCE(y,x,x0,y0,R2) (x-x0)*(x-x0)+(y-y0)*(y-y0)-R2;
#define _EDISTANCE(y,x,x0,y0,x1,y1,R2) (x-x0)*(x-x0)+(y-y0)*(y-y0)+(x-x1)*(x-x1)+(y-y1)*(y-y1)-R2;

Plist::Plist()
{
    m_lpPointlist=NULL;
    m_isum=0;
    m_color = GREEN;
}

Plist::~Plist()
{
    DeletList();
}

void Plist::ListMove(int changesumx,int changesumy)
{
    if(!m_lpPointlist)
        return;
    //Need add THROW
    for(int j2=0;j2<m_isum;j2++)
    {
        (*(m_lpPointlist+j2)).x=(*(m_lpPointlist+j2)).x+changesumx;
        (*(m_lpPointlist+j2)).y=(*(m_lpPointlist+j2)).y+changesumy;
    }
    return;
}
//////////////////////////////////////////////////////////////////////////
void Plist::ListRotate(int ix0,int iy0,double dangle)
{
    if(!m_lpPointlist)
        return;
    double dblAngle=PI*dangle/180;
    double sr, cr;
    sr = sin(dblAngle);
    cr = cos(dblAngle);
    int ix,iy;
    for(int j2=0;j2<m_isum;j2++)
    {
        ix = (*(m_lpPointlist+j2)).x - ix0;
        iy = (*(m_lpPointlist+j2)).y - iy0;
        (*(m_lpPointlist+j2)).x = ix0 + (cr * ix - sr * iy);
        (*(m_lpPointlist+j2)).y = iy0 + (sr * ix + cr * iy);
    }
}
//////////////////////////////////////////////////////////////////////////
/*
dblAngle=PI*dblAngle/180;
Point_Shape q;
double sr, cr;
q.x = Bx - Ax;
q.y = By - Ay;

sr = sin(dblAngle); cr = cos(dblAngle);
Bx = Ax + (cr * q.x - sr * q.y);
By = Ay + (sr * q.x + cr * q.y);
*/
/*
void Plist::Draw(HDC hdc,int iLineSize,COLORREF CrlRef)
{
    if(NONECOLOR!=CrlRef)
        for(int WI=0;WI<iLineSize;WI++)
        {
            for(int sum=0;sum<m_isum;sum++)
            {
                if(m_lpPointlist[sum].x>0&&m_lpPointlist[sum].y>0)
                    SetPixel(hdc, m_lpPointlist[sum].x, m_lpPointlist[sum].y,CrlRef );
            }
        }
}
void Plist::DrawBrush(HDC hdc,int ibrush)
{
    HPEN pen = CreatePen(PS_SOLID, ibrush, m_color);
    if (pen != NULL)
        SelectObject(hdc,(HPEN)pen);
    int ihfbsh = ibrush/2;
    for(int sum=0;sum<m_isum;sum++)
    {
        if(m_lpPointlist[sum].x>0
        &&m_lpPointlist[sum].y>0
        &&m_lpPointlist[sum].x<9999
        &&m_lpPointlist[sum].y<9999)
        {
            //for(int ik=0;ik<ibrush;ik++)
            {
                MoveToEx(hdc,m_lpPointlist[sum].x-ihfbsh,m_lpPointlist[sum].y-ihfbsh,(LPPOINT) NULL);
                LineTo(hdc,m_lpPointlist[sum].x+ihfbsh,m_lpPointlist[sum].y-ihfbsh);
                LineTo(hdc,m_lpPointlist[sum].x+ihfbsh,m_lpPointlist[sum].y+ihfbsh);
                LineTo(hdc,m_lpPointlist[sum].x-ihfbsh,m_lpPointlist[sum].y+ihfbsh);
                LineTo(hdc,m_lpPointlist[sum].x-ihfbsh,m_lpPointlist[sum].y-ihfbsh);

            }

        }
    }
    DeleteObject(pen);
}

void Plist::Draw(HDC hdc)
{
    for(int sum=0;sum<m_isum;sum++)
    {
        if(NONECOLOR !=m_color)
            if(m_lpPointlist[sum].x>0&&m_lpPointlist[sum].y>0)
                SetPixel(hdc, m_lpPointlist[sum].x, m_lpPointlist[sum].y,m_color );
    }
}
void Plist::DrawMove(HDC hdc,int ix,int iy)
{
    for(int sum=0;sum<m_isum;sum++)
    {
        if(NONECOLOR !=m_color)
            if(m_lpPointlist[sum].x>0&&m_lpPointlist[sum].y>0)
                SetPixel(hdc, m_lpPointlist[sum].x+ix, m_lpPointlist[sum].y+iy,m_color );
    }
}
void Plist::DrawSpec(HDC hdc,int ix,int iy,int izoom)
{
    for(int sum=0;sum<m_isum;sum++)
    {
        if(NONECOLOR !=m_color)
            if(m_lpPointlist[sum].x>0&&m_lpPointlist[sum].y>0)
                SetPixel(hdc,(m_lpPointlist[sum].x>>izoom)+ix, (m_lpPointlist[sum].y>>izoom)+iy,m_color );
    }
}

void Plist::Draw(HDC hdc,COLORREF CrlRef)
{
    for(int sum=0;sum<m_isum;sum++)
    {
        if(m_lpPointlist[sum].x>0&&m_lpPointlist[sum].y>0)
            SetPixel(hdc, m_lpPointlist[sum].x, m_lpPointlist[sum].y,CrlRef );
    }
}

void Plist::DrawLine(HDC hdc,int iLineSize,COLORREF CrlRef,COLORREF CrlEdge)
{
    // int PenStyle=PS_SOLID;
    // int PenWidth = 1;
    //HGDIOBJ pen;
    //HGDIOBJ brush;
    //int oldDrawingMode=SetROP2(hdc,R2_COPYPEN);//this->m_DrawingMode);
    //pen.CreatePen(PenStyle,PenWidth,m_color);
    //HGDIOBJ pen= SelectObject(hdc,pen);
    //LOGBRUSH brushlog;
    //brushlog.lbColor=m_color;
    //brushlog.lbHatch=HS_BDIAGONAL;
    //brushlog.lbStyle=BS_HOLLOW;
    //brush.CreateBrushIndirect(&brushlog);
    //HGDIOBJ brush= SelectObject(hdc,brush);
    //int OldBkMode= SetBkMode(hdc,TRANSPARENT);
    //COLORREF OldColor=SetBkColor(hdc,RGB(0,255,255));
    // int PenStyle=PS_SOLID;
    // int PenWidth = 1;
    //HPEN pen = ExtCreatePen(PenStyle,PenWidth,m_color)
    if(m_isum<2)
        return;
    HPEN pen = CreatePen(PS_SOLID, iLineSize, CrlRef);
    if (pen != NULL)
        SelectObject(hdc,(HPEN)pen);
    //CPen penFrame(PS_SOLID,iLineSize,CrlRef);

    //SelectObject(hdc,&penFrame);
    //SelectObject(hdc,GetStockObject(DC_PEN));
    //SetDCPenColor(hdc,CrlRef);

    //SetBkMode(hdc,)
    if(m_lpPointlist[0].x>0
        &&m_lpPointlist[0].y>0)
        MoveToEx(hdc,m_lpPointlist[0].x,m_lpPointlist[0].y,(LPPOINT) NULL);

    for(int i=1;i<m_isum;i++)
    {
        if(m_lpPointlist[i].x>0
            &&m_lpPointlist[i].y>0
            &&m_lpPointlist[i].x!=11111
            &&m_lpPointlist[i].y!=11111)
        {
            LineTo(hdc,m_lpPointlist[i].x,m_lpPointlist[i].y);

        }
    }

    for(int i=0;i<m_isum;i++)
    {

        SetPixel(hdc, m_lpPointlist[i].x,m_lpPointlist[i].y,CrlEdge );

    }

    DeleteObject(pen);

}
void Plist::DrawLine2(HDC hdc,int iLineSize,COLORREF CrlRef)
{
    if(m_isum<2)
        return;
    HPEN pen = CreatePen(PS_SOLID, iLineSize, CrlRef);
    if (pen != NULL)
        SelectObject(hdc,(HPEN)pen);


    for(int i=0;i<m_isum;)
    {
        if(m_lpPointlist[i].x>0
            &&m_lpPointlist[i].y>0)
            MoveToEx(hdc,m_lpPointlist[i].x,m_lpPointlist[i].y,(LPPOINT) NULL);
        i = i + 1;
        if(m_lpPointlist[i].x>0
            &&m_lpPointlist[i].y>0)
            LineTo(hdc,m_lpPointlist[i].x,m_lpPointlist[i].y);
        i = i + 1;
    }



    DeleteObject(pen);

}
*/
void Plist::CopyFrom(const Plist& Acopylist)
{
    DeletList();
    m_isum=Acopylist.m_isum;
    m_lpPointlist= new POINT_LP[m_isum];//

    memcpy(m_lpPointlist,Acopylist.m_lpPointlist,sizeof(POINT_LP)*Acopylist.m_isum);
}
void Plist::CopySample(const Plist& Acopylist,int isamplenumbegin,int isamplenumend)
{
    DeletList();
    int itemp;
    if(isamplenumbegin>isamplenumend)
    {
        itemp = isamplenumend;
        isamplenumend = isamplenumbegin;
        isamplenumbegin = isamplenumend;
    }
    if(isamplenumbegin<0)
        isamplenumbegin =0;
    if(isamplenumend > Acopylist.m_isum)
        isamplenumend = Acopylist.m_isum;
    m_isum=isamplenumend - isamplenumbegin +1;

    m_lpPointlist= new POINT_LP[m_isum];

    memcpy(m_lpPointlist,Acopylist.m_lpPointlist + isamplenumbegin,sizeof(POINT_LP)*m_isum);

}
void Plist::CopyBeginSample(const Plist& Acopylist,int isamplenum)
{
    DeletList();
    if(isamplenum > Acopylist.m_isum)
        isamplenum = Acopylist.m_isum;
    m_isum=isamplenum;
    m_lpPointlist= new POINT_LP[m_isum];

    memcpy(m_lpPointlist,Acopylist.m_lpPointlist,sizeof(POINT_LP)*isamplenum);

}
void Plist::CopyEndSample(const Plist& Acopylist,int isamplenum)
{
    DeletList();
    if(isamplenum > Acopylist.m_isum)
        isamplenum = Acopylist.m_isum;
    m_isum = isamplenum;
    m_lpPointlist = new POINT_LP[m_isum];
    int ibeginnum = Acopylist.m_isum - isamplenum;
    memcpy(m_lpPointlist,Acopylist.m_lpPointlist + ibeginnum,sizeof(POINT_LP)*isamplenum);
}

void Plist::ListComb(Plist& Alist,Plist& Blist)
{
    DeletList();
    m_isum=Alist.m_isum+Blist.m_isum;
    m_lpPointlist= new POINT_LP[m_isum];
    memset(m_lpPointlist,0,sizeof(struct POINT_LP)*m_isum);
    memcpy(m_lpPointlist,Alist.m_lpPointlist,sizeof(struct POINT_LP)*Alist.m_isum);
    memcpy((POINT_LP*)(m_lpPointlist)+Alist.m_isum,Blist.m_lpPointlist,sizeof(struct POINT_LP)*Blist.m_isum);
}
void Plist::ListComb(Plist& Alist,Plist& Blist,Plist& Clist)
{
    DeletList();
    m_isum=Alist.m_isum+Blist.m_isum+Clist.m_isum;
    m_lpPointlist= new POINT_LP[m_isum];
    memset(m_lpPointlist,0,sizeof(struct POINT_LP)*m_isum);
    memcpy(m_lpPointlist,Alist.m_lpPointlist,sizeof(struct POINT_LP)*Alist.m_isum);
    memcpy((POINT_LP*)(m_lpPointlist)+Alist.m_isum,Blist.m_lpPointlist,sizeof(struct POINT_LP)*Blist.m_isum);
    memcpy((POINT_LP*)(m_lpPointlist)+Alist.m_isum+Blist.m_isum,Clist.m_lpPointlist,sizeof(POINT_LP)*Clist.m_isum);
}
void Plist::ListComb(Plist& Alist,Plist& Blist,Plist& Clist,Plist& Dlist)
{
    DeletList();
    m_isum=Alist.m_isum+Blist.m_isum+Clist.m_isum+Dlist.m_isum;
    m_lpPointlist= new POINT_LP[m_isum];
    memcpy(m_lpPointlist,Alist.m_lpPointlist,sizeof(POINT_LP)*Alist.m_isum);
    memcpy((POINT_LP*)(m_lpPointlist)+Alist.m_isum,Blist.m_lpPointlist,sizeof(POINT_LP)*Blist.m_isum);
    memcpy((POINT_LP*)(m_lpPointlist)+Alist.m_isum+Blist.m_isum,Clist.m_lpPointlist,sizeof(POINT_LP)*Clist.m_isum);
    memcpy((POINT_LP*)(m_lpPointlist)+Alist.m_isum+Blist.m_isum+Clist.m_isum,Dlist.m_lpPointlist,sizeof(POINT_LP)*Dlist.m_isum);
}

inline void Plist::DeletList()
{
    if(0!=m_isum||NULL!=m_lpPointlist)
    {
        delete[] m_lpPointlist;
        m_lpPointlist=NULL;
        m_isum=0;
    }
}

void Plist::SetColor(QRgb CrlRef)
{
    m_color = CrlRef;
    //if(NULL!=m_lpPointlist)
    //{
    //	for(int i=0;i<m_isum;i++)
    //	{
    //		m_lpPointlist[i].color=CrlRef;
    //	}
    //}
}

void Plist::GetCirclePointList(int i_x,int i_y,int i_x0,int i_y0)
{
    DeletList();
    int i_R_2=(i_x-i_x0)*(i_x-i_x0)+(i_y-i_y0)*(i_y-i_y0);
    int x_=i_x-i_x0;
    int y_=i_y-i_y0;
    float R=sqrt((float)(i_R_2));
    int i_circle_pointsum=R>(int)R?(R+1)*2*PI:R*2*PI;
    int curX,curY;
    m_isum=i_circle_pointsum+1;
    m_lpPointlist= new POINT_LP[m_isum];
    for(int s=0;s<i_circle_pointsum+1;s++)
    {
        m_lpPointlist[s].x=11111;
        m_lpPointlist[s].y=11111;
    }
    //zero memory
    int Direct_y=0,Direct_x=0;
    int D_y=0,D_x=0;
    int x=i_x;
    int y=i_y;
    //get first find point
    int apha=0;
    int igetsum=0;
    int inix=0;
    int iniy=0;

    int iR4 = R*4;
    for(int i=0;i<i_circle_pointsum;i++)
    {
        apha=(CircleStep(apha,x,y,i_x0,i_y0,i_R_2))%360;
        switch(apha)
        {
        case 0:
            y=y+1;
            break;
        case 45:
            x=x+1;
            y=y+1;
            break;
        case 90:
            x=x+1;
            break;
        case 135:
            x=x+1;
            y=y-1;
            break;
        case 180:
            y=y-1;
            break;
        case 225:
            y=y-1;
            x=x-1;
            break;
        case 270:
            x=x-1;
            break;
        case 315:
            x=x-1;
            y=y+1;
            break;
        default:
           QMessageBox::warning(0, ("Error"), ("circle error"));

        }


        m_lpPointlist[i].x=x;
        m_lpPointlist[i].y=y;

        if(0==i)
        {
            inix=m_lpPointlist[i].x;
            iniy=m_lpPointlist[i].y;
        }
        igetsum=igetsum+1;
        apha=(180+apha)%360;
        if(i>iR4)
        {
            if(x==inix&&y==iniy)
                break;
            if(x==inix+1&&y==iniy)
                break;
            if(x==inix-1&&y==iniy)
                break;
            if(x==inix&&y==iniy-1)
                break;
            if(x==inix&&y==iniy+1)
                break;
            if(x==inix-1&&y==iniy-1)
                break;
            if(x==inix+1&&y==iniy+1)

                break;
        }



    }

}
//////////////////////////////////////////////////////////////////////////
//������GetCirclePointList û�� �����Ŀռ�����
//////////////////////////////////////////////////////////////////////////
void Plist::GetCirclePointListA(int i_x,int i_y,int i_x0,int i_y0)
{
    DeletList();
    int i_R_2=(i_x-i_x0)*(i_x-i_x0)+(i_y-i_y0)*(i_y-i_y0);
    int x_=i_x-i_x0;
    int y_=i_y-i_y0;
    float R=sqrt((float)(i_R_2));
    int i_circle_pointsum=R>(int)R?(R+1)*2*PI:R*2*PI;
    int curX,curY;
    m_isum=i_circle_pointsum+1;
    POINT_LP *pPointlist;
    pPointlist= new POINT_LP[m_isum];
    for(int s=0;s<i_circle_pointsum+1;s++)
    {
        pPointlist[s].x=11111;
        pPointlist[s].y=11111;
    }
    //zero memory
    int Direct_y=0,Direct_x=0;
    int D_y=0,D_x=0;
    int x=i_x;
    int y=i_y;
    //get first find point
    int apha=0;//ѭ������
    int igetsum=0;
    int inix=0;
    int iniy=0;
    int iR4 = R*4;
    for(int i=0;i<i_circle_pointsum;i++)
    {
        apha=(CircleStep(apha,x,y,i_x0,i_y0,i_R_2))%360;
        switch(apha)
        {
        case 0:
            y=y+1;
            break;
        case 45:
            x=x+1;
            y=y+1;
            break;
        case 90:
            x=x+1;
            break;
        case 135:
            x=x+1;
            y=y-1;
            break;
        case 180:
            y=y-1;
            break;
        case 225:
            y=y-1;
            x=x-1;
            break;
        case 270:
            x=x-1;
            break;
        case 315:
            x=x-1;
            y=y+1;
            break;
        default:
            ("THIS IS ERROR");
        }
        pPointlist[i].x=x;
        pPointlist[i].y=y;
        if(0==i)
        {
            inix=pPointlist[i].x;
            iniy=pPointlist[i].y;
        }
        igetsum=igetsum+1;
        apha=(180+apha)%360;
        if(i>iR4)
        {
            if(x==inix&&y==iniy)
                break;
            if(x==inix+1&&y==iniy)
                break;
            if(x==inix-1&&y==iniy)
                break;
            if(x==inix&&y==iniy-1)
                break;
            if(x==inix&&y==iniy+1)
                break;
            if(x==inix-1&&y==iniy-1)
                break;
            if(x==inix+1&&y==iniy+1)
                break;
        }
    }
    m_lpPointlist= new POINT_LP[m_isum];
    memcpy(m_lpPointlist,pPointlist,igetsum*sizeof(POINT_LP));
    m_isum=igetsum;
    delete pPointlist;
}

//////////////////////////////////////////////////////////////////////////
//������GetCirclePointList û�� �����Ŀռ�����
//////////////////////////////////////////////////////////////////////////
void Plist::GetRectPointListA(int i_x,int i_y,int i_x0,int i_y0)
{
    DeletList();

    int x_=i_x-i_x0>0?i_x-i_x0:i_x0-i_x;
    int y_=i_y-i_y0>0?i_y-i_y0:i_y0-i_y;
    //float R=sqrt((float)(i_R_2));
    //int i_circle_pointsum=R>(int)R?(R+1)*2*PI:R*2*PI;
    int clen = (2 * x_) +(2 * y_);
    int curX,curY;
    m_isum= clen+1;
    POINT_LP *pPointlist;
    pPointlist= new POINT_LP[m_isum];
    for(int s=0;s<clen+1;s++)
    {
        pPointlist[s].x=11111;
        pPointlist[s].y=11111;
    }
    //zero memory

    int x=i_x<i_x0?i_x:i_x0;
    int y=i_y<i_y0?i_y:i_y0;

    int igetsum=0;
    int inix=0;
    int iniy=0;
    for(int i=0;i<x_;i++)
    {
        pPointlist[i].x=x+i;
        pPointlist[i].y=y;
    }
    for(int i=0;i<y_;i++)
    {
        pPointlist[i].x=x+x_;
        pPointlist[i].y=y+i;
    }
    for(int i=0;i<x_;i++)
    {
        pPointlist[i].x=x+x_-i;
        pPointlist[i].y=y+y_;
    }
    for(int i=0;i<y_;i++)
    {
        pPointlist[i].x=x;
        pPointlist[i].y=y+y_-i;
    }

    m_lpPointlist= new POINT_LP[m_isum];
    memcpy(m_lpPointlist,pPointlist,igetsum*sizeof(POINT_LP));
    m_isum=igetsum;
    delete pPointlist;
}

//////////////////////////////////////////////////////////////////////////
//
void Plist::GetListPointListA(int* px0,int* py0,int isize)
{
    DeletList();

    Plist  edgepart1,edgepart2,edgelist;
    POINT_LP m_PA,m_PB,m_PC ;

    for(int i=0;i<isize-1;i++)
    {
        m_PA.x = px0[i];
        m_PA.y = py0[i];
        m_PB.x = px0[i+1];
        m_PB.y = py0[i+1];

        edgepart1.GetLine(m_PA,m_PB);
        edgepart2.CopyFrom(edgelist);
        edgelist.ListComb(edgepart2,edgepart1);
    }

    CopyFrom(edgelist);
    edgepart2.DeletList();
    edgepart1.DeletList();
    edgelist.DeletList();

}
//////////////////////////////////////////////////////////////////////////
void Plist::GetEllipsePointListA(int i_x,int i_y,int i_x0,int i_y0,int idis)
{
    DeletList();
    int i_R_2=(i_x-i_x0)*(i_x-i_x0)+(i_y-i_y0)*(i_y-i_y0);
    int x_=i_x-i_x0;
    int y_=i_y-i_y0;
    float R=sqrt((float)(i_R_2));
    //�󳤱ߺͶ̱�
    if(idis<R)
        return;
    int iedge1 = idis;
    int iedge2 = 2*sqrt((float)(idis*idis/4 - R*R/4)) ;

    //int iedge1_2= idis*idis;
    //
    int i_circle_pointsum=iedge1*2+iedge2*2;

    //
    int curX,curY;
    m_isum=i_circle_pointsum+1;
    POINT_LP *pPointlist;
    pPointlist= new POINT_LP[m_isum];
    for(int s=0;s<i_circle_pointsum+1;s++)
    {
        pPointlist[s].x=11111;
        pPointlist[s].y=11111;
    }
    //zero memory
    int Direct_y=0,Direct_x=0;
    int D_y=0,D_x=0;
    int x=i_x;
    int y=i_y;
    //get first find point
    int apha=0;//ѭ������
    int igetsum=0;
    int inix=0;
    int iniy=0;
    for(int i=0;i<i_circle_pointsum;i++)
    {
        apha=(EllipseStep(apha,x,y,i_x,i_y,i_x0,i_y0,idis))%360;
        switch(apha)
        {
        case 0:
            y=y+1;
            break;
        case 45:
            x=x+1;
            y=y+1;
            break;
        case 90:
            x=x+1;
            break;
        case 135:
            x=x+1;
            y=y-1;
            break;
        case 180:
            y=y-1;
            break;
        case 225:
            y=y-1;
            x=x-1;
            break;
        case 270:
            x=x-1;
            break;
        case 315:
            x=x-1;
            y=y+1;
            break;
        default:

            QMessageBox::warning(0, ("Error"), (" error2"));
        }
        if(x==inix&&y==iniy)
            break;

        pPointlist[i].x=x;
        pPointlist[i].y=y;
        if(0==i)
        {
            inix=pPointlist[i].x;
            iniy=pPointlist[i].y;
        }
        igetsum=igetsum+1;
        apha=(180+apha)%360;

    }

    m_lpPointlist= new POINT_LP[m_isum];
    memcpy(m_lpPointlist,pPointlist,igetsum*sizeof(POINT_LP));
    m_isum=igetsum;
    delete []pPointlist;
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void Plist::GetArcPointListA(int i_x,int i_y,int i_x0,int i_y0,int ipoinsum)
{

    DeletList();
    int i_R_2=(i_x-i_x0)*(i_x-i_x0)+(i_y-i_y0)*(i_y-i_y0);
    int x_=i_x-i_x0;
    int y_=i_y-i_y0;
    float R=sqrt((float)(i_R_2));
    int i_circle_pointsum=R>(int)R?(R+1)*PI*2:R*PI*2;
    if(i_circle_pointsum<ipoinsum)
        ipoinsum = i_circle_pointsum -1;
    int curX,curY;
    m_isum=i_circle_pointsum+1;
    POINT_LP *pPointlist;
    pPointlist = new POINT_LP[m_isum];
    for(int s=0;s<i_circle_pointsum+1;s++)
    {
        pPointlist[s].x=11111;
        pPointlist[s].y=11111;
    }
    //zero memory
    int Direct_y=0,Direct_x=0;
    int D_y=0,D_x=0;
    int x=i_x;
    int y=i_y;
    //get first find point
    int apha=0;
    int igetsum=0;
    int inix=0;
    int iniy=0;
    for(int i=0;i<ipoinsum;i++)
    {
        apha=(CircleStep(apha,x,y,i_x0,i_y0,i_R_2))%360;
        switch(apha)
        {
        case 0:
            y=y+1;
            break;
        case 45:
            x=x+1;
            y=y+1;
            break;
        case 90:
            x=x+1;
            break;
        case 135:
            x=x+1;
            y=y-1;
            break;
        case 180:
            y=y-1;
            break;
        case 225:
            y=y-1;
            x=x-1;
            break;
        case 270:
            x=x-1;
            break;
        case 315:
            x=x-1;
            y=y+1;
            break;
        default:

            QMessageBox::warning(0, ("Error"), (" error3"));
        }

        pPointlist[i].x=x;
        pPointlist[i].y=y;
        if(0==i)
        {
            inix=pPointlist[i].x;
            iniy=pPointlist[i].y;
        }
        igetsum=igetsum+1;
        apha=(180+apha)%360;

    }

    m_lpPointlist= new POINT_LP[m_isum];
    memcpy(m_lpPointlist,pPointlist,igetsum*sizeof(POINT_LP));
    m_isum=igetsum;
    delete []pPointlist;
}

void Plist::SearchCirclePointList(int i_x0,int i_y0,int i_x,int i_y)
{
    DeletList();
    //Search Condition
    int i_R_2_SearchCondition=(i_x-i_x0)*(i_x-i_x0)+(i_y-i_y0)*(i_y-i_y0);
    int x_=i_x-i_x0;
    int y_=i_y-i_y0;
    float R=sqrt((float)(i_R_2_SearchCondition));
    int i_circle_pointsum=R>(int)R?(R+1)*16:R*16;
    int curX,curY;
    m_isum=i_circle_pointsum+1;
    m_lpPointlist= new POINT_LP[m_isum];
    for(int s=0;s<i_circle_pointsum+1;s++)
    {
        m_lpPointlist[s].x=11111;
        m_lpPointlist[s].y=11111;
    }
    //zero memory
    int Direct_y=0,Direct_x=0;
    int D_y=0,D_x=0;
    int x=i_x;
    int y=i_y;
    //get first find point
    int apha=0;
    int igetsum=0;
    for(int i=0;i<i_circle_pointsum;i++)
    {

        apha=(CircleStep(apha,x,y,i_x0,i_y0,i_R_2_SearchCondition))%360;
        switch(apha)
        {
        case 0:
            y=y+1;
            break;
        case 45:
            x=x+1;
            y=y+1;
            break;
        case 90:
            x=x+1;
            break;
        case 135:
            x=x+1;
            y=y-1;
            break;
        case 180:
            y=y-1;
            break;
        case 225:
            y=y-1;
            x=x-1;
            break;
        case 270:
            x=x-1;
            break;
        case 315:
            x=x-1;
            y=y+1;
            break;
        default:

            QMessageBox::warning(0, ("Error"), (" error4"));
        }
        m_lpPointlist[i].x=x;
        m_lpPointlist[i].y=y;

        apha=(180+apha)%360;
        if(x==x_&&y==y_)
            break;
    }
}

void Plist::SearchCirclePointListA(int i_x0,int i_y0,int i_x,int i_y)
{
    DeletList();
    //Search Condition
    int i_R_2_SearchCondition=(i_x-i_x0)*(i_x-i_x0)+(i_y-i_y0)*(i_y-i_y0);
    int x_=i_x-i_x0;
    int y_=i_y-i_y0;
    float R=sqrt((float)(i_R_2_SearchCondition));
    int i_circle_pointsum=R>(int)R?(R+1)*16:R*16;
    int curX,curY;
    m_isum=i_circle_pointsum+1;

    POINT_LP *pPointlist;
    pPointlist= new POINT_LP[m_isum];

    for(int s=0;s<m_isum;s++)
    {
        pPointlist[s].x=11111;
        pPointlist[s].y=11111;
    }
    //zero memory
    int Direct_y=0,Direct_x=0;
    int D_y=0,D_x=0;
    int x=i_x;
    int y=i_y;
    //get first find point
    int apha=0;
    int igetsum=0;
    for(int i=0;i<i_circle_pointsum;i++)
    {
        //QString debugstr;
        apha=(CircleStep(apha,x,y,i_x0,i_y0,i_R_2_SearchCondition))%360;
        switch(apha)
        {
        case 0:
            y=y+1;
            break;
        case 45:
            x=x+1;
            y=y+1;
            break;
        case 90:
            x=x+1;
            break;
        case 135:
            x=x+1;
            y=y-1;
            break;
        case 180:
            y=y-1;
            break;
        case 225:
            y=y-1;
            x=x-1;
            break;
        case 270:
            x=x-1;
            break;
        case 315:
            x=x-1;
            y=y+1;
            break;
        default:

            QMessageBox::warning(0, ("Error"), (" error5"));
        }
        pPointlist[i].x=x;
        pPointlist[i].y=y;
        igetsum++;
        apha=(180+apha)%360;
        if(x==x_&&y==y_)
            break;
    }
    m_lpPointlist= new POINT_LP[m_isum];
    memcpy(m_lpPointlist,pPointlist,igetsum*sizeof(POINT_LP));
    m_isum=igetsum;
    delete []pPointlist;
}

void Plist::GetPixelPoint(int i_x,int i_y)
{
    DeletList();
    m_isum=1;
    m_lpPointlist= new POINT_LP[m_isum];
    m_lpPointlist[0].x=i_x;
    m_lpPointlist[0].y=i_y;
}

void Plist::GetInsideList(Plist& pointlist)
{
    DeletList();
    pointlist.PointSort();
    pointlist.DeletePointSame();
    POINT_LP*m_lpEdgePointlist=pointlist.m_lpPointlist;
    int m_iEdgeRectPointsum=pointlist.m_isum;
    int MaxSUM;
    MaxSUM=m_iEdgeRectPointsum-1;
    for(int i=0;i<m_iEdgeRectPointsum;i++)
    {
        if(m_lpEdgePointlist[MaxSUM].y!=11111)
            break;
        MaxSUM--;
    }
    //fill draw
    int m_iInsidePointsum=0;

    for(int j=0;j<MaxSUM+1;j++)
    {
        //after sort the list ,collect the x sum between two y coordinate
        if(m_lpEdgePointlist[j].y==m_lpEdgePointlist[j+1].y)
        {
            if(m_lpEdgePointlist[j].x>=0&&m_lpEdgePointlist[j+1].x>=0)
            {
                //the x number between the same y coordinate
                m_isum=m_lpEdgePointlist[j+1].x-m_lpEdgePointlist[j].x>0?
                    m_lpEdgePointlist[j+1].x-m_lpEdgePointlist[j].x:
                -m_lpEdgePointlist[j+1].x+m_lpEdgePointlist[j].x;
                for(int t=1;t<m_isum;t++)
                {
                    m_iInsidePointsum++;
                }
            }
        }
    }
    POINT_LP * m_lpInsidePointlist= new POINT_LP[m_iInsidePointsum];
    m_isum=m_iInsidePointsum;
    int iFillsum = 0;
    int ihsum = 0;
    for(int j=0;j<MaxSUM+1;j++)
    {
        if(m_lpEdgePointlist[j].y==m_lpEdgePointlist[j+1].y)
        {
            if(m_lpEdgePointlist[j].x>=0&&m_lpEdgePointlist[j+1].x>=0)
            {
                ihsum=m_lpEdgePointlist[j+1].x-m_lpEdgePointlist[j].x>0?m_lpEdgePointlist[j+1].x-m_lpEdgePointlist[j].x:-m_lpEdgePointlist[j+1].x+m_lpEdgePointlist[j].x;
                for(int t=1;t<ihsum;t++)
                {
                    m_lpInsidePointlist[iFillsum].y=m_lpEdgePointlist[j].y;
                    m_lpInsidePointlist[iFillsum].x=m_lpEdgePointlist[j].x+t;
                    iFillsum++;
                }
            }
        }
    }
    m_lpPointlist=m_lpInsidePointlist;
    m_isum=m_iInsidePointsum;
}

void Plist::PointSort()
{
    POINT_LP MinX,MinY;
    int sum=m_isum;
    //��ѭ��
    for(int j1=0;j1<sum;j1++)
    {
        //��ѭ��
        for(int i1=0;i1<sum-1;i1++)
        {
            //x���ݱ任
            if(m_lpPointlist[i1].y>m_lpPointlist[i1+1].y)
            {
                MinX=m_lpPointlist[i1+1];
                m_lpPointlist[i1+1]=m_lpPointlist[i1];
                m_lpPointlist[i1]=MinX;
            }
            if(i1==sum)
                sum=sum-1;
        }
    }
    sum=m_isum;
    for(int j2=0;j2<sum;j2++)
    {
        //��ѭ��
        for(int i2=0;i2<sum-1;i2++)
        {
            //x���ݱ任
            if(m_lpPointlist[i2].y==m_lpPointlist[i2+1].y&&m_lpPointlist[i2].x>m_lpPointlist[i2+1].x)
            {
                MinX=m_lpPointlist[i2+1];
                m_lpPointlist[i2+1]=m_lpPointlist[i2];
                m_lpPointlist[i2]=MinX;
            }
            if(i2==sum)
                sum=sum-1;
        }
    }
}
void Plist::GetInsideList_XTX(Plist& pointlist)
{
    DeletList();
    pointlist.PointSort_YX();
    pointlist.DeletePointSame();
    POINT_LP*m_lpEdgePointlist=pointlist.m_lpPointlist;
    int m_iEdgeRectPointsum=pointlist.m_isum;
    int MaxSUM;
    MaxSUM=m_iEdgeRectPointsum-1;
    for(int i=0;i<m_iEdgeRectPointsum;i++)
    {
        if(m_lpEdgePointlist[MaxSUM].y!=11111)
            break;
        MaxSUM--;
    }
    //fill draw
    int m_iInsidePointsum=0;

    for(int j=0;j<MaxSUM+1;j++)
    {
        //after sort the list ,collect the x sum between two y coordinate
        if(m_lpEdgePointlist[j].x==m_lpEdgePointlist[j+1].x)
        {
            if(m_lpEdgePointlist[j].y>=0&&m_lpEdgePointlist[j+1].y>=0)
            {
                //the x number between the same y coordinate
                m_isum=m_lpEdgePointlist[j+1].y-m_lpEdgePointlist[j].y>0?
                    m_lpEdgePointlist[j+1].y-m_lpEdgePointlist[j].y:
                -m_lpEdgePointlist[j+1].y+m_lpEdgePointlist[j].y;
                for(int t=1;t<m_isum;t++)
                {
                    m_iInsidePointsum++;
                }
            }
        }
    }
    POINT_LP * m_lpInsidePointlist= new POINT_LP[m_iInsidePointsum];
    m_isum=m_iInsidePointsum;
    int iFillsum = 0;
    int ihsum = 0;
    for(int j=0;j<MaxSUM+1;j++)
    {
        if(m_lpEdgePointlist[j].x==m_lpEdgePointlist[j+1].x)
        {
            if(m_lpEdgePointlist[j].y>=0&&m_lpEdgePointlist[j+1].y>=0)
            {
                ihsum=m_lpEdgePointlist[j+1].y-m_lpEdgePointlist[j].y>0?m_lpEdgePointlist[j+1].y-m_lpEdgePointlist[j].y:-m_lpEdgePointlist[j+1].y+m_lpEdgePointlist[j].y;
                for(int t=1;t<ihsum;t++)
                {
                    m_lpInsidePointlist[iFillsum].x=m_lpEdgePointlist[j].x;
                    m_lpInsidePointlist[iFillsum].y=m_lpEdgePointlist[j].y+t;
                    iFillsum++;
                }
            }
        }
    }
    m_lpPointlist=m_lpInsidePointlist;
    m_isum=m_iInsidePointsum;
}

void Plist::PointSort_YX()
{
    POINT_LP MinX,MinY;
    int sum=m_isum;
    //��ѭ��
    for(int j1=0;j1<sum;j1++)
    {
        //��ѭ��
        for(int i1=0;i1<sum-1;i1++)
        {
            //x���ݱ任
            if(m_lpPointlist[i1].x>m_lpPointlist[i1+1].x)
            {
                MinX=m_lpPointlist[i1+1];
                m_lpPointlist[i1+1]=m_lpPointlist[i1];
                m_lpPointlist[i1]=MinX;
            }
            if(i1==sum)
                sum=sum-1;
        }
    }
    sum=m_isum;
    for(int j2=0;j2<sum;j2++)
    {
        //��ѭ��
        for(int i2=0;i2<sum-1;i2++)
        {
            //x���ݱ任
            if(m_lpPointlist[i2].x==m_lpPointlist[i2+1].x
                &&m_lpPointlist[i2].y>m_lpPointlist[i2+1].y)
            {
                MinX=m_lpPointlist[i2+1];
                m_lpPointlist[i2+1]=m_lpPointlist[i2];
                m_lpPointlist[i2]=MinX;
            }
            if(i2==sum)
                sum=sum-1;
        }
    }
}
void Plist::Reserve()
{
    POINT_LP temppoint_f,temppoint_b;
    int j=m_isum-1;
    for(int i=0;i<m_isum;i++)
    {
        if(i==j)
            return;
        temppoint_f=m_lpPointlist[i];
        temppoint_b=m_lpPointlist[j];
        m_lpPointlist[i]=temppoint_b;
        m_lpPointlist[j]=temppoint_f;

        j--;
        if(i==j)
            return;
    }

}
void Plist::DeletePointSame()
{
    int sum=m_isum;
    for(int j2=0;j2<sum-1;j2++)
    {
        if(m_lpPointlist[j2].x==m_lpPointlist[j2+1].x
            &&m_lpPointlist[j2].y==m_lpPointlist[j2+1].y&&m_lpPointlist[j2].x!=11111)
        {
            for(int j1=j2+1;j1<sum-1;j1++)
            {
                m_lpPointlist[j1]=m_lpPointlist[j1+1];
            }
            {
                m_lpPointlist[sum-1].x=11111;
                m_lpPointlist[sum-1].y=11111;
            }
            j2--;
        }
    }
}
//////////////////////////////////////////////////////////////////////////

int  Plist::CircleStep(int apha,int x,int y,int i_x0,int i_y0,int  i_R_2)
{
    float Eightsum[8];
    apha=apha%360;
    Eightsum[0]=_DISTANCE(y+1,x,i_x0,i_y0,i_R_2);
    Eightsum[1]=_DISTANCE(y+1,x+1,i_x0,i_y0,i_R_2);
    Eightsum[2]=_DISTANCE(y,x+1,i_x0,i_y0,i_R_2);
    Eightsum[3]=_DISTANCE(y-1,x+1,i_x0,i_y0,i_R_2);
    Eightsum[4]=_DISTANCE(y-1,x,i_x0,i_y0,i_R_2);
    Eightsum[5]=_DISTANCE(y-1,x-1,i_x0,i_y0,i_R_2);
    Eightsum[6]=_DISTANCE(y,x-1,i_x0,i_y0,i_R_2);
    Eightsum[7]=_DISTANCE(y+1,x-1,i_x0,i_y0,i_R_2);
    for(int i=0;i<8;i++)
    {
        //Direction
        if(Eightsum[i]*Eightsum[i+1]<=0)//)||Eightsum[i]>0&&Eightsum[i+1]<0)
        {
            if(Eightsum[i]<0)
                if(i*45!=apha)
                    return i*45;
                else
                    if(Eightsum[i+1]<0)
                        if(i*45+45!=apha)
                            return i*45+45;
        }
        if(7==i)
            if(Eightsum[i]*Eightsum[0]<=0)
                if(Eightsum[i]<0)
                    if(i*45!=apha)
                        return i*45;
                    else
                        if(Eightsum[0]<0)
                            if(apha!=0)
                                return 0;
    }
    return 0;
    //	AfxMessageBox("error");
}

//////////////////////////////////////////////////////////////////////////
//ѭ������
//���������Ĺ켣���� �� Լ������ ���� �켣����
//��Բ��ѭ�� ����Բ�� �� Բ�ϵ� �� �뾶

int  Plist::EllipseStep(int apha,int x,int y,int i_ax,int i_ay,int i_bx,int i_by,int  i_dis)
{
    float Eightsum[8];
    apha=apha%360;
    //Eightsum[0]=T_EDISTANCE(y+1,x,i_ax,i_ay,i_bx,i_by,i_dis);
    //Eightsum[1]=T_EDISTANCE(y+1,x+1,i_ax,i_ay,i_bx,i_by,i_dis);
    //Eightsum[2]=T_EDISTANCE(y,x+1,i_ax,i_ay,i_bx,i_by,i_dis);
    //Eightsum[3]=T_EDISTANCE(y-1,x+1,i_ax,i_ay,i_bx,i_by,i_dis);
    //Eightsum[4]=T_EDISTANCE(y-1,x,i_ax,i_ay,i_bx,i_by,i_dis);
    //Eightsum[5]=T_EDISTANCE(y-1,x-1,i_ax,i_ay,i_bx,i_by,i_dis);
    //Eightsum[6]=T_EDISTANCE(y,x-1,i_ax,i_ay,i_bx,i_by,i_dis);
    //Eightsum[7]=T_EDISTANCE(y+1,x-1,i_ax,i_ay,i_bx,i_by,i_dis);
    for(int i=0;i<8;i++)
    {
        //Direction
        if(Eightsum[i]*Eightsum[i+1]<=0)//)||Eightsum[i]>0&&Eightsum[i+1]<0)
        {
            if(Eightsum[i]<0)
                if(i*45!=apha)
                    return i*45;
                else
                    if(Eightsum[i+1]<0)
                        if(i*45+45!=apha)
                            return i*45+45;
        }
        if(7==i)
            if(Eightsum[i]*Eightsum[0]<=0)
                if(Eightsum[i]<0)
                    if(i*45!=apha)
                        return i*45;
                    else
                        if(Eightsum[0]<0)
                            if(apha!=0)
                                return 0;
    }
    return 0;
    //	AfxMessageBox("error");
}

void Plist::GetXPointList(float iCenterX, float iCenterY,int iPointSize)
{
    int i,j;
    int iRadius = iPointSize/2;
    DeletList();
    m_isum=iPointSize*iPointSize;
    m_lpPointlist= new POINT_LP[m_isum];
}

void Plist::GetLinePointList(int m_x, int m_y,int  m_x0, int m_y0)
{
    DeletList();
    int getsumx=m_x>m_x0 ? m_x-m_x0+1:m_x0-m_x+1;
    int getsumy=m_y>m_y0?m_y-m_y0+1:m_y0-m_y+1;
    float increasex=(float)getsumx/(float)getsumy;
    float increasey=(float)getsumy/(float)getsumx;
    m_isum=getsumx>getsumy?getsumx:getsumy;
    m_lpPointlist= new POINT_LP[m_isum];


    int tim=0;
    if(getsumx>getsumy)
    {
        for(int i=0;i<getsumx;i++)
        {
            m_lpPointlist[i].x=m_x0>=m_x?m_x+i:m_x-i;
            m_lpPointlist[i].y=m_y0>=m_y?m_y+increasey*i:m_y-increasey*i;

        }
    }
    else
    {
        for(int i=0;i<getsumy;i++)
        {
            m_lpPointlist[i].y=m_y0>=m_y?m_y+i:m_y-i;
            m_lpPointlist[i].x=m_x0>=m_x?m_x+increasex*i:m_x-increasex*i;

        }
    }
}
void Plist::GetLinePointList(POINT_LP apoint,POINT_LP bpoint)
{
    int m_x=apoint.x;
    int m_y=apoint.y;
    int m_x0=bpoint.x;
    int m_y0=bpoint.y;
    DeletList();
    int getsumx=m_x>m_x0 ? m_x-m_x0+1:m_x0-m_x+1;
    int getsumy=m_y>m_y0?m_y-m_y0+1:m_y0-m_y+1;
    float increasex=(float)getsumx/(float)getsumy;
    float increasey=(float)getsumy/(float)getsumx;
    m_isum=getsumx>getsumy?getsumx:getsumy;
    m_lpPointlist= new POINT_LP[m_isum];

    int tim=0;
    if(getsumx>getsumy)
    {
        for(int i=0;i<getsumx;i++)
        {
            m_lpPointlist[i].x=m_x0>=m_x?m_x+i:m_x-i;
            m_lpPointlist[i].y=m_y0>=m_y?m_y+increasey*i:m_y-increasey*i;

        }
    }
    else
    {
        for(int i=0;i<getsumy;i++)
        {
            m_lpPointlist[i].y=m_y0>=m_y?m_y+i:m_y-i;
            m_lpPointlist[i].x=m_x0>=m_x?m_x+increasex*i:m_x-increasex*i;

        }
    }
}

void Plist::Translation(unsigned char*FirstBits,int bitsSUM)
{
    int *firstlp;
    switch(bitsSUM)
    {
    case 1:
        break;
    case 2:
        firstlp=(int*)FirstBits;
        break;
    case 3:
        break;
    }
}
//////////////////////////////////////////////////////////////////////////
//��(m_x,m_y) -> (m_x0,m_y0)�ĵ�����
void Plist::GetLine_OLD(int m_x, int m_y,int  m_x0, int m_y0 )
{
    DeletList();
    POINT_LP*m_pPointlist;
    int m_m_isum=0;
    int getsumx,getsumy;
    float increasex,increasey;
    int isignx,isigny;
    int sum=0;
    int interv=0;
    int icurx,icury,inexty,inextx;
    getsumx=m_x>m_x0 ? m_x-m_x0+1:m_x0-m_x+1;
    getsumy=m_y>m_y0?m_y-m_y0+1:m_y0-m_y+1;
    increasex=(float)getsumx/(float)getsumy;
    increasey=(float)getsumy/(float)getsumx;
    m_m_isum=getsumx+getsumy;
    m_pPointlist= new POINT_LP[m_m_isum];
    for(int i=0;i<getsumx+getsumy;i++)
    {
        m_pPointlist[i].x=11111;
        m_pPointlist[i].y=11111;
    }
    icurx=m_x;
    icury=m_y;
    if(m_x0>m_x)
        isignx=1;
    else if(m_x0==m_x)
        isignx=0;
    else
        isignx=-1;

    if(m_y0>m_y)
        isigny=1;
    else if(m_y0==m_y)
        isigny=0;
    else
        isigny=-1;
    for(int i=0;i<getsumx;i++)
    {
        //according the coordinate x increase 1,Get the y coordinate
        m_pPointlist[sum].x=icurx;
        m_pPointlist[sum].y=icury;
        //between the current y coordinate and pre y coordinate ,the x coordinate is same
        //that means (x0,y0)->(x1,y1)
        //x1=x0+1,
        //the point list is (x0 or x0+1,y0 to y1)
        //example (10,20)->(11,26)
        // one result		  :(10,20),(10,21),(10,22),(10,23),(10,24),(10,25),(11,26)
        // another one result :(10,20),(11,21),(11,22),(11,23),(11,24),(11,25),(11,26)
        // maybe one          :(10,20),(10,21),(10,22),(11,23),(11,24),(11,25),(11,26)
        //forture aim: so using a method make it accuracy.if the line's k (y=kx+b)means y/x=k
        //in here i select :middle  (10,20),(10,21),(11,22),(11,23)'end
        //			inextx=m_x0>=m_x?(m_x0==m_x?m_x0:m_x+(i+1)):m_x-(i+1);
        //			inexty=m_y0>=m_y?(m_y0==m_y?m_y0:m_y+increasey*(i+1)):m_y-increasey*(i+1);
        switch(isignx)
        {
        case 0:
            inextx=m_x;
            break;
        case 1:
            inextx=m_x+(i+1);
            break;
        case -1:
            inextx=m_x-(i+1);
            break;
        default:
            break;
        }
        switch(isigny)
        {
        case 0:
            inexty=m_y;
            break;
        case 1:
            inexty=m_y+increasey*(i+1);
            break;
        case -1:
            inexty=m_y-increasey*(i+1);
            break;
        default:
            break;
        }

        interv=inexty-icury>0?(inexty-icury-1):0==inexty-icury?0:(inexty-icury)+1 ;
        if(interv>0)
        {
            for(int j=0;j<interv;j++)
            {
                // middle fill the interv
                if(j<(interv>>1))
                {
                    sum++;
                    m_pPointlist[sum].x=icurx;
                    m_pPointlist[sum].y=icury+j+1;
                }
                else
                {
                    sum++;
                    m_pPointlist[sum].x=inextx;
                    m_pPointlist[sum].y=icury+j+1;
                }

            }
        }
        else
        {
            if(0==interv)
            {
                //no point
            }
            else
            {
                for(int j=0;j<(-interv);j++)
                {
                    // middle fill the interv
                    if(j<(-interv)>>1)
                    {
                        sum++;
                        m_pPointlist[sum].x=icurx;
                        m_pPointlist[sum].y=icury-j-1;
                    }
                    else
                    {
                        sum++;
                        m_pPointlist[sum].x=inextx;
                        m_pPointlist[sum].y=icury-j-1;
                    }

                }
            }
        }
        icury=inexty;
        icurx=inextx;
        sum++;
    }
    m_lpPointlist=  new POINT_LP[sum];
    m_isum=sum;
    memcpy(m_lpPointlist,m_pPointlist,sizeof(POINT_LP)*(sum));
    delete []m_pPointlist;
}

void Plist::GetLine_OLD(POINT_LP apoint,POINT_LP bpoint)
{
    int m_x=apoint.x;
    int m_y=apoint.y;
    int m_x0=bpoint.x;
    int m_y0=bpoint.y;
    DeletList();
    POINT_LP*m_pPointlist;
    int m_m_isum=0;
    int getsumx,getsumy;
    float increasex,increasey;
    int isignx,isigny;
    int sum=0;
    int interv=0;
    int icurx,icury,inexty,inextx;
    getsumx=m_x>m_x0 ? m_x-m_x0+1:m_x0-m_x+1;
    getsumy=m_y>m_y0?m_y-m_y0+1:m_y0-m_y+1;
    increasex=(float)getsumx/(float)getsumy;
    increasey=(float)getsumy/(float)getsumx;
    m_m_isum=getsumx+getsumy;
    m_pPointlist= new POINT_LP[m_m_isum];
    for(int i=0;i<getsumx+getsumy;i++)
    {
        m_pPointlist[i].x=11111;
        m_pPointlist[i].y=11111;
    }
    icurx=m_x;
    icury=m_y;
    if(m_x0>m_x)
        isignx=1;
    else
        if(m_x0==m_x)
            isignx=0;
        else
            isignx=-1;

    if(m_y0>m_y)
        isigny=1;
    else
        if(m_y0==m_y)
            isigny=0;
        else
            isigny=-1;
    for(int i=0;i<getsumx;i++)
    {
        //according the coordinate x increase 1,Get the y coordinate

        m_pPointlist[sum].x=icurx;
        m_pPointlist[sum].y=icury;
        //between the current y coordinate and pre y coordinate ,the x coordinate is same
        //that means (x0,y0)->(x1,y1)
        //x1=x0+1,
        //the point list is (x0 or x0+1,y0 to y1)
        //example (10,20)->(11,26)
        // one result		  :(10,20),(10,21),(10,22),(10,23),(10,24),(10,25),(11,26)
        // another one result :(10,20),(11,21),(11,22),(11,23),(11,24),(11,25),(11,26)
        // maybe one          :(10,20),(10,21),(10,22),(11,23),(11,24),(11,25),(11,26)
        //forture aim: so using a method make it accuracy.if the line's k (y=kx+b)means y/x=k
        //in here i select :middle  (10,20),(10,21),(11,22),(11,23)'end
        //			inextx=m_x0>=m_x?(m_x0==m_x?m_x0:m_x+(i+1)):m_x-(i+1);
        //			inexty=m_y0>=m_y?(m_y0==m_y?m_y0:m_y+increasey*(i+1)):m_y-increasey*(i+1);
        switch(isignx)
        {
        case 0:
            inextx=m_x;
            break;
        case 1:
            inextx=m_x+(i+1);
            break;
        case -1:
            inextx=m_x-(i+1);
            break;
        default:
            break;
        }
        switch(isigny)
        {
        case 0:
            inexty=m_y;
            break;
        case 1:
            inexty=m_y+increasey*(i+1);
            break;
        case -1:
            inexty=m_y-increasey*(i+1);
            break;
        default:
            break;
        }

        interv=inexty-icury>0?(inexty-icury-1):0==inexty-icury?0:(inexty-icury)+1 ;
        if(interv>0)
        {
            for(int j=0;j<interv;j++)
            {
                // middle fill the interv
                if(j<(interv>>1))
                {
                    sum++;
                    m_pPointlist[sum].x=icurx;
                    m_pPointlist[sum].y=icury+j+1;
                }
                else
                {
                    sum++;
                    m_pPointlist[sum].x=inextx;
                    m_pPointlist[sum].y=icury+j+1;
                }

            }
        }
        else
        {
            if(0==interv)
            {
                //no point
            }
            else
            {
                for(int j=0;j<(-interv);j++)
                {
                    // middle fill the interv
                    if(j<(-interv)>>1)
                    {
                        sum++;
                        m_pPointlist[sum].x=icurx;
                        m_pPointlist[sum].y=icury-j-1;
                    }
                    else
                    {
                        sum++;
                        m_pPointlist[sum].x=inextx;
                        m_pPointlist[sum].y=icury-j-1;
                    }

                }
            }
        }
        icury=inexty;
        icurx=inextx;
        sum++;
    }
    m_lpPointlist= new POINT_LP[sum];
    m_isum=sum;
    memcpy(m_lpPointlist,m_pPointlist,sizeof(POINT_LP)*(sum));
     delete []m_pPointlist;
}






void Plist::GetLine(int m_x, int m_y,int  m_x0, int m_y0 )
{

    DeletList();
    //�м��ĵ������˵Ķ˵�
    int getsumx=m_x>m_x0 ? m_x-m_x0+1:m_x0-m_x+1;
    int getsumy=m_y>m_y0 ? m_y-m_y0+1:m_y0-m_y+1;

    //ASSERT(getsumx<640&&getsumy<640);
    if(getsumx>640||getsumy>640)
    {
        GetLine_OLD(m_x,m_y,m_x0,m_y0);
        return;
    }
    //
    int icx = getsumx /getsumy;
    int icy = getsumy /getsumx;
    //
    float increasex=(float)getsumx/(float)getsumy;
    float increasey=(float)getsumy/(float)getsumx;
    //
    float isx = increasex - icx;
    float isy = increasey - icy;
    //
    int iratex = isx/0.001;
    int iratey = isy/0.001;


    getsumx = getsumx -1;
    getsumy = getsumy -1;

    //
    m_isum=getsumx>getsumy?getsumx:getsumy;
    //
    m_lpPointlist=new POINT_LP[m_isum];

    if(0==m_x-m_x0||0==m_y-m_y0)
    {
        iratey = 0;
        icy = 0;
        iratex = 0;
        icx = 0;
    }

    //���ϲ���
    int tim=0;
    //ʵ��ֱ���ε����ϣ������ֿ���ɨ��x��������Ӧ�ĵ�
    if(getsumx>getsumy)
    {
        if(m_x0>=m_x)
        {
            if(m_y0>=m_y)
            {
                for(int i=0;i<getsumx;i++)
                {
                    m_lpPointlist[i].x= m_x + i ;
                    m_lpPointlist[i].y= m_y+ G_ifloatvect[iratey][i]  ;//+increasey*i;//
                }
            }
            else
            {
                for(int i=0;i<getsumx;i++)
                {
                    m_lpPointlist[i].x= m_x+i ;
                    m_lpPointlist[i].y= m_y- G_ifloatvect[iratey][i]  ;//-increasey*i;//
                }
            }
        }
        else
        {
            if(m_y0>=m_y)
            {
                for(int i=0;i<getsumx;i++)
                {
                    m_lpPointlist[i].x= m_x-i ;
                    m_lpPointlist[i].y= m_y+ G_ifloatvect[iratey][i] ;//+increasey*i;//
                }
            }
            else
            {
                for(int i=0;i<getsumx;i++)
                {
                    m_lpPointlist[i].x= m_x-i ;
                    m_lpPointlist[i].y= m_y- G_ifloatvect[iratey][i] ;//-increasey*i; //
                }
            }
        }
        //for(int i=0;i<getsumx;i++)
        //{
        //	m_lpPointlist[i].x=m_x0>=m_x?m_x+i:m_x-i;
        //	m_lpPointlist[i].y=m_y0>=m_y?m_y+increasey*i:m_y-increasey*i;
        //}
    }
    else if(getsumx<getsumy)
    {
        if(m_x0>=m_x)
        {
            if(m_y0>=m_y)
            {
                for(int i=0;i<getsumy;i++)
                {
                    m_lpPointlist[i].y= m_y+i ;
                    m_lpPointlist[i].x= m_x+ G_ifloatvect[iratex][i] ;//+increasex*i;//
                }
            }
            else
            {
                for(int i=0;i<getsumy;i++)
                {
                    m_lpPointlist[i].y= m_y-i;
                    m_lpPointlist[i].x= m_x+ G_ifloatvect[iratex][i]  ;//+increasex*i;//
                }
            }
        }
        else
        {
            if(m_y0>=m_y)
            {
                for(int i=0;i<getsumy;i++)
                {
                    m_lpPointlist[i].y= m_y + i ;
                    m_lpPointlist[i].x= m_x- G_ifloatvect[iratex][i]  ;//-increasex*i;//
                }
            }
            else
            {
                for(int i=0;i<getsumy;i++)
                {
                    m_lpPointlist[i].y= m_y - i;
                    m_lpPointlist[i].x= m_x- G_ifloatvect[iratex][i] ;//-increasex*i;//
                }
            }
        }
        //for(int i=0;i<getsumy;i++)
        //{
        //	m_lpPointlist[i].y=m_y0>=m_y?m_y+i:m_y-i;
        //	m_lpPointlist[i].x=m_x0>=m_x?m_x+increasex*i:m_x-increasex*i;
        //}
    }
    else if(getsumx==getsumy)
    {
        if(m_x0>=m_x)
        {
            if(m_y0>=m_y)
            {
                for(int i=0;i<getsumy;i++)
                {
                    m_lpPointlist[i].y= m_y+i ;
                    m_lpPointlist[i].x= m_x+i ;//+increasex*i;//
                }
            }
            else
            {
                for(int i=0;i<getsumy;i++)
                {
                    m_lpPointlist[i].y= m_y-i ;
                    m_lpPointlist[i].x= m_x+i ;//+increasex*i;//
                }
            }
        }
        else
        {
            if(m_y0>=m_y)
            {
                for(int i=0;i<getsumy;i++)
                {
                    m_lpPointlist[i].y= m_y + i ;
                    m_lpPointlist[i].x= m_x - i ;//-increasex*i;//
                }
            }
            else
            {
                for(int i=0;i<getsumy;i++)
                {
                    m_lpPointlist[i].y= m_y - i ;
                    m_lpPointlist[i].x= m_x - i ;//-increasex*i;//
                }
            }
        }
        //for(int i=0;i<getsumy;i++)
        //{
        //	m_lpPointlist[i].y=m_y0>=m_y?m_y+i:m_y-i;
        //	m_lpPointlist[i].x=m_x0>=m_x?m_x+increasex*i:m_x-increasex*i;
        //}
    }
    //
}

void Plist::GetLine(POINT_LP apoint,POINT_LP bpoint)
{
    int m_x=apoint.x;
    int m_y=apoint.y;
    int m_x0=bpoint.x;
    int m_y0=bpoint.y;
    DeletList();
    //�м��ĵ������˵Ķ˵�
    int getsumx=m_x>m_x0 ? m_x-m_x0+1:m_x0-m_x+1;
    int getsumy=m_y>m_y0 ? m_y-m_y0+1:m_y0-m_y+1;
    //ASSERT(getsumx<640&&getsumy<640);
    if(getsumx>640||getsumy>640)
    {
        GetLine_OLD(m_x,m_y,m_x0,m_y0);
        return;
    }
    //
    int icx = getsumx /getsumy;
    int icy = getsumy /getsumx;
    //
    float increasex=(float)getsumx/(float)getsumy;
    float increasey=(float)getsumy/(float)getsumx;
    //
    float isx = increasex - icx;
    float isy = increasey - icy;
    //
    int iratex = isx/0.001;
    int iratey = isy/0.001;


    getsumx = getsumx -1;
    getsumy = getsumy -1;

    //
    m_isum=getsumx>getsumy?getsumx:getsumy;
    //
    m_lpPointlist=new POINT_LP[m_isum];

    if(0==m_x-m_x0||0==m_y-m_y0)
    {
        iratey = 0;
        icy = 0;
        iratex = 0;
        icx = 0;
    }

    //���ϲ���
    int tim=0;
    //ʵ��ֱ���ε����ϣ������ֿ���ɨ��x��������Ӧ�ĵ�
    if(getsumx>getsumy)
    {
        if(m_x0>=m_x)
        {
            if(m_y0>=m_y)
            {
                for(int i=0;i<getsumx;i++)
                {
                    m_lpPointlist[i].x= m_x + i ;
                    m_lpPointlist[i].y= m_y+ G_ifloatvect[iratey][i]  ;//+increasey*i;//
                }
            }
            else
            {
                for(int i=0;i<getsumx;i++)
                {
                    m_lpPointlist[i].x= m_x+i ;
                    m_lpPointlist[i].y= m_y- G_ifloatvect[iratey][i]  ;//-increasey*i;//
                }
            }
        }
        else
        {
            if(m_y0>=m_y)
            {
                for(int i=0;i<getsumx;i++)
                {
                    m_lpPointlist[i].x= m_x-i ;
                    m_lpPointlist[i].y= m_y+ G_ifloatvect[iratey][i] ;//+increasey*i;//
                }
            }
            else
            {
                for(int i=0;i<getsumx;i++)
                {
                    m_lpPointlist[i].x= m_x-i ;
                    m_lpPointlist[i].y= m_y- G_ifloatvect[iratey][i] ;//-increasey*i; //
                }
            }
        }
    }
    else if(getsumx<getsumy)
    {
        if(m_x0>=m_x)
        {
            if(m_y0>=m_y)
            {
                for(int i=0;i<getsumy;i++)
                {
                    m_lpPointlist[i].y= m_y+i ;
                    m_lpPointlist[i].x= m_x+ G_ifloatvect[iratex][i] ;//+increasex*i;//
                }
            }
            else
            {
                for(int i=0;i<getsumy;i++)
                {
                    m_lpPointlist[i].y= m_y-i;
                    m_lpPointlist[i].x= m_x+ G_ifloatvect[iratex][i]  ;//+increasex*i;//
                }
            }
        }
        else
        {
            if(m_y0>=m_y)
            {
                for(int i=0;i<getsumy;i++)
                {
                    m_lpPointlist[i].y= m_y + i ;
                    m_lpPointlist[i].x= m_x- G_ifloatvect[iratex][i]  ;//-increasex*i;//
                }
            }
            else
            {
                for(int i=0;i<getsumy;i++)
                {
                    m_lpPointlist[i].y= m_y - i;
                    m_lpPointlist[i].x= m_x- G_ifloatvect[iratex][i] ;//-increasex*i;//
                }
            }
        }
        //for(int i=0;i<getsumy;i++)
        //{
        //	m_lpPointlist[i].y=m_y0>=m_y?m_y+i:m_y-i;
        //	m_lpPointlist[i].x=m_x0>=m_x?m_x+increasex*i:m_x-increasex*i;
        //}
    }
    else if(getsumx==getsumy)
    {
        if(m_x0>=m_x)
        {
            if(m_y0>=m_y)
            {
                for(int i=0;i<getsumy;i++)
                {
                    m_lpPointlist[i].y= m_y+i ;
                    m_lpPointlist[i].x= m_x+i ;//+increasex*i;//
                }
            }
            else
            {
                for(int i=0;i<getsumy;i++)
                {
                    m_lpPointlist[i].y= m_y-i ;
                    m_lpPointlist[i].x= m_x+i ;//+increasex*i;//
                }
            }
        }
        else
        {
            if(m_y0>=m_y)
            {
                for(int i=0;i<getsumy;i++)
                {
                    m_lpPointlist[i].y= m_y + i ;
                    m_lpPointlist[i].x= m_x - i ;//-increasex*i;//
                }
            }
            else
            {
                for(int i=0;i<getsumy;i++)
                {
                    m_lpPointlist[i].y= m_y - i ;
                    m_lpPointlist[i].x= m_x - i ;//-increasex*i;//
                }
            }
        }
    }
}

void Plist::Mirror(int ix, int iy)
{


    POINT_LP* pTempList = NULL;
    int itempsum = 0;

    itempsum= m_isum*2;
    pTempList= new POINT_LP[m_isum];

    memcpy(pTempList,m_lpPointlist,sizeof(struct POINT_LP)*m_isum);
    POINT_LP * pPoint = NULL;


    for(int i=0;i<m_isum;i++)
    {
        pPoint = m_lpPointlist + i ;
        pPoint->x = pPoint->x + ix;
        pPoint->y = pPoint->y + iy;
    }
    memcpy((POINT_LP*)(pTempList)+m_isum,m_lpPointlist,sizeof(struct POINT_LP)*m_isum);

    DeletList();

    m_lpPointlist=pTempList;
    m_isum=itempsum;
}

void Plist::Add(Plist*plist)
{
    if(NULL==plist)
        return;
    int isize = plist->getisum();
    if(0==isize)
        return;

    POINT_LP* pTempList = NULL;
    int itempsum = 0;

    itempsum= m_isum+isize;
    pTempList= new POINT_LP[m_isum];

    memcpy(pTempList, getalloc(),sizeof(struct POINT_LP)*m_isum);
    memcpy((POINT_LP*)(pTempList)+m_isum,plist->getalloc(),sizeof(struct POINT_LP)*isize);

    DeletList();

    m_lpPointlist=pTempList;
    m_isum=itempsum;
}
void Plist::SetListNum(int inum)
{
    DeletList();

    m_isum=inum;
    POINT_LP *pPointlist;
    pPointlist= new POINT_LP[m_isum];
    //ZeroMemory(pPointlist,m_isum* sizeof(POINT_LP));
    memset(pPointlist,0,m_isum* sizeof(POINT_LP));
    m_lpPointlist = pPointlist;
}
void Plist::PushPoint(int inum,int ix, int iy)
{
    if(inum>=m_isum)
        return;
    (*(m_lpPointlist+inum)).x = ix;
    (*(m_lpPointlist+inum)).y = iy;
}
void Plist::ReSize(int inum)
{
    POINT_LP *pPointlist = new POINT_LP[inum];

    if(inum>=m_isum)
        memcpy(pPointlist,m_lpPointlist,sizeof(POINT_LP)*m_isum);
    else
        memcpy(pPointlist,m_lpPointlist,sizeof(POINT_LP)*inum);
    DeletList();
    m_isum=inum;
    m_lpPointlist = pPointlist;
}
void Plist::AddSize(int inum)
{

    if(inum<=m_isum)
        return;

    POINT_LP *pPointlist = new POINT_LP[inum];
    memcpy(pPointlist,m_lpPointlist,sizeof(POINT_LP)*m_isum);
    DeletList();
    m_isum=inum;
    m_lpPointlist = pPointlist;
}

void Plist::SaveList(const char * pchar)
{

  //  QString fn = getlocationstringx(filenames);

    QFile f(pchar);

    if ( !f.open(QFile::WriteOnly) )
    {
        return;
    }
    f.write((char*)m_lpPointlist,m_isum*sizeof(POINT_LP));

   f.close();
}
void Plist::LoadList(const char * pchar)
{
    DeletList();
    //	int ishit = (sizeof(POINT_LP) )/4 ;
    QFile f(pchar);

    if ( !f.open(QFile::ReadOnly) )
    {
        return;
    }
    QByteArray d = f.readAll();

    f.close();

    m_isum = d.size()/sizeof(POINT_LP) ;

    m_lpPointlist = new POINT_LP[m_isum];

    memcpy(m_lpPointlist,d.data(),sizeof(POINT_LP)*m_isum);

}
//////////////////////////////////////////////////////////////////////////
//������ĩ�����л�
//������ֻ������С��������
//������������һ��ֻ����ĩ����
void Plist::BeginPointList(Plist* beginlist)
{
    PointSort();
    //////////////////////////////////////////////////////////////////////////
    //
    int ibeginy = m_lpPointlist[0].y;
    int iendy = m_lpPointlist[m_isum-1].y;

    int ibeginlinesum = iendy - ibeginy +1;
    int iendlinesum = ibeginlinesum;

    beginlist->DeletList();
    beginlist->SetListNum(ibeginlinesum);
    POINT_LP *pbeginlist =beginlist->getalloc();

    int ix0 = 0;
    int iy0 = 0;
    int ix1 = 0;
    int iy1 = 0;
    int ibeginx = 0;
    int iendx =0;
    int icury = 0;
    int icurnum = 0;
    ibeginx = m_lpPointlist[0].x;
    icury =  m_lpPointlist[0].y;
    int italynum = 0;

    pbeginlist[0].x = ibeginx;
    pbeginlist[0].y = ibeginy;


    iy0 = m_lpPointlist[icurnum].y;
    for(int i= ibeginy;i<iendy+1;i++ )
    {
        italynum = 0;//��ǰ�д��ڵ���Ŀ
        ibeginx = 0;
        iendx = 0;
        while(iy0<i)
        {
            if(icurnum<m_isum)
                icurnum++;
            else
                return;
            iy0 = m_lpPointlist[icurnum].y;

        }
        if(iy0==i)
        {

            ibeginx = m_lpPointlist[icurnum].x;
            iendx = ibeginx;
            //while(iy0 == i )
            //{
            //	if(m_lpPointlist[icurnum].x>iendx)
            //	{
            //		iendx =  m_lpPointlist[icurnum].x;
            //	}
            //	iy0 = m_lpPointlist[icurnum].y;
            //	icurnum++;
            //}

            pbeginlist[i-ibeginy].x = ibeginx;
            pbeginlist[i-ibeginy].y = i;

        }
        else if(iy0>i)
        {//�ϵ���
            pbeginlist[i-ibeginy].x = pbeginlist[i-ibeginy-1].x ;
            pbeginlist[i-ibeginy].y = pbeginlist[i-ibeginy-1].y;
            //pendlist[i-ibeginy].x = pendlist[i-ibeginy-1].x;
            //pendlist[i-ibeginy].y = pendlist[i-ibeginy-1].y ;

        }

    }
}
void Plist::EndPointList(Plist* endlist)
{
    PointSort();
    //////////////////////////////////////////////////////////////////////////
    //
    int ibeginy = m_lpPointlist[0].y;
    int iendy = m_lpPointlist[m_isum-1].y;

    int ibeginlinesum = iendy - ibeginy +1;
    int iendlinesum = ibeginlinesum;

    endlist->DeletList();
    endlist->SetListNum(ibeginlinesum);
    POINT_LP *pendlist =endlist->getalloc();

    int ix0 = 0;
    int iy0 = 0;
    int ix1 = 0;
    int iy1 = 0;
    int ibeginx = 0;
    int iendx =0;
    int icury = 0;
    int icurnum = 0;
    ibeginx = m_lpPointlist[0].x;
    icury =  m_lpPointlist[0].y;
    int italynum = 0;

    pendlist[0].x = ibeginx;
    pendlist[0].y = ibeginy;


    iy0 = m_lpPointlist[icurnum].y;
    for(int i= ibeginy;i<iendy+1;i++ )
    {
        italynum = 0;//��ǰ�д��ڵ���Ŀ
        ibeginx = 0;
        iendx = 0;
        while(iy0<i)
        {
            if(icurnum<m_isum)
                icurnum++;
            else
                return;
            iy0 = m_lpPointlist[icurnum].y;

        }
        if(iy0==i)
        {

            ibeginx = m_lpPointlist[icurnum].x;
            iendx = ibeginx;
            while(iy0 == i )
            {
                if(m_lpPointlist[icurnum].x>iendx)
                {
                    iendx =  m_lpPointlist[icurnum].x;
                }
                pendlist[i-ibeginy].x = iendx;
                pendlist[i-ibeginy].y = i;
                iy0 = m_lpPointlist[icurnum].y;
                icurnum++;
            }

            //pbeginlist[i-ibeginy].x = ibeginx;
            //pbeginlist[i-ibeginy].y = i;

        }
        else if(iy0>i)
        {//�ϵ���
            pendlist[i-ibeginy].x = pendlist[i-ibeginy-1].x ;
            pendlist[i-ibeginy].y = pendlist[i-ibeginy-1].y;
            //pendlist[i-ibeginy].x = pendlist[i-ibeginy-1].x;
            //pendlist[i-ibeginy].y = pendlist[i-ibeginy-1].y ;

        }

    }
}

void Plist::GetMinXY(int &ix,int &iy)
{
    if(m_isum<=0)
        return;
    int iminx =9999;
    int iminy =9999;
    for(int i=0;i<m_isum;i++)
    {
        if(iminx>m_lpPointlist[i].x)
            iminx = m_lpPointlist[i].x;
        if(iminy>m_lpPointlist[i].y)
            iminy = m_lpPointlist[i].y;
    }
    ix = iminx;
    iy = iminy;
}
void Plist::ZeroPosList()
{
    int iminx;
    int iminy;
    GetMinXY(iminx,iminy);
    ListMove(-iminx,-iminy);
}
//////////////////////////////////////////////////////////////////////////
//
void Plist::LineVH(Plist*plist,int idistance,double doffset)
{

    if(plist==NULL)
        return;


    DeletList();


    POINT_LP* pTempList = plist->getalloc();
    int itempsum = plist->getisum();
    int iAx = 0;
    int iAy = 0;
    int iBx = 0;
    int iBy = 0;

    iAx = pTempList[0].x;
    iAy = pTempList[0].y;


    m_isum = 2*(itempsum - 1) ;
    POINT_LP *pPointlist;
    pPointlist= new POINT_LP[m_isum];
    m_lpPointlist = pPointlist;
    int irunnum = 0;
    int ixgap = 0;
    int iygap = 0;
    float ddistance = 0;
    double drate = 0;
    double dx = 0;
    double dy = 0;
    for(int i=1;i<itempsum;i++)
    {
        iBx = pTempList[i].x;
        iBy = pTempList[i].y;
        ixgap =  iBx-iAx ;
        iygap =  iBy-iAy ;
        ixgap = ixgap >0?ixgap:-ixgap;
        iygap = iygap >0?iygap:-iygap;

        ddistance =  PointDistance[ixgap][iygap];
        drate = idistance/ddistance;
        dx = -1*(iBy-iAy)*drate ;
        dy = (iBx - iAx)*drate ;



        m_lpPointlist[irunnum].x = iAx + dx*doffset;
        m_lpPointlist[irunnum].y = iAy + dy*doffset;
        irunnum = irunnum + 1;
        m_lpPointlist[irunnum].x = iAx - dx*(1-doffset);
        m_lpPointlist[irunnum].y = iAy - dy*(1-doffset);
        irunnum = irunnum + 1;

        iAx = iBx;
        iAy = iBy;

    }
}
//////////////////////////////////////////////////////////////////////////
//
void Plist::RectFilter(int ix0,int iy0,int ix1,int iy1)
{

    POINT_LP* pTempList = NULL;

    int itempsum = 0;

    for(int i=0;i<m_isum;i++)
    {
        if(m_lpPointlist[i].x<ix0
            ||m_lpPointlist[i].x>ix1
            ||m_lpPointlist[i].y<iy0
            ||m_lpPointlist[i].y>iy1)
        {
            itempsum = itempsum + 1;
        }
    }

    pTempList= new POINT_LP[m_isum];

    int irunnum = 0;
    for(int i=0;i<m_isum;i++)
    {
        if(m_lpPointlist[i].x<ix0
            ||m_lpPointlist[i].x>ix1
            ||m_lpPointlist[i].y<iy0
            ||m_lpPointlist[i].y>iy1)
        {
            pTempList[irunnum].x =  m_lpPointlist[i].x;
            pTempList[irunnum].y =  m_lpPointlist[i].y;

            irunnum = irunnum + 1;
        }

    }
    DeletList();

    m_lpPointlist=pTempList;
    m_isum=itempsum;
}
void Plist::Draw(QPainter &painter,QPen &pen)
{
    painter.setPen(pen);
    painter.drawPoints((QPoint *)m_lpPointlist, m_isum);
}


