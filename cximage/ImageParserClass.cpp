#include "muParserDef.h"
#include "muParserTest.h"
#include "ImageParserClass.h"
#include "imagebase.h"
#include "shapebase.h"
#include "shape.h"
//#include "grid.h"
//#include "findline.h"
//#include "findobject.h"
//#include "gridobject.h"
//#include "fastmatch.h"
//#include "orc.h"
#include "backimagemanager.h"
#include "../p2p/dialog.h"
#include "../AI/stringai.h"
#ifdef USE_CAM
//#include "HWGrabImage.h"
#include "qt_hik.hpp"
#endif
typedef std::vector<double> vectordouble;

class CVectDouble
{
    //char m_filename[128];
    vectordouble  m_vect;
public:
    CVectDouble(){}
    ~CVectDouble(){}
    void push(double dvalue){m_vect.push_back(dvalue);}
    double get(int inum){return m_vect[inum];}
    void set(double inum,double dvalue){m_vect[((int)inum)]=dvalue;}
    void clear(){m_vect.clear();}
    int size(){return m_vect.size();}
    double average()
    {
        double daverage=0;
        for(int i=0;i<m_vect.size();i++)
        {
            daverage +=m_vect[i];
        }
        return daverage/m_vect.size();
    }
    double maxvalue()
    {
        double dmax=-11111;
        for(int i=0;i<m_vect.size();i++)
        {
            if(dmax<m_vect[i])
                dmax =m_vect[i];
        }
        return dmax;
    }
    int maxnum()
    {
        double dmax=-11111;int i;
        for(i=0;i<m_vect.size();i++)
        {
            if(dmax<m_vect[i])
                dmax =m_vect[i];
        }
        return i;
    }
    double minvalue()
    {
        double dmin=9999;
        for(int i=0;i<m_vect.size();i++)
        {
            if(dmin<m_vect[i])
                dmin =m_vect[i];
        }
        return dmin;
    }
    void savefile()
    {


    }
};


