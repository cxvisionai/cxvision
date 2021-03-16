#include <QtWidgets>
#include <QtNetwork>
#include <QtGui/qevent.h>

#include <QTest>

#include "codeeditor.h"
#include "../auto/ImageParserClass.h"

#ifdef win32
#include <windows.h>
#endif

#include "dialog.h"


#ifdef CXVISIONIMAGEVIEW
#include "imageviewer.h"
#endif

#include "cximage/visionmanager.h"

#include "diagramtab.h"

#include "norwegianwoodstyle.h"

 
qint64 Dialog::g_listID = 0;
QString  getlocationstringx(const QString &strfile)
{
    QString locationstring;
#if defined( Q_OS_WINx )
    wchar_t module_name[260];
    GetModuleFileName(0, module_name, 260);
    QFileInfo filePath = QString::fromWCharArray(module_name);
    locationstring = filePath.filePath();
#else
    QString argv0 = strfile;//QFile::decodeName(QByteArray(strfile));
    QString absPath;

    if (!argv0.isEmpty() && argv0.at(0) == QLatin1Char('/'))
    {
        /*
        If argv0 starts with a slash, it is already an absolute
        file path.
        */
        absPath = argv0;
    }
    else if (argv0.contains(QLatin1Char('/')))
    {
        /*
        If argv0 contains one or more slashes, it is a file path
        relative to the current directory.
        */
        absPath = QDir::current().absoluteFilePath(argv0);
    }
    else
    {
        /*
        Otherwise, the file path has to be determined using the
        PATH environment variable.
        */
        QByteArray pEnv = qgetenv("PATH");
        QDir currentDir = QDir::current();
        QStringList paths = QString::fromLocal8Bit(pEnv.constData()).split(QLatin1String(":"));
        for (QStringList::const_iterator p = paths.constBegin(); p != paths.constEnd(); ++p) {
            if ((*p).isEmpty())
                continue;
            QString candidate = currentDir.absoluteFilePath(*p + QLatin1Char('/') + argv0);
            QFileInfo candidate_fi(candidate);
            if (candidate_fi.exists() && !candidate_fi.isDir()) {
                absPath = candidate;
                break;
            }
        }
    }

    absPath = QDir::cleanPath(absPath);
    if(!absPath.isEmpty())
       return absPath;
    QFileInfo fi(absPath);
    locationstring = fi.exists() ? fi.canonicalFilePath() : QString();
#endif
    if(!locationstring.isEmpty())
        locationstring =QFileInfo(locationstring).absolutePath()+ QLatin1Char('/') +strfile;
    return locationstring;
}


QProcess proc;
// void* Dialog::hEventWaitFinish = NULL;
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    m_getfilefinish = 1;
    m_iallfinish = 0;
    m_sendTime.start();

    //settfilefinish();

    m_btimerun = 0;
    m_ibegintime_d =0;
    m_ibegintime_h=0;
    m_ibegintime_m=0;
    m_ibegintime_s=0;
	m_runonce =0;
	m_idownlodtype = 0;
    g_cond.wakeOne();
    m_pmsize = TRANSFOR_SIZE;//1024*500;//1024000;//4096
	m_sqlmain = NULL;
	m_sqlbrower = NULL;
	m_imageViewer = NULL;
    m_ishowtextlinenum = 0;
	m_ilogin = 0;
	m_pcurrentconnect = NULL;
	
	m_pirc = NULL;
	m_ptcpClient = NULL;

    clientProgressBar = new QProgressBar;
    clientStatusLabel = new QLabel(tr("Client ready"));
    //serverProgressBar = new QProgressBar;
    serverStatusLabel = new QLabel(tr("Server ready"));

	ServerTextEdit = new QTextEdit;
	ClientTextEdit = new QTextEdit;
    m_editor = new QTextEdit;
	
	CommandEdit = new QLineEdit;
	TimeTextEdit = new QLineEdit;
	adminiEdit =  new QLineEdit;
	
	widgetList = new QListWidget;
	widgetList2= new QListWidget;
    widgetList3= new QListWidget;
    //m_codeeditor = new CodeEditor;

    ServerTextEdit->setMinimumHeight(80);
    ClientTextEdit->setMinimumHeight(50);
	ClientTextEdit->setMaximumHeight(50); 
	 
    widgetList->setMinimumHeight(80);
    widgetList2->setMinimumHeight(80);
    widgetList3->setMinimumHeight(80);

    m_editor->setMinimumHeight(650);
   m_editor->setMaximumHeight(1600);

    ServerstartButton = new QPushButton(tr("StartSer&ver"));
    ServerstartButton->setMinimumWidth(80);

    ClientstartButton = new QPushButton(tr("StartC&lient"));
    ClientstartButton->setMinimumWidth(80);

	MsgButton = new QPushButton(tr("&SendMsg"));
    MsgButton->setMinimumWidth(80);

	SendFileButton = new QPushButton(tr("SendDa&ta"));
    SendFileButton->setMinimumWidth(80);

    SendCtrlButton = new QPushButton(tr("SendC&trl"));
    SendCtrlButton->setMinimumWidth(80);

	CommandButton = new QPushButton(tr("Comman&d"));
    CommandButton->setMinimumWidth(80);

    quitButton = new QPushButton(tr("&Reset"));
    quitButton->setMinimumWidth(80);
    adminiButton = new QPushButton(tr("&RunParser"));
    adminiButton->setMinimumWidth(80);
    compressButton = new QPushButton(tr("Start/Stop"));
    compressButton->setMinimumWidth(80);
	addDutyButton = new QPushButton(tr("AddDuty"));
    addDutyButton->setMinimumWidth(80);


    SaveParserButton = new QPushButton(tr("&SaveParser"));
    SaveParserButton->setMinimumWidth(80);
    TestParserButton = new QPushButton(tr("&TestParser"));
    TestParserButton->setMinimumWidth(80);
    SetValueButton = new QPushButton(tr("&SetValue"));
    SetValueButton->setMinimumWidth(80);
    GetValueButton = new QPushButton(tr("&GetValue"));
    GetValueButton->setMinimumWidth(80);
    ValueEdit = new QLineEdit;
    ValueEdit->setMinimumWidth(80);
    SetEdit = new QLineEdit;
    SetEdit->setMinimumWidth(80);

    buttonBox = new QDialogButtonBox;
    buttonCxBox = new QHBoxLayout;

	layoutbox = new QHBoxLayout;

    parserlayoutbox = new  QHBoxLayout;

	CxConnection::Loadmyfunction();

    buttonBox->addButton(CommandButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(ServerstartButton, QDialogButtonBox::ActionRole );

    buttonBox->addButton(ClientstartButton, QDialogButtonBox::ActionRole);

	buttonBox->addButton(compressButton, QDialogButtonBox::ActionRole);
	buttonBox->addButton(addDutyButton, QDialogButtonBox::ActionRole);
	buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    buttonCxBox->addWidget(adminiButton );
    buttonCxBox->addWidget(SetValueButton);
    buttonCxBox->addWidget(GetValueButton);
    buttonCxBox->addWidget(ValueEdit);
    buttonCxBox->addWidget(SetEdit);


    buttonCxBox->addWidget(MsgButton );
    buttonCxBox->addWidget(SendCtrlButton );
    buttonCxBox->addWidget(SendFileButton );

    connect(ServerstartButton, SIGNAL(clicked()), this, SLOT(serverStart()));
    connect(ClientstartButton, SIGNAL(clicked()), this, SLOT(clientStart()));
    connect(compressButton, SIGNAL(clicked()), this, SLOT(Testurl()));
	connect(addDutyButton, SIGNAL(clicked()), this, SLOT(sendDuty()));
	
	connect(MsgButton, SIGNAL(clicked()), this, SLOT(SendMsg()));
	connect(SendFileButton, SIGNAL(clicked()), this, SLOT(SendPiece()));
    connect(SendCtrlButton, SIGNAL(clicked()), this, SLOT(SendControl()));


	connect(CommandButton, SIGNAL(clicked()), this, SLOT(sendCommand()));
    connect(CommandEdit, SIGNAL(returnPressed()), this, SLOT(sendCommand()));

    connect(quitButton, SIGNAL(clicked()), this, SLOT(restart()));

    reconnectTimer.setInterval(8 * 1000);

	connect(&reconnectTimer, SIGNAL(timeout()), this, SLOT(restart()));

    layoutbox->addWidget(widgetList);
    layoutbox->addWidget(widgetList2);
    layoutbox->addWidget(widgetList3);

parserlayoutbox->addWidget(TestParserButton);
parserlayoutbox->addWidget(SaveParserButton);
//parserlayoutbox->addWidget(SetValueButton);
//parserlayoutbox->addWidget(GetValueButton);
//parserlayoutbox->addWidget(ValueEdit);
//parserlayoutbox->addWidget(SetEdit);
ValueEdit->setText(QString("0"));
SetEdit->setText(QString("doutputvalue"));

connect(SaveParserButton, SIGNAL(clicked()), this, SLOT(SaveParser()));
connect(TestParserButton, SIGNAL(clicked()), this, SLOT(TestParser()));
connect(adminiButton, SIGNAL(clicked()), this, SLOT(TestParser()));


connect(SetValueButton, SIGNAL(clicked()), this, SLOT(SetValue()));
connect(GetValueButton, SIGNAL(clicked()), this, SLOT(GetValue()));

    m_mainLayout = new QVBoxLayout;

    m_mainLayout->addStretch(1);
    m_mainLayout->addSpacing(10);
    m_mainLayout->addLayout(parserlayoutbox);
    m_mainLayout->addWidget(buttonBox);
    m_mainLayout->addWidget(CommandEdit);

    m_mainLayout->addWidget(ServerTextEdit);
    m_mainLayout->addWidget(serverStatusLabel);
    m_mainLayout->addWidget(m_editor);
    m_editor->setFont(QFont("Fixedsys", 16));
    CodeViewIni();
    m_mainLayout->addWidget(clientProgressBar);
    m_mainLayout->addWidget(clientStatusLabel);
    m_mainLayout->addWidget(ClientTextEdit);

	QHBoxLayout *editlayoutbox = new QHBoxLayout;
	editlayoutbox->addWidget(adminiEdit); 
	editlayoutbox->addWidget(TimeTextEdit); 


    m_pvisionview = new visionView("cxvision");
    m_pmanager = m_pvisionview->view();

    //m_pspectrum_input = new SpectrumWidget;
    //m_pspectrum_output = new SpectrumWidget;

    m_diagramtab = new DiagramTab;
    m_tabWidget = new QTabWidget;
    m_tabWidget->addTab(new VisionTab(m_pvisionview), tr("Vision"));
    m_tabWidget->addTab(m_diagramtab, tr("AI"));

    QHBoxLayout *alayout = new QHBoxLayout;
    QVBoxLayout *piclayout = new QVBoxLayout;

    piclayout->addWidget(m_tabWidget);

    piclayout->addLayout(buttonCxBox);
    piclayout->addLayout(editlayoutbox);
    piclayout->addLayout(layoutbox);

    alayout->addLayout(piclayout);
    alayout->addLayout(m_mainLayout);
    setLayout(alayout);

    QObject::connect(m_diagramtab, SIGNAL(SelectState(const QString &)),m_pmanager, SLOT(SetOptStateName(const QString &)));
    QObject::connect(m_diagramtab, SIGNAL(SelectConnect(const QString &)),m_pmanager, SLOT(SetOptConnectName(const QString &)));
    QObject::connect(m_diagramtab, SIGNAL(SelectConnect(const QString &)),m_pmanager, SLOT(loadconnectstring()));
    QObject::connect(m_diagramtab, SIGNAL(SelectState(const QString &)),m_pmanager, SLOT(loadstatestring()));
    QObject::connect(m_pmanager, SIGNAL(OperationModelList(const QStringList&)),m_diagramtab, SLOT(AddModelList(const QStringList&)));
    QObject::connect(m_pmanager, SIGNAL(OperationModelConnectList(const QStringList&)),m_diagramtab, SLOT(AddModelConnectList(const QStringList&)));
    QObject::connect(m_pmanager, SIGNAL(RunState(const QString&)),m_diagramtab, SLOT(SetModelColor(const QString&)));
    QObject::connect(m_pmanager, SIGNAL(RunState(const QString&)),m_diagramtab, SLOT(HighlighterDebugModel(const QString&)));
    QObject::connect(m_pmanager, SIGNAL(RunConnect(const QString&)),m_diagramtab, SLOT(SetModelConnectColor(const QString&)));
    QObject::connect(m_pmanager, SIGNAL(RunConnect(const QString&)),m_diagramtab, SLOT(HighlighterDebugModel(const QString&)));

    QObject::connect(m_pmanager, SIGNAL(runopt(const QString&)),m_diagramtab, SLOT(SetModelColor(const QString&)));
    QObject::connect(m_pmanager, SIGNAL(runopt(const QString&)),m_diagramtab, SLOT(HighLightModelColor(const QString&)));

    QObject::connect(m_pmanager, SIGNAL(runopt(const QString&)),m_diagramtab, SLOT(AddModelConnectB(const QString&)));
    QObject::connect(m_pmanager, SIGNAL(runoptover(const QString&)),m_diagramtab, SLOT(AddModelConnectA(const QString&)));
  //QObject::connect(m_pmanager, SIGNAL(runoptover(const QString&)),m_diagramtab, SLOT(NormalModelColor(const QString&)));

    QObject::connect(m_pmanager, SIGNAL(StateClear()),m_diagramtab, SLOT(deleteAllItem()));
    QObject::connect(m_pmanager, SIGNAL(signals_GetConnectString(const QString&)),m_editor, SLOT(setText(const QString&)));
    QObject::connect(m_pmanager, SIGNAL(signals_GetStateString(const QString&)),m_editor, SLOT(setText(const QString&)));


    m_pmanager->initialparser();//cxvision important

    ClientGetTotalBytes =  -1;
    ServerGetTotalBytes = - 1;
    
   m_icurconnect = -1;
   m_iconnecttype = 0;
	
   m_idutytype = 0;
   m_reqstring =tr("file_");

   m_ireq = 0;
   m_pagesnum = 0;
   m_pagelevel = 0;
   m_iviewuse = 0;
   m_pagevisitnum  = 3;

   QString qstrtime = QTime::currentTime().toString("Uhhmmsszzz");//U123359375C123415656
   m_useradmin = qstrtime;
   m_igaptime = 100;

	{ 


        m_pdialogopt = 0;
        m_pdialogopt = m_pmanager->getopt();
        m_pdialogopt->setdialog(this);
  
        m_editor->setText(m_pmanager->getloadcode());


    }


    m_localdutytimer.start(1000, this);
   m_connectsdutytimer.start(100, this);
   m_eventtimer.start(20,this);
   
   m_begintimes = 0;
   m_begintimer.start(1000,this);
 SetLoop(0,1);
 SetLoop(1,1);
 SetLoop(2,1);
 SetLoop(3,1);
 


m_idutynum = 0;
}
Dialog::~Dialog()
{
    m_pmanager->saveautocreate();

    if(NULL!=m_pmanager)
        delete m_pmanager;
	if(NULL!=m_imageViewer)
		delete m_imageViewer;
	m_imageViewer = NULL;



}







void Dialog::newsql()
{

}
QString Dialog::GetInputText()
{
	return adminiEdit->text();
}
bool Dialog::islibload()
{

}
bool Dialog::LicenseFind(int ifeature)
{


    return true;
}
void Dialog::HTTPserverStart()
{  

 
}
void Dialog::HTTPserverStop()
{   
}

void Dialog::sethttpreqstring(const QByteArray &pagestr)
{
	
}
void Dialog::GetHttpFile(const QByteArray& filecontents)
{

}
QByteArray Dialog::gethttpreqstring()
{

	return m_httpsendbuf;
}


bool Dialog::sethttplog(const QByteArray &qlog )
{

	return false; 
	
}

void Dialog::serverStart()
{
    //m_ics = 0;//server
	ServerstartButton->setEnabled(false);
	ClientstartButton->setEnabled(false);
    connect(&tcpServer, SIGNAL(newConnection()),this, SLOT(ServeracceptConnection()));



    bytesWritten = 0;
    bytesReceived = 0;
 
    while (!tcpServer.isListening() && !tcpServer.listen(QHostAddress::Any,82)) 
    {
        QMessageBox::StandardButton ret = QMessageBox::critical(this,
                                        tr("CxServer"),
                                        tr("Unable to start the test: %1.")
					.arg(tcpServer.errorString()),
                                        QMessageBox::Retry
					| QMessageBox::Cancel);
        if (ret == QMessageBox::Cancel)
            return;
    }

    serverStatusLabel->setText(tr("Listening"));
 
 
}
void Dialog::restartserver()
{

	tcpServer.close();

	connect(&tcpServer, SIGNAL(newConnection()),this, SLOT(ServeracceptConnection()));

	connect(&tcpServer, SIGNAL(newConnection()),this, SLOT(NewConnections()));

	bytesWritten = 0;
	bytesReceived = 0;
	QString serverport = adminiEdit->text();
	int iport = 82;
	if(serverport!="")
		iport = serverport.toInt(); 
	while (!tcpServer.isListening() && !tcpServer.listen(QHostAddress::Any,iport)) 
	{ 
		reconnectTimer.start();
	}

	serverStatusLabel->setText(tr("Listening"));
}
void Dialog::restartruntimer()
{
	reconnectTimer.start();
}
void Dialog::restartclient( )
{	
	reconnectTimer.stop();

	ServerstartButton->setEnabled(false);

	ClientstartButton->setEnabled(false);

	{

        {
			m_ptcpClient = new QTcpSocket;
			connect(m_ptcpClient, SIGNAL(connected()),this, SLOT(ClientIsConnection()));
			QHostAddress qaddress;

			m_serverip = adminiEdit->text();
			m_serverport = tr("82") ;
			QStringList	alistparm  = m_serverip.split(QLatin1String(":"));
			if(alistparm.size()>=2)
			{
				m_serverip = alistparm.at(0);
				m_serverport = alistparm.at(1);
			}	

			if(m_serverip=="")
			{

                qaddress.setAddress(QLatin1String("192.168.1.2"));//192.168.1.2"));
                m_serverip = QLatin1String("192.168.1.2");//192.168.1.2");
            //	qaddress.setAddress(QLatin1String("192.168.1.2"));
            //	m_serverip = QLatin1String("192.168.1.2");
				m_serverport = tr("82");
            //	ShowLog(0,tr("connectaddress:192.168.1.2:82"));
                ShowLog(0,tr("connectaddress:192.168.1.2:82"));
			}
			else 
			{
				qaddress.setAddress(m_serverip);
				ShowLog(0,tr("connectaddress:")+m_serverip+tr(":")+m_serverport);
			}

			CxConnection *pconnect = GetActiveConnect();
			if(NULL==pconnect)  
				SetActiveConnect(pconnect);
			if(pconnect)
				pconnect->reconnect(m_ptcpClient,0);

			if(m_serverport!="")
				pconnect->connectToHost(qaddress,m_serverport.toInt());
			else
				pconnect->connectToHost(qaddress,82);


			const int Timeout = 10 * 1000;

			if (!m_ptcpClient->waitForConnected(Timeout)) 
			{
				pconnect->overconnect();

				ClientstartButton->setEnabled(true);
				clientStatusLabel->setText(tr("No Connected"));
				ShowLog(0,tr("retry No Connected"));
				reconnectTimer.start();
				return;
			}
			else
				pconnect->connectisok();
		}
	} 
	//ClienttransferButton->setEnabled(false);

	setWindowTitle(QString::fromLatin1("CxVisionAImini V5.8"));
}

