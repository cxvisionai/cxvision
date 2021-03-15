
#include <QTest>
#include"stringai.h"
QString  StringAI::getlocationstring(const QString &strfile)
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
void StringAI::urltreemkdir(const QString &astr)
{
            const QDir cxDir(astr);
            if (!cxDir.exists())
            {
                if (!cxDir.mkpath(astr))
                {
                    return ;
                }
            }
}
void StringAI::saveurltreecreate(const QString &strurl)
{
        QUrl qurl = strurl;
        QString strhost = qurl.host();
        QString strpath = qurl.path();//split
        QString strfile = qurl.fileName();//query
        QStringList strlist = strpath.split("/");
        QString strlast = strurl.right(1);


        QString strdirpath;
        strdirpath =  getlocationstring("./url");
        strdirpath = strdirpath + "/"+strhost;
        int isize =strlist.size()-1 ;
        for(int i=0;i<isize;i++)
        {
          if(!strlist[i].isEmpty())
          {
           strdirpath = strdirpath + "/"+strlist[i];
           urltreemkdir(strdirpath);
          }
        }
        if(strlast!="/")
        {
            QString savefilename = strdirpath + "./all.txt";

            FILE * fp = 0;
            fp=fopen(savefilename.toStdString().c_str(),"at");

            fprintf(fp,strurl.toStdString().c_str());

            fclose(fp);



        }
}
void StringAI::saveurltreelistcreate(const QStringList &strurllist)
{
    for(int i=0;i<strurllist.size();i++)
    {
       saveurltreecreate(strurllist[i]);
    }
}
QStringList StringAI::loadurltreefiletobuff(const QString &str)
{
    QUrl qurl = str;
    QString strhost = qurl.host();
    QString strpath = qurl.path();//split
    QString strfile = qurl.fileName();//query
    QStringList strlist = strpath.split("/");
    QString strlast = str.right(1);

    QStringList alist;
    QString strdirpath;
    strdirpath =  getlocationstring("./url");
    strdirpath = strdirpath + "/"+strhost + strpath + "/all.txt";

    QFile file(strdirpath);
    if (file.open(QIODevice::ReadOnly))
        {
            QString line;
           QTextStream in(&file);
         while (!in.atEnd())
           {
              QString line = in.readLine();
             alist.push_back(line);
            }
        }
    return alist;
} //

QStringList StringAI::loadrelationtreefiletobuff(const QString &qstr)
{
    QStringList alist;
    QString strdirpath;
    strdirpath =  getlocationstring("./"+qstr);
    strdirpath = strdirpath + "/"+qstr + "/all.txt";

    QFile file(strdirpath);
    if (file.open(QIODevice::ReadOnly))
    {
            QString line;
           QTextStream in(&file);
            while (!in.atEnd())
           {
              QString line = in.readLine();
              //
              int i=0;
              for(i = 0;i<line.size() ;i++)
              {
                  QChar achar = line.at(i);
                  if(achar==':')
                      break;
              }
              strdate = line.mid(0,i);
              if(i<line.size())
                  line = line.mid(i+1);
              //
             alist.push_back(line);
            }
        }
    return alist;
}
QString StringAI::runstep()
{
    m_corelist.pop_front();
   // for(int iz=0;iz<m_corelist.size();iz++)
    {
        QString qclientsend = m_corelist[i];
        QString strurl = m_corelist[i];
        int ifilefiltersize = m_filefilterlist.size();
        int ifilter = 0;
       int ineedstrsize = m_strneedstringlist.size();
       if(ineedstrsize>0)
           ifilter = 1;
       int igetneednum = 0;
       for(int i=0;i<ineedstrsize;i++)
       {
           bool bfilter = strurl.contains(m_strneedstringlist[i]);
           if(bfilter==true)
               igetneednum = igetneednum + 1;
       }
       if(igetneednum==ineedstrsize)
           ifilter = 0;
       for(int i=0;i<ifilefiltersize;i++)
       {
           QFileInfo afile(strurl);
           QString strsuffix = afile.completeSuffix();
           if(m_filefilterlist[i]==strsuffix)
           ifilter += 1;
       }
      int ifiltersize = m_keywordfilterlist.size();
      for(int i=0;i<ifiltersize;i++)
      {
         QString strfilter = m_keywordfilterlist[i];
         bool bfilter = strurl.contains(strfilter);
         if(bfilter==true)
         ifilter += 1;
      }

        if(ifilter<1)
        {
           m_pageviewlist[0]->currentTab()->setUrl(qclientsend.mid(4));
          if(0!=m_pdialogopt)
               m_pdialogopt->setcurstring(m_qclientsend.mid(4).toStdString().c_str());
           m_pageviewlist[0]->setcollectionvalidnum();
           m_pageviewlist[0]->maketabviewone();

           m_localdutylist.pop_front();
           int isize =  widgetList3->count();
           if(isize>0)
           {
               QListWidgetItem *pitem = widgetList3->takeItem(0);
               if(pitem)
                   widgetList3->removeItemWidget(pitem);
           }
           return;
        }




    }

 }
QStringList StringAI::prelist()
{
  return  m_corelist.mid(0,10);
}

void StringAI::clearfilter()
{
    m_filefilterlist.clear();
    m_keywordfilterlist.clear();
    m_strneedstringlist.clear();
}
void StringAI::addextfilter(const QString &str)
{
    m_filefilterlist.append(str);

    m_filefilterlist.removeDuplicates();
}
void StringAI::addkeywordfilter(const QString &str)
{
    m_keywordfilterlist.append(str);
    m_keywordfilterlist.removeDuplicates();
}
void StringAI::addneedstringlist(const QString &str)
{
    m_strneedstringlist.append(str);
    m_strneedstringlist.removeDuplicates();
}