namespace mu
{
    CImageParserClass::CImageParserClass():
    m_iVal(0),
    m_lLicenseState(0)
    {
        g_testcal = 0;
    }
    void CImageParserClass::ParserInitialClassFunction(int iusing)
    {
        m_lLicenseState = m_lLicenseState + (0x01<<iusing);

        switch(iusing)
        {
                    case 0:
                    {
                        double *apdouble=0;
                        m_parser.DefineOrgClass("double",apdouble);


                        BackImageManager *pmodule = 0;
                       m_parser.DefineClass("Module",pmodule);
                       m_parser.DefineClassFun("Module",pmodule,"Show",&BackImageManager::setshow);
                       m_parser.DefineClassFun("Module",pmodule,"setobjectshow",&BackImageManager::setobjectshow);


        StringAI *pstringai =0;
        m_parser.DefineClass("StringAI",pstringai);
        m_parser.DefineClassFun("StringAI",pstringai,"addsearchkey",&StringAI::addsearchkey);
        m_parser.DefineClassFun("StringAI",pstringai,"clearkeys",&StringAI::clearkeys);
        m_parser.DefineClassFun("StringAI",pstringai,"addneedstringlist",&StringAI::addneedstringlist);
        m_parser.DefineClassFun("StringAI",pstringai,"addextfilter",&StringAI::addextfilter);
        m_parser.DefineClassFun("StringAI",pstringai,"addkeywordfilter",&StringAI::addkeywordfilter);
        m_parser.DefineClassFun("StringAI",pstringai,"clearfilter",&StringAI::clearfilter);
        m_parser.DefineClassFun("StringAI",pstringai,"updateurltree",&StringAI::updateurltree);
        m_parser.DefineClassFun("StringAI",pstringai,"setrelationlabel",&StringAI::setrelationlabel);
        m_parser.DefineClassFun("StringAI",pstringai,"seturllabel",&StringAI::seturllabel);
        m_parser.DefineClassFun("StringAI",pstringai,"update",&StringAI::update);
        m_parser.DefineClassFun("StringAI",pstringai,"addrelation",&StringAI::addrelation);
        m_parser.DefineClassFun("StringAI",pstringai,"addurl",&StringAI::addurl);
        m_parser.DefineClassFun("StringAI",pstringai,"addocrstring",&StringAI::addocrstring);
        m_parser.DefineClassFun("StringAI",pstringai,"clearstring",&StringAI::clearstring);

/*
                       Console *pconsole =0;
                       m_parser.DefineClass("Console",pconsole);
                       m_parser.DefineClassFun("Console",pconsole,"mousemove",&Console::mousemove);
                       m_parser.DefineClassFun("Console",pconsole,"mousedown",&Console::mousedown);
                       m_parser.DefineClassFun("Console",pconsole,"mouseup",&Console::mouseup);
                       m_parser.DefineClassFun("Console",pconsole,"click",&Console::click);
                       m_parser.DefineClassFun("Console",pconsole,"key",&Console::key);
                       m_parser.DefineClassFun("Console",pconsole,"keystring",&Console::keystring);
                       m_parser.DefineClassFun("Console",pconsole,"doevent",&Console::doevent);
                       m_parser.DefineClassFun("Console",pconsole,"keyaistring",&Console::keyaistring);
*/
                                     dialogOPT *pdialogopt = 0;
                                     m_parser.DefineClass("dialogopt",pdialogopt);

                                     m_parser.DefineClassFun("dialogopt",pdialogopt,"testurl",&dialogOPT::test);
                                     m_parser.DefineClassFun("dialogopt",pdialogopt,"viewparserlist",&dialogOPT::runviewparser);
                                     m_parser.DefineClassFun("dialogopt",pdialogopt,"viewrunjavascript",&dialogOPT::runjavascript);
                                     m_parser.DefineClassFun("dialogopt",pdialogopt,"download",&dialogOPT::download);
                                     m_parser.DefineClassFun("dialogopt",pdialogopt,"setgaptime",&dialogOPT::setgaptime);
                                     m_parser.DefineClassFun("dialogopt",pdialogopt,"runconsole",&dialogOPT::runconsole);
                                     m_parser.DefineClassFun("dialogopt",pdialogopt,"downloada",&dialogOPT::downloadA);
                                     m_parser.DefineClassFun("dialogopt",pdialogopt,"desktopimage",&dialogOPT::desktopimage);
                                     m_parser.DefineClassFun("dialogopt",pdialogopt,"desktopimagerect",&dialogOPT::desktopimagerect);
                                     m_parser.DefineClassFun("dialogopt",pdialogopt,"getcomtcpipdesktop",&dialogOPT::getcomtcpipdesktop);
                                     m_parser.DefineClassFun("dialogopt",pdialogopt,"com2tcpipstring",&dialogOPT::com2tcpipstring);
                                     m_parser.DefineClassFun("dialogopt",pdialogopt,"com2tcpipkeystring",&dialogOPT::com2tcpipkeystring);
                                     m_parser.DefineClassFun("dialogopt",pdialogopt,"tcp2comimage",&dialogOPT::tcp2comimage);
                                     m_parser.DefineClassFun("dialogopt",pdialogopt,"tcp2comvalue",&dialogOPT::tcp2comvalue);
                                     m_parser.DefineClassFun("dialogopt",pdialogopt,"tcp2comstring",&dialogOPT::tcp2comstring);
                                    m_parser.DefineClassFun("dialogopt",pdialogopt,"sendserialfile",&dialogOPT::sendserialfile);
                                    m_parser.DefineClassFun("dialogopt",pdialogopt,"tcp2tcpimage",&dialogOPT::tcp2tcpimage);
                                    m_parser.DefineClassFun("dialogopt",pdialogopt,"tcp2tcpvalue",&dialogOPT::tcp2tcpvalue);
                                    m_parser.DefineClassFun("dialogopt",pdialogopt,"tcp2tcpstring",&dialogOPT::tcp2tcpstring);


                            m_parser.DefineClassFun("dialogopt",pdialogopt,"downloadreset",&dialogOPT::downloadreset);
                            m_parser.DefineClassFun("dialogopt",pdialogopt,"runcommand",&dialogOPT::runcommand);
                            m_parser.DefineClassFun("dialogopt",pdialogopt,"runstringai",&dialogOPT::runstringai);
                            m_parser.DefineClassFun("dialogopt",pdialogopt,"runopt",&dialogOPT::runopt);
                            m_parser.DefineClassFun("dialogopt",pdialogopt,"pagelistwork",&dialogOPT::pagelistwork);
                            m_parser.DefineClassFun("dialogopt",pdialogopt,"sleep",&dialogOPT::asleep);
                            m_parser.DefineClassFun("dialogopt",pdialogopt,"scale",&dialogOPT::scale);
                            m_parser.DefineClassFun("dialogopt",pdialogopt,"rundutylist",&dialogOPT::rundutylist);
                            m_parser.DefineClassFun("dialogopt",pdialogopt,"cleartab",&dialogOPT::cleartab);

                            m_parser.DefineClassFun("dialogopt",pdialogopt,"updateview",&dialogOPT::updateview);
                            m_parser.DefineClassFun("dialogopt",pdialogopt,"showlog",&dialogOPT::showlog);
                            m_parser.DefineClassFun("dialogopt",pdialogopt,"showstring",&dialogOPT::showstring);
                            m_parser.DefineClassFun("dialogopt",pdialogopt,"urlfilter",&dialogOPT::urlfilter);
                            m_parser.DefineClassFun("dialogopt",pdialogopt,"filextfilter",&dialogOPT::filextfilter);
                            m_parser.DefineClassFun("dialogopt",pdialogopt,"urlneedstring",&dialogOPT::urlneedstring);
                            m_parser.DefineClassFun("dialogopt",pdialogopt,"clearfilter",&dialogOPT::clearfilter);

                             m_parser.DefineClassFun("dialogopt",pdialogopt,"setstring",&dialogOPT::setcurstring);
                            m_parser.DefineClassFun("dialogopt",pdialogopt,"addstring",&dialogOPT::addstring);
                            m_parser.DefineClassFun("dialogopt",pdialogopt,"addparservaluestring",&dialogOPT::addparservaluestring);
                            m_parser.DefineClassFun("dialogopt",pdialogopt,"com2tcpipcurstring",&dialogOPT::com2tcpipcurstring);


                           m_parser.DefineClassFun("dialogopt",pdialogopt,"camshow",&dialogOPT::CamTest);



#ifdef USE_CAM
                HKCam *pcam = 0;
                m_parser.DefineClass("CamRun",pcam);
                m_parser.DefineClassFun("CamRun",pcam,"enumdevice",&HKCam::EnumDevicesA);
                m_parser.DefineClassFun("CamRun",pcam,"opendevice",&HKCam::OpenDeviceA);
                m_parser.DefineClassFun("CamRun",pcam,"closedevice",&HKCam::CloseDeviceA);



                m_parser.DefineClassFun("CamRun",pcam,"startgarb",&HKCam::startgarb);
                m_parser.DefineClassFun("CamRun",pcam,"softtrig",&HKCam::SoftwareTrg);
                m_parser.DefineClassFun("CamRun",pcam,"softtrig2image",&HKCam::SoftwareTrg2Image);


                //m_parser.DefineClassFun("CamRun",pcam,"savejpg",&CamRun::SaveImageJPG);
                m_parser.DefineClassFun("CamRun",pcam,"savebmp",&HKCam::SaveBmp);

            //    m_parser.DefineClassFun("CamRun",pcam,"garbimage",&HKCam::OnBnClickedGarb);

              //  m_parser.DefineClassFun("CamRun",pcam,"reversex",&HKCam::OnBnClickedReverseX);
              //  m_parser.DefineClassFun("CamRun",pcam,"reversey",&HKCam::OnBnClickedReverseY);

#endif
#ifdef USE_CAMxxx
                CamRun *pcam = 0;
                m_parser.DefineClass("CamRun",pcam);
                m_parser.DefineClassFun("CamRun",pcam,"enumdevice",&CamRun::OnBnClickedEnumButton);
                m_parser.DefineClassFun("CamRun",pcam,"opendevice",&CamRun::OnBnClickedOpenButton);
                m_parser.DefineClassFun("CamRun",pcam,"closedevice",&CamRun::OnBnClickedCloseButton);
                m_parser.DefineClassFun("CamRun",pcam,"getresetvalue",&CamRun::GetResetValue);



                m_parser.DefineClassFun("CamRun",pcam,"startgarb",&CamRun::OnBnClickedStartGrabbingButton);
                m_parser.DefineClassFun("CamRun",pcam,"softtrig",&CamRun::OnBnClickedSoftwareOnceButton);
                //m_parser.DefineClassFun("CamRun",pcam,"savejpg",&CamRun::SaveImageJPG);
                m_parser.DefineClassFun("CamRun",pcam,"savebmp",&CamRun::SaveImageBMP);

                m_parser.DefineClassFun("CamRun",pcam,"garbimage",&CamRun::OnBnClickedGarb);

                m_parser.DefineClassFun("CamRun",pcam,"reversex",&CamRun::OnBnClickedReverseX);
                m_parser.DefineClassFun("CamRun",pcam,"reversey",&CamRun::OnBnClickedReverseY);

#endif




                        ImageBase *pimage = 0;
                       m_parser.DefineClass("Image",pimage);
                       m_parser.DefineClassFun("Image",pimage,"clear",&ImageBase::ImageClear);
                       m_parser.DefineClassFun("Image",pimage,"thre",&ImageBase::ImageThre);
                       m_parser.DefineClassFun("Image",pimage,"bar",&ImageBase::ImageBar);
                       m_parser.DefineClassFun("Image",pimage,"edge",&ImageBase::Image_5Blur_Gap_mud_thre_BW);
                       //m_parser.DefineClassFun("Image",pimage,"hsothre",&ImageBase::Image_HSOThreshold);
                       m_parser.DefineClassFun("Image",pimage,"muldiv",&ImageBase::ImageMulDiv);
                       m_parser.DefineClassFun("Image",pimage,"pixeladd",&ImageBase::ImageIncrease);
                       m_parser.DefineClassFun("Image",pimage,"pixelmud",&ImageBase::ImageDecrease);
                       m_parser.DefineClassFun("Image",pimage,"filp",&ImageBase::ImageFilp);
                       m_parser.DefineClassFun("Image",pimage,"mirror",&ImageBase::ImageMirror);
                       m_parser.DefineClassFun("Image",pimage,"imageand",&ImageBase::ImageAnd);
                       m_parser.DefineClassFun("Image",pimage,"setmode",&ImageBase::SetMode);
                       m_parser.DefineClassFun("Image",pimage,"zoom",&ImageBase::ImageZoom);
                       m_parser.DefineClassFun("Image",pimage,"pen",&ImageBase::ImagePen);

                       m_parser.DefineClassFun("Image",pimage,"roicolortab",&ImageBase::ROIColorTable);
                       m_parser.DefineClassFun("Image",pimage,"roibartab",&ImageBase::ROIColorTableBlur);
                       m_parser.DefineClassFun("Image",pimage,"roieasythre",&ImageBase::ROIColorTableEasyThre);

                       m_parser.DefineClassFun("Image",pimage,"savepath",&ImageBase::SavePath);
                       m_parser.DefineClassFun("Image",pimage,"loadpath",&ImageBase::LoadPath);
                       m_parser.DefineClassFun("Image",pimage,"saveroi",&ImageBase::SaveROI);

                       m_parser.DefineClassFun("Image",pimage,"roitoroi",&ImageBase::roitoroi);
                       m_parser.DefineClassFun("Image",pimage,"clipboardroi",&ImageBase::clipboardROIimage);



                       m_parser.DefineClassFun("Image",pimage,"x_and",&ImageBase::ImageMoveAndX);
                       m_parser.DefineClassFun("Image",pimage,"y_and",&ImageBase::ImageMoveAndY);
                       m_parser.DefineClassFun("Image",pimage,"x_or",&ImageBase::ImageMoveOrX);
                       m_parser.DefineClassFun("Image",pimage,"y_or",&ImageBase::ImageMoveOrY);

                       m_parser.DefineClassFun("Image",pimage,"roix_and",&ImageBase::ROIImageMoveAndX);
                       m_parser.DefineClassFun("Image",pimage,"roiy_and",&ImageBase::ROIImageMoveAndY);

                       m_parser.DefineClassFun("Image",pimage,"roix_or",&ImageBase::ROIImageMoveOrX);
                       m_parser.DefineClassFun("Image",pimage,"roiy_or",&ImageBase::ROIImageMoveOrY);



                       m_parser.DefineClassFun("Image",pimage,"setroi",&ImageBase::SetROI);
                       m_parser.DefineClassFun("Image",pimage,"roiclear",&ImageBase::ROIImageClear);
                       m_parser.DefineClassFun("Image",pimage,"roibar",&ImageBase::ROIImageBar);
                       m_parser.DefineClassFun("Image",pimage,"roithre",&ImageBase::ROIImageThre);
                       m_parser.DefineClassFun("Image",pimage,"roiedge",&ImageBase::ROIImage_5Blur_Gap_mud_thre_BW);
                       m_parser.DefineClassFun("Image",pimage,"roiedge7",&ImageBase::ROIImage_7Blur_Gap_mud_thre_BW);

                       m_parser.DefineClassFun("Image",pimage,"roiedgeh",&ImageBase::ROIImage_5Blur_Gap_mud_thre_BW_H);
                       m_parser.DefineClassFun("Image",pimage,"roiedgeh7",&ImageBase::ROIImage_7Blur_Gap_mud_thre_BW_H);

                       m_parser.DefineClassFun("Image",pimage,"roihsothre",&ImageBase::ROIImage_HSOThreshold);
                       m_parser.DefineClassFun("Image",pimage,"roimuldiv",&ImageBase::ROIImageMulDiv);
                       m_parser.DefineClassFun("Image",pimage,"roipixeladd",&ImageBase::ROIImageIncrease);
                       m_parser.DefineClassFun("Image",pimage,"roipixelmud",&ImageBase::ROIImageDecrease);
                       m_parser.DefineClassFun("Image",pimage,"roifilp",&ImageBase::ROIImageFilp);
                       m_parser.DefineClassFun("Image",pimage,"roimirror",&ImageBase::ROIImageMirror);
                       m_parser.DefineClassFun("Image",pimage,"setmode",&ImageBase::SetMode);
                       m_parser.DefineClassFun("Image",pimage,"gray",&ImageBase::ImageGray);
                       m_parser.DefineClassFun("Image",pimage,"roigray",&ImageBase::ROIImageGray);
                       m_parser.DefineClassFun("Image",pimage,"Show",&ImageBase::setshow);
                       m_parser.DefineClassFun("Image",pimage,"roicopy",&ImageBase::ROIImageCopy);

                       m_parser.DefineClassFun("Image",pimage,"roior",&ImageBase::ROIImageOr);
                       m_parser.DefineClassFun("Image",pimage,"roiand",&ImageBase::ROIImageAnd);
                       m_parser.DefineClassFun("Image",pimage,"image_and",&ImageBase::Image_And);
                       m_parser.DefineClassFun("Image",pimage,"image_or",&ImageBase::Image_Or);
                       m_parser.DefineClassFun("Image",pimage,"roiimage",&ImageBase::ROIImage);




                       QShape *pshape = 0;
                       m_parser.DefineClass("Shape",pshape);
                       m_parser.DefineClassFun("Shape",pshape,"settype",&QShape::settype);
                       m_parser.DefineClassFun("Shape",pshape,"setname",&QShape::setname);
                       m_parser.DefineClassFun("Shape",pshape,"setrect",&QShape::setrect);
                       m_parser.DefineClassFun("Shape",pshape,"setcolor",&QShape::setcolor);



                       ShapeBase *pshapebase = 0;
                       m_parser.DefineClass("ShapeBase",pshapebase);
                       m_parser.DefineClassFun("ShapeBase",pshapebase,"settransformed",&ShapeBase::setTransformed);
                       m_parser.DefineClassFun("ShapeBase",pshapebase,"setshape",&ShapeBase::setShape);
                       m_parser.DefineClassFun("ShapeBase",pshapebase,"setantialiased",&ShapeBase::setAntialiased);
                       m_parser.DefineClassFun("ShapeBase",pshapebase,"Show",&ShapeBase::setshow);
                       m_parser.DefineClassFun("ShapeBase",pshapebase,"setpen",&ShapeBase::setPen);
                       m_parser.DefineClassFun("ShapeBase",pshapebase,"setbrush",&ShapeBase::setBrush);
                       m_parser.DefineClassFun("ShapeBase",pshapebase,"setpercent",&ShapeBase::setPercent);
                       m_parser.DefineClassFun("ShapeBase",pshapebase,"setpenw",&ShapeBase::setpenw);



        LineShape *plineshape = 0;
        m_parser.DefineClass("LineShape",plineshape);
        m_parser.DefineClassFun("LineShape",plineshape,"setline",&LineShape::setline);
        m_parser.DefineClassFun("LineShape",plineshape,"Show",&LineShape::setshow);
        m_parser.DefineClassFun("LineShape",plineshape,"move",&LineShape::Move);
        m_parser.DefineClassFun("LineShape",plineshape,"rotate",&LineShape::Rotate);
        m_parser.DefineClassFun("LineShape",plineshape,"zoom",&LineShape::Zoom);
        m_parser.DefineClassFun("LineShape",plineshape,"setpenw",&LineShape::setpenw);


        CVectDouble *avect=0;
        m_parser.DefineClass("vector",avect);

        m_parser.DefineClassFun("vector",avect,"push",&CVectDouble::push);
        m_parser.DefineClassFun("vector",avect,"get",&CVectDouble::get);
        m_parser.DefineClassFun("vector",avect,"set",&CVectDouble::set);
        m_parser.DefineClassFun("vector",avect,"clear",&CVectDouble::clear);
        m_parser.DefineClassFun("vector",avect,"size",&CVectDouble::size);
        m_parser.DefineClassFun("vector",avect,"average",&CVectDouble::average);
        m_parser.DefineClassFun("vector",avect,"maxvalue",&CVectDouble::maxvalue);
        m_parser.DefineClassFun("vector",avect,"minvalue",&CVectDouble::minvalue);
        m_parser.DefineClassFun("vector",avect,"maxnum",&CVectDouble::maxnum);
        m_parser.DefineClassFun("vector",avect,"savefile",&CVectDouble::savefile);
                    }
                    m_parser.UsingClass(true);
                    m_pdoubleclass = (classbase*)GetClass(string("double"));
                    break;
        }
        m_parser.UsingClass(true);
        //m_pdoubleclass = (classbase*)GetClass(string("double"));

    }
    long CImageParserClass::GetLicenseValue(){
        return m_lLicenseState;
    }
    void CImageParserClass::ParserElementShow(int ishow)
    {
        mu::Parser &Pparser=m_parser;
        mu::classbasemap_type classmap = Pparser.GetClassMap();
        if(!classmap.size())
            return;
        classbasemap_type::const_iterator item = classmap.end();

        for (; item!=classmap.begin(); )
        {
            --item;
            classbase *pclass=(item->second);

            if("CFindPointEx"==item->first)
            {
#ifdef USE_CROIMeasurePointEx
                CROIMeasurePointEx *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CROIMeasurePointEx *)pclass->getvarpoint(i);
                    if(ishow>0)
                    apimage->SetShow(35);
                    else
                    apimage->SetShow(0);
                }
#endif
            }


                }
    }
    void *CImageParserClass::GetClass(const string & strclass)
    {
        mu::classbasemap_type classmap = m_parser.GetClassMap();
        classbasemap_type::const_iterator item = classmap.find(strclass);
        if(item!=classmap.end())
        {
            return (item->second);
        }
        else
            return 0;
    }


    void* CImageParserClass::GetClassObj(const string & strclass,const string & strobj)
    {
        mu::classbasemap_type classmap = m_parser.GetClassMap();
        classbase *pclass;
        //if(!classmap.size())
        //	return NULL;
        /*
        OrgStorage_type::iterator item = m_ObjectStorage.find(a_str);
        if (item!=m_ObjectStorage.end())
        return item->second;
        else
        return 0;

        return pclass->getvarpoint(pclass);
        */
        classbasemap_type::const_iterator item = classmap.find(strclass);
        if(item!=classmap.end())
        {
            pclass=(item->second);
            return pclass->getvarpoint(strobj);
        }
        else
        return 0;
        //classbasemap_type::const_iterator item = classmap.begin();
        //for (; item!=classmap.end(); ++item)
        //{
        //	if(strclass==item->first)
        //	{
        //		pclass=(item->second);
        //		for(int i=0;i<pclass->size();i++)
        //		{
        //			if(strobj == pclass->getvar(i))
        //				return pclass->getvarpoint(i);
        //		}
        //	}
        //}
    }
    void* CImageParserClass::GetClassObj(const string & strclass,const int &iobjnum)
    {
        mu::classbasemap_type classmap = m_parser.GetClassMap();
        classbase *pclass;
        if(!classmap.size())
            return 0;

        classbasemap_type::const_iterator item = classmap.begin();
        for (; item!=classmap.end(); ++item)
        {
            if(strclass==item->first)
            {
                pclass=(item->second);

                if(iobjnum>=pclass->size())
                    return 0;
                //for(int i=0;i<pclass->size();i++)
                {
                     *m_stream <<pclass->getvar(iobjnum);
                     *m_stream <<"\r\n";
                    return pclass->getvarpoint(iobjnum);
                }
            }
        }
        return NULL;
    }


    void* CImageParserClass::GetDoubleValue(const string & strname)
    {
        if(m_pdoubleclass!=0)
            return m_pdoubleclass->getvarpoint(strname);
    }
    int CImageParserClass::GetClassObjSum(const string & strclass)
    {
        mu::classbasemap_type classmap = m_parser.GetClassMap();
        classbase *pclass;
        if(!classmap.size())
            return 0;

        classbasemap_type::const_iterator item = classmap.begin();
        for (; item!=classmap.end(); ++item)
        {
            if(strclass==item->first)
            {
                pclass=(item->second);

                return pclass->size();
            }
        }
        return 0;
    }
    CImageParserClass::~CImageParserClass()
    {

    }
    void CImageParserClass::SetExpr(const string & str)
    {

        m_parser.SetExpr(str);
    }
    value_type CImageParserClass::Eval()
    {
        return m_parser.Eval();
    }
    double CImageParserClass::GetResult()
    {
        return m_parser.Eval();
    }
    void CImageParserClass::DefineVar(const string & str,double *dvalue)
    {
        m_parser.DefineVar(str,dvalue);
    }
    //need to do
    void CImageParserClass::SetVarFactory()
    {
        //CXListCtrl *pGrid=	reinterpret_cast<CXListCtrl *>(pView);
        m_parser.SetVarFactory((mu::facfun_type)&CImageParserClass::AddVariable,this);
    }

    void CImageParserClass::FunTest(void *func)
    {
        m_Func=func;
        //*func;
    }

    double* CImageParserClass::AddVariable(const char *a_szName,void *pClass)
    {
        // I don't want dynamic allocation here, so i used this static buffer
        // If you want dynamic allocation you must allocate all variables dynamically
        // in order to delete them later on. Or you find other ways to keep track of
        // variables that have been created implicitely.
        CImageParserClass *me= reinterpret_cast<CImageParserClass *>(pClass);
        // static double afValBuf[100];
        // static int iVal = 0;

        *(me->m_stream) << "Generating new variable \""
            << a_szName << "\" (slots left: "
            << 99-me->m_iVal << ")" << "\r\n";

        me->m_afValBuf[me->m_iVal] = 0;
        if (me->m_iVal>=99)
            throw mu::ParserError("Variable buffer overflow.");

        return &(me->m_afValBuf[me->m_iVal++]);
    }

    ParserByteCode::storage_type CImageParserClass::GetByteCode()
    {
        return m_parser.GetStorageBase();
    }
    double CImageParserClass::RunByteCode(ParserByteCode::storage_type Base)
    {
        m_parser.m_vByteCodeCollection.SetStorageBase(Base);
        double dresult= m_parser.RunCollectionCmdCode();
        m_parser.ClearCollection();
        return dresult;
    }
    double CImageParserClass::RunOptCode()
    {
        return m_parser.RunCollectionOpt();
    }
    void CImageParserClass::RunFastCode()
    {
        return m_parser.RunCode();
    }
    void CImageParserClass::CopyRunOpt(int inum)
    {
        m_parser.CopyRUNOpt(inum);
    }
    void CImageParserClass::RunOptNum(int inum)
        {
        //////////////////////////////////////////////////////////////////////////
        try
        {
            m_parser.RunOpt(inum);
        }
        catch(mu::Parser::exception_type &e)
        {

            *m_stream  << "\n RunOptNum RunTime Error exception_type ";
        }
    }

    void CImageParserClass::SetRunOpt(const string &strname)
    {
        m_parser.SetOptStack(strname);
    }


    void CImageParserClass::RunOptString(const char *a_szName)
    {
        try
        {
            m_parser.RunOptString(a_szName);
        }
        catch(mu::Parser::exception_type &e)
        {
            *m_stream  << a_szName ;
            *m_stream  << "\n RunOptString RunTime Error \n";
        }
    }
    void CImageParserClass::ClearOptMap()
    {
        m_parser.ClearOptStack();
    }
    void CImageParserClass::RunOptNum_TimeLimit(int inum)
    {
        //m_parser.RunOptTimeLimit(inum);
        }
        void CImageParserClass::SetOptCollect(bool Open_Close)
    {
        m_parser.SetOptCollect(Open_Close);
    }
    void CImageParserClass::SetByteCollection(bool btruefalse)
    {
        m_parser.SetColllection(btruefalse);
    }
    void CImageParserClass::ClearByteCollection()
    {
        m_parser.ClearCollection();
    }
    //////////////////////////////////////////////////////////////////////////
    bool CImageParserClass::CommandLine(const string & astr)
    {
        std::string sLine=astr;
        if(sLine==">>quit"||sLine==">>quit\r\n")
        {
            exit(0);
        }
        else if(sLine==">>help"||sLine==">>help\r\n")
        {
            ShowHelp();
            return true;
        }
        else if (sLine==">>list var"||sLine==">>list var\r\n")
        {
            ListVar(&m_parser);
            return true;
        }
        else if (sLine==">>list const"||sLine==">>list const\r\n")
        {
            ListConst(&m_parser);
            return true;
        }
        else if (sLine==">>list exprvar"||sLine==">>list exprvar\r\n")
        {
            ListExprVar(&m_parser);
            return true;
        }
        else if (sLine==">>list const"||sLine==">>list const\r\n")
        {
            ListConst(&m_parser);
            return true;
        }
        else if(sLine==">>list func"||sLine==">>list func\r\n")
        {
            ListFunction(&m_parser);
            return true;
        }
        else if(sLine==">>list class"||sLine==">>list class\r\n")
        {
            ListClass( m_parser);
            return true;
        }
        else if(sLine==">>list only name")
        {
            ListClassONLYName();
            ListClassONLYFUNCTION();
            return true;
        }
        else if(sLine ==">>run"||sLine ==">>run\r\n")
        {
            m_parser.RunCode();
            *m_stream <<"create code :"<<"\r\n";
            *m_stream <<m_parser.m_StrCollection;
            return true;
        }
        else if(sLine ==">>run"||sLine ==">>run\r\n")
        {
            m_parser.RunCollectionOpt();
            return true;

        }
        else if(sLine==">>open collec"||sLine==">>open collec\r\n")
        {
            m_parser.SetColllection(true);
            return true;
        }
        else if(sLine==">>close collec"||sLine==">>close collec\r\n")
        {
            m_parser.SetColllection(false);
            return true;
        }
        else if(sLine==">>clear collec"||sLine==">>clear collec\r\n")
        {
            m_parser.ClearCollection();
            return true;
        }
        else if(sLine==">>clear"||sLine==">>clear\r\n")
        {
            m_stream->clear();
            return true;
        }
        else if(sLine==">>clear var"||sLine==">>clear var\r\n")
        {
            m_parser.ClearVar();
            return true;
        }
        else if(sLine==">>clear all"||sLine==">>clear all\r\n")
        {
            m_parser.ClearClassObj();
            m_parser.ClearVar();
            return true;
        }
        else if(sLine==">>test"||sLine==">>test\r\n")
        {
            SelfTest();
            return true;
        }
        else if(sLine==">>Set VarFact"||sLine==">>Set VarFact\r\n")
        {
            SetVarFactory();
            return true;
        }
        else if(sLine==">>open classdef"||sLine==">>open classdef\r\n")
        {
            m_parser.UsingClass(true);
            return true;
        }
        else if(sLine==">>close classdef"||sLine==">>close classdef\r\n")
        {
            m_parser.UsingClass(false);
            return true;
        }
        else
        {

            //
            //if(std::strncmp(sLine.c_str(),">>list", 5))
            //{
            //	FindClassObject(m_parser,sLine.c_str()+4);
            //}
        }
        return false;
    }
    void CImageParserClass::ShowHelp()
    {
        *m_stream<<"!-----------------help------------------- "<<"\r\n";
        *m_stream << "Commands:\r\n";
        *m_stream << "  list var     - list parser variables\r\n";
        *m_stream << "  list exprvar - list expression variables\r\n";
        *m_stream << "  list const   - list all numeric parser constants\r\n";
        *m_stream << "  exit         - exits the parser\r\n";

        *m_stream << "  list func    - list parser express function"<<"\r\n";
        *m_stream << "  list class   - list parser class define and class var"<<"\r\n";


        *m_stream << "  open collec  - open a vector to collection expression"<<"\r\n";
        *m_stream << "  close collec - close vector collection"<<"\r\n";
        *m_stream << "  clear collec - clear expression vector"<<"\r\n";
        *m_stream << "  run          - run collection expression"<<"\r\n";

        *m_stream << "  clear       -clear output screen\r\n"<<"\r\n";
        *m_stream << "  clear var   -clear variables"<<"\r\n";

        *m_stream << "  open classdef  - make parser to recognize class define"<<"\r\n";
        *m_stream << "  close classdef - close recognize class define"<<"\r\n";


        *m_stream << "  test        -self test "<<"\r\n";

        *m_stream << "Constants:\r\n";
        *m_stream << "  \"_e\"   2.718281828459045235360287\r\n";
        *m_stream << "  \"_pi\"  3.141592653589793238462643\r\n";
        *m_stream << "---------------------------------------\r\n";
        *m_stream << "Enter a formula or a command:\r\n";
    }
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    void  CImageParserClass::SelfTest()
    {
        mu::Test::ParserTester pt;
        pt.Run();
    }

    //---------------------------------------------------------------------------
    void CImageParserClass::ClearAll()
    {
        m_parser.ClearClassObj();
        m_parser.ClearVar();
    }
    //---------------------------------------------------------------------------
    void  CImageParserClass::ListVar(const mu::Parser  *Pparser)
    {
        //std::ostringstream os;
        // Query the used variables (must be done after calc)
        mu::varmap_type variables = Pparser->GetVar();
        if (!variables.size())
            return;
        *m_stream << "\nParser variables:\r\n";
        *m_stream <<   "-----------------\r\n";
        *m_stream << "Number: " << (int)variables.size() << "\r\n";
        varmap_type::const_iterator item = variables.begin();
        for (; item!=variables.end(); ++item)
        {
            *m_stream << "Name: " << item->first << "   Address: [0x" << item->second << "]  ";
            m_parser.SetExpr(item->first);
            *m_stream << "Result: "<<Pparser->Eval()<<"\r\n";
        }
    }

    //////////////////////////////////////////////////////////////////////////
    void CImageParserClass::ListVar()
    {
        ListVar(&m_parser);
    }
    //---------------------------------------------------------------------------
    void  CImageParserClass::ListConst(const mu::Parser * Pparser)
    {
        //std::ostringstream os;
        *m_stream << "\nParser constants:\r\n";
        *m_stream <<   "-----------------\r\n";
        mu::valmap_type cmap = Pparser->GetConst();
        if (!cmap.size())
        {
            *m_stream << "Expression does not contain constants\r\n";
        }
        else
        {
            valmap_type::const_iterator item = cmap.begin();
            for (; item!=cmap.end(); ++item)
                *m_stream << "  " << item->first << " =  " << item->second << "\r\n";
        }
    }

    //---------------------------------------------------------------------------
    void  CImageParserClass::ListExprVar(const mu::Parser * Pparser)
    {
        //std::ostringstream os;
        std::string sExpr = Pparser->GetExpr();
        if (sExpr.length()==0)
        {
            *m_stream << "Expression string is empty\r\n";
            return;
        }
        // Query the used variables (must be done after calc)
        *m_stream << "\nExpression variables:\r\n";
        *m_stream <<   "---------------------\r\n";
        *m_stream << "Expression: " <<Pparser->GetExpr() << "\r\n";
        varmap_type variables =Pparser->GetUsedVar();
        if (!variables.size())
        {
            *m_stream << "Expression does not contain variables\r\n";
        }
        else
        {
            *m_stream << "Number: " << (int)variables.size() << "\r\n";
            mu::varmap_type::const_iterator item = variables.begin();
            for (; item!=variables.end(); ++item)
                *m_stream << "Name: " << item->first << "   Address: [0x" << item->second << "]\r\n";
        }
    }
    //---------------------------------------------------------------------------
    void  CImageParserClass::ListFunction(const mu::Parser  *pParser)
    {
        // Make the type of the map containing function prototypes visible
        using mu::funmap_type;
        funmap_type funmap = pParser->GetFunDef();
        funmap_type::const_iterator item;
        *m_stream << "\nFunctions available:\r\n";
        // iterate over all function definitions
        for (item=funmap.begin(); item!=funmap.end(); ++item)
        {
            *m_stream << "  " << item->first;   // This is the Function Name
            *m_stream << "(";
            // Deal with different argument numbers
            int iArgc = item->second.GetArgc();
            if (iArgc>=0)
            {
                // Indicate number of input variables
                for (int i=0; i<iArgc; ++i)
                {
                    char cVar[] = "val ";
                    cVar[3] = '1' + (char)i;
                    *m_stream << cVar;
                    if (i!=iArgc-1)
                        *m_stream << ",";
                }
            }
            else
            {
                *m_stream << "...";  // Multiargument function
            }
            *m_stream << ")\r\n";
        }

    }

    //----------------------------------------------------------------------------
    void CImageParserClass::ListClass(mu::Parser &Pparser)
    {
        mu::classbasemap_type classmap = Pparser.GetClassMap();
        if(!classmap.size())
            return;
        *m_stream << "\nParser Class:\r\n";
        *m_stream <<   "-----------------\r\n";
        *m_stream << "Number: " << (int)classmap.size() << "\r\n";
        classbasemap_type::const_iterator item = classmap.begin();
        for (; item!=classmap.end(); ++item)
        {
            *m_stream << "Class Name: " << item->first << "   Address: [0x" << item->second << "]  "<< "\r\n";
            classbase *pclass=(item->second);

            *m_stream << "      Object Number: " <<  (int)pclass->size() << "\r\n";
            *m_stream << "      member Function Number: " <<  (int)pclass->funcsize() << "\r\n";

            for(int i=0;i<pclass->size();i++)
            {
                *m_stream << "            Object Name: " << pclass->getvar(i) <<"   Address: [0x"
                    << pclass->getvarpoint(i)<< "]  "<< "\r\n";
            }
            for(int i=0;i<pclass->funcsize();i++)
            {
                *m_stream << "             member Function: " << pclass->getfuncname(i) <<"   Type: ["
                    << pclass->getfunctype(i)<< "]  "<< "\r\n";
            }
        }
        *m_stream <<"\r\n";
    }

    void CImageParserClass::ListClassONLYName()
    {
        mu::classbasemap_type classmap = m_parser.GetClassMap();
        if(!classmap.size())
            return;

        classbasemap_type::const_iterator item = classmap.begin();
        for (; item!=classmap.end(); ++item)
        {
            *m_stream << item->first << "\r\n";
        }
        *m_stream <<"\r\n";
    }

    void CImageParserClass::ListClassONLYFUNCTION()
    {
        mu::classbasemap_type classmap = m_parser.GetClassMap();
        if(!classmap.size())
            return;

        classbasemap_type::const_iterator item = classmap.begin();
        for (; item!=classmap.end(); ++item)
        {
            classbase *pclass=(item->second);
            for(int i=0;i<pclass->funcsize();i++)
            {
                *m_stream << pclass->getfuncname(i)<< "\r\n";
            }
        }
        *m_stream <<"\r\n";
    }


    //////////////////////////////////////////////////////////////////////////
    void CImageParserClass::ListClass()
    {
        ListClass(m_parser);
    }
    //----------------------------------------------------------------------------
    void CImageParserClass::FindClassObject(mu::Parser &Pparser,const char *a_szClass)
    {
        string astrclass(a_szClass);

        mu::classbasemap_type classmap = Pparser.GetClassMap();
        if(!classmap.size())
            return;
        classbasemap_type::const_iterator item = classmap.begin();
        for (; item!=classmap.end(); ++item)
        {
            classbase *pclass=(item->second);
            if(item->first == astrclass)
            {
                for(int i=0;i<pclass->size();i++)
                {
                    *m_stream << "            Object Name: " << pclass->getvar(i) <<"   Address: [0x"
                        << pclass->getvarpoint(i)<< "]  "<< "\r\n";
                }
                for(int i=0;i<pclass->funcsize();i++)
                {
                    *m_stream << "             member Function: " << pclass->getfuncname(i) <<"   Type: ["
                        << pclass->getfunctype(i)<< "]  "<< "\r\n";
                }
            }
        }
        *m_stream <<"\r\n";
    }
    //----------------------------------------------------------------------------
    bool CImageParserClass::IsObject(mu::Parser &Pparser,const char *a_szClassObject)
    {
        string astrclassobj(a_szClassObject);
        mu::classbasemap_type classmap = Pparser.GetClassMap();
        if(!classmap.size())
            return false;
        classbasemap_type::const_iterator item = classmap.begin();
        for (; item!=classmap.end(); ++item)
        {
            classbase *pclass=(item->second);
            for(int i=0;i<pclass->size();i++)
            {
                if(astrclassobj == pclass->getvar(i))
                    return true;
            }
        }
        return false;
    }
    //----------------------------------------------------------------------------
    bool CImageParserClass::IsObjectVar(const char *a_sz)
    {
        return IsObject(m_parser,a_sz);
    }
    //----------------------------------------------------------------------------
    void CImageParserClass::ListFormula(mu::Parser &Pparser)
    {
        mu::string_type astring=Pparser.GetFormula();
        *m_stream << " Formula: " <<  astring << "\r\n";
    }
    //---------------------------------------------------------------------------
    void CImageParserClass::SetStream(std::ostream *a_stream)
    {
        assert(a_stream);
        m_stream = a_stream;
    }
    //---------------------------------------------------------------------------
    void CImageParserClass::SetCreateCodeStream(std::ostream *a_stream)
    {
        assert(a_stream);
        m_createstream = a_stream;
    }
    //---------------------------------------------------------------------------
    bool CImageParserClass::Compile(const char *a_szLine)
    {
        try
        {
            if (CommandLine(a_szLine))
            {
                m_iget=0;
                return 1;
            }
            //else
            {
                SetExpr(a_szLine);
                *m_stream<<"Result:"<<Eval()<<"\r\n";

                *m_stream<<"================Build: 1 OK , 0 Fail ============"<<"\r\n";
            }
        }
        catch(mu::Parser::exception_type &e)
        {

            *m_stream  << "\nError: ";
            //*m_stream  << "------\r\n";
            *m_stream  << " Message:  " << e.GetMsg() << "\r\n";
            //*m_stream  << " Formula:  " << e.GetExpr() << "\r\n";
            *m_stream  << " Token: " << e.GetToken() ;// << "\r\n";
            *m_stream  << " Position: " << (int)e.GetPos() << "\r\n";
            *m_stream  << " Errc: " << e.GetCode()  << "\r\n";
            *m_stream  <<"================Build: 0 OK , 1 Fail ============"<<"\r\n";
            return 0;
        }
        return 1;
    }

