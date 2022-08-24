#ifndef CXVISION_GRID_H
#define CXVISION_GRID_H

#include <vector>
#include <string>
#include <map>
#include "shapebase.h"
#include "imagebase.h"


typedef struct objectrelation
{

    short m_itopx;
    short m_itopy;
    short m_ibottomx;
    short m_ibottomy;
    short m_ileftx;
    short m_ilefty;
    short m_irightx;
    short m_irighty;

    short m_igdigridx;
    short m_igdigridy;
    short m_ishowgridx;
    short m_ishowgridy;

    int m_isize;

    int m_itype;
}ObjRelation;



typedef std::map<std::string,QRect> RectMap;
typedef std::map<std::string,QRect>::iterator RectMapItor;

typedef std::map<std::string,int> GridType;
typedef std::map<std::string,int>::iterator GridTypeItor;

typedef std::map<std::string,PointsShape> GridPoints;
typedef std::map<std::string,PointsShape>::iterator GridPointsItor;

typedef vector<ObjRelation> RelationVect;
typedef vector<ObjRelation>::iterator RelationVectItor;

typedef std::map<std::string,RelationVect> GridObjects;
typedef std::map<std::string,RelationVect>::iterator GridObjectsItor;

typedef struct easyobject
{
  int s_iwobjnum;
  int s_ibobjnum;

}easyobj;


class findobject;

class QGrid
{

public:
    QGrid();
    ~QGrid();

    enum eGRIDType{
        GRIDType_THING		= 1,
        GRIDType_GROUP      = 2,
        GRIDType_EDGE       = 4,
        GRIDType_EDGE_A     = 8,
        GRIDType_EDGE_B     = 16,
        GRIDType_FAIL       = 32,
        GRIDType_NOVALID    = 64,
        GRIDType_EDGE_C     = 128,
        GRIDType_EDGE_D     = 256,
        GRIDType_NONE		= 0
    };

private:

    QBrush m_brush;
    int m_ishow;
    RectMap m_grid;
    GridType m_gridT;
    GridObjects m_gridR;//result

    PointsShape m_modelpoints;

    findobject * g_pbackfindobject;
    int m_iorgx;
    int m_iorgy;
    int m_iw;
    int m_ih;

    int m_igridw;
    int m_igridh;
    int m_iwgridsum;
    int m_ihgridsum;
    int m_iwgridgap;
    int m_ihgridgap;

    int m_irealw;
    int m_irealh;

    int m_ishowpicw;
    int m_ishowpich;

    int m_imodelmatrixsum;

    int m_imapmodelw;
    int m_imapmodelh;

    int m_iunitw;
    int m_iunith;


    QVector<int> m_fastmodel;


public:
    void setroi(int iOrgX, int iOrgY,int iWidth, int iHeight);
    void setgrid(int ih,int iw,int iwsum,int ihsum,int ihgap,int iwgap);
    virtual void drawshape(QPainter &painter);
    virtual void Move(int ix,int iy){return ;}
    virtual void Rotate(double iangle){return ;}
    virtual void Zoom(double dx0,double dy0){return ;}
    virtual int getpointnum(){return 0;}

    void settype(int igdix,int igdiy,int ivalue);
    void settypevalue(int igdix,int igdiy,int ivalue);
    int gettypevalue(int igridx,int igridy);
    void clear();

    int show() const;
    void setshow(int ishow);
    void setbrush(int itype,int icolor);

    void roiimagetomodel(void *pimage);
    void ROIImagetoModel(ImageBase &aimage);

    void ROIImagetoModel_gray(ImageBase &aimage);


    void savemodelfile(const char * pchar);
    void loadmodelfile(const char * pchar);
    void modeltogrid(int iatype,int ibtype);

    void savemapmodel(const char * pchar);
    void loadmapmodel(const char * pchar);

    void ReSetModelGrid();

    void UnitGrid();
    void ZeroModel();
    void EdgeGrid();
    void ReGrid(int iw,int ih);

    void ModelGridMethod_Gauss();
    void ModelGridMethod_Object();//
    void ModelGridMethod_Inside();
    void ModelGridMethod_Outside();

    int getfastmodelvalue(int ix,int iy);
    int getfastmodelw();
    int getfastmodelh();

    int getobject_areagrid_max();
    int getobject_totalvalue_max();


    void SetModelWH(int iw,int ih);
    easyobject ModelGridMethod_ObjectA();

    //other method ..

    QRect CentRect(QRect &rect);
    QRect CentRect_Condition(QRect &rect,int imax);
    void CentGrid(int iw,int ih);
    void SetUnit(int iw,int ih);
    QString GetGridString();
    void setfastlistvalue(int inum,int ivalue);
    QVector<int> &getfastmodel();
    PointsShape &getpatmodel();
    void SetFastModel(QVector<int> &fastmodel);
    void GridZoom(int iw,int ih);
    void Grid2PattenModel(int icompgap);
    void Grid2PattenModel_org(int icompgap);
    void GridFastModel(QGrid *pgrid);
    bool GridCompare(QGrid *pgrid);
};

#endif
