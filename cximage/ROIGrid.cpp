#include "stdafx.h"
#include "ROIGrid.h"

#include "SSE2GDIImage.h"
//#include "ROIMeasureLineAex.h"
#include "ObjectAnalysisExA.h"
#include "CxVisionImage.h"

using namespace std;


CROIGrid::CROIGrid()
{
	m_b_lock=false;
	m_bshow = DrawType_drawframe;

	m_imodelmatrixsum = 0;
	
	m_irealw = 640;
	m_irealh = 480;
	
	m_ishowpicw = 80;
	m_ishowpich = 80;
	
	m_ihitx = 0;
	m_ihity = 0;

	m_prelationgrid1 = NULL;
	m_prelationgrid2 = NULL;
	m_irelationX = 0;
	m_irelationY = 0;
}

CROIGrid::~CROIGrid()
{

}
void CROIGrid::SetShow(int bshow)
{
	m_bshow=bshow;
}	

int CROIGrid::GetShow()
{
	return m_bshow;
}
void CROIGrid::Draw(HDC hdc,int edrawtype)
{
	if(edrawtype&DrawType_drawframe)
	{
		int x0= m_iorgx;
		int y0= m_iorgy;
		// CRectShape::Draw(hdc,1,1,x0,y0);
	}
	if(edrawtype&DrawType_drawfindresult)
	{
		int x0= m_iorgx;
		int y0= m_iorgy;
//for(int i=0;i<m_grid.size();i++)
//{
//int ix = m_grid[i].
//char chnum[25];
//sprintf(chnum, "%d,%d" ,icurx,icury);
//m_gridT[std::string(chnum)] = 0;
//m_grid[i].draw(hdc,x0,y0,DKGREEN);
//}
		RectMap:: iterator pIter;
		for ( pIter = m_grid.begin( ) ; pIter != m_grid.end( ) ; pIter++ )
		{
			//int itype = m_gridT[pIter->first];
			//if(itype==1)
			//	pIter->second.draw(hdc,x0,y0,RGB(128,0,0),2);
			//else if(itype==2)
			//	pIter->second.draw(hdc,x0,y0,RGB(0,0,228),1);
			//else 
			//	pIter->second.draw(hdc,x0,y0,LTGRAY);
			int itype = m_gridT[pIter->first];
			pIter->second.draw(hdc,x0,y0,LTGRAY);
			if(itype&GRIDType_THING)
				pIter->second.draw(hdc,x0,y0,RGB(125,0,125),2);
			if(itype&GRIDType_FAIL)
				pIter->second.draw(hdc,x0,y0,RGB(240,0,0),2);
			if(itype&GRIDType_GROUP)
				pIter->second.draw(hdc,x0,y0,RGB(0,0,228),2);
			if(itype&GRIDType_EDGE)
				pIter->second.draw(hdc,x0,y0,RGB(0,0,150),2);
				
		}
	}
	if(edrawtype&DrawType_drawsearchline)
	{
		//DrawPoints(hdc,3,BLUE);
		//if(m_CLShapeList.getisum()>0)
		//	m_CLShapeList.DrawLine(hdc,3,GREEN);
		DrawTypeFont(hdc);
	}
	if(edrawtype&DrawType_drawfitresult)
	{
		DrawResult(hdc);
	}
}
void CROIGrid::Draw(HDC hdc,int imx0,int imy0,int ixoffset,int iyoffset,int edrawtype)
{
	int imx = imx0 + ixoffset;
	int imy = imy0 + iyoffset;
	if(edrawtype&DrawType_drawframe)
	{
		int x0= m_iorgx;
		int y0= m_iorgy;
		// CRectShape::Draw(hdc,1,1,x0,y0);
	}
	if(edrawtype&DrawType_drawfindresult)
	{
		int x0= m_iorgx;
		int y0= m_iorgy;

		RectMap:: iterator pIter;
		for ( pIter = m_grid.begin( ) ; pIter != m_grid.end( ) ; pIter++ )
		{/*1 2 4 8 16 32 64*/
			int itype = m_gridT[pIter->first];
			pIter->second.draw(hdc,x0+imx,y0+imy,LTGRAY);
			if(itype&GRIDType_THING)
				pIter->second.draw(hdc,x0+imx,y0+imy,RGB(105,0,0),2);
			if(itype&GRIDType_FAIL)
				pIter->second.draw(hdc,x0+imx,y0+imy,RGB(240,0,0),2);
			if(itype&GRIDType_GROUP)
				pIter->second.draw(hdc,x0+imx,y0+imy,RGB(0,120,228),2);
			if(itype&GRIDType_EDGE)
				pIter->second.draw(hdc,x0+imx,y0+imy,RGB(0,0,250),3);
			
				
			//if(itype==5)
			//{
			//	int itype = m_gridT[pIter->first];
			//	int ipos = pIter->first.find(','); 
			//	int ix = atoi(pIter->first.c_str());
			//	int iy = atoi(pIter->first.c_str() +1+ ipos);
			//	ShowGridimage(hdc,ix+imx,iy+imy,0,0,640,480,imx,imy);
			//}
			//if(itype==4)
			//{
			//	int itype = m_gridT[pIter->first];
			//	int ipos = pIter->first.find(','); 
			//	int ix = atoi(pIter->first.c_str());
			//	int iy = atoi(pIter->first.c_str() +1+ ipos);
			//}
		}
	}
	if(edrawtype&DrawType_drawsearchline)
	{
		int x0= m_iorgx;
		int y0= m_iorgy;
		//DrawPoints(hdc,3,BLUE,imx,imy);
		DrawTypeFont(hdc,x0+imx,y0+imy);
	}
	if(edrawtype&DrawType_drawfitresult)
	{
		DrawResult(hdc,imx,imy);
	}
	if(edrawtype&DrawType_drawtrace)
	{
		ShowPartImage(hdc,m_ihitx,m_ihity,imx,imy,ixoffset,iyoffset);
	}
}
void CROIGrid::HitTest(int x,int y)
{
	m_ihitx = x;
	m_ihity = y;
	if(m_bshow&DrawType_drawtrace)
	{
		
	}
}
void CROIGrid::TypeClear()
{ 
	m_gridT.clear();
	for(int ish=0;ish<m_ihgridsum;ish++)
	{
		for(int isw=0;isw<m_iwgridsum;isw++)
		{  
			char chnum[25];
			sprintf(chnum, "%d,%d" ,isw,ish);
			m_gridT[std::string(chnum)] = GRIDType_NONE;
 
		}
	}
}
void CROIGrid::SetGrid(int iw,int ih,int iwsum,int ihsum,int iwgap,int ihgap)
{ 
	m_igridw = iw;
	m_igridh = ih;
	m_iwgridsum = iwsum;
	m_ihgridsum = ihsum;
	m_iwgridgap = iwgap;
	m_ihgridgap = ihgap;
	m_gridT.clear();
	m_grid.clear();
	int itotalnum = iwsum *ihsum;
	Rect_Shape arect;
	int ibeginx = 0;
	int ibeginy = 0;
	int icurx = 0;
	int icury = 0;
	for(int ish=0;ish<ihsum;ish++)
	{
		for(int isw=0;isw<iwsum;isw++)
		{
			icurx = ibeginx + iwgap*isw;
			icury = ibeginy + ihgap*ish;
			Rect_Shape arect(icurx,icury,icurx+iw,icury+ih);
			arect.SetColor(BLUE);
			char chnum[25];
			sprintf(chnum, "%d,%d" ,isw,ish);
			m_gridT[std::string(chnum)] = GRIDType_NONE;
			m_grid[std::string(chnum)] = arect;
		}
	}
  
}
void CROIGrid::GetMLine(void *pline)
{
	if(pline==NULL)
		return;
	 //CROIMeasureLineAex *pmline = (CROIMeasureLineAex *)pline;
	 //CShapeAllocA * pmlist = pmline->GetMList();
	 //m_CLShapeList.AddShapeAllocA(pmlist);
}
void CROIGrid::GetMObject(void *pObject)
{		
	//坐标转化
	int	igx = m_icurgridx  ;
	int igy = m_ihgridsum - m_icurgridy -1;
	CObjectAnalysisExA *pmobject = (CObjectAnalysisExA *)pObject;
	RelationVect avect;
	int igetobjectsum = pmobject->GetObjectSize();
	if(igetobjectsum<=0)
		return;
	
	ObjRelation arelation;
	for(int i=0;i<igetobjectsum;i++)
	{
		arelation = pmobject->Get_GDIMAP_Grid_ObjectRelation(i);
		
		//////////////////////////////////////////////////////////////////////////
		//
		 arelation.m_igdigridx = igx;
		 arelation.m_igdigridy = igy;
		//////////////////////////////////////////////////////////////////////////

		arelation.m_ishowgridx = m_icurgridx;
		arelation.m_ishowgridy = m_icurgridy;
		


		//////////////////////////////////////////////////////////////////////////
		arelation.m_itype = GRIDType_FAIL;//1边界标识 2内部标识
		avect.push_back(arelation);

	}
	char chnum[25];
	if(m_icurgridx>=0&&m_icurgridy>=0)
	{ 
		sprintf(chnum, "%d,%d" ,m_icurgridx,m_icurgridy);
		m_gridR[std::string(chnum)] = avect;
		m_gridT[std::string(chnum)] = GRIDType_FAIL;
	}
}

