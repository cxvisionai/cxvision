#ifndef BACKIMAGEMANAGER_H
#define BACKIMAGEMANAGER_H
#include "imagebase.h"
//#include "findobject.h"
#define  OBJSCANNNUM  1024*1024//307200//20480//20480//10240//
#define  OBJCOLLECTIONNUM  1024*1024//307200//20480//20480

#define BACKIMAGEWITH 1024
#define BACKIMAGEHIGH 1024

#define TRANSFERIMAGEWITH 320
#define TRANSFERIMAGEHIGH 320

#define COLORNUMSUM 256

static QColor g_color[16] =
{
    Qt::red,
    Qt::green,
    Qt::blue,
    Qt::cyan,
    Qt::magenta,
    Qt::yellow,
    Qt::darkRed,
    Qt::darkGreen,
    Qt::darkBlue,
    Qt::darkCyan,
    Qt::darkMagenta,
    Qt::darkYellow,
    Qt::white,
    Qt::darkGray,
    Qt::gray,
    Qt::lightGray
};

class findobject;

class BackImageManager
{
public:

    BackImageManager(){CurMode();}
    ~BackImageManager();

    static int m_curmodul;
    static int m_imodulid;
    static void CurMode();
    static int GetCurMode();

    static ImageBase* m_pBackImage;
    static ImageBase* m_pBackObjectImage;
    static ImageBase* m_pMapImage;
    static ImageBase* m_pModelImage;
    static ImageBase* m_pTransferImage;

    int m_ishow;
    int m_iobjectshow;

    void setshow(int ishow);
    void setobjectshow(int ishow);

    int show();

    void draw(QPainter &painter);
    void drawtable(QPainter &painter);

    static void CreateBackImage(int iw,int ih);

    static ImageBase* GetBackImage(int curmodul=1);


    static ImageBase& BackImage();


    static void CreateBackObjectImage(int iw,int ih);

    static ImageBase* GetBackObjectImage(int curmodul=1);

    static ImageBase& BackObjectImage();


    static void CreateMapImage(int iw,int ih);

    static ImageBase* GetMapImage(int curmodul=1);

    static ImageBase& MapImage();


    static void CreateModelImage(int iw,int ih);

    static ImageBase* GetModelImage(int curmodul=1);

    static ImageBase& ModelImage();

    static void CreateTransferImage(int iw,int ih);

    static ImageBase* GetTransferImage(int curmodul=1);

    static ImageBase& TransferImage();


    static SPoint * _thelistscanorA;
//    static SPoint * _thelistscanorB;
//    static SPoint * _thelistscanorC;
//    static SPoint * _thelistscanorD;

    static SPoint * _thelistcollectorA;
//    static SPoint * _thelistcollectorB;
//    static SPoint * _thelistcollectorC;
//    static SPoint * _thelistcollectorD;

    static SPoint *GetListScan(int curmodul=1);
    static SPoint *GetListCollect(int curmodul=1);


    static findobject *m_pfindobject;

    static findobject* Getbackfindobject(int curmodul=1);


    static qint64 *g_r_table;
    static qint64 *g_g_table;
    static qint64 *g_b_table;

    static qint64* GetRtable(int curmodul=1);
    static qint64* GetGtable(int curmodul=1);
    static qint64* GetBtable(int curmodul=1);
    
    static int g_r_table_thre;
    static int g_g_table_thre;
    static int g_b_table_thre;

    static int g_table_rate;

};



#endif
