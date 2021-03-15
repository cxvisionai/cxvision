
#include <qevent.h>
#include <QPainter>
#include <QTextStream>
#include <QUndoStack>
#include <QDir>
#include <QFileInfo>
#include <qcoreapplication.h>
#include <QStyle>
#include <QToolButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QLabel>
#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#else
#include <QtWidgets>
#endif
#include "visionmanager.h"
#include "shapecommands.h"

#include "shapebase.h"
#include "backimagemanager.h"
#include "FuSMAIControl.h"//statemachine



QString getlocationstringa(const QString &strfile)
{
    QString locationstring;
#if defined( Q_OS_WIN )
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
QByteArray loadfilestring(const QString& file)
{
    QFile f(file);

    QByteArray qfilereads;

    const int size = f.size();
    //const int size = m_lastFileState.size = f.size();
    if ( !f.open(QFile::ReadOnly) )
    {
        return QByteArray();
    }
    if ( size < 500000 )
    {
        // instant load for files smaller than 500kb
        QByteArray d = f.readAll();
        f.close();
        return d;
    }
    else
    {
        // load by chunks of 100kb otherwise to avoid huge peaks of memory usage
        // and driving mad the disk drivers
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

/******************************************************************************
** visionmanager
*/

visionmanager::visionmanager(QWidget *parent)
    : QWidget(parent),// m_currentIndex(-1),      m_mousePressIndex(-1),
      m_resizeHandlePressed(false),
      m_pcurshape(0),
      m_pmousepressshape(0),
        m_dmovx(0),
        m_dmovy(0),
    m_dangle(0),
    m_dzoomx(1),
    m_dzoomy(1)
{

    m_undoStack = new QUndoStack(this);

    m_FSMControl = new FuSMAIControl();

    setAutoFillBackground(true);
    setBackgroundRole(QPalette::Base);

}
visionmanager::~visionmanager()
{
   delete m_undoStack ;

   delete m_FSMControl ;

}
void visionmanager::initialparser()
{
    m_imageparser.ParserInitialClassFunction(0);
    m_imageparser.SetStream(&m_os);
    m_imageparser.SetCreateCodeStream(&m_createcodeos);

    m_imageparser.Compile("Module accd1;");
    //load module
    QString strfile = getlocationstringa("./static.cxsc");
    QString strcode = loadfilestring(strfile);
    m_imageparser.Compile(strcode.toStdString().c_str());

    //load roi ini
    m_strROIlist.clear();
    QStringList files;
    files = DirFileFind(getlocationstringa(QString("./archives/")),QString("*.cxsc"));
    for(int i=0;i<files.size();++i)
    {
        QString filename = files[i];
        QFileInfo afile(filename);
        QString getfilename = afile.completeBaseName()+tr(".")+afile.completeSuffix();

        m_stcxutlist << afile.completeBaseName();
        m_strROIlist.push_back(afile.completeBaseName());

        QString strfile = getlocationstringa("./"+getfilename);
        m_strcode = loadfilestring(strfile);

        m_imageparser.Compile(m_strcode.toStdString().c_str());
    }

    //load module
    strfile = getlocationstringa("./autocreate.cxsc");
    strcode = loadfilestring(strfile);
    m_imageparser.Compile(strcode.toStdString().c_str());

 /*
    strfile = getlocationstringx("./run.cxut");
    m_strcode = loadfilestring(strfile);
    collectionstringopt(m_strcode.toStdString().c_str(),"run");

    strfile = getlocationstringx("./imagerun.cxut");
    m_strcode = loadfilestring(strfile);
    collectionstringopt(m_strcode.toStdString().c_str(),"imagerun");

    strfile = getlocationstringx("./main.cxut");
    m_strcode = loadfilestring(strfile);
    collectionstringopt(m_strcode.toStdString().c_str(),"main");
 */
    OperationEngine();
    stringaioperation();
    strfile = getlocationstringa("./main.cxut");
    m_strcode = loadfilestring(strfile);
    //
    //  m_imageparser.SetCreateCodeStream(&m_createcodeos);
    for(int i=0;i<m_imageparser.GetClassObjSum("Image");i++)
    {
        ImageBase* pimage = (ImageBase*)m_imageparser.GetClassObj("Image",i);
        QSize imagesize(1600,1200);//(1024,768);
        if(pimage)
            *pimage=ImageBase(imagesize, QImage::Format_ARGB32_Premultiplied);
    }

}
//////////////////////////////////////////////////////////////////////////
void visionmanager::stringaioperation()
{
  StringAI *pstringai = getstringai();

  QStringList files;
  files = DirFileFind(getlocationstringa(QString("./")),QString("*.cxut"));
  for(int i=0;i<files.size();++i)
  {
      QString filename = files[i];

      QFileInfo afile(filename);
      //QString getfilename = afile.completeBaseName()+tr(".")+afile.completeSuffix();

      m_stcxutlist << afile.completeBaseName();

      QString qcxstrwho = QString("cxvisionai");
      QString qcxstrwhat = QString("what:") + afile.completeBaseName();
      QString qcxrelation = QString(">");
      QString qall = qcxstrwho+qcxrelation+qcxstrwhat;
      //pstringai->addrelation(qall.toStdString().c_str());
  }

}
void visionmanager::OperationEngine()
{
    m_stcnxutlist.clear();
    m_stcxutlist.clear();
    m_FSMControl->Init();
    m_imageparser.ClearOptMap();

    emit StateClear();

    QStringList files;
    files = DirFileFind(getlocationstringa(QString("./")),QString("*.cxut"));
    for(int i=0;i<files.size();++i)
    {
        QString filename = files[i];
        QFileInfo afile(filename);
        QString astrfile = afile.fileName();
        QString endName = afile.suffix();
        QString getfilename = afile.completeBaseName()+tr(".")+afile.completeSuffix();

        m_stcxutlist << afile.completeBaseName();
        m_FSMControl->AddState(afile.completeBaseName().toStdString());

        QString strfile = getlocationstringa("./"+getfilename);
        m_strcode = loadfilestring(strfile);
        collectionstringopt(m_strcode.toStdString().c_str(),afile.completeBaseName().toStdString().c_str());
    }
    emit OperationModelList(m_stcxutlist);

    //////////////////////////////////////////////////////////////////////////
    files = DirFileFind(getlocationstringa(QString("./")),QString("*.cnxut"));
    for(int i=0;i<files.size();++i)
    {
        QString filename = files[i];
        //char szPath[800],szDrive[800],szFileName[800],szExt[800];
        //_splitpath(filename.toLocal8Bit(),szDrive,szPath,szFileName,szExt);
        //QString qszfilename = QString::fromLocal8Bit(szFileName);

        QFileInfo afile(filename);
        QString astrfile = afile.fileName();
        QString endName = afile.suffix();
        QString getfilename = afile.completeBaseName()+tr(".")+afile.completeSuffix();

        m_stcnxutlist << afile.completeBaseName();
        QString qszfilename = afile.completeBaseName();
        //////////////////////////////////////////////////////////////////////////
        {
            QChar qchar_ ='_';
            int num_=0;
            int ifilenamesize =qszfilename.size();
            for(int j=0;j<ifilenamesize;j++)
            {
                if(qchar_==qszfilename.at(j))
                    num_ = j;
            }
            QString modellinka=qszfilename.left(num_);
            QString modellinkb=qszfilename.right(ifilenamesize-num_-1);

            m_FSMControl->AddTransition(((const char *)modellinka.toLocal8Bit()),((const char *)modellinkb.toLocal8Bit())) ;
        }
        /////////////////////////////////////////////////////////

        QString strfile = getlocationstringa("./"+getfilename);
        m_strcode = loadfilestring(strfile);
        collectionstringopt(m_strcode.toStdString().c_str(),afile.completeBaseName().toStdString().c_str());

    }
    //m_iloadengcodeok = 1;
    emit OperationModelConnectList(m_stcxutlist);

    //////////////////////////////////////////////////////////////////
    //set statemachine


}
void visionmanager::savefilestring(const QString& fn)
{
    QFile f(fn);

    if ( !f.open(QFile::WriteOnly) )
    {
        return;
    }

    f.write(m_curparserstring.toLocal8Bit());
    f.close();
}
void visionmanager::savestatestring()
{
    QString qfile = getlocationstringa(QString("./"))+QString("/")+m_currentstatename+QString(".cxut");
    //m_curparserstring = m_curstateparserstring;
    savefilestring(qfile);
    //CompileCodeToOpt(m_curparserstring);
}
void visionmanager::saveconnectstring()
{
    QString qfile =  getlocationstringa(QString("./"))+QString("/")+m_currentconnectname+QString(".cnxut");
    //m_curparserstring = m_curstateparserstring;
    savefilestring(qfile);
    //CompileCodeToOpt(m_curparserstring);
}
void visionmanager::loadstatestring()
{

    QString qfile = getlocationstringa(QString("./")) +QString("/")+m_currentstatename+QString(".cxut") ;
    m_curparserstring =loadfilestring(qfile);
    m_curstateparserstring = m_curparserstring;

    emit signals_GetStateString(m_curparserstring);
}
void visionmanager::loadconnectstring()
{
    QString qfile = getlocationstringa(QString("./")) +QString("/")+m_currentconnectname+QString(".cnxut");
    m_curparserstring =loadfilestring(qfile);

    m_curconnectparserstring= m_curparserstring;
    emit signals_GetConnectString(m_curparserstring);
}
QStringList visionmanager::DirFileFind(const QString &path,const QString &filename)
{
    QString filenameto=filename;
    QString text ;
    //! [4]
    m_currentDir = QDir(path);

    if (filenameto.isEmpty())
        filenameto = "*";
     QStringList files = m_currentDir.entryList(QStringList(filenameto),
        QDir::Files | QDir::NoSymLinks);

    if (!text.isEmpty())
        files = findFiles(files, text);
     return files;
}
QStringList visionmanager::findFiles(const QStringList &files, const QString &text)
{
//    QProgressDialog progressDialog(this);
//    progressDialog.setCancelButtonText(tr("&Cancel"));
//    progressDialog.setRange(0, files.size());
//    progressDialog.setWindowTitle(tr("Find Files"));

    //! [5] //! [6]
    QStringList foundFiles;

    for (int i = 0; i < files.size(); ++i) {
//        progressDialog.setValue(i);
//        progressDialog.setLabelText(tr("Searching file number %1 of %2...")
//            .arg(i).arg(files.size()));
//        qApp->processEvents();
        //! [6]

//        if (progressDialog.wasCanceled())
//            break;

        //! [7]
        QFile file(m_currentDir.absoluteFilePath(files[i]));

        if (file.open(QIODevice::ReadOnly)) {
            QString line;
            QTextStream in(&file);
            while (!in.atEnd()) {
                //if (progressDialog.wasCanceled())
                //    break;
                line = in.readLine();
                if (line.contains(text)) {
                    foundFiles << files[i];
                    break;
                }
            }
        }
    }
    return foundFiles;
}

void visionmanager::SetOptStateName(const QString &statename)
{
    m_currentstatename = statename;
    m_curname = statename;
    m_icurtype = 1;
}
void visionmanager::SetOptConnectName(const QString &connectname)
{
    m_currentconnectname = connectname;
    m_curname = connectname;
    m_icurtype = 2;
}
void visionmanager::RunCurStateOpt()
{
    //RunStrOpt(m_currentstatename);
    runoptcode(m_currentstatename.toStdString().c_str());
    update();
}
void visionmanager::SetParserValue(const QString & codestr,double dvalue)
{
    QString astr(codestr);
    QString numst(tr("=%1;").arg(dvalue));
    if(!m_imageparser.IsObjectVar(codestr.toLocal8Bit()))
        return ;
    astr=astr+numst;
    m_imageparser.Compile(astr.toLocal8Bit());
}
double visionmanager::GetParserValue(const QString & codestr)
{
    m_curdebugvalue = codestr;
    double *pdouble=NULL;
    if(!m_imageparser.IsObjectVar( (const char *)codestr.toLocal8Bit() ))
        return 0x00;//0xFFFF;
    //pdouble = (double *)m_arunparser.GetClassObj("double",codestr);
    pdouble = (double *)m_imageparser.GetDoubleValue( (const char *)codestr.toLocal8Bit() );
    if(NULL==pdouble)
        return 0x00;//0xFFFF;
    double dvalue=0;
    dvalue = (*pdouble);
    if(dvalue<0.0000001&&dvalue>0)
        return 0;
    return dvalue;
}
QString visionmanager::GetParserString(const QString & namestr)
{
       return  namestr;
}
QImage* visionmanager::GetParserImage(const QString & namestr)
{
    ImageBase* pimage = (ImageBase*)m_imageparser.GetClassObj("Image",namestr.toStdString().c_str());
    if(pimage!=0)
        return pimage ;
    else
        return 0;
}
void visionmanager::StepRunState()
{
    double dstateoutput;
    SetParserValue(QString("doutputvalue"),0);
    m_FSMControl->SetRunState(string((const char *)m_currentstatename.toLocal8Bit()));
    runoptcode(m_currentstatename.toStdString().c_str());
    emit RunState(m_currentstatename);
    update();
}
void visionmanager::StepRunStateConnect()
{
    double dstateoutput;
    int iconstate = m_FSMControl->GetRunStateConnectSum();
    QString qconnectname;
    QString qnextstatename;
    dstateoutput=GetParserValue(QString("doutputvalue"));
    if(dstateoutput>0)
    for(int i=0;i<iconstate;i++)
    {
        SetParserValue(QString("doutputvalue"),0);
        qconnectname =QString::fromLocal8Bit(m_FSMControl->GetRunStateConnectName(i).c_str()) ;
        qnextstatename =QString::fromLocal8Bit(m_FSMControl->GetRunStateConnectStateName(i).c_str()) ;
        runoptcode(qconnectname.toStdString().c_str());
        emit RunConnect(qconnectname);
        dstateoutput=GetParserValue(QString("doutputvalue"));
        if(dstateoutput>0)
        {
            SetOptStateName(qnextstatename);
            return;
        }
    }
    update();
}
void visionmanager::StateMachineUpDate()
{
    StepRunState();
    StepRunStateConnect();
    double dvalue = GetParserValue(m_curdebugvalue);
    GetValue(dvalue);
}

QString visionmanager::addShape(const QShape &shape)
{
    QString name = QShape::typeToString(shape.type());
    name = uniqueName(name);
    assert(0);
    //m_shapeList.append(shape);
    //m_shapeList[m_shapeList.count() - 1].m_name = name;
    //setCurrentShape(m_shapeList.count() - 1);

    return name;
}

void visionmanager::deleteShape(const QString &shapeName)
{
    assert(0);
  //  int index = indexOf(shapeName);
  //  if (index == -1)
  //      return;

  //  update(m_shapeList.at(index).rect());

  //  m_shapeList.removeAt(index);

  //  if (index <= m_currentIndex) {
  //      m_currentIndex = -1;
  //      if (index == m_shapeList.count())
  //          --index;
   //     setCurrentShape(index);
   // }
}

QShape visionmanager::shape(const QString &shapeName)
{
    QShape *pshape = indexOf(shapeName);
    if (pshape == 0)
        return QShape();
    return *pshape;
}

void visionmanager::setShapeRect(const QString &shapeName, const QRect &rect)
{
    QShape* pshape = indexOf(shapeName);
    if (pshape == 0)
        return;

    update(pshape->rect());
    update(rect);

    pshape->m_rect = rect;
}
dialogOPT * visionmanager::getopt()
{
    dialogOPT *pdialogopt = (dialogOPT*)m_imageparser.GetClassObj("dialogopt","opt");
    return pdialogopt;
}
StringAI *visionmanager::getstringai()
{

    StringAI *pstringai = (StringAI*)m_imageparser.GetClassObj("StringAI","astrai");
    return pstringai;

}
void visionmanager::setShapeColor(const QString &shapeName, const QColor &color)
{

    QShape *pshape = indexOf(shapeName);
    if (pshape == 0)
        return;

    pshape->m_color = color;

    update(pshape->rect());
}

QUndoStack *visionmanager::undoStack() const
{
    return m_undoStack;
}

bool visionmanager::load(QTextStream &stream)
{
/*    m_shapeList.clear();

    while (!stream.atEnd()) {
        QString shapeType, shapeName, colorName;
        int left, top, width, height;
        stream >> shapeType >> shapeName >> colorName >> left >> top >> width >> height;
        if (stream.status() != QTextStream::Ok)
            return false;
        bool ok;
        Shape::Type type = Shape::stringToType(shapeType, &ok);
        if (!ok)
            return false;
        QColor color(colorName);
        if (!color.isValid())
            return false;

        QShape shape(type);
        shape.m_name = shapeName;
        shape.m_color = color;
        shape.m_rect = QRect(left, top, width, height);

        m_shapeList.append(shape);
    }

    m_currentIndex = m_shapeList.isEmpty() ? -1 : 0;
*/
    return true;
}

void visionmanager::save(QTextStream &stream)
{
/*
    for (int i = 0; i < m_shapeList.count(); ++i) {
        const QShape &shape = m_shapeList.at(i);
        QRect r = shape.rect();
        stream << Shape::typeToString(shape.type()) << QLatin1Char(' ')
                << shape.name() << QLatin1Char(' ')
                << shape.color().name() << QLatin1Char(' ')
                << r.left() << QLatin1Char(' ')
                << r.top() << QLatin1Char(' ')
                << r.width() << QLatin1Char(' ')
                << r.height();
        if (i != m_shapeList.count() - 1)
            stream << QLatin1Char('\n');
    }
    m_undoStack->setClean();
*/
}

QString visionmanager::fileName() const
{
    return m_fileName;
}

void visionmanager::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}

QShape* visionmanager::indexAt(const QPoint &pos)
{
    int isize =m_imageparser.GetClassObjSum("Shape");
    for(int i=0;i<isize;i++)
    {
        QShape* pshape = (QShape*)m_imageparser.GetClassObj("Shape",i);

        if (pshape->rect().contains(pos))
            if(pshape->show())
            return pshape;
    }
    isize =m_imageparser.GetClassObjSum("findline");
    for(int i=0;i<isize;i++)
    {
        QShape* pshape = (QShape*)m_imageparser.GetClassObj("findline",i);

        if (pshape->rect().contains(pos))
            if(pshape->show())
            return pshape;
    }
    isize =m_imageparser.GetClassObjSum("findobject");
    for(int i=0;i<isize;i++)
    {
        QShape* pshape = (QShape*)m_imageparser.GetClassObj("findobject",i);

        if (pshape->rect().contains(pos))
            if(pshape->show())
            return pshape;
    }
    isize =m_imageparser.GetClassObjSum("gridobject");
    for(int i=0;i<isize;i++)
    {
        QShape* pshape = (QShape*)m_imageparser.GetClassObj("gridobject",i);

        if (pshape->rect().contains(pos))
            if(pshape->show())
            return pshape;
    }
    isize =m_imageparser.GetClassObjSum("fastmatch");
    for(int i=0;i<isize;i++)
    {
        QShape* pshape = (QShape*)m_imageparser.GetClassObj("fastmatch",i);

        if (pshape->rect().contains(pos))
            if(pshape->show())
            return pshape;
    }
    isize =m_imageparser.GetClassObjSum("easyorc");
    for(int i=0;i<isize;i++)
    {
        QShape* pshape = (QShape*)m_imageparser.GetClassObj("easyorc",i);

        if (pshape->rect().contains(pos))
            if(pshape->show())
            return pshape;
    }


    return 0;

}

void visionmanager::mousePressEvent(QMouseEvent *event)
{
    event->accept();
    QShape* pshape = indexAt(event->pos());
    if (pshape != 0)
    {
        setCurrentShape(pshape);

        m_resizeHandlePressed = pshape->resizeHandle().contains(event->pos());

        if (m_resizeHandlePressed)
            m_mousePressOffset = pshape->rect().bottomRight() - event->pos();
        else
            m_mousePressOffset = event->pos() - pshape->rect().topLeft();
    }
    m_pmousepressshape = pshape;
}

void visionmanager::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
    m_pmousepressshape = 0;

}

void visionmanager::mouseMoveEvent(QMouseEvent *event)
{
    event->accept();

    if (m_pmousepressshape == 0)
        return;

    const QShape &shape = *m_pmousepressshape;

    QRect rect;
    if (m_resizeHandlePressed) {
        rect = QRect(shape.rect().topLeft(), event->pos() + m_mousePressOffset);
    } else {
        rect = shape.rect();
        rect.moveTopLeft(event->pos() - m_mousePressOffset);
    }

    QSize size = rect.size().expandedTo(QShape::minSize);
    rect.setSize(size);

     m_pmousepressshape->setrect(rect.x(),rect.y(),rect.width(),rect.height());

    update();

  //  m_undoStack->push(new SetShapeRectCommand(this, shape.name(), rect));
}

void visionmanager::paintEvent(QPaintEvent *event)
{

    QMutexLocker locker(&m_runmutex);
    //image show
    ImageBase* pshowimage = 0 ;
    for(int i=0;i<m_imageparser.GetClassObjSum("Image");i++)
    {
        ImageBase* pimage = (ImageBase*)m_imageparser.GetClassObj("Image",i);
        if(pimage->getshow()==1)
           pshowimage=pimage;
    }

    QRegion paintRegion = event->region();
    QPainter painter(this);


        painter.save();
        painter.translate(m_dmovx, m_dmovy);
        painter.rotate(m_dangle);
        painter.scale(m_dzoomx, m_dzoomy);


        if(1)
        {
            if(0!=pshowimage)
            {
                pshowimage->draw(painter);
            }

            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

            for(int i=0;i<m_imageparser.GetClassObjSum("Module");i++)
            {
                BackImageManager* pmodule = (BackImageManager*)m_imageparser.GetClassObj("Module",i);
                if(pmodule->show()>0)
                   pmodule->draw(painter);
            }

            QPalette pal = palette();

           // ShapeBase* pshapebase = 0 ;
            for(int i=0;i<m_imageparser.GetClassObjSum("ShapeBase");i++)
            {
                ShapeBase* pshapebase = (ShapeBase*)m_imageparser.GetClassObj("ShapeBase",i);

                if(pshapebase->show()>0)
                    pshapebase->drawshape(painter);
            }
            //draw lineshape
            for(int i=0;i<m_imageparser.GetClassObjSum("LineShape");i++)
            {
                LineShape* plineshape = (LineShape*)m_imageparser.GetClassObj("LineShape",i);
                QPoint apoint = plineshape->getline().p1();
                QPoint bpoint = plineshape->getline().p2();
                if (!paintRegion.contains(QRect(apoint,bpoint)))
                    continue;
                if(plineshape->show()>0)
                    plineshape->drawshape(painter);
            }
            //draw shape and object
          //QShape* pshape = 0 ;
            for(int i=0;i<m_imageparser.GetClassObjSum("Shape");i++)
            {
                QShape* pshape = (QShape*)m_imageparser.GetClassObj("Shape",i);

                if (!paintRegion.contains(pshape->rect()))
                    continue;
                if(pshape->show()>0)
                    pshape->drawshape(painter,pal);
            }

        }

        painter.restore();

}
void visionmanager::setCurrentShape(QShape *pshape)
{
    QString currentName;

    if (pshape != 0)
        update(pshape->rect());

    m_pcurshape = pshape;

    if (m_pcurshape != 0)
    {
        update(m_pcurshape->rect());
        currentName = m_pcurshape->name();
    }

    emit currentShapeChanged(currentName);
}

QShape* visionmanager::indexOf(const QString &shapeName)
{
//   for (int i = 0; i < m_shapeList.count(); ++i) {
//       if (m_shapeList.at(i).name() == shapeName)
//           return i;
//    }
//    return -1;

    QShape* pshape = 0 ;
    for(int i=0;i<m_imageparser.GetClassObjSum("Shape");i++)
    {
        QShape* pshape = (QShape*)m_imageparser.GetClassObj("Shape",i);

        if (pshape->name() == shapeName)
            return pshape;
    }
    return 0;

}

QString visionmanager::uniqueName(const QString &name)
{
    QString unique;

    for (int i = 0; ; ++i) {
        unique = name;
        if (i > 0)
            unique += QString::number(i);
        if (indexOf(unique) == 0)
            break;
    }

    return unique;
}

QString visionmanager::currentShapeName()
{
 //   if (m_currentIndex == -1)
 //       return QString();
 //   return m_shapeList.at(m_currentIndex).name();
    if(m_pcurshape==0)
          return QString();
    return m_pcurshape->name();
}
void visionmanager::saveeditcode(const char *pcodes,const char *filenames)
{
    QString fn = getlocationstringa(filenames);
    QFile f(fn);

    if ( !f.open(QFile::WriteOnly) )
    {
        return;
    }

    f.write(pcodes);//codeeditor->toPlainText().toStdString().c_str());
    f.close();
}
void visionmanager::runoptcode(const char *optstr)
{
     QMutexLocker locker(&m_runmutex);

m_elapsed.start();

    qDebug() << QString("runopt :")+QString(optstr);

    clearos();
    runopt(QString(optstr));

    m_imageparser.RunOptString(optstr);

    runoptover(QString(optstr));

qDebug("elapsed %lld ms", m_elapsed.restart());

}

void visionmanager::autorun(const char *optstr)
{
    runoptcode(optstr);
}

void visionmanager::autorunstringcode(const char *optstr)
{
    QMutexLocker locker(&m_runmutex);
    m_elapsed.start();
    m_imageparser.Compile(optstr);
    qDebug("elapsed %lld ms", m_elapsed.restart());
}
void visionmanager::runstringcode(const char *optstr)
{
    QMutexLocker locker(&m_runmutex);


    m_elapsed.start();

    m_imageparser.Compile(optstr);

   // qDebug("elapsed %lld ms", m_elapsed.restart());

    m_os << "================elapsed: " << (int)m_elapsed.restart() << " ms============\r\n";
}
void visionmanager::runstringcodex(const char *optstr)
{
    QMutexLocker locker(&m_runmutex);
    m_elapsed.start();
    m_imageparser.Compile(optstr);
    qDebug("elapsed %lld ms", m_elapsed.restart());
}

void visionmanager::mutexupdate()
{
        update();
}
void visionmanager::collectionstringopt(const char *optcode,const char *optstrname)
{

    m_imageparser.SetOptCollect(true);

    m_imageparser.Compile(optcode);

    m_imageparser.SetOptCollect(false);

    m_imageparser.SetRunOpt(optstrname);

    qDebug() << QString("<-collectionstringopt :")+QString(optstrname);

}
void visionmanager::trigger(const QImage& imginput)
{

    ImageBase* pinputimage = (ImageBase*)m_imageparser.GetClassObj("Image","aimage");
    *pinputimage = imginput;
    runoptcode("imagerun");

}
void visionmanager::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    update();
}
void visionmanager::setimageframe(QImage& imginput)
{


    ImageBase* pimage = (ImageBase*)m_imageparser.GetClassObj("Image","aimage");

    imginput = imginput.convertToFormat(QImage::Format_ARGB32_Premultiplied);

    QImage resultImage = imginput;
    QPainter painter(&resultImage);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(resultImage.rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, imginput);
    painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
    painter.drawImage(0, 0, *pimage);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    painter.fillRect(resultImage.rect(), Qt::white);
    painter.end();
    *pimage = resultImage;

    double dvaluef = GetParserValue("df");
    dvaluef = dvaluef + 1;
    SetParserValue("df",dvaluef);

}
void visionmanager::setimage(QImage& imginput)
{


    ImageBase* pimage = (ImageBase*)m_imageparser.GetClassObj("Image","aimage");

    imginput = imginput.convertToFormat(QImage::Format_ARGB32_Premultiplied);

    *pimage = imginput;

    double dvaluef = GetParserValue("df");
    dvaluef = dvaluef + 1;
    SetParserValue("df",dvaluef);

}
void visionmanager::saveautocreate()
{
    QString qstrcreate;
    m_imageparser.GetImageObjectAutoSave();
    qstrcreate=(m_createcodeos.str().c_str());
    clearcreateos();

    QString qfile =  getlocationstringa(QString("./"))+QString("/autocreate.cxsc");

    QFile f(qfile);

    if ( !f.open(QFile::ReadWrite) )
    {
        return;
    }

    f.write(qstrcreate.toStdString().c_str());
    f.close();
}
void visionmanager::clearos()
{
    m_os.str("");
    m_os.clear();
}
void visionmanager::clearcreateos()
{
    m_createcodeos.str("");
    m_createcodeos.clear();
}
void visionmanager::zoomin(double dzoomx,double dzoomy)
{
    m_dzoomx = dzoomx;
    m_dzoomy = dzoomy;
}
void visionmanager::rotateangle(double dangle)
{
    m_dangle = dangle;
}




