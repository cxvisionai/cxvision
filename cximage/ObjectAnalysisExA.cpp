
#include "ObjectAnalysisExA.h"


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

//////UPCODE////////////////////////////////////////////////////////////////////
#define MAP(ix,iy) m_pgdi->CopyImagePointColor(ix,iy)
#define MAP_pixel(ix,iy) GetPixelValue(m_pgdi->CopyImagePointColor(ix,iy))
#define MAP_service(ix,iy) GetServiceValue(m_pgdi->CopyImagePointColor(ix,iy))
#define MAP_analysis(ix,iy) GetAnalysisValue(m_pgdi->CopyImagePointColor(ix,iy))
#define MAP_edge(ix,iy) GetEdgeValue(m_pgdi->CopyImagePointColor(ix,iy))

#define SetMAP_pixel(ix,iy,ivalue) m_pgdi->SetCopyImagePixelValue(ix,iy,ivalue)
#define SetMAP_service(ix,iy,ivalue) m_pgdi->SetCopyImageServiceValue(ix,iy,ivalue)
#define SetMAP_analysis(ix,iy,ivalue) m_pgdi->SetCopyImageAnalysisValue(ix,iy,ivalue)
#define SetMAP_edge(ix,iy,ivalue) m_pgdi->SetCopyImageEdgeValue(ix,iy,ivalue)
//////GDIManager Map////////////////////////////////////////////////////////////////////
#define ExMAP(ix,iy) m_gdialloc->PointColor(ix,iy)
#define ExMAP_pixel(ix,iy) GetPixelValue(m_gdialloc->PointColor(ix,iy))
#define ExMAP_service(ix,iy) GetServiceValue(m_gdialloc->PointColor(ix,iy))
#define ExMAP_analysis(ix,iy) GetAnalysisValue(m_gdialloc->PointColor(ix,iy))
#define ExMAP_edge(ix,iy) GetEdgeValue(m_gdialloc->PointColor(ix,iy))

#define ExSetMAP_pixel(ix,iy,ivalue) m_gdialloc->SetImagePixelValue(ix,iy,ivalue)
#define ExSetMAP_service(ix,iy,ivalue) m_gdialloc->SetImageServiceValue(ix,iy,ivalue)
#define ExSetMAP_analysis(ix,iy,ivalue) m_gdialloc->SetImageAnalysisValue(ix,iy,ivalue)
#define ExSetMAP_edge(ix,iy,ivalue) m_gdialloc->SetImageEdgeValue(ix,iy,ivalue)
//////GDI Shape Map////////////////////////////////////////////////////////////////////
#define ExSHAPEMAP(ix,iy) m_gdishapemap->PointColor(ix,iy)
#define ExSHAPEMAP_pixel(ix,iy) GetPixelValue(m_gdishapemap->PointColor(ix,iy))
#define ExSHAPEMAP_service(ix,iy) GetServiceValue(m_gdishapemap->PointColor(ix,iy))
#define ExSHAPEMAP_analysis(ix,iy) GetAnalysisValue(m_gdishapemap->PointColor(ix,iy))
#define ExSHAPEMAP_edge(ix,iy) GetEdgeValue(m_gdishapemap->PointColor(ix,iy))

#define ExSetSHAPEMAP_pixel(ix,iy,ivalue) m_gdishapemap->SetImagePixelValue(ix,iy,ivalue)
#define ExSetSHAPEMAP_service(ix,iy,ivalue) m_gdishapemap->SetImageServiceValue(ix,iy,ivalue)
#define ExSetSHAPEMAP_analysis(ix,iy,ivalue) m_gdishapemap->SetImageAnalysisValue(ix,iy,ivalue)
#define ExSetSHAPEMAP_edge(ix,iy,ivalue) m_gdishapemap->SetImageEdgeValue(ix,iy,ivalue)

