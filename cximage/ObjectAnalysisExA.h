#ifndef OBJECTANALYSISEXA_H
#define OBJECTANALYSISEXA_H

#define  MAXEDGENUM 100

using namespace std;


class CGDIPosition;
class CText;
typedef struct SubRect
{
	int m_irectnum;
}subrect;
typedef struct ObjectClass
{
	short s_ID;
        short s_level;
	std::vector<ObjectClass*> s_psub;
	ObjectClass *s_pU;
	ObjectClass *s_pD;
	ObjectClass *s_pL;
	ObjectClass *s_pR;
	ObjectClass *s_pO;
}objclass;


typedef struct RecognizeObj
{ 
	
	Rect_Shape s_rect;
	short s_type;
	short s_relation;
	
	short s_singleW;
	short s_singleH;
	short s_num;
	COLORREF s_background;
}recogobj;

typedef vector<Rect_Shape> RectVect;
typedef vector<Rect_Shape>::iterator RectVectIter;
 

class  CObjectAnalysisExA
{
public:
	CObjectAnalysisExA(void);

	~CObjectAnalysisExA(void);
 
	void BuildSpace();
	void ResetSpace();
	
	void ResetSpaceGrid(int iw,int ih);
	void ShowSpace(HDC hdc);
 
	SPoint* m_objlistscanorA;
	SPoint* m_objlistcollectorA;

	int m_iMEASUREPOINTSUMOBJ;

	vectorint m_irow;/*object size list*/
	vectorint m_iBorW;
	vectorfloat m_fangle;

	VectorPoint m_keypoint;
	VectorPoint m_fitcWH;
	CText *m_pText;
	short *m_piobjID;
	short *m_piobjColor;
	int m_icurcol;//当前点堆
	int m_icol;//测量点堆行数

	int m_imassnum;
	int m_iedgenum;
	int m_itoledgenum;

	int m_ipointtype; 
	int m_IMAGECHAR; 
	int m_ifontshow;//-1 显身全部对象物体 0 不显示 1显示白色 2显示黑色

	int m_icurobj;
	void AttachToImage(CWinImage*pimage,bool renewmemory=false);



	//
        ImageBase *m_gdiMask;

	LONGLONG m_totalarea;//整个面积

	LONGLONG m_iminarea;//object过滤最小面积
	LONGLONG m_imaxarea;//object过滤最大面积
	int m_iminpixel;//
	int m_imaxpixel;//
	Point_Shape m_PointHighLight;//错误点


	void SetRect(int iOrgX, int iOrgY,int iWidth, int iHeight,COLORREF Linecolor=GREEN,COLORREF Fullcolor=NONECOLOR)
	{
                //CROIMeasureRect::SetPlacement( iOrgX,  iOrgY,  iWidth,  iHeight );
	}
	void SetPlacement(int iOrgX, int iOrgY,int iWidth, int iHeight)
	{
                //CROIMeasureRect::SetPlacement( iOrgX,  iOrgY,  iWidth,  iHeight );
        }

        //黑白阈值
	//int m_itolence;
	int m_idistance;
	//void SetTread(int ithreshod);
	void Drag(int x,int y);
	void lock();
	void unlock();
	//void SetThreshodTolance(int itol);
	void SetFindDistance(int idis);
 
	void ObjectMove(int iobj,int ix,int iy);
	void ObjectMeasure(int iobj);
	int m_ishowobj;
	int m_ishowedge;
	int m_ishowRect;
	void SetShowObjEdge(int ishowobj=-1,int ishowedge=-1,int ishowrect=-1);
	void GetShowObjEdge(int &ishowobj,int &ishowedge,int &ishowrect);

	void SetROIColor(int ir,int ig,int ib);
	//int m_bshow;

	void SetShow(int bshow);
	int GetShow();
	int  GetObjectSize();
	void DrawObject(HDC hdc,int ishowway=3,int ix=0,int iy=0);/*draw object and edge*/
	void DrawFont(HDC hdc);
	void DrawRect(HDC hdc);
	void DrawRectA(HDC hdc);
	int GetObjectArea(int iobjnum);
//GDI 
	void GDIEdgeMeasure(void *pgdi);
	void GDIMeasure(void *pgdi);
	void GDIMeasure_Mask(void *pgdi);
    void GDIMeasure_256color(void *pgdi);
    void GDIMeasure_256color_Mask(void *pgdi);
    void GDIMeasure_256color_GDIMask(void *pgdi);
	void GDIMeasure_Org_GDIMask(void *pgdi);
    void GDIMeasure_ObjectMask(void *pgdi);
	void GDIMeasure_Oper_GDI(void *pgdi);
	void GDIMeasure_Oper_GDI_Temp(void *pgdi);
    void GDIMeasure_org(void *pgdi);
	void GDIMeasure_org_Mask(void *pgdi);
	void GDIMeasure_Grid(void *pgrid);
	void GDIMeasure_RelationGrid(void *pgrid);
	void ClearGDI(void *pgrid);
//利用GDI Copy Map Search Object
    int m_imaptype;
	void SetMeasureType(int itype){m_imaptype= itype;}
    void GDIMeasure_org_GDIMAP_AAA(void *pgdi);
    void GDIMeasure_org_GDIMAP_8bit_AAA(void *pgdi);
	