void Dialog::clientStart()
{
    newconnect();
}

void Dialog::LoginUSER(CxConnection*pconnect)
{
	QString qstuser = pconnect->GetUser();

	CxConnectS aconnect;
	aconnect.m_pconnect = pconnect;
	aconnect.m_qsuser = qstuser;
	m_connections.push_back(aconnect);
	
	ReFlashUSER();


}
void Dialog::ReFlashUSER()
{
	widgetList2->clear();
	
	for(int i=0;i<m_connections.size();i++)
	{  
		CxConnection*pconnect = m_connections[i].m_pconnect;

		QString qstuser = pconnect->GetUser();
		
		//if(1==m_ics)//client //cxf
        if(0==pconnect->IScors())//client
            widgetList2->addItem(pconnect->getio_local_address_string()
                                 +tr(":%1").arg(pconnect->iolocalport())
                                 +tr(" ")+qstuser);
		else if(1==pconnect->IScors())//server
            widgetList2->addItem(pconnect->getio_local_address_string()
                                 +tr(":%1").arg(pconnect->iopeerport())
                                 +tr(" ")+qstuser);
	} 

	for(int i=0;i<m_P2Pconnections.size();i++)
	{  
		CxConnection*pconnect = m_P2Pconnections[i].m_pconnect;

		QString qstuser = pconnect->GetUser();

		if(0==pconnect->IScors())//client
            widgetList2->addItem(tr("[P2P]")+pconnect->getio_local_address_string()
                                 +tr(":%1").arg(pconnect->iopeerport())
                                 +tr(" ")+qstuser);
		else if(1==pconnect->IScors())
            widgetList2->addItem(tr("[P2P]")+pconnect->getio_peer_address_string()
                                 +tr(":%1").arg(pconnect->iopeerport())
                                 +tr(" ")+qstuser);

	} 

}
CxConnection*Dialog::GetConnect(int inum)
{
	for(int i=0;i<m_connections.size();i++)
	{  
		if(inum == i)
			return m_connections[i].m_pconnect;
	}
}

void Dialog::ServeracceptConnection()
{
    tcpServerConnection  = tcpServer.nextPendingConnection();
	
	CxConnection *pconnection = new CxConnection(tcpServerConnection,this,1);//server 1

	CxConnection *pconnect = GetActiveConnect();
    if(NULL==pconnect)
		SetActiveConnect(pconnect);

	 pconnection->connectisok();

	quint16 iport2 =  tcpServerConnection ->peerPort();
	QHostAddress hostadress2 =  tcpServerConnection ->peerAddress();
	QString hostname =  tcpServerConnection ->peerName();
	
	QString cxtest =QString(tr("Accepted connection: ")) + hostadress2.toString() + QString(tr(" Port: %1").arg(iport2));

	serverStatusLabel->setText(cxtest);

	ShowLog(0,cxtest); 
}

void Dialog::ServerConnection_Serial()
{

    m_pconnection_serial = new CxConnection_serial(this,1);//server 1


    quint16 iport2 = 82;
    QHostAddress hostadress2 = QHostAddress( QString("192.168.1.100"));

    QString cxtest =QString(tr("Accepted connection: ")) + hostadress2.toString() + QString(tr(" Port: %1").arg(iport2));

    serverStatusLabel->setText(cxtest);

    ShowLog(0,cxtest);
}
void Dialog::ClientConnection_Serial()
{

    m_ptcpClient = new QTcpSocket;

    clientStatusLabel->setText(tr("Connecting"));

    QHostAddress qaddress;

    m_serverip = adminiEdit->text();

    m_serverport = tr("82") ;

    QStringList	alistparm  = m_serverip.split(QLatin1String(":"));

    if(alistparm.size()>=2)
    {
        m_serverip = alistparm.at(0);
        m_serverport = alistparm.at(1);
    }

    if(m_serverip=="")
    {
        qaddress.setAddress(QLatin1String("192.168.1.2"));//192.168.1.2"));
        m_serverip = QLatin1String("192.168.1.2");//192.168.1.2");
    }
    else
        qaddress.setAddress(m_serverip);

    m_pconnection_serial = new CxConnection_serial(this,m_ptcpClient);//server 1

    m_pcurrentconnect = m_pconnection_serial;

    quint16 iport2 = 82;
    QHostAddress hostadress2 = QHostAddress( QString("192.168.1.100"));

    QString cxtest =QString(tr("Accepted connection: ")) + hostadress2.toString() + QString(tr(" Port: %1").arg(iport2));

    serverStatusLabel->setText(cxtest);

    ShowLog(0,cxtest);
}

void Dialog::ClientIsConnection()
{  

    quint16 iport2 =  m_ptcpClient->localPort();
	
	QHostAddress hostadress2 =  m_ptcpClient->localAddress();
 
	QString cxtest = hostadress2.toString() + QString(tr(" Port: %1").arg(iport2)) + QString(tr("connection be Accepted "))  ;

    serverStatusLabel->setText(cxtest);
 
}

void Dialog::SendMsg()
{
	m_qclientsend = CommandEdit->text();
	CxConnection *pconnect = GetActiveConnect();
	if(pconnect!=0)
		pconnect->sendMessage(m_qclientsend);
 
}


void Dialog::SendMessageToUser(const QString &qsmsg)
{
	CxConnection *pconnect = GetActiveConnect();
	if(pconnect!=0)
		pconnect->sendMessage(qsmsg);

}
void Dialog::SendComm(const QString &qcomm)
{ 	
    CxConnection *pconnect = GetActiveConnect();
	if(pconnect!=0) 
        AddDuty(pconnect,qcomm);
}

void Dialog::setEditText(const QString &str)
{
    CommandEdit->setText(str);
}
void Dialog::sendCommand()
{
	m_qclientsend = CommandEdit->text();

	QStringList	alist  =  m_qclientsend.split(QLatin1String(" "));	
	QString qcomm;
	QString qparm1;
	QString qparm2;
	QString qparm3;
	QString qparm4;
	QString qparm5;

	if(m_qclientsend==tr("isclient"))
	{
		//m_ics = 1;//client
		m_iconnecttype = 1;
		tcpServer.close(); 
		ServerstartButton->setEnabled(false);
		ClientstartButton->setEnabled(true);
		return;
		//ClienttransferButton->setEnabled(false);
	}
	else if(m_qclientsend==tr("pageconnect"))
	{
		m_iconnecttype = 2;
		return;
	}
	else if(m_qclientsend==tr("clearall"))
	{
        m_localdutylist.clear();
        widgetList3->clear();
		widgetList->clear();
        Clear();
        ClearAll();
        CollectionDataBufClear();
		return;
	}
	else if(m_qclientsend==tr("zip"))
	{
		Test1(0);
		return;
	}
	else if(m_qclientsend==tr("unzip"))
	{
		Test1(1);
		return;
	} 
	else if(m_qclientsend==tr("record"))
	{
		FILE * fpfile; 
			fpfile=fopen("record.txt","at");
			for(int i=0;i<m_textlist.size();i++)
				fprintf(fpfile,(m_textlist[i]+tr("\r\n")).toLocal8Bit());
			fclose(fpfile);  
	}
	else if(m_qclientsend==tr("local"))
	{
		Show_ConnectList();
		return;
	}		
	else if(m_qclientsend==tr("newc"))
	{  
		newconnect();//adminiEdit
		return;
    }
    else if(m_qclientsend==tr("news"))
    {
        serverStart();
        //newserial();//adminiEdit
        //ServerConnection_Serial();
        return;
    }
    else if(m_qclientsend==tr("newss"))
    {
        //newserial();//adminiEdit
        ServerConnection_Serial();
        return;
    }
    else if(m_qclientsend==tr("newsc"))
    {
        ClientConnection_Serial();
        return;
    }
    else if(m_qclientsend==tr("serialtransf"))
    {
        m_pconnection_serial->transforstart();
        return;
    }
    else if(m_qclientsend==tr("serialfile"))
    {
        sendserialfile();//adminiEdit
        return;
    }
    else if(m_qclientsend==tr("serialdir"))
    {
        senddirfile();
        return;
    }
    else if(m_qclientsend==tr("sfilex"))
    {
        sendserialfilex();
        return;
    }
    else if(m_qclientsend==tr("pageimage"))
    {
#ifdef BrowWin
        m_image = m_pageviewlist[0]->GetFramQImage();

        m_pmanager->setimage(m_image);
#endif
    }
    else if(m_qclientsend==tr("desktopimage"))
    {
#ifdef BrowWin
        m_image = QPixmap::grabWindow(QApplication::desktop()->winId()).toImage();
        m_pmanager->setimage(m_image);
#endif
        return;
    }
    else if(m_qclientsend==tr("trigger"))
    {
        m_pmanager->trigger(m_image);
        //m_pmanager->update();
        //m_scrollArea->update();
        return;
    }
    else if(m_qclientsend==tr("autosave"))
    {
        m_pmanager->saveautocreate();

        ShowLog(0,tr("autosave finish"));
        return;
    }
    else if(m_qclientsend==tr("testparser"))
    {
        m_pmanager->runstringcode(m_editor->toPlainText().toStdString().c_str());
        //m_pmanager->update();
        //m_scrollArea->update();
        ShowLog(0,m_pmanager->getoutputstring());
        return;
    }
    else if(m_qclientsend==tr("autotestparser"))
    {
        m_pmanager->autorunstringcode(m_editor->toPlainText().toStdString().c_str());
        ShowLog(0,m_pmanager->getoutputstring());
        return;
    }
    else if(m_qclientsend==tr("runparser"))
    {
        m_pmanager->collectionstringopt(m_editor->toPlainText().toStdString().c_str(),"run");
        ShowLog(0,m_pmanager->getoutputstring());
        return;
    }
    else if(m_qclientsend==tr("pagelink"))
    {
#ifdef BrowWin
       QUrl qurl0 = m_pageviewlist[0]->currentTab()->url();
       QStringList astrlist = CreateurlList(m_pageviewlist[0]->GetLink("a","href"),qurl0);
       CreateDateList(astrlist,m_igaptime,m_igaptime);//+100s begin and gap 100
#endif
       return;
    }
    else if(m_qclientsend==tr("mouselink"))
    {
     #ifdef BrowWin
        m_pmanager->runstringcode(m_editor->text().toStdString().c_str());
        ShowLog(0,m_pmanager->getoutputstring());

       QStringList astrlist = m_pageviewlist[0]->getlist();

       CreateDateListX(astrlist,m_igaptime,m_igaptime);//+100s begin and gap 100
       #endif   return;
    }
	else if(m_qclientsend==tr("sqlstop"))
	{
		m_pagesruntimer.stop();
		ShowLog(0,tr("sqlrun stop"));
			return;
	}
	else if(m_qclientsend==tr("sqlquery"))
	{
		m_pagesnum = -1;
		m_pagesruntimer.start(2000, this);
		ShowLog(0,tr("sqlrun start"));
		return;
	}
	else if(m_qclientsend==tr("test"))
	{
        QString qstr = QString("http://ftp.debian.org/debian/pool/main/?C=S;O=A");
        QString qstr1 = QString("http://ftp.debian.org/debian/pool/main/");
        m_pstringai->loadurltreefiletonode(qstr);

        //QApplication::desktop();
        
        

		return;

    }
	if(alist.size()>=2)
	{
		qcomm = alist.at(0);
		qparm1 = alist.at(1);
		if(qcomm==tr("psize"))
		{
			int ipmsize = qparm1.toInt();
            SetPieceSize(ipmsize);
             ShowLog(0,tr("set psize ") + qparm1);
            return;
		}
        else if(qcomm==tr("savefiledir"))
        {
            m_savefiledir = qparm1;
            ShowLog(0,tr("savefiledir ") + m_savefiledir);
            return;
        }
        else if(qcomm==tr("serialmsg"))
        {
            m_pconnection_serial->sendCommand(qparm1);//adminiEdit
            return;
        }
        else if(qcomm==tr("sfile"))
        {
             m_pconnection_serial->sendfile(qparm1);
             return;
        }
		else if(qcomm==tr("saveset"))
		{
			int isetnum = qparm1.toInt();
			setrequeststring(isetnum);
			ShowLog(0,tr("set save date image %1").arg(isetnum));
			return;
		}
		else if(qcomm==tr("dtimer"))
        {
			return;
		}	
		else if(qcomm==tr("deletec"))
		{  
			deleteconnect(qparm1);
			return ;
        }
        else if(qcomm==tr("run"))
        {
            m_cxvisiontimer.start(500,this);
            return ;
        }
        else if(qcomm==tr("stop"))
        {
            m_cxvisiontimer.stop();
            return ;
        }

		else if(qcomm==tr("touser"))
		{
			CxConnection*p1 = GetUserConnect(qparm1,0);
			ShowLog(p1,tr("<-current"));
			if(p1!=NULL)
				SetActiveConnect(p1);
			return;
		}
		else if(qcomm==tr("selco"))
		{
			CxConnection*p1 = GetConnect(qparm1.toInt());
			ShowLog(p1,tr("<-current"));
			if(p1!=NULL)
				SetActiveConnect(p1);
			return;
		}

        else if(qcomm==tr("url"))
        {

            return;
        }
        else if(qcomm==tr("runparser"))
        {
            m_pmanager->collectionstringopt(m_editor->toPlainText().toStdString().c_str(),
                     qparm1.toStdString().c_str());

            ShowLog(0,m_pmanager->getoutputstring());
            return;
        }
        else if(qcomm==tr("autorun"))
        {
            m_pmanager->autorun(qparm1.toStdString().c_str());
             ShowLog(0,m_pmanager->getoutputstring());
            return;
        }
        else if(qcomm==tr("runopt"))
        {
            m_pmanager->runoptcode(qparm1.toStdString().c_str());

            ShowLog(0,m_pmanager->getoutputstring());

            return;
        }
        else if(qcomm==tr("setvalue"))
        {
            qparm2 = alist.at(2);
            m_pmanager->SetParserValue(qparm1,qparm2.toDouble());
            ShowLog(0,m_pmanager->getoutputstring());
            return;
        }

        else if(qcomm==tr("getvalue"))
        {
           double dvalue = m_pmanager->GetParserValue(qparm1);

            ShowLog(0,tr("%1").arg(dvalue));
             return;
        }
        else if(qcomm==tr("loadparser"))
        {
            QString strname = "./"+qparm1+".cxut";
            QString strfile = getlocationstringx(strname.toStdString().c_str());
            QString strcode = loadfilestring(strfile);
            m_editor->setText(strcode);
              ShowLog(0,tr("load ok"));
               return;
        }
        else if(qcomm==tr("saveparser"))
        {
           QString strname = "./"+qparm1+".cxut";
            m_pmanager->saveeditcode(m_editor->toPlainText().toStdString().c_str()
                                     ,strname.toStdString().c_str()
                                     );
            ShowLog(0,tr("save ok"));
            return;
        }
        else if(qcomm==tr("download"))
        {
            QString qstr =m_qclientsend.mid(9);
            downloadlink(qstr);
            //if(0!=m_pdownload)
                //m_pdownload->append(QUrl(m_qclientsend.mid(9).toStdString().c_str()));
        }
        else  if(qcomm==tr("newc"))
        {
            adminiEdit->setText(qparm1);
            newconnect();//adminiEdit
            return;
        }
        else if(qcomm==tr("mouse"))
        {
            qparm2 = alist.at(2);
            int ix = qparm1.toInt();
            int iy = qparm2.toInt();
            QPoint apoint(ix,iy);
      
            return;
        }
        else if(qcomm==tr("showlist"))
        {

            return;
        }

    }
	CxConnection * pconnect = GetActiveConnect();
	if(NULL!=pconnect)
		AddDuty_front(pconnect,m_qclientsend);
}
void Dialog::viewparserlist(const char *pchar)
{
 
}
void Dialog::viewrunjavascript(const char *pchar)
{
 

}