CObjectAnalysisExA::CObjectAnalysisExA(void)
{
        m_iminarea=10;
        m_imaxarea=50000;
        m_pimage=NULL;

        m_ishowobj=-1;
        m_ishowedge=-1;
        m_ishowRect=-1;
        m_idistance=24;
        m_imaxpixel=30240;
        m_iminpixel=20;
        m_dthreshodgapH=1;
        m_dthreshodgapS=1;
        m_dthreshodgapO=1;
        m_ig=0;
        m_ib=0;
        m_ir=0;

        m_icurcol = 0;
        m_bshow = DrawOBJType_drawframe+DrawOBJType_drawfindobject;

        m_pgdi =NULL;

        m_isearchfirstx = 0;
        m_isearchfirsty = 0;

        m_iedgenum = 0;

        m_b_lock=true;

        m_icurobj= 0;
        m_ipointtype = 0;

        m_ifontshow =3;

        m_pmaskmap =NULL;
        m_pmaskAddress= NULL;
        m_imaskmapw =0;
        m_imaskmaph =0;
        m_measureObject = NULL;
        m_measureEdge= NULL;

        m_measureObject = new CShapeAllocA[20];
        m_measureEdge = new CShapeAllocA[20];
        m_fitelipse = new CShapeAllocA[20];
        m_fitcWH.resize(20);
        m_fangle.resize(20);

        m_psubrects = NULL;
        m_iMEASUREPOINTSUMOBJ = 100;//200;
        m_IMAGECHAR =m_iMEASUREPOINTSUMOBJ;
        m_ptype = new short[m_iMEASUREPOINTSUMOBJ];
        m_irow.resize(m_iMEASUREPOINTSUMOBJ);
        m_iBorW.resize(m_iMEASUREPOINTSUMOBJ);
        m_keypoint.resize(m_iMEASUREPOINTSUMOBJ);

        m_maskalloc = NULL;

        m_pText = new CText[m_IMAGECHAR];
        m_piobjID = new short[m_IMAGECHAR];
        m_piobjColor = new short[m_IMAGECHAR];
        m_pObjectCS = new ObjectClass[m_IMAGECHAR];
        m_pObjClass = new short[m_IMAGECHAR];
        m_pObjCID = new short[m_IMAGECHAR];
        m_pObjWH = new short[m_IMAGECHAR];
        for(int i=0;i<m_IMAGECHAR;i++)
        {
                m_pText[i].SetNumberStr(i);
                m_pText[i].SetColor(0,0,190);
                m_piobjID[i] = -1;
                m_piobjColor[i] = -1;
                m_ptype[i] = -1;
                m_pObjWH[i]=0;
                //////////////////////////////////////////////////////////////////////////
                m_pObjectCS[i].s_ID = i;
                m_pObjectCS[i].s_pU = NULL;
                m_pObjectCS[i].s_pD = NULL;
                m_pObjectCS[i].s_pL = NULL;
                m_pObjectCS[i].s_pR = NULL;
                m_pObjectCS[i].s_pO = NULL;
                m_pObjectCS[i].s_level = 0;
                m_pObjClass[i] = -1;
                m_pObjCID[i] = 0;
        }
#define RECGRECTNUM 100
        m_precgobj = new RecognizeObj[RECGRECTNUM];
        for(int i=0;i<RECGRECTNUM;i++)
        {
                m_precgobj[i].s_type = 0;
                m_precgobj[i].s_type = 0;
                m_precgobj[i].s_relation = 0;
                m_precgobj[i].s_singleW = 0;
                m_precgobj[i].s_singleH = 0;
                m_precgobj[i].s_num = 0;
        }
        m_irecognum = 0;

        m_prectfilter = new Rect_Shape[10];

#define PARTRECTNUM 80
        m_partrect = new Rect_Shape[PARTRECTNUM];
        for(int i=0;i<PARTRECTNUM;i++)
        {
                m_partrect[i].SetColor(CYAN);
        }


        m_ifilterrectnum = 0;
        m_imassnum = 0;

        m_imaptype = 1;//

        DWORD id=0;

        m_gdialloc = NULL;
        HANDLE	hSchedule=CreateThread(NULL,0,Dog_Thread_Object,this ,/*CREATE_SUSPENDED*/0 ,&id);

        m_icurmodule = CGDIManager::GetCurMode();
        m_objlistscanorA = CGDIManager::GetListScan(m_icurmodule);
        m_objlistcollectorA = CGDIManager::GetListCollect(m_icurmodule);

        m_imaskvalue = 1;
        //m_imeasurevalue =1;

        m_itoledgenum = 0;

        m_iedgethre1 = 8;
        m_iedgethre2 = 1;
        m_ifindgap = 3;

        m_dthreshodgapH =0;
        m_dthreshodgapS =0;
        m_dthreshodgapO =0;

        m_igrid = 0;
        m_colorb = 0x000ff;
        m_colorw = 0x000f0;
        m_colormask = 0x00011;

        m_gdiMask =NULL;
        m_prelationobj = NULL;
        m_iparentobjnum = 0;

        m_pihbeginvalue = NULL;
        m_pihendvalue= NULL;
        m_pivbeginvalue= NULL;
        m_pivendvalue=  NULL;

        m_ipartrectnum = 0;

        m_iminobjw = 0;
        m_imaxobjw = 9999;
        m_iminobjh = 0;
        m_imaxobjh = 9999;

        m_ifilterNedge = 0;

}