void CROIGrid::GetLine(void *pline)
{
	if(pline==NULL)
		return;
	Line_Shape *pcline = (Line_Shape *)pline;
	Plist lineList;
	lineList.GetLine(pcline->Ax,pcline->Ay,pcline->Bx,pcline->By);
	m_CLShapeList.Add(&lineList);
}
void CROIGrid::ClearResult()
{
	m_gridR.clear();
}
void CROIGrid::ClearPoint()
{	
	//GridType:: iterator pIter;
	//for ( pIter = m_gridT.begin( ) ; pIter != m_gridT.end( ) ; pIter++ )
	//{ 
	//	pIter->second  = GRIDType_NONE;
	//}
	//m_gridP.clear();
	//
	m_CLShapeList.DeletList();
}

///////////////////////////////////////////
void CROIGrid::SetTypeList(int igdix0,int igdix1,int igdiy0,int igdiy1,int ivalue)
{
	for(int iy = igdiy0;iy<=igdiy1;iy++)
	{
		for(int ix = igdix0;ix<=igdix1;ix++)
		{
			char chnum[25];
			int iny = m_ihgridsum - iy - 1;
			sprintf(chnum, "%d,%d" ,ix,iny);
			int itype = m_gridT[std::string(chnum)] ; 
			m_gridT[std::string(chnum)] = itype|ivalue;
	
		}
	}

}


///////////////////////////////////////////
void CROIGrid::SetType(int igdix,int igdiy,int ivalue)	
{
	char chnum[25];
	int iny = m_ihgridsum - igdiy - 1;
	sprintf(chnum, "%d,%d" ,igdix,iny);

	int itype = m_gridT[std::string(chnum)] ; 
	m_gridT[std::string(chnum)] = itype|ivalue;
}

inline void CROIGrid::SetTypeValue(int igdix,int igdiy,int ivalue)
{
	char chnum[25]; 
	sprintf(chnum, "%d,%d" ,igdix,igdiy);
	m_gridT[std::string(chnum)] =ivalue;
}
inline int CROIGrid::GetTypeValue(int igridx,int igridy)
{
	char chnum[25];
	sprintf(chnum, "%d,%d" ,igridx,igridy);
	return m_gridT[std::string(chnum)] ; 
}
///////////////////////////////////////////
//GDI    Grid
//3        0
//2        1
//1        2
//0        3
int CROIGrid::GetGridGDIPosY(int igridy)
{
	return m_ihgridsum - igridy -1;
	//int itype = m_gridT[pIter->first];

	//int ipos = pIter->first.find(',');//strcmp( pIter->first, "," );

	//const char *pchar =  pIter->first.c_str();

	//int ix = atoi(pIter->first.c_str());
	//int iy = atoi(pIter->first.c_str() +1+ ipos);
}
int CROIGrid::GetType(int igridx,int igridy)
{
	char chnum[25];
	int iny = m_ihgridsum - igridy - 1;
	sprintf(chnum, "%d,%d" ,igridx,iny);
	return m_gridT[std::string(chnum)] ; 
}


