
#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QMap>
#include <QMutex>
#include <QWaitCondition>
 
#include "connection.h"
#include "connection_serial.h"


#include"../auto/ImageParserClass.h"
#include"../download/downloadmanager.h"

#include"../AI/stringai.h"
#include"../hk/mainwindow.h"


QT_BEGIN_NAMESPACE
class QMainWindow;
class QDialogButtonBox;
class QLabel;
class QListWidget;
class QProgressBar;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QTcpServer;
class QTcpSocket;
class QAction;
class QTextEdit;
class QLineEdit;
class BrowserWindow;
class IrcClient;
class CxImageViewer;
class SQLBrowser;
class CxPythonTest;
class QScriptEngine;
class CxConnection_serial;
class QGraphicsScene;
class QScrollArea;
class QSplitter;
class QTabWidget;
class DiagramTab;

class QEditor;
class QCodeEdit;
class QEditConfig;
class QEditSession;
class QFormatConfig;
class QFormatScheme;
class QLanguageFactory;

class QSnippetBinding;
class QSnippetManager;

class CodeEditor;
class DownloadManager;
class Console;
class dialogOPT;
class visionmanager;
class visionView;
class UrlAI;
class SpectrumWidget;
QT_END_NAMESPACE
 

struct CxGetFile
{
    int m_ifinish;
    int m_ifinishsign;
    QString m_filename;
    QMap<qint64,bool> m_bwrite;
    QMap<qint64,int> m_istatus;
    QMap<qint64,QByteArray> m_getfilebytes;
    QFile *m_tfile;
};
struct CxPieceFile
{	
	QString qsname;
	qint64 totallength; 
	int pieceLength;
	int icurpiece;
	QString qsid;
	QMap<int,QByteArray> m_qcontents;
	QMap<int,int> m_istatus;//0 no use 1 transfer 2 finish
	int m_isfinish;
    int m_isEND;
};
struct cxurl
{
	CxConnection *m_pconnect;
	QString m_url;
	QString m_qsdate;
	QByteArray m_buf;
};
struct CxConnectS
{
	CxConnection *m_pconnect;
	QString m_qsuser;
};
struct cxdutysend
{
	CxConnection *m_pconnectsend;
	QString m_qtype;
	QString m_qstring;
	QString m_stime; 
	QString m_stcomm;
};
struct cxconnectnode
{
    CxConnection * m_pconnect;
    QString m_username;
    int m_itype;
};


static QTime m_sendTime;

class Dialog : public QDialog
{
    Q_OBJECT

public:
	 QMutex g_mutex;
	 QWaitCondition g_cond;
	QByteArray m_reqarray;
	QBasicTimer m_downloadtimer;
	
    void setwintext(QString qstr);
	void SetPosition(int x, int y, int w, int h);
	void newsql();
	void SetDateLoop(int inum,int ivalue,QString &qdate,QString &qstring);
	void ResetDateLoop(QString &qdate,int ivalue);
	void SetSQLRecord(QString &qdate);
	void RunPyton(QString qcomm);
    void CodeViewIni();

private:
    //

    QEditSession *m_session;

    QEditConfig *m_config;
    QFormatConfig *m_formatConfig;

    QTextEdit *m_editor;
 //   QTextEdit *m_editControl;
    QFormatScheme *m_formats;
    QLanguageFactory *m_languages;

   // QToolBar *m_edit, *m_find;

    QSnippetBinding *m_snippetBinding;
    QSnippetManager *m_snippetManager;
    //
    int CanLoop(int inum){return m_pagesrunfinish[inum];}
	void SetLoop(int inum,int ivalue);
	
	CxPythonTest * m_pythontest;
	int m_runonce; 
	QProgressBar *clientProgressBar; 
	QLabel *clientStatusLabel;
	QLabel *serverStatusLabel;


	QPushButton *ServerstartButton;
	QPushButton *ClientstartButton;
 
	QTextEdit *ServerTextEdit;
	int m_ishowtextlinenum;
	QStringList m_textlist;

	QLineEdit *CommandEdit; 
	QLineEdit *TimeTextEdit;
	QLineEdit *adminiEdit;

	QListWidget *widgetList2;

	QPushButton *quitButton;
	QDialogButtonBox *buttonBox;

	QPushButton *adminiButton;
	QPushButton *compressButton;

	QPushButton *addDutyButton;
	QPushButton *MsgButton;
	QPushButton *SendFileButton;
	QPushButton *CommandButton;
    QPushButton *SendCtrlButton;