void CObjectAnalysisExA::GDIMeasure_org_GDIMAP_AAA(void *pgdi)
{

        m_pgdi = (CGDIImage32*)pgdi;
        m_imaptype = 1;
        m_igrid = 0;
        m_totalarea = 0;

        m_icurcol = 0;
        if(NULL==m_pgdi)
                return;
        if(m_pgdi->IsNull())
                return;
        if(8==m_pgdi->GetBitDepth())
        {
                GDIMeasure_org_GDIMAP_8bit(pgdi);
        }
        if(32!=m_pgdi->GetBitDepth())
                return;
        int x0=GetOrgX();
        int y0=GetOrgY();
        int iw=GetWidth();
        int ih=GetHeight();
        int x1=x0+GetWidth();
        int y1=y0+GetHeight();
        if(x0>m_pgdi->GetVisibleWidth()
                ||y0>m_pgdi->GetVisibleHeight()
                ||x1>m_pgdi->GetVisibleWidth()
                ||y1>m_pgdi->GetVisibleHeight()
                ||x0<0
                ||y0<0)
                return;


        m_pgdi->ImageCopyROIclear(0,0,iw,ih);
        int nx0=0;
        int ny0=0;
        int nx=0;
        int ny=0;
        int nservice=0;
        int nanalysis=0;
        COLORREF abyte,bytenext;
        int nTol=0;
        //收集
        int nScanerID=1;

        int icurScanerNUM=0;

        bool boverflow =false;
        //////DOWNCODE////////////////////////////////////////////////////////////////////
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
        m_prect.clear();

        CLEAR_SEARCHSEEK();
        //////UPCODE////////////////////////////////////////////////////////////////////
        //将地图第一个节点进入堆栈
        PUSH_SEARCHSEEK(0,0);
        m_pgdi->LockGDI();
        for(int icurSeekNum=0;icurSeekNum<nsearchseeksize;)//searchseek.size();)
        {
FORBEGIN:

                mapservice = MAP_service(m_objlistcollectorA[icurSeekNum].x,m_objlistcollectorA[icurSeekNum].y);

                if(mapservice>0)//当前种子节点被提取
                {
                        icurSeekNum++;

                        if(icurSeekNum>=nsearchseeksize)//searchseek.size())
                                break;
                        else
                                goto FORBEGIN;
                }
                //扫描器清空
                CLEAR_SCANOR();
                //扫描器推入种子
                PUSH_SCANOR(m_objlistcollectorA[icurSeekNum].x,m_objlistcollectorA[icurSeekNum].y);
                //重新设置种子点的状态
                SetMAP_service(m_objlistcollectorA[icurSeekNum].x,m_objlistcollectorA[icurSeekNum].y,nScanerID);
                //扫描开始
                icurScanerNUM=0;

CURSCANERBEGIN:
                while(icurScanerNUM!=nscansize)//scanor.size())
                {
                        nx0= m_objlistscanorA[icurScanerNUM].x;//pDot->m_ix;
                        ny0= m_objlistscanorA[icurScanerNUM].y;//pDot->m_iy;

                        abyte =  m_pgdi->PointColor(nx0+x0,ny0+y0);//RowAddress[ny0][nx0+nx0+nx0];//(RowAddress[ny0][nx0]);
                        abyte = abyte&0x0fff0;
                        //探索pshape
                        //判断连接法(当前点与周围点超过最远连接的为无效过滤点)
                        mapanalysis = MAP_analysis(nx0,ny0);

                        if(mapanalysis==ANLAYSIS_OVER)
                                goto NEXTFINDSTEP;
                        for(int i= 0;i<m_idistance;i++)
                        {//here a problem SEARCH_LENGTH

                                nx=nx0+G_SearchPointGroup[i].x;
                                ny=ny0+G_SearchPointGroup[i].y;
                                if(nx<=0 ||ny<=0 ||nx>=iw ||ny>=ih)
                                        continue;//搜索不在范围内

                                mapanalysis = MAP_analysis(nx,ny);

                                mapservice = MAP_service(nx,ny);

                                if(mapanalysis==ANLAYSIS_OVER
                                        ||mapservice>0//属于其他OBJECT
                                        ||-nScanerID==nservice//搜索过不属于当前OBJECT
                                        )
                                        continue;

                                bytenext = m_pgdi->PointColor(nx+x0,ny+y0) ;
                                bytenext = bytenext&0x0fff0;

                                //////////////////////////////////////////////////////////////////////////
                                //nservice 满足同一Object的条件 分析
                                if(abyte==bytenext)//满足归为一类的条件
                                {
                                        //结点推入扫描列表
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
                                        //分析完成不是同一Object //退入种子列表
                                        PUSH_SEARCHSEEK(nx,ny);
                                        SetMAP_service(nx,ny,-nScanerID);
                                }

                        }
                        //从地图上设置该点OVER
                        SetMAP_analysis(nx0,ny0,ANLAYSIS_OVER);
                        //分析扫描器上的下个节点
NEXTFINDSTEP:
                        icurScanerNUM++;

                }
CURSCANEREND:
                //if(nscansize>m_iminarea
                //	&&nscansize<m_imaxarea)
                int iobjw = imaxx - iminx;
                int iobjh = imaxy - iminy;
                if((m_ifilterNedge>0
                        &&(iminx>m_ifilterNedge
                        &&imaxx<iw-m_ifilterNedge
                        &&iminy>m_ifilterNedge
                        &&imaxy<ih-m_ifilterNedge))
                        ||m_ifilterNedge==0)
                        if(nscansize>m_iminarea
                                &&nscansize<m_imaxarea
                                &&iobjw<m_imaxobjw
                                &&iobjw>=m_iminobjw
                                &&iobjh<m_imaxobjh
                                &&iobjh>=m_iminobjh)
                        {
                                if(m_icurcol<m_IMAGECHAR)
                                        m_irow[m_icurcol] = nscansize;
                                else
                                        goto FOREND;


                                abyte= m_pgdi->PointColor( m_objlistscanorA[0].x+x0, m_objlistscanorA[0].y+y0);

                                if(m_icurcol<m_IMAGECHAR)
                                        m_iBorW[m_icurcol] = abyte&0x0fff0;

                                boverflow=false;
                                {
                                        switch(m_ifontshow)
                                        {
                                        case 3://不管黑白
                                                if(m_icurcol<m_IMAGECHAR)
                                                {
                                                        m_keypoint[ishowfont].x = m_objlistscanorA[0].x;
                                                        m_keypoint[ishowfont].y = m_objlistscanorA[0].y;
                                                        m_pText[ishowfont].SetPosition(x0+m_objlistscanorA[0].x,y0+m_objlistscanorA[0].y);
                                                        m_piobjID[ishowfont] = nScanerID;
                                                        m_piobjColor[ishowfont] = m_iBorW[m_icurcol];

                                                        m_prect.push_back(Rect_Shape(x0 + iminx,y0 + iminy,x0 + imaxx,y0 + imaxy));

                                                        //m_prect[ishowfont].m_Ax = x0 + iminx ;
                                                        //m_prect[ishowfont].m_Ay = y0 + iminy ;
                                                        //m_prect[ishowfont].m_Cx = x0 + imaxx ;
                                                        //m_prect[ishowfont].m_Cy = y0 + imaxy ;

                                                        ishowfont++;
                                                        m_icurcol ++;
                                                }

                                                break;
                                        case 0:break;//不选
                                        case 1://选白
                                                {
                                                        if(m_icurcol<m_IMAGECHAR)
                                                        {
                                                                if(m_iBorW[m_icurcol] >0)
                                                                {
                                                                        m_keypoint[ishowfont].x = m_objlistscanorA[0].x;
                                                                        m_keypoint[ishowfont].y = m_objlistscanorA[0].y;
                                                                        m_pText[ishowfont].SetPosition(x0+m_objlistscanorA[0].x,y0+m_objlistscanorA[0].y);
                                                                        m_piobjID[ishowfont] = nScanerID;
                                                                        m_piobjColor[ishowfont] = m_iBorW[m_icurcol];

                                                                        m_prect.push_back(Rect_Shape(x0 + iminx,y0 + iminy,x0 + imaxx,y0 + imaxy));
                                                                        //m_prect[ishowfont].m_Ax = x0 + iminx ;
                                                                        //m_prect[ishowfont].m_Ay = y0 + iminy ;
                                                                        //m_prect[ishowfont].m_Cx = x0 + imaxx ;
                                                                        //m_prect[ishowfont].m_Cy = y0 + imaxy ;

                                                                        m_totalarea = m_totalarea + nscansize;
                                                                        ishowfont++;
                                                                        m_icurcol ++;
                                                                }

                                                        }
                                                }
                                                break;
                                        case 2://选黑
                                                {
                                                        if(m_icurcol<m_IMAGECHAR)
                                                        {
                                                                if(m_iBorW[m_icurcol] <255)
                                                                {
                                                                        m_keypoint[ishowfont].x = m_objlistscanorA[0].x;
                                                                        m_keypoint[ishowfont].y = m_objlistscanorA[0].y;
                                                                        m_pText[ishowfont].SetPosition(x0+m_objlistscanorA[0].x,y0+m_objlistscanorA[0].y);
                                                                        m_piobjID[ishowfont] = nScanerID;
                                                                        m_piobjColor[ishowfont] = m_iBorW[m_icurcol];

                                                                        m_prect.push_back(Rect_Shape(x0 + iminx,y0 + iminy,x0 + imaxx,y0 + imaxy));
                                                                        //m_prect[ishowfont].m_Ax = x0 + iminx ;
                                                                        //m_prect[ishowfont].m_Ay = y0 + iminy ;
                                                                        //m_prect[ishowfont].m_Cx = x0 + imaxx ;
                                                                        //m_prect[ishowfont].m_Cy = y0 + imaxy ;


                                                                        m_totalarea = m_totalarea + nscansize;
                                                                        ishowfont++;
                                                                        m_icurcol ++;
                                                                }

                                                        }
                                                }
                                                break;
                                        case 4://对象填充
                                                {

                                                        abyte= m_pgdi->PointColor(m_objlistscanorA[0].x+x0, m_objlistscanorA[0].y+y0);
                                                        if(abyte>0)
                                                        {
                                                                for(int ik=0;ik<nscansize;ik++)
                                                                        m_pgdi->PointColor(m_objlistscanorA[ik].x+x0, m_objlistscanorA[ik].y+y0,NONECOLOR);

                                                        }
                                                        //else
                                                        //{
                                                        //	for(int ik=0;ik<m_nscansize;ik++)
                                                        //		am_pgdi->CopyImagePointColor(m_listscanorA[ik].x+x0, m_listscanorA[ik].y+y0,WHITE);

                                                        //}

                                                }
                                                break;
                                        case 5://对象填充
                                                {

                                                        abyte= m_pgdi->PointColor(m_objlistscanorA[0].x+x0, m_objlistscanorA[0].y+y0);
                                                        if(abyte<=0)
                                                        {
                                                                for(int ik=0;ik<nscansize;ik++)
                                                                        m_pgdi->PointColor(m_objlistscanorA[ik].x+x0, m_objlistscanorA[ik].y+y0,WHITE);
                                                        }
                                                }
                                                break;
                                        case 6://
                                                for(int ik=0;ik<nscansize;ik++)
                                                        m_pgdi->PointColor(m_objlistscanorA[ik].x+x0, m_objlistscanorA[ik].y+y0,m_staticcolor[nScanerID]);
                                                break;
                                        }
                                }
                        }

                        iminx = 9999;
                        iminy = 9999;
                        imaxx = 0;
                        imaxy = 0;
                        //////////////////////////////////////////////////////////////////////////
                        //扫描结束
                        nScanerID++;
                        icurSeekNum++;
FOREND:
                        ;
        }

        m_pgdi->UnLockGDI();
        CLEAR_SCANOR();
        CLEAR_SEARCHSEEK();
        m_imassnum = m_icurcol;
        //m_icurcol = 1;

}