visionView::visionView(const QString &name, QFrame *parent)
    : QFrame(parent)
{

  /*  graphicsView = new GraphicsView(this);
    graphicsView->setRenderHint(QPainter::Antialiasing, false);
    graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
*/
    manageView = new visionmanager(this);
    manageView->setMinimumHeight(2700);
    manageView->setMinimumWidth(3600);

    m_scrollArea = new QScrollArea;
    m_scrollArea->setMinimumHeight(640);
    m_scrollArea->setMinimumWidth(800);


    m_scrollArea->setBackgroundRole(QPalette::Dark);
    m_scrollArea->setWidget(manageView);
    m_scrollArea->setVisible(true);


    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);

    QToolButton *zoomInIcon = new QToolButton;
    zoomInIcon->setAutoRepeat(true);
    zoomInIcon->setAutoRepeatInterval(33);
    zoomInIcon->setAutoRepeatDelay(0);
    zoomInIcon->setIcon(QPixmap(":/zoomin.png"));
    zoomInIcon->setIconSize(iconSize);
    QToolButton *zoomOutIcon = new QToolButton;
    zoomOutIcon->setAutoRepeat(true);
    zoomOutIcon->setAutoRepeatInterval(33);
    zoomOutIcon->setAutoRepeatDelay(0);
    zoomOutIcon->setIcon(QPixmap(":/zoomout.png"));
    zoomOutIcon->setIconSize(iconSize);
    zoomSlider = new QSlider;
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(500);
    zoomSlider->setValue(250);
    zoomSlider->setTickPosition(QSlider::TicksRight);

    // Zoom slider layout
    QVBoxLayout *zoomSliderLayout = new QVBoxLayout;
    zoomSliderLayout->addWidget(zoomInIcon);
    zoomSliderLayout->addWidget(zoomSlider);
    zoomSliderLayout->addWidget(zoomOutIcon);

    QToolButton *rotateLeftIcon = new QToolButton;
    rotateLeftIcon->setIcon(QPixmap(":/rotateleft.png"));
    rotateLeftIcon->setIconSize(iconSize);
    QToolButton *rotateRightIcon = new QToolButton;
    rotateRightIcon->setIcon(QPixmap(":/rotateright.png"));
    rotateRightIcon->setIconSize(iconSize);
    rotateSlider = new QSlider;
    rotateSlider->setOrientation(Qt::Horizontal);
    rotateSlider->setMinimum(-360);
    rotateSlider->setMaximum(360);
    rotateSlider->setValue(0);
    rotateSlider->setTickPosition(QSlider::TicksBelow);

    // Rotate slider layout
    QHBoxLayout *rotateSliderLayout = new QHBoxLayout;
    rotateSliderLayout->addWidget(rotateLeftIcon);
    rotateSliderLayout->addWidget(rotateSlider);
    rotateSliderLayout->addWidget(rotateRightIcon);

    resetButton = new QToolButton;
    resetButton->setText(tr("0"));
    resetButton->setEnabled(false);

    // Label layout
    QHBoxLayout *labelLayout = new QHBoxLayout;
    label = new QLabel(name);
    label2 = new QLabel(tr("Pointer Mode"));
    selectModeButton = new QToolButton;
    selectModeButton->setText(tr("Select"));
    selectModeButton->setCheckable(true);
    selectModeButton->setChecked(true);
    dragModeButton = new QToolButton;
    dragModeButton->setText(tr("Drag"));
    dragModeButton->setCheckable(true);
    dragModeButton->setChecked(false);
    antialiasButton = new QToolButton;
    antialiasButton->setText(tr("Antialiasing"));
    antialiasButton->setCheckable(true);
    antialiasButton->setChecked(false);
    openGlButton = new QToolButton;
    openGlButton->setText(tr("OpenGL"));
    openGlButton->setCheckable(true);