    QPushButton *SaveParserButton;
    QPushButton *TestParserButton;
    QPushButton *SetValueButton;
    QPushButton *GetValueButton;
    QLineEdit *ValueEdit;
    QLineEdit *SetEdit;

    QVBoxLayout *m_mainLayout;
    QHBoxLayout *buttonCxBox;
	QHBoxLayout *layoutbox;
    QHBoxLayout *parserlayoutbox;

    QString m_savefiledir;

	CxImageViewer *m_imageViewer;

	QMainWindow *m_sqlmain;
	SQLBrowser *m_sqlbrower;
    QFile m_tfile;

public:	
	QListWidget *widgetList;
    QListWidget *widgetList3;
	QTextEdit *ClientTextEdit;
 
	Dialog(QWidget *parent = 0);
	~Dialog();

	CxConnection * GetActiveConnect();
    void removeconnect(CxConnection*pconnect);
	
    void SetDownloadType(int itype){ m_idownlodtype = itype;}
    int GetDownloadType(){return m_idownlodtype;}
	void AddSendReco(CxConnection *pconnect,const QString &strtype,const QString &qstr);//m_waitlist
	void RemoveRecoT(const QString &qstr);
	void RemoveRecoN(const QString &qstr);
	void Sql_PageSave();
	QString GetInputText();

     void newserial();
     void senddirfile();
     void sendserialfile();
     void sendserialfile(const char *pfile);
     void sendserialfilex();
     void sendserialmsg();
	 void deleteconnect(const QString &qstconnect);
	void AppendText(const QString &strtext);
	void ShowLog(CxConnection*pconnect ,const QString &qmsglog);
    void AddLog( const QString &qmsglog);
    void SetLog( const QString &qmsglog);
	void ShowHttpLog(const QString &qmsglog);
	void GetHttpFile(const QByteArray& filecontents);
    void SetCodeText(const QString &strcode);
	void WGetPage(CxConnection*pconnect,const QString& qstring,const QString &qsdate=tr("0"));
	void WSQLPage(CxConnection*pconnect,const QString& qstring,const QString &qsdate=tr("0"));
	void WSQLPageC(CxConnection*pconnect,const QString& qstring,const QString &qsdate=tr("0"));

#ifdef NEWADD
	void WImagePage(CxConnection*pconnect,const QString& qstring,const QString &qsdate=tr("0"));
#endif

	void WGetFile(CxConnection*pconnect,const QString& qstring,const QString &qsdate=tr("0"));
	void WSQLGetFile(CxConnection*pconnect,const QString& qstring,const QString &qsdate=tr("0"));
	
	void FileDir(CxConnection*pconnect,const QString &qsDir);
	void AllShow(CxConnection*pconnect);
	
	
	void GSearch(CxConnection*pconnect,int itype,const QString &qstrselect=tr("div a"));
	
	
	void Send_UserList(CxConnection*pconnect);
	void Send_Page(CxConnection*pconnect);
	void Send_PageList(CxConnection*pconnect,int itype=1,const QString &qstrselect=tr(""));
	void Send_ConnectList(CxConnection*pconnect);
	void Send_Picture(CxConnection*pconnect);
	void Send_Desktop(CxConnection*pconnect,const QString &qmsg);
	void Send_View(CxConnection*pconnect,const QString &qmsg);
	
	void ResetPro();
	void UpDatePro();

	void Set_FrameView(int inum =0,int isx=0,int isy=0,int ivw=0,int ivh=0);
	
    void Add_Data(CxConnection*pconnect,int id);

	void SendLocalFile(CxConnection*pconnect,const QString &fileName);
	void Send_Image(CxConnection*pconnect,const QString &fileName,const QByteArray &pagestr);
	void Send_ImageFrame(CxConnection*pconnect,const QString &fileName,const QByteArray &pagestr);
	void Send_LoadFile(CxConnection*pconnect);
	void Send_File(CxConnection*pconnect,const QString &fileName,const QByteArray &pagestr);
	void Send_Page(CxConnection*pconnect,const QString &fileName,const QByteArray &pagestr);
	void Send_List(CxConnection*pconnect,const QString &fileName,const QByteArray &pagestr);
	void Send_Request(CxConnection*pconnect,const QString &fileName,const QByteArray &pagestr);

	
    void Send_LoadFile_Serial(CxConnection_serial*pconnect);

    void Add_Data_Multi(const QString &user,int id,const QString &qsdate=tr("0"));
    void Add_Data_Multi_OneUser(int id,const QString &qsdate=tr("0"));