void CROIGrid::EdgeTypeGrid(int iexlen,int iedgetype)
{
	int isize =m_CLShapeList.getisum();
	if(isize<2)
		return;  
	//m_gridP.clear();
	POINT_LP *lpoint = m_CLShapeList.getalloc();
	int ix0 = lpoint->x;
	int iy0 = lpoint->y;
	int igridwnum = 0;
	int igridhnum = 0;
	
	for(int i=0;i<isize;i++)
	{  
		ix0 = lpoint->x - m_iorgx;
		iy0 = lpoint->y - m_iorgy;
		
		igridwnum = ix0 /m_igridw;
		igridhnum = iy0 /m_igridh;
 
		char chnum[25];
		sprintf(chnum, "%d,%d" ,igridwnum,igridhnum);
		int itype = m_gridT[std::string(chnum)] ; 
		m_gridT[std::string(chnum)] = itype|iedgetype;//GRIDType_EDGE; 
		//扩展 上下左右空间
		//上
		ix0 = lpoint->x - m_iorgx;
		iy0 = lpoint->y - m_iorgy-iexlen;
		
		igridwnum = ix0 /m_igridw;
		igridhnum = iy0 /m_igridh;
		ZeroMemory(chnum,sizeof(char)*25);
		sprintf(chnum, "%d,%d" ,igridwnum,igridhnum);
		itype = m_gridT[std::string(chnum)] ; 
		m_gridT[std::string(chnum)] = itype|iedgetype;//GRIDType_EDGE; 
		//下
		ix0 = lpoint->x - m_iorgx;
		iy0 = lpoint->y - m_iorgy+iexlen;

		igridwnum = ix0 /m_igridw;
		igridhnum = iy0 /m_igridh;
		ZeroMemory(chnum,sizeof(char)*25);
		sprintf(chnum, "%d,%d" ,igridwnum,igridhnum);
		itype = m_gridT[std::string(chnum)] ; 
		m_gridT[std::string(chnum)] = itype|iedgetype;//GRIDType_EDGE; 
		//左
		ix0 = lpoint->x - m_iorgx-iexlen;
		iy0 = lpoint->y - m_iorgy;

		igridwnum = ix0 /m_igridw;
		igridhnum = iy0 /m_igridh;
		ZeroMemory(chnum,sizeof(char)*25);
		sprintf(chnum, "%d,%d" ,igridwnum,igridhnum);
		itype = m_gridT[std::string(chnum)] ; 
		m_gridT[std::string(chnum)] = itype|iedgetype;//GRIDType_EDGE; 
		//右
		ix0 = lpoint->x - m_iorgx+iexlen;
		iy0 = lpoint->y - m_iorgy;

		igridwnum = ix0 /m_igridw;
		igridhnum = iy0 /m_igridh;
		ZeroMemory(chnum,sizeof(char)*25);
		sprintf(chnum, "%d,%d" ,igridwnum,igridhnum);
		itype = m_gridT[std::string(chnum)] ; 
		m_gridT[std::string(chnum)] = itype|iedgetype;//GRIDType_EDGE; 
		
		
		
		lpoint++;
	}
	
	//m_CLShapeList.DeletList();
	//m_Edgelist.DeletList();
	//m_Insidelist.DeletList();
	//int itotalsize = m_grid.size();
	// m_Edgelist.SetListNum(itotalsize);
	//int ipointnum = 0;
	//////////////////////////////////////////////////////////////////////////
	//RectMap:: iterator pIter;
	//for ( pIter = m_grid.begin( ) ; pIter != m_grid.end( ) ; pIter++ )
	//{
	//	int itype = m_gridT[pIter->first];
	//	
	//	int ipos = pIter->first.find(',');//strcmp( pIter->first, "," );
	//	
	//	const char *pchar =  pIter->first.c_str();
	//	
	//	int ix = atoi(pIter->first.c_str());
	//	int iy = atoi(pIter->first.c_str() +1+ ipos);
	//	if(itype&GRIDType_EDGE_A)//if(itype>0)
	//	{
	//		m_Edgelist.PushPoint(ipointnum, ix , iy);
	//		ipointnum = ipointnum +1;
	//	}
	//}
	//m_Edgelist.ReSize(ipointnum);
	//m_Insidelist.GetInsideList(m_Edgelist);

	//isize =m_Edgelist.getisum();
	//lpoint = m_Edgelist.getalloc();
	//for(int i=0;i<isize;i++)
	//{
	//	int ix  = lpoint->x;
	//	int iy  = lpoint->y;
	//	char chnum[25];
	//	sprintf(chnum, "%d,%d" ,ix,iy);
	//	m_gridT[std::string(chnum)] = GRIDType_EDGE;//1边界标识 2内部标识
	//	lpoint = lpoint + 1;
	//}
	//isize = m_Insidelist.getisum();
	//lpoint = m_Insidelist.getalloc();
	//for(int i=0;i<isize;i++)
	//{
	//	int ix  = lpoint->x;
	//	int iy  = lpoint->y;
	//	char chnum[25];
	//	sprintf(chnum, "%d,%d" ,ix,iy);
	//	m_gridT[std::string(chnum)] = GRIDType_A;//1边界标识 2内部标识
	//	lpoint = lpoint + 1;
	//} 
}

void CROIGrid::ClearTypeList()
{
	 
}
void CROIGrid::AddTypeList(Plist *alist)
{
Plist blist;
int isize = m_typelist.size();
m_typelist.push_back(blist);
m_typelist[isize].CopyFrom(*alist);

}
void CROIGrid::SetEdgeType()
{
	int isize = m_typelist.size();
	for(int i=0;i<isize;i++)
	{

		int ipsize = m_typelist[i].getisum();
		if(ipsize<4)
		{
			POINT_LP *lpoint = m_typelist[i].getalloc();
			for(int j=0;j<ipsize;j++)
			{
				int ix  = lpoint->x;
				int iy  = lpoint->y;

				char chnum[25];
				sprintf(chnum, "%d,%d" ,ix,iy);
				m_gridT[std::string(chnum)] = GRIDType_THING;//1边界标识 2内部标识 3缺陷
				lpoint = lpoint + 1;
			}
		}
		else
		{		
			POINT_LP *lpoint = m_typelist[i].getalloc();
			for(int j=0;j<ipsize;j++)
			{
				int ix  = lpoint->x;
				int iy  = lpoint->y;

				char chnum[25];
				sprintf(chnum, "%d,%d" ,ix,iy);
				m_gridT[std::string(chnum)] = GRIDType_EDGE;//1边界标识 2内部标识 3缺陷
				lpoint = lpoint + 1;
		}
		
		}
	}
}
void CROIGrid::TypeGridResult()
{
	GridObjects:: iterator pIter;
	for ( pIter = m_gridR.begin( ) ; pIter != m_gridR.end( ) ; pIter++ )
	{
		int itype = m_gridT[pIter->first];
  
		for(int i=0;i<(pIter->second).size();i++)
		{
			//////////////////////////////////////////////////////////////////////////
			//判断边界踩线
			int icentx= ((pIter->second)[i].m_irightx + (pIter->second)[i].m_ileftx)/2;
			int icenty =  ((pIter->second)[i].m_ibottomy +  (pIter->second)[i].m_itopy)/2;
		 
			if(icentx<80||icenty<80)
			{
				//string to int x y
				int ipos = pIter->first.find(',');//strcmp( pIter->first, "," );
 
				int ix = atoi(pIter->first.c_str());
				int iy = atoi(pIter->first.c_str() +1+ ipos);
				if(icentx<80)
				{
					char chnum[25];
					sprintf(chnum, "%d,%d" ,ix-1,iy);
					int itype2 =m_gridT[std::string(chnum)];
					 if(1==itype2)
					  (pIter->second)[i].m_itype = GRIDType_EDGE;
				}
				if(icenty<80)
				{
					char chnum[25];
					sprintf(chnum, "%d,%d" ,ix,iy-1);
					int itype2 =m_gridT[std::string(chnum)];
					if(1==itype2)
						(pIter->second)[i].m_itype = GRIDType_EDGE;
				}

			}
			else
			 (pIter->second)[i].m_itype = itype;
		}
	}
}

