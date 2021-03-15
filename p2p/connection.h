
#ifndef CONNECTION_H
#define CONNECTION_H

#include <QHostAddress>
#include <QString>
#include <QTcpSocket>
#include <QTime>
#include <QTimer>

static const int TRANSFOR_SIZE =  5*1024*1024;//1024*500
static const int MaxBufferSize =  10*1024*1024;//1024*1024
static const int TEMP_SIZE =  6*1024*1024;//1024*500


static   int TransferTimeout = 40 * 1000;
static   int PongTimeout = 50 * 1000;
static   int PingInterval = 20 * 1000;
static const char SeparatorToken = ' ';

struct cxdata
{
	QString m_stype;
	QByteArray m_data;
};
struct cxduty
{
	int m_istats;
	int m_itype;
	QString m_stime; 
	QString m_stcomm;
	QString m_stname;
	QString m_stparm1;
	QString m_stparm2;
	QString m_stparm3;
	QString m_qsparmall;
};

class Dialog;
struct CxPiece
{	
    //bool m_bpresend;
    int m_ipiecesize;
    int pieceLength;
    int icurpiece;

    QMap<int,qint64> m_ipiecestatus;//0 no use 1 transfer 2 finish
	qint64 totallength; 
	QByteArray *pstringbuf;
	QString qsname;
	QString qsid;
    QString m_user;
};
class CxConnection : public QObject//: public QTcpSocket
{
    Q_OBJECT

public:
    enum ConnectionState {
        WaitingForGreeting,
        ReadingGreeting,
        ReadyForUse,
        P2Pconnection,
        NO_Encrypt_WaitingForGreeting
    };
    enum DataType {
        PlainText,
        Ping,
        Pong,
        Greeting,
        GFileINF,
        CxCommand,
        CxData,
        CxNext,
		CxSaveFile,
        Login,
        Logok,
        Undefined
    };
    CxConnection(Dialog *pdialog);
    CxConnection(QTcpSocket *parent,Dialog *pdialog,int icos);
    CxConnection(Dialog *pdialog,QTcpSocket *parent);
    CxConnection(QTcpSocket *parent,Dialog *pdialog,int icos,int itemp);
	~CxConnection();

	static void Loadmyfunction();

	static int Encrypt_(char *pbuf,int isize);
	static int Decrypt_(char *pbuf,int isize);
	
	void TestA();
	//void Run_my_function(const QByteArray &qdata);

	bool IsBusy();
    void setdatabusy(){m_isendfinish_data = 0;m_nextdataTime.restart();}
    void setdatafree(){m_isendfinish_data = 1;m_nextdataTime.restart();}

    void setdutybusy(){m_isendfinish_duty = 0;m_nextdataTime.restart();}
    void setdutyfree(){m_isendfinish_duty = 1;m_nextdataTime.restart();}

    void ShowLog(const QString &strlog);
    void AddLog(const QString &strlog);
    void SetLog(const QString &strlog);
    QByteArray gemfieldCRC(QByteArray gemfield);

    void connectstart();
	void connectisok(int iconnectsum = 1);
	void reconnect(QTcpSocket *parent, int icos);
	void deletesocket();
	
	void stoptimer();
	QByteArray *getbufp();
    QByteArray getbufx() const;
    QTcpSocket *getsocket(){return m_psocket;}
    void setbufx(const QByteArray &buf );
	void bufcompress(){m_databufx = qCompress(m_databufx);}
	void CommandProcess(QString &qcomm);
	QString name() const;
	QString shortname() const;
	QString localAddress() const;
	QString peerAddress() const;
	int peerPort()const;
	int localPort()const;
	
    QByteArray EnHeadFormat(QByteArray headmsg);
    QByteArray EnHeadFormat2(QByteArray headmsg);
	QByteArray EnHeadFormatRand(QByteArray headmsg);
	void setGreetingMessage(const QString &message);
    bool sendLogin(const QString &message);
    bool sendLogok();
	bool sendMessage(const QString &message);
	void connectToHost(QHostAddress adress,int iport);

    bool sendData(qint64 iID);
	bool sendOnePiece(int ipiece,const QString &stid);

    virtual bool sendCommand(const QString &message,const QString &stid="00000000");
	bool sendNext(int ipiece);
	bool sendNextResend();
    bool WaitNextMsg(int iwaittime=120*1000);
	void sendIPKEY();
	void sendpublicK(int ivalue1,int ivalue2,int ivalue3,int ivalue4,int ivalue5);
	void setpiecesize(int isize);
	QString RandSum();
	QString getipstring(){return ipstring;}
	
	void SetUser(const QString &user);
	QString GetUser(){return m_user;}

	QByteArray Test(QByteArray data);
	void AddDuty(const cxduty &aduty);
	void AddDuty(const QString &qstring,const QString &qstime =tr("0"),int itype=0);
	void AddDuty_front(const cxduty &aduty);
	void AddDuty_frontstr(const QString &qstring,const QString &qstime=tr("0"),
									int itype=0,const QString &qsname=tr(""),
									const QString &qsparm1=tr(""),
									const QString &qsparm2=tr(""),
									const QString &qsparm3=tr(""),
									const QString &qsparmall=tr(""));

	void AddMsg(const QString &qstring);

	void Clear();
	void Reset();
    void Run();