    void Send_Picture_Multi(const QString &user,BrowserWindow*pview=NULL,const QString &qsdate=tr("0"));
    void Send_Page_Multi(const QString &user,BrowserWindow*pview=NULL,const QString &qsdate=tr("0"));
    void Send_PageList_Multi(const QString &user, int itype=1,const QString &qstrselect=tr(""),BrowserWindow*pview =NULL,const QString &qsdate=tr("0"));
    void Send_PageSQL_Multi(const QString &user, int itype=1,const QString &qstrselect=tr(""),BrowserWindow*pview = NULL,const QString &qsdate=tr("0"));
    void Send_PageSQL_MultiEx(const QString &user,BrowserWindow*pview = NULL, int itype=1,const QString &qstrselect=tr(""),const QString &qsdate=tr("0"));

	void Send_Desktop_Multi_OneUser(  const QString &qsdate=tr("0"));

	void Send_Desktop_Multi( const QString &user,const QString &qmsg,const QString &qsdate=tr("0"));
	void Send_View_Multi( const QString &user,const QString &qmsg,const QString &qsdate=tr("0"));
    void Send_PageView_Multi( const QString &user,BrowserWindow*pview = NULL,const QString &qsdate=tr("0"));
	
	void Send_MouseKeyBoardMsg_Multi(const QString &qmsg,const QString &qsdate=tr("0"));
	
	void SendLocalFile_Multi(const QString &user,const QString &fileName,const QString &qsdate=tr("0"));
	void Send_Image_Multi(const QString &user,const QString &fileName,const QByteArray &pagestr,const QString &qsdate=tr("0"));
	void Send_ImageFrame_Multi(const QString &user,const QString &fileName,const QByteArray &pagestr,const QString &qsdate=tr("0"));
	void Send_ImageFrame_Multi_OneUser(const QString &fileName,const QByteArray &pagestr,const QString &qsdate=tr("0"));
	

	void Send_LoadFile_Multi(const QString &user,const QString &qsdate=tr("0"));
	void Send_File_Multi(const QString &user,const QString &fileName,const QByteArray &pagestr,const QString &qsdate=tr("0"));
    bool Send_TFile_Multi(const QString &fileName,const QByteArray &pagestr,int ifilename,int iend,const QString &qsdate=tr("0"));
    bool Send_Control_Multi(const QString &pagestr, const QString &qsdate=tr("0"));

    void CollectionDataBufClear();
    int MapSize();
    int Get_SaveTFileSend(const QString &qname,  QByteArray &databuf);
    void Get_ControlString(const QString &qname );
    void Send_FileSQL_Multi(const QString &user,const QString &fileName,const QByteArray &pagestr,const QString &qsdate=tr("0"));
	void Send_Page_Multi(const QString &user,const QString &fileName,const QByteArray &pagestr,const QString &qsdate=tr("0"));
	void Send_List_Multi(const QString &user,const QString &fileName,const QByteArray &pagestr,const QString &qsdate=tr("0"));
	void Send_Request_Multi(const QString &user,const QString &fileName,const QByteArray &pagestr,const QString &qsdate=tr("0"));
	void Send_SQL_Multi(const QString &user,const QString &fileName,const QByteArray &pagestr,const QString &qsdate=tr("0"));

	void Send_SQL_MultiEx(const QString &user,const QString &fileName,const QByteArray &pagestr,const QString &qsdate=tr("0"));
 
    void SetPieceSize(int isize){m_pmsize = isize;}
    int GetPieceSize(){return m_pmsize;}
	
	void Show_ConnectList();
	
	void IrcOpt(CxConnection*pconnect,int iopt,const QString &qstring1,const QString &qstring2);
	void IrcMsg(CxConnection*pconnect,const QString &qstring);
 
	void Set_Clear(CxConnection*pconnect);

	CxConnection*GetConnect(const QString &qstring);
	void P2P_connect(CxConnection*pconnect,const QString &qstring);
	void U2U_connect(CxConnection*pconnect,const QString &qstring,const QString &csum);
	void reset_connect(CxConnection*pconnect,const QString &qstring);
	void reset_all();
	void new_connect(CxConnection*pconnect,const QString &quserstring);
	