inline thrdint CROIGrid::DotISO_FAST(stRGB &lpA)
{
	thrdint dotiso;
	//I用I/0.416=(R+B+G)>>3表示;
	//以免溢出所以>>3;当点光度<8时不计；
	//由于当点色>0x80时右移动会发生左边位填1入内而不是填0；所以移动一位后 & 0x7f
	int R1=lpA.R&0x0ff;
	int B1=lpA.B&0x0ff;
	int G1=lpA.G&0x0ff;
	int max[3];
	int temp;
	//指示最大的色型
	char Smax[3];
	char stemp;
	max[0]=R1;
	max[1]=B1;
	max[2]=G1;
	Smax[0]='R';
	Smax[1]='B';
	Smax[2]='G';
	if(max[1]>max[0])
	{
		temp=max[0];
		max[0]=max[1];
		max[1]=temp;

		stemp=Smax[0];
		Smax[0]=Smax[1];
		Smax[1]=stemp;
	}
	if(max[2]>max[1])
	{
		temp=max[1];
		max[1]=max[2];
		max[2]=temp;

		stemp=Smax[1];
		Smax[1]=Smax[2];
		Smax[2]=stemp;
	}
	if(max[1]>max[0])
	{
		temp=max[0];
		max[0]=max[1];
		max[1]=temp;

		stemp=Smax[0];
		Smax[0]=Smax[1];
		Smax[1]=stemp;
	}
	//色亮分量:H=√3/3*√(G*G+B*B+R*R+2RB+2GB+2GR)=(√3/3)*(R+B+G);
	//色度分量:S=√(2*(G*G+B*B+R*R-RB-GB-GR)/3);
	//S在最大色上的分量 :    S1=(√6/6)*(2R-(G+B));		方向：最大色的色调分量 
	//S在垂直最大色上的分量：S2=(√2/2)*|G-B|;	方向：最大色的偏向第二色调分量
	//S的偏转角：			 tagα=S1/S2;       方向判断：最大色的偏向第二色调分量
	//double H=(max[0]+max[1]+max[2])/1.732;
	int iH = (max[0]+max[1]+max[2]);
	int H=intx0_5(iH) +intx0_062(iH) +intx0_015(iH);

	//double H=(max[0]+max[1]+max[2])/3;//方便显示/3
	//double S=sqrt((double)2*(max[1]*max[1]+max[2]*max[2]+max[0]*max[0]-max[0]*max[2]-max[1]*max[2]-max[1]*max[0])/3);
	//double S=sqrt((double)2*(max[1]*max[1]+max[2]*max[2]+max[0]*max[0]-max[0]*max[2]-max[1]*max[2]-max[1]*max[0])/3);
	int snum=(max[1]*max[1]+max[2]*max[2]+max[0]*max[0]-max[0]*max[2]-max[1]*max[2]-max[1]*max[0]);
	int S=ISOsqrtX[snum];
	int S1=(((max[0])<<1)-(max[1]+max[2]));
	int S2=(max[1]-max[2]);
	int a=ISOAtanAngle[S1][S2];

	a=(a-30)>0?(a-30):0;
	if(Smax[0]== 'R')
	{
		if(Smax[1]=='B')
			a=a+180;
		else
			a=300-a;//360+(360-a);
	}
	else
	{
		if(Smax[0]== 'B')
		{
			if(Smax[1]=='G')
				a=a+60;//120+a;
			else 
				a=180-a;//360+120-a;
		}
		else
		{
			if(Smax[0]=='G')
			{
				if(Smax[1]=='R')
					a=a+300;//240+a;
				else 
					a=60-a;//360+240-a;
			}
			else
			{
				ASSERT(0);
				//AfxMessageBox("!!!!!!!!");
			}
		}
	}	
	dotiso.varH=H ;   //色亮  (明/亮)
	dotiso.varS=S ;   //色彩  （颜色的鲜艳饱和 程度） 
	dotiso.varO=a ;   //色度   （颜色的色调 红<->黄<->蓝 ）
	return dotiso;
}


void CROIGrid::GetGridMean(void *pgdi)
{
	//CGDIImage32 *pGDIimage = (CGDIImage32*) pgdi;
	//if(pGDIimage->IsNull())
	//	return;
	//int igridsize = m_grid.size();
	//Rect_Shape *prect =NULL;
	//int x0=GetOrgX();
	//int y0=GetOrgY();
	//for(int i = 0;i<igridsize;i++)
	//{
	//	pGDIimage->ImageROI_Mean(m_grid[i].Ax+x0,m_grid[i].Ay+y0,m_imodelmatrixsum,m_imodelmatrixsum);
	//	m_rgbmean[i].G = pGDIimage->Get_ROI_Mean_G();
	//	m_rgbmean[i].R = pGDIimage->Get_ROI_Mean_R();
	//	m_rgbmean[i].B = pGDIimage->Get_ROI_Mean_B();
	//	prect->SetColor(WHITE,RGB(m_rgbmean[i].B,m_rgbmean[i].G,m_rgbmean[i].R));
	//}
}
//double CROIGrid::GetMaxGridMean_H()
//{
//	int igridsize = m_grid.size();
//	thrdint dotiso;
//	int imax =0;
//	for(int i=0;i<igridsize;i++)
//	{
//		dotiso=DotISO_FAST(m_rgbmean[i]);
//		if(dotiso.varH>imax)	
//			imax = dotiso.varH;
//	}
//	return imax;
//}