void Dialog::AddDutyfile()
{
    //local duty
    QFile file(getlocationstringx("./duty.txt"));

    if (file.open(QIODevice::ReadOnly))
    {
        QString line;
        QString strdate;
        QTextStream in(&file);
        while (!in.atEnd())
        {
            line = in.readLine();
            int i=0;
            for(i = 0;i<line.size() ;i++)
            {
                QChar achar = line.at(i);
                if(achar=='|')
                {
                    goto ADDDUTYNEXT;
                }
            }

ADDDUTYNEXT:
            strdate = line.mid(0,i);
            if(i<line.size())
                line = line.mid(i+1);
            AddLoacalDuty(line,-2,strdate);
        }
    }

{
    QFile file(getlocationstringx("./cduty.txt"));

    if (file.open(QIODevice::ReadOnly))
    {
        QString line;
        QString strdate;
        QTextStream in(&file);
        while (!in.atEnd())
        {
            line = in.readLine();
            int i=0;
            for(i = 0;i<line.size() ;i++)
            {
                QChar achar = line.at(i);
                if(achar=='|')
                {
                    goto ADDDUTYNEXT0;
                }
            }
ADDDUTYNEXT0:
            strdate = line.mid(0,i);
            if(i<line.size())
                line = line.mid(i+1);
            CxConnection *pconnect = GetActiveConnect();
            if(NULL!=pconnect)
                AddDuty(pconnect,line,strdate);
        }
    }
}

}
void Dialog::sendDuty()
{
	QString fileName =
		QFileDialog::getOpenFileName(this, tr("Open File"),
		QDir::currentPath(),
		tr("ALL Files (*.*)"));
	if (fileName.isEmpty())
		return;
	QFile file(fileName);

	if (file.open(QIODevice::ReadOnly)) 
	{
		QString line;
		QString strdate;
		QTextStream in(&file);
		while (!in.atEnd()) 
		{
			line = in.readLine();
            int i=0;
            for(i = 0;i<line.size() ;i++)
			{
				QChar achar = line.at(i);
				if(achar=='|')
				{
					goto ADDDUTYNEXT;
				}
			}
			
ADDDUTYNEXT:
			strdate = line.mid(0,i);
			if(i<line.size())
				line = line.mid(i+1);

            AddLoacalDuty(line,-1,strdate);
		}
	}
}
void Dialog::LoginOK()
{
	m_ilogin = 1;

	ShowLog(0,tr("[login ok]"));
}
void Dialog::SetClear()
{
	Clear();
}
void Dialog::SendPiece()
{
    CxConnection * pactconnect = GetActiveConnect();
    QString  user ;
    if(0!=pactconnect)
    {
       user = pactconnect->GetUser();
       m_recorduser = user;
    }
    else
    {
        return ;
    }

    Send_LoadFile_Multi(user);
}


void Dialog::ServerGetFile()
{

}
void Dialog::ClientGetFile()
{

} 
void Dialog::updateProgress(const QString &qstrgos,qint64 numBytes,qint64 totalBytes)
{ 
    clientProgressBar->setMaximum(totalBytes);
    clientProgressBar->setValue(numBytes);
    clientStatusLabel->setText(qstrgos + tr(" %1MB").arg(numBytes / (1024 * 1024)));
}
QByteArray Dialog::loadfilestring(const QString& file) const
{
	QFile f(file);

	QByteArray qfilereads;

	const int size = f.size();
	if ( !f.open(QFile::ReadOnly) )
	{
		return QByteArray();
	}
	if ( size < 500000 )
	{
		QByteArray d = f.readAll();
		f.close();
		return d;
	} 
	else
	{
		int count = 0;
		QByteArray ba;
		do
		{
			ba = f.read(100000);
			count += ba.count();
			qfilereads += ba ;
		} while ( (count < size) && ba.count() );
		f.close();
		return qfilereads;
	}
}
void Dialog::CodeViewIni()
{
}
void Dialog::SaveParser()
{
    QString strcur = m_pmanager->getcurrentstatename();
    QString strname = "./"+strcur+".cxut";
    m_pmanager->saveeditcode(m_editor->toPlainText().toStdString().c_str()
                              ,strname.toStdString().c_str()
                              );
    ShowLog(0,strname + QString(tr(" save ok")));
}
void Dialog::TestParser()
{
    m_pmanager->runstringcode(m_editor->toPlainText().toStdString().c_str());
    ShowLog(0,m_pmanager->getoutputstring());
}
void Dialog::TestParserX()
{
    m_pmanager->runstringcodex(m_editor->toPlainText().toStdString().c_str());
    ShowLog(0,m_pmanager->getoutputstring());
}
void Dialog::SetCommandEditText(QString &qstr)
{
    CommandEdit->setText(qstr);
}
void Dialog::SetTextEditText(QString &qstr)
{
    SetEdit->setText(qstr);
}
void Dialog::SetValueEditText(QString &qstr)
{
    ValueEdit->setText(qstr);
}
void Dialog::SetValue()
{
    m_pmanager->SetParserValue(SetEdit->text(),ValueEdit->text().toDouble());
    ShowLog(0,m_pmanager->getoutputstring());
}
void Dialog::GetValue()
{
   double dvalue = m_pmanager->GetParserValue(SetEdit->text());
   ShowLog(0,tr("%1").arg(dvalue));
   ValueEdit->setText(tr("%1").arg(dvalue));
}
void Dialog::savefilestring(const QString& fn)
{
	QFile f(fn);

	if ( !f.open(QFile::WriteOnly) )
	{
		return;
	}

	f.write(m_databuf);
	f.close();
}
void Dialog::getonepiecefile(int icp,int ipsize,int itolsize,const QString &qname,const QString &qid,const QByteArray &qcontent)
{
	m_piecefile.m_istatus[icp] = -1;
	m_piecefile.icurpiece = icp;
	m_piecefile.pieceLength = ipsize;
	m_piecefile.totallength = itolsize;
	m_piecefile.qsname = qname;
	m_piecefile.qsid = qid;
	m_piecefile.m_qcontents[icp] = qcontent;

}
int Dialog::getonepiecefilex(int icp,int ipsize,int itolsize,const QByteArray &qname,const QByteArray &qid,const QByteArray &qcontent)
{

    int imaxlen = 0;
    if(ipsize>0)
        imaxlen = (itolsize/ipsize) + ((itolsize%ipsize)>0?1:0);
    else
        imaxlen = 1;

    int inum = 0;

    int ipicfilesize = m_piecefilelist.size();


    ipicfilesize = m_piecefilelist.size();
    int icurfilenum  = -1;
    for(int i=0;i<ipicfilesize;i++)
    {
        if(m_piecefilelist[i].qsname == qname)
        {
            icurfilenum = i;
        }
    }
    if(icurfilenum>-1)
    {
        if(m_piecefilelist[icurfilenum].m_istatus[icp]!= -1)
        {
            m_piecefilelist[icurfilenum].m_istatus[icp] = -1;
            m_piecefilelist[icurfilenum].icurpiece = icp;
            m_piecefilelist[icurfilenum].pieceLength = ipsize;
            m_piecefilelist[icurfilenum].totallength = itolsize;
            m_piecefilelist[icurfilenum].qsname = qname;
            m_piecefilelist[icurfilenum].qsid = qid;
            m_piecefilelist[icurfilenum].m_qcontents[icp] = qcontent;
            m_piecefilelist[icurfilenum].m_isfinish = 0;
        }
        for(int i=0;i<imaxlen;i++)
        {
            if(-1==m_piecefilelist[icurfilenum].m_istatus[i])
            {
                inum = inum + 1;
            }
        }
    }
    else
    {
        CxPieceFile apiecefile;
        m_piecefilelist.push_back(apiecefile);
        icurfilenum = 0;
        m_piecefilelist[0].m_istatus[icp] = -1;
        m_piecefilelist[0].icurpiece = icp;
        m_piecefilelist[0].pieceLength = ipsize;
        m_piecefilelist[0].totallength = itolsize;
        m_piecefilelist[0].qsname = qname;
        m_piecefilelist[0].qsid = qid;
        m_piecefilelist[0].m_qcontents[icp] = qcontent;
        m_piecefilelist[0].m_isEND = 0;
        m_piecefilelist[0].m_isfinish = 0;
        inum = 1;

    }
//
    if(inum>=imaxlen)
    {
        if(0==m_piecefilelist[icurfilenum].m_isfinish)
        {
            m_piecefilelist[icurfilenum].m_isfinish = 1;

            QByteArray getbuf;
            int itolsize = m_piecefilelist[icurfilenum].totallength;
            int ipsize =m_piecefilelist[icurfilenum].pieceLength;
            int ipiecenum = itolsize/ipsize + (itolsize%ipsize>0?1:0);
            for(int i=0;i<ipiecenum;i++)
            {
               getbuf  += m_piecefilelist[icurfilenum].m_qcontents[i];
            }
            //
            QByteArray igettype;
            QByteArray igetstring;
            for(int i = 0;i<qname.size() ;i++)
            {
                QChar achar = qname.at(i);
                if(achar=='_')
                {
                    igettype = qname.mid(0,i);
                    igetstring = qname.mid(i+1);
                    break;
                }
            }
            if(igettype=="file")
            {
                Get_SaveFileSend(igetstring,getbuf);
                inum = 99999999;
                m_piecefilelist[icurfilenum].m_istatus.clear();
                m_piecefilelist[icurfilenum].m_qcontents.clear();
                m_piecefilelist[icurfilenum].m_isEND = 1;

            }
            else if(igettype=="Tfile")
            {
               int iends = Get_SaveTFileSend(igetstring,getbuf);

               if(0==iends)
               {
                   inum = 99999999;

                   if(m_getfiles.size()>4)
                   {
                       ShowLog(0,tr("clear get file map"));
                       m_getfiles[0].m_filename.clear();
                       m_getfiles[0].m_istatus.clear();
                       m_getfiles[0].m_bwrite.clear();
                       m_getfiles[0].m_getfilebytes.clear();
                       m_getfiles.pop_front();

                   }

                   m_piecefilelist[icurfilenum].m_istatus.clear();
                   m_piecefilelist[icurfilenum].m_qcontents.clear();
                   m_piecefilelist[icurfilenum].m_isEND = 1;
               }
                else if(1==iends)
                {
                   inum = 90000000;

                   if(m_getfiles.size()>0)
                   {
                       m_getfiles[0].m_filename.clear();
                       m_getfiles[0].m_istatus.clear();
                       m_getfiles[0].m_bwrite.clear();
                       m_getfiles[0].m_getfilebytes.clear();
                       m_getfiles.pop_front();

                   }

                   m_piecefilelist[icurfilenum].m_istatus.clear();
                   m_piecefilelist[icurfilenum].m_qcontents.clear();
                   m_piecefilelist[icurfilenum].m_isEND = 1;
               }
               else if(2==iends)
                   inum = 90000001;
               else if(10==iends)
                   inum = 90000010;
               else inum = 90000002;
            }
            else if(igettype=="image")
            {
                Get_SaveImageSend(igetstring,getbuf);
                inum = 99999999;
                m_piecefilelist[icurfilenum].m_istatus.clear();
                m_piecefilelist[icurfilenum].m_qcontents.clear();
                m_piecefilelist[icurfilenum].m_isEND = 1;
            }
            else if(igettype=="imageframe")
            {
                Get_SaveImageFrameSend(igetstring,getbuf);
                inum = 99999999;
                m_piecefilelist[icurfilenum].m_istatus.clear();
                m_piecefilelist[icurfilenum].m_qcontents.clear();
                m_piecefilelist[icurfilenum].m_isEND = 1;

            }
            else
            {
                inum = 99999999;
            }
            ////////////

            int ipicfilesize0 = m_piecefilelist.size();
            if(ipicfilesize0>5)
                m_piecefilelist.pop_front();
        }
        else
        {
            inum = 90000002;
        }
    }

    return inum;
}

inline int Dialog::getpiecetransferwaittime(int icp)
{
    QString qstime = tr("%1").arg(Dialog::GetGolbalTime());

	int itimes = GetPieceFile()->m_istatus[icp];
	if(itimes>0)	
	{
		int icurtimes = qstime.toInt() - itimes ;
		return icurtimes;
	}
	else if(itimes==0)
	{
		GetPieceFile()->m_istatus[icp] = qstime.toInt() - 1;
		return 0;	
	}
 
}
inline int Dialog::piececanbetransfer(int icp)//1 can be transfer, 0 no be transfer
{ 
	int icsize = m_piecefile.m_qcontents[icp].size();
	if(icsize>0)
		return 0;
	int ipasstime = getpiecetransferwaittime(icp);
	if(0== ipasstime)
		return 1;
	if(ipasstime>30)//transfer over 30 s no finish to retransfer
	{
		return 1;
	} 
	else 
		return 0;
}