#ifndef QT_NO_OPENGL
    openGlButton->setEnabled(QGLFormat::hasOpenGL());
#else
    openGlButton->setEnabled(false);
#endif
    printButton = new QToolButton;
    printButton->setIcon(QIcon(QPixmap(":/fileprint.png")));

    QButtonGroup *pointerModeGroup = new QButtonGroup(this);
    pointerModeGroup->setExclusive(true);
    pointerModeGroup->addButton(selectModeButton);
    pointerModeGroup->addButton(dragModeButton);

    labelLayout->addWidget(label);
    labelLayout->addStretch();
    labelLayout->addWidget(label2);
    labelLayout->addWidget(selectModeButton);
    labelLayout->addWidget(dragModeButton);
    labelLayout->addStretch();
    labelLayout->addWidget(antialiasButton);
    labelLayout->addWidget(openGlButton);
    labelLayout->addWidget(printButton);

    QGridLayout *topLayout = new QGridLayout;
    //topLayout->addLayout(labelLayout, 0, 0);
    topLayout->addWidget(m_scrollArea, 0, 0);
    topLayout->addLayout(zoomSliderLayout, 0, 1);
    topLayout->addLayout(rotateSliderLayout, 1, 0);
    topLayout->addWidget(resetButton, 1, 1);
    setLayout(topLayout);

    connect(resetButton, SIGNAL(clicked()), this, SLOT(resetView()));
    connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
    connect(rotateSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
//    connect(graphicsView->verticalScrollBar(), SIGNAL(valueChanged(int)),
//            this, SLOT(setResetButtonEnabled()));
//    connect(graphicsView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
//            this, SLOT(setResetButtonEnabled()));
    connect(selectModeButton, SIGNAL(toggled(bool)), this, SLOT(togglePointerMode()));
    connect(dragModeButton, SIGNAL(toggled(bool)), this, SLOT(togglePointerMode()));
    connect(antialiasButton, SIGNAL(toggled(bool)), this, SLOT(toggleAntialiasing()));
    connect(openGlButton, SIGNAL(toggled(bool)), this, SLOT(toggleOpenGL()));
    connect(rotateLeftIcon, SIGNAL(clicked()), this, SLOT(rotateLeft()));
    connect(rotateRightIcon, SIGNAL(clicked()), this, SLOT(rotateRight()));
    connect(zoomInIcon, SIGNAL(clicked()), this, SLOT(zoomIn()));
    connect(zoomOutIcon, SIGNAL(clicked()), this, SLOT(zoomOut()));
    connect(printButton, SIGNAL(clicked()), this, SLOT(print()));

    setupMatrix();
}