void CROIGrid::DrawPoints(HDC hdc,int iLineSize,COLORREF CrlRef,int imx,int imy)
{ 
	//HPEN pen = CreatePen(PS_SOLID, iLineSize, CrlRef);
	//if (pen != NULL)
	//	SelectObject(hdc,(HPEN)pen);
 
	//GridPoints::iterator pIter;
	//for(pIter = m_gridP.begin();pIter!=m_gridP.end();pIter++)
	//{
	//	POINT_LP *lpoint = pIter->second.getalloc();
	//	int isize =pIter->second.getisum();
	//	int ix0 = lpoint->x;
	//	int iy0 = lpoint->y;

	//	if(ix0>0&&iy0>0)
	//		MoveToEx(hdc,ix0+imx,iy0+imy,(LPPOINT) NULL);
	//	int ix1 = 0;
	//	int iy1 = 0;
	//	int igapx = 0;
	//	int igapy = 0;
	//	for(int i=1;i<isize;i++)
	//	{ 
	//		lpoint = lpoint +1;
	//		ix1 = lpoint->x;
	//		iy1 = lpoint->y;
	//		igapx = ix1 - ix0;
	//		igapy = iy1 - iy0;
	//		igapx = igapx>0?igapx:-igapx;
	//		igapy = igapy>0?igapy:-igapy;

	//		if(ix1>0&&iy1>0
	//			&&igapx<10&&igapy<10)
	//		{
	//			LineTo(hdc,ix1+imx,iy1+imy);
	//		}
	//		else
	//		{
	//			MoveToEx(hdc,ix1+imx,iy1+imy,(LPPOINT) NULL);
	//		}
	//		ix0 = ix1;
	//		iy0 = iy1;
	//	}

	//}
	//DeleteObject(pen);

}
void CROIGrid::DrawResult(HDC hdc,int imx,int imy,int ixoffset,int iyoffset)
{
	GridObjects:: iterator pIter;
	for ( pIter = m_gridR.begin( ) ; pIter != m_gridR.end( ) ; pIter++ )
	{
		RelationVect objectvect = pIter->second;	
		int iobjsize =  objectvect.size();
		int imaxnum =0;
		if(iobjsize>2)
		{
			int imaxsize =0;
			ObjRelation arelation;
			for(int i=0;i<iobjsize;i++)
			{
				arelation = objectvect[i];
				if(arelation.m_isize>imaxsize)
				{
					imaxnum = i;
					imaxsize = arelation.m_isize;
				}
			}
		} 
		ObjRelation arelation = objectvect[imaxnum];
		
		int icentx = (arelation.m_irightx + arelation.m_ileftx)/2;
		int icenty = (arelation.m_ibottomy + arelation.m_itopy)/2;
		//int iturex = icentx + arelation.m_igdigridx * 640;
		//int iturey = icenty + arelation.m_igdigridy * 480 + 35;
		//int itruew = arelation.m_irightx - arelation.m_ileftx;
		//int itrueh = arelation.m_ibottomy - arelation.m_itopy;
		//itruew = itruew>0?itruew:-itruew;
		//itrueh = itrueh>0?itrueh:-itrueh;
		//double dwrate = (arelation.m_irightx - arelation.m_ileftx)/(1.0*m_irealw);
		//double dhrate = (arelation.m_ibottomy - arelation.m_itopy)/(1.0*m_irealh);
		//dwrate = dwrate>0?dwrate:-dwrate;
		//dhrate = dhrate>0?dhrate:-dhrate;
		icentx = (icentx*m_iwgridgap) /m_irealw;
		icenty = (icenty*m_ihgridgap) /m_irealh;
		icentx = arelation.m_ishowgridx * m_iwgridgap + icentx;
		icenty = arelation.m_ishowgridy * m_ihgridgap + icenty;
		 
		//int idwgap = 5;
		//if(dwrate<0.2)
		//{
		//	idwgap = 5;
		//	itruew =80;
		//}
		//else 
		//{ 
		//	idwgap = dwrate*m_iwgridgap*0.5;
		//}
		//int idhgap = 5;
		//if(dhrate<0.2)
		//{
		//	idhgap = 5;
		//	itrueh =80;
		//}
		//else 
		//{ 
		//	idhgap = dhrate*m_ihgridgap*0.5;
		//}	
		//if(iobjsize>5)
		//{
		//	itruew =180;
		//	itrueh =180;
		//}
		//Rect_Shape arect( icentx-idwgap,icenty-idhgap,icentx+idwgap,icenty+idhgap);
		//arect.draw(hdc,m_iorgx+imx,m_iorgy+imy,RED,2);
		int itype =arelation.m_itype;
		if(itype&GRIDType_FAIL)
		{
			int ix = icentx + m_iorgx + imx;
			int iy = icenty + m_iorgy + imy;
			
			ShowPartImageA(hdc,ix,iy,imx,imy,ixoffset,iyoffset);
			//SelectObject(hdc,GetStockObject(DC_PEN));
			//SetDCPenColor(hdc,BLUE );
			//MoveToEx(hdc,icentx+m_iorgx+imx ,icenty+m_iorgy+imy,(LPPOINT) NULL);

			//ShowGridimage(hdc,  arelation.m_ishowgridx, arelation.m_ishowgridy,iturex -itruew/2,iturey -itrueh/2,itruew,itrueh,imx+m_iorgx,imy+m_iorgy);

		}
	}
}
void CROIGrid::DrawTypeFont(HDC hdc,int imx,int imy)
{

	//////////////////////////////////////////////////////
	CDC dc;
	dc.Attach(hdc);

	GridType:: iterator pIter;
	for ( pIter = m_gridT.begin( ) ; pIter != m_gridT.end( ) ; pIter++ )
	{ 
		int itype = m_gridT[pIter->first];
		int ipos = pIter->first.find(','); 
		int ix = atoi(pIter->first.c_str()) + imx;
		int iy = atoi(pIter->first.c_str() +1+ ipos) + imy; 
		int icurx =  m_iwgridgap*ix;
		int icury =  m_ihgridgap*iy;
		//Rect_Shape arect(icurx,icury,icurx+m_igridw,icury+m_igridh);
		if(itype!=0)
		{

			CString strtypenum;
			strtypenum.Format("%d",itype);
			dc.SetBkMode(TRANSPARENT);
			dc.SetTextColor(BLUE);
			dc.TextOut(icurx,icury,strtypenum);
		}

	}

	dc.Detach();
	//int x0= m_iorgx;
	//int y0= m_iorgy;
	RectMap:: iterator pRectIter;
	for ( pRectIter = m_grid.begin( ) ; pRectIter != m_grid.end( ) ; pRectIter++ )
	{/*1 2 4 8 16 32 64*/
		int itype = m_gridT[pRectIter->first];
		if(itype!=0)
			pRectIter->second.draw(hdc,imx,imy,itype,log(itype*1.0));	 
	}


}
void CROIGrid::DrawResultCximage(HDC hdc,int imx,int imy)
{
	GridObjects:: iterator pIter;
	for ( pIter = m_gridR.begin( ) ; pIter != m_gridR.end( ) ; pIter++ )
	{
		RelationVect objectvect = pIter->second;	  
		for(int i=0;i<objectvect.size();i++)
		{

			ObjRelation arelation = objectvect[i];

			int icentx = (arelation.m_irightx + arelation.m_ileftx)/2;
			int icenty = (arelation.m_ibottomy + arelation.m_itopy)/2;

			double dwrate = (arelation.m_irightx - arelation.m_ileftx)/(1.0*m_irealw);
			double dhrate = (arelation.m_ibottomy - arelation.m_itopy)/(1.0*m_irealh);
			dwrate = dwrate>0?dwrate:-dwrate;
			dhrate = dhrate>0?dhrate:-dhrate;
			icentx = (icentx*m_iwgridgap) /m_irealw;
			icenty = (icenty*m_ihgridgap) /m_irealh;
			icentx = arelation.m_ishowgridx * m_iwgridgap + icentx;
			icenty = arelation.m_ishowgridy * m_ihgridgap + icenty;
			int idwgap = 5;
			if(dwrate<0.2)
				idwgap = 5;
			else 
			{ 
				idwgap = dwrate*m_iwgridgap*0.5;
			}
			int idhgap = 5;
			if(dhrate<0.2)
				idhgap = 5;
			else 
			{ 
				idhgap = dhrate*m_ihgridgap*0.5;
			}	
			Rect_Shape arect(icentx-idwgap,icenty-idhgap,icentx+idwgap,icenty+idhgap);
			int itype =arelation.m_itype;
			if(itype==2)
				arect.draw(hdc,m_iorgx+imx,m_iorgy+imy,RED,1);
		}
	}
}
void CROIGrid::GetCximage(void *pimage)
{
	m_pcximage = (CxVisionImage *)pimage;
}