    static qint64 GetGolbalTime();
	void AddMsg(const QString &qstring,CxConnection*pconnect=NULL);
	void AddLoacalDuty(const QString &qstring,int itype,const QString &qstime =tr("0"));
	void AddDuty(CxConnection*pconnect,const QString &qstring,const QString & qstime=tr("0"),int itype=0);
	void AddDuty_date(CxConnection*pconnect,const QString &qstring,const QString & qstime,int itype = 5);
	void AddDuty_front(CxConnection*pconnect,const QString &qstring,const QString &qstime =tr("0"),int itype=0);
	void AddDutyNoReply(CxConnection*pconnect,const QString &qstring,const QString &qstrdate=tr("0"));
   // QString getlocationstringx(const QString &strfile);
	QByteArray loadfilestring(const QString& file) const;
	void savefilestring(const QString& fn);
	void getonepiecefile(int icp,int ipsize,int itolsize,const QString &qname,const QString &qid,const QByteArray &qcontent);
    int getonepiecefilex(int icp,int ipsize,int itolsize,const QByteArray &qname,const QByteArray &qid,const QByteArray &qcontent);
    int getnextpiecenum();
	int getfinishnum(int itotalnum);
	int getfilefinish();
	void settransferover();

    //int gettfilefinish();
    //void settfilefinish();
    //void settfilepartfinish();
    //void resetfilefinish();
 
    QStringList CreateurlList(const QStringList &astrlist,const QUrl &aurl);
    void CreateDateList(const QStringList &astrlist,qint64 ibegintime,qint64 igaptime);
    void CreateDateListX(const QStringList &astrlist,qint64 ibegintime,qint64 igaptime);
    void AddDutyfile();
    void AddTFileDuty(QString &strdownloadtfile);
  
	inline int getpiecetransferwaittime(int icp);
	inline int piececanbetransfer(int icp);
 
	CxPieceFile *GetPieceFile(){return &m_piecefile;}
    void setcurconnect(CxConnection *pconnect){m_pcurrentconnect = pconnect;}
    void addpageview(BrowserWindow *pagev){m_pageviewlist.push_back(pagev);}
	void removepageview(int inum);
  
	void SendComm(const QString &fileName);

	void setrequeststring(int itype);
    QString getrequeststring(){return m_reqstring;}
	
	int getcstype(){ return m_iconnecttype;}
	void setdutypage(int idutytype){m_idutytype = idutytype;}
	int getdutypage(){return m_idutytype;}
 
    void Get_SaveFileSend(const QString &qname,const QByteArray &databuf );
	void Get_SaveDateFileSend(const QString &qname );
    void Get_SaveImageSend(const QByteArray &qname,const QByteArray &databuf);
    void Get_SaveImageFrameSend(const QByteArray &qname,const QByteArray &databuf);
	void Get_ShowPageSend();
	void Get_SaveListSend();
	void Get_SqlSend(CxConnection*pconnect);
	void Get_SqlxSend(CxConnection*pconnect);
	
	void savefile(const QByteArray &qcontent,const QString &qname);
	
    void PageFinish(const QString &qname,int itype,BrowserWindow*pview=NULL);
    void PageSQLFinish(const QString &qname,int itype,BrowserWindow*pview=NULL);
    void PageImageFinish(const QString &qname ,int itype,BrowserWindow*pview=NULL);
	
    void PageSQLFinish_Local(const QString &qname,int itype,BrowserWindow*pview=NULL);

	void DownloadFinish(const QString &qname);
	void DownloadSQLFinish(const QString &qname,const QString &sdate);

	void SendFinish(const QString &qmsg1,const QString &qmsg2=tr(""),const QString &qstrdate=tr("00000000"));
	void SendMessage(CxConnection*pconnect,const QString &qstring);
	void SendIrcMessage(const QString &qstring);

	void deleteconnect(CxConnection *pconnect);
	void restartruntimer();
    int getloguse(){return m_ilogin;}

    QString getadminuser(){return m_useradmin;}
	
	void appendreqbuf(const QString &qstrurl,const QByteArray &buff);
//	QByteArray *getreqbuf(const QString &qstrurl);
//	void setreqbuf(const QString &qstrurl,QByteArray* pbuf);
//	void gethandle( Pillow::HttpConnection* rq){m_rq = rq;};
	
	bool sethttplog(const QByteArray &qlog);
	void sethttpreqstring(const QByteArray &qlog);
	QByteArray gethttpreqstring();

    QMap<qint64,CxPiece> * getpmap(){return &m_piecemap;}
    QMap<qint64,cxdata> * getdmap(){return &m_totaldatamap;}
    QMap<qint64,bool> * getfmap(){return &m_datafinishmap;}

    qint64 getsendpiece();
    void setsendpiece(qint64 iID);
    void setfinishpiece(qint64 iID);
#ifdef CXVISIONPILLOW
	HttpHeaderCollection gethttphead();
#endif
    void sethttpreqtype(int ireq){m_ireq = ireq;}
	int gethttpreqtype(){return m_ireq;}
	void SetKy(char akey);
	void SetKy2(char akey1,char akey2);
	void SetKy3(char akey1,char akey2,char akey3);