int Dialog::getfilefinish()
{ 
	return	m_piecefile.m_isfinish ;
}
void Dialog::settransferover()
{
	m_piecefile.m_isfinish = 2;
}
int Dialog::getnextpiecenum()
{ 
	int icurpiece = m_piecefile.icurpiece;
	int istatus = piececanbetransfer(icurpiece+1);//m_piecefile.m_qcontents[icurpiece+1].size();
	if(istatus!=1)
	{
		int itolsize = m_piecefile.totallength;
		int ipsize =m_piecefile.pieceLength;

		int ipiecenum = itolsize/ipsize + (itolsize%ipsize>0?1:0);

		for(int i=0;i<ipiecenum;i++)
		{ 
			istatus =  piececanbetransfer(i);//m_piecefile.m_qcontents[i].size();
			if(istatus!=1)
				return i;
		}

		m_piecefile.m_isfinish = 1;
		return -1;
	}
	else
		return icurpiece+1;
}
int Dialog::getfinishnum(int itotalnum)
{ 
	int inum = 0;
	for(int i=0;i<itotalnum;i++)
	{
		if(-1==m_piecefile.m_istatus[i])
		{
			inum = inum + 1;
		}
	}
	return inum;
}
void Dialog::Get_SaveFileSend(const QString &qname,const QByteArray&databuf  )
{
    m_databuf =  qUncompress(databuf);

	QFileInfo afile(qname);
	
	QString astrfile = afile.fileName();
	QString endName = afile.suffix(); 
	QString getfilename = afile.completeBaseName()+tr(".")+afile.completeSuffix();
	if(afile.completeBaseName()!=tr("") )
	{
		savefilestring(getlocationstringx("./file/"+getfilename));	
	}
	else
	{
		QDateTime now = QDateTime::currentDateTime();
		QString qyear = now.toString(QLatin1String("yyyy"));
		QString qmh = now.toString(QLatin1String("MM"));
		QString qdy = now.toString(QLatin1String("dd"));
 
		QString qfilenamex = qyear + tr("_") +qmh + tr("_") +qdy + QTime::currentTime().toString("_hh_mm_ss");
		savefilestring(getlocationstringx("./file/"+qfilenamex));	
	}
}
void Dialog::Get_SaveDateFileSend(const QString &qname )
{
	int itolsize = m_piecefile.totallength;
	int ipsize =m_piecefile.pieceLength;

	if(itolsize==0
		||ipsize==0)
		return;
		
	int ipiecenum = itolsize/ipsize + (itolsize%ipsize>0?1:0);
	m_databuf.clear();
	for(int i=0;i<ipiecenum;i++)
	{
		m_databuf += m_piecefile.m_qcontents[i];	
	}

	m_piecefile.icurpiece = 0;
	m_piecefile.pieceLength = 0;
	m_piecefile.totallength = 0;
	m_piecefile.qsname = "";
	m_piecefile.qsid ="";
	m_piecefile.m_qcontents.clear();
	m_piecefile.m_istatus.clear();
	m_piecefile.m_isfinish = 0;

	m_databuf =  qUncompress(m_databuf);

	QFileInfo afile(qname);

	QString astrfile = afile.fileName();
	QString endName = afile.suffix(); 
	QString getfilename = afile.completeBaseName()+tr(".")+afile.completeSuffix();

	{
		QDateTime now = QDateTime::currentDateTime();
		QString qyear = now.toString(QLatin1String("yyyy"));
		QString qmh = now.toString(QLatin1String("MM"));
		QString qdy = now.toString(QLatin1String("dd"));
		int ih = QTime::currentTime().hour();
		int im = QTime::currentTime().minute();
		int is = QTime::currentTime().second();	
		QString qfilenamex = qyear + tr("_") +qmh + tr("_") +qdy + QTime::currentTime().toString("_hh_mm_ss_zzz")+getfilename;
		savefilestring(getlocationstringx("./file/"+qfilenamex));

	}
}
void Dialog::savefile(const QByteArray &qcontent,const QString &qname)
{
	m_databuf =  qcontent;
	savefilestring(getlocationstringx("./file/"+qname));
}
void Dialog::Get_SaveImageSend(const QByteArray &qname,const QByteArray &databuf)
{ 
    QByteArray qbw;
    QByteArray qbh;
    int i_num = 0;
    int iprenum = 0;
    for(int i = 0;i<qname.size() ;i++)
    {
        QChar achar = qname.at(i);
        if(achar=='_')
        {
            if(0==i_num)
            {
                qbw = qname.mid(iprenum,i-iprenum);
                iprenum = i + 1;
                i_num = i_num + 1;
            }
            else if(1==i_num)
            {
                qbh = qname.mid(iprenum,i-iprenum);
                iprenum = i + 1;
                i_num = i_num + 1;
                goto NEXTRUN3;
            }
        }
    }
    if(i_num<=1)
        qbh = qname.mid(iprenum);
NEXTRUN3:
    int iw =qbw.toInt();
    int ih =qbh.toInt();

    QByteArray adatabuf =  qUncompress(databuf);
    QImage image((uchar*)adatabuf.data(),iw,ih,QImage::Format_RGB16);
 
    setimage(image);
    ShowLog(0,tr("getimage %1 x%2 ").arg(iw).arg(ih));
}
void Dialog::Get_SaveImageFrameSend(const QByteArray &qname,const QByteArray &databuf)
{
    QByteArray qbw;
    QByteArray qbh;
    int i_num = 0;
    int iprenum = 0;
    for(int i = 0;i<qname.size() ;i++)
    {
        QChar achar = qname.at(i);
        if(achar=='_')
        {
            if(0==i_num)
            {
                qbw = qname.mid(iprenum,i-iprenum);
                iprenum = i + 1;
                i_num = i_num + 1;
            }
            else if(1==i_num)
            {
                qbh = qname.mid(iprenum,i-iprenum);
                iprenum = i + 1;
                i_num = i_num + 1;
                goto NEXTRUN3;
            }
        }
    }
    if(i_num<=1)
        qbh = qname.mid(iprenum);
NEXTRUN3:
    int iw =qbw.toInt();
    int ih =qbh.toInt();

    QByteArray adatabuf =  qUncompress(databuf);
    QImage image((uchar*)adatabuf.data(),iw,ih,QImage::Format_RGB16);

    setimage(image);
    ShowLog(0,tr("getimage %1 x%2 ").arg(iw).arg(ih));



#ifdef CXVISIONIMAGEVIEW
			if(NULL!=m_imageViewer)
			{ 
				m_imageViewer->loadimageframe(image);
			} 
#else
             setimageframe(image);
#endif
             ShowLog(0,tr("getimageframe %1 x%2 ").arg(iw).arg(ih));

}
void Dialog::Set_Clear(CxConnection*pconnect)
{

	pconnect->Clear();
	
	m_piecefile.icurpiece = 0;
	m_piecefile.pieceLength = 0;
	m_piecefile.totallength = 0;
	m_piecefile.qsname = "";
	m_piecefile.qsid ="";
	m_piecefile.m_qcontents.clear();
	m_piecefile.m_istatus.clear();
	m_piecefile.m_isfinish = 0;
 
	QPainter painter(&m_image); 
	painter.fillRect(m_image.rect(), Qt::black);
	painter.end();
	QPainter painter1(&m_resultImage); 
	painter1.fillRect(m_resultImage.rect(), Qt::black);
	painter1.end();
	QPainter painter2(&m_ioldimage); 
	painter2.fillRect(m_ioldimage.rect(), Qt::black);
	painter2.end();
}
void Dialog::Get_ShowPageSend()
{ 
	int itolsize = m_piecefile.totallength; 
	int ipsize =m_piecefile.pieceLength;

	if(itolsize==0
		||ipsize==0)
		return;
	int ipiecenum = itolsize/ipsize + (itolsize%ipsize>0?1:0);
	m_databuf.clear();
	for(int i=0;i<ipiecenum;i++)
	{
		m_databuf += m_piecefile.m_qcontents[i];	
	}

	m_piecefile.icurpiece = 0;
	m_piecefile.pieceLength = 0;
	m_piecefile.totallength = 0;
	m_piecefile.qsname = "";
	m_piecefile.qsid ="";
	m_piecefile.m_qcontents.clear();
	m_piecefile.m_istatus.clear();
	m_piecefile.m_isfinish = 0;

	m_databuf =  qUncompress(m_databuf);	

}
void Dialog::Get_SqlSend(CxConnection*pconnect)
{

	
}
void Dialog::Get_SqlxSend(CxConnection*pconnect)
{
	int itolsize = m_piecefile.totallength; 
	int ipsize =m_piecefile.pieceLength;

	if(itolsize==0
		||ipsize==0)
		return;
	int ipiecenum = itolsize/ipsize + (itolsize%ipsize>0?1:0);
	
	m_databuf.clear();
	
	for(int i=0;i<ipiecenum;i++)
	{
		m_databuf += m_piecefile.m_qcontents[i];	
	}

	m_piecefile.icurpiece = 0;
	m_piecefile.pieceLength = 0;
	m_piecefile.totallength = 0;
	m_piecefile.qsname = "";
	m_piecefile.qsid ="";
	m_piecefile.m_qcontents.clear();
	m_piecefile.m_istatus.clear();
	m_piecefile.m_isfinish = 0;

	m_databuf =  qUncompress(m_databuf);	


	QString getliststr = m_databuf;

	ShowLog(0,getliststr);

	m_getlist = getliststr.split("\r\n") ;


}
void Dialog::SetLoop(int inum,int ivalue)
{
	m_pagesrunfinish[inum] = ivalue; 
}
void Dialog::SetDateLoop(int inum,int ivalue,QString &qdate,QString &qstring)
{
	m_pagesrunfinish[inum] = ivalue; 
	m_pagesrundate[inum] = qdate;
	m_pagestring[inum] = qstring;
}
void Dialog::ResetDateLoop(QString &qdate,int ivalue)
{
	if(m_pagesrundate[0]==qdate)
		m_pagesrunfinish[0] = ivalue; 
	if(m_pagesrundate[1]==qdate)
		m_pagesrunfinish[1] = ivalue; 
	if(m_pagesrundate[2]==qdate)
		m_pagesrunfinish[2] = ivalue; 
	if(m_pagesrundate[3]==qdate)
		m_pagesrunfinish[3] = ivalue; 
}
void Dialog::SetSQLRecord(QString &qdate)
 {



}
void Dialog::Get_SaveListSend()
{ 
	int itolsize = m_piecefile.totallength; 
	int ipsize =m_piecefile.pieceLength;

	if(itolsize==0
		||ipsize==0)
		return;
	int ipiecenum = itolsize/ipsize + (itolsize%ipsize>0?1:0);
	m_databuf.clear();
	for(int i=0;i<ipiecenum;i++)
	{
		m_databuf += m_piecefile.m_qcontents[i];	
	}

	m_piecefile.icurpiece = 0;
	m_piecefile.pieceLength = 0;
	m_piecefile.totallength = 0;
	m_piecefile.qsname = "";
	m_piecefile.qsid ="";
	m_piecefile.m_qcontents.clear();
	m_piecefile.m_istatus.clear();
	m_piecefile.m_isfinish = 0;

	m_databuf =  qUncompress(m_databuf);	

 
	QString getliststr = m_databuf;
	
	ShowLog(0,getliststr);
	
	m_getlist.append(getliststr.split("\r\n")) ;
	m_getlist.removeDuplicates();

}
void Dialog::Run_Clear()
{
	ShowLog(0,tr("List Clear"));
	m_getlist.clear();
}
void Dialog::Run_PageElementClassic(const QString & qsfilesuffix,const QString & qsopt,const QString & qduty1,const QString & qduty2)
{
	QStringList filelist;
	//filter
	for(int i=0;i<m_getlist.size();i++)
	{ 
		QFileInfo qfileinf(m_getlist[i]); 
		QString qname = qfileinf.fileName();
		QString endName = qfileinf.suffix(); 

		if(endName==qsfilesuffix)
		{
			filelist.append(m_getlist[i]);
			ShowLog(0,qsfilesuffix+tr(":")+m_getlist[i]);
		}
	}
	//add duty
	for(int i=0;i<filelist.size();i++)
	{	
		CxConnection * pactconnect =GetActiveConnect();
		if(filelist[i]!=tr(""))
		{
			AddDuty(pactconnect,qsopt+tr(" ")+filelist[i]);
			if(qduty1!=tr(""))
			{
				AddDuty(pactconnect,qduty1);
				if(qduty2!=tr(""))
				{
					AddDuty(pactconnect,qduty2); 
				}
			}
		}
	}	 
}


void Dialog::Send_LoadFile(CxConnection *pconnect)
{
    QString fileName =
        QFileDialog::getOpenFileName(this, tr("Open File"),
        QDir::currentPath(),
        tr("ALL Files (*.*)"));

    if (fileName.isEmpty())
        return;

    pconnect->setbufx(loadfilestring(fileName)) ;

    Send_File(pconnect,fileName,pconnect->getbufx());

}

void Dialog::Send_LoadFile_Serial(CxConnection_serial*pconnect)
{
	QString fileName =
		QFileDialog::getOpenFileName(this, tr("Open File"),
		QDir::currentPath(),
		tr("ALL Files (*.*)"));
	if (fileName.isEmpty())
		return;

    pconnect->setbufx(loadfilestring(fileName)) ;

    pconnect->packagedata(pconnect->getbufx());

    pconnect->writeData(pconnect->getbufx());

}
void Dialog::Send_LoadFile_Multi(const QString &user,const QString &qsdate)
{
	QString fileName =
		QFileDialog::getOpenFileName(this, tr("Open File"),
		QDir::currentPath(),
		tr("ALL Files (*.*)"));
	if (fileName.isEmpty())
		return;

	Send_File_Multi(user,fileName,loadfilestring(fileName),qsdate);

}

void Dialog::PageFinish(const QString &qname,int itype,BrowserWindow*pview)
{
	CxConnection *pconnect = geturlconnect(qname);
	QString sgetdate = geturldate(qname);
	ShowLog(pconnect,"PageFinish "+ qname);
	if(pconnect!=NULL)
	{
		if(1==pconnect->IScors())//server
		{ 	
			pconnect->AddMsg("pagefinish "+ sgetdate+tr(":") +qname);
		}
	}
	else
	{	
		if(1==pconnect->IScors())//server
		{	
			if(m_localdutylist.size()>0)
				m_localdutylist.pop_front();
		}
	}
	removeurllist(qname);
}
void Dialog::PageSQLFinish(const QString &qname,int itype,BrowserWindow*pview)
{
	CxConnection *pconnect = geturlconnect(qname);
	QString sgetdate = geturldate(qname);
	ShowLog(pconnect,"PageFinish "+ qname);
	if(pconnect!=NULL)
	{ 
		if(1==pconnect->IScors())//server
		{ 
			if(getrequeststring()==tr("datefile_"))
			{
				Send_PageSQL_MultiEx(pconnect->GetUser(),pview,itype,"",sgetdate);
				Send_PageView_Multi(pconnect->GetUser(),pview,sgetdate);
			}
			else if(getrequeststring()==tr("list_"))
				Send_PageSQL_MultiEx(pconnect->GetUser(),pview,itype,"",sgetdate);
			else 
				Send_PageSQL_MultiEx(pconnect->GetUser(),pview,itype,"",sgetdate);
		}
	}
 
	removeurllist(qname);
}
void Dialog::PageImageFinish(const QString &qname ,int itype,BrowserWindow*pview)
{
	CxConnection *pconnect = geturlconnect(qname);
	QString sgetdate = geturldate(qname);
	ShowLog(pconnect,"PageFinish "+ qname);
	if(pconnect!=NULL)
	{  
		if(1==pconnect->IScors())//server
		{  
			Send_PageView_Multi(pconnect->GetUser(),pview,sgetdate);
		}
	}

	removeurllist(qname);
}
void Dialog::PageSQLFinish_Local(const QString &qname,int itype,BrowserWindow*pview)
{
	CxConnection *pconnect = geturlconnect(qname);
	QString sgetdate = geturldate(qname);
	ShowLog(pconnect,"PageFinish "+ qname);


}
void Dialog::DownloadFinish(const QString &qname)
{
	CxConnection *pconnect = geturlconnect(qname);
	QString sgetdate = geturldate(qname);

	ShowLog(pconnect,"DownloadFinish:"+ qname);
	if(pconnect!=NULL)
	{
		if(geturlbuf(qname)->size()>0)
		{

			sethttpreqstring(*geturlbuf(qname));
			Send_Request_Multi(pconnect->GetUser(),qname,*geturlbuf(qname),sgetdate);
		}
	}
	else
	{ 
		sethttpreqtype(0);
		savefile((*geturlbuf(qname)),"temp.html"); 
	}
	removeurllist(qname);
}
void Dialog::DownloadSQLFinish(const QString &qname,const QString &sdate)
{
	CxConnection *pconnect = geturlconnect(qname);
	QString sgetdate = geturldate(qname); 

	ShowLog(pconnect,"Download SQL Finish:"+ qname);
	if(pconnect!=NULL)
	{
		if(geturlbuf(qname)->size()>0)
		{

			sethttpreqstring(*geturlbuf(qname));

			//Send_Request(pconnect,qname,*geturlbuf(qname));
			Send_Request_Multi(pconnect->GetUser(),qname,*geturlbuf(qname),sdate);
		}
	}
	else
	{ 
		sethttpreqtype(0);
		savefile((*geturlbuf(qname)),"temp.html"); 
	}
	removeurllist(qname);
}
void Dialog::appendreqbuf(const QString &qstrurl,const QByteArray &buff)
{ 
	QByteArray *preqbuff = geturlbuf(qstrurl);
 
	preqbuff->append(buff);
}
void Dialog::GSearch(CxConnection*pconnect,int itype,const QString &qstrselect)
{

}
void Dialog::timerEvent(QTimerEvent *event)
{
   
        QObject::timerEvent(event);
	
}
void Dialog::SetPosition(int x, int y, int w, int h)
{
	setGeometry( x, y, w, h);
}
void Dialog::SqlTimerRun()
{
	CxConnection *pconnect = GetActiveConnect();
	if(NULL==pconnect) 
	{
		return;
	}
}
void Dialog::setrequeststring(int itype)
{
	switch(itype)
	{
	default:
	case 0:
		m_reqstring =tr("page_");	
		break;
	case 1:
		m_reqstring =tr("file_");	
		break;
	case 10:
		m_reqstring =tr("datefile_");	
		break;
	case 2:
		m_reqstring =tr("image_");	
		break;
	case 3:
		m_reqstring =tr("list_");	
		break;
	case 4:
		m_reqstring =tr("imageframe_");	
		break;
	case 5:
		m_reqstring =tr("sql_");	
		break;
	}
}
void Dialog::SetCodeText(const QString &strcode)
{
    m_editor->setText(strcode);
}

void Dialog::Send_Picture(CxConnection*pconnect)
{

}
void Dialog::Send_Picture_Multi( const QString &user,BrowserWindow*pview,const QString &qsdate)
{
}

void Dialog::Set_FrameView(int inum ,int isx,int isy,int ivw,int ivh)
{


}
void Dialog::Send_Page(CxConnection*pconnect)
{  

}

void Dialog::Send_Page_Multi(const QString &user,BrowserWindow*pview ,const QString &qsdate)
{  

}


void Dialog::Send_PageList(CxConnection*pconnect,int itype,const QString &qstrselect )
{
}


void Dialog::Send_PageSQL_MultiEx(const QString &user,BrowserWindow*pview ,int itype,const QString &qstrselect,const QString &qsdate)
{
}

void Dialog::Send_PageView_Multi( const QString &user,BrowserWindow*pview,const QString &qsdate)
{ 

	if(NULL==pview)
		pview =  m_pageviewlist[0];

	if(NULL==pview)
		return;

}

void Dialog::Send_PageSQL_Multi(const QString &user,int itypex,const QString &qstrselect ,BrowserWindow*pview,const QString &qsdate)
{
}
void Dialog::Send_PageList_Multi(const QString &user,int itype,const QString &qstrselect ,BrowserWindow*pview ,const QString &qsdate)
{
}