	void GDIMeasure_org_GDIMAP_Grid(void *pgdi);
	void GDIMeasure_org_GDIMAP_Grid_AB(void *pgdi);
	void GDIMeasure_org_GDIMAP_Grid_Mask(void *pgdi);
	void GDIMeasure_org_GDIMAP_Mask(void *pgdi);
	
	void GDIMAP_Grid_Edge(void *pgdi);
	void GDIMAP_Grid_Object(void *pgdi);
	void GDIMAP_Grid_NoObject(void *pgdi);
	void SetMasKGDI(void *pgdi);
 
	
    int m_igrid;

	void SetGridRate(int igrid){m_igrid = igrid;};

	COLORREF m_colorb;
	COLORREF m_colorw;
	COLORREF m_colormask;
	
	
	void MaskMapMeasure_GDIMAP(void *pgdi);
	void GDIMeasureEdge_GDIMAP(void *pgdi);
	void Get_GDIMAP_MaxCent(void *point);
	
	void Get_GDIMAP_MaxCent_Grid(void *point);
	void Get_GDIMAP_MaxUp_Grid(void *point);
	void Get_GDIMAP_MaxDown_Grid(void *point);
	void Get_GDIMAP_MaxLeft_Grid(void *point);
	void Get_GDIMAP_MaxRight_Grid(void *point);

	ObjRelation Get_GDIMAP_Grid_ObjectRelation(int iobjnum);
 
	void Search_CopyImage_ExGDIMAP(void *pgdi);

	int m_copythre;
	void SetMeasureCopythre(int icopythre);
	void GDIMeasure_CopyImage_ExGDIMAP(void *pgdi);


    void GDIMeasure_GDIMAP_Continue(void *pgdi);
	void Clear_GDIMAP();
	void Get_Object_GDIMAP(int iobjectnum);
	void Set_Object_GDIMAP(int iobjectnum,int ir,int ig,int ib);
	void PositionObject_GDIMAP(int ipointx,int ipointy);
	void SetMaskMapType_GDIMAP(int itype);
	void ImportCircleMask_GDIMAP(void *pImport);
	void ImportRectMask_GDIMAP(void *pImport);
	void ImportMask_GDIMAP(void *pImport);

	int m_imaskvalue;
 
	void ImportMaskPixelValue(int ivalue);
 
	void SetMapGDI(void *pgdi);
    void GDIMAP_ColorTable_H(void *pgdi);
    void GDIMAP_ColorTableBlur_H(int iGauss_Smoth=0,int imudthre=3);
	void GDIMAP_Binarization(int ithre);
	void GDIMAP_EasyThre_H(void *pgdi);
	void GDIMAP_SetMlineArea(void *pmline);

	void Shlink_GDIMAP_Condition_Area(int iarea,int ishinknum);
	void Shlink_GDIMAP_Condition_WH(int imaxw,int imaxl,int ishinknum);

    void MeasureObjectFill(void *pgdi);
    void MeasureAllObjectFill(void *pgdi);

	void ObjectFill(int iobj,int ir,int ig,int ib);
	void ObjectFill_org(void *pgdi);
	void ObjectFill_org_Mask(void *pgdi);
	int m_isearchfirstx;
	int m_isearchfirsty;
	void SetSearchFirstPoint(int ix,int iy);
	void GetSearchFirstPoint(void *point);
	void SetSKeyPoint(void *point);
    void GetObjectEdge(){EdgeAnalysis_OutSide_ShapeAllocA();};
	void GetObjectEdgeB(){EdgeAnalysis_InSide_ShapeAllocA();}
	void ShlinkEdge(int itimes){for(int i=0;i<itimes;i++)EdgeAnalysis_Shlink_ShapeAllocA();}
	void InflateEdge(int itimes){for(int i=0;i<itimes;i++)EdgeAnalysis_Inflate_ShapeAllocA();}
	void ShlinkEdgeb(int itimes){for(int i=0;i<itimes;i++)EdgeAnalysis_Inflate_ShapeAllocA();}
	void CombAreaShlink_PJ(int iarea,int imaxw,int imaxl,int ishinknum);
	void GetObjectEdgeNum(int inum);
	
	int GetBlackObjectNum();
	int GetWhiteObjectNum();
	int GetObjWorB(int iobj);

	void EdgeAnalysis_ShapeAllocA(CShapeAllocA * palloca);
	void EdgeAnalysis_OutSide_ShapeAllocA();
	void EdgeAnalysis_InSide_ShapeAllocA();
	void EdgeAnalysis_Shlink_ShapeAllocA();
	void EdgeAnalysis_Inflate_ShapeAllocA();
	iRGB m_meanRGB;
	void GetObjectMean(void *pgdi);
	int Get_Mean_R(){return (m_meanRGB.R&0x0ff);};
	int Get_Mean_G(){return (m_meanRGB.G&0x0ff);};
	int Get_Mean_B(){return (m_meanRGB.B&0x0ff);};
	double Get_H();
	double Get_S();
	double Get_O();
	int getcuredge();
	void setcuredge(int iedge);
	void setmaxedge();
	CShapeAllocA *GetEdge();
	void GetObjectRect(int &ix0,int &iy0,int &ix1,int &iy1);
	void SetGetpointtype(int itype){m_ipointtype=itype;};
	void SetAnalysisObject(int iobject){m_icurobj = iobject;};
	int GetAttributionObject(int ibw,int itype);
	void GetEdgePoint(void *pPoint);
    void FilterMaskEdgeObj(double drate);
	void SetFontShow(int ifwb);