	void SetMouse(int imouseid,int ix,int iy);
	void UseMouseKeyBoardMsg(const QString &qmsg);
	void Test1(int itype=0);
	CxConnection* geturlconnect(const QString &qstring);
	CxConnection* getdateconnnect(const QString &qstring);
	QString geturldate(const QString &qstring) const;
	QByteArray* geturlbuf(const QString &qstring);
	void removeurllist(const QString &qsurl);
	
	
	void removelist(int item);
	void clearlist();
	void AddConnections(CxConnection*pconnect);
	bool islibload(); 
	
	void MKDir(const QString &astr);
	void RMDir(const QString & astr);
	void cxCopyFile(const QString &stfloderorg,const QString &stflodersrc);
	void RemoveFile(const QString &stfileorg );
	void RenameFile(const QString &strfileold,const QString &strfilenew);
	bool runWithQtInEnvironment(const QString &cmd);

    void addneedstringlist(const QString &str);
    void addextfilter(const QString &str);
    void addkeywordfilter(const QString &str);
    void clearfilter();

    void newconnect();
    void newconnect_pingpong();
public slots:
    void serverStart();

	void clientStart();



	//void restart();
	void restartserver();
	void restartclient();
	
    void ServeracceptConnection();
    void ServerConnection_Serial();
    void ClientConnection_Serial();

    void ClientIsConnection();
	void ServerGetFile();
	void ClientGetFile();
	
	void Clear();
	
	void SendPiece();
	void SendMsg();
    void SendControl();

	void SendMessageToUser(const QString &qsmsg);
	void SetClear();
	void sendCommand();
	void sendDuty();
	//void LoginUser();
	
	void updateProgress(const QString &qstrgos,qint64 numBytes,qint64 totalBytes);
	//void clearProgress();

    //void displayError(QAbstractSocket::SocketError socketError);
	void LoginOK();
	//void SetLoginUSER(CxConnection*pconnect);
	void LoginUSER(CxConnection*pconnect);
	void ReFlashUSER();
	CxConnection*GetConnect(int inum);
	void Run_PageElementClassic(const QString & qsfilesuffix=tr(""),const QString & qsopt=tr("wget"),const QString & qduty1=tr(""),const QString & qduty2=tr(""));
	void Run_Clear();

    void SaveParser();
    void TestParser();
    void TestParserX();
    void SetValue();
    void SetCommandEditText(QString &qstr);
    void SetTextEditText(QString &qstr);
    void SetValueEditText(QString &qstr);
    void GetValue();
    void setallfinish(){m_iallfinish=1;ClearAll();}
    void resetallfinish(){m_iallfinish=0;}
    int getallfinish(){return m_iallfinish;}
    void ClearAll();
private:
	int m_ilogin;
	
	QString m_serverip ;
	QString m_serverport ;
    QTcpServer tcpServer;
    QTcpSocket *m_ptcpClient;
    QTcpSocket *tcpServerConnection;

#ifdef CXVISIONPILLOW
    Pillow::HttpClient *m_client;
    QScriptEngine* m_scriptEngine;

    Pillow::HttpsServer *m_httpserver;
	Pillow::HttpHandlerLog* m_loghandler; 
	Pillow::HttpHandlerStack* m_handler;
    HttpHandlerStats * m_statshandler;
	Pillow::HttpHandlerFile *m_filehandler; 
	Pillow::HttpHandler404 *m_404handler; 
	
	HttpHeaderCollection m_httpsendhead;
#endif

	int m_icurconnect;
   //QDataStream m_serverin;


	int bytesToWrite;
	int bytesWritten;
	int bytesReceived;

    QList<cxconnectnode> m_clientlist;
    QList<cxconnectnode> m_serverlist;

	CxConnection * m_pcurrentconnect;
    CxConnection_serial *m_pconnection_serial;

    void SetActiveConnect(CxConnection * pconnect){m_pcurrentconnect = pconnect;}
	CxConnection*getcurrentconnect(){return m_pcurrentconnect;}

	
	int ClientbytesToWrite;
	int ClientbytesWritten;
	int ClientbytesReceived;
	int ClientGetTotalBytes;
    
    QString m_qclientsend;
	QString m_qserversend;
    
    int ServerbytesToWrite;
    int ServerbytesWritten;
    int ServerbytesReceived;
    int ServerGetTotalBytes;
    
    QString m_useradmin;
	QByteArray savebuffer;

	
    CxPieceFile m_piecefile;
    QList<CxPieceFile> m_piecefilelist;
    QList<CxGetFile> m_getfiles;

