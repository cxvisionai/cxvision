
#ifndef visionmanager_H
#define visionmanager_H

#include "imagebase.h"
#include "ImageParserClass.h"
#include <QFrame>
#include <QMutex>
#include <QDir>
#include <QGraphicsScene>
#include <QElapsedTimer>
#include "shape.h"

QT_FORWARD_DECLARE_CLASS(QUndoStack)
QT_FORWARD_DECLARE_CLASS(QTextStream)
class FuSMAIControl;
class dialogOPT;
class StringAI;
using namespace mu;

QT_BEGIN_NAMESPACE
class QScrollArea;
class QLabel;
class QSlider;
class QToolButton;
QT_END_NAMESPACE

class visionmanager : public QWidget
{
    Q_OBJECT

public:
    visionmanager(QWidget *parent = 0);
    ~visionmanager();

    void initialparser();
    void interfaceini();

    QString addShape(const QShape &shape);
    void deleteShape(const QString &shapeName);
    QShape shape(const QString &shapeName);
    QString currentShapeName();

    void setShapeRect(const QString &shapeName, const QRect &rect);
    void setShapeColor(const QString &shapeName, const QColor &color);

    bool load(QTextStream &stream);
    void save(QTextStream &stream);

    QString fileName() const;
    void setFileName(const QString &fileName);

    QUndoStack *undoStack() const;

    void runeditcode();
    void runstringcode(const char *optstr);
    void runstringcodex(const char *optstr);
    void saveeditcode(const char *pcodes,const char *filenames="./run.cxut");
    void collectionstringopt(const char *optcode,const char *optstr);
    void trigger(const QImage& imginput);
    void setimage(QImage& imginput);
    void setimageframe(QImage& imginput);
    QString getoutputstring(){return QString(m_os.str().c_str());}

    void SetParserValue(const QString & codestr,double dvalue);
    double GetParserValue(const QString & codestr);
    QImage* GetParserImage(const QString & namestr);
    QString GetParserString(const QString & namestr);
    void runoptcode(const char *optstr);

    QString getloadcode(){return m_strcode;}
    dialogOPT *getopt();
    StringAI *getstringai();
    void mutexupdate();
    void clearos();
    void clearcreateos();
    void saveautocreate();
    void autorun(const char *optstr);
    void autorunstringcode(const char *optstr);
    void zoomin(double dzoomx,double dzoomy);
    void rotateangle(double dangle);
    QString getcurrentstatename(){return m_currentstatename;}
public slots:
    void SetOptStateName(const QString &statename);
    void SetOptConnectName(const QString &connectname);
    void loadstatestring();
    void loadconnectstring();

signals:
    void currentShapeChanged(const QString &shapeName);
    //AI
    void OperationModelList(const QStringList&modellist);
    void OperationModelConnectList(const QStringList&modellist);
    void RunState(const	QString &statename);
    void RunConnect(const QString&connectname);
    void StateChange();
    void StateClear();
    void signals_GetStateString(const QString &statestring);
    void signals_GetConnectString(const QString &connectstring);
    void GetValue(const double & dvalue);
    void runopt(const QString &stropt);
    void runoptover(const QString &stropt);

protected:
   virtual void paintEvent(QPaintEvent *event) override;
   virtual void mousePressEvent(QMouseEvent *event) override;
   virtual void mouseReleaseEvent(QMouseEvent *event) override;
   virtual void mouseMoveEvent(QMouseEvent *event) override;

private:
    void setCurrentShape(QShape *pshape);
    QShape* indexOf(const QString &shapeName) ;
    QShape* indexAt(const QPoint &pos) ;
    QString uniqueName(const QString &name) ;
    void resizeEvent(QResizeEvent *event) override;

//    QList<Shape> m_shapeList;
//
    int m_currentIndex;
//    int m_mousePressIndex;
    QPoint m_mousePressOffset;
    bool m_resizeHandlePressed;
    QString m_fileName;
    QString m_strcode;

    QUndoStack *m_undoStack;


    mu::CImageParserClass m_imageparser;
    std::ostringstream m_os;//compile result output os
    std::ostringstream m_createcodeos;//compile result output os
    QShape* m_pcurshape;
    QShape* m_pmousepressshape;

  //  static bool m_block;
 //   bool m_bruntimes;
    QMutex m_runmutex;
    QElapsedTimer m_elapsed;

     FuSMAIControl * m_FSMControl;
     QDir m_currentDir;
     QStringList m_strROIlist;

    QStringList m_files;
     QStringList m_stcnxutlist;
     QStringList m_stcxutlist;
     QStringList DirFileFind(const QString &path,const QString &filename);

     QStringList findFiles(const QStringList &files, const QString &text);


     void RunCurStateOpt();

     void StepRunState();
     void StepRunStateConnect();
     void StateMachineUpDate();

     void OperationEngine();
     void savestatestring();

     void stringaioperation();

     void saveconnectstring();
     void savefilestring(const QString& fn);

     //
     QString m_curfilename;

     QString m_curparserstring;
     QString m_curstateparserstring;
     QString m_curconnectparserstring;

     int m_icurtype;
     QString m_curname;
     QString m_currentstatename;
     QString m_currentconnectname;

     QString m_curdebugvalue;

     double m_dmovx;
     double m_dmovy;
     double m_dangle;
     double m_dzoomx;
     double m_dzoomy;

private://interface
     QLabel *label;
     QLabel *label2;
     QToolButton *selectModeButton;
     QToolButton *dragModeButton;
     QToolButton *openGlButton;
     QToolButton *antialiasButton;
     QToolButton *printButton;
     QToolButton *resetButton;
     QSlider *zoomSlider;
     QSlider *rotateSlider;
};


class visionView : public QFrame
{
    Q_OBJECT
public:
    explicit visionView(const QString &name, QFrame *parent = 0);

    visionmanager *view() const;

public slots:
    void zoomIn(int level = 1);
    void zoomOut(int level = 1);

private slots:
    void resetView();
    void setResetButtonEnabled();
    void setupMatrix();
    void togglePointerMode();
    void toggleOpenGL();
    void toggleAntialiasing();
    void rotateLeft();
    void rotateRight();

private:
    visionmanager *manageView;
    QScrollArea *m_scrollArea;
    QLabel *label;
    QLabel *label2;
    QToolButton *selectModeButton;
    QToolButton *dragModeButton;
    QToolButton *openGlButton;
    QToolButton *antialiasButton;
    QToolButton *printButton;
    QToolButton *resetButton;
    QSlider *zoomSlider;
    QSlider *rotateSlider;
};

#endif // DOCUMENT_H
