#ifndef PLIST_H
#define PLIST_H

#include <vector>
#include <string>
#include <QRgb>
#include <QPainter>


//define color const
const QRgb CLOUDBLUE = qRgb(128, 184, 223);
const QRgb WHITE = qRgb(255, 255, 255);
const QRgb BLACK = qRgb(1, 1, 1);
const QRgb DKGRAY = qRgb(128, 128, 128);
const QRgb LTGRAY = qRgb(192, 192, 192);
const QRgb YELLOW = qRgb(255, 255, 0);
const QRgb DKYELLOW = qRgb(128, 128, 0);
const QRgb DKYELLOWM = qRgb(88, 88, 0);
const QRgb RED = qRgb(255, 0, 0);
const QRgb DKRED = qRgb(128, 0, 0);
const QRgb BLUE = qRgb(0, 0, 255);
const QRgb DKBLUE = qRgb(0, 0, 128);
const QRgb CYAN = qRgb(0, 255, 255);
const QRgb DKCYAN = qRgb(0, 128, 128);
const QRgb GREEN = qRgb(0, 255, 0);
const QRgb DKGREEN = qRgb(0, 128, 0);
const QRgb MAGENTA = qRgb(255, 0, 255);
const QRgb DKMAGENTA = qRgb(128, 0, 128);
const QRgb NONECOLOR = qRgb(0,0,0);

typedef struct  POINT_LP
{
    int x;
    int y;
#ifdef CXVISIONOLDVER
    QRgb color;
    unsigned char*pByte;
#endif
} POINT_LP;

typedef struct  LINE_LP
{
    POINT_LP pointA;
    POINT_LP pointB;
    QRgb color;
} LINE_LP;
typedef struct PointDouble
{
    POINT_LP apoint;
    double dvalue;
    int ivalue;
}PointDb;


using namespace std;
typedef std::vector<PointDb> vectorPointDb_LP;
typedef vectorPointDb_LP::iterator vectorPointDb_LPitor;

class Plist
{
private:
        POINT_LP*m_lpPointlist;
        int m_isum;
        QRgb m_color;
public:
        Plist();
        ~Plist();

        inline POINT_LP*getalloc()
        {
            return m_lpPointlist;
        }
        inline int getisum()
        {
            return m_isum;
        }
        int cxAlign( int size, int align );
        void DeletList();
        void ListRotate(int ix0,int iy0,double dangle);
        void ListMove(int changesumx,int changesumy);
        void ListMove_SSE2(int changesumx,int changesumy);
        void ListShrink_SSE2(int ishrink);
        void CopyFrom( const Plist& Acopylist);
        void CopyBeginSample(const Plist& Acopylist,int isamplenum);//�����ӿ�ʼ��isamplenum����
        void CopyEndSample(const Plist& Acopylist,int isamplenum);//������������isamplenum����
        void CopySample(const Plist& Acopylist,int isamplenumbegin,int isamplenumend);
        void SetColor(QRgb CrlRef=BLUE);
        void ListComb(Plist& Alist,Plist& Blist);
        void ListComb(Plist& Alist,Plist& Blist,Plist& Clist);
        void ListComb(Plist& Alist,Plist& Blist,Plist& Clist,Plist& Dlist);
        void PointSort();
        void DeletePointSame();
        void Translation(unsigned char*FirstBits,int bitsSUM);
        //Geometry
        void GetPixelPoint(int x,int y);
        void GetInsideList(Plist& pointlist);
        //rect
        void GetRectPointListA(int i_x,int i_y,int i_x0,int i_y0);
        //circle
        void GetCirclePointList(int i_circle_x,int i_circle_y,int i_center_x0,int i_center_y0);
        void GetCirclePointListA(int i_circle_x,int i_circle_y,int i_center_x0,int i_center_y0);
        void GetArcPointListA(int i_x,int i_y,int i_x0,int i_y0,int ipoinsum);
        //ellipse
        void GetEllipsePointListA(int i_circle_x,int i_circle_y,int i_center_x0,int i_center_y0,int idis);
        //pointlist
        void GetListPointListA(int* px0,int* py0,int isize);
        //line
        void GetLinePointList(int m_x, int m_y,int  m_x0, int m_y0);
        void GetLinePointList(POINT_LP apoint,POINT_LP bpoint);
        void GetXPointList(float iCenterX, float iCenterY,int iPointSize);
        //
        void GetLine(int m_x, int m_y,int  m_x0, int m_y0 );
        void GetLine(POINT_LP apoint,POINT_LP bpoint);
        //
        void GetLine_OLD(int m_x, int m_y,int  m_x0, int m_y0 );
        void GetLine_OLD(POINT_LP apoint,POINT_LP bpoint);
        //Search
        void SearchCirclePointList(int i_x,int i_y,int i_x0,int i_y0);
        void SearchCirclePointListA(int i_x0,int i_y0,int i_x,int i_y);

        //Draw
        void Draw(QPainter &painter,QPen &pen);

        //reserve
        void Reserve();
        //shapeallca

        //SetNum push point
        void SetListNum(int inum);
        void PushPoint(int inum,int ix,int iy);
        void AddSize(int inum);
        void ReSize(int inum);
        //
        void Mirror(int ix, int iy);
        void Add(Plist*plist);

        //
        void SaveList(const char * pchar);
        void LoadList(const char * pchar);
        void BeginPointList(Plist* beginlist);
        void EndPointList(Plist* beginlist);

        void GetMinXY(int &ix,int &iy);
        void ZeroPosList();
        void LineVH(Plist*plist,int irate,double doffset=0.5);

        void GetInsideList_XTX(Plist& pointlist);
        void PointSort_YX();

        void RectFilter(int ix0,int iy0,int ix1,int iy1);
private:
        inline int CircleStep(int apha,int y,int x,int i_x0,int i_y0,int  i_R_2);
        inline int EllipseStep(int apha,int x,int y,int i_ax,int i_ay,int i_bx,int i_by,int  i_dis);
};

typedef double   (*_asPlistFunc)(POINT_LP * , int  , int  ,void * );



class PlistTestRun
{
    Plist m_list;
    std::ostream *m_stream;
public:
    void Test_ListComb()
    {
        POINT_LP PA,PB,PC,PD;
        Plist ABLine,ACLine,CDLine,BDLine,ROIEdge,ROILine;
        for(int i=0;i<1000;i++)
        {
            PA.x=rand()%1000;
            PA.y=rand()%1000;
            PB.x=rand()%1000;
            PB.y=rand()%1000;
            PC.x=rand()%1000;
            PC.y=rand()%1000;
            PD.x=rand()%1000;
            PD.y=rand()%1000;


            ABLine.GetLinePointList( PA, PB);
            ACLine.GetLinePointList( PA, PC);
            CDLine.GetLinePointList( PC, PD);
            BDLine.GetLinePointList( PB, PD);
            ROILine.ListComb(ABLine,ACLine);
            ROIEdge.ListComb(ABLine,ACLine,CDLine,BDLine);

            //ABLine.DeletList();
            //ACLine.DeletList();
            //CDLine.DeletList();
            //BDLine.DeletList();
            //ROIEdge.DeletList();
        }
    }
    void TestCircle(int x0,int y0,int x1,int y1)
    {
        m_list.GetCirclePointListA(x1,y1,x0,y0);
    }
    void SetStream(std::ostream *a_stream)
    {
        assert(a_stream);
        m_stream = a_stream;
    }
    void TestDataOut()
    {
      /*  POINT_LP *lpoint=m_list.getalloc();
        *m_stream <<tr(" isum=") <<m_list.getisum() <<endl;
        for(int sum=0;sum<m_list.getisum();sum++)
        {
            if(sum<10||m_list.getisum()-sum<10)
            *m_stream<<" x="<<lpoint[sum].x<<" y="<<lpoint[sum].y<<" "<<endl;;
        }
            *m_stream<<" isum="<<m_list.getisum()<<endl;
        */
    }
};



#endif