#if defined(_WIN32x)
    //---------------------------------------------------------------------------
    void CImageParserClass::ShowImageParserElement(HDC Hdc,int ishowtype)
    {
        eShapeClass m_edrawpoint;
        switch(ishowtype)
        {
        case 1:
            m_edrawpoint=ShapeClass_Point_OX;
            break;
        case 2:
            m_edrawpoint=ShapeClass_Point_O;
            break;
        case 3:
            m_edrawpoint=ShapeClass_Point_DOT;
            break;
        case 4:
            m_edrawpoint=ShapeClass_Point_X;
            break;
        default:
            m_edrawpoint=ShapeClass_Point_X;
            break;
        }
        mu::Parser &Pparser=m_parser;
        mu::classbasemap_type classmap = Pparser.GetClassMap();
        if(!classmap.size())
            return;
        classbasemap_type::const_iterator item = classmap.end();

        for (; item!=classmap.begin(); )
        {
            --item;
            classbase *pclass=(item->second);

            if("CWinImage"==item->first)
            {

                CWinImage/*CWinImageOpCV*/ *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CWinImage/*CWinImageOpCV*/ *)pclass->getvarpoint(i);
                    if(apimage->GetShow())
                        apimage->Draw(Hdc);
                }
            }
            else if("CModule"==item->first)
            {
                CGDIManager *apmodule=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apmodule=(CGDIManager *)pclass->getvarpoint(i);
                    apmodule->Draw(Hdc,apmodule->GetShow());
                }
            }
            else if("GDIimage"==item->first)
            {
                CGDIImage *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CGDIImage *)pclass->getvarpoint(i);
                    if(apimage->GetShow())
                        apimage->Draw(Hdc);
                }
            }
            else if("GDIimage24"==item->first)
            {
                CGDIImage24 *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CGDIImage24 *)pclass->getvarpoint(i);
                    switch(apimage->GetShow())
                    {
                    case 1:
                        apimage->Draw(Hdc);
                    break;
                    }
                }
            }
            else if("GDIimage32"==item->first)
            {
                CGDIImage32 *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CGDIImage32 *)pclass->getvarpoint(i);
                    switch(apimage->GetShow())
                    {
                        case 8:
                            apimage->DrawFifo(Hdc);
                        break;
                        case 2:
                            apimage->DrawShowXY(Hdc);
                        break;
                        default:
                            apimage->Draw(Hdc);
                        break;
                    }
                }
            }
            else if("CxImage"==item->first)
            {
                CxVisionImage *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CxVisionImage *)pclass->getvarpoint(i);
                    if(apimage->GetShow())
                        apimage->Draw(Hdc);
                }
            }
            else if("GDIPos"==item->first)
                {
                CGDIPosition *apimage=0;
                for(int i=0;i<pclass->size();i++)
                    {
                    apimage=(CGDIPosition *)pclass->getvarpoint(i);
                    switch(apimage->GetShow())
                        {
                        case 2:
                            apimage->DrawShowXY(Hdc);
                            break;
                        default:
                            apimage->Draw(Hdc);
                            break;
                        }
                    }
                }
            else if("CFindPointEx"==item->first)
            {
#ifdef USE_CROIMeasurePointEx
                CROIMeasurePointEx *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CROIMeasurePointEx *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,apimage->GetDrawPoint(),apimage->GetShow());
                }