void CROIGrid::GetMObj(void *pobject)
{
	m_pobj = (CObjectAnalysisExA*)pobject;
}
void CROIGrid::GetGDI(void *pgdi)
{
	m_pgdi = (CGDIImage32*)pgdi;

}
void CROIGrid::ShowGridimage(HDC hdc,int igridx,int igridy,int ioffsetx,int ioffsety,int iw,int ih,int imx,int imy)
{
	int icurx =  m_iwgridgap*igridx ;
	int icury =  m_ihgridgap*igridy ;


	LineTo(hdc,icurx+ imx+100-2,icury+imy- 100-2);
	LineTo(hdc,icurx+ imx+100+2+m_ishowpicw,icury+imy- 100-2);
	LineTo(hdc,icurx+ imx+100+2+m_ishowpicw,icury+imy- 100+2+m_ishowpich);
	LineTo(hdc,icurx+ imx+100-2,icury+imy- 100+2+m_ishowpich);
	LineTo(hdc,icurx+ imx+100-2,icury+imy- 100-2);
	int igdiy =  GetGridGDIPosY(igridy);
	if(m_pcximage!=NULL)
		m_pcximage->StretchPatA(hdc,icurx+ imx+100,icury+imy- 100,m_ishowpicw,m_ishowpich, ioffsetx,ioffsety,iw,ih );
}
void CROIGrid::ShowPartImage(HDC hdc,int ix0,int iy0,int imx,int imy,int ixoffset,int iyoffset)
{
//////////////////////////////////////////////////////////////////////////
	//arect.draw(hdc,m_iorgx+imx,m_iorgy+imy,RED,2);

	//SelectObject(hdc,GetStockObject(DC_PEN));
	//SetDCPenColor(hdc,BLUE );
	//MoveToEx(hdc,icentx+m_iorgx+imx ,icenty+m_iorgy+imy,(LPPOINT) NULL);

	//ShowGridimage(hdc,  arelation.m_ishowgridx, arelation.m_ishowgridy,iturex -itruew/2,iturey -itrueh/2,itruew,itrueh,imx+m_iorgx,imy+m_iorgy);

//////////////////////////////////////////////////////////////////////////
	int iorgx = m_iorgx;
	int iorgy = m_iorgy;

	
	int imx0 = imx - ixoffset;
	int imy0 = imy - iyoffset;
	
	int ix = ix0 + imx0  ;
	int iy = iy0 + imy0  ;
	
	
	
	int iw = 16;
	int ih = 16;
	int iw0 = iw;
	int ih0 = ih;
	int ishowgap = 4;
	iw = iw*ishowgap;
	ih = ih*ishowgap;

	int icurx = ix -imx0-ixoffset ;
	int icury = iy -imy0-iyoffset ;

	icurx = icurx * 16 ;
	icury = icury * 16 ;
	
	int ilinelen = 100;
	
	int imaxw = 0;
	int imaxh = 0;

	if(m_pcximage!=NULL)
	{
		imaxw = m_pcximage->GetImageWidth();
		imaxh = m_pcximage->GetImageHeigh();
		
		icury = imaxh - icury ;
		icurx = icurx -(iw/2);
		icury = icury -(ih/2);
	}

	if(imaxw>icurx
	&&imaxh>icury)
	{
		SelectObject(hdc,GetStockObject(DC_PEN));
		SetDCPenColor(hdc,RED );

		MoveToEx(hdc,ix -ishowgap,iy -ishowgap ,(LPPOINT) NULL);
		LineTo(hdc,ix +ishowgap ,iy -ishowgap);
		LineTo(hdc,ix +ishowgap ,iy +ishowgap);
		LineTo(hdc,ix -ishowgap ,iy +ishowgap );
		LineTo(hdc,ix -ishowgap,iy -ishowgap );
 
		
		SelectObject(hdc,GetStockObject(DC_PEN));
		SetDCPenColor(hdc,BLUE );
		MoveToEx(hdc,ix ,iy ,(LPPOINT) NULL);

		LineTo(hdc,ix+100-2 ,iy- 100-2 );
		LineTo(hdc,ix+100+2+iw ,iy- 100-2 );
		LineTo(hdc,ix+100+2+iw ,iy- 100+2+ih );
		LineTo(hdc,ix+100-2 ,iy- 100+2+ih );
		LineTo(hdc,ix+100-2 ,iy- 100-2 ); 

		MoveToEx(hdc,ix+100+2+iw ,iy- 100+2+ih ,(LPPOINT) NULL);
		LineTo(hdc,ix ,iy ); 

		
		if(m_pcximage!=NULL)
			m_pcximage->StretchPatA(hdc,ix + 100 ,iy - 100 ,iw,ih, icurx ,icury,iw,ih );
	
	}

}
void CROIGrid::ShowPartImageA(HDC hdc,int ix,int iy,int imx,int imy,int ixoffset,int iyoffset)
{
	int iorgx = m_iorgx;
	int iorgy = m_iorgy;


	int imx0 = imx - ixoffset;
	int imy0 = imy - iyoffset;

//	int ix = ix0 + imx0  ;
//	int iy = iy0 + imy0  ;



	int iw = 16;
	int ih = 16;
	int iw0 = iw;
	int ih0 = ih;
	int ishowgap = 4;
	iw = iw*ishowgap;
	ih = ih*ishowgap;

	int icurx = ix -imx0-ixoffset ;
	int icury = iy -imy0-iyoffset ;

	icurx = icurx * 16 ;
	icury = icury * 16 ;

	int ilinelen = 100;

	int imaxw = 0;
	int imaxh = 0;

	if(m_pcximage!=NULL)
	{
		imaxw = m_pcximage->GetImageWidth();
		imaxh = m_pcximage->GetImageHeigh();

		icury = imaxh - icury ;
		icurx = icurx -(iw/2);
		icury = icury -(ih/2);
	}

	if(imaxw>icurx
		&&imaxh>icury)
	{
		SelectObject(hdc,GetStockObject(DC_PEN));
		SetDCPenColor(hdc,RED );

		MoveToEx(hdc,ix -ishowgap,iy -ishowgap ,(LPPOINT) NULL);
		LineTo(hdc,ix +ishowgap ,iy -ishowgap);
		LineTo(hdc,ix +ishowgap ,iy +ishowgap);
		LineTo(hdc,ix -ishowgap ,iy +ishowgap );
		LineTo(hdc,ix -ishowgap,iy -ishowgap );


		SelectObject(hdc,GetStockObject(DC_PEN));
		SetDCPenColor(hdc,BLUE );
		MoveToEx(hdc,ix ,iy ,(LPPOINT) NULL);

		LineTo(hdc,ix+100-2 ,iy- 100-2 );
		LineTo(hdc,ix+100+2+iw ,iy- 100-2 );
		LineTo(hdc,ix+100+2+iw ,iy- 100+2+ih );
		LineTo(hdc,ix+100-2 ,iy- 100+2+ih );
		LineTo(hdc,ix+100-2 ,iy- 100-2 ); 

		MoveToEx(hdc,ix+100+2+iw ,iy- 100+2+ih ,(LPPOINT) NULL);
		LineTo(hdc,ix ,iy ); 


		if(m_pcximage!=NULL)
			m_pcximage->StretchPatA(hdc,ix + 100 ,iy - 100 ,iw,ih, icurx ,icury,iw,ih );

	}
}
void CROIGrid::FindFicture(int igridx,int igridy,int ithre,int iminobj,int imaxobj)
{
	//int icurx =  m_iwgridgap*igridx ;
	//int icury =  m_ihgridgap*igridy ;
	//int igdiy =  GetGridGDIPosY(igridy);
	int	igdiy = GetGridGDIPosY(igridy);
	 
	 if(m_pcximage!=NULL)
	m_pcximage->SetCxImageROI(igridx*640 ,igdiy*480,656,496);
	 if(m_pcximage!=NULL)
	m_pcximage->CxImageROIToGDI(m_pgdi);
	 if(m_pgdi!=NULL)
	m_pgdi->Image_Threshold(ithre);
	 if(m_pgdi!=NULL)
	m_pobj->SetFindDistance(8);
	 if(m_pgdi!=NULL)
 	m_pobj->SetFontShow(2);
 	 if(m_pgdi!=NULL)
	m_pobj->SetArea(iminobj,imaxobj,100,230);
	 if(m_pgdi!=NULL)
	m_pobj->GDIMeasure_org_GDIMAP_Grid_AB(m_pgdi);
	
 //	m_ccd1_value1 = m_ccd1_aobjectx.getobjectsize();
 //	m_ccd1_value2 = m_ccd1_aobjectx.getobjectarea(0);
   
    SetCurGrid(igridx,igridy);
	GetMObject(m_pobj);

//	agrid.setcurgrid(dccd1gridx,dccd1gridy);
//	agrid.getmobject(m_ccd1_aobjectx);
}
int CROIGrid::GetFictureGridX(int inum)
{
	int ichecknum = 0;
	GridType:: iterator pIter;
	for ( pIter = m_gridT.begin( ) ; pIter != m_gridT.end( ) ; pIter++ )
	{ 
		int itype = m_gridT[pIter->first];
		if(itype&GRIDType_FAIL)
		{
			if(ichecknum==inum)
			{
				int ipos = pIter->first.find(','); 
				int ix = atoi(pIter->first.c_str());
				int iy = atoi(pIter->first.c_str() +1+ ipos);
				return ix;
			}
			ichecknum = ichecknum +1;
		}
	}
}
int CROIGrid::GetFictureGridY(int inum)
{
	int ichecknum = 0;
/*
int itype = m_gridT[pIter->first];
int ipos = pIter->first.find(','); 
int ix = atoi(pIter->first.c_str());
int iy = atoi(pIter->first.c_str() +1+ ipos);
*/
	GridType:: iterator pIter;
	for ( pIter = m_gridT.begin( ) ; pIter != m_gridT.end( ) ; pIter++ )
	{ 
		int itype = m_gridT[pIter->first];
		if(itype&GRIDType_FAIL)
		{
			if(ichecknum==inum)
			{
				int ipos = pIter->first.find(','); 
				int ix = atoi(pIter->first.c_str());
				int iy = atoi(pIter->first.c_str() +1+ ipos);
				return iy;
			}
			ichecknum = ichecknum +1;
		}
	}
}