void Dialog::C2C(CxConnection*pconnect,CxConnection*pconnect2)
{
    QString qsip4= pconnect->getio_peer_address_string();
	QStringList	alistparm  = qsip4.split(QLatin1String("."));	
	int ip1 =  alistparm.at(0).toInt();
	int ip2 =  alistparm.at(1).toInt();
	int ip3 =  alistparm.at(2).toInt();
	int ip4 =  alistparm.at(3).toInt();
    int iport =  pconnect->iopeerport();

    qsip4= pconnect->iopeerport();
	alistparm  = qsip4.split(QLatin1String("."));	
	int ip_1 =  alistparm.at(0).toInt();
	int ip_2 =  alistparm.at(1).toInt();
	int ip_3 =  alistparm.at(2).toInt();
	int ip_4 =  alistparm.at(3).toInt();
	
    int iport2 =  pconnect->iopeerport();

	pconnect->stoptimer();
	pconnect2->stoptimer();
 
	pconnect->sendpublicK(ip4,iport,ip_1,ip_4,iport2);
	pconnect2->sendpublicK(ip4,iport,ip_1,ip_4,iport2);
	//////////////////////////////////////////////////////////////////////////
	QString ipstring = tr("%1").arg(ip4)+ tr("%1").arg(ip_1)+tr("%1").arg(ip_4);
 
	ipstring = "U" + ipstring +"000000000";

	QString qstrtime = QTime::currentTime().toString("Chhmmsszzz");

	ipstring =  ipstring.mid(0,10)	+ qstrtime;	//("Uhhmmsszzz");//U123359375C123415656

	pconnect->SetUser(ipstring);//reset user
	pconnect2->SetUser(ipstring);//reset user
	ReFlashUSER();
	//////////////////////////////////////////////////////////////////////////
	pconnect->setattachconnect(pconnect2);
	pconnect2->setattachconnect(pconnect);
	pconnect->setstate(CxConnection::ConnectionState::P2Pconnection);
	pconnect2->setstate(CxConnection::ConnectionState::P2Pconnection);
}
void Dialog::U2U_connect(CxConnection*pconnect,const QString &quserb,const QString &csum)
{
	if(NULL==pconnect)
		return;  
	int isum  = csum.toInt();
	if(isum<=0)
		isum = 1;
	QString qusera =pconnect->GetUser();
	qusera = qusera.mid(0,10);////("Uhhmmsszzz");//U123359375C123415656

	int iuserasize =  GetUserConnectSize(qusera);
	int iuserbsize =  GetUserConnectSize(quserb);

 

	int iadda = isum - iuserasize ;
	int iaddb = isum - iuserbsize ;
	
	int iminsum = iuserasize>iuserbsize?iuserasize:iuserbsize;
	

	
	if(iadda>0)
	{
		CxConnection *p1 = GetUserConnect(qusera,0);
		if(p1!=NULL)
		p1->sendCommand(tr("new %1").arg(iadda));
		//AddUserConnect(qusera,iadda); 
		//pconnect->AddMsg("AVG Connecting");
		//return;
	}
	if(iaddb>0)
	{
		CxConnection *p2 = GetUserConnect(quserb,0);
		if(p2!=NULL)
		p2->sendCommand(tr("new %1").arg(iaddb));
		//AddUserConnect(quserb,iaddb);  	
		//pconnect->AddMsg("AVG Connecting");
		//return;
	}
	if(iaddb>0||iadda>0)
		return;
	else
	{
		for(int i=0;i<isum;i++)
		{
			CxConnection *p1 = GetUserConnect(qusera,i);
			CxConnection *p2 = GetUserConnect(quserb,i);

			if(p1!=NULL&&p2!=NULL)
			{
				C2C(p1,p2);
				removeconnect(p1);
                removeconnect(p2);
				CxConnectS aconnect;
				aconnect.m_pconnect = p1;
				aconnect.m_qsuser = qusera;
				m_P2Pconnections.push_back(aconnect);
 
				aconnect.m_pconnect = p2;
				aconnect.m_qsuser = quserb;
                m_P2Pconnections.push_back(aconnect);
			}
		}
		ReFlashUSER();
    }
}
void Dialog::P2P_connect(CxConnection*pconnect,const QString &qstring)
{
	if(NULL==pconnect)
		return; 
	CxConnection*pconnect2 = GetConnect(qstring );
	if(NULL==pconnect2)
		return;
		
    QString qsip4= pconnect->getio_peer_address_string();
	QStringList	alistparm  = qsip4.split(QLatin1String("."));	
	int ip1 =  alistparm.at(0).toInt();
	int ip2 =  alistparm.at(1).toInt();
	int ip3 =  alistparm.at(2).toInt();
	int ip4 =  alistparm.at(3).toInt();
    int iport =  pconnect->iopeerport();
	
    qsip4= pconnect->getio_peer_address_string();
	alistparm  = qsip4.split(QLatin1String("."));	
	int ip_1 =  alistparm.at(0).toInt();
	int ip_2 =  alistparm.at(1).toInt();
	int ip_3 =  alistparm.at(2).toInt();
	int ip_4 =  alistparm.at(3).toInt();
    int iport2 =  pconnect->iopeerport();
	
	 pconnect->stoptimer();
	 pconnect2->stoptimer();


	 pconnect->sendpublicK(ip4,iport,ip_1,ip_4,iport2);
	 pconnect2->sendpublicK(ip4,iport,ip_1,ip_4,iport2);
	 //////////////////////////////////////////////////////////////////////////
	 QString ipstring = tr("%1").arg(ip4)+ tr("%1").arg(ip_1)+tr("%1").arg(ip_4);

	 ipstring = "U" + ipstring +"000000000";

	 QString qstrtime = QTime::currentTime().toString("Chhmmsszzz");

	 ipstring =  ipstring.mid(0,10)	+ qstrtime;	//("Uhhmmsszzz");//U123359375C123415656

	 pconnect->SetUser(ipstring);//reset user
	 pconnect2->SetUser(ipstring);//reset user
	 ReFlashUSER();
	 //////////////////////////////////////////////////////////////////////////
	 pconnect->setattachconnect(pconnect2);
	 pconnect2->setattachconnect(pconnect);
	 pconnect->setstate(CxConnection::ConnectionState::P2Pconnection);
	 pconnect2->setstate(CxConnection::ConnectionState::P2Pconnection);
}
void Dialog::reset_connect(CxConnection*pconnect,const QString &qstring)
{
	if(NULL==pconnect)
		return;
 
	CxConnection*pconnect2 = GetConnect(qstring);
	if(NULL==pconnect2)
		return;
 
	pconnect2->overconnect(); 

}
void Dialog::reset_all()
{
	for(int i=0;i<m_connections.size();i++)
	{   
		 m_connections[i].m_pconnect->overconnect();
	}
	widgetList2->clear();
}
void Dialog::newserial()
{
    CxConnection_serial *pconnection_serial = new CxConnection_serial(this);//serial
    pconnection_serial->openSerialPort();
    m_pconnection_serial = pconnection_serial;
}
void Dialog::senddirfile()
{
    const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a destination directory"));
    if (dir.isEmpty())
        return;
    m_pconnection_serial->senddir(dir);
}
void Dialog::sendserialfile(const char *pfile)
{
    m_pconnection_serial->sendfile(pfile);//sendfilex
}
void Dialog::sendserialfile()
{
    QString fileName =
        QFileDialog::getOpenFileName(this, tr("Open File"),
        QDir::currentPath(),
        tr("ALL Files (*.*)"));
    if (fileName.isEmpty())
        return;
    m_pconnection_serial->sendfile(fileName);

}
void Dialog::sendserialfilex()
{
    QString fileName =
        QFileDialog::getOpenFileName(this, tr("Open File"),
        QDir::currentPath(),
        tr("ALL Files (*.*)"));
    if (fileName.isEmpty())
        return;
    m_pconnection_serial->sendfilex(fileName);//sendfilex

}

void Dialog::newconnect()
{
	QHostAddress qaddress;
 
	m_ptcpClient = new QTcpSocket;
	connect(m_ptcpClient, SIGNAL(connected()),this, SLOT(ClientIsConnection()));

	m_serverip =  GetInputText();
	if(m_serverip=="")
	{
        qaddress.setAddress(QLatin1String("192.168.1.2"));//192.168.1.2"));
        m_serverip = QLatin1String("192.168.1.2");//192.168.1.2");
    }
	else
		qaddress.setAddress(m_serverip);

    CxConnection *pconnection = new CxConnection(m_ptcpClient,this,0);//client 0

	if(m_serverport!="")
		pconnection->connectToHost(qaddress,m_serverport.toInt());
	else
	{
		m_serverport = tr("82");
		pconnection->connectToHost(qaddress,m_serverport.toInt());
	}
  
	const int Timeout = 10 * 1000;

	if (!m_ptcpClient->waitForConnected(Timeout)) 
	{
		pconnection->overconnect();
		ClientstartButton->setEnabled(true);
		clientStatusLabel->setText(tr("No Connected"));
		return; 
	}
	else
		pconnection->connectisok(m_connections.size());

}


void Dialog::deleteconnect(const QString &qstconnect)
{
	CxConnection*pconnect = GetConnect(qstconnect); 
	removeconnect(pconnect);
}
void Dialog::new_connect(CxConnection*pconnect,const QString &quserstring)
{
	int isize = quserstring.toInt();
	for(int i=0;i<isize;i++)
		newconnect();
}

CxConnection*Dialog::GetConnect(const QString &qstring)
{
	QString getstr = qstring;
	
	for(int i=0;i<m_connections.size();i++)
	{  
        if(m_connections[i].m_pconnect->getio_peer_address_string()
                +tr(":%1").arg(m_connections[i].m_pconnect->iopeerport())
                ==getstr.remove(' '))
			return m_connections[i].m_pconnect;
	}
	return NULL;
}
void Dialog::removeconnect(CxConnection*pconnect)
{
	for(int i=0;i<m_connections.size();i++)
	{
		if(m_connections[i].m_pconnect==NULL)
			m_connections.removeAt(i);
		if(m_connections[i].m_pconnect==pconnect)
		{
			m_connections.removeAt(i);	
			//if(m_ics!= 0)
            //if(0==pconnect->IScors())//client
            //{//client
            //pconnect->deletesocket();
            //}
			ReFlashUSER();
			
			return;
		}
	}	
}
void Dialog::Send_ConnectList(CxConnection*pconnect)//server func
{ 
	for(int i=0;i<m_connections.size();i++)
	{  
        AddMsg(m_connections[i].m_pconnect->getio_peer_address_string()
               +tr(":%1").arg(m_connections[i].m_pconnect->iopeerport()),pconnect);
	} 
	//Send_List(pconnect,tr("connect"),arry);
}
int Dialog::GetUserConnectSize(const QString &quser)
{

	int isize = 0;
	for(int i=0;i<m_connections.size();i++)
	{  
		QString qsconuser =m_connections[i].m_pconnect->GetUser();

		qsconuser = qsconuser.mid(0,10);////("Uhhmmsszzz");//U123359375C123415656
		if(qsconuser==quser)
			isize = isize + 1;
	} 
	return isize;
}
void Dialog::AddUserConnect(const QString &quser,int isize)
{
	for(int i=0;i<m_connections.size();i++)
	{  
		QString qsconuser =m_connections[i].m_pconnect->GetUser();

		qsconuser = qsconuser.mid(0,10);////("Uhhmmsszzz");//U123359375C123415656
		if(qsconuser==quser)
		{
			if(isize>0)
				AddDuty_front(m_connections[i].m_pconnect,tr("new %1").arg(isize));		
			return;
		}
	} 
}
CxConnection* Dialog::GetUserConnect(const QString &quser,int inum)
{
	int igetnum  =0 ;
	for(int i=0;i<m_connections.size();i++)
	{  
		QString qsconuser =m_connections[i].m_pconnect->GetUser();

		qsconuser = qsconuser.mid(0,10);////("Uhhmmsszzz");//U123359375C123415656
		if(qsconuser==quser)
		{
			if(igetnum==inum)
				return m_connections[i].m_pconnect;
			igetnum = igetnum + 1;
		
		}
	} 
	return NULL;
}

CxConnection * Dialog::GetActiveConnect()
{  
	int igetnum  =0 ;
	for(int i=0;i<m_connections.size();i++)
	{  
		if(m_connections[i].m_pconnect!=NULL)
		{ 
			bool bisbusy =m_connections[i].m_pconnect->IsBusy();
			if(bisbusy==false)
			{
				return m_connections[i].m_pconnect;
			}
		}
	} 
	return NULL;
}
void Dialog::Send_UserList(CxConnection*pconnect)//server func
{ 

	QStringList userlist;
	for(int i=0;i<m_connections.size();i++)
	{  
		QString qsconuser =m_connections[i].m_pconnect->GetUser();
 
		qsconuser = qsconuser.mid(0,10);////("Uhhmmsszzz");//U123359375C123415656
		userlist<<qsconuser;
 	} 
 	userlist.removeDuplicates();

	for(int i=0;i<userlist.size();i++)
	{  
		AddMsg(userlist.at(i) ,pconnect);
	}
	//Send_List(pconnect,tr("connect"),arry);
}
void Dialog::Show_ConnectList()
{ 
	for(int i=0;i<m_connections.size();i++)
	{  
         ShowLog(0,m_connections[i].m_pconnect->getio_local_address_string()
                 +tr(":%1").arg(m_connections[i].m_pconnect->iolocalport())) ;
	} 
	//Send_List(pconnect,tr("connect"),arry);
}
void Dialog::Send_Desktop(CxConnection*pconnect,const QString &qmsg)
{ 
	QString qpagelink = GetDesktopSize();
	QByteArray markup = GetDesktopImage();
	QFileInfo qfileinf(qpagelink);
	QString qname = qfileinf.fileName();
	Send_ImageFrame(pconnect,qname,markup);
	UseMouseKeyBoardMsg(qmsg);

}
void Dialog::Send_View(CxConnection*pconnect,const QString &qnum)
{ 

    BrowserWindow*pview = NULL;
	int inum =  qnum.toInt();
	if(inum<m_pageviewlist.size())
		pview  = m_pageviewlist[inum];
	else
		pview = m_pageviewlist[0];

	if(pview==NULL)	
		return;
	QString qpagelink;// = GetViewSize(pview);
	QByteArray markup = GetViewImage(qpagelink,pview);
	QFileInfo qfileinf(qpagelink);
	QString qname = qfileinf.fileName();
	Send_ImageFrame(pconnect,qname,markup);
//	UseMouseKeyBoardMsg(qmsg);

}

void Dialog::Send_Desktop_Multi( const QString &user,const QString &qmsg,const QString &qsdate)
{ 
	QString qpagelink = GetDesktopSize();
	QByteArray markup = GetDesktopImage();
    //QFileInfo qfileinf(qpagelink);
    QString qname = qpagelink;//qfileinf.fileName();
	Send_ImageFrame_Multi( user,qname,markup,qsdate);
    //UseMouseKeyBoardMsg(qmsg);

}
void Dialog::Send_Desktop_Multi_OneUser( const QString &qsdate)
{ 
	QString qpagelink = GetDesktopSize();
	QByteArray markup = GetDesktopImage();
    //QFileInfo qfileinf(qpagelink);
    QString qname =qpagelink;// qfileinf.fileName();
	Send_ImageFrame_Multi_OneUser( qname,markup,qsdate);
 

}
void Dialog::Send_View_Multi( const QString &user,const QString &qnum,const QString &qsdate)
{ 
    BrowserWindow*pview = NULL;
	int inum =  qnum.toInt();
	if(inum<m_pageviewlist.size())
		pview  = m_pageviewlist[inum];
	else
		pview = m_pageviewlist[0];
		
	if(pview==NULL)	
		return;

}
void Dialog::Send_MouseKeyBoardMsg_Multi(const QString &qmsg,const QString &qsdate)
{ 
 
	UseMouseKeyBoardMsg(qmsg);

}
void Dialog::setsendpiece(qint64 iID)
{
	m_datasendmap[iID] = true;
}
void Dialog::setfinishpiece(qint64 iID)
{
	m_datafinishmap[iID] = true;
}
qint64 Dialog::getsendpiece()
{
	QList<qint64> idmaplist = m_datafinishmap.keys();
	int imapsize = idmaplist.size();

	for(int i=0;i<imapsize;i++)
	{
		qint64 mapid = idmaplist[i];
		if(false==m_datafinishmap[mapid]
		&&false==m_datasendmap[mapid])
		{
			return mapid;
		}
	}
	return -1;
}
void Dialog::Add_Data(CxConnection*pconnect,int id)
{
    QFileInfo qfileinf(m_totaldatamap[id].m_stype);
	QString qname = qfileinf.fileName();  

	QDateTime now = QDateTime::currentDateTime();
	QString fileget = now.toString(QLatin1String("get_dd_hh_mm_ss"));

	CxPiece apiece;
	if(qname!=tr(""))
		apiece.qsname = qname ;
	else
		apiece.qsname = fileget;

    apiece.totallength = m_totaldatamap[id].m_data.size();//m_databuf.size();
	apiece.pieceLength = GetPieceSize();
	apiece.icurpiece = 0;
    apiece.pstringbuf = &m_totaldatamap[id].m_data;//&m_databuf;

    m_piecemap.insert(id,apiece);
    m_datafinishmap.insert(id,false);
    m_datasendmap.insert(id,false);

    CxPiece *pPiece = &m_piecemap[id];
	
	int isize  =  pPiece->totallength/pPiece->pieceLength + (pPiece->totallength%pPiece->pieceLength>0?1:0);
	 
	for(int i=0;i<isize;i++)
	{
		pPiece->m_ipiecestatus[i] = 0;
	}

    pPiece->m_user = pconnect->GetUser();
//	pconnect->AddData(pPiece);
}

void Dialog::Add_Data_Multi(const QString &user,int id,const QString &qsdate)
{
    QFileInfo qfileinf(m_totaldatamap[id].m_stype);
	QString qname = qfileinf.fileName();  

	QDateTime now = QDateTime::currentDateTime();
	QString fileget = now.toString(QLatin1String("get_dd_hh_mm_ss"));

	CxPiece apiece;
	if(qname!=tr(""))
		apiece.qsname = qname ;
	else
		apiece.qsname = fileget;

    apiece.totallength =m_totaldatamap[id].m_data.size();//m_databuf.size();
	apiece.pieceLength = GetPieceSize();
	apiece.icurpiece = 0;
	apiece.qsid = qsdate; //adata->m_qid;
    apiece.pstringbuf = &m_totaldatamap[id].m_data;//&m_databuf;

    m_piecemap.insert(id,apiece);
    m_datafinishmap.insert(id,false);
    m_datasendmap.insert(id,false);

    CxPiece *pPiece = &m_piecemap[id];

	int isize  =  pPiece->totallength/pPiece->pieceLength + (pPiece->totallength%pPiece->pieceLength>0?1:0);

	for(int i=0;i<isize;i++)
	{
		pPiece->m_ipiecestatus[i] = 0;
	}

    pPiece->m_user = user;

}

void Dialog::Add_Data_Multi_OneUser(int id,const QString &qsdate)
{
    QFileInfo qfileinf(m_totaldatamap[id].m_stype);
	QString qname = qfileinf.fileName();  

	QDateTime now = QDateTime::currentDateTime();
	QString fileget = now.toString(QLatin1String("get_dd_hh_mm_ss"));

	CxPiece apiece;
	if(qname!=tr(""))
		apiece.qsname = qname ;
	else
		apiece.qsname = fileget;

    apiece.totallength = m_totaldatamap[id].m_data.size();//m_databuf.size();
	apiece.pieceLength = GetPieceSize();
	apiece.icurpiece = 0;
	apiece.qsid = qsdate; //adata->m_qid;
    apiece.pstringbuf = &m_totaldatamap[id].m_data;//&m_databuf;

    m_piecemap.insert(id,apiece);
    m_datafinishmap.insert(id,false);
    m_datasendmap.insert(id,false);

    CxPiece *pPiece = &m_piecemap[id];

	int isize  =  pPiece->totallength/pPiece->pieceLength + (pPiece->totallength%pPiece->pieceLength>0?1:0);

	for(int i=0;i<isize;i++)
	{
		pPiece->m_ipiecestatus[i] = 0;
	}

    pPiece->m_user = m_connections[0].m_pconnect->GetUser();

}