#endif
            }
            else if("EasyLine"==item->first)
            {
#ifdef USE_CEasyLine
                CEasyLine *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CEasyLine *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,m_edrawpoint,apimage->GetShow());
                }
#endif
            }
            else if("CFindLineEx"==item->first)
            {
#ifdef USE_CROIMeasureLineAex
                CROIMeasureLineAex *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CROIMeasureLineAex *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,m_edrawpoint,apimage->GetShow());
                }
#endif
            }
            else if("CSearchLine"==item->first)
            {
#ifdef USE_CROISearchLine
                CROISearchLine *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CROISearchLine *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,m_edrawpoint,apimage->GetShow());
                }
#endif
            }
            else if("CFindValid"==item->first)
            {
#ifdef USE_CROIMeasureLineValid
                CROIMeasureLineValid *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CROIMeasureLineValid *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,m_edrawpoint,apimage->GetShow());
                }
#endif
            }
            else if("CFind"==item->first)
            {
#ifdef USE_CROIMeasureAnalysis
                CROIMeasureAnalysis *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CROIMeasureAnalysis *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,m_edrawpoint,apimage->GetShow());
                }
#endif
            }
            else if("CFastMatch"==item->first)
            {
#ifdef USE_CFastMatch
                CFastMatch *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CFastMatch *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,m_edrawpoint,apimage->GetShow());
                }