visionmanager *visionView::view() const
{
    return static_cast<visionmanager *>(manageView);
}

void visionView::resetView()
{
    zoomSlider->setValue(250);
    rotateSlider->setValue(0);
 //   setupMatrix();
 //   manageView->ensureVisible(QRectF(0, 0, 0, 0));
    manageView->zoomin(1,1 );
    manageView->rotateangle(0);
    manageView->update();
    resetButton->setEnabled(false);
}

void visionView::setResetButtonEnabled()
{
    resetButton->setEnabled(true);
}

void visionView::setupMatrix()
{
    qreal scale = qPow(qreal(2), (zoomSlider->value() - 250) / qreal(50));

    QMatrix matrix;
    matrix.scale(scale, scale);
    matrix.rotate(rotateSlider->value());

//    manageView->setMatrix(matrix);
    setResetButtonEnabled();
}

void visionView::togglePointerMode()
{
//    manageView->setDragMode(selectModeButton->isChecked()
//                              ? QGraphicsView::RubberBandDrag
//                              : QGraphicsView::ScrollHandDrag);
//    manageView->setInteractive(selectModeButton->isChecked());
}

void visionView::toggleOpenGL()
{
#ifndef QT_NO_OPENGL
//    manageView->setViewport(openGlButton->isChecked() ? new QGLWidget(QGLFormat(QGL::SampleBuffers)) : new QWidget);
#endif
}

void visionView::toggleAntialiasing()
{
//    manageView->setRenderHint(QPainter::Antialiasing, antialiasButton->isChecked());
}

void visionView::zoomIn(int level)
{
    int icur = zoomSlider->value() + level -250;
    zoomSlider->setValue(zoomSlider->value() + level);
    manageView->zoomin(1+icur*0.05,1+icur*0.05);
    manageView->update();
}

void visionView::zoomOut(int level)
{
    int icur = zoomSlider->value() - level -250;
    zoomSlider->setValue(zoomSlider->value() - level);
    manageView->zoomin(1+icur*0.05,1+icur*0.05);
    manageView->update();
}

void visionView::rotateLeft()
{
    int ivalue = rotateSlider->value() - 1;
    rotateSlider->setValue(rotateSlider->value() - 1);
    manageView->rotateangle(ivalue);
        manageView->update();
}

void visionView::rotateRight()
{
    int ivalue = rotateSlider->value() + 1;
    rotateSlider->setValue(rotateSlider->value() + 1);
    manageView->rotateangle(ivalue);
        manageView->update();
}