    int m_getfilefinish;

     QList<BrowserWindow *> m_pageviewlist;
	 int m_iviewuse;
	
    //int m_itfsendfinish;
	IrcClient *m_pirc;
	
	QMap<QString,IrcClient *> m_ircs;

	QString m_reqstring;
	int m_iconnecttype;
	//int m_ics;
	int m_idownlodtype;
	int m_idutytype;

private:
	void timerEvent(QTimerEvent *event);
    QBasicTimer m_cxvisiontimer;
    int m_btimerun;
	QTimer reconnectTimer;


#ifndef	 TEMPTESTCXVISION
	QTime resqlrunTimer;
	QTime resqlrunTimer2;
	QTime resqlrunTimer3;
	QTime resqlrunTimer4;
	
	QTime m_view1time;
	QTime m_view2time;
	QTime m_view3time;
	QTime m_view4time;
#endif

private:
	QBasicTimer m_localdutytimer;
	QBasicTimer m_connectsdutytimer;
	QBasicTimer m_pagesruntimer;
	QBasicTimer m_eventtimer;
	
	QBasicTimer m_begintimer;
	int m_begintimes;
	
	 QByteArray m_databuf;	
	 QByteArray m_httpsendbuf;

	 int m_pagesrunfinish[4];
	 QString m_pagesrundate[4];
	 QString m_pagestring[4];
	int m_pagesnum;
	int m_pagelevel;
	int m_pagevisitnum;
	
	int m_idutynum;
	
    QString m_recorduser;
	QString m_pagehost;
	QString m_strpage ;
	 QList<cxurl> m_urllist;
	 QList<CxConnectS> m_connections;
	 QList<CxConnectS> m_P2Pconnections;
	 QList<cxdutysend> m_waitlist;

	 QByteArray GetDesktopImage();
     QByteArray GetViewImage(QString&imagesize,BrowserWindow*pview);
	 QString GetDesktopSize();
     QString GetViewSize(BrowserWindow*pview);
	 void HTTPserverStart();
	 void HTTPserverStop();
	 bool LicenseFind(int ifeature); 
	 int m_ireq;

	double m_dxWidthScalar;
	double m_dyHeightScalar; 
	
	 QImage m_image;
	 QImage m_ioldimage;
	 QImage m_resultImage; 
	 QList<cxduty> m_localdutylist;
	 QList<cxduty> m_connectsdutylist;	 
	 QStringList m_getlist;
 
	 int m_pmsize;
     QMap<qint64,CxPiece> m_piecemap;
     QMap<qint64,cxdata> m_totaldatamap;
     QMap<qint64,bool> m_datafinishmap;
     QMap<qint64,bool> m_datasendmap;

     int m_iallfinish;

    static qint64 g_listID;
	
    int GetUserConnectSize(const QString &quser);
    void AddUserConnect(const QString &quser,int isize);
    CxConnection* GetUserConnect(const QString &quser,int inum);
    void C2C(CxConnection* pconnect,CxConnection* pconnect2);

    void Run_my_function();
	void SqlTimerRun();
 
private:

#ifdef CXVISIONBSERVER
	typedef struct CxVisionGolbalStruct
	{
		// Every xx weeks 
		__int32 m_iapphearton[8];
	//	char m_cstr[1024];
	}cxstruct;//m_runpro
	void*  m_hCxvisionMap;
	char *m_pcxvisionstruct;

	void*  m_hResultfile1Map;  
	char   *m_pResultfileBuf1;
	
	cxstruct m_runpro;

	void*  m_hCxvisionMap2;
	char *m_pcxvisionstruct2;
	void GetCxVisionGolbalStruct(); 
	//	void CurAppGolbalSet();
#endif


#ifdef CXVISIONBSERVER
	int CurAppGolbalSet();
	void CurAppGolbalReset();
	void CurAppGolbalUpData();
#endif
 