#endif
            }
            else if("COcr"==item->first)
            {
#ifdef USE_CROIOCR
                CROIOCR *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CROIOCR *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,m_edrawpoint,apimage->GetShow());
                }
#endif
            }
            else if("CFindCircleEx"==item->first)
            {
#ifdef USE_CROIMeasureCircleAEx
                CROIMeasureCircleAEx *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CROIMeasureCircleAEx *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,(eShapeClass)apimage->GetPointShow(),apimage->GetShow());
                }
#endif
            }
            else if("CFindTool"==item->first)
            {
#ifdef USE_CROIMeasureCustomTool
                CROIMeasureCustomTool *aptool=0;
                for(int i=0;i<pclass->size();i++)
                {
                    aptool=(CROIMeasureCustomTool *)pclass->getvarpoint(i);
                    aptool->Draw(Hdc,m_edrawpoint,aptool->GetShow());
                }
#endif
            }
            else if("CObjectA"==item->first)
            {
#ifdef USE_CObjectAnalysisExA
                CObjectAnalysisExA *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CObjectAnalysisExA *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,apimage->GetShow());
                }
#endif
            }
            else if("CRecog"==item->first)
            {
#ifdef USE_CShapeRecognition
                CShapeRecognition *aprecog=0;
                for(int i=0;i<pclass->size();i++)
                {
                    aprecog=(CShapeRecognition *)pclass->getvarpoint(i);
                    aprecog->Draw(Hdc,aprecog->GetShow());
                }
#endif
            }
            else if("CMask"==item->first)
            {
#ifdef USE_CObjectAnalysisMask
                CObjectAnalysisMask *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CObjectAnalysisMask *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,apimage->GetShow());
                }
#endif
            }
            else if("CObjectCircleA"==item->first)
            {
#ifdef USE_CObjectAnalysisCircleExA
                CObjectAnalysisCircleExA *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CObjectAnalysisCircleExA *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,m_edrawpoint,apimage->GetShow());
                }
#endif
            }
            else if("CAnalysis"==item->first)
            {
#ifdef USE_CCollectionAnalysis
                CCollectionAnalysis *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CCollectionAnalysis *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,m_edrawpoint,apimage->GetShow());
                }