void CROIGrid::CheckGrid(int ithre,int iminobj,int imaxobj)
{
#ifdef DEMOSHOW//演示版本
	for(int idemo=0;idemo<10;idemo++)
		Sleep(10);
#endif
	int ichecknum = 0;

	ichecknum =GetFictureSum();
	if(ichecknum>20)
		return;
	ClearResult();

	GridType:: iterator pIter;
	for ( pIter = m_gridT.begin( ) ; pIter != m_gridT.end( ) ; pIter++ )
	{ 
		int itype =  pIter->second;
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		if((itype!=GRIDType_NONE)
		&&(!(itype&GRIDType_GROUP))
		&&(!(itype&GRIDType_EDGE)) )
		{
			pIter->second = GRIDType_THING;
			int ipos = pIter->first.find(','); 
			int ix = atoi(pIter->first.c_str());
			int iy = atoi(pIter->first.c_str() +1+ ipos);
			
			FindFicture(ix,iy,ithre,iminobj,imaxobj);
		}
	}
}
int CROIGrid::GetFictureSum()
{
	int ichecknum = 0;

	GridType:: iterator pIter;
	for ( pIter = m_gridT.begin( ) ; pIter != m_gridT.end( ) ; pIter++ )
	{ 
		int itype = pIter->second;
		int ipos = pIter->first.find(','); 
		int ix = atoi(pIter->first.c_str());
		int iy = atoi(pIter->first.c_str() +1+ ipos);
		if(ix>=0&&iy>=0)
		{ 
			if(itype&GRIDType_FAIL)
				ichecknum = ichecknum +1;
		}
	}
	return ichecknum;
}
void CROIGrid::FictureTotal()
{

	m_ficlist.clear();
	GridType:: iterator pIter;
	for ( pIter = m_gridT.begin( ) ; pIter != m_gridT.end( ) ; pIter++ )
	{  

		int itype = pIter->second;
		int ipos = pIter->first.find(','); 
		int ix = atoi(pIter->first.c_str());
		int iy = atoi(pIter->first.c_str() +1+ ipos);
		if(ix>=0&&iy>=0)
		if(itype&GRIDType_FAIL)
		{ 
			POINT_LP apoint;
			apoint.x = ix;
			apoint.y = iy;			
			m_ficlist.push_back(apoint);
		}	
	}	
}

int CROIGrid::FictureCalibX(int inum)
{
	if(m_ficlist.size()>inum)
	return m_ficlist[inum].x;
	else
	return -1;
	
}
int CROIGrid::FictureCalibY(int inum)
{
	if(m_ficlist.size()>inum)
		return m_ficlist[inum].y;
	else
		return -1;
}
int CROIGrid::GetFictureSize(int idnum)
{
	int inum = 0;
	GridObjects:: iterator pIter;
	
	for ( pIter = m_gridR.begin( ) ; pIter != m_gridR.end( ) ; pIter++ )
	{
		RelationVect objectvect = pIter->second;	  
		for(int i=0;i<objectvect.size();i++)
		{
			if(inum == idnum)
			{
				ObjRelation arelation = objectvect[i];
				return arelation.m_isize;
			}
			inum = inum +1;
		}
	}
}
int CROIGrid::GetGridFictureSize(int ix,int iy)
{
	int inum = 0;
	int isize = 0;
	char chnum[25];

	sprintf(chnum, "%d,%d" ,ix,iy);

	RelationVect objectvect = m_gridR[std::string(chnum)];
	for(int i=0;i<objectvect.size();i++)
	{ 
 			ObjRelation arelation = objectvect[i];
			isize =isize + arelation.m_isize;
	}
	return isize;
	//GridObjects:: iterator pIter;

	//for ( pIter = m_gridR.begin( ) ; pIter != m_gridR.end( ) ; pIter++ )
	//{
	//	RelationVect objectvect = m_gridR;	  
	//	for(int i=0;i<objectvect.size();i++)
	//	{
	//		if(inum == idnum)
	//		{
	//			ObjRelation arelation = objectvect[i];
	//			return arelation.m_isize;
	//		}
	//		inum = inum +1;
	//	}
	//}
}

