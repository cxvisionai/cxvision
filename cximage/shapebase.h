#ifndef OBJECTSHAPE_H
#define OBJECTSHAPE_H
#include<QRgb>
#include<QSize>
#include<QPen>
#include<QLine>
#include<QMap>

#include<vector>
#include<string>


using namespace std;


enum POINT_TYPE
{
  POINTTYPE_DOT=0,
  POINTTYPE_X,
  POINTTYPE_CIRCLE,
  POINTTYPE_RECT,
  POINTTYPE_UNKNOWN
};
enum HT_SHOW_SELECT
{
    POINT_SHOW,
    LINE_SHOW,
    ALL_SHOW
};
class ShapeBase
{
    enum ShapeType { Line, Points,
                     Polyline,
                     Polygon, Rect,
                     RoundedRect,
                     Ellipse, Arc,
                 Chord, Pie, Path,
                     Text, Pixmap };

public:
    ShapeType m_shapetype;
    QPen m_pen;
    QBrush m_brush;
    bool m_antialiased;
    bool m_transformed;
    QPixmap m_pixmap;
    int m_ishow;
    double m_dminpercent;
    int m_ishowlines;

public:
    ShapeBase();
    ~ShapeBase();
    int show();
    void setshow(int ishow);
    void setShape(int ishape);
    void setPen(int ir,int ig,int ib);
    void setPen(QColor qcolor);
    void setpenw(int iw);
    void setBrush(int ir,int ig, int ib);
    void setAntialiased(int antialiased);
    void setTransformed(int transformed);
    void setPercent(double dvalue);

    virtual void drawshape(QPainter &painter);
    //list move
    virtual void Move(int ix,int iy){}
    virtual void Rotate(double iangle){}
    virtual void Zoom(double dx0,double dy0){}
    virtual void setshowlines(int ilines){m_ishowlines=ilines;}
    virtual int getpointnum(){return 0;}
};



class QRootGrid;
//背景 白 目标 黑
//背景 黑 目标 白
//A 横向 上白下黑
//A_ 横向 上黑下白
//B 纵向 左白右黑
//B_ 纵向 左黑右白
class QRootGrid:public ShapeBase
{
public:
    QRootGrid();
    ~QRootGrid();

private:
     //QMap<QPoint,QRootGrid> m_map ;
     QList <QRootGrid> m_glist;
     QList <QPoint> m_plist;

     int m_ilevel;
     int m_itype;//A=1,A_=2,B=4,B_=8
     int m_iclasstype;
     QString m_name;
     QPoint m_point;

public:
     void release();
     int level(){return m_ilevel;}
     QString name(){return m_name;}
     void setlevel(int id){m_ilevel =id;}
     void setname(QString name){m_name = name;}
     void settype(int itype){m_itype =itype;}
     void setshow(int ishow){m_ishow=ishow;}
     void addpoint(QPoint &apoint);

     int getlevel();
     void addrootpointlist(QList <QPoint> &keypoints,int itype);
     void addpoint(QPoint &arootpoint,QPoint &apoint);

     virtual void drawshape(QPainter &painter);
     void drawgrid(QPainter &painter);
     void drawlayer(QPainter &painter,int ilevel);
     void gridclasstype();
};








class LineShape:public ShapeBase
{
public:
    LineShape();
    ~LineShape();
private:
    QLine m_line;
    QPainterPath m_path;
public:
    QLine getline(){return m_line;}
    void setline(int ix0,int iy0,int ix1,int iy1);
    void setshow(int ishow);
    void linecopy(QImage &srcImage,QImage &desImage);
    void linecopyex(QImage &srcImage,QImage &desImage,int ix,int iy);

    QPointF getlinepoint(int inum);
    int getlinesize();
    void copy(LineShape &aline);
    virtual void drawshape(QPainter &painter);
    virtual void Move(int ix,int iy);
    virtual void Rotate(double iangle);
    virtual void Zoom(double dx0,double dy0);
    virtual void setpenw(int iw);
    virtual void setshowlines(int ilines){m_ishowlines=ilines;}
    LineShape& operator=(const  LineShape& aline);
};


typedef std::vector<LineShape> LineVector;
class PointsShape:public ShapeBase
{
public:
    QPainterPath m_path;
    void addpoint(QPoint &apoint);
    void addpoint(QPointF &apoint);
    void addpoints(PointsShape &points);

    void setshow(int ishow);
    void addapoint(double apointx,double apointy);
    void clear();
    int size();
    void calibration(double dx,double dy,double dangle);
    double getx(int inum);
    double gety(int inum);

    void addpoint(qreal &apointx,qreal &apointy);
    void addpointa(qreal &apointx,qreal &apointy);
    void addpointb(qreal &apointx,qreal &apointy);
    void addText(qreal x, qreal y,QFont &f,QString &text);
    QRectF boundingRect();
    QRectF controlPointRect();
    void save(const char * pchar);
    void load(const char * pchar);

    QPainterPath &getpath();
    void onepattern(double igap,int idirect,PointsShape& apoints);
    void doublepattern(double igap,int idirect,PointsShape& apoints);
    void doublesample(int isamplerate,PointsShape& apoints);
    void patterntokeys(QList <QPoint> &keypointsA,QList <QPoint> &keypointsA_,
                       QList <QPoint> &keypointsB,QList <QPoint> &keypointsB_,
                       int ibackgroundtype=0);
    void keystopattern(QList <QPoint> &keypointsA,QList <QPoint> &keypointsA_,
                       QList <QPoint> &keypointsB,QList <QPoint> &keypointsB_,
                       int igap=2,int itype=1,int isgap =2,int iline=2);
    void keyszoom(QList <QPoint> &keypointsA,QList <QPoint> &keypointsA_,
                       QList <QPoint> &keypointsB,QList <QPoint> &keypointsB_,
                  double dxz,double dyz);
    void patterntranform(int igap,int itype,int isgap,int iline);
    void patternzoom(double dxz,double dyz,int igap,int itype=0,int iline=1);