#endif
            }
            else if("CAnalysisEx"==item->first)
            {
#ifdef USE_CCollectionAnalysisEx
                CCollectionAnalysisEx *apsis=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apsis=(CCollectionAnalysisEx *)pclass->getvarpoint(i);
                    apsis->Draw(Hdc,m_edrawpoint,apsis->GetShow());
                }
#endif
            }
            else if("CModel"==item->first)
            {
#ifdef USE_CCollectionModel
                CCollectionModel *apsis=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apsis=(CCollectionModel *)pclass->getvarpoint(i);
                    apsis->Draw(Hdc,m_edrawpoint,apsis->GetShow());
                }
#endif
            }
            else if("CShapeListA"==item->first)
            {
#ifdef USE_CShapeListA
                CShapeListA *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CShapeListA *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc);
                }
#endif
            }
            else if("CShapeListB"==item->first)
            {
#ifdef USE_CShapeListB
                CShapeListB *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CShapeListB *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc);
                }
#endif
            }
            else if("CShapeFont"==item->first)
            {
                CShapeFont *apfont=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apfont=(CShapeFont *)pclass->getvarpoint(i);
                    apfont->Draw(Hdc);
                }

            }
            else if("CObjectFont"==item->first)
            {
                CObjectFont *apfont=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apfont=(CObjectFont *)pclass->getvarpoint(i);
                    apfont->Draw(Hdc);
                }

            }
            else if("CShapeAlloc"==item->first)
            {
#ifdef USE_CShapeAlloc
                CShapeAlloc *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CShapeAlloc *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc);
                }
#endif
            }
            else if("CTRect"==item->first)
            {
#ifdef USE_CROIMeasureTRect
                CROIMeasureTRect *aptrect=0;
                for(int i=0;i<pclass->size();i++)
                {
                    aptrect=(CROIMeasureTRect *)pclass->getvarpoint(i);

                    aptrect->Draw(Hdc,m_edrawpoint,aptrect->GetShow());
                }
#endif
            }
            else if("CRect"==item->first)
            {
#ifdef USE_CROIMeasureRect
                CROIMeasureRect *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {

                    apimage=(CROIMeasureRect *)pclass->getvarpoint(i);

                    apimage->Draw(Hdc,m_edrawpoint,apimage->GetShow());
                }
#endif
            }
            else if("CROI"==item->first)
            {
#ifdef USE_COrigialROI
                COrigialROI *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(COrigialROI *)pclass->getvarpoint(i);
                    apimage->DrawFrame(Hdc);
                }
#endif
            }
            else if("CCalibration"==item->first)
            {
#ifdef USE_CCalibration
                CCalibration *pcal=0;
                for(int i=0;i<pclass->size();i++)
                {
                    pcal=(CCalibration *)pclass->getvarpoint(i);
                    pcal->Draw(Hdc);
                }
#endif
            }
            else if("CCalibrationEx"==item->first)
            {
#ifdef USE_CCalibrationEx
                CCalibrationEx *pcal=0;
                for(int i=0;i<pclass->size();i++)
                {
                    pcal=(CCalibrationEx *)pclass->getvarpoint(i);
                    pcal->Draw(Hdc);
                }
#endif
            }
            else if("CPoint"==item->first)
            {
                Point_Shape*pPoint=0;
                for(int i=0;i<pclass->size();i++)
                {
                    pPoint=(Point_Shape *)pclass->getvarpoint(i);

                    pPoint->drawshape(Hdc);
                }

            }
            else if("CLine"==item->first)
            {
                Line_Shape*pLine=0;
                for(int i=0;i<pclass->size();i++)
                {
                    pLine=(Line_Shape *)pclass->getvarpoint(i);
                    pLine->draw(Hdc);
                }

            }
            else if("CCustomShape"==item->first)
            {
                CCustomShape *pshape = 0;
                for(int i=0;i<pclass->size();i++)
                {
                    pshape=(CCustomShape *)pclass->getvarpoint(i);
                    if(pshape->GetShow())
                        pshape->Draw(Hdc);
                }

            }
            else if("CPolygonShape"==item->first)
            {

                CPolygonShape *pshape = 0;
                for(int i=0;i<pclass->size();i++)
                {
                    pshape=(CPolygonShape *)pclass->getvarpoint(i);
                    if(pshape->GetShow())
                    pshape->Draw(Hdc);
                }

            }
            else if("CLineShape"==item->first)
            {

                CLineShape *pline = 0;
                for(int i=0;i<pclass->size();i++)
                {
                    pline=(CLineShape *)pclass->getvarpoint(i);
                    if(pline->GetShow())
                    pline->DrawShape(Hdc);
                }

            }
            else if("CCircleShape"==item->first)
            {

                CCircleShape *pcircle = 0;
                for(int i=0;i<pclass->size();i++)
                {
                    pcircle=(CCircleShape *)pclass->getvarpoint(i);
                    if(pcircle->GetShow())
                    pcircle->Draw(Hdc);
                }

            }
            else if("CEllipseShape"==item->first)
            {

                CEllipseShape *pellipse = 0;
                for(int i=0;i<pclass->size();i++)
                {
                    pellipse=(CEllipseShape *)pclass->getvarpoint(i);
                    pellipse->Draw(Hdc);
                }

            }
            else if("clistbrun"==item->first)
            {
#ifdef USE_CShapeListBTestRun
                CShapeListBTestRun *prun =0;
                for(int i=0;i<pclass->size();i++)
                {
                    prun=(CShapeListBTestRun *)pclass->getvarpoint(i);
                    prun->draw(Hdc);
                }
#endif
            }
            else if("CROIHistg"==item->first)
            {
#ifdef USE_CROIHisg
                CROIHisg *proihisg =0;
                for(int i=0;i<pclass->size();i++)
                {
                    proihisg=(CROIHisg *)pclass->getvarpoint(i);
                    proihisg->Draw(Hdc,proihisg->GetShow());
                }
#endif
            }
            else if("CHistogram"==item->first)
            {
#ifdef USE_CHistogram
                CHistogram *proihisg =0;
                for(int i=0;i<pclass->size();i++)
                {
                    proihisg=(CHistogram *)pclass->getvarpoint(i);
                    proihisg->Draw(Hdc,proihisg->GetShow());
                }
#endif
            }
            else if("CROIGrid"==item->first)
            {
#ifdef USE_CROIGrid
                CROIGrid *pGrid =0;
                for(int i=0;i<pclass->size();i++)
                {
                    pGrid=(CROIGrid *)pclass->getvarpoint(i);
                    //pGrid->Draw(Hdc,pGrid->GetShow());
                    int ishow = pGrid->GetShow();
                    pGrid->Draw(Hdc,0,0,0,0,ishow);
                }
#endif
            }
            else if("CEdgeAnalysis"==item->first)
            {
#ifdef USE_CEdgeAnalysis
                CEdgeAnalysis *pedgeansis =0;
                for(int i=0;i<pclass->size();i++)
                {
                    pedgeansis=(CEdgeAnalysis *)pclass->getvarpoint(i);
                    pedgeansis->Draw(Hdc,pedgeansis->GetShow());
                }
#endif
            }
            else if("CText"==item->first)
            {
                CText *ptext = 0;
                for(int i=0;i<pclass->size();i++)
                {
                    ptext=(CText *)pclass->getvarpoint(i);
                    ptext->Draw(Hdc);
                }

            }
            else if("CROIList"==item->first)
            {
#ifdef USE_CROIGroup
                CROIGroup *plist = 0;
                for(int i=0;i<pclass->size();i++)
                {
                    plist=(CROIGroup *)pclass->getvarpoint(i);
                    plist->Draw(Hdc,plist->GetShow());
                }
#endif
            }
        }
    }
    //---------------------------------------------------------------------------
    void CImageParserClass::ShowImageParserElementA(HDC Hdc,int ix,int iy,int cx,int cy,int ishowtype)
    {
        eShapeClass m_edrawpoint;
        switch(ishowtype)
        {
        case 1:
            m_edrawpoint=ShapeClass_Point_OX;
            break;
        case 2:
            m_edrawpoint=ShapeClass_Point_O;
            break;
        case 3:
            m_edrawpoint=ShapeClass_Point_DOT;
            break;
        case 4:
            m_edrawpoint=ShapeClass_Point_X;
            break;
        default:
            m_edrawpoint=ShapeClass_Point_X;
            break;
        }
        mu::Parser &Pparser=m_parser;
        mu::classbasemap_type classmap = Pparser.GetClassMap();
        if(!classmap.size())
            return;
        classbasemap_type::const_iterator item = classmap.end();

        for (; item!=classmap.begin(); )
        {
            --item;
            classbase *pclass=(item->second);

            if("CWinImage"==item->first)
            {
#ifdef USE_CWinImage
                CWinImage/*CWinImageOpCV*/ *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CWinImage/*CWinImageOpCV*/ *)pclass->getvarpoint(i);
                    if(apimage->GetShow())
                        apimage->Draw(Hdc);
                }
#endif
            }
            else if("GDIimage"==item->first)
            {
                CGDIImage *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CGDIImage *)pclass->getvarpoint(i);
                    if(apimage->GetShow())
                        apimage->Draw(Hdc,ix,iy);
                }

            }
            else if("GDIimage24"==item->first)
            {
                CGDIImage24 *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CGDIImage24 *)pclass->getvarpoint(i);
                    switch(apimage->GetShow())
                    {
                    case 1:
                        apimage->Draw(Hdc);
                        break;
                    }
                }

            }
            else if("GDIimage32"==item->first)
            {

                CGDIImage32 *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CGDIImage32 *)pclass->getvarpoint(i);
                    switch(apimage->GetShow())
                    {
                    case 8:
                        apimage->DrawFifo(Hdc);
                        break;
                    case 2:
                        apimage->DrawShowXY(Hdc);
                        break;
                    default:
                        apimage->Draw(Hdc,ix,iy);
                        break;
                    }
                }

            }
            else if("CxImage"==item->first)
            {

                CxVisionImage *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CxVisionImage *)pclass->getvarpoint(i);
                    if(apimage->GetShow())
                        apimage->Draw(Hdc,ix,iy,cx,cy);
                }

            }
            else if("GDIPos"==item->first)
            {

                CGDIPosition *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CGDIPosition *)pclass->getvarpoint(i);
                    switch(apimage->GetShow())
                    {
                    case 2:
                        apimage->DrawShowXY(Hdc);
                        break;
                    default:
                        apimage->Draw(Hdc);
                        break;
                    }
                }

            }
            else if("CModule"==item->first)
            {

                CGDIManager *apmodule=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apmodule=(CGDIManager *)pclass->getvarpoint(i);
                    apmodule->Draw(Hdc,apmodule->GetShow());
                }

            }
            else if("CFindPointEx"==item->first)
            {
#ifdef USE_CROIMeasurePointEx
                CROIMeasurePointEx *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CROIMeasurePointEx *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,apimage->GetDrawPoint(),apimage->GetShow());
                }