void CROIGrid::ShowFicture(int ishowx,int ishowy,int ishoww,int ishowh)
{
 
}
void CROIGrid::SetRelationPos(int ix,int iy,int iw,int ih)
{
	m_irelationX = ix;
	m_irelationY = iy;
	m_irelationW = iw;
	m_irelationH = ih;
}
void CROIGrid::SetRelationGridA(void *pgrid)
{
	m_prelationgrid1 = (CROIGrid*) pgrid;
}
void CROIGrid::SetRelationGridB(void *pgrid)
{
	m_prelationgrid2 = (CROIGrid*) pgrid;
}
int CROIGrid::GetRelationType(int ix,int iy)
{
	if(NULL==m_prelationgrid1
	||NULL==m_prelationgrid2)
		return 0;
	int ix0_0 = m_irelationX;
	int iy0_0 = m_irelationY;
	int ix0_1 = m_irelationX + m_iwgridsum;
	int iy0_1 = m_irelationY + m_ihgridsum;
	
	int ix1_0 = m_prelationgrid1->m_irelationX;
	int iy1_0 = m_prelationgrid1->m_irelationY;
	int ix1_1 = m_prelationgrid1->m_irelationX + m_prelationgrid1->m_iwgridsum;
	int iy1_1 = m_prelationgrid1->m_irelationY + m_prelationgrid1->m_ihgridsum;
	
	int ix2_0 = m_prelationgrid2->m_irelationX;
	int iy2_0 = m_prelationgrid2->m_irelationY;
	int ix2_1 = m_prelationgrid2->m_irelationX + m_prelationgrid2->m_iwgridsum;
	int iy2_1 = m_prelationgrid2->m_irelationY + m_prelationgrid2->m_ihgridsum;

	int itx = 0;
	int ity = 0;
	if(ix>=ix0_0&&iy>=iy0_0
	&&ix<ix0_1&&iy<iy0_1)
	{
		itx = ix - m_irelationX;
		ity = iy - m_irelationY;
		char chnum[25];
		int iny = m_ihgridsum - ity - 1;
		sprintf(chnum, "%d,%d" ,itx,iny);
		return m_gridT[std::string(chnum)] ; 
	}
	else if(ix>=ix1_0&&iy>=iy1_0
		&&ix<ix1_1&&iy<iy1_1)
	{
		itx = ix -  m_prelationgrid1->m_irelationX;
		ity = iy -  m_prelationgrid1->m_irelationY;
		char chnum[25];
		int iny = m_ihgridsum - ity - 1;
		sprintf(chnum, "%d,%d" ,itx,iny);
		return m_prelationgrid1->m_gridT[std::string(chnum)] ; 
	}	
	else if(ix>=ix2_0&&iy>=iy2_0
		&&ix<ix2_1&&iy<iy2_1)
	{
		itx = ix -  m_prelationgrid2->m_irelationX;
		ity = iy -  m_prelationgrid2->m_irelationY;
		char chnum[25];
		int iny = m_ihgridsum - ity - 1;
		sprintf(chnum, "%d,%d" ,itx,iny);
		return m_prelationgrid2->m_gridT[std::string(chnum)] ; 
	}
}
void CROIGrid::SetRelationType(int ix,int iy,int itype)
{
	if(NULL==m_prelationgrid1
		||NULL==m_prelationgrid2)
		return ;
	int ix0_0 = m_irelationX;
	int iy0_0 = m_irelationY;
	int ix0_1 = m_irelationX + m_iwgridsum;
	int iy0_1 = m_irelationY + m_ihgridsum;

	int ix1_0 = m_prelationgrid1->m_irelationX;
	int iy1_0 = m_prelationgrid1->m_irelationY;
	int ix1_1 = m_prelationgrid1->m_irelationX + m_prelationgrid1->m_iwgridsum;
	int iy1_1 = m_prelationgrid1->m_irelationY + m_prelationgrid1->m_ihgridsum;

	int ix2_0 = m_prelationgrid2->m_irelationX;
	int iy2_0 = m_prelationgrid2->m_irelationY;
	int ix2_1 = m_prelationgrid2->m_irelationX + m_prelationgrid2->m_iwgridsum;
	int iy2_1 = m_prelationgrid2->m_irelationY + m_prelationgrid2->m_ihgridsum;

	int itx = 0;
	int ity = 0;
	if(ix>=ix0_0&&iy>=iy0_0
		&&ix<ix0_1&&iy<iy0_1)
	{
		itx = ix - m_irelationX;
		ity = iy - m_irelationY;
		char chnum[25];
		int iny = m_ihgridsum - ity - 1;
		sprintf(chnum, "%d,%d" ,itx,iny);
		m_gridT[std::string(chnum)] = itype ; 
	}
	else if(ix>=ix1_0&&iy>=iy1_0
		&&ix<ix1_1&&iy<iy1_1)
	{
		itx = ix -  m_prelationgrid1->m_irelationX;
		ity = iy -  m_prelationgrid1->m_irelationY;
		char chnum[25];
		int iny = m_prelationgrid1->m_ihgridsum - ity - 1;
		sprintf(chnum, "%d,%d" ,itx,iny);
		m_prelationgrid1->m_gridT[std::string(chnum)]  = itype; 
	}	
	else if(ix>=ix2_0&&iy>=iy2_0
		&&ix<ix2_1&&iy<iy2_1)
	{
		itx = ix -  m_prelationgrid2->m_irelationX;
		ity = iy -  m_prelationgrid2->m_irelationY;
		char chnum[25];
		int iny = m_prelationgrid1->m_ihgridsum - ity - 1;
		sprintf(chnum, "%d,%d" ,itx,iny);
		m_prelationgrid2->m_gridT[std::string(chnum)] = itype ; 
	}
}

void CROIGrid::DrawTypeFont(HDC hdc)
{
 
//////////////////////////////////////////////////////
	CDC dc;
	dc.Attach(hdc);
 
	GridType:: iterator pIter;
	for ( pIter = m_gridT.begin( ) ; pIter != m_gridT.end( ) ; pIter++ )
	{ 
		int itype = m_gridT[pIter->first];
		int ipos = pIter->first.find(','); 
		int ix = atoi(pIter->first.c_str());
		int iy = atoi(pIter->first.c_str() +1+ ipos); 
		int icurx =  m_iwgridgap*ix;
		int icury =  m_ihgridgap*iy;
		//Rect_Shape arect(icurx,icury,icurx+m_igridw,icury+m_igridh);
		if(itype!=0)
		{

			CString strtypenum;
			strtypenum.Format("%d",itype);
			dc.SetBkMode(TRANSPARENT);
			dc.SetTextColor(BLUE);
			dc.TextOut(icurx,icury,strtypenum);
		}

	}
  
	dc.Detach();
	int x0= m_iorgx;
	int y0= m_iorgy;
	RectMap:: iterator pRectIter;
	for ( pRectIter = m_grid.begin( ) ; pRectIter != m_grid.end( ) ; pRectIter++ )
	{/*1 2 4 8 16 32 64*/
		int itype = m_gridT[pRectIter->first];
		if(itype!=0)
		pRectIter->second.draw(hdc,x0,y0,itype,log(itype*1.0));	 
	}
 

}
void CROIGrid::ObjectGridType()
{
	//m_pobj = (CObjectAnalysisExA*) pobj;
	if(NULL==m_pobj)
		return;
	Rect_Shape arect;
	int iobjsize = m_pobj->GetObjectSize();
	for(int i=0;i<iobjsize;i++)
	{
		arect = m_pobj->GetObjectRect(i);
		//if(arect!=NULL)
		{
			int icentx = (arect.m_Ax+arect.m_Cx)/2;
			int icenty = (arect.m_Ay+arect.m_Cy)/2;
			int igridx = icentx/m_iwgridgap;
			int igridy = icenty/m_ihgridgap;

			int ivalue = GetTypeValue(igridx,igridy);
			
			//int iarea = m_pobj->GetObjWH(i);
			int iarea = 1;

			ivalue = ivalue + iarea;

			SetTypeValue(igridx,igridy,ivalue);

		}
	}
}
int CROIGrid::ObjectGridTypeCal(int icalway)
{
	char chnum[25];
	char chmax[25];
	int imaxvalue = 0;
	switch(icalway)
	{
		case 0:
		
		break;
		case 4:
		//////////////////////////////////////////////////////////////////////////
 

			for(int ish=0;ish<m_ihgridsum-1;ish++)
			{
				for(int isw=0;isw<m_iwgridsum-1;isw++)
				{  
					sprintf(chnum, "%d,%d" ,isw,ish);
					int icurvalue0 = m_gridT[std::string(chnum)] ;
					sprintf(chnum, "%d,%d" ,isw+1,ish);
					int icurvalue1 = m_gridT[std::string(chnum)] ;
					sprintf(chnum, "%d,%d" ,isw,ish+1);
					int icurvalue2 = m_gridT[std::string(chnum)] ;
					sprintf(chnum, "%d,%d" ,isw+1,ish+1);
					int icurvalue3 = m_gridT[std::string(chnum)] ;
					
					
					sprintf(chnum, "%d,%d" ,isw,ish);
					int icurvaluetal = icurvalue3 + icurvalue2 +icurvalue1 +icurvalue0;
					if(imaxvalue<icurvaluetal)
					{
						imaxvalue = icurvaluetal;
						sprintf(chmax, "%d,%d" ,isw,ish);
					}
 


				}
			}
			return imaxvalue;
		break;
	}
}