#include "hk/qt_hik.hpp"

#include"cximage/imagebase.h"
#include <string>
#include <qwidget>
#define IMAGE_NAME_LEN 64


using namespace std;
HKCam::HKCam()
{
    //一定要初始化， 私有变量未初始化不是空指针。
    //不初始化导致莫名其妙的bug
    //连判断都进不去

    //指针一定要初始化
    m_pBufForDriver = nullptr;
    m_pBufForSaveImage  = nullptr;
    m_pcMyCamera = nullptr;
    m_hwndDisplay = nullptr;

    m_nBufSizeForDriver = 0;
    m_bOpenDevice =FALSE;
    m_bStartGrabbing = FALSE;
    m_nTriggerMode = 0;
    m_nBufSizeForSaveImage = 0;

   // QVBoxLayout *hLayout = new QVBoxLayout(widget);
   // hLayout->addWidget(container, 1);
   // hLayout->addWidget(genNewButton, 1);
   // hLayout->addWidget(changeSizeSlider, 1);
   // hLayout->addWidget(sizeLabel);

    // Show window
    m_pwidget = new QWidget();

    // Show window
    m_pwidget->hide();

    m_pwidget->setGeometry(1600, 1200,1600, 1200);
    m_pwidget->setWindowTitle(QStringLiteral("QPaintedPic"));

}
HKCam::~HKCam()
{
    //回收的好习惯
    delete m_pBufForDriver;
    delete m_pBufForSaveImage;
    delete m_pcMyCamera;
    delete m_pwidget;
}
void HKCam::EnumDevicesA()
{
    // ch:设备信息列表结构体变量，用来存储设备列表
    // ch:初始化设备信息列表
    memset(&this->m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
    int nRet = CMyCamera::EnumDevices(&this->m_stDevList);
    if(nRet != MV_OK)
    { 
        return  ;
    }
    for(size_t i = 0; i < this->m_stDevList.nDeviceNum; i++)
    {
        //设备信息
        MV_CC_DEVICE_INFO *pDeviceInfo = this->m_stDevList.pDeviceInfo[i];
        if(pDeviceInfo == nullptr)
        {
            continue;
        }
        unsigned char *pUserName = pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber;
        string pUserName2 = reinterpret_cast<char *>(pUserName);
        //QUserNames.push_back( QString::fromStdString(pUserName2) );
    }
    if(this->m_stDevList.nDeviceNum == 0)
    {
        //throw "No device" ;
        return  ;
    }
    return  ;
}
int HKCam::EnumDevices(vector< QString> &QUserNames)
{
    // ch:设备信息列表结构体变量，用来存储设备列表
    // ch:初始化设备信息列表
    memset(&this->m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
    int nRet = CMyCamera::EnumDevices(&this->m_stDevList);
    if(nRet != MV_OK)
    {
        return -1;
    }
    for(size_t i = 0; i < this->m_stDevList.nDeviceNum; i++)
    {
        //设备信息
        MV_CC_DEVICE_INFO *pDeviceInfo = this->m_stDevList.pDeviceInfo[i];
        if(pDeviceInfo == nullptr)
        {
            continue;
        }
        unsigned char *pUserName = pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber;
        string pUserName2 = reinterpret_cast<char *>(pUserName);
        QUserNames.push_back( QString::fromStdString(pUserName2) );
    }
    if(this->m_stDevList.nDeviceNum == 0)
    {
        //throw "No device" ;
        return -1;
    }
    return MV_OK;
}
int HKCam::OpenDevice(int &nIndex)
{
    if(m_bOpenDevice == TRUE)
        return -1;

    if( (nIndex < 0) || (nIndex >= MV_MAX_DEVICE_NUM))
    {
        //throw "Please select device ";
        return -1;
    }
    if(this->m_stDevList.pDeviceInfo[nIndex] == nullptr)
    {
        //throw "Open Device Failed";
        return -1;
    }

    this->m_pcMyCamera = new CMyCamera;
    if(this->m_pcMyCamera == nullptr)
    {
        //throw "Initailized Failed";
        return -1;
    }
    int nRet = m_pcMyCamera->Open(this->m_stDevList.pDeviceInfo[nIndex]);
    if(nRet != MV_OK)
    {
        delete this->m_pcMyCamera;
        this->m_pcMyCamera = nullptr;
        return nRet;
    }
    m_bOpenDevice = TRUE;
    return MV_OK;
}
void HKCam::OpenDeviceA(int nIndex)
{
    if(m_bOpenDevice == TRUE)
        return ;

    if( (nIndex < 0) || (nIndex >= MV_MAX_DEVICE_NUM))
    {
        //throw "Please select device ";
        return ;
    }
    if(this->m_stDevList.pDeviceInfo[nIndex] == nullptr)
    {
        //throw "Open Device Failed";
        return ;
    }

    this->m_pcMyCamera = new CMyCamera;
    if(this->m_pcMyCamera == nullptr)
    {
        //throw "Initailized Failed";
        return ;
    }
    int nRet = m_pcMyCamera->Open(this->m_stDevList.pDeviceInfo[nIndex]);
    if(nRet != MV_OK)
    {
        delete this->m_pcMyCamera;
        this->m_pcMyCamera = nullptr;
        return ;
    }
    m_bOpenDevice = TRUE;
    return ;
}

int HKCam::CloseDevice()
{
    if(m_pcMyCamera != nullptr)
    {
        m_pcMyCamera->Close();
        delete m_pcMyCamera;
        m_pcMyCamera = nullptr;
    }
    m_bOpenDevice = FALSE;
    m_bStartGrabbing = FALSE;

    return MV_OK;
}

void HKCam::CloseDeviceA()
{
    if(m_pcMyCamera != nullptr)
    {
        m_pcMyCamera->Close();
        delete m_pcMyCamera;
        m_pcMyCamera = nullptr;
    }
    m_bOpenDevice = FALSE;
    m_bStartGrabbing = FALSE;

    return ;
}

int HKCam::SetTriggerMode(int trigger_mode)
{
    if(trigger_mode == 0)
        this->m_nTriggerMode = MV_TRIGGER_MODE_OFF;
    else {
        this->m_nTriggerMode = MV_TRIGGER_MODE_ON;
    }
    int nRet = m_pcMyCamera->SetEnumValue("TriggerMode", this->m_nTriggerMode);
    if(nRet != MV_OK)
        return nRet;
    return MV_OK;
}

void HKCam::SoftwareTrg()
{

   // int nRet = m_pcMyCamera->CommandExecute("TriggerSoftware");
    //if(nRet == MV_OK)
    if(m_pcMyCamera != nullptr)
    {
        HWND MainWndID = (HWND)m_pwidget->winId();
       m_pcMyCamera->GarbDisplay(MainWndID);
    }

}
void HKCam::startgarb()
{

    int nRet = m_pcMyCamera->SetEnumValue("TriggerMode", MV_TRIGGER_MODE_ON);
    if(nRet != MV_OK)
        return  ;
    nRet = m_pcMyCamera->SetEnumValue("TriggerSource", MV_TRIGGER_SOURCE_SOFTWARE);
    if(nRet != MV_OK)
        return  ;
    m_pcMyCamera->RegisterImageCallBack();
    HWND MainWndID = (HWND)m_pwidget->winId();
    StartGrabbing(MainWndID);
}
int HKCam::StartGrabbing()
{
    //开始采集
    if(m_bOpenDevice == FALSE || m_bStartGrabbing == TRUE)
    {
        return -1;
    }
    int nRet = -1;
    if(m_pcMyCamera != nullptr)
    {
        nRet = m_pcMyCamera->StartGrabbing();
    }
    else {
        return -1;
    }
    if(nRet != MV_OK)
        return -1;
    if(nRet != MV_OK)
    {
        //QMessageBox::information(this,"hint","failed in get payloadsize");
        return -1;
    }
    m_bStartGrabbing = TRUE;
    return nRet;
}

int HKCam::StartGrabbing(HWND MainWndID)
{
    //开始采集
    if(m_bOpenDevice == FALSE || m_bStartGrabbing == TRUE)
    {
        return -1;
    }
    int nRet = -1;
    if(m_pcMyCamera != nullptr)
    {
        nRet = m_pcMyCamera->StartGrabbing();
        //m_pcMyCamera->m_hDevHandle = MainWndID;
        this->m_hwndDisplay = MainWndID;
        if(nRet == MV_OK)
        {
            nRet = m_pcMyCamera->Display(m_hwndDisplay);
        }
    }
    else {
        return -1;
    }
    if(nRet != MV_OK)
        return -1;
    if(nRet != MV_OK)
    {
        //QMessageBox::information(this,"hint","failed in get payloadsize");
        return -1;
    }
    m_bStartGrabbing = TRUE;
    return nRet;
}

int HKCam::StopGrabbing()
{
    if(m_bOpenDevice == FALSE || m_bStartGrabbing == FALSE)
    {
        return -1;
    }
    int nRet = -1;
    if(m_pcMyCamera != nullptr)
    {
        nRet = m_pcMyCamera->StopGrabbing();
    }
    else {
        return -1;
    }
    m_bStartGrabbing = FALSE;
    return nRet;
}

int HKCam::SaveBmp()
{
    if(m_bStartGrabbing == FALSE)
    {
        return -1;
    }
    unsigned int nRecvBufSize=0;
    int nRet = MV_OK;
    if(m_pBufForDriver == nullptr)
    {
        nRet = m_pcMyCamera->GetIntValue("PayloadSize",&nRecvBufSize);
        if(nRet != MV_OK)
        {
            return nRet;
        }
        m_nBufSizeForDriver = nRecvBufSize;
        m_pBufForDriver = static_cast<unsigned char*>(
                    malloc(m_nBufSizeForDriver));
        if(m_pBufForDriver == nullptr)
        {
            return nRet;
        }
    }

    MV_FRAME_OUT_INFO_EX stImageInfo = {0};
    memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
    unsigned int nDataSize = nRecvBufSize;
    unsigned int nImageNum = 1;
    unsigned int nDataLen = 0;

    //==========
    while(nImageNum)
    {
    //一定要初始化， 私有变量， 不然各种bug
    nRet = m_pcMyCamera->GetOneFrameTimeout(m_pBufForDriver,
                                            &nDataLen,
                                            m_nBufSizeForDriver,
                                            &stImageInfo,
                                            10000000);
    if(nRet == MV_OK)
    {
        nImageNum--;
        if(m_pBufForSaveImage == nullptr)
        {
           m_nBufSizeForSaveImage = stImageInfo.nWidth *
                   stImageInfo.nHeight *
                   3 +
                   2048;

           m_pBufForSaveImage = static_cast<unsigned char *>(
                       malloc(m_nBufSizeForSaveImage));

           if(m_pBufForSaveImage == nullptr)
           {
               return -1;
           }
        }
        // ch:设置对应的相机参数 | en:Set camera parameter
        MV_SAVE_IMAGE_PARAM_EX stParam = {0};
        stParam.enImageType = MV_Image_Bmp; // ch:需要保存的图像类型 | en:Image format to save
        stParam.enPixelType = stImageInfo.enPixelType;  // ch:相机对应的像素格式 | en:Camera pixel type
        stParam.nWidth      = stImageInfo.nWidth;         // ch:相机对应的宽 | en:Width
        stParam.nHeight     = stImageInfo.nHeight;          // ch:相机对应的高 | en:Height
        stParam.nDataLen    = stImageInfo.nFrameLen;
        stParam.pData       = m_pBufForDriver;
        stParam.pImageBuffer = m_pBufForSaveImage;
        stParam.nBufferSize = m_nBufSizeForSaveImage;  // ch:存储节点的大小 | en:Buffer node size
        stParam.nJpgQuality     = 80;       // ch:jpg编码，仅在保存Jpg图像时有效。保存BMP时SDK内忽略该参数
        //保存图片这里会返回错误
        nRet = m_pcMyCamera->SaveImage(&stParam);

        if(nRet != MV_OK)
        {
            return -1;
        }


        char chImageName[IMAGE_NAME_LEN] = {0};
        sprintf_s(chImageName,
                  IMAGE_NAME_LEN,
                  "Image_w%d_h%d_fn%03d.bmp",
                  stImageInfo.nWidth,
                  stImageInfo.nHeight,
                  stImageInfo.nFrameNum);
        FILE *fp = fopen(chImageName, "wb");
        fwrite(m_pBufForSaveImage, 1, stParam.nImageLen, fp);
        fclose(fp);
    }
    else {
        return -1;
    }
    }//while
    return nRet;
}

void HKCam::GetSoftwareTrgQImage(QImage & aimage)
{
    getwidget()->show();
    SoftwareTrg();
    aimage = QPixmap::grabWindow(getwidget()->winId()).toImage();
    aimage.save("d:/testxxwww2.bmp");
    getwidget()->hide();
}

void HKCam::SoftwareTrg2Image(void *pimage)
{
    ImageBase *pimagebase = (ImageBase *)pimage;
    getwidget()->show();
    SoftwareTrg();
    *pimagebase = QPixmap::grabWindow(getwidget()->winId()).toImage();
  //  pimagebase->save("d:/testxxwww55.bmp");
    getwidget()->hide();
}