void Dialog::Send_File(CxConnection*pconnect,const QString &fileName,const QByteArray &pagestr)
{
	QFileInfo qfileinf(fileName);
	QString qname = qfileinf.fileName();  

	cxdata adata; 
	if(qname!="")
		adata.m_stype = tr("file_") + qname;
	else
		adata.m_stype = tr("file_") + QTime::currentTime().toString(QLatin1String("hhmmss"));

	adata.m_data = pagestr;//m_databuf;
	adata.m_data = qCompress(adata.m_data);

    g_listID = g_listID + 1;
    m_totaldatamap[g_listID] = adata;

    Add_Data(pconnect,g_listID);

    AddSendReco(pconnect,tr(" "),qname);
	return;

}
void Dialog::SendControl()
{

    QString qstr = CommandEdit->text();

    Send_Control_Multi(qstr);

}
int Dialog::MapSize()
{
	return m_datafinishmap.keys().size();
}
void Dialog::ClearAll()
{
	QList<qint64> idmaplist = m_datafinishmap.keys();
	int imapsize = idmaplist.size();

	for(int i=0;i<imapsize;i++)
	{
		qint64 mapid = idmaplist[i];
		{
			ShowLog(0,tr("mapid = %1 remove").arg(mapid));
			m_totaldatamap[mapid].m_data.clear();
			m_totaldatamap[mapid].m_stype.clear();
			m_totaldatamap.remove(mapid);
			m_piecemap[mapid].m_ipiecestatus.clear();
			m_piecemap.remove(mapid);
			m_datafinishmap.remove(mapid);
			m_datasendmap.remove(mapid);

			removelist(0);
		}
	}
}
void Dialog::CollectionDataBufClear()
{

	QList<qint64> idmaplist = m_datafinishmap.keys();
	int imapsize = idmaplist.size();

	for(int i=0;i<imapsize;i++)
	{
		qint64 mapid = idmaplist[i];
		if(true==m_datafinishmap[mapid])
		{
			ShowLog(0,tr("mapid = %1 remove").arg(mapid));
			m_totaldatamap[mapid].m_data.clear();
			m_totaldatamap[mapid].m_stype.clear();
			m_totaldatamap.remove(mapid);
			m_piecemap[mapid].m_ipiecestatus.clear();
			m_piecemap.remove(mapid);
			m_datafinishmap.remove(mapid);
			m_datasendmap.remove(mapid);

			removelist(0);
		}
	}
}
bool Dialog::Send_TFile_Multi(const QString &fileName,const QByteArray &pagestr,int ifilenum,int iend,const QString &qsdate)
{
  QString  user  = m_connections[0].m_pconnect->GetUser();


    QFileInfo qfileinf(fileName);
    QString qname = qfileinf.fileName();

    cxdata adata;

    if(qname!="")
        adata.m_stype = tr("Tfile_%1|%2|").arg(ifilenum).arg(iend) + qname;
    else
        adata.m_stype = tr("Tfile_%1|%2|").arg(ifilenum).arg(iend) + QTime::currentTime().toString(QLatin1String("hhmmss"));

    adata.m_data = pagestr;//m_databuf;
    adata.m_data = adata.m_data;// qCompress(adata.m_data);//

    g_listID = g_listID + 1;
    m_totaldatamap[g_listID] = adata;

    Add_Data_Multi(user, g_listID ,qsdate);


    AddSendReco(NULL,tr(" "),qname);


    //widgetList->addItem(adata.m_stype);
    return true;


}
bool Dialog::Send_Control_Multi(const QString &pagestr, const QString &qsdate)
{
    CxConnection * pactconnect = GetActiveConnect();
    QString  user ;
if(0!=pactconnect)
{
   user = pactconnect->GetUser();
   m_recorduser = user;
}
else
{
    return false;
}

    cxdata adata;
    adata.m_stype = tr("Ctl_");
    adata.m_data = pagestr.toStdString().c_str();//m_databuf;
    adata.m_data = qCompress(adata.m_data);

    g_listID = g_listID + 1;
    m_totaldatamap[g_listID] = adata;

    Add_Data(pactconnect,g_listID);





    return true;
}

void Dialog::Get_ControlString(const QString &qname )
{
    int itolsize = m_piecefile.totallength;
    int ipsize =m_piecefile.pieceLength;

    if(itolsize==0
    ||ipsize==0)
        return;

    int ipiecenum = itolsize/ipsize + (itolsize%ipsize>0?1:0);
    m_databuf.clear();
    for(int i=0;i<ipiecenum;i++)
    {
        m_databuf += m_piecefile.m_qcontents[i];
    }

    m_piecefile.icurpiece = 0;
    m_piecefile.pieceLength = 0;
    m_piecefile.totallength = 0;
    m_piecefile.qsname = "";
    m_piecefile.qsid ="";
    m_piecefile.m_isfinish = 0;
    m_piecefile.m_qcontents.clear();
    m_piecefile.m_istatus.clear();

    m_databuf =  qUncompress(m_databuf);

    SetCodeText(m_databuf);
    TestParser();
}
int Dialog::Get_SaveTFileSend(const QString &qname, QByteArray &databuf)
{
   // QByteArray databuf = qUncompress(databuf0);
    QString qfilename ;
    int ifilenum;
    int iendnum;
    QStringList alist = qname.split("|");
    ifilenum = alist[0].toInt();
    iendnum = alist[1].toInt();
    qfilename = alist[2];

    QString qsavefilestring;
    if(m_savefiledir.isEmpty())
    {
        qsavefilestring = getlocationstringx("./file/"+qfilename);
    }
    else
    {
        qsavefilestring = m_savefiledir + qfilename;
    }

    ShowLog(0,tr(" ifilenum = %1").arg(ifilenum));

    int igetfilesize = m_getfiles.size();

    int ifindfile = 0;
    int ifindnum = -1;
    for(int ig =0;ig<igetfilesize;ig++)
    {
        if(m_getfiles[ig].m_filename == qsavefilestring)
        {
            ifindfile = 1;
            ifindnum = ig;
            if(0==m_getfiles[ig].m_istatus[ifilenum])
            {
                m_getfiles[ig].m_istatus[ifilenum] = 1;//get data
                m_getfiles[ig].m_getfilebytes[ifilenum] = databuf;
                m_getfiles[ig].m_bwrite[ifilenum] = false;
            }
        }
     }
    if(0==ifindfile)
    {
        CxGetFile acxgetfile;
        m_getfiles.push_back(acxgetfile);
        int isizex = m_getfiles.size();
        m_getfiles[isizex-1].m_filename = qsavefilestring;
        m_getfiles[isizex-1].m_istatus[ifilenum] = 1;
        m_getfiles[isizex-1].m_ifinish = 0;
        m_getfiles[isizex-1].m_ifinishsign = 0;
        m_getfiles[isizex-1].m_bwrite[ifilenum] = false;
        m_getfiles[isizex-1].m_getfilebytes[ifilenum] = databuf;
        m_getfiles[isizex-1].m_tfile = new QFile(qsavefilestring);

        ifindnum = isizex -1;

        if ( !m_getfiles[ifindnum].m_tfile->open(QFile::WriteOnly) )
        {
            ShowLog(0,tr("Tfile can't be open ")+qsavefilestring);

            return 0;

        }
        ShowLog(0,tr("Tfile file begin")+qsavefilestring);
        if(1==iendnum)
        {
            ShowLog(0,tr("Tfile file beg close ")+qsavefilestring);

            g_mutex.lock();
            m_getfiles[ifindnum].m_tfile->write(databuf);
            m_getfiles[ifindnum].m_getfilebytes[0].clear();
            m_getfiles[ifindnum].m_bwrite[0] = true;
            g_mutex.unlock();
            m_getfiles[ifindnum].m_tfile->close();
            delete m_getfiles[ifindnum].m_tfile;
            m_getfiles[ifindnum].m_tfile = 0;
            m_getfiles[ifindnum].m_ifinish = 1;
            m_getfilefinish = 1;
            return 1;
        }
    }
    if(-1!=ifindnum)
    if(1==iendnum)
    {
        m_getfiles[ifindnum].m_ifinishsign = ifilenum;
    }

    int ifindleak = -1;
    if(m_getfiles[ifindnum].m_ifinishsign>0
     &&1!=m_getfiles[ifindnum].m_ifinish)
    {
        ifindleak = 0;
        //check finish
        for(int i=0;i<=ifilenum;i++)
        {
            if(m_getfiles[ifindnum].m_istatus[i]==0)
            {
                ifindleak = 1;
            }
        }
     }

        int ifilesize = m_getfiles[ifindnum].m_istatus.size();
        for(int it=0;it<ifilesize;it++)
        {
            if(false ==m_getfiles[ifindnum].m_bwrite[it])
            {
                m_getfiles[ifindnum].m_bwrite[it] = true;
                g_mutex.lock();
                m_getfiles[ifindnum].m_tfile->write(m_getfiles[ifindnum].m_getfilebytes[it]);
                m_getfiles[ifindnum].m_getfilebytes[it].clear();
                g_mutex.unlock();
            }
        }

        if(1==iendnum)
        {
            if(0==ifindleak)
            {
                ShowLog(0,tr("Tfile file finish close")+qsavefilestring);
                m_getfiles[ifindnum].m_tfile->close();
                delete m_getfiles[ifindnum].m_tfile;
                m_getfiles[ifindnum].m_tfile = 0;

                m_getfiles[ifindnum].m_ifinish = 1;
                m_getfilefinish = 1;
                return 1;
            }
            ShowLog(0,tr("end no finish")+qsavefilestring);
            return 2;
        }

    if(0==ifindleak)
    {
        ShowLog(0,tr("Tfile file finishdata close!!!!")+qsavefilestring);
        m_tfile.close();
        m_getfiles[ifindnum].m_ifinish = 1;
        return 0;
    }

    ShowLog(0,tr("Tfile file end ")+qsavefilestring);

    return 0;
}

void Dialog::Send_File_Multi( const QString &user,const QString &fileName,const QByteArray &pagestr,const QString &qsdate)
{
	QFileInfo qfileinf(fileName);
	QString qname = qfileinf.fileName();

	cxdata adata;
	
	if(qname!="")
		adata.m_stype = tr("file_") + qname;
	else
		adata.m_stype = tr("file_") + QTime::currentTime().toString(QLatin1String("hhmmss"));

	adata.m_data = pagestr;//m_databuf;
	adata.m_data = qCompress(adata.m_data);

    g_listID = g_listID + 1;
    m_totaldatamap[g_listID] = adata;

    Add_Data_Multi(user,g_listID,qsdate);

	AddSendReco(NULL,tr(" "),qname);

    return;

}
void Dialog::Send_Page(CxConnection*pconnect,const QString &fileName,const QByteArray &pagestr)
{
	QFileInfo qfileinf(fileName);
	QString qname = qfileinf.fileName();  

	cxdata adata; 
	if(qname!="")
		adata.m_stype = tr("page_") + qname;
	else
		adata.m_stype = tr("page_") + QTime::currentTime().toString(QLatin1String("hhmmss"));

	adata.m_data = pagestr;//m_databuf;
	adata.m_data = qCompress(adata.m_data);

    g_listID = g_listID + 1;
    m_totaldatamap[g_listID] = adata;

    Add_Data(pconnect,g_listID);



	AddSendReco(pconnect,tr(" "),qname);
    return;


}
void Dialog::Send_Page_Multi( const QString &user,const QString &fileName,const QByteArray &pagestr,const QString &qsdate)
{
	QFileInfo qfileinf(fileName);
	QString qname = qfileinf.fileName();  

	cxdata adata; 
	if(qname!="")
		adata.m_stype = tr("page_") + qname;
	else
		adata.m_stype = tr("page_") + QTime::currentTime().toString(QLatin1String("hhmmss"));

	adata.m_data = pagestr;//m_databuf;
	adata.m_data = qCompress(adata.m_data);

    g_listID = g_listID + 1;
    m_totaldatamap[g_listID] = adata;

    Add_Data_Multi(user, g_listID,qsdate);

	AddSendReco(NULL,tr(" "),qname);

    return;


}
void Dialog::Send_List(CxConnection*pconnect,const QString &fileName,const QByteArray &pagestr)
{
	QFileInfo qfileinf(fileName);
	QString qname = qfileinf.fileName(); 


	cxdata adata; 
	if(qname!="")
		adata.m_stype = tr("list_") + qname;
	else
		adata.m_stype = tr("list_") + QTime::currentTime().toString(QLatin1String("hhmmss"));

	adata.m_data = pagestr;//m_databuf;
	adata.m_data = qCompress(adata.m_data);
    g_listID = g_listID + 1;
    m_totaldatamap[g_listID] = adata;

    Add_Data(pconnect,g_listID);


    AddSendReco(pconnect,tr(" "),qname);
	return;
}
void Dialog::Send_List_Multi( const QString &user,const QString &fileName,const QByteArray &pagestr,const QString &qsdate)
{
	QFileInfo qfileinf(fileName);
	QString qname = qfileinf.fileName(); 


	cxdata adata; 
	if(qname!="")
		adata.m_stype = tr("list_") + qname;
	else
		adata.m_stype = tr("list_") + QTime::currentTime().toString(QLatin1String("hhmmss"));

	adata.m_data = pagestr;//m_databuf;
	adata.m_data = qCompress(adata.m_data);
    g_listID = g_listID + 1;
    m_totaldatamap[g_listID] = adata;

    Add_Data_Multi(user,  g_listID,qsdate);

    AddSendReco(NULL,tr(" "),qname);
	return;


}
void Dialog::Send_SQL_Multi( const QString &user,const QString &fileName,const QByteArray &pagestr,const QString &qsdate)
{
	QFileInfo qfileinf(fileName);
	QString qname = qfileinf.fileName(); 


	cxdata adata; 
	if(qname!="")
		adata.m_stype = tr("sql_") + qname;
	else
		adata.m_stype = tr("sql_") + QTime::currentTime().toString(QLatin1String("hhmmss"));

	adata.m_data = pagestr;//m_databuf;
	adata.m_data = qCompress(adata.m_data);

    g_listID = g_listID + 1;
    m_totaldatamap[g_listID] = adata;

    Add_Data_Multi(user,  g_listID,qsdate);


    AddSendReco(NULL,tr(" "),qname);
	return;


}
void Dialog::Send_SQL_MultiEx( const QString &user,const QString &fileName,const QByteArray &pagestr,const QString &qsdate)
{
	QFileInfo qfileinf(fileName);
	QString qname = qfileinf.fileName(); 


	cxdata adata; 
	if(qname!="")
		adata.m_stype = tr("sqlx_") + qname;
	else
		adata.m_stype = tr("sqlx_") + QTime::currentTime().toString(QLatin1String("hhmmss"));

	adata.m_data = pagestr;//m_databuf;
	adata.m_data = qCompress(adata.m_data);
    g_listID = g_listID + 1;
    m_totaldatamap[g_listID] = adata;

    Add_Data_Multi(user,  g_listID,qsdate);

    AddSendReco(NULL,qsdate,qname);
	return;

}
void Dialog::Send_FileSQL_Multi( const QString &user,const QString &fileName,const QByteArray &pagestr,const QString &qsdate)
{
	QFileInfo qfileinf(fileName);
	QString qname = qfileinf.fileName();  


	cxdata adata; 
	if(qname!="")
		adata.m_stype = tr("sqlf_") + qname;
	else
		adata.m_stype = tr("sqlf_") + QTime::currentTime().toString(QLatin1String("hhmmss"));

	adata.m_data = pagestr;//m_databuf;
	adata.m_data = qCompress(adata.m_data);

    g_listID = g_listID + 1;
    m_totaldatamap[g_listID] = adata;

    Add_Data_Multi(user, g_listID,qsdate);

    AddSendReco(NULL,qsdate,qname);
	return;

}

void Dialog::Send_Image(CxConnection*pconnect,const QString &fileName,const QByteArray &pagestr)
{
	QFileInfo qfileinf(fileName);

	cxdata adata; 

	adata.m_stype = tr("image_") + fileName;

	adata.m_data = pagestr;//m_databuf;
	adata.m_data = qCompress(adata.m_data);

    g_listID = g_listID + 1;
    m_totaldatamap[g_listID] = adata;


    Add_Data(pconnect,g_listID);

    AddSendReco(pconnect,tr(" "),fileName);
	return;


}


void Dialog::Send_Image_Multi(const QString &user, const QString &fileName,const QByteArray &pagestr,const QString &qsdate)
{
	cxdata adata; 

	adata.m_stype = tr("image_") + fileName;

	adata.m_data = pagestr;//m_databuf;
	adata.m_data = qCompress(adata.m_data);

    g_listID = g_listID + 1;
    m_totaldatamap[g_listID] = adata;

    Add_Data_Multi( user, g_listID,qsdate);

	AddSendReco(NULL,tr(" "),fileName);

    return;
}

void Dialog::Send_ImageFrame(CxConnection*pconnect,const QString &fileName,const QByteArray &pagestr)
{ 
	QFileInfo qfileinf(fileName);	

	cxdata adata; 

	adata.m_stype = tr("imageframe_") + fileName;

	adata.m_data = pagestr;//m_databuf;
	adata.m_data = qCompress(adata.m_data);
    g_listID = g_listID + 1;
    m_totaldatamap[g_listID] = adata;

    Add_Data(pconnect,g_listID);
	AddSendReco(pconnect,tr(" "),fileName);

    return;


}

void Dialog::Send_ImageFrame_Multi_OneUser( const QString &fileName,const QByteArray &pagestr,const QString &qsdate)
{ 
	QFileInfo qfileinf(fileName);	

	cxdata adata; 

	adata.m_stype = tr("imageframe_") + fileName;

	adata.m_data = pagestr;//m_databuf;
	adata.m_data = qCompress(adata.m_data);
    g_listID = g_listID + 1;
    m_totaldatamap[g_listID] = adata;

    Add_Data_Multi_OneUser( g_listID,qsdate);

	return;
}

void Dialog::Send_ImageFrame_Multi( const QString &user,const QString &fileName,const QByteArray &pagestr,const QString &qsdate)
{ 
	QFileInfo qfileinf(fileName);	

	cxdata adata; 

	adata.m_stype = tr("imageframe_") + fileName;

	adata.m_data = pagestr;//m_databuf;
	adata.m_data = qCompress(adata.m_data);
    g_listID = g_listID + 1;
    m_totaldatamap[g_listID] = adata;

    Add_Data_Multi(user, g_listID,qsdate);

    AddSendReco(NULL,tr(" "),fileName);
	return;


}
void Dialog::Send_Request(CxConnection*pconnect,const QString &fileName,const QByteArray &pagestr)
{	
	QFileInfo qfileinf(fileName);
	QString qname = qfileinf.fileName();  

	cxdata adata; 

	if(qname!="")
		adata.m_stype = m_reqstring + qname;
	else
		adata.m_stype = m_reqstring + QTime::currentTime().toString(QLatin1String("hhmmss"));


	adata.m_data = pagestr;//m_databuf;
	adata.m_data = qCompress(adata.m_data);
    g_listID = g_listID + 1;
    m_totaldatamap[g_listID] = adata;

    Add_Data(pconnect,g_listID);

    AddSendReco(pconnect,tr(" "),qname);
    return;
}