 public:
    void setEditText(const QString &str);
	static int CxEncrypt_A(char *pbuf,int isize);
	static int CxDecrypt_A(char *pbuf,int isize);
    void Testurl();
    void sceneScale(double dscale);
    void mousegridmove();
    void desktopimage();
    void desktopimagerect(int ix,int iy,int iw,int ih);
    void setimage(QImage& imginput );
    void setimageframe(QImage& imginput );
    void runopt(const char *pname);
    void runstringai();
    void updateview();
    void GetComTcpIpDesktop();
    QString getparservaluestring(QString astrname);
    void Com2TcpIpSendString(const char *pchar);
    void TcpIp2ComSendImage(const char *pvarname);
    void TcpIp2ComSendValue(const char *parvarname);
    void TcpIp2ComSendString(const char *parvarname);
    bool TcpIp2TcpIpSendImage(const char *pvarname);
    bool TcpIp2TcpIpSendValue(const char *pvarname);
    bool TcpIp2TcpIpCompile(const char *pvarname);
    void showmanagelog();
    void pagelistwork(int igap1,int igap2);
    void viewparserlist(const char *pchar);
    void viewrunjavascript(const char *pchar);
    void setgaptime(int itime){m_igaptime=itime;}
    //void setdownload(DownloadManager *pdownload){m_pdownload = pdownload;}
    void rundutylist();
    void cleartab();
    void downloadlink(QString &qstr);
private:
  //  mu::CImageParserClass m_imageparser;
  //  std::ostringstream m_os;//compile result output os
  //  std::ostringstream m_createcodeos;//compile result output os
   QList<DownloadManager *> m_downloadlist;//m_pdownload;
    // DownloadManager *m_pdownload;
 //   Console *m_ptestconsole;

    qint64 m_ibegintime_d;
    qint64 m_ibegintime_h;
    qint64 m_ibegintime_m;
    qint64 m_ibegintime_s;

    int m_igaptime;
    dialogOPT *m_pdialogopt;
    visionView *m_pvisionview;
    visionmanager *m_pmanager;

    //SpectrumWidget *m_pspectrum_input;
    //SpectrumWidget *m_pspectrum_output;


    QSplitter *m_h1Splitter;
    //QScrollArea *m_scrollArea;
    QTabWidget * m_tabWidget;
    DiagramTab * m_diagramtab;
    double m_scaleFactor;


    StringAI *m_pstringai;
};
class VisionTab : public QWidget
{
    Q_OBJECT

public:
    explicit VisionTab(visionView*pview,QWidget *parent = 0);
};
class SpectrumTab : public QWidget
{
    Q_OBJECT

public:
    explicit SpectrumTab(SpectrumWidget*pview1,SpectrumWidget*pview2,QWidget *parent = 0);
};

class dialogOPT
{
private:
    Dialog *m_pdialog;
    DownloadManager *m_pdownload;
    Console *m_pconsole;
    QString m_curstr;
    QTime m_time;
    MainWindow w;

public:
    dialogOPT(){m_pdialog = 0;}
    ~dialogOPT(){}
public:
    void setdialog(Dialog *pdialog)
    {
        m_pdialog = pdialog;
    }
 //   void setdownload(DownloadManager *pdownload)
 //   {
 //       m_pdownload = pdownload;
 //   }
    //void setconsole(Console *pconsole)
    //{
       // m_pconsole = pconsole;
    //}
    //opt


    void test()
    {
        if(0!=m_pdialog)
        {
            m_pdialog->Testurl();
        }
    }
    void download(const char*pchar)
    {
        QString astr(pchar);
        if(0!=m_pdialog)
            m_pdialog->downloadlink(astr);

        //m_pdownload->append(astr);
    }
    void downloadA()
    {
        //QStringList astrlist;
        //astrlist.append(m_curstr);
        //m_pdownload->append(astrlist);
        if(0!=m_pdialog)
            m_pdialog->downloadlink(m_curstr);

    }
    void runviewparser(const char*pchar)
    {
        if(0!=m_pdialog)
            m_pdialog->viewparserlist(pchar);
    }
    void runjavascript(const char *pchar)
    {
        if(0!=m_pdialog)
            m_pdialog->viewrunjavascript(pchar);
    }
    void setgaptime(int igaptime)
    {
        if(0!=m_pdialog)
            m_pdialog->setgaptime(igaptime);
    }
    void getcomtcpipdesktop()
    {
        if(0!=m_pdialog)
            m_pdialog->GetComTcpIpDesktop();
    }