#endif
            }
            else if("EasyLine"==item->first)
            {
#ifdef USE_CEasyLine
                CEasyLine *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CEasyLine *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,m_edrawpoint,apimage->GetShow());
                }
#endif
            }
            else if("CFindLineEx"==item->first)
            {
#ifdef USE_CROIMeasureLineAex
                CROIMeasureLineAex *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CROIMeasureLineAex *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,m_edrawpoint,apimage->GetShow());
                }
#endif
            }
            else if("CSearchLine"==item->first)
            {
#ifdef USE_CROISearchLine
                CROISearchLine *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CROISearchLine *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,m_edrawpoint,apimage->GetShow());
                }
#endif
            }
            else if("CFindValid"==item->first)
            {
#ifdef USE_CROIMeasureLineValid
                CROIMeasureLineValid *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CROIMeasureLineValid *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,m_edrawpoint,apimage->GetShow());
                }
#endif
            }
            else if("CFind"==item->first)
            {
#ifdef USE_CROIMeasureAnalysis
                CROIMeasureAnalysis *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CROIMeasureAnalysis *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,m_edrawpoint,apimage->GetShow());
                }
#endif
            }
            else if("CFastMatch"==item->first)
            {
#ifdef USE_CFastMatch
                CFastMatch *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CFastMatch *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,m_edrawpoint,apimage->GetShow());
                }
#endif
            }
            else if("COcr"==item->first)
            {
#ifdef USE_CROIOCR
                CROIOCR *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CROIOCR *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,m_edrawpoint,apimage->GetShow());
                }
#endif
            }
            else if("CFindCircleEx"==item->first)
            {
#ifdef USE_CROIMeasureCircleAEx
                CROIMeasureCircleAEx *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CROIMeasureCircleAEx *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,(eShapeClass)apimage->GetPointShow(),apimage->GetShow());
                }
#endif
            }
            else if("CFindTool"==item->first)
            {
#ifdef USE_CROIMeasureCustomTool
                CROIMeasureCustomTool *aptool=0;
                for(int i=0;i<pclass->size();i++)
                {
                    aptool=(CROIMeasureCustomTool *)pclass->getvarpoint(i);
                    aptool->Draw(Hdc,m_edrawpoint,aptool->GetShow());
                }
#endif
            }
            else if("CObjectA"==item->first)
            {
#ifdef USE_CObjectAnalysisExA
                CObjectAnalysisExA *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CObjectAnalysisExA *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,apimage->GetShow());
                }
#endif
            }
            else if("CRecog"==item->first)
            {
#ifdef USE_CShapeRecognition
                CShapeRecognition *aprecog=0;
                for(int i=0;i<pclass->size();i++)
                {
                    aprecog=(CShapeRecognition *)pclass->getvarpoint(i);
                    aprecog->Draw(Hdc,aprecog->GetShow());
                }
#endif
            }
            else if("CMask"==item->first)
            {
#ifdef USE_CObjectAnalysisMask
                CObjectAnalysisMask *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CObjectAnalysisMask *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,apimage->GetShow());
                }
#endif
            }
            else if("CObjectCircleA"==item->first)
            {
#ifdef USE_CObjectAnalysisCircleExA
                CObjectAnalysisCircleExA *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CObjectAnalysisCircleExA *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,m_edrawpoint,apimage->GetShow());
                }
#endif
            }
            else if("CAnalysis"==item->first)
            {
#ifdef USE_CCollectionAnalysis
                CCollectionAnalysis *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CCollectionAnalysis *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc,m_edrawpoint,apimage->GetShow());
                }
#endif
            }
            else if("CAnalysisEx"==item->first)
            {
#ifdef USE_CCollectionAnalysisEx
                CCollectionAnalysisEx *apsis=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apsis=(CCollectionAnalysisEx *)pclass->getvarpoint(i);
                    apsis->Draw(Hdc,m_edrawpoint,apsis->GetShow());
                }
#endif
            }
            else if("CModel"==item->first)
            {
#ifdef USE_CCollectionModel
                CCollectionModel *apsis=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apsis=(CCollectionModel *)pclass->getvarpoint(i);
                    apsis->Draw(Hdc,m_edrawpoint,apsis->GetShow());
                }
#endif
            }
            else if("CShapeListA"==item->first)
            {
#ifdef USE_CShapeListA
                CShapeListA *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CShapeListA *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc);
                }
#endif
            }
            else if("CShapeListB"==item->first)
            {
#ifdef USE_CShapeListB
                CShapeListB *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CShapeListB *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc);
                }
#endif
            }
            else if("CShapeFont"==item->first)
            {

                CShapeFont *apfont=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apfont=(CShapeFont *)pclass->getvarpoint(i);
                    apfont->Draw(Hdc);
                }

            }
            else if("CObjectFont"==item->first)
            {

                CObjectFont *apfont=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apfont=(CObjectFont *)pclass->getvarpoint(i);
                    apfont->Draw(Hdc);
                }

            }
            else if("CShapeAlloc"==item->first)
            {
#ifdef USE_CShapeAlloc
                CShapeAlloc *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CShapeAlloc *)pclass->getvarpoint(i);
                    apimage->Draw(Hdc);
                }
#endif
            }
            else if("CTRect"==item->first)
            {
#ifdef USE_CROIMeasureTRect
                CROIMeasureTRect *aptrect=0;
                for(int i=0;i<pclass->size();i++)
                {
                    aptrect=(CROIMeasureTRect *)pclass->getvarpoint(i);

                    aptrect->Draw(Hdc,m_edrawpoint,aptrect->GetShow());
                }
#endif
            }
            else if("CRect"==item->first)
            {
#ifdef USE_CROIMeasureRect
                CROIMeasureRect *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {

                    apimage=(CROIMeasureRect *)pclass->getvarpoint(i);

                    apimage->Draw(Hdc,m_edrawpoint,apimage->GetShow());
                }
#endif
            }
            else if("CROI"==item->first)
            {
#ifdef USE_COrigialROI
                COrigialROI *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(COrigialROI *)pclass->getvarpoint(i);
                    apimage->DrawFrame(Hdc);
                }
#endif
            }
            else if("CCalibration"==item->first)
            {
#ifdef USE_CCalibration
                CCalibration *pcal=0;
                for(int i=0;i<pclass->size();i++)
                {
                    pcal=(CCalibration *)pclass->getvarpoint(i);
                    pcal->Draw(Hdc);
                }
#endif
            }
            else if("CCalibrationEx"==item->first)
            {
#ifdef USE_CCalibrationEx
                CCalibrationEx *pcal=0;
                for(int i=0;i<pclass->size();i++)
                {
                    pcal=(CCalibrationEx *)pclass->getvarpoint(i);
                    pcal->Draw(Hdc);
                }
#endif
            }
            else if("CPoint"==item->first)
            {
                Point_Shape*pPoint=0;
                for(int i=0;i<pclass->size();i++)
                {
                    pPoint=(Point_Shape *)pclass->getvarpoint(i);

                    pPoint->drawshape(Hdc);
                }


            }
            else if("CLine"==item->first)
            {
                Line_Shape*pLine=0;
                for(int i=0;i<pclass->size();i++)
                {
                    pLine=(Line_Shape *)pclass->getvarpoint(i);
                    pLine->draw(Hdc);
                }
            }
            else if("CCustomShape"==item->first)
            {
                CCustomShape *pshape = 0;
                for(int i=0;i<pclass->size();i++)
                {
                    pshape=(CCustomShape *)pclass->getvarpoint(i);
                    if(pshape->GetShow())
                        pshape->Draw(Hdc);
                }
            }
            else if("CPolygonShape"==item->first)
            {

                CPolygonShape *pshape = 0;
                for(int i=0;i<pclass->size();i++)
                {
                    pshape=(CPolygonShape *)pclass->getvarpoint(i);
                    if(pshape->GetShow())
                        pshape->Draw(Hdc);
                }

            }
            else if("CLineShape"==item->first)
            {

                CLineShape *pline = 0;
                for(int i=0;i<pclass->size();i++)
                {
                    pline=(CLineShape *)pclass->getvarpoint(i);
                    if(pline->GetShow())
                        pline->DrawShape(Hdc);
                }

            }
            else if("CCircleShape"==item->first)
            {

                CCircleShape *pcircle = 0;
                for(int i=0;i<pclass->size();i++)
                {
                    pcircle=(CCircleShape *)pclass->getvarpoint(i);
                    if(pcircle->GetShow()==1)
                        pcircle->Draw(Hdc);
                    else if(pcircle->GetShow()>1)
                        pcircle->DrawBrush(Hdc,pcircle->GetShow());
                }

            }
            else if("CEllipseShape"==item->first)
            {

                CEllipseShape *pellipse = 0;
                for(int i=0;i<pclass->size();i++)
                {
                    pellipse=(CEllipseShape *)pclass->getvarpoint(i);
                    pellipse->Draw(Hdc);
                }

            }
            else if("clistbrun"==item->first)
            {
#ifdef USE_CShapeListBTestRun
                CShapeListBTestRun *prun =0;
                for(int i=0;i<pclass->size();i++)
                {
                    prun=(CShapeListBTestRun *)pclass->getvarpoint(i);
                    prun->draw(Hdc);
                }
#endif
            }
            else if("CROIHistg"==item->first)
            {
#ifdef USE_CROIHisg
                CROIHisg *proihisg =0;
                for(int i=0;i<pclass->size();i++)
                {
                    proihisg=(CROIHisg *)pclass->getvarpoint(i);
                    proihisg->Draw(Hdc,proihisg->GetShow());
                }
#endif
            }
            else if("CHistogram"==item->first)
            {
#ifdef USE_CHistogram
                CHistogram *proihisg =0;
                for(int i=0;i<pclass->size();i++)
                {
                    proihisg=(CHistogram *)pclass->getvarpoint(i);
                    proihisg->Draw(Hdc,proihisg->GetShow());
                }
#endif
            }
            else if("CROIGrid"==item->first)
            {
#ifdef USE_CROIGrid
                CROIGrid *pGrid =0;
                for(int i=0;i<pclass->size();i++)
                {
                    pGrid=(CROIGrid *)pclass->getvarpoint(i);

                    int ishow = pGrid->GetShow();
                    pGrid->Draw(Hdc,ix,iy,0,0,ishow);
                    //pGrid->Draw(Hdc,ix,iy,pGrid->GetShow());
                }
#endif
            }
            else if("CEdgeAnalysis"==item->first)
            {
#ifdef USE_CEdgeAnalysis
                CEdgeAnalysis *pedgeansis =0;
                for(int i=0;i<pclass->size();i++)
                {
                    pedgeansis=(CEdgeAnalysis *)pclass->getvarpoint(i);
                    pedgeansis->Draw(Hdc,pedgeansis->GetShow());
                }
#endif
            }
            else if("CText"==item->first)
            {
                CText *ptext = 0;
                for(int i=0;i<pclass->size();i++)
                {
                    ptext=(CText *)pclass->getvarpoint(i);
                    ptext->Draw(Hdc);
                }
            }
        }
    }

    void CImageParserClass::ShowParserGDIimage(HDC Hdc)
    {
        mu::Parser &Pparser=m_parser;
        mu::classbasemap_type classmap = Pparser.GetClassMap();
        if(!classmap.size())
            return;
        classbasemap_type::const_iterator item = classmap.end();

        for (; item!=classmap.begin(); )
        {
            --item;
            classbase *pclass=(item->second);

            if("GDIimage32"==item->first)
            {
                CGDIImage32 *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CGDIImage32 *)pclass->getvarpoint(i);
                    switch(apimage->GetShow())
                    {
                    case 8:
                        apimage->DrawFifo(Hdc);
                        break;
                    case 2:
                        apimage->DrawShowXY(Hdc);
                        break;
                    default:
                        apimage->Draw(Hdc);
                        break;
                    }
                }
                return;
            }
            else if("GDIimage24"==item->first)
            {
                CGDIImage24 *apimage=0;
                for(int i=0;i<pclass->size();i++)
                {
                    apimage=(CGDIImage24 *)pclass->getvarpoint(i);
                    switch(apimage->GetShow())
                    {
                    case 1:
                        apimage->Draw(Hdc);
                        break;
                    }
                }
                return;
            }
        }
    }
    //---------------------------------------------------------------------------