void Dialog::Send_Request_Multi( const QString &user,const QString &fileName,const QByteArray &pagestr,const QString &qsdate)
{	
	QFileInfo qfileinf(fileName);
	QString qname = qfileinf.fileName();  

	cxdata adata; 

	if(qname!="")
		adata.m_stype = m_reqstring + qname;
	else
		adata.m_stype = m_reqstring + QTime::currentTime().toString(QLatin1String("hhmmss"));


	adata.m_data = pagestr;//m_databuf;
	adata.m_data = qCompress(adata.m_data);
    g_listID = g_listID + 1;
    m_totaldatamap[g_listID] = adata;

    Add_Data_Multi(user, g_listID,qsdate);

    AddSendReco(NULL,tr(" "),qname);
	return;


}
void Dialog::SendLocalFile(CxConnection*pconnect,const QString &fileName)
{
	m_databuf.clear();
	m_databuf = loadfilestring(fileName);
	QFileInfo qfileinf(fileName);

	QString qname = qfileinf.fileName();
	Send_File(pconnect,qname,m_databuf);


}
void Dialog::SendLocalFile_Multi( const QString &user,const QString &fileName,const QString &qsdate)
{

	m_databuf.clear();
	m_databuf = loadfilestring(fileName);
	QFileInfo qfileinf(fileName);

	QString qname = qfileinf.fileName();
	Send_File_Multi( user, qname,m_databuf,qsdate);


}

void Dialog::removeurllist(const QString &qsurl)
{
	int inum = 0;
	int iurlistsize = m_urllist.size();
 
	for(int i=0;i<iurlistsize;i++)
	{
		if(qsurl==m_urllist[i].m_url)
		{

			m_urllist.removeAt(i);
 
			return;
		} 
	} 
 
}
CxConnection* Dialog::geturlconnect(const QString &qstring)
{
	int isize = m_urllist.size();
	for(int i= 0 ;i< isize ;i++)
	{
		if(m_urllist[i].m_url == qstring)
			return m_urllist[i].m_pconnect;
	}
	return NULL;
}
QString Dialog::geturldate(const QString &qstring) const
{
	int isize = m_urllist.size();
	for(int i= 0 ;i< isize ;i++)
	{
		if(m_urllist[i].m_url == qstring)
			return m_urllist[i].m_qsdate;
	}
	return "0";
}
QByteArray* Dialog::geturlbuf(const QString &qstring) 
{
	int isize = m_urllist.size();
	for(int i= 0 ;i< isize ;i++)
	{
		if(m_urllist[i].m_url == qstring)
			return &(m_urllist[i].m_buf);
	}
	return NULL;
}
CxConnection* Dialog::getdateconnnect(const QString &qstring)
{
	int isize = m_urllist.size();
	for(int i= 0 ;i< isize ;i++)
	{
		if(m_urllist[i].m_qsdate == qstring)
			return m_urllist[i].m_pconnect;
	}
	return NULL;
}
void Dialog::WGetPage(CxConnection*pconnect,const QString &qstring,const QString &qsdate)
{

}
void Dialog::WSQLPage(CxConnection*pconnect,const QString &qstring,const QString &qsdate)
{
}
void Dialog::WSQLPageC(CxConnection*pconnect,const QString &qstring,const QString &qsdate)
{
}


void Dialog::WGetFile(CxConnection*pconnect,const QString &qstring,const QString &qsdate)
{
}
void Dialog::WSQLGetFile(CxConnection*pconnect,const QString &qstring,const QString &qsdate)
{
}
void Dialog::ResetPro()
{

}
void Dialog::UpDatePro()
{  

}

void Dialog::FileDir(CxConnection*pconnect,const QString &qsDir)
{
  
	const QDir cxDir(qsDir); 

	QStringList stfilterlist;

	QStringList filelist =  cxDir.entryList(QDir::AllEntries, QDir::Name);
	int isize = filelist.size();
	QString astr = "\r\n";
	for(int i=0;i<isize;i++)
	{
		astr += filelist.at(i); 
		astr += "\r\n";
 
	}
	pconnect->AddMsg(astr); 
}
void Dialog::AllShow(CxConnection*pconnect)
{
	QString qstrall;
	for(int i=0;i<m_textlist.size();i++)
	{
		qstrall = qstrall + m_textlist[i] + "\r\n";;
	}
	pconnect->AddMsg(qstrall); 
}
void Dialog::Clear()
{ 
	m_piecefile.icurpiece = 0;
	m_piecefile.pieceLength = 0;
	m_piecefile.totallength = 0;
	m_piecefile.qsname = "";
	m_piecefile.m_qcontents.clear();
	m_piecefile.m_istatus.clear();
	m_piecefile.qsid = ""; 
	m_piecefile.m_isfinish = 0;
	

	ShowLog(0,tr("Buff Clear"));
	clientProgressBar->reset();

}

void Dialog::Test1(int itype)
{ 
	if(0==itype)
	{
		QString fileName =
			QFileDialog::getOpenFileName(this, tr("Open File"),
			QDir::currentPath(),
			tr("ALL Files (*.*)"));
		if (fileName.isEmpty())
			return;

 
		QByteArray buf3 = qCompress(loadfilestring(fileName));

		QString fileName2 =
			QFileDialog::getSaveFileName(this, tr("Save File"),
			QDir::currentPath(),
			tr("ALL Files (*.*)"));
		QFile f(fileName2);

		if ( !f.open(QFile::WriteOnly) )
		{
			return;
		}

		f.write(buf3);
		f.close();
	
	}
	else
	{ 
		QString fileName =
			QFileDialog::getOpenFileName(this, tr("Open File"),
			QDir::currentPath(),
			tr("ALL Files (*.*)"));
		if (fileName.isEmpty())
			return;

 
		QByteArray buf3 = qUncompress(loadfilestring(fileName));

		QString fileName2 =
			QFileDialog::getSaveFileName(this, tr("Save File"),
			QDir::currentPath(),
			tr("ALL Files (*.*)"));
		QFile f(fileName2);

		if ( !f.open(QFile::WriteOnly) )
		{
			return;
		}

		f.write(buf3);
		f.close();
	}

}

void Dialog::SendMessage(CxConnection*pconnect,const QString &qstring)
{ 
	if(pconnect!=0)
		pconnect->sendMessage(qstring);
}
void Dialog::SendIrcMessage(const QString &qstring)
{ 
	CxConnection *pactconnect = GetActiveConnect();
	if(pactconnect!=0)
		pactconnect->sendMessage("<-[IRC]:"+qstring);

}