	void setattachconnect(CxConnection*pconnect){m_pattachconnect=pconnect;}
    CxConnection*getattachconnect(){return m_pattachconnect;}
    QTimer m_reconnectTimer;

	//cxf
    int IScors(){return m_cos;}
    void setcos(int itype){m_cos=itype;}
signals:
    void readyForUse();
    void newMessage(const QString &from, const QString &message);

protected:
    void timerEvent(QTimerEvent *timerEvent);

public slots:

    void sendPong();
    void sendGreetingMessage();
    void restart();

public:
    void setstate(ConnectionState state){m_state = state;}
    ConnectionState getstate(){return m_state;}
    virtual void processData();
    bool readProtocolHeader();
    void setdatetype(DataType atype){m_currentDataType = atype;}
    DataType getdatetype(){return m_currentDataType;}
    bool hasEnoughData();
    void setconnectbuf(const QByteArray &buf ){m_buffer=buf;}
    QByteArray &getconnectbuf() {return m_buffer;}

    void setbytesfordatenum(int itype){m_numBytesForCurrentDataType=itype;}
    int getbytesfordatenum(){return m_numBytesForCurrentDataType;}


    inline qint64 Ewrite(const QByteArray &data);
    QByteArray Eread(qint64 maxlen);
    void setgreetingusername(QString quser){m_greetingusername=quser;}
    QString getgreetingusername()const {return m_greetingusername;}

    void relogin();
    bool getloguse();
    void LoginUSER(CxConnection*pconnect);
    void LoginOK();

    void removelnull(QByteArray &buffer);

    QBasicTimer &getdutytimer(){return m_dutytimer;}
    QBasicTimer &getpingtimer(){return m_pingTimer;}
    QTime &getpongtime(){return m_pongTime;}


    int IsAdminist(const QString &user);
    void setcustomtype(bool bvalue){m_customtype = bvalue;}
    bool getcustomtype(){return m_customtype;}
    void setcustomstring(QString &string){m_customstring = string;}
    QString getcustomstring(){return m_customstring;}

private:
    int sendpiecewaittime(int icp);
	int piececanbesend(int icp);
	void setsendfinish(int icp);
    qint64 getnextsend();

	void SendFinish(const QString &qmsg1,const QString &qmsg2,const QString &qstrdate);
	int readEnProtolBuffer(int maxSize = MaxBufferSize);
	int dataLengthForCurrentEnDataType();
    int readDataIntoBuffer(int maxSize = MaxBufferSize);
    int dataLengthForCurrentDataType();

	void DataPieceProcess(QByteArray &qcomm);
	void Send_Data();
    void Duty_Run();

    void processRead_easy();//2020 10 06
    QTcpSocket *m_psocket;

	Dialog *m_pdialog;
	
	QString m_user;
	QString m_peerAddress;
    bool m_bencrypt;

    bool m_customtype;
    QString m_customstring;
	
	int m_cos;
	int m_irestarttimes;

	QString m_greetingMessage;
    QString m_greetingusername;

    QString m_qsprecommand;

    QBasicTimer m_dutytimer;
    QBasicTimer m_pingTimer;
    QTime m_pongTime;

	QTime m_nextdataTime;
    QByteArray m_buffer;
	QString ProtocolHeaderbuff;
	QString ProtocolSizebuff;
	QString ipstring;
    ConnectionState m_state;
    DataType m_currentDataType;

    int m_numBytesForCurrentDataType;

    bool isGreetingMessageSent;

    
    int m_iuserclass;
    
    qint64 m_imaxlen;
    QByteArray *m_pstrdata;

    uint m_pkey1[8];
    uint m_pkey2[8];
    
	inline int CxDecrypt_1(char *pbuf,int isize);
	inline int CxEncrypt_1(char *pbuf,int isize);
	inline int CxDecrypt_2(char *pbuf,int isize);
	inline int CxEncrypt_2(char *pbuf,int isize);

	inline int CxDecrypt(char *pbuf,int isize);
	inline int CxEncrypt(char *pbuf,int isize);
	
	void SetKey(long a1,long a2,long a3,long a4,long b1,long b2,long b3,long b4,
		long c1,long c2,long c3,long c4,long d1,long d2,long d3,long d4);

	QByteArray m_databufx;	
	
    QMap<qint64,CxPiece> *m_Pmap;
    QMap<qint64,cxdata> *m_Dmap;
    QMap<qint64,bool> *m_Fmap;

	QList<cxduty> m_dutylist;
	QList<QString> m_msglist;
		
	CxPiece *m_apiece;
    qint64 m_icurID;

	int m_isendfinish_data;
	int m_isendfinish_duty;
	
	int m_irecaltimes;
	
	CxConnection *m_pattachconnect;
public:
    Dialog* getdialog(){return m_pdialog;}
    virtual QString getio_local_address_string();
    virtual QString getio_peer_address_string();
    virtual int iolocalport();
    virtual int iopeerport();

    virtual bool iovalid();
    virtual void ioclose();
    virtual qint64 iowrite(const QByteArray &data);
    virtual QByteArray ioread(qint64 maxlen);
    virtual void ioabort();
    virtual qint64 iobytesAvailable();
    virtual QByteArray ioreadall();
    virtual QByteArray EreadAll();
    virtual void overconnect();
    virtual void processRead();
public slots:
    void processReadyRead();

    //virtual void connectToHost(QHostAddress adress,int iport);

//	QByteArray *m_preqbuff;	
};

#endif