	void MoveToCentPos(int ix,int iy);
	void Move(int ix,int iy);
	void RotateCent(double ix,double iy,double dangle);

	int GetWObjSize(int iobj);
	int GetBObjSize(int iobj);

	void GetMaxWObjCent(void *point);
	void GetMaxObjCent(void *point);
	void GetMaxWHCent(void *point);
	void GetMaxWHAPoint(void *point);
	void GetMaxWHBPoint(void *point);
	
	void GetMinWHAPoint(void *point);
	void GetMinWHBPoint(void *point);

	int GetMaxWObjSize();
	int GetMaxBObjSize();

	int GetMaxObjSize();
	int GetMinObjSize();
	int GetMaxObjWH();
	int GetMaxObjW();
	int GetMaxObjH();
	int GetTotalObjSize(); 
	int GetObjWH(int inum);
	void GroupFilterCenter(void *point);

	void SetCurObj(int iobj);
	void GetCurWObjectCent(void *point);
	void GetCurBObjectCent(void *point);
	void GetCurObjectCent(void *point);
	void ReleaseObject();

	void GetCurObjUpPoint(void *pPoint);
	void GetCurObjDownPoint(void *pPoint);
	void GetCurObjLPoint(void *pPoint);
	void GetCurObjRPoint(void *pPoint);
	void SetLargeNum();//100 objects
	void SetMassNum();
	void SetLargeMassNum();
	void ClearFont();
    void LoadMaskFile();
	void GDIMask(void *pgdi);

	void ObjectPointSort(int ilist);
	void ObjectListSort();
	void ObjectEdgeFitEllipse();
	double ObjectEdgeToPointMinDistance(int ipx,int ipy);

	void ObjectFilter(int iw,int igapw,int ih,int igaph);

	int GetAvgFitW();
	int GetAvgFitH();

	int GetFitW(int iobj);
	int GetFitH(int iobj);
	double GetFitAngle(int iobj);

	CShapeAllocA *m_maskalloc;
	void GetMaskAlloc(CShapeAllocA * pshapealloc);
	int GetMaskAllocMeanH(void *pgdi);

	void ObjectSizeSort();
	int GetSortSize(int iobj);

    void ObjectColorTable(void *pgdi);
	void ObjectColorTableBlur(int iGauss_Smoth);

	void ObjectColorMean(void *pgdi);

    double ObjectPointBWRate(int ix,int iy,int idist);

	void AreaAverage_GDIMAP(int iheadercancel,int itialcancel);
	void ThinCenterEdge_GDIMAP(int ilinestep);
	void ThinCenterEdge_ExGDIMAP(int ilinestep,int iborw,int ioffset=3,int iattrib = 1);

	int ObjectMaxV_GDIMAP();
	void EdgeFilter(int iminpointnum,int imaxpointnum,int ilinenum);
	void EdgeMean(int iedgenum,int iw,int imovx,int imovy);
	void EdgeGapMean(int iedgenum1,int iedgenum2,int ishink);
	void EdgeCent(int iedgenum);
	void EdgeUp(int iedgenum);
	void EdgeDown(int iedgenum);
	int GetPointX(); 
	int GetPointY();
	void EdgeSort_X();
	void EdgeSmooth(int itype,int ismooth);
	void EdgeUnit();
	int GetEdgeNum(){return m_itoledgenum;};
	
	int m_iedgethre1;
	int m_iedgethre2;
	int m_ifindgap;

	void SelectShapeMouseDown(int PointX,int PointY);
	void SelectShapeMouseUp(int PointX,int PointY);
	void SelectShapeBegin();
	void SelectShapeEnd();

	bool GetSelect(){return m_maskarea.m_bselect;};
	int GetCentX(int iobj);
	int GetCentY(int iobj);
	//shapevector m_shapevector;
 
 

	int m_iparentobjnum; 
	ObjectClass *m_pObjectCS; 
	short *m_pObjClass;
	short *m_pObjCID;
	short *m_pObjWH;

	RecognizeObj *m_precgobj;
	short m_irecognum;


	//////////////////////////////////////////////////////////////////////////
	short m_iminobjw;
	short m_imaxobjw;
	short m_iminobjh;
	short m_imaxobjh;
	
	void SetMinMaxWH(int iminw,int imaxw,int iminh,int imaxh);
	
	void GetRelationObjectResult(int iobjrect);
	
	short m_ifilterNedge;
	
	
	void SetFilterNedge(int ifilter);

	//
	void GetRectAPoint(void *point);
	void GetRectBPoint(void *point);

};
#endif
