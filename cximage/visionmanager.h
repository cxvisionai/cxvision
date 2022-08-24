/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef visionmanager_H
#define visionmanager_H

#include "imagebase.h"
#include "ImageParserClass.h"
#include <QFrame>
#include <QMutex>
#include <QWaitCondition>
#include <QDir>
#include <QGraphicsScene>
#include <QElapsedTimer>
#include <QOpenGLWidget>
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

class visionmanager : public QOpenGLWidget
{
    Q_OBJECT

public:

    visionmanager(QWidget *parent = 0);
    ~visionmanager();

    QString initialparser();
    void interfaceini();

    void clearparserobject();
    void resetparser();
    QString addShape(const QShape &shape);
    void deleteShape(const QString &shapeName);
    QShape shape(const QString &shapeName);
    QString currentShapeName();
    void setparserstring(const QString &parserstring){m_curparserstring=parserstring;}
    void savecalibfile();
    QString getcodeparserstring();
    QString getcodeparsernumstring(int inum);
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
    QString collectionstringopt(const char *optcode,const char *optstr);
    void trigger(const QImage& imginput);
    void setimage(QImage& imginput);
    void setimagelist(QImage& imginput);
    void setimageframe(QImage& imginput);
    QString getoutputstring(){return QString(m_os.str().c_str());}
    void cameracalib(const QString& qnamecalib);
    void SetParserValue(const QString & codestr,double dvalue);
    double GetParserValue(const QString & codestr);
    QImage* GetParserImage(const QString & namestr);
    QString GetParserString(const QString & namestr);
    void runoptcode(const char *optstr);

    QString getloadcode(){return m_strcode;}
    dialogOPT *getopt();
    StringAI *getstringai();
    void mutexupdate();
    void flashview();
    void clearos();
    void clearcreateos();
    void saveautocreate();
    void autorun(const char *optstr);
    void autorunstringcode(const char *optstr);
    void zoomin(double dzoomx,double dzoomy);
    void rotateangle(double dangle);
    QString getcurrentstatename(){return m_currentstatename;}
    void editviewenable();
    void closeflash();
    static QElapsedTimer g_runelapsed;
    static int g_showdebug;
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

    //static bool m_block;
    //bool m_bruntimes;
    QMutex m_runmutex;
    int m_flashview;
    QPixmap m_pixmap;
    //QMutex m_flashmutex;
    //QWaitCondition m_flashcondition;


    QElapsedTimer m_elapsed;
    QElapsedTimer m_flashelapsed;

    qint64 m_lcalnum;
    qint64 m_lflashecalnum;


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

     QString OperationEngine();
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

     bool m_beditmanagerview;
     bool m_bcloseflash;

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
    void verticalScroll();
    void horizontalScroll();
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