//////////////////////////////////////////////////////////////////////////
void CObjectAnalysisExA::GDIMeasure_org_GDIMAP_8bit_AAA(void *pgdi)
{
#ifdef DEMOSHOW//演示版本
        for(int idemo=0;idemo<10;idemo++)
                Sleep(10);
#endif
        m_imaptype = 1;

        m_igrid = 0;

        m_pgdi = (CGDIImage32*)pgdi;

        m_totalarea = 0;
        m_icurcol = 0;
        if(NULL==m_pgdi)
                return;
        if(m_pgdi->IsNull())
                return;
        int x0=GetOrgX();
        int y0=GetOrgY();
        int iw=GetWidth();
        int ih=GetHeight();
        int x1=x0+GetWidth();
        int y1=y0+GetHeight();
        if(x0>m_pgdi->GetVisibleWidth()
                ||y0>m_pgdi->GetVisibleHeight()
                ||x1>m_pgdi->GetVisibleWidth()
                ||y1>m_pgdi->GetVisibleHeight()
                ||x0<0
                ||y0<0)
                return;
        m_gdialloc->ImageROIclear(0,0,iw,ih);

        //m_pgdi->
        int nx0=0;
        int ny0=0;
        int nx=0;
        int ny=0;
        int nservice=0;
        int nanalysis=0;
        BYTE abyte,bytenext;
        int nTol=0;
        //收集
        int nScanerID=1;

        int icurScanerNUM=0;

        bool boverflow =false;
        //////DOWNCODE////////////////////////////////////////////////////////////////////
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
        m_prect.clear();
        CLEAR_SEARCHSEEK();
        //////UPCODE////////////////////////////////////////////////////////////////////
        //将地图第一个节点进入堆栈
        PUSH_SEARCHSEEK(0,0);
        //m_pgdi->LockGDI();
        for(int icurSeekNum=0;icurSeekNum<nsearchseeksize;)//searchseek.size();)
        {
FORBEGIN:

                mapservice = ExMAP_service(m_objlistcollectorA[icurSeekNum].x,m_objlistcollectorA[icurSeekNum].y);

                if(mapservice>0)//当前种子节点被提取
                {
                        icurSeekNum++;

                        if(icurSeekNum>=nsearchseeksize)//searchseek.size())
                                break;
                        else
                                goto FORBEGIN;
                }
                //扫描器清空
                CLEAR_SCANOR();
                //扫描器推入种子
                PUSH_SCANOR(m_objlistcollectorA[icurSeekNum].x,m_objlistcollectorA[icurSeekNum].y);
                //重新设置种子点的状态
                ExSetMAP_service(m_objlistcollectorA[icurSeekNum].x,m_objlistcollectorA[icurSeekNum].y,nScanerID);
                //扫描开始
                icurScanerNUM=0;

CURSCANERBEGIN:
                while(icurScanerNUM!=nscansize)//scanor.size())
                {
                        nx0= m_objlistscanorA[icurScanerNUM].x;//pDot->m_ix;
                        ny0= m_objlistscanorA[icurScanerNUM].y;//pDot->m_iy;

                        abyte =  m_pgdi->PointColor8bit(nx0+x0,ny0+y0);//RowAddress[ny0][nx0+nx0+nx0];//(RowAddress[ny0][nx0]);

                        //探索pshape
                        //判断连接法(当前点与周围点超过最远连接的为无效过滤点)
                        mapanalysis = ExMAP_analysis(nx0,ny0);

                        if(mapanalysis==ANLAYSIS_OVER)
                                goto NEXTFINDSTEP;
                        for(int i= 0;i<m_idistance;i++)
                        {//here a problem SEARCH_LENGTH

                                nx=nx0+G_SearchPointGroup[i].x;
                                ny=ny0+G_SearchPointGroup[i].y;
                                if(nx<=0 ||ny<=0 ||nx>=iw ||ny>=ih)
                                        continue;//搜索不在范围内

                                mapanalysis = ExMAP_analysis(nx,ny);

                                mapservice = ExMAP_service(nx,ny);

                                if(mapanalysis==ANLAYSIS_OVER
                                        ||mapservice>0//属于其他OBJECT
                                        ||-nScanerID==nservice//搜索过不属于当前OBJECT
                                        )
                                        continue;

                                bytenext = m_pgdi->PointColor8bit(nx+x0,ny+y0) ;


                                //////////////////////////////////////////////////////////////////////////
                                //nservice 满足同一Object的条件 分析
                                if(abyte==bytenext)//满足归为一类的条件
                                {
                                        //结点推入扫描列表
                                        PUSH_SCANOR(nx,ny);
                                        ExSetMAP_service(nx,ny,nScanerID);

                                        ExSetMAP_pixel(nx,ny,abyte);
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
                                        //分析完成不是同一Object //退入种子列表
                                        PUSH_SEARCHSEEK(nx,ny);
                                        ExSetMAP_service(nx,ny,-nScanerID);
                                }

                        }
                        //从地图上设置该点OVER
                        ExSetMAP_analysis(nx0,ny0,ANLAYSIS_OVER);
                        //分析扫描器上的下个节点
NEXTFINDSTEP:
                        icurScanerNUM++;

                }
CURSCANEREND:
                //if(nscansize>m_iminarea
                //	&&nscansize<m_imaxarea)
                int iobjw = imaxx - iminx;
                int iobjh = imaxy - iminy;
                if((m_ifilterNedge>0
                        &&(iminx>m_ifilterNedge
                        &&imaxx<iw-m_ifilterNedge
                        &&iminy>m_ifilterNedge
                        &&imaxy<ih-m_ifilterNedge))
                        ||m_ifilterNedge==0)
                        if(nscansize>m_iminarea
                                &&nscansize<m_imaxarea
                                &&iobjw<m_imaxobjw
                                &&iobjw>=m_iminobjw
                                &&iobjh<m_imaxobjh
                                &&iobjh>=m_iminobjh)
                        {
                                if(m_icurcol<m_IMAGECHAR)
                                        m_irow[m_icurcol] = nscansize;
                                else
                                        goto FOREND;
                                //else
                                //	m_irow.push_back(nscansize);

                                abyte= m_pgdi->PointColor8bit( m_objlistscanorA[0].x+x0, m_objlistscanorA[0].y+y0);

                                if(m_icurcol<m_IMAGECHAR)
                                        m_iBorW[m_icurcol] = abyte ;
                                //else
                                //	m_iBorW[m_icurcol]=abyte&0x0fff0;
                                boverflow=false;
                                {
                                        switch(m_ifontshow)
                                        {
                                        case 3://不管黑白
                                                if(m_icurcol<m_IMAGECHAR)
                                                {
                                                        m_keypoint[ishowfont].x = m_objlistscanorA[0].x;
                                                        m_keypoint[ishowfont].y = m_objlistscanorA[0].y;
                                                        m_pText[ishowfont].SetPosition(x0+m_objlistscanorA[0].x,y0+m_objlistscanorA[0].y);
                                                        m_piobjID[ishowfont] = nScanerID;
                                                        m_piobjColor[ishowfont] = m_iBorW[m_icurcol];

                                                        m_prect.push_back(Rect_Shape(x0 + iminx,y0 + iminy,x0 + imaxx,y0 + imaxy));
                                                        //m_prect[ishowfont].m_Ax = x0 + iminx ;
                                                        //m_prect[ishowfont].m_Ay = y0 + iminy ;
                                                        //m_prect[ishowfont].m_Cx = x0 + imaxx ;
                                                        //m_prect[ishowfont].m_Cy = y0 + imaxy ;



                                                        ishowfont++;
                                                        m_icurcol ++;
                                                }

                                                break;
                                        case 0:break;//不选
                                        case 1://选白
                                                {
                                                        if(m_icurcol<m_IMAGECHAR)
                                                        {
                                                                if(m_iBorW[m_icurcol] >0)
                                                                {
                                                                        m_keypoint[ishowfont].x = m_objlistscanorA[0].x;
                                                                        m_keypoint[ishowfont].y = m_objlistscanorA[0].y;
                                                                        m_pText[ishowfont].SetPosition(x0+m_objlistscanorA[0].x,y0+m_objlistscanorA[0].y);
                                                                        m_piobjID[ishowfont] = nScanerID;
                                                                        m_piobjColor[ishowfont] = m_iBorW[m_icurcol];

                                                                        m_prect.push_back(Rect_Shape(x0 + iminx,y0 + iminy,x0 + imaxx,y0 + imaxy));
                                                                        //m_prect[ishowfont].m_Ax = x0 + iminx ;
                                                                        //m_prect[ishowfont].m_Ay = y0 + iminy ;
                                                                        //m_prect[ishowfont].m_Cx = x0 + imaxx ;
                                                                        //m_prect[ishowfont].m_Cy = y0 + imaxy ;


                                                                        m_totalarea = m_totalarea + nscansize;
                                                                        ishowfont++;
                                                                        m_icurcol ++;
                                                                }

                                                        }
                                                }
                                                break;
                                        case 2://选黑
                                                {
                                                        if(m_icurcol<m_IMAGECHAR)
                                                        {
                                                                if(m_iBorW[m_icurcol] <255)
                                                                {
                                                                        m_keypoint[ishowfont].x = m_objlistscanorA[0].x;
                                                                        m_keypoint[ishowfont].y = m_objlistscanorA[0].y;
                                                                        m_pText[ishowfont].SetPosition(x0+m_objlistscanorA[0].x,y0+m_objlistscanorA[0].y);
                                                                        m_piobjID[ishowfont] = nScanerID;
                                                                        m_piobjColor[ishowfont] = m_iBorW[m_icurcol];

                                                                        m_prect.push_back(Rect_Shape(x0 + iminx,y0 + iminy,x0 + imaxx,y0 + imaxy));
                                                                        //m_prect[ishowfont].m_Ax = x0 + iminx ;
                                                                        //m_prect[ishowfont].m_Ay = y0 + iminy ;
                                                                        //m_prect[ishowfont].m_Cx = x0 + imaxx ;
                                                                        //m_prect[ishowfont].m_Cy = y0 + imaxy ;

                                                                        m_totalarea = m_totalarea + nscansize;
                                                                        ishowfont++;
                                                                        m_icurcol ++;
                                                                }

                                                        }
                                                }
                                                break;
                                        }
                                }

                                //if(m_icurcol>=m_iMEASUREPOINTSUMOBJ-1)
                                //{
                                //	return;
                                //}
                        }

                        iminx = 9999;
                        iminy = 9999;
                        imaxx = 0;
                        imaxy = 0;
                        //////////////////////////////////////////////////////////////////////////
                        //扫描结束
                        nScanerID++;
                        icurSeekNum++;
FOREND:
                        ;
        }

        //m_pgdi->UnLockGDI();
        CLEAR_SCANOR();
        CLEAR_SEARCHSEEK();
        m_imassnum = m_icurcol;
        //m_icurcol = 1;

}

