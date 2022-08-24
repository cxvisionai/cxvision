#include "muParserDef.h"
#include "muParserTest.h"
#include "ImageParserClass.h"
#include "imagebase.h"
#include "imagelist.h"
#include "shapebase.h"
#include "shape.h"
#include "grid.h"
#include "findline.h"
#include "findobject.h"
#include "gridobject.h"
#include "fastmatch.h"
#include "orc.h"
#include "imagecodeparser.h"
#include "imageroi.h"
#include "backimagemanager.h"
#include "../p2p/dialog.h"
#include "../AI/stringai.h"
#ifdef USE_CAM
//#include "HWGrabImage.h"
#include "qt_hik.h"
#endif
#ifdef USE_USBCAM
#include "usbcam.h"
#endif

typedef std::vector<double> vectordouble;
typedef std::vector<int> vectorint;

class SmartDouble
{
    vectordouble m_vectresult;
    vectordouble m_vectdouble;
public:
    SmartDouble(){}
    ~SmartDouble(){}
    void push(double dvalue,double dresult){m_vectdouble.push_back(dvalue);m_vectresult.push_back(dresult);}
    double getvalue(int inum){return m_vectdouble[inum];}
    double getresult(int inum){return m_vectresult[inum];}

    void set(double inum,double dvalue,double dresult)
    {m_vectdouble[((int)inum)]=dvalue;m_vectresult[((int)inum)]=dresult;}
    void clear(){m_vectdouble.clear();m_vectresult.clear();}
    int size(){return m_vectdouble.size();}
    int getvaluetimes(double dvalue)
    {
        int itimes=0;
        for(int i =0;i<m_vectdouble.size();i++)
        {
            if(m_vectdouble[i]==dvalue)
                itimes=itimes+1;
        }
        return itimes;
    }
    double average()
    {
        double daverage=0;
        for(int i=0;i<m_vectdouble.size();i++)
        {
            daverage +=m_vectdouble[i];
        }
        return daverage/m_vectdouble.size();
    }
    double maxvalue()
    {
        double dmax=-11111;
        for(int i=0;i<m_vectdouble.size();i++)
        {
            if(dmax<m_vectdouble[i])
                dmax =m_vectdouble[i];
        }
        return dmax;
    }
    int maxnum()
    {
        double dmax=-11111;int i;
        for(i=0;i<m_vectdouble.size();i++)
        {
            if(dmax<m_vectdouble[i])
                dmax =m_vectdouble[i];
        }
        return i;
    }
    double minvalue()
    {
        double dmin=9999;
        for(int i=0;i<m_vectdouble.size();i++)
        {
            if(dmin<m_vectdouble[i])
                dmin =m_vectdouble[i];
        }
        return dmin;
    }
    void save(const char * pchar)
    {
        int isize = m_vectdouble.size();
        if(isize<=0)
            return;
        FILE    *rf = fopen(pchar, "w+");
       if ( rf == nullptr)
           return;
       rewind(rf);
       for(int i=0;i<isize-1;i++)
       {
           double dv = m_vectdouble[i];
           double dr = m_vectresult[i];
           fprintf(rf,"%f$%f,",dv,dr);
       }

       fprintf(rf,"%f",m_vectdouble[isize-1]);
       fclose(rf);
    }
    void load(const char * pchar)
    {
        clear();
        FILE    *rf = fopen(pchar, "rb");
        if(nullptr==rf)
            return;
        fseek(rf,0,SEEK_END);
        int filesize = ftell(rf);
        char *pcharget =new char[filesize+10];
        memset(pcharget,0,filesize+10);
        rewind(rf);
        fread((char*)(pcharget),filesize,1,rf);

        QString qstr = pcharget;
        QStringList strnumlist = qstr.split(",");
        for(int i=0;i<strnumlist.size();i++)
        {
            QStringList strdr = strnumlist.at(i).split("$");
            double dvalue = strdr.at(0).toDouble();
            double dresult = strdr.at(1).toDouble();
            push(dvalue,dresult);
        }
        delete []pcharget;
        fclose(rf);
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

/**/
#if defined(Q_OS_UNIX)
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

#endif

               dialogOPT *pdialogopt = 0;
               m_parser.DefineClass("dialogopt",pdialogopt);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"viewtimer",&dialogOPT::viewtimer);
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

               m_parser.DefineClassFun("dialogopt",pdialogopt,"flashview",&dialogOPT::flashview);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"updateview",&dialogOPT::updateview);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"showlog",&dialogOPT::showlog);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"resetrun",&dialogOPT::resetrun);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"showdlg",&dialogOPT::showdlg);

               m_parser.DefineClassFun("dialogopt",pdialogopt,"showstring",&dialogOPT::showstring);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"urlfilter",&dialogOPT::urlfilter);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"filextfilter",&dialogOPT::filextfilter);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"urlneedstring",&dialogOPT::urlneedstring);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"clearfilter",&dialogOPT::clearfilter);

               m_parser.DefineClassFun("dialogopt",pdialogopt,"setstring",&dialogOPT::setcurstring);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"addstring",&dialogOPT::addstring);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"addparservaluestring",&dialogOPT::addparservaluestring);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"com2tcpipcurstring",&dialogOPT::com2tcpipcurstring);

             //  m_parser.DefineClassFun("dialogopt",pdialogopt,"camshow",&dialogOPT::CamTest);
             //  m_parser.DefineClassFun("dialogopt",pdialogopt,"showdebug",&dialogOPT::showdebug);

               m_parser.DefineClassFun("dialogopt",pdialogopt,"connect2plc",&dialogOPT::connect2PLC);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"write2plc",&dialogOPT::write2PLC);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"runplctimer",&dialogOPT::runplctimer);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"stopplctimer",&dialogOPT::stopplctimer);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"setplctrig",&dialogOPT::setplctrig);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"setplctrigrun0",&dialogOPT::setplctrigrun0);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"setplctrigrun1",&dialogOPT::setplctrigrun1);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"setplctrigrun2",&dialogOPT::setplctrigrun2);




               m_parser.DefineClassFun("dialogopt",pdialogopt,"modbus",&dialogOPT::modbus);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"setmodbus",&dialogOPT::setmodbus);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"runmodbustimer",&dialogOPT::runmodbustimer);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"stopmodbustimer",&dialogOPT::stopmodbustimer);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"setmodbustrig",&dialogOPT::setmodbustrig);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"setmodbusoutput",&dialogOPT::setmodbusoutput);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"modbusoutput",&dialogOPT::modbusoutput);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"setmodbustrigrun0",&dialogOPT::setmodbustrigrun0);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"setmodbustrigrun1",&dialogOPT::setmodbustrigrun1);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"setmodbustrigrun2",&dialogOPT::setmodbustrigrun2);

               m_parser.DefineClassFun("dialogopt",pdialogopt,"resetmodbus",&dialogOPT::resetmodbus);



   //            m_parser.DefineClassFun("dialogopt",pdialogopt,"softtrigset",&dialogOPT::softtrigset);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"setstyle",&dialogOPT::setstyle);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"savevalue",&dialogOPT::savevalue);

               m_parser.DefineClassFun("dialogopt",pdialogopt,"resettime",&dialogOPT::resettime);
               m_parser.DefineClassFun("dialogopt",pdialogopt,"getruntime",&dialogOPT::getruntime);




 //              m_parser.DefineClassFun("dialogopt",pdialogopt,"startslave",&dialogOPT::startslave);
  //             m_parser.DefineClassFun("dialogopt",pdialogopt,"showslave",&dialogOPT::showslave);



#ifdef USE_USBCAM
               USBCam *pusbcam = 0;
               m_parser.DefineClass("USBCam",pusbcam);
               m_parser.DefineClassFun("USBCam",pusbcam,"Show",&USBCam::Show);
               m_parser.DefineClassFun("USBCam",pusbcam,"start",&USBCam::Start);
               m_parser.DefineClassFun("USBCam",pusbcam,"stop",&USBCam::Stop);
               m_parser.DefineClassFun("USBCam",pusbcam,"setimage",&USBCam::SetImage);
               m_parser.DefineClassFun("USBCam",pusbcam,"continuecap",&USBCam::ContinueCapture);
               m_parser.DefineClassFun("USBCam",pusbcam,"stopcap",&USBCam::StopCapture);
               m_parser.DefineClassFun("USBCam",pusbcam,"setimagelist",&USBCam::SetImageList);
               m_parser.DefineClassFun("USBCam",pusbcam,"resettrg",&USBCam::resettrg);
               m_parser.DefineClassFun("USBCam",pusbcam,"gettrgnum",&USBCam::gettrgnum);
               m_parser.DefineClassFun("USBCam",pusbcam,"getopt",&USBCam::getopt);


#if defined(Q_OS_UNIX)
               m_parser.DefineClassFun("USBCam",pusbcam,"getbrightness",&USBCam::GetBrightness);
               m_parser.DefineClassFun("USBCam",pusbcam,"getcontrast",&USBCam::GetContrast);
               m_parser.DefineClassFun("USBCam",pusbcam,"getsaturation",&USBCam::GetSaturation);
               m_parser.DefineClassFun("USBCam",pusbcam,"gethue",&USBCam::GetHue);
               m_parser.DefineClassFun("USBCam",pusbcam,"getgamma",&USBCam::GetGamma);
               m_parser.DefineClassFun("USBCam",pusbcam,"getwhiteness",&USBCam::GetWhiteness);
               m_parser.DefineClassFun("USBCam",pusbcam,"gethflip",&USBCam::GetHflip);
               m_parser.DefineClassFun("USBCam",pusbcam,"getvfilp",&USBCam::GetVfilp);
               m_parser.DefineClassFun("USBCam",pusbcam,"gethueAuto",&USBCam::GetHueAuto);
               m_parser.DefineClassFun("USBCam",pusbcam,"getexposureAuto",&USBCam::GetExposureAuto);
               m_parser.DefineClassFun("USBCam",pusbcam,"getfocusAuto",&USBCam::GetFocusAuto);
               m_parser.DefineClassFun("USBCam",pusbcam,"getgain",&USBCam::GetGain);
               m_parser.DefineClassFun("USBCam",pusbcam,"getexposure",&USBCam::GetExposure);
               m_parser.DefineClassFun("USBCam",pusbcam,"getfocus",&USBCam::GetFocus);
               m_parser.DefineClassFun("USBCam",pusbcam,"setbrightness",&USBCam::SetBrightness);
               m_parser.DefineClassFun("USBCam",pusbcam,"setcontrast",&USBCam::SetContrast);
               m_parser.DefineClassFun("USBCam",pusbcam,"setsaturation",&USBCam::SetSaturation);
               m_parser.DefineClassFun("USBCam",pusbcam,"sethue",&USBCam::SetHue);
               m_parser.DefineClassFun("USBCam",pusbcam,"setgamma",&USBCam::SetGamma);
               m_parser.DefineClassFun("USBCam",pusbcam,"setwhiteness",&USBCam::SetWhiteness);
               m_parser.DefineClassFun("USBCam",pusbcam,"sethflip",&USBCam::SetHflip);
               m_parser.DefineClassFun("USBCam",pusbcam,"setvfilp",&USBCam::SetVfilp);
               m_parser.DefineClassFun("USBCam",pusbcam,"sethueAuto",&USBCam::SetHueAuto);
               m_parser.DefineClassFun("USBCam",pusbcam,"setexposureAuto",&USBCam::SetExposureAuto);
               m_parser.DefineClassFun("USBCam",pusbcam,"setfocusAuto",&USBCam::SetFocusAuto);
               m_parser.DefineClassFun("USBCam",pusbcam,"setgain",&USBCam::SetGain);
               m_parser.DefineClassFun("USBCam",pusbcam,"setexposure",&USBCam::SetExposure);
               m_parser.DefineClassFun("USBCam",pusbcam,"setfocus",&USBCam::SetFocus);

#endif

#endif
#ifdef USE_CAM
            HKCam *pcam = 0;
            m_parser.DefineClass("CamRun",pcam);
            m_parser.DefineClassFun("CamRun",pcam,"enumdevice",&HKCam::EnumDevicesA);
            m_parser.DefineClassFun("CamRun",pcam,"opendevice",&HKCam::OpenDeviceA);
            m_parser.DefineClassFun("CamRun",pcam,"closedevice",&HKCam::CloseDeviceA);

            m_parser.DefineClassFun("CamRun",pcam,"startgarb",&HKCam::startgarb);
            m_parser.DefineClassFun("CamRun",pcam,"softtrig",&HKCam::SoftwareTrg);
            m_parser.DefineClassFun("CamRun",pcam,"softtrig2image",&HKCam::SoftwareTrg2Image);

            m_parser.DefineClassFun("CamRun",pcam,"garbopen",&HKCam::GarbOpen);
            m_parser.DefineClassFun("CamRun",pcam,"garbclose",&HKCam::GarbClose);

            m_parser.DefineClassFun("CamRun",pcam,"camstats",&HKCam::camstats);

            m_parser.DefineClassFun("CamRun",pcam,"garbone",&HKCam::GarbOne);
            m_parser.DefineClassFun("CamRun",pcam,"garbonet",&HKCam::GarbOneT);

            //m_parser.DefineClassFun("CamRun",pcam,"savejpg",&CamRun::SaveImageJPG);
            m_parser.DefineClassFun("CamRun",pcam,"savebmp",&HKCam::SaveBmp);

            //m_parser.DefineClassFun("CamRun",pcam,"garbimage",&HKCam::OnBnClickedGarb);
            m_parser.DefineClassFun("CamRun",pcam,"reversex",&HKCam::ReverseX);
            m_parser.DefineClassFun("CamRun",pcam,"reversey",&HKCam::ReverseY);

            m_parser.DefineClassFun("CamRun",pcam,"resetview",&HKCam::ReSetView);





            m_parser.DefineClassFun("CamRun",pcam,"setexptime",&HKCam::SetExptime);


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



               ImageList *pimagelist = 0;
               m_parser.DefineClass("ImageList",pimagelist);
               m_parser.DefineClassFun("ImageList",pimagelist,"Show",&ImageList::setshow);
               m_parser.DefineClassFun("ImageList",pimagelist,"save",&ImageList::save);
               m_parser.DefineClassFun("ImageList",pimagelist,"load",&ImageList::load);
               m_parser.DefineClassFun("ImageList",pimagelist,"setcurimgenum",&ImageList::setcurimgenum);
               m_parser.DefineClassFun("ImageList",pimagelist,"settoimage",&ImageList::settoimage);
               m_parser.DefineClassFun("ImageList",pimagelist,"clearimage",&ImageList::clearimage);



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

                       m_parser.DefineClassFun("Image",pimage,"setimagepath",&ImageBase::setimagepath);

                       m_parser.DefineClassFun("Image",pimage,"savepath",&ImageBase::SavePath);
                       m_parser.DefineClassFun("Image",pimage,"loadpath",&ImageBase::LoadPath);
                       m_parser.DefineClassFun("Image",pimage,"load",&ImageBase::Load);
                       m_parser.DefineClassFun("Image",pimage,"loadnum",&ImageBase::LoadNum);
                       m_parser.DefineClassFun("Image",pimage,"loadnumjpg",&ImageBase::LoadNumJpg);

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
                       m_parser.DefineClassFun("Image",pimage,"rotate",&ImageBase::ImageCentRotate);
                       m_parser.DefineClassFun("Image",pimage,"imagefont",&ImageBase::ImageFont);
                       m_parser.DefineClassFun("Image",pimage,"shapesetroi",&ImageBase::shapesetroi);
                       m_parser.DefineClassFun("Image",pimage,"imagefontvalue",&ImageBase::ImageFontValue);
                       m_parser.DefineClassFun("Image",pimage,"imagefontset",&ImageBase::ImageFontSet);




                       QShape *pshape = nullptr;
                       m_parser.DefineClass("Shape",pshape);
                       m_parser.DefineClassFun("Shape",pshape,"settype",&QShape::settype);
                       m_parser.DefineClassFun("Shape",pshape,"setname",&QShape::setname);
                       m_parser.DefineClassFun("Shape",pshape,"setrect",&QShape::setrect);
                       m_parser.DefineClassFun("Shape",pshape,"setcolor",&QShape::setcolor);
                       m_parser.DefineClassFun("Shape",pshape,"setfont",&QShape::setfont);



                       ShapeBase *pshapebase = nullptr;
                       m_parser.DefineClass("ShapeBase",pshapebase);
                       m_parser.DefineClassFun("ShapeBase",pshapebase,"settransformed",&ShapeBase::setTransformed);
                       m_parser.DefineClassFun("ShapeBase",pshapebase,"setshape",&ShapeBase::setShape);
                       m_parser.DefineClassFun("ShapeBase",pshapebase,"setantialiased",&ShapeBase::setAntialiased);
                       m_parser.DefineClassFun("ShapeBase",pshapebase,"Show",&ShapeBase::setshow);
                       m_parser.DefineClassFun("ShapeBase",pshapebase,"setpen",&ShapeBase::setPen);
                       m_parser.DefineClassFun("ShapeBase",pshapebase,"setbrush",&ShapeBase::setBrush);
                       m_parser.DefineClassFun("ShapeBase",pshapebase,"setpercent",&ShapeBase::setPercent);
                       m_parser.DefineClassFun("ShapeBase",pshapebase,"setpenw",&ShapeBase::setpenw);


                       PointsShape *apoints = nullptr;
                       m_parser.DefineClass("PointsShape",apoints);
                       m_parser.DefineClassFun("PointsShape",apoints,"Show",&PointsShape::setshow);
                       m_parser.DefineClassFun("PointsShape",apoints,"addpoint",&PointsShape::addapoint);
                       m_parser.DefineClassFun("PointsShape",apoints,"clear",&PointsShape::clear);
                       m_parser.DefineClassFun("PointsShape",apoints,"calibration",&PointsShape::calibration);
                       m_parser.DefineClassFun("PointsShape",apoints,"size",&PointsShape::size);
                       m_parser.DefineClassFun("PointsShape",apoints,"getx",&PointsShape::getx);
                       m_parser.DefineClassFun("PointsShape",apoints,"gety",&PointsShape::gety);



                        LineShape *plineshape = nullptr;
                        m_parser.DefineClass("LineShape",plineshape);
                        m_parser.DefineClassFun("LineShape",plineshape,"setline",&LineShape::setline);
                        m_parser.DefineClassFun("LineShape",plineshape,"Show",&LineShape::setshow);
                        m_parser.DefineClassFun("LineShape",plineshape,"move",&LineShape::Move);
                        m_parser.DefineClassFun("LineShape",plineshape,"rotate",&LineShape::Rotate);
                        m_parser.DefineClassFun("LineShape",plineshape,"zoom",&LineShape::Zoom);
                        m_parser.DefineClassFun("LineShape",plineshape,"setpenw",&LineShape::setpenw);

                        findline *pfindline = nullptr;
                        m_parser.DefineClass("findline",pfindline);
                        m_parser.DefineClassFun("findline",pfindline,"setrect",&findline::setrect);
                        m_parser.DefineClassFun("findline",pfindline,"Show",&findline::setshow);
                        m_parser.DefineClassFun("findline",pfindline,"clear",&findline::clear);
                        m_parser.DefineClassFun("findline",pfindline,"setwhgap",&findline::SetWHgap);
                        m_parser.DefineClassFun("findline",pfindline,"measure",&findline::measure);

                        m_parser.DefineClassFun("findline",pfindline,"setlinesample",&findline::setlinesamplerate);
                        m_parser.DefineClassFun("findline",pfindline,"setlinegap",&findline::setlinegap);
                        m_parser.DefineClassFun("findline",pfindline,"setmethod",&findline::setmethod);
                        m_parser.DefineClassFun("findline",pfindline,"setthre",&findline::setthre);
                        m_parser.DefineClassFun("findline",pfindline,"setgama",&findline::setgamarate);
                        m_parser.DefineClassFun("findline",pfindline,"setfindsetting",&findline::setfindsetting);
                        m_parser.DefineClassFun("findline",pfindline,"setfilter",&findline::setfilter);
        m_parser.DefineClassFun("findline",pfindline,"findpattern",&findline::findpattern);
        m_parser.DefineClassFun("findline",pfindline,"setcompgap",&findline::setcomparegap);
        m_parser.DefineClassFun("findline",pfindline,"shapesetroi",&findline::shapesetroi);

                        ImageROI * pimageroi= nullptr;
                        m_parser.DefineClass("Imageroi",pimageroi);
                        m_parser.DefineClassFun("Imageroi",pimageroi,"getimage",&ImageROI::getimage);
                        m_parser.DefineClassFun("Imageroi",pimageroi,"setrect",&ImageROI::setrect);
                        m_parser.DefineClassFun("Imageroi",pimageroi,"setrelmatch",&ImageROI::setrelationrectfrom_matchresult);
                        m_parser.DefineClassFun("Imageroi",pimageroi,"setrelzoom",&ImageROI::setrelationzoom);
                        m_parser.DefineClassFun("Imageroi",pimageroi,"setrelresultnum",&ImageROI::setrelationrectfromresultnum);
                        m_parser.DefineClassFun("Imageroi",pimageroi,"setrelxy",&ImageROI::setrelationxy);
                        m_parser.DefineClassFun("Imageroi",pimageroi,"setrelpart",&ImageROI::setrelationpart);
                        m_parser.DefineClassFun("Imageroi",pimageroi,"setreltorect",&ImageROI::setrelationtorect);
                        m_parser.DefineClassFun("Imageroi",pimageroi,"setsplitrect",&ImageROI::setsplitrect);
                        m_parser.DefineClassFun("Imageroi",pimageroi,"selectrect",&ImageROI::selectrect);

                        m_parser.DefineClassFun("Imageroi",pimageroi,"findrect",&ImageROI::findrect);
                        m_parser.DefineClassFun("Imageroi",pimageroi,"roitable",&ImageROI::ColorTable);
                        m_parser.DefineClassFun("Imageroi",pimageroi,"tablevalue",&ImageROI::tablevalue);
                        m_parser.DefineClassFun("Imageroi",pimageroi,"tableblur",&ImageROI::ColorTableBlur);
                        m_parser.DefineClassFun("Imageroi",pimageroi,"roieasythre",&ImageROI::roieasythre);


                        m_parser.DefineClassFun("Imageroi",pimageroi,"roiedgew",&ImageROI::roiedge);
                        m_parser.DefineClassFun("Imageroi",pimageroi,"roiedgeh",&ImageROI::roiedgeh);
                        m_parser.DefineClassFun("Imageroi",pimageroi,"roiedge7w",&ImageROI::roiedge7);
                        m_parser.DefineClassFun("Imageroi",pimageroi,"roiedge7h",&ImageROI::roiedgeh7);
                        m_parser.DefineClassFun("Imageroi",pimageroi,"shapesetroi",&ImageROI::shapesetroi);



                        findobject *pfindobject = nullptr;
                        m_parser.DefineClass("findobject",pfindobject);
                        m_parser.DefineClassFun("findobject",pfindobject,"setrect",&findobject::setrect);
                        m_parser.DefineClassFun("findobject",pfindobject,"measure",&findobject::measure);
                        m_parser.DefineClassFun("findobject",pfindobject,"Show",&findobject::setshow);
                        m_parser.DefineClassFun("findobject",pfindobject,"measurex",&findobject::measurex);
                        m_parser.DefineClassFun("findobject",pfindobject,"sethsogap",&findobject::sethsogap);
                        m_parser.DefineClassFun("findobject",pfindobject,"setminmax",&findobject::setminmaxarea);
                        m_parser.DefineClassFun("findobject",pfindobject,"setminmaxwh",&findobject::setminmaxwh);
                        m_parser.DefineClassFun("findobject",pfindobject,"setbrow",&findobject::setbrow);
                        m_parser.DefineClassFun("findobject",pfindobject,"setdistance",&findobject::setdistance);
                        m_parser.DefineClassFun("findobject",pfindobject,"setsearchtype",&findobject::setsearchtype);
                        m_parser.DefineClassFun("findobject",pfindobject,"edgeimage",&findobject::edgeimage);
                        m_parser.DefineClassFun("findobject",pfindobject,"setedgeoi",&findobject::setedgeoi);
                        m_parser.DefineClassFun("findobject",pfindobject,"setoffset",&findobject::setoffset);


                        m_parser.DefineClassFun("findobject",pfindobject,"getresultcentx",&findobject::getresultcentx);
                        m_parser.DefineClassFun("findobject",pfindobject,"getresultcenty",&findobject::getresultcenty);
                        m_parser.DefineClassFun("findobject",pfindobject,"getresultx",&findobject::getresultx);
                        m_parser.DefineClassFun("findobject",pfindobject,"getresulty",&findobject::getresulty);
                        m_parser.DefineClassFun("findobject",pfindobject,"getresultw",&findobject::getresultw);
                        m_parser.DefineClassFun("findobject",pfindobject,"getresulth",&findobject::getresulth);
                        m_parser.DefineClassFun("findobject",pfindobject,"getresultsize",&findobject::getresultsize);
                        m_parser.DefineClassFun("findobject",pfindobject,"getresultobjsnum",&findobject::getresultobjsnum);
                        m_parser.DefineClassFun("findobject",pfindobject,"objectgrid",&findobject::objectgrid);
                        m_parser.DefineClassFun("findobject",pfindobject,"setobjectgrid",&findobject::setobjectgrid);
                        m_parser.DefineClassFun("findobject",pfindobject,"objectsort",&findobject::objectsort);
                        m_parser.DefineClassFun("findobject",pfindobject,"edge",&findobject::Edge);


                        m_parser.DefineClassFun("findobject",pfindobject,"setrelresultnum",&findobject::setrelationrectfromresultnum);
                        m_parser.DefineClassFun("findobject",pfindobject,"setrelmatch",&findobject::setrelationrectfrom_matchresult);
                        m_parser.DefineClassFun("findobject",pfindobject,"setrelxy",&findobject::setrelationxy);
                        m_parser.DefineClassFun("findobject",pfindobject,"setrelzoom",&findobject::setrelationzoom);
                        m_parser.DefineClassFun("findobject",pfindobject,"setreltorect",&findobject::setrelationtorect);

                        m_parser.DefineClassFun("findobject",pfindobject,"setcolor",&findobject::setcolorstyle);
                        m_parser.DefineClassFun("findobject",pfindobject,"setroithre",&findobject::SetImageROIthre);
                        m_parser.DefineClassFun("findobject",pfindobject,"setroiincrease",&findobject::SetImageROIincrease);
                        m_parser.DefineClassFun("findobject",pfindobject,"setroicomparegap",&findobject::SetImageROIcomparegap);
                        m_parser.DefineClassFun("findobject",pfindobject,"setroifindborw",&findobject::SetImageROIfindBorW);
                        m_parser.DefineClassFun("findobject",pfindobject,"setroiedge5o7",&findobject::SetImageROIedge_5o7);
                        m_parser.DefineClassFun("findobject",pfindobject,"roithre",&findobject::ImageROIthre);
                        m_parser.DefineClassFun("findobject",pfindobject,"roiedge",&findobject::ImageROIedge);
                        m_parser.DefineClassFun("findobject",pfindobject,"roiedgeh",&findobject::ImageROIedgeH);

                        m_parser.DefineClassFun("findobject",pfindobject,"shapesetroi",&findobject::shapesetroi);






        QGrid *pgrid = nullptr;
        m_parser.DefineClass("grid",pgrid);
        m_parser.DefineClassFun("grid",pgrid,"setgrid",&QGrid::setgrid);
        m_parser.DefineClassFun("grid",pgrid,"Show",&QGrid::setshow);
        m_parser.DefineClassFun("grid",pgrid,"settype",&QGrid::settype);
        m_parser.DefineClassFun("grid",pgrid,"settypevalue",&QGrid::settypevalue);
        m_parser.DefineClassFun("grid",pgrid,"clear",&QGrid::clear);
        m_parser.DefineClassFun("grid",pgrid,"setbrush",&QGrid::setbrush);
        m_parser.DefineClassFun("grid",pgrid,"modeltogrid",&QGrid::modeltogrid);
        m_parser.DefineClassFun("grid",pgrid,"loadmodel",&QGrid::loadmodelfile);
        m_parser.DefineClassFun("grid",pgrid,"savemodel",&QGrid::savemodelfile);

        m_parser.DefineClassFun("grid",pgrid,"savemap",&QGrid::savemapmodel);
        m_parser.DefineClassFun("grid",pgrid,"loadmap",&QGrid::loadmapmodel);
        m_parser.DefineClassFun("grid",pgrid,"roiimagetomap",&QGrid::roiimagetomodel);

        m_parser.DefineClassFun("grid",pgrid,"unit",&QGrid::UnitGrid);
        m_parser.DefineClassFun("grid",pgrid,"method",&QGrid::ModelGridMethod_Gauss);
        m_parser.DefineClassFun("grid",pgrid,"methodobject",&QGrid::ModelGridMethod_Object);


        m_parser.DefineClassFun("grid",pgrid,"zeromodel",&QGrid::ZeroModel);
        m_parser.DefineClassFun("grid",pgrid,"setunit",&QGrid::SetUnit);
        m_parser.DefineClassFun("grid",pgrid,"centgrid",&QGrid::CentGrid);
        m_parser.DefineClassFun("grid",pgrid,"gridzoom",&QGrid::GridZoom);
        m_parser.DefineClassFun("grid",pgrid,"grid2patmodel",&QGrid::Grid2PattenModel);




        gridobject *pgridobject = nullptr;
        m_parser.DefineClass("gridobject",pgridobject);

        m_parser.DefineClassFun("gridobject",pgridobject,"setrect",&gridobject::setrect);
        m_parser.DefineClassFun("gridobject",pgridobject,"Show",&gridobject::setshow);
        m_parser.DefineClassFun("gridobject",pgridobject,"gridmeasure",&gridobject::gridmeasure_l0);
        m_parser.DefineClassFun("gridobject",pgridobject,"sethsogap",&gridobject::sethsogap);
        m_parser.DefineClassFun("gridobject",pgridobject,"setminmax",&gridobject::setminmaxarea);
        m_parser.DefineClassFun("gridobject",pgridobject,"setminmaxwh",&gridobject::setminmaxwh);
        m_parser.DefineClassFun("gridobject",pgridobject,"setbrow",&gridobject::setbrow);
        m_parser.DefineClassFun("gridobject",pgridobject,"setdistance",&gridobject::setdistance);
        m_parser.DefineClassFun("gridobject",pgridobject,"setsearchtype",&gridobject::setsearchtype);
        m_parser.DefineClassFun("gridobject",pgridobject,"clear",&gridobject::Clear);

        m_parser.DefineClassFun("gridobject",pgridobject,"getresultcentx",&gridobject::getresultcentx);
        m_parser.DefineClassFun("gridobject",pgridobject,"getresultcenty",&gridobject::getresultcenty);
        m_parser.DefineClassFun("gridobject",pgridobject,"getresultx",&gridobject::getresultx);
        m_parser.DefineClassFun("gridobject",pgridobject,"getresulty",&gridobject::getresulty);
        m_parser.DefineClassFun("gridobject",pgridobject,"getresultw",&gridobject::getresultw);
        m_parser.DefineClassFun("gridobject",pgridobject,"getresulth",&gridobject::getresulth);
        m_parser.DefineClassFun("gridobject",pgridobject,"getresultsize",&gridobject::getresultsize);
        m_parser.DefineClassFun("gridobject",pgridobject,"getresultobjsnum",&gridobject::getresultobjsnum);
        m_parser.DefineClassFun("gridobject",pgridobject,"setshowspec",&gridobject::setshowspec);



        fastmatch *pfastmatch = nullptr;
        m_parser.DefineClass("fastmatch",pfastmatch);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"setrect",&fastmatch::setrect);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"Show",&fastmatch::setshow);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"learn",&fastmatch::learn);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"setcompgap",&fastmatch::setcomparegap);

        //

        m_parser.DefineClassFun("fastmatch",pfastmatch,"setwhgap",&fastmatch::SetWHgap);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"measure",&fastmatch::measure);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"setlinesample",&fastmatch::setlinesamplerate);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"setlinegap",&fastmatch::setlinegap);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"setmethod",&fastmatch::setmethod);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"setthre",&fastmatch::setthre);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"setfindsetting",&fastmatch::setfindsetting);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"setfilter",&fastmatch::setfilter);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"savemodel",&fastmatch::savemodelfile);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"loadmodel",&fastmatch::loadmodelfile);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"modelrotate",&fastmatch::modelrotate);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"modelzoom",&fastmatch::modelzoom);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"modelzeroposition",&fastmatch::modelzeroposition);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"match",&fastmatch::match);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"setmatchrect",&fastmatch::setmatchrect);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"setminscore",&fastmatch::setminscore);

        m_parser.DefineClassFun("fastmatch",pfastmatch,"patternrootgrid",&fastmatch::patternrootgrid);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"patternzoom",&fastmatch::patternzoom);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"modeltranform",&fastmatch::patterntranform);

        m_parser.DefineClassFun("fastmatch",pfastmatch,"rotatematch",&fastmatch::rotatematch);

        m_parser.DefineClassFun("fastmatch",pfastmatch,"loadrotatemodel",&fastmatch::loadrotatemodelfile);

        m_parser.DefineClassFun("fastmatch",pfastmatch,"setrotateangle",&fastmatch::setrotateangle);


        m_parser.DefineClassFun("fastmatch",pfastmatch,"clearmodels",&fastmatch::clearmodels_l12);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"addmodels",&fastmatch::addmodels_l12);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"modelstocurrent",&fastmatch::modelstocurrent_l12);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"multimatch",&fastmatch::multimatch);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"setmultimatchrect",&fastmatch::setmultimatchrect);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"setmatchrectnum",&fastmatch::setmatchrectnum);

        m_parser.DefineClassFun("fastmatch",pfastmatch,"imagelearn",&fastmatch::imagelearn);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"imagematch",&fastmatch::imagematch);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"imagemodelcompareshow",&fastmatch::imagemodelcompareshow);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"savematchroi",&fastmatch::savematchroi);

        m_parser.DefineClassFun("fastmatch",pfastmatch,"loadmapmodel",&fastmatch::loadfastimagemodel);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"savemapmodel",&fastmatch::savefastimagemodel);


        m_parser.DefineClassFun("fastmatch",pfastmatch,"imagemodesclear",&fastmatch::imagemodesclear_l12);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"addimagemodels",&fastmatch::addimagemodels_l12);


        m_parser.DefineClassFun("fastmatch",pfastmatch,"getimagemodelreslut",&fastmatch::getimagemodelreslut);

        m_parser.DefineClassFun("fastmatch",pfastmatch,"setclustergap",&fastmatch::setclustergap);


        m_parser.DefineClassFun("fastmatch",pfastmatch,"saveimagemodel",&fastmatch::savematchimagemodel);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"setmatchthre",&fastmatch::setmatchthre);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"setfindnum",&fastmatch::setfindnum);

        m_parser.DefineClassFun("fastmatch",pfastmatch,"getresultnum",&fastmatch::getresultnum);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"getresultcentx",&fastmatch::getresultcentx);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"getresultcenty",&fastmatch::getresultcenty);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"getmaxresult",&fastmatch::getmaxresult);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"setspecshow",&fastmatch::setspecshow);

        m_parser.DefineClassFun("fastmatch",pfastmatch,"setrelresultnum",&fastmatch::setrelationrectfromresultnum);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"setrelmatch",&fastmatch::setrelationrectfrom_matchresult);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"setrelxy",&fastmatch::setrelationxy);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"setrelzoom",&fastmatch::setrelationzoom);
        m_parser.DefineClassFun("fastmatch",pfastmatch,"setreltorect",&fastmatch::setrelationtorect);

        m_parser.DefineClassFun("fastmatch",pfastmatch,"setcolor",&fastmatch::setcolorstyle);

        m_parser.DefineClassFun("fastmatch",pfastmatch,"shapesetroi",&fastmatch::shapesetroi);

        QEasyORC *aporc =nullptr;
        m_parser.DefineClass("easyorc",aporc);
        m_parser.DefineClassFun("easyorc",aporc,"setorcareasnum",&QEasyORC::setorcareasnum);
        m_parser.DefineClassFun("easyorc",aporc,"setorcareas",&QEasyORC::setorcareas);
        m_parser.DefineClassFun("easyorc",aporc,"setorcthre",&QEasyORC::setorcthre);
        m_parser.DefineClassFun("easyorc",aporc,"setrect",&QEasyORC::setrect);
        m_parser.DefineClassFun("easyorc",aporc,"Show",&QEasyORC::setshow);
        m_parser.DefineClassFun("easyorc",aporc,"setspecshow",&QEasyORC::setspecshow);
        m_parser.DefineClassFun("easyorc",aporc,"areasorc",&QEasyORC::areasorc);
        m_parser.DefineClassFun("easyorc",aporc,"setdebug",&QEasyORC::setdebug);
        m_parser.DefineClassFun("easyorc",aporc,"stringsplit",&QEasyORC::stringsplit);
        m_parser.DefineClassFun("easyorc",aporc,"fontsplit",&QEasyORC::fontsplit);
        m_parser.DefineClassFun("easyorc",aporc,"exfontsplit",&QEasyORC::exfontsplit);
        m_parser.DefineClassFun("easyorc",aporc,"modelmethod",&QEasyORC::modelmethod);

        m_parser.DefineClassFun("easyorc",aporc,"match72_matchimg",&QEasyORC::match72_matchimg);
        m_parser.DefineClassFun("easyorc",aporc,"match72_matchpat",&QEasyORC::match72_matchpat);


        m_parser.DefineClassFun("easyorc",aporc,"setsplitimage",&QEasyORC::setsplitimage);
        m_parser.DefineClassFun("easyorc",aporc,"setsplitgrid",&QEasyORC::setsplitgrid);

        m_parser.DefineClassFun("easyorc",aporc,"setsplitobject",&QEasyORC::setsplitobject);
        m_parser.DefineClassFun("easyorc",aporc,"setsplitobjectbg",&QEasyORC::setsplitobjectbg);
        m_parser.DefineClassFun("easyorc",aporc,"setsplitobjectoffset",&QEasyORC::setsplitobjectoffset);

        m_parser.DefineClassFun("easyorc",aporc,"setb2w",&QEasyORC::setb2w);

        m_parser.DefineClassFun("easyorc",aporc,"fontorc",&QEasyORC::fontorc);
        m_parser.DefineClassFun("easyorc",aporc,"levelmodel",&QEasyORC::levelmodel);
        m_parser.DefineClassFun("easyorc",aporc,"selectmodel",&QEasyORC::SelectModel);

        m_parser.DefineClassFun("easyorc",aporc,"selectmodel72",&QEasyORC::selectmodel72);

        m_parser.DefineClassFun("easyorc",aporc,"autolearn",&QEasyORC::autolearn);
        m_parser.DefineClassFun("easyorc",aporc,"autolearnex",&QEasyORC::autolearnex);
        m_parser.DefineClassFun("easyorc",aporc,"autolearnmass",&QEasyORC::autolearnmass);
        m_parser.DefineClassFun("easyorc",aporc,"checklearn",&QEasyORC::checklearn);
        m_parser.DefineClassFun("easyorc",aporc,"checkmatch",&QEasyORC::checkmatch);
        m_parser.DefineClassFun("easyorc",aporc,"match72",&QEasyORC::match72);


        m_parser.DefineClassFun("easyorc",aporc,"learnmass_36",&QEasyORC::learnmass_36);

        m_parser.DefineClassFun("easyorc",aporc,"string_exnum",&QEasyORC::string_exnum);
        m_parser.DefineClassFun("easyorc",aporc,"string_autolearnmass",&QEasyORC::string_autolearnmass);

        m_parser.DefineClassFun("easyorc",aporc,"setlearngridwh",&QEasyORC::setlearngridwh);

        m_parser.DefineClassFun("easyorc",aporc,"setgrid",&QEasyORC::setgrid);

        m_parser.DefineClassFun("easyorc",aporc,"autolearnobj",&QEasyORC::autolearnobj);

        m_parser.DefineClassFun("easyorc",aporc,"setmatchvalid",&QEasyORC::setmatchvalid);
        m_parser.DefineClassFun("easyorc",aporc,"setusingobject",&QEasyORC::setusingobject);


        m_parser.DefineClassFun("easyorc",aporc,"modelshow",&QEasyORC::imagemodelshow);
        m_parser.DefineClassFun("easyorc",aporc,"matchshow",&QEasyORC::imagematchshow);
        m_parser.DefineClassFun("easyorc",aporc,"compareshow",&QEasyORC::imagecompareshow);

        m_parser.DefineClassFun("easyorc",aporc,"setshowpos",&QEasyORC::setshowpos);
        m_parser.DefineClassFun("easyorc",aporc,"mapgrid",&QEasyORC::mapgrid);
        m_parser.DefineClassFun("easyorc",aporc,"setshowmap",&QEasyORC::setshowmap);
        m_parser.DefineClassFun("easyorc",aporc,"fontorc_level",&QEasyORC::fontorc_level);
        m_parser.DefineClassFun("easyorc",aporc,"savelevelmodel",&QEasyORC::savelevelmodel);
        m_parser.DefineClassFun("easyorc",aporc,"fontorc_levelex",&QEasyORC::fontorc_levelex);
        m_parser.DefineClassFun("easyorc",aporc,"fontorc_level2",&QEasyORC::fontorc_level2);
        m_parser.DefineClassFun("easyorc",aporc,"checkorc_level3",&QEasyORC::checkorc_level3);

        m_parser.DefineClassFun("easyorc",aporc,"setimagetype",&QEasyORC::setimagetype);

        m_parser.DefineClassFun("easyorc",aporc,"fontorc_levelnode",&QEasyORC::fontorc_levelnode);
        m_parser.DefineClassFun("easyorc",aporc,"shownoderesult",&QEasyORC::shownoderesult);
        m_parser.DefineClassFun("easyorc",aporc,"shownoderesultex",&QEasyORC::shownoderesultex);

        m_parser.DefineClassFun("easyorc",aporc,"shownodelistresult12x12",&QEasyORC::shownodelistresult12x12);
        m_parser.DefineClassFun("easyorc",aporc,"clipboardresult",&QEasyORC::clipboardresult);
        m_parser.DefineClassFun("easyorc",aporc,"setminscore",&QEasyORC::setminscore);
        m_parser.DefineClassFun("easyorc",aporc,"stringresulthead",&QEasyORC::stringresulthead);
        m_parser.DefineClassFun("easyorc",aporc,"stringresulttail",&QEasyORC::stringresulttail);
        m_parser.DefineClassFun("easyorc",aporc,"fontorc_levelnodelist",&QEasyORC::fontorc_levelnodelist);
        m_parser.DefineClassFun("easyorc",aporc,"shapesetroi",&QEasyORC::shapesetroi);

        imagecodeparser *acode=nullptr;
        m_parser.DefineClass("imagecodeparser",acode);
        m_parser.DefineClassFun("imagecodeparser",acode,"setrect",&imagecodeparser::setrect);
        m_parser.DefineClassFun("imagecodeparser",acode,"Show",&imagecodeparser::setshow);
        m_parser.DefineClassFun("imagecodeparser",acode,"measure",&imagecodeparser::measure);
        m_parser.DefineClassFun("imagecodeparser",acode,"measure0",&imagecodeparser::measure0);

        m_parser.DefineClassFun("imagecodeparser",acode,"setrotate",&imagecodeparser::setrotate);

        m_parser.DefineClassFun("imagecodeparser",acode,"setrelresultnum",&imagecodeparser::setrelationrectfromresultnum);
        m_parser.DefineClassFun("imagecodeparser",acode,"setrelmatch",&imagecodeparser::setrelationrectfrom_matchresult);
        m_parser.DefineClassFun("imagecodeparser",acode,"setrelxy",&imagecodeparser::setrelationxy);
        m_parser.DefineClassFun("imagecodeparser",acode,"setrelzoom",&imagecodeparser::setrelationzoom);
        m_parser.DefineClassFun("imagecodeparser",acode,"setreltorect",&imagecodeparser::setrelationtorect);
        m_parser.DefineClassFun("imagecodeparser",acode,"findcode",&imagecodeparser::findcode);
        m_parser.DefineClassFun("imagecodeparser",acode,"findcodex",&imagecodeparser::findcodex);
        m_parser.DefineClassFun("imagecodeparser",acode,"coderule",&imagecodeparser::coderule);

        m_parser.DefineClassFun("imagecodeparser",acode,"setsmooth",&imagecodeparser::setsmoothtransformation);
        m_parser.DefineClassFun("imagecodeparser",acode,"setdecoder",&imagecodeparser::setdecoder);
        m_parser.DefineClassFun("imagecodeparser",acode,"shapesetroi",&imagecodeparser::shapesetroi);

        SmartDouble *avect=nullptr;
        m_parser.DefineClass("vector",avect);
        m_parser.DefineClassFun("vector",avect,"push",&SmartDouble::push);
        m_parser.DefineClassFun("vector",avect,"get",&SmartDouble::getvalue);
        m_parser.DefineClassFun("vector",avect,"get",&SmartDouble::getresult);

        m_parser.DefineClassFun("vector",avect,"set",&SmartDouble::set);
        m_parser.DefineClassFun("vector",avect,"clear",&SmartDouble::clear);
        m_parser.DefineClassFun("vector",avect,"size",&SmartDouble::size);
        m_parser.DefineClassFun("vector",avect,"average",&SmartDouble::average);
        m_parser.DefineClassFun("vector",avect,"maxvalue",&SmartDouble::maxvalue);
        m_parser.DefineClassFun("vector",avect,"minvalue",&SmartDouble::minvalue);
        m_parser.DefineClassFun("vector",avect,"maxnum",&SmartDouble::maxnum);
        m_parser.DefineClassFun("vector",avect,"save",&SmartDouble::save);
        m_parser.DefineClassFun("vector",avect,"load",&SmartDouble::load);


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
                double *adouble=0;
                for(int i=0;i<pclass->size();i++)
                {
                    adouble=(double *)pclass->getvarpoint(i);
                    if(adouble)//if(_finite(*adouble))
                    {
                        *m_createstream << pclass->getvar(i) <<"="<<*adouble<<";\r\n";
                    }
                }
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
            else if("findline"==item->first)
            {
                findline *ashape=0;
                for(int i=0;i<pclass->size();i++)
                {
                    ashape=(findline *)pclass->getvarpoint(i);
                    if(ashape)
                    {
                        *m_createstream << pclass->getvar(i) \
                                        <<".setrect("\
                                        <<ashape->rect().x()<<"," \
                                        <<ashape->rect().y()<<"," \
                                        <<ashape->rect().width()<<"," \
                                        <<ashape->rect().height()<<")" \
                                        <<";\r\n";
                    }
                }
            }
            else if("findobject"==item->first)
            {
                findobject *ashape=0;
                for(int i=0;i<pclass->size();i++)
                {
                    ashape=(findobject *)pclass->getvarpoint(i);
                    if(ashape)
                    {
                        *m_createstream << pclass->getvar(i) \
                                        <<".setrect(" \
                                        <<ashape->rect().x()<<"," \
                                        <<ashape->rect().y()<<"," \
                                        <<ashape->rect().width()<<"," \
                                        <<ashape->rect().height()<<")" \
                                        <<";\r\n";
                    }
                }
            }
            else if("gridobject"==item->first)
            {
                gridobject *ashape=0;
                for(int i=0;i<pclass->size();i++)
                {
                    ashape=(gridobject *)pclass->getvarpoint(i);
                    if(ashape)
                    {
                        *m_createstream << pclass->getvar(i) \
                                        <<".setrect(" \
                                        <<ashape->rect().x()<<"," \
                                        <<ashape->rect().y()<<"," \
                                        <<ashape->rect().width()<<"," \
                                        <<ashape->rect().height()<<")" \
                                        <<";\r\n";
                    }
                }
            }
            else if("fastmatch"==item->first)
            {
                fastmatch *ashape=0;
                for(int i=0;i<pclass->size();i++)
                {
                    ashape=(fastmatch *)pclass->getvarpoint(i);
                    if(ashape)
                    {
                        *m_createstream << pclass->getvar(i) \
                                        <<".setrect(" \
                                        <<ashape->rect().x()<<"," \
                                        <<ashape->rect().y()<<"," \
                                        <<ashape->rect().width()<<"," \
                                        <<ashape->rect().height()<<")" \
                                        <<";\r\n";
                    }
                }

            }
            else if("Imageroi"==item->first)
            {
                ImageROI *ashape=0;
                for(int i=0;i<pclass->size();i++)
                {
                    ashape=(ImageROI *)pclass->getvarpoint(i);
                    if(ashape)
                    {
                        *m_createstream << pclass->getvar(i) \
                                        <<".setrect(" \
                                        <<ashape->rect().x()<<"," \
                                        <<ashape->rect().y()<<"," \
                                        <<ashape->rect().width()<<"," \
                                        <<ashape->rect().height()<<")" \
                                        <<";\r\n";
                    }
                }

            }
            else if("easyorc"==item->first)
            {
                QEasyORC *ashape=0;
                for(int i=0;i<pclass->size();i++)
                {
                    ashape=(QEasyORC *)pclass->getvarpoint(i);
                    if(ashape)
                    {
                        *m_createstream << pclass->getvar(i) \
                                        <<".setrect(" \
                                        <<ashape->rect().x()<<"," \
                                        <<ashape->rect().y()<<"," \
                                        <<ashape->rect().width()<<"," \
                                        <<ashape->rect().height()<<")" \
                                        <<";\r\n";
                    }
                }

            }
            else if("imagecodeparser"==item->first)
            {
                imagecodeparser *ashape=0;
                for(int i=0;i<pclass->size();i++)
                {
                    ashape=(imagecodeparser *)pclass->getvarpoint(i);
                    if(ashape)
                    {
                        *m_createstream << pclass->getvar(i) \
                                        <<".setrect(" \
                                        <<ashape->rect().x()<<"," \
                                        <<ashape->rect().y()<<"," \
                                        <<ashape->rect().width()<<"," \
                                        <<ashape->rect().height()<<")" \
                                        <<";\r\n";
                    }
                }

            }

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