    QString inputstring(const char pchar)
    {return QString(" ");
    }
    void com2tcpipkeystring(const char *pchar)
    {
        QString strget(pchar);
        QString strsend = "aconsole.click(1);aconsole.doevent();";
        int isize = strget.size();
        for(int i=0;i<isize;i++)
        {
           QString strchar = inputstring(pchar[i]);
           strsend = strsend + QString("aconsole.key(\"")+strchar+QString("\");aconsole.doevent();");
        }
        if(0!=m_pdialog)
            m_pdialog->Com2TcpIpSendString(strsend.toStdString().c_str());

    }
    void addparservaluestring(const char *pchar)
    {
        QString stradd;
        if(0!=m_pdialog)
              stradd = m_pdialog->getparservaluestring(QString(pchar));
        m_curstr = m_curstr + stradd;
    }
    void addstring(const char *pchar)
    {
        m_curstr = m_curstr + QString(pchar);
    }
    void setcurstring(const char *pchar)
    {
        m_curstr = QString(pchar);
    }
    void com2tcpipcurstring()
    {
        if(0!=m_pdialog)
            m_pdialog->Com2TcpIpSendString(m_curstr.toStdString().c_str());
        m_pdialog->ShowLog(0,m_curstr);
    }
    void com2tcpipstring(const char *pchar)
    {
        if(0!=m_pdialog)
            m_pdialog->Com2TcpIpSendString(pchar);
    }
    void tcp2comvalue(const char *pvar)
    {
        if(0!=m_pdialog)
            m_pdialog->TcpIp2ComSendValue(pvar);
    }
    void tcp2comimage(const char *pchar)
    {
        if(0!=m_pdialog)
            m_pdialog->TcpIp2ComSendImage(pchar);
    }
    void tcp2tcpimage(const char *pchar)
    {
        if(0!=m_pdialog)
            m_pdialog->TcpIp2TcpIpSendImage(pchar);
    }
    void tcp2tcpvalue(const char *pchar)
    {
        if(0!=m_pdialog)
            m_pdialog->TcpIp2TcpIpSendValue(pchar);
    }
    void tcp2tcpstring(const char *pchar)
    {
        if(0!=m_pdialog)
            m_pdialog->TcpIp2TcpIpCompile(pchar);
            //m_pdialog->Send_Control_Multi(QString(pchar));
    }
    void tcp2comstring(const char *pchar)
    {
        if(0!=m_pdialog)
            m_pdialog->TcpIp2ComSendString(pchar);
    }
    void sendserialfile(const char *pfile)
    {
        if(0!=m_pdialog)
            m_pdialog->sendserialfile(pfile);

    }
    void runconsole(const char *pchar)
    {
        //if(0!=m_pconsole)
        //    m_pconsole->RunConsole(pchar);
    }

    void desktopimagerect(int ix,int iy,int iw,int ih)
    {
        if(0!=m_pdialog)
            m_pdialog->desktopimagerect(ix,iy,iw,ih);
    }
    void desktopimage()
    {
        if(0!=m_pdialog)
            m_pdialog->desktopimage();
    }
    void downloadreset()
    {
       // if(0!=m_pdialog)
       //     m_pdialog->settfilefinish();
    }
    void runcommand(const char *pname)
    {
        if(0!=m_pdialog)
        {
            QString astr(pname);
            m_pdialog->SetCommandEditText(astr);
            m_pdialog->sendCommand();
        }
    }
    void runopt(const char *pname)
    {
        if(0!=m_pdialog)
            m_pdialog->runopt(pname);
    }
    void runstringai()
    {
        if(0!=m_pdialog)
             m_pdialog->runstringai();
    }
    void pagelistwork(int igap1,int igap2)
    {
        if(0!=m_pdialog)
            m_pdialog->pagelistwork( igap1, igap2);
    }
    void asleep(int isleeptime)
    {
        //m_time.start();
       // while(m_time.elapsed()<isleeptime)
        //    QCoreApplication::processEvents();

            QTime time;
            time.restart();
            while (time.elapsed() < isleeptime)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 50);


    }
    void scale(double dscale)
    {
        if(0!=m_pdialog)
            m_pdialog->sceneScale(dscale);
    }
    void rundutylist()
    {
        if(0!=m_pdialog)
            m_pdialog->rundutylist();
    }
    void cleartab()
    {
        if(0!=m_pdialog)
            m_pdialog->cleartab();
    }
    void updateview()
    {
        if(0!=m_pdialog)
            m_pdialog->updateview();
    }
    void showlog()
    {
        if(0!=m_pdialog)
            m_pdialog->showmanagelog();
    }
    void showstring(const char *pchar)
    {
        if(0!=m_pdialog)
            m_pdialog->ShowLog(0,QString(pchar));
    }
    void urlfilter(const char *pchar)
    {
        if(0!=m_pdialog)
            m_pdialog->addkeywordfilter(QString(pchar));
    }
    void filextfilter(const char *pchar)
    {
        if(0!=m_pdialog)
            m_pdialog->addextfilter(QString(pchar));


    }
    void urlneedstring(const char *pchar)
    {
        if(0!=m_pdialog)
            m_pdialog->addneedstringlist(QString(pchar));

    }
    void clearfilter()
    {
        if(0!=m_pdialog)
            m_pdialog->clearfilter();

    }
    void CamTest()
    {
        w.show();
    }

};



#endif
