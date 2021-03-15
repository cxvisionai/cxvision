
#include <QtGui>
#include <QLabel>
#include <QScrollArea>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QtWidgets>
#include <QPrintDialog>

#include "dialog.h"
#include "imageviewer.h"

//! [0]
CxImageViewer::CxImageViewer(Dialog *pdialog)
{
	m_pdialog = pdialog;
  //  imageLabel = new QLabel;
    m_pictimer = new QBasicTimer;
  //  imageLabel->setBackgroundRole(QPalette::Base);
  //  imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  //  imageLabel->setScaledContents(true);


 //   m_pmanager->setBackgroundRole(QPalette::Base);
 //   m_pmanager->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
 //   m_pmanager->setScaledContents(true);

 //   m_pmanager->setMinimumWidth(resultSize.width());


    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
 //   scrollArea->setWidget(m_pmanager);
    setCentralWidget(scrollArea);

    createActions();
    createMenus();

    setWindowTitle(tr("Image Viewer"));
    resize(500, 400);
    
    m_igetpicsend = 1;
    //ashape1.setrect(100,100,20,20);

	//QPainter painter(&m_image); 
	//painter.fillRect(m_image.rect(), Qt::black);
	//painter.end();
	m_bloading = false;
}
CxImageViewer::~CxImageViewer()
{
//	delete imageLabel;
//    delete m_pmanager;
	delete m_pictimer;
	delete scrollArea;

}
//! [0]
void CxImageViewer::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == m_pictimer->timerId())
	{	
#ifndef TEMPTESTCXVISION
		if(1==m_igetpicsend)
		{
			m_igetpicsend = 0;
			QString mkact = m_MKact;
			m_pdialog->AddDuty(m_pdialog->GetActiveConnect(),m_qsendstr+mkact);//getdesktop
			m_MKact.clear();
		}
#endif

	}	
	else
	{
		QObject::timerEvent(event);
	}
}
//! [1]
void CxImageViewer::open()
//! [1] //! [2]
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        QImage image(fileName);
        if (image.isNull()) {
            QMessageBox::information(this, tr("Image Viewer"),
                                     tr("Cannot load %1.").arg(fileName));
            return;
        }
//! [2] //! [3]
//        m_pmanager->trigger(image);
//        m_pmanager->update();
//! [3] //! [4]
        scaleFactor = 1.0;

        printAct->setEnabled(true);
        fitToWindowAct->setEnabled(true);
        updateActions();

     //   if (!fitToWindowAct->isChecked())
     //       imageLabel->adjustSize();
    }
}

void CxImageViewer::loadimageframe(QImage &image)
{
	////////////////////////////////////////////////////////////////////////////
	////difference
	//	image.save("get1.bmp");
	if(false!=m_bloading)
	return;
	
	m_bloading = true;
	
	QImage resultImage = image; 

	QPainter painter(&resultImage);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(resultImage.rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawImage(0, 0, image);
	painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
	painter.drawImage(0, 0, m_image);
	painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
	painter.fillRect(resultImage.rect(), Qt::white);
	painter.end();
	////////////////////////////////////////////////////////////////////////////
	//	//! [2] //! [3]
	m_image = resultImage;

	//	 m_image.save("get2.bmp");
//    m_pmanager->trigger(m_image);
//    m_pmanager->update();
	m_igetpicsend = 1;
	//! [3] //! [4]
	scaleFactor = 1.0;

	printAct->setEnabled(true);
	fitToWindowAct->setEnabled(true);
	updateActions();

//	if (!fitToWindowAct->isChecked())
//		imageLabel->adjustSize();

	m_bloading = false;
	//	}
}
//! [4]
void CxImageViewer::loadimage(QImage &image)
//! [1] //! [2]
{
	if(true==m_bloading)
		return;

	m_bloading = true;
	//	//! [2] //! [3]
	m_image = image;

//    m_pmanager->trigger(m_image);
//    m_pmanager->update();

	m_igetpicsend = 1;
	//! [3] //! [4]
	scaleFactor = 1.0;

	printAct->setEnabled(true);
	fitToWindowAct->setEnabled(true);
	updateActions();

//	if (!fitToWindowAct->isChecked())
//		imageLabel->adjustSize();


	QPainter painter(&m_image); 
	painter.fillRect(m_image.rect(), Qt::black);
	painter.end();
	m_bloading = false;
	//	}
}
//! [5]
void CxImageViewer::print()
//! [5] //! [6]
{

	if(true==m_bloading)
		return;
//    Q_ASSERT(m_pmanager->pixmap());
#ifndef QT_NO_PRINTER
//! [6] //! [7]
    QPrintDialog dialog(&printer, this);
//! [7] //! [8]
    if (dialog.exec())
    {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
//      QSize size = m_pmanager->pixmap()->size();
//        size.scale(rect.size(), Qt::KeepAspectRatio);
//        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
//      painter.setWindow(m_pmanager->pixmap()->rect());
//      painter.drawPixmap(0, 0, *m_pmanager->pixmap());
    }
#endif

}
//! [8]

//! [9]
void CxImageViewer::zoomIn()
//! [9] //! [10]
{
    scaleImage(1.25);
}

void CxImageViewer::zoomOut()
{
    scaleImage(0.8);
}

//! [10] //! [11]
void CxImageViewer::normalSize()
//! [11] //! [12]
{
//    m_pmanager->adjustSize();
    scaleFactor = 1.0;
}
//! [12]

//! [13]
void CxImageViewer::fitToWindow()
//! [13] //! [14]
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        normalSize();
    }
    updateActions();
}
//! [14]