    void keysrootgrid(int ibackgroundtype,double drate =0.25,int ilevel = 4);


    virtual void drawshape(QPainter &painter);
    void drawshapex(QPainter &painter,double dmovx,double dmovy,
                                double dangle,double dzoomx,double dzoomy);
    virtual void Move(int ix,int iy);
    virtual void Rotate(double dangle);
    virtual void Zoom(double dx0,double dy0);
    virtual void setshowlines(int ilines){m_ishowlines=ilines;}
    virtual int getpointnum(){return 0;}
};

class TwoPointsShape:public ShapeBase
{
public:
    TwoPointsShape();
public:
    QPainterPath m_path;
    QMap<int,QPoint> m_linemap;//x, (miny ,maxy) or y,(minx,maxx)

    int m_insidewidth;
    int m_movoffset;
    int m_iheadtail;
    void setedgeoi(int insidewidth,int outsidewidth,int iheadtail);

    void addpoint(int ivaluea ,int ivalueb);

    void clear();
    void makepath(int ivh);
    void edgeimage(QImage &aImage,int itype);
    int size();
    virtual void drawshape(QPainter &painter);
    virtual void Move(int ix,int iy);
    virtual void Rotate(double dangle);
    virtual void Zoom(double dx0,double dy0);
    virtual void setshowlines(int ilines){m_ishowlines=ilines;}
    virtual int getpointnum(){return 0;}
};



typedef QVector<QRect> RectVector;
typedef QVector<double> AngleVector;
enum RectDirction{
    inside_dir = 0,
    outside_dir = 1,
    up_dir = 2,
    down_dir = 3,
    right_dir = 4,
    left_dir = 5,
    rightup_dir = 6,
    leftup_dir = 7,
    rightdown_dir = 8,
    leftdown_dir = 9,
    cross_dir = 10 ,
    same_dir = 11 ,
    up_pure_dir = 12,
    up_cross_dir = 13,
    down_pure_dir = 14,
    down_cross_dir = 15,
    down_purex_dir = 16
} ;
/*         0
       _________
       |       |
       |   O1  |
       |       |
       ---------

           U2
   ----------------
           D3

           |
    R4     |      L5
           |


           |
   RU6     |      LU7
           |
   -----------------
           |
   RD8     |      LD9
           |
*/


class RectsShape:public ShapeBase
{
public:
    RectsShape();
    void addrect(QRect &arect);
    void addrect(QRect &arect,QString &astring);
    void clear();
    int size();
    QRect getrect(int inum);
    void setspecshow(int ishownum);
    void setrect(int inum,int ix,int iy,int iw,int ih);
    void setstring(int inum,const QString &str);
    void setangle(int inum,double dangle);
    void removecontains_c();

    QRect comb(QRect &rect1,QRect &rect2);
    QRect comb(RectVector &rects1);
    RectDirction pos(QRect &rect1,QRect &rect2);

    void sort();
    void drawshapex(QPainter &painter,
                                double dmovx,
                                double dmovy,
                                double dangle,
                                double dzoomx,
                                double dzoomy);
    virtual void drawshape(QPainter &painter);
    virtual void Move(int ix,int iy);
    virtual void Rotate(double iangle);
    virtual void Zoom(double dx0,double dy0);
    virtual int getpointnum(){return 0;}
    virtual void setshowlines(int ilines){m_ishowlines=ilines;}
private:
    RectVector m_rects;
    QStringList m_strlist;
    AngleVector m_angles;
    int m_ispecshow;
};

class QArea: public QRect
{
 public:
    enum RectType{
        orgarea = 0,
        unitarea = 1, //2rects ->1rect no cross other rect
        crossarea = 2,//2rects ->1 rect cross
        multiarea = 3, //multi rects can be a one then no cross other rect
        releasearea = 4 //
    };

    QArea(RectType type =orgarea);
    int ID(){return m_id;}
    RectType type(){return m_type;}
    QString name(){return m_name;}
    void setid(int id){m_id =id;}
    void settype(RectType type){m_type = type;}
private:
    typedef QVector<QArea*> InsideAreas;
    InsideAreas m_insideareas;
    int m_id;
    RectType m_type;
    QString m_name;

};
typedef QVector<QArea> AreaVector;
class QAreas
{


    typedef struct unitrelation
    {
        int igapx;
        int igapy;
        QArea *m_pA;
        QArea *m_pB;
        RectDirction m_dirs;
    } unitone;
    typedef QVector<unitone> unitvect;

    typedef struct ObjectClass
    {
        QVector<ObjectClass*> s_psub;
        short s_id;
        short s_level;
        ObjectClass *s_pU;
        ObjectClass *s_pD;
        ObjectClass *s_pL;
        ObjectClass *s_pR;
        ObjectClass *s_pO;
        //temp no be used
        ObjectClass *s_pRU;
        ObjectClass *s_pLU;
        ObjectClass *s_pLD;
        ObjectClass *s_pRD;


    } objectarea;

    typedef struct RecognizeObj
    {
        QRect s_rect;
        short s_type;
        short s_relation;

        short s_singleW;
        short s_singleH;
        short s_num;
        QColor s_background;
    }recogobj;

    AreaVector m_areas;
    RectsShape m_rects;

public:
    QAreas(RectsShape &rects);
    void GenMap();
    void relation();
    void regroup();
    void sort();



};










#endif