#endif

    void CImageParserClass::ResetRun()
    {
        mu::Parser &Pparser=m_parser;
        mu::classbasemap_type classmap = Pparser.GetClassMap();
        if(!classmap.size())
            return;
        classbasemap_type::const_iterator item = classmap.end();

        //CGDIManager::Reset();
        for (; item!=classmap.begin(); )
        {
            --item;
            classbase *pclass=(item->second);

            if("GDIimage32"==item->first)
                        {
                            return;
            }
        }
    }
    //---------------------------------------------------------------------------
    void CImageParserClass::DragImageParserElement(int ipointx,int ipointy)
    {
        mu::Parser &Pparser=m_parser;
        mu::classbasemap_type classmap = Pparser.GetClassMap();
        if(!classmap.size())
            return;
        classbasemap_type::const_iterator item = classmap.end();



    }
    //---------------------------------------------------------------------------
    void CImageParserClass::HitTestImageParserElement(int ipointx,int ipointy)
    {
        mu::Parser &Pparser=m_parser;
        mu::classbasemap_type classmap = Pparser.GetClassMap();
        if(!classmap.size())
            return;
        classbasemap_type::const_iterator item = classmap.end();

        for (; item!=classmap.begin(); )
        {
            --item;
            classbase *pclass=(item->second);

            if("CFindPoint"==item->first)
            {
#ifdef USE_CROIMeasurePoint
#endif
            }

        }

    }
    //---------------------------------------------------------------------------
    void CImageParserClass::MouseDownParserElement(int PointX,int PointY)
    {
        mu::Parser &Pparser=m_parser;
        mu::classbasemap_type classmap = Pparser.GetClassMap();
        if(!classmap.size())
            return;
        classbasemap_type::const_iterator item = classmap.end();

        for (; item!=classmap.begin(); )
        {
            --item;
            classbase *pclass=(item->second);

            if("CFastMatch"==item->first)
            {
#ifdef USE_CFastMatch

#endif
            }

        }

    }
    void CImageParserClass::MouseUpParserElement(int PointX,int PointY)
    {
        mu::Parser &Pparser=m_parser;
        mu::classbasemap_type classmap = Pparser.GetClassMap();
        if(!classmap.size())
            return;
        classbasemap_type::const_iterator item = classmap.end();

        for (; item!=classmap.begin(); )
        {
            --item;
            classbase *pclass=(item->second);
            if("CFastMatch"==item->first)
            {
#ifdef USE_CFastMatch

#endif
            }

        }

    }


    bool CImageParserClass::MouseRDownParserElement()
    {

        mu::Parser &Pparser=m_parser;
        mu::classbasemap_type classmap = Pparser.GetClassMap();
        if(!classmap.size())
            return false;
        classbasemap_type::const_iterator item = classmap.end();

        for (; item!=classmap.begin(); )
        {
            --item;
            classbase *pclass=(item->second);
                        if("CPolygonShape"==item->first)
            {

            }
        }

        return false;
    }

    //----------------------------------------------------------------------------
    void CImageParserClass::StopRun()
    {
        m_parser.RunStop();
    }
    //----------------------------------------------------------------------------
    void CImageParserClass::SetRunOk()
    {
        m_parser.RunOk();
    }
    //----------------------------------------------------------------------------
    void CImageParserClass::GetImageObjectAtt()//
    {
        mu::Parser &Pparser=m_parser;
        mu::classbasemap_type classmap = Pparser.GetClassMap();
        if(!classmap.size())
            return;
        classbasemap_type::const_iterator item = classmap.end();

        for (; item!=classmap.begin(); )
        {
            --item;
            classbase *pclass=(item->second);
            if("double"==item->first)
            {
#ifdef USE_CCollectionAnalysisEx
#endif
            }
        }
    }
    //----------------------------------------------------------------------------
    void CImageParserClass::GetImageObjectAutoSave()//
    {
        mu::Parser &Pparser=m_parser;
        mu::classbasemap_type classmap = Pparser.GetClassMap();
        if(!classmap.size())
            return;
        classbasemap_type::const_iterator item = classmap.end();

        for (; item!=classmap.begin(); )
        {
            --item;
            classbase *pclass=(item->second);
            if("double"==item->first)
            {
                double *adouble=0;
                for(int i=0;i<pclass->size();i++)
                {
                    adouble=(double *)pclass->getvarpoint(i);
                    if(adouble)
                    {
                        *m_createstream << pclass->getvar(i) <<"="<<*adouble<<";\r\n";
                    }
                }
            }
            else if("Shape"==item->first)
            {
                QShape *ashape=0;
                for(int i=0;i<pclass->size();i++)
                {
                    ashape=(QShape *)pclass->getvarpoint(i);
                    if(ashape)
                    {
                        *m_createstream << pclass->getvar(i)
                                        <<".setrect("\
                                        <<ashape->rect().x()<<","
                                        <<ashape->rect().y()<<","
                                        <<ashape->rect().width()<<","
                                        <<ashape->rect().height()<<")"
                                        <<";\r\n";
                    }
                }
            }
            else if("ShapeBase"==item->first){}
            else if("LineShape"==item->first){}
        }
    }


    //--------------------------------------------------------------
    void CImageParserClass::CreateClassDef(const char *pclassname,											const char *pclassdef)
    {
        m_parser.DefineCreateClass(pclassname,pclassdef);
    }
    void CImageParserClass::CreateClassFunc(const char *pclassname,
                                            const char *pclassfucname,
                                            const char *pclassfucdef)
    {
        m_parser.DefineCreateClasFun(pclassname,pclassfucname,pclassfucdef);
    }
    void CImageParserClass::ListCreateClassDef(mu::Parser &Pparser
        ,const char *pclassname)
    {
        string_type astr(pclassname);
        string_type strclass;
        CreateClass *paclass;
        int iclassmemsum=0;
        mu::classbasemap_type classmap = Pparser.GetClassMap();
        if(!classmap.size())
            return;

        classbasemap_type::const_iterator item = classmap.begin();
        for (; item!=classmap.end(); ++item)
        {
            classbase *pclass=(item->second);
            strclass= pclass->getclass();
            if(astr == strclass)
            if(pclass->Iscreateclass())
            {
                paclass = (CreateClass*)pclass;
                iclassmemsum = paclass->GetClassMemberNum();
                for(int i=0;i<iclassmemsum;i++)
                {
                    *m_stream<<paclass->GetClassDefName(i)<<" "<<paclass->GetClassMemberName(i) <<";\r\n";
                }
            }
        }
        *m_stream <<"\r\n";
    }
    void CImageParserClass::ListCreateClassFunDef(mu::Parser &Pparser
       ,const char *pclassname
       ,const char *pclassfuncname)
    {
        string_type astr(pclassname);
        string_type strclass;
        CreateClass *paclass;
        int iclassmemsum=0;
        mu::classbasemap_type classmap = Pparser.GetClassMap();
        if(!classmap.size())
            return;

        classbasemap_type::const_iterator item = classmap.begin();
        for (; item!=classmap.end(); ++item)
        {
            classbase *pclass=(item->second);
            strclass= pclass->getclass();
            if(astr == strclass)
                if(pclass->Iscreateclass())
                {
                    paclass = (CreateClass*)pclass;
                    *m_stream<< paclass->GetFuncDef(pclassfuncname)<<"\r\n";
                }
        }
        *m_stream <<"\r\n";
    }
    void CImageParserClass::ListCreateClassDef(const char *pclassname)
    {
        ListCreateClassDef(m_parser,pclassname);
    }
    void CImageParserClass::ListCreateClassFunDef(const char *pclassname
        ,const char *pclassfuncname)
    {
        ListCreateClassFunDef(m_parser,pclassname,pclassfuncname);
    }
}