//! [15]
void CxImageViewer::desktopruntimer()
//! [15] //! [16]
{ 
	QPainter painter(&m_image); 
	painter.fillRect(m_image.rect(), Qt::black);
	painter.end();
	
	if(m_pictimer->isActive())
	{
		
		m_pictimer->stop();
	}
	else
	{
		m_pdialog->AddDuty(m_pdialog->GetActiveConnect(),"clear");
		m_pictimer->start(100, this);
	}
	m_qsendstr = tr("getdesktop ");
}
//! [15]
void CxImageViewer::viewruntimer()
//! [15] //! [16]
{ 
	QPainter painter(&m_image); 
	painter.fillRect(m_image.rect(), Qt::black);
	painter.end();

	if(m_pictimer->isActive())
	{

		m_pictimer->stop();
	}
	else
	{
		m_pdialog->AddDuty(m_pdialog->GetActiveConnect(),"clear");
		m_pictimer->start(100, this);
	}
	m_qsendstr = tr("getview ");
} 
void CxImageViewer::saveimage()
{
	QDateTime now = QDateTime::currentDateTime();
	QString qyear = now.toString(QLatin1String("yyyy"));
	QString qmh = now.toString(QLatin1String("MM"));
	QString qdy = now.toString(QLatin1String("dd"));
	int ih = QTime::currentTime().hour();
	int im = QTime::currentTime().minute();
	int is = QTime::currentTime().second();	
	QString qfilename = qyear + tr("_") +qmh + tr("_") +qdy + QTime::currentTime().toString("_hh_mm_ss")+tr(".bmp");

	if(m_pdialog!=NULL)
		m_image.save(m_pdialog->getlocationstringx("./file/"+qfilename));

	//QPainter painter(&m_image); 
	//painter.fillRect(m_image.rect(), Qt::black);
	//painter.end();
}
//! [16]

//! [17]
void CxImageViewer::createActions()
//! [17] //! [18]
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    printAct = new QAction(tr("&Print..."), this);
    printAct->setShortcut(tr("Ctrl+P"));
    printAct->setEnabled(false);
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    zoomInAct = new QAction(tr("Zoom &In (25%)"), this);
    zoomInAct->setShortcut(tr("Ctrl++"));
    zoomInAct->setEnabled(false);
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAct = new QAction(tr("Zoom &Out (25%)"), this);
    zoomOutAct->setShortcut(tr("Ctrl+-"));
    zoomOutAct->setEnabled(false);
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

    normalSizeAct = new QAction(tr("&Normal Size"), this);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    normalSizeAct->setEnabled(false);
    connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

    fitToWindowAct = new QAction(tr("&Fit to Window"), this);
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));
    connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

    timer1Act = new QAction(tr("&DeskTopTimer"), this);
    connect(timer1Act, SIGNAL(triggered()), this, SLOT(desktopruntimer()));
	timer1Act->setCheckable(true);

	timer2Act = new QAction(tr("&ViewTimer"), this);
	connect(timer2Act, SIGNAL(triggered()), this, SLOT(viewruntimer()));
	timer2Act->setCheckable(true);
	
    saveimageAct = new QAction(tr("&saveimage"), this);
    connect(saveimageAct, SIGNAL(triggered()), qApp, SLOT(saveimage()));
}
//! [18]

