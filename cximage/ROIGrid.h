//////////////////////////////////////////////////////////////////////////
//
#pragma once
#include "VisionCore.h"
#include "ObjectShape.h"
#include "ShapeFont.h"
#include "ShapeAllocA.h"

#include <vector>
#include <string>
#include <map>
using namespace std;

#ifdef DLLDIR_EX
#define DLLDIR  __declspec(dllexport)
#else
#define DLLDIR  //__declspec(dllimport)
#endif

// 列表 
typedef std::map<std::string,Rect_Shape> RectMap;
typedef std::map<std::string,Rect_Shape>::iterator RectMapItor;

typedef std::map<std::string,int> GridType;
typedef std::map<std::string,int>::iterator GridTypeItor;

typedef vector<Plist> VectPlist;
typedef vector<Plist>::iterator VectPlistItor;

typedef std::map<std::string,Plist> GridPoints;
typedef std::map<std::string,Plist>::iterator GridPointsItor;

typedef vector<ObjRelation> RelationVect;
typedef vector<ObjRelation>::iterator RelationVectItor;

typedef std::map<std::string,RelationVect> GridObjects;
typedef std::map<std::string,RelationVect>::iterator GridObjectsItor;

typedef vector<POINT_LP> CxPointVect;
typedef vector<POINT_LP>::iterator CxPointVectItor;


class CGDIImage32;
class CObjectAnalysisExA;
class CxVisionImage;
class DLLDIR CROIGrid :public CCore
{
public:
	CROIGrid(void);
public:
	~CROIGrid(void);
	
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
	
	bool m_b_lock;//移动锁定
	void lock(){m_b_lock=true;};
	void unlock(){m_b_lock=false;};
	
	void Drag(int x,int y)
	{
		if(true==m_b_lock)
			return;
	}
	
	int m_ihitx;
	int m_ihity;
	void HitTest(int x,int y);
	
	virtual void Draw(HDC hDC,float f32ZoomX = 1.0,float f32ZoomY = 1.0,int iOffsetx=0,int iOffsety=0)
	{
	}

	void SetPlacement(int iOrgX, int iOrgY,int iWidth, int iHeight)
	{
		m_iorgx = iOrgX;
		m_iorgy = iOrgY;
		m_iw = iWidth;
		m_ih = iHeight;
	}

	void GetPlacement(int &iorgx,int &iorgy,int &iwidth,int &iheight)
	{
		iorgx=m_iorgx;
		iorgy=m_iorgy;
		iwidth=m_iw;
		iheight=m_ih;
	}

 	int m_bshow;

	void SetShow(int bshow);
	int GetShow();

	void DrawTypeFont(HDC hdc);
	void DrawTypeFont(HDC hdc,int imx,int imy);
	void Draw(HDC hdc,int edrawtype);
	void Draw(HDC hdc,int imx,int imy,int ixoffset,int iyoffset,int edrawtype);
	Plist m_CLShapeList;
	//Plist m_Edgelist;
	//Plist m_Insidelist;

 	RectMap m_grid;
 	//GridPoints m_gridP;
 	GridType m_gridT;
 	GridObjects m_gridR;//result
	VectPlist m_typelist;
	CxPointVect m_ficlist;
	
	CxVisionImage *m_pcximage;
	CGDIImage32 *m_pgdi;
	CObjectAnalysisExA *m_pobj;
	int m_ishowpicw;
	int m_ishowpich;
	
	int GetFictureGridX(int inum);
	int GetFictureGridY(int inum);
	void FindFicture(int igridx,int igridy,int ithre,int iminobj= 5,int imaxobj=99999);
	//void ShowGridimage(HDC hdc,int igridx,int igridy);
	void ShowGridimage(HDC hdc,int igridx,int igridy,int ioffsetx=0,int ioffsety=0,int iw=640,int ih=480,int imx=0,int imy=0);
	void ShowFicture(int ishowx,int ishowy,int ishoww,int ishowh);
	void ShowPartImage(HDC hdc,int ix,int iy,int imx,int imy,int ixoffset,int iyoffset);
	void ShowPartImageA(HDC hdc,int ix,int iy,int imx,int imy,int ixoffset,int iyoffset);
	
	int GetFictureSum();
	void FictureTotal();
	int FictureCalibX(int inum);
	int FictureCalibY(int inum);
	int GetFictureSize(int idnum);

	int GetGridFictureSize(int ix,int iy);
	void CheckGrid(int ithre,int imaxobj,int iminobj);
	void GetCximage(void *pimage);
	void GetMObj(void *pobject);
	void GetGDI(void *pgdi);
	void SetEdgeType();
	void ClearTypeList();
	void AddTypeList(Plist* alsit);
	void DrawPoints(HDC hdc,int iLineSize,COLORREF CrlRef,int imx = 0,int imy = 0);
	void ClearPoint();
	void GetMLinePoint(void *pline);
	void GetMLine(void *pline);
	void EdgeTypeGrid(int iexlen,int iedgetype);
	void TypeGridResult();
	void SetType(int igdix,int igdiy,int ivalue);

	void SetTypeValue(int igdix,int igdiy,int ivalue);
	int GetTypeValue(int igridx,int igridy);
	
	void SetTypeList(int igdix0,int igdix1,int igdiy0,int igdiy1,int ivalue);
	int GetGridGDIPosY(int igridy);
	int GetType(int igridx,int igridy);
	void TypeClear();
	
	void GetMObject(void *pObject);
	void GetLine(void *pLine);
	void ClearResult();
	void DrawResult(HDC hdc,int imx = 0,int imy = 0,int ixoffset = 0,int iyoffset = 0);
	void DrawResultCximage(HDC hdc,int imx = 0,int imy = 0);
	int m_icurgridx;
	int m_icurgridy;
	void SetCurGrid(int ix,int iy){m_icurgridx = ix;m_icurgridy = iy;};

	void AddGridType(const std::string & strname,int itype)
	{
		GridType::iterator item = m_gridT.find(strname);
		if (item!=m_gridT.end())
			return ;
		m_gridT[strname]=itype;
	};
	int m_imodelmatrixsum;	
	void SetGrid(int ih,int iw,int iwsum,int ihsum,int ihgap,int iwgap);
	int GetGrid(){return m_imodelmatrixsum;};

	void GetGridMean(void *pgdi);

	thrdint DotISO_FAST(stRGB &lpA);

//	stRGB m_rgbmean[1000];//[256];//
//	double GetMaxGridMean_H();
	
	CROIGrid *m_prelationgrid1;//相关Grid1
	CROIGrid *m_prelationgrid2;//相关Grid2
	int m_irelationX;
	int m_irelationY;
	int m_irelationW;
	int m_irelationH;
	void SetRelationPos(int ix,int iy,int iw,int ih);
	void SetRelationGridA(void *pgrid);
	void SetRelationGridB(void *pgrid);

	int GetRelationType(int ix,int iy);
	void SetRelationType(int ix,int iy,int itype);


	void ObjectGridType();
	int ObjectGridTypeCal(int icalway);
 };
