
#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <QPrinter>

#include "cximage/visionmanager.h"

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
class QBasicTimer;
class Dialog;
QT_END_NAMESPACE
class visionmanager;

//! [0]
class CxImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    CxImageViewer(Dialog *pdialog);
    ~CxImageViewer();
	void loadimage(QImage &image);
	void loadimageframe(QImage &image);
    //visionmanager * getmanager(){return m_pmanager;}
protected:
	void timerEvent(QTimerEvent *event);

	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);
private slots:
    void open();
    void print();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();
    void desktopruntimer();
    void viewruntimer();

public slots:
	void saveimage();
private:
    void createActions();
    void createMenus();
    void updateActions();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

	Dialog *m_pdialog;
    QLabel *m_imageLabel;
    QScrollArea *scrollArea;
    double scaleFactor;
	int m_igetpicsend;
#ifndef QT_NO_PRINTER
    QPrinter printer;
#endif


    QAction *openAct;
    QAction *printAct;
    QAction *exitAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
    QAction *timer1Act;
    QAction *timer2Act;
    QAction *saveimageAct;

    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;	
    QBasicTimer *m_pictimer;
    
    QImage m_image;
 
    QString m_MKact;
	QString m_qsendstr;
	bool m_bloading ;


    //visionmanager * m_pmanager;
};
//! [0]

#endif