//! [19]
void CxImageViewer::createMenus()
//! [19] //! [20]
{
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    viewMenu = new QMenu(tr("&View"), this);
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    viewMenu->addAction(normalSizeAct);
    viewMenu->addSeparator();
    viewMenu->addAction(fitToWindowAct);

    helpMenu = new QMenu(tr("&Other"), this);
	helpMenu->addAction(timer1Act);
	helpMenu->addAction(timer2Act);
    helpMenu->addAction(saveimageAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(helpMenu);
}
//! [20]

//! [21]
void CxImageViewer::updateActions()
//! [21] //! [22]
{
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}
//! [22]

//! [23]
void CxImageViewer::scaleImage(double factor)
//! [23] //! [24]
{
    //Q_ASSERT(m_pmanager->pixmap());
    scaleFactor *= factor;
  //  m_pmanager->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}
//! [24]

//! [25]
void CxImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
//! [25] //! [26]
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}
#define MOUSEEVENTF_MOVE        0x0001 /* mouse move */
#define MOUSEEVENTF_LEFTDOWN    0x0002 /* left button down */
#define MOUSEEVENTF_LEFTUP      0x0004 /* left button up */
#define MOUSEEVENTF_RIGHTDOWN   0x0008 /* right button down */
#define MOUSEEVENTF_RIGHTUP     0x0010 /* right button up */
#define MOUSEEVENTF_MIDDLEDOWN  0x0020 /* middle button down */
#define MOUSEEVENTF_MIDDLEUP    0x0040 /* middle button up */
#define MOUSEEVENTF_XDOWN       0x0080 /* x button down */
#define MOUSEEVENTF_XUP         0x0100 /* x button down */
#define MOUSEEVENTF_WHEEL       0x0800 /* wheel button rolled */
#define MOUSEEVENTF_VIRTUALDESK 0x4000 /* map to entire virtual desktop */
#define MOUSEEVENTF_ABSOLUTE    0x8000 /* absolute move */


//! [26]
void CxImageViewer::mousePressEvent(QMouseEvent *event)
{ 
	if(m_pictimer->isActive())
	{
		QString qmsg ;
		if(event->button()== Qt::MouseButton::LeftButton)
		{
			qmsg =tr("EM%1_%2_%3")
				.arg(MOUSEEVENTF_LEFTDOWN)
				.arg(event->x())
				.arg(event->y() - menuBar()->height());
		}
		else if(event->button()== Qt::MouseButton::RightButton)
		{
			qmsg =tr("EM%1_%2_%3")
				.arg(MOUSEEVENTF_RIGHTDOWN)
				.arg(event->x())
				.arg(event->y() - menuBar()->height());
		}
		if(qmsg!="")
		{
			m_MKact.append(qmsg);
			
			m_pdialog->ClientTextEdit->append(qmsg);
		}
	}
	
 	QWidget::mousePressEvent(event);
}

void CxImageViewer::mouseReleaseEvent(QMouseEvent *event)
{
if(m_pictimer->isActive())
{
	QString qmsg ;
	if(event->button()== Qt::MouseButton::LeftButton)
	{
		qmsg =tr("EM%1_%2_%3")
			.arg(MOUSEEVENTF_LEFTUP)
			.arg(event->x())
			.arg(event->y() - menuBar()->height());
	}	
	else if(event->button()== Qt::MouseButton::RightButton)
	{
		qmsg =tr("EM%1_%2_%3")
			.arg(MOUSEEVENTF_RIGHTUP)
			.arg(event->x())
			.arg(event->y() - menuBar()->height());
	
	}  
	else if(event->button()== Qt::MouseButton::MidButton)
	{ 

		QPainter painter(&m_image); 
		painter.fillRect(m_image.rect(), Qt::black);
		painter.end();
	} 
	
	
	if(qmsg!="")
	{
	m_MKact.append(qmsg);
	
		m_pdialog->ClientTextEdit->append(qmsg);

	}
}
	QWidget::mouseReleaseEvent(event);
 
}
void CxImageViewer::mouseMoveEvent(QMouseEvent *event)
{
	if(m_pictimer->isActive())
	{
		QString qmsg = tr("EM%1_%2_%3")
			.arg(MOUSEEVENTF_MOVE)
			.arg(event->x())
			.arg(event->y() - menuBar()->height());
		m_MKact.append(qmsg);
		
		
		m_pdialog->ClientTextEdit->append(qmsg);
	}
	QWidget::mouseMoveEvent(event);

} 


void CxImageViewer::keyPressEvent(QKeyEvent *event)
{
	if(m_pictimer->isActive())
	{
	
		Qt::KeyboardModifiers amf =  event->modifiers();
		QString qmsg ;
		
		switch(amf)
		{
		case  Qt::NoModifier://keyToAscii
		default:
			qmsg = tr("EK")+ event->text();//tr("%1").arg(0x0ffff&event->key()) ;
			break; 
		case  Qt::MetaModifier://20
			qmsg = tr("EK")+ event->text()+tr("_%1").arg(20);
			break;
		case  Qt::ShiftModifier://16
			qmsg = tr("EK")+ event->text()+tr("_%1").arg(16);
			break;
		case  Qt::ControlModifier://17
			qmsg = tr("EK")+ event->text()+tr("%1").arg(17);
			break;
		case  Qt::AltModifier://18
			qmsg = tr("EK")+ event->text()+tr("%1").arg(18);
			break;
		}
		
		m_MKact.append(qmsg);
		

		m_pdialog->ClientTextEdit->append(qmsg);
	}
	QWidget::keyPressEvent(event);

} 