void Dialog::IrcOpt(CxConnection*pconnect,int iopt,const QString &qstring1,const QString &qstring2)
{

	CxConnection *phconnect= GetActiveConnect();
	if(NULL==phconnect)  
		SetActiveConnect(pconnect);
		
	switch(iopt)
	{
	case 0:

        break;
	case 9:
		QStringList alist = m_ircs.keys();
		for(int i=0;i<alist.size();i++)
		{
			m_pirc = m_ircs[alist[i]];
			delete m_pirc;
			m_pirc = NULL;
        }
		break;
	}
}
void Dialog::IrcMsg(CxConnection*pconnect,const QString &qstring)
{ 
	//m_pcurrentconnect = pconnect;
	
	QString qircmsg = qstring.mid(7);
		
 
	int inulnum = 0;
	int isize = qircmsg.size();

	for(int i = 0;i<isize ;i++)
	{
		QChar achar = qircmsg.at(i);
		if(achar==' ')
		{
			inulnum = i;
			goto IRCMSGNEXT0;
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
IRCMSGNEXT0:
	QString ircserver = qircmsg.mid(0,inulnum);
}
QByteArray Dialog::GetDesktopImage()
{
    //QSize asize = QApplication::desktop()->size();
    m_image = QPixmap::grabWindow(QApplication::desktop()->winId()).toImage();

	m_image = m_image.convertToFormat(QImage::Format_RGB16);
	m_resultImage = m_image;
 	//////////////////////////////////////////////////////////////////////////
	//difference
	QPainter painter(&m_resultImage);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(m_resultImage.rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawImage(0, 0, m_image);
	painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
	painter.drawImage(0, 0, m_ioldimage);
	painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
	painter.fillRect(m_resultImage.rect(), Qt::white);
	painter.end();
 	//////////////////////////////////////////////////////////////////////////
 	m_ioldimage = m_image; 
 	QByteArray imagebits((char*)m_resultImage.bits(),m_resultImage.byteCount());
	return imagebits;
}
//////////////////////////////////////////////////////////////////////////
QString Dialog::GetDesktopSize()
{	
	QSize qsize;
	QDesktopWidget* desktop = QApplication::desktop();
 
	qsize = desktop->size();


//#ifdef CXVISIONBSERVER 
//#endif
	return tr("%1_").arg(qsize.width())+tr("%1").arg(qsize.height());
}

//////////////////////////////////////////////////////////////////////////
QByteArray Dialog::GetViewImage(QString&imagesize,BrowserWindow*pview)
{
	//////////////////////////////////////////////////////////////////////////
	m_image = m_image.convertToFormat(QImage::Format_RGB16);
	m_resultImage = m_image;
	//////////////////////////////////////////////////////////////////////////
	//difference
	QPainter painter(&m_resultImage);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(m_resultImage.rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawImage(0, 0, m_image);
	painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
	painter.drawImage(0, 0, m_ioldimage);
	painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
	painter.fillRect(m_resultImage.rect(), Qt::white);
	painter.end();
	//////////////////////////////////////////////////////////////////////////
	m_ioldimage = m_image; 
	QByteArray imagebits((char*)m_resultImage.bits(),m_resultImage.byteCount());
 
	return imagebits;

}


//////////////////////////////////////////////////////////////////////////
QString Dialog::GetViewSize(BrowserWindow*pview)
{	
	QRect qarea;
	return tr("%1_").arg(qarea.width())+tr("%1").arg(qarea.height());
}
int asciiToKey(char ascii)
{
	switch ((unsigned char)ascii) 
	{
	case 'A': return 65;
	case 'B': return 66;
	case 'C': return 67;
	case 'D': return 68;
	case 'E': return 69;
	case 'F': return 70;
	case 'G': return 71;
	case 'H': return 72;
	case 'I': return 73;
	case 'J': return 74;
	case 'K': return 75;
	case 'L': return 76;
	case 'M': return 77;
	case 'N': return 78;
	case 'O': return 79;
	case 'P': return 80;
	case 'Q': return 81;
	case 'R': return 82;
	case 'S': return 83;
	case 'T': return 84;
	case 'U': return 85;
	case 'V': return 86;
	case 'W': return 87;
	case 'X': return 88;
	case 'Y': return 89;
	case 'Z': return 90;
	case '*': return 106;
	case '+': return 107;
	case '-': return 109;
	case '/': return 111;
	case '.': return 110; 
	case 'a': return 65;
	case 'b': return 66;
	case 'c': return 67;
	case 'd': return 68;
	case 'e': return 69;
	case 'f': return 70;
	case 'g': return 71;
	case 'h': return 72;
	case 'i': return 73;
	case 'j': return 74;
	case 'k': return 75;
	case 'l': return 76;
	case 'm': return 77;
	case 'n': return 78;
	case 'o': return 79;
	case 'p': return 80;
	case 'q': return 81;
	case 'r': return 82;
	case 's': return 83;
	case 't': return 84;
	case 'u': return 85;
	case 'v': return 86;
	case 'w': return 87;
	case 'x': return 88;
	case 'y': return 89;
	case 'z': return 90;
	
	
	default:   return ascii;
	}
}

void Dialog::SetKy(char akey)
{
#ifdef win32
	keybd_event(asciiToKey(akey),0,0,0);

	keybd_event(asciiToKey(akey),0,KEYEVENTF_KEYUP,0);
#endif
}
void Dialog::SetKy2(char akey1,char akey2)
{
#ifdef win32
	keybd_event(akey2,0,0,0);

	keybd_event(asciiToKey(akey1),0,0,0);
	
	keybd_event(asciiToKey(akey1),0,KEYEVENTF_KEYUP,0);

	keybd_event(akey2,0,KEYEVENTF_KEYUP,0);
#endif
}
void Dialog::SetKy3(char akey1,char akey2,char akey3)
{
#ifdef win32
	keybd_event(akey3,0,0,0);

	keybd_event(akey2,0,0,0);

	keybd_event(akey1,0,0,0);

	keybd_event(akey1,0,KEYEVENTF_KEYUP,0);
	keybd_event(akey2,0,KEYEVENTF_KEYUP,0);
	keybd_event(akey3,0,KEYEVENTF_KEYUP,0);
#endif
}
void Dialog::SetMouse(int imouseid,int ix,int iy)
{
#ifdef win32
	//MOUSEEVENTF_MOVE
	mouse_event(imouseid,ix,iy,0,0);
	QCursor acursor(Qt::CursorShape::ArrowCursor);
	acursor.setPos(ix,iy); 
	QDesktopWidget* desktop = QApplication::desktop();
	desktop->setCursor(acursor );
#endif
}
//////////////////////////////////////////////////////////////////////////
void Dialog::AddDuty(CxConnection*pconnect,const QString &qstring,const QString & qstime,int itype)
{
    QString qsdd = qstime;
    qint64 inum = Dialog::GetGolbalTime();
    if(qstime.toLongLong()>inum)
		qsdd = qstime;

	cxduty aduty;
	aduty.m_istats = 0;
	aduty.m_itype = itype;
	aduty.m_stime = qsdd;
	aduty.m_stcomm = qstring;

	pconnect->AddDuty(aduty);
	AddSendReco(pconnect,aduty.m_stime,qstring);
	
	//widgetList->addItem(tr("[%1 TYPE(%2)]:").arg(aduty.m_stime).arg(aduty.m_itype)+aduty.m_stcomm );

} 

void Dialog::AddDuty_date(CxConnection*pconnect,const QString &qstring,const QString & qstime,int itype)
{
    QString qsdd = qstime;

    qint64 inum = Dialog::GetGolbalTime();
    if(qstime.toLongLong()>inum)
        qsdd = qstime;

	cxduty aduty;
	aduty.m_istats = 0;
	aduty.m_itype = itype;
    aduty.m_stime = qsdd;
	aduty.m_stcomm = qstring;

	pconnect->AddDuty(aduty);
	AddSendReco(pconnect,aduty.m_stime,qstring);
 
} 
void Dialog::AddDuty_front(CxConnection*pconnect,const QString &qstring,const QString &qstime,int itype)
{
    QString qsdd = qstime;

    qint64 inum = Dialog::GetGolbalTime();
    if(qstime.toLongLong()>inum)
        qsdd = qstime;

	cxduty aduty;
	aduty.m_istats = 0;
	aduty.m_itype = itype;
	aduty.m_stime = qsdd;
	aduty.m_stcomm = qstring;

	pconnect->AddDuty_front(aduty);

	AddSendReco(pconnect,aduty.m_stime,qstring);
	
	//widgetList->addItem(tr("[%1 TYPE(%2)]:").arg(aduty.m_stime).arg(aduty.m_itype)+aduty.m_stcomm );

} 
void Dialog::AddLoacalDuty(const QString &qstring,int itype,const QString &qstime)
{
    QString qsdd = qstime;

    if(itype==-2)
    {
        //qint64 inum = Dialog::GetGolbalTime();
        //if(qstime.toLongLong()>inum)
        {
            qsdd = qstime;

            cxduty aduty;
            aduty.m_istats = 0;
            aduty.m_itype = -1;
            aduty.m_stime = qstime;//tr("%1").arg(inum);
            aduty.m_stcomm = qstring;
            m_localdutylist.push_back(aduty);
            widgetList3->addItem(tr("[%1 TYPE(%2)]:").arg(aduty.m_stime).arg(aduty.m_itype)+aduty.m_stcomm );

        }

    }
    else
    {
        //if(>inum)
        qsdd = qstime;

        cxduty aduty;
        aduty.m_istats = 0;
        aduty.m_itype = itype;
        aduty.m_stime = qstime;//tr("%1").arg(inum);
        aduty.m_stcomm = qstring;
        m_localdutylist.push_back(aduty);
        widgetList3->addItem(tr("[%1 TYPE(%2)]:").arg(aduty.m_stime).arg(aduty.m_itype)+aduty.m_stcomm );

    }
}

qint64 Dialog::GetGolbalTime()
{
    return m_sendTime.elapsed();
}
QStringList Dialog::CreateurlList(const QStringList &astrlist,const QUrl & qurl0)
{
    QStringList resultlist;

    return resultlist;
}
void Dialog::AddTFileDuty(QString &strdownloadtfile)
{
    cxduty aduty;

    aduty.m_istats = 0;
    aduty.m_itype = -2;
    aduty.m_stime = tr(" ");//tr("%1").arg(inum);
    aduty.m_stcomm = strdownloadtfile;
    m_localdutylist.push_back(aduty);
    widgetList3->addItem(tr("[downloadtfile]").arg(aduty.m_stime)+aduty.m_stcomm );

}
void Dialog::CreateDateList(const QStringList &astrlist,qint64 ibegintime,qint64 igaptime)
{
    qint64 inum = Dialog::GetGolbalTime();

        QString strdate = tr("%1").arg(inum+ibegintime);
        QString strdate2 = tr("%1").arg(inum+ibegintime+igaptime);

        for(int i=0;i<astrlist.size();i++)
        {
            cxduty aduty;
            aduty.m_istats = 0;
            aduty.m_itype = -1;
            aduty.m_stime = strdate;//tr("%1").arg(inum);
            aduty.m_stcomm = astrlist[i];
            m_localdutylist.push_back(aduty);
            widgetList3->addItem(tr("[%1 TYPE(%1)]:").arg(aduty.m_stime)+aduty.m_stcomm );

            aduty.m_istats = 0;
            aduty.m_itype = -1;
            aduty.m_stime = strdate2;//tr("%1").arg(inum);
            aduty.m_stcomm = tr("mouse 1");
            m_localdutylist.push_back(aduty);
            widgetList3->addItem(tr("[%1 TYPE(%1)]:").arg(aduty.m_stime)+aduty.m_stcomm );

            aduty.m_istats = 0;
            aduty.m_itype = -1;
            aduty.m_stime = strdate2;//tr("%1").arg(inum);
            aduty.m_stcomm = tr("runopt run");
            m_localdutylist.push_back(aduty);
            widgetList3->addItem(tr("[%1 TYPE(%1)]:").arg(aduty.m_stime)+aduty.m_stcomm );

        }

}
void Dialog::CreateDateListX(const QStringList &astrlist,qint64 ibegintime,qint64 igaptime)
{

    qint64 inum = Dialog::GetGolbalTime();
    QString strdate = tr("%1").arg(inum+ibegintime);

    FILE * fp = 0;

    QString filename = getlocationstringx("./duty.txt");

    fp=fopen(filename.toStdString().c_str(),"at");


    for(int i=0;i<astrlist.size();i++)
    {

        cxduty aduty;

        aduty.m_istats = 0;
        aduty.m_itype = -1;
        aduty.m_stime = strdate;//tr("%1").arg(inum);
        aduty.m_stcomm = tr("url ")+astrlist[i];
        m_localdutylist.push_back(aduty);
        widgetList3->addItem(tr("[%1 TYPE(%1)]:").arg(aduty.m_stime)+aduty.m_stcomm );

        /////////////////////////////////
        /// \brief fp

        QString filestring = strdate + tr("|")+aduty.m_stcomm+tr("\r\n");
            fprintf(fp,filestring.toStdString().c_str());
//            fprintf(fp,filestring2.toStdString().c_str());


    }
  fclose(fp);
}
void Dialog::AddMsg(const QString &qstring,CxConnection*pconnect)
{
	if(NULL!=pconnect)
		pconnect->AddMsg(qstring);
	else
		GetActiveConnect()->AddMsg(qstring);
	//widgetList->addItem(tr("MSG:")+qstring);
}
void Dialog::AddDutyNoReply(CxConnection*pconnect,const QString &qstring,const QString &qstrdate)
{ 
	QString qsdd;

    qint64 inum = Dialog::GetGolbalTime();
   if(qstrdate.toLongLong()>inum)
       qsdd = qstrdate;

	if(NULL!=pconnect)
		pconnect->sendCommand(qstring,qsdd);
} 
void Dialog::UseMouseKeyBoardMsg(const QString &qmsg)
{
	//QString qmsg = tr("EM%1_%2_%3")
	//QString qmsg = tr("EK")+event->key();
	QStringList	mkqslist  = qmsg.split("E");
	int ilistsize = mkqslist.size();
	for(int i=0;i<ilistsize;i++)
	{
		QString qstr = mkqslist[i];
		if(qstr.at(0)=='M')
		{
			qstr = qstr.mid(1);
			QStringList	mslist =qstr.split("_");
			int itype = mslist[0].toInt();
			int ix = mslist[1].toInt();
			int iy = mslist[2].toInt();
			SetMouse(itype,ix,iy);
			ShowLog(0,tr("mouse:")+qstr);
		}
		else if(qstr.at(0)=='K')
        {

			qstr = qstr.mid(1);
			QStringList	mslist =qstr.split("_");
			if(mslist.size()==2)
			{
                //int iget1 = mslist[0].toInt();
				int iget2 = mslist[1].toInt();
                char ichar1 = mslist[0].at(0).toLatin1();//(char) iget1;
				char ichar2 = iget2;//(char) iget2;
				SetKy2(ichar1,ichar2);
				
			}
			else 
			{ 
                char ichar1 = (char) qstr.at(0).toLatin1();//qstr.toInt();
				SetKy(ichar1);
			}
			ShowLog(0,tr("key:")+ qstr );
		}
	}
}
void Dialog::AppendText(const QString &strtext)
{ 
if(0)//debug
{
	ServerTextEdit->append(strtext);
}
else
{ 
	m_textlist.append(strtext);
	int isize = m_textlist.size();
	ServerTextEdit->clear();
 
	if(isize-30>0)
	{
		for(int i=isize-29;i<=isize-1;i++)
		{ 
			ServerTextEdit->append(m_textlist[i]);

		} 
		ServerTextEdit->append(tr(""));
		ServerTextEdit->append(tr(""));
	}
	else
	{
		for(int i=0;i<=isize-1;i++)
		{ 
			ServerTextEdit->append(m_textlist[i]);
		} 
		ServerTextEdit->append(tr(""));
		ServerTextEdit->append(tr(""));
	
	}
	if(isize>20000)
    {
		m_textlist.clear();
	}
 }
	
}

void Dialog::setwintext(QString qstr)
{
    serverStatusLabel->setText(qstr);

}
void Dialog::ShowLog(CxConnection*pconnect,const QString &qmsglog)
{
	if(NULL!=pconnect)
	{
		QString connectname = pconnect->GetUser();

		CxConnection *phconnect= GetActiveConnect();
		if(NULL==phconnect)  
			SetActiveConnect(pconnect);

		
		AppendText(QTime::currentTime().toString("hh:mm ")+connectname +":"+qmsglog);

	}
	else
	{
		AppendText(QTime::currentTime().toString("hh:mm") +":"+tr("Local:")+qmsglog);

	}
}
void Dialog::AddLog( const QString &qmsglog)
{
    int isize = m_textlist.size();
    m_textlist[isize-1] = m_textlist[isize-1] + qmsglog;
      isize = m_textlist.size();
    ServerTextEdit->clear();

    if(isize-30>0)
    {
        for(int i=isize-29;i<=isize-1;i++)
        {
            ServerTextEdit->append(m_textlist[i]);

        }
        ServerTextEdit->append(tr(""));
        ServerTextEdit->append(tr(""));
    }
    else
    {
        for(int i=0;i<=isize-1;i++)
        {
            ServerTextEdit->append(m_textlist[i]);
        }
        ServerTextEdit->append(tr(""));
        ServerTextEdit->append(tr(""));

    }
    if(isize>20000)
    {
        m_textlist.clear();
    }

}
void Dialog::SetLog( const QString &qmsglog)
{
    int isize = m_textlist.size();
    m_textlist[isize-1] = qmsglog;
      isize = m_textlist.size();
    ServerTextEdit->clear();

    if(isize-30>0)
    {
        for(int i=isize-29;i<=isize-1;i++)
        {
            ServerTextEdit->append(m_textlist[i]);

        }
        ServerTextEdit->append(tr(""));
        ServerTextEdit->append(tr(""));
    }
    else
    {
        for(int i=0;i<=isize-1;i++)
        {
            ServerTextEdit->append(m_textlist[i]);
        }
        ServerTextEdit->append(tr(""));
        ServerTextEdit->append(tr(""));

    }
    if(isize>20000)
    {
        m_textlist.clear();
    }

}

void Dialog::ShowHttpLog(const QString &qmsglog)
{ 
	ShowLog(0, tr("HTTP:")+qmsglog);
}
void Dialog::removelist(int item)
 {
	int isize =  widgetList->count();
	if(isize>0)
	{
	 QListWidgetItem *pitem = widgetList->takeItem(0);

     if(pitem)
		 widgetList->removeItemWidget(pitem);
	}
 }
void Dialog::clearlist()
{
	widgetList->clear();
}
void Dialog::MKDir(const QString &astr)
{
	//QString qstrlocal = getlocationstringx(astr);
	const QDir cxDir(astr); 
	if (!cxDir.exists()) 
	{
		if (!cxDir.mkpath(astr)) 
		{
			return ;
		}
	} 
}
void Dialog::RMDir(const QString & astr)
{
	const QDir cxDir(astr); 
	if (!cxDir.exists()) 
	{
		if (!cxDir.rmpath(astr)) 
		{
			return ;
		}
	}  
}
void Dialog::cxCopyFile(const QString &stfloderorg,const QString &stflodersrc)
{ 
	QFile::copy(stfloderorg,stflodersrc);
} 
void Dialog::RemoveFile(const QString &stfileorg )
{ 
	QFile::remove(stfileorg);
}
void Dialog::RenameFile(const QString &strfileold,const QString &strfilenew)
{ 
	QFile::rename(strfileold , strfilenew);
}
bool Dialog::runWithQtInEnvironment(const QString &cmd)
{
	

	// prepend the qt binary directory to the path
	QStringList env = QProcess::systemEnvironment();
	for (int i=0; i<env.count(); ++i) {
		QString var = env.at(i);
		int setidx = var.indexOf(QLatin1Char('='));
		if (setidx != -1) {
			QString varname = var.left(setidx).trimmed().toUpper();
			if (varname == QLatin1String("PATH")) {
				var = var.mid(setidx + 1);
				var = QLatin1String("PATH=") + 
					QLibraryInfo::location(QLibraryInfo::BinariesPath) +
					QLatin1Char(';') + var;
				env[i] = var;
				break;
			}
		}
	}
	//for (int i=0; i<env.count(); ++i) 
	//{
	//ShowLog(NULL,tr("env =") + env[i]);
	//} 
	//ShowLog(NULL,tr("cmd =") + cmd);
	proc.setEnvironment(env);
	proc.start(cmd);
	//proc.waitForFinished(-1);

	return (proc.exitCode() == 0);
}


void Dialog::AddSendReco(CxConnection *pconnect,const QString &stime,const QString &qstr)//m_waitlist
{
	cxdutysend asend;
	asend.m_pconnectsend = pconnect;
	asend.m_stime = stime;
	asend.m_qstring = qstr;
	m_waitlist.push_back(asend);
	if(pconnect!=NULL)
		widgetList->addItem(pconnect->GetUser()+tr(" ")+ asend.m_stime+tr(" ")+qstr);
	else
		widgetList->addItem( tr("All User ")+ asend.m_stime+tr(" ")+qstr );
}

void Dialog::RemoveRecoT(const QString &stime)
{
	//ShowLog(NULL,tr("R removeT t=")+stime);
	 for(int i=0;i<m_waitlist.size();i++)
	 {
		if(m_waitlist[i].m_stime == stime)
		{
			//ShowLog(NULL,tr("debug R removeT t=")+stime);
			m_waitlist[i].m_pconnectsend->Reset();
			m_waitlist.removeAt(i);	
			removelist(i);
			return;
		}
	 }

}
void Dialog::RemoveRecoN(const QString &sname)
{
	ShowLog(NULL,tr("R removeN n=")+sname);
	for(int i=0;i<m_waitlist.size();i++)
	{
		if(m_waitlist[i].m_qstring == sname)
		{
			m_waitlist.removeAt(i);	
			removelist(i);
			return;
		}
	}
}

void Dialog::removepageview(int inum)
{
	if(inum<m_pageviewlist.size())
		m_pageviewlist.removeAt(inum);
}
void Dialog::desktopimage()
{
    m_image = QPixmap::grabWindow(QApplication::desktop()->winId()).toImage();
    m_pmanager->setimage(m_image);
}
void Dialog::desktopimagerect(int ix,int iy,int iw,int ih)
{
    m_image = QPixmap::grabWindow(QApplication::desktop()->winId(),ix,iy,iw,ih).toImage();
    m_pmanager->setimage(m_image);
}
void Dialog::setimage(QImage& imginput )
{
    m_pmanager->setimage(imginput);
}
void Dialog::setimageframe(QImage& imginput )
{
    m_pmanager->setimageframe(imginput);
}
void Dialog::pagelistwork(int igap1,int igap2)
{
    #ifdef BrowWin
    QStringList astrlist = m_pageviewlist[0]->getlist();
        m_pstringai->saveurltreelistcreate(astrlist);
         #endif
    //    CreateDateListX(astrlist,igap1,igap1);//+100s begin and gap 100
}
void Dialog::TcpIp2ComSendString(const char *pvarname)
{
    QString strget = m_pmanager->GetParserString(QString(pvarname));
    if(strget.isEmpty()!=true)
    {
        if(m_pconnection_serial)
            m_pconnection_serial->sendstring(strget);
    }
}
bool Dialog::TcpIp2TcpIpSendImage(const char *pvarname)
{
    CxConnection * pactconnect = GetActiveConnect();
    QString  user ;
if(0!=pactconnect)
{
   user = pactconnect->GetUser();
   m_recorduser = user;
}
else
{
    return false;
}

    QImage *pimage = m_pmanager->GetParserImage(QString(pvarname));

    if(pimage==0)
        return true;

    QImage image = pimage->convertToFormat(QImage::Format_RGB16);

    QByteArray imagebits((char*)image.bits(),image.byteCount());

    QString qpagelink = tr("%1_").arg(image.width())+tr("%1").arg(image.height());

    QString qname = QString(qpagelink);

    Send_Image_Multi(user, qname,imagebits);

    return true;
}
bool Dialog::TcpIp2TcpIpSendValue(const char *pvarname)
{
    CxConnection * pactconnect = GetActiveConnect();
    QString  user ;
    if(0!=pactconnect)
    {
       user = pactconnect->GetUser();
       m_recorduser = user;
    }
    else
    {
        return false;
    }


    QString astrname(pvarname);
    double adouble = m_pmanager->GetParserValue(astrname);

    QString strdvalue = tr("%1").arg(adouble);

    QString strs= astrname + QString("_") +strdvalue;

    pactconnect->sendMessage(tr("value_")+strs);

    return true;
}
bool Dialog::TcpIp2TcpIpCompile(const char *pstring)
{
    CxConnection * pactconnect = GetActiveConnect();
    QString  user ;
    if(0!=pactconnect)
    {
       user = pactconnect->GetUser();
       m_recorduser = user;
    }
    else
    {
        return false;
    }


    QString astring(pstring);

    pactconnect->sendMessage(tr("compile ")+astring);

    return true;
}
void Dialog::TcpIp2ComSendImage(const char *pvarname)
{
    QImage *pimage = m_pmanager->GetParserImage(QString(pvarname));
    if(pimage!=0)
    {
        if(m_pconnection_serial)
            m_pconnection_serial->sendimage(*pimage);
    }
}
QString Dialog::getparservaluestring(QString astrname)
{
    double adouble = m_pmanager->GetParserValue(astrname);
    QString strdvalue = tr("%1").arg(adouble);
    return strdvalue;
}
void Dialog::TcpIp2ComSendValue(const char *parvarname)
{

    QString astrname(parvarname);
    double adouble = m_pmanager->GetParserValue(astrname);

    QString strdvalue = tr("%1").arg(adouble);
     if(m_pconnection_serial)
        m_pconnection_serial->sendvalue(astrname,strdvalue);

}
void Dialog::GetComTcpIpDesktop()
{
	if(m_pconnection_serial)
		m_pconnection_serial->sendCommand("A");
}
void Dialog::Com2TcpIpSendString(const char *pstr)
{
    if(m_pconnection_serial)
    {
        QString strsend = QString("B")+ pstr;
        m_pconnection_serial->sendCommand(strsend);
    }

}
void Dialog::runopt(const char *pname)
{

    m_pmanager->runoptcode(pname);
}
void Dialog::showmanagelog()
{
  ShowLog(0,m_pmanager->getoutputstring());
  m_pmanager->clearos();
}
void Dialog::updateview()
{
    m_pmanager->mutexupdate();
}
void Dialog::mousegridmove()
{
  
}
void Dialog::Testurl()
{
    if(0==m_btimerun)
    {
        m_cxvisiontimer.start(500, this);
        m_btimerun =1;
    }
    else
    {
        m_btimerun = 0;
        m_cxvisiontimer.stop();
    }



}
void Dialog::sceneScale(double newScale)
{

}

void Dialog::rundutylist()
{

    QString qrunstring = m_pstringai->runstep();

    if(qrunstring.isEmpty())
        return;


    widgetList3->clear();
    widgetList3->addItems(m_pstringai->prelist());
}
void Dialog::downloadlink(QString &qstr)
{
    for(int i=0;i<m_downloadlist.size();i++)
    {
        if(m_downloadlist[i]->IsFinish())
        {
           DownloadManager *padownload = m_downloadlist[i];
           delete padownload;
           m_downloadlist.removeAt(i);
        }
    }
    DownloadManager *pdownload = new DownloadManager(this);
    pdownload->append(qstr);
    // m_pdownload->append(qstr);
    m_downloadlist.push_back(pdownload);
}
void Dialog::cleartab()
{

}
void Dialog::clearfilter()
{
    m_pstringai->clearfilter();
}
void Dialog::addextfilter(const QString &str)
{
    m_pstringai->addextfilter(str.toStdString().c_str());
}

void Dialog::addkeywordfilter(const QString &str)
{
    m_pstringai->addkeywordfilter(str.toStdString().c_str());
}
void Dialog::addneedstringlist(const QString &str)
{
    m_pstringai->addneedstringlist(str.toStdString().c_str());
}
void Dialog::runstringai()
{//easy ai
    QString qstr = m_pstringai->getstring(-1);
    QString qopt = m_pstringai->cxvisionaiparserstring("cxvisionai",qstr);
    ShowLog(0,"cxvisionai run:"+qopt);
    runopt(qopt.toStdString().c_str());
}
//! [6]

//! [7]

VisionTab::VisionTab(visionView*pview, QWidget *parent)
    : QWidget(parent)
{

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(pview);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}
//! [7]


SpectrumTab::SpectrumTab(SpectrumWidget*pview1,SpectrumWidget*pview2, QWidget *parent)
    : QWidget(parent)
{

    QVBoxLayout *mainLayout = new QVBoxLayout;

}

