
#include <QTest>
#include"stringai.h"
//#include"cximage/orc.h"

#define MAXSEARCHLEVEL 50
StringAI::StringAI():
    m_ibufsize(30),
    m_isearchlevel(3)
{

}
StringAI::~StringAI()
{

}
QString  StringAI::getlocationstring(const QString &strfile)
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
void StringAI::updateurltree(const char *purl)
{
    saveurltreecreate(QString(purl));
}
void StringAI::addrelation(const char *pkeyandlabel)
{
    QString strkeyandrelation(pkeyandlabel);
    QStringList alist = strkeyandrelation.split(">");
    QString qkeystr = alist.at(0);
    QString strrelation = alist.at(1);
    saverelationcreate(qkeystr,strrelation);
    saveurltreecreate(strrelation);
}
void StringAI::addurl(const char *pkeyandlabel)
{
    QString strkeyandrelation(pkeyandlabel);
    QStringList alist = strkeyandrelation.split(">");
    QString qkeystr = alist.at(0);
    QString strrelation = alist.at(1);
    QString strparm = "1";
    if(alist.size()>2)
        strparm = alist.at(2);
    saverelationcreate(qkeystr,strrelation);
    saveurltreecreate(strrelation);
}

void StringAI::saverelationcreate(const QString &qkeystr,const QString &qstr)
{

    QString strdirpath;
    strdirpath =  getlocationstring("./relation/"+qkeystr + "/all.txt");
    QFileInfo afileinf(strdirpath);
    urltreemkdir( afileinf.absolutePath());

    FILE * fp = 0;
     fp=fopen(strdirpath.toStdString().c_str(),"at");
     QString savestr = qstr+"\r\n";
     fprintf(fp,savestr.toStdString().c_str());
     fclose(fp);

    QString qstrhttp = qstr;
    if(qstrhttp.contains(":"))
    {
        QStringList strthing = qstrhttp.split(":");
        if(strthing[0]!=QString("http")
           ||strthing[0]!=QString("https")
           ||strthing[0]!=QString("ftp")
           ||strthing[0]!=QString("file"))
        {
            return;
        }
    }
    else
    {
        strdirpath =  getlocationstring("./relation/"+qkeystr+ "/" +qstr);
        QFileInfo afileinf0(strdirpath);
        urltreemkdir( afileinf0.absoluteFilePath());
    }

}
void StringAI::saveurltreecreate(const QString &qkeystr,const QString &strurl)
{
    saverelationcreate(qkeystr,strurl);
    saveurltreecreate(strurl);
}
void StringAI::saveurltreecreate(const QString &strurl)
{
    QUrl qurl = strurl;
    QString strhost = qurl.host();
    if(strhost.isEmpty())
        return;

    QString strpath = qurl.path();//split
    QString strfile = qurl.fileName();//query
    QStringList strlist = strpath.split("/");
    QString strlast = strurl.right(1);
    QString strdirpath;
    strdirpath =  getlocationstring("./url");
    strdirpath = strdirpath + "/"+strhost+strpath;

    QFileInfo afileinf(strdirpath);
    urltreemkdir( afileinf.absolutePath());
    strdirpath = afileinf.absolutePath();
    QString savefilename = strdirpath + "/all.txt";

    FILE * fp = 0;

    fp=fopen(savefilename.toStdString().c_str(),"at");

    fprintf(fp,QString(strurl+"\r\n").toStdString().c_str());

    fclose(fp);

}
void StringAI::saveurltreelistcreate(const QStringList &strurllist)
{
    for(int i=0;i<strurllist.size();i++)
    {
       saveurltreecreate(strurllist[i]);
    }
}
void StringAI::setsearchlabel(const QString &qkeystr,const QString &qstr)
{
    QString strdirpath;
    strdirpath =  getlocationstring("./relation/"+qkeystr+ "/searched.txt");

    QFileInfo afileinf(strdirpath);
    urltreemkdir( afileinf.absolutePath());

    FILE * fp = 0;

    fp=fopen(strdirpath.toStdString().c_str(),"at");

    QString savestr = qkeystr+">"+qstr+">1\r\n";
    fprintf(fp,savestr.toStdString().c_str());
    fclose(fp);

}
void StringAI::setsearchurl(const QString &qkeystr,const QString &qstrurl)
{
    QString strdirpath;
    strdirpath =  getlocationstring("./relation/"+qkeystr + "/searched.txt");
    QFileInfo afileinf(strdirpath);
    urltreemkdir( afileinf.absolutePath());


    FILE * fp = 0;
    fp=fopen(strdirpath.toStdString().c_str(),"at");
    QString savestr = qkeystr+">"+qstrurl+">1\r\n";
    fprintf(fp,savestr.toStdString().c_str());
    fclose(fp);

    QUrl qurl = qstrurl;
    QString strhost = qurl.host();
    QString strpath = qurl.path();//split
    QString strfile = qurl.fileName();//query
    QStringList strlist = strpath.split("/");
    QString strlast = qstrurl.right(1);
    strdirpath =  getlocationstring("./url");
    strdirpath = strdirpath + "/"+strhost + strpath;

    QFileInfo afileinf0(strdirpath);
    urltreemkdir( afileinf0.absolutePath());
    strdirpath = afileinf0.absolutePath();
    strdirpath = strdirpath + "/searched.txt";

    fp = 0;
    fp=fopen(strdirpath.toStdString().c_str(),"at");
    savestr = qkeystr+">"+qstrurl+">1\r\n";
    fprintf(fp,savestr.toStdString().c_str());
    fclose(fp);
}
void StringAI::loadurltreefiletonode(const QString &qstr)
{
    QString strdirpath;
    strdirpath =  getlocationstring("./"+qstr+ "/searched.txt");

    QStringList searchedlist;
    QFile file(strdirpath);
    if (file.open(QIODevice::ReadOnly))
    {
        QString line;
        QTextStream in(&file);
        while (!in.atEnd())
        {
             QString line = in.readLine();
             QStringList strlist = line.split(">");
             if(strlist.size()<3)
             {
                 if(qstr==strlist[0])
                 {
                     searchedlist.push_back(strlist[1]);
                 }
             }
             else
             {//>FINDSTRING>D
                 if("D"==strlist[2])
                 {
                     //TODO
                 }
             }
        }
    }
    searchedlist.removeDuplicates();





    //

    QUrl qurl = qstr;
    QString strhost = qurl.host();
    QString strpath = qurl.path();//split
    QString strfile = qurl.fileName();//query
    QStringList strlist = strpath.split("/");
    QString strlast = qstr.right(1);

    strdirpath =  getlocationstring("./url");
    strdirpath = strdirpath + "/"+strhost + strpath + "/all.txt";

    QStringList afilestringlist;
    QFile file0(strdirpath);
    if (file0.open(QIODevice::ReadOnly))
    {
        QString line;
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            if(!searchedlist.contains(line))
                afilestringlist.push_back(line);
               // m_searchmap.insert(qstr, line);
        }
    }
    ///

    afilestringlist.removeDuplicates();
    for(int i=0;i<afilestringlist.size();i++)
    {
        if(!searchedlist.contains(afilestringlist[i]))
            m_searchmap[qstr].push_back(afilestringlist[i]);
    }
    //resave
   QString strwrite;
    for(int i=0;i<afilestringlist.size();i++)
      strwrite +=  afilestringlist[i]+"\r\n";
   QFile f(strdirpath);
    if (f.open(QFile::WriteOnly) )
    {
        f.write(strwrite.toStdString().c_str());
        f.close();
    }
    ///
    QFileInfo afileinf(strdirpath);
    QDir cxDir = afileinf.absoluteDir();

    QStringList filelist = cxDir.entryList(QDir::AllEntries, QDir::Name);
    for(int i=0;i<filelist.size();i++)
    {
        if(filelist[i]!="."&&
           filelist[i]!=".."&&
           filelist[i]!="all.txt"&&
           filelist[i]!="searched.txt")
            if((!searchedlist.contains(filelist[i]))
             &&(!searchedlist.contains(filelist[i]+"/")))
                m_searchmap[qstr].push_back("son:"+filelist[i]);
    }
    //

    cxDir.cdUp();
    filelist = cxDir.entryList(QDir::AllEntries, QDir::Name);
    for(int i=0;i<filelist.size();i++)
    {
        if(filelist[i]!="."&&
           filelist[i]!=".."&&
           filelist[i]!="all.txt"&&
           filelist[i]!="searched.txt"&&
           filelist[i]!="url"
                )
            if((!searchedlist.contains(filelist[i]))
               &&(!searchedlist.contains(filelist[i]+"/")))
                 m_searchmap[qstr].push_back("parent:"+filelist[i]);
    }
}
void StringAI::loadrelationtreefiletonode(const QString &qstr)
{
    QString strdirpath = getlocationstring("./relation/"+qstr+ "/searched.txt");

    QStringList searchedlist;
    QFile file0(strdirpath);
    if (file0.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file0);
        while (!in.atEnd())
        {
             QString line = in.readLine();
             QStringList strlist = line.split(">");
              if(strlist.size()<3)
              {
                  if(qstr==strlist[0])
                  {
                      //searchedlist[qstr] = strlist[1];
                      searchedlist.push_back(strlist[1]);
                  }

              }
              else
              {
                  //todo
              }
        }
    }
     searchedlist.removeDuplicates();
    strdirpath =  getlocationstring("./relation/"+qstr+ "/all.txt");
    QStringList afilestringlist;
    QFile file(strdirpath);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);

        while (!in.atEnd())
        {
              QString line = in.readLine();
              afilestringlist.push_back(line);
              //if(searchedlist[qstr]!=line)
              //    m_searchmap.insert(qstr, line);
          }
      }
    file.close();
    afilestringlist.removeDuplicates();
    for(int i=0;i<afilestringlist.size();i++)
    {
        if(!searchedlist.contains(afilestringlist[i]))
            m_searchmap[qstr].push_back(afilestringlist[i]);
    }
    //resave
   QString strwrite;
    for(int i=0;i<afilestringlist.size();i++)
      strwrite +=  afilestringlist[i]+"\r\n";
   QFile f(strdirpath);
    if (f.open(QFile::WriteOnly) )
    {
        f.write(strwrite.toStdString().c_str());
        f.close();
    }


    ////////////
    QFileInfo afileinf(strdirpath);
    QDir cxDir = afileinf.absoluteDir();

    QStringList filelist = cxDir.entryList(QDir::AllEntries, QDir::Name);
    for(int i=0;i<filelist.size();i++)
    {
        if(filelist[i]!="."&&
           filelist[i]!=".."&&
                filelist[i]!="all.txt"&&
                filelist[i]!="searched.txt")
            if(!searchedlist.contains(filelist[i]))
               m_searchmap[qstr].push_back("son:"+filelist[i]);
    }
    //
    filelist.clear();

    cxDir.cdUp();
    filelist = cxDir.entryList(QDir::AllEntries, QDir::Name);
    for(int i=0;i<filelist.size();i++)
    {
        if(filelist[i]!="."&&
           filelist[i]!=".."&&
           filelist[i]!="all.txt"&&
           filelist[i]!="searched.txt"&&
           filelist[i]!=qstr&&
           filelist[i]!="relation")
            if(!searchedlist.contains(filelist[i])) 
                m_searchmap[qstr].push_back("parent:"+filelist[i]);
    }
}
QString StringAI::runstep()
{
    if(m_corelist.size()<=0)
    {
        update();
    }
    if(m_corelist.size()>0)
    {
        QString qstr = m_corelist[0];
        m_corelist.pop_front();
        return qstr;
    }
    return "";
}
void StringAI::runfilter()
{
    for(int iz=0;iz<m_corelist.size();iz++)
    {
      QString qclientsend = m_corelist[iz];
      QString strurl = m_corelist[iz];
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
       if(ifilter>0)
           m_corelist.removeOne(m_corelist.at(iz));
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
void StringAI::addextfilter(const char *str)
{
    if(m_filefilterlist.contains(QString(str)))
        return;
    m_filefilterlist.append(QString(str));
    m_filefilterlist.removeDuplicates();
}
void StringAI::addkeywordfilter(const char *str)
{
    if(m_keywordfilterlist.contains(QString(str)))
        return;
    m_keywordfilterlist.append(QString(str));
    m_keywordfilterlist.removeDuplicates();
}
void StringAI::addneedstringlist(const char *str)
{
    if(m_strneedstringlist.contains(QString(str)))
        return;
    m_strneedstringlist.append(QString(str));
    m_strneedstringlist.removeDuplicates();
}
void StringAI::addsearchkey(const char *pkeychar)
{
    QString strkey = QString(pkeychar);

    if(m_strkey.contains(pkeychar))
        return;
    m_strkey.push_back(strkey);
    loadrelationtreefiletonode(strkey);
    relationkeyscan(strkey,0,m_isearchlevel);

    for(int j=0;j<MAXSEARCHLEVEL;j++)
    {
      QStringList strlist = m_urlmap.keys(j);
      if(strlist.size()>0)
          m_corelist += strlist;
    }

    runfilter();
}
void StringAI::clearkeys()
{
    m_strkey.clear();
}
void StringAI::setrelationlabel(const char *pkeyandlabel)
{
    QString strkeyandrelation(pkeyandlabel);
    QStringList alist = strkeyandrelation.split(">");
    QString qkeystr = alist.at(0);
    QString strrelation = alist.at(1);
    QString strparm = "1";
    if(alist.size()>2)
        strparm = alist.at(2);

    QString strdirpath;
    strdirpath =  getlocationstring("./relation/"+qkeystr+ "/searched.txt");



    QFileInfo afileinf(strdirpath);
    urltreemkdir( afileinf.absolutePath());

    FILE * fp = 0;

    fp=fopen(strdirpath.toStdString().c_str(),"at");

    QString savestr = qkeystr+">"+strrelation+">"+strparm+"\r\n";
    fprintf(fp,savestr.toStdString().c_str());
    fclose(fp);

}
void StringAI::seturllabel(const char *pkeyandlabel)
{
    QString strkeyandrelation(pkeyandlabel);
    QStringList alist = strkeyandrelation.split(">");
    QString qkeystr = alist.at(0);
    QString qstrurl = alist.at(1);
    QString strparm = "1";
    if(alist.size()>2)
        strparm = alist.at(2);

    QString strdirpath;
    strdirpath =  getlocationstring("./relation/"+qkeystr + "/searched.txt");


    QFileInfo afileinf(strdirpath);
    urltreemkdir( afileinf.absolutePath());

    FILE * fp = 0;

    fp=fopen(strdirpath.toStdString().c_str(),"at");

    QString savestr = qkeystr+">"+qstrurl+">"+strparm+"\r\n";
    fprintf(fp,savestr.toStdString().c_str());
    fclose(fp);

    QUrl qurl = qstrurl;
    QString strhost = qurl.host();
    QString strpath = qurl.path();//split

    strdirpath =  getlocationstring("./url");
    strdirpath = strdirpath + "/"+strhost + strpath;

    QFileInfo afileinf0(strdirpath);
    urltreemkdir( afileinf0.absolutePath());
    strdirpath = afileinf0.absolutePath();
    strdirpath = strdirpath + "/searched.txt";

    fp = 0;
    fp=fopen(strdirpath.toStdString().c_str(),"at");
    savestr = qkeystr+">"+qstrurl+">"+strparm+"\r\n";
    fprintf(fp,savestr.toStdString().c_str());
    fclose(fp);

}
void StringAI::update()
{
    int isize = m_strkey.size();
    for(int i=0;i<isize;i++)
    {
        loadrelationtreefiletonode(m_strkey[i]);
        relationkeyscan(m_strkey[i],0,m_isearchlevel);
    }

    for(int j=0;j<MAXSEARCHLEVEL;j++)
    {
      QStringList strlist =  m_urlmap.keys(j);
      if(strlist.size()>0)
          m_corelist += strlist;
    }

    runfilter();
}
void StringAI::clear()
{
    m_strkey.clear();
    m_urlmap.clear();
    m_searchmap.clear();
}
void StringAI::relationkeyscan(const QString &strkey,int ilevel,int iendlevel)
{
    if(ilevel>=iendlevel)
        return;
    QStringList values = m_searchmap[strkey];

    values.removeDuplicates();
    m_searchmap[strkey] = values;
    for (int i = 0; i < values.size(); ++i)
    {
        QStringList strthing = values.at(i).split(QLatin1String(":"));
        if(strthing[0]==QString("http")
           ||strthing[0]==QString("https")
           ||strthing[0]==QString("ftp")
           ||strthing[0]==QString("file"))
        {
            if(m_urlmap.contains(values.at(i)))
            {
                if(m_urlmap[values.at(i)]>ilevel)
                {
                    m_urlmap[values.at(i)] = ilevel;
                    setsearchurl(strkey,values.at(i));
                }
            }
            else
            {
                m_urlmap[values.at(i)] = ilevel;
                setsearchurl(strkey,values.at(i));
            }
            QUrl aurl(values.at(i));
            QString strhost = strthing[0]+"://"+aurl.host();
            QString strpath = strthing[0]+"://"+aurl.host()+aurl.path();
             if(m_urlmap.contains(strhost))
             {
                 if(m_urlmap[strhost]>ilevel+1)
                 {
                     m_urlmap[strthing[0]+"://"+aurl.host()] = ilevel + 1;
                     setsearchlabel(strkey,aurl.host());
                 }
             }
             else
             {
                 m_urlmap[strthing[0]+"://"+aurl.host()] = ilevel + 1;
                 setsearchlabel(strkey,aurl.host());
             }
             if(m_urlmap.contains(strpath))
             {
                 if(m_urlmap[strpath]>ilevel+1)
                 {
                     m_urlmap[strthing[0]+"://"+aurl.host()+aurl.path()] = ilevel + 1;
                     setsearchlabel(strkey,aurl.host()+aurl.path());
                 }

             }
             else
             {
                 m_urlmap[strthing[0]+"://"+aurl.host()+aurl.path()] = ilevel + 1;
                 setsearchlabel(strkey,aurl.host()+aurl.path());
             }
            if(ilevel+1 < iendlevel)
            {
                loadurltreefiletonode(strhost);
                relationkeyscan(strhost,ilevel + 1, iendlevel);

            }
            if(ilevel+1 < iendlevel)
            {
                loadurltreefiletonode(strpath);
                relationkeyscan(strpath,ilevel + 1, iendlevel);
            }
        }
        else if(strthing[0]==QString("son"))
        {
            if(ilevel+1 < iendlevel)
            {
               loadrelationtreefiletonode(strthing[1]);
               relationkeyscan(strthing[1],ilevel + 1, iendlevel);
            }
        }
        else if(strthing[0]==QString("parent"))
        {
            if(ilevel+1 < iendlevel)
            {
                loadrelationtreefiletonode(strthing[1]);
                relationkeyscan(strthing[1],ilevel + 1, iendlevel);
            }
        }
    }
}

QStringList StringAI::parserstring_what(QString qstr)
{
    QString strdirpath;
    strdirpath =  getlocationstring("./relation/"+qstr+ "/all.txt");


    QStringList searchedlist;
    QFile file(strdirpath);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
             QString line = in.readLine();
             QStringList strlist = line.split(":");
             if(strlist.size()<3)
             {
                 if(QString("what")==strlist[0])
                 {
                     searchedlist.push_back(strlist[1]);
                 }
             }
        }
    }
    searchedlist.removeDuplicates();

    return searchedlist;
}
QStringList StringAI::parserstring_who(QString qstr)
{
    QString strdirpath;
    strdirpath =  getlocationstring("./relation/"+qstr+ "/all.txt");

    QStringList searchedlist;
    QFile file(strdirpath);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
             QString line = in.readLine();
             QStringList strlist = line.split(":");
             if(strlist.size()<3)
             {
                 if(QString("who")==strlist[0])
                 {
                     searchedlist.push_back(strlist[1]);
                 }
             }
        }
    }
    searchedlist.removeDuplicates();

    return searchedlist;
}
QStringList StringAI::parserstring_why(QString qstr)
{
    QString strdirpath;
    strdirpath =  getlocationstring("./relation/"+qstr+ "/all.txt");

    QStringList searchedlist;
    QFile file(strdirpath);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
             QString line = in.readLine();
             QStringList strlist = line.split(":");
             if(strlist.size()<3)
             {
                 if(QString("why")==strlist[0])
                 {
                     searchedlist.push_back(strlist[1]);
                 }
             }
        }
    }
    searchedlist.removeDuplicates();

    return searchedlist;
}
QStringList StringAI::parserstring_where(QString qstr)
{
    QString strdirpath;
    strdirpath =  getlocationstring("./relation/"+qstr+ "/all.txt");

    QStringList searchedlist;
    QFile file(strdirpath);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
             QString line = in.readLine();
             QStringList strlist = line.split(":");
             if(strlist.size()<3)
             {
                 if(QString("where")==strlist[0])
                 {
                     searchedlist.push_back(strlist[1]);
                 }
             }
        }
    }
    searchedlist.removeDuplicates();

    return searchedlist;
}
QStringList StringAI::parserstring_when(QString qstr)
{
    QString strdirpath;
    strdirpath =  getlocationstring("./relation/"+qstr+ "/all.txt");

    QStringList searchedlist;
    QFile file(strdirpath);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
             QString line = in.readLine();
             QStringList strlist = line.split(":");
             if(strlist.size()<3)
             {
                 if(QString("when")==strlist[0])
                 {
                     searchedlist.push_back(strlist[1]);
                 }
             }
        }
    }
    searchedlist.removeDuplicates();

    return searchedlist;
}
QStringList StringAI::parserstring_same(QString qstr)
{
    QString strdirpath;
    strdirpath =  getlocationstring("./relation/"+qstr+ "/all.txt");

    QStringList searchedlist;
    QFile file(strdirpath);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
             QString line = in.readLine();
             QStringList strlist = line.split(":");
             if(strlist.size()<3)
             {
                 if(QString("same")==strlist[0])
                 {
                     searchedlist.push_back(strlist[1]);
                 }
             }
        }
    }
    searchedlist.removeDuplicates();

    return searchedlist;
}

QString StringAI::cxvisionaiparserstring(QString qstrwho,QString qstrwhat,QString qstrwhen,QString qstrwhere,QString qstrwhy)
{
    QStringList qstrwhatlist = parserstring_what(qstrwho);
    QStringList qstrsamelist = parserstring_same(qstrwhat);
    qstrsamelist.push_front(qstrwhat);
    int isize0 = qstrwhatlist.size();
    int isize1 = qstrsamelist.size();
    for(int ia=0;ia<isize0;ia++)
    {
        for(int ib=0;ib<isize1;ib++)
        {
            if(qstrwhatlist[ia]==qstrsamelist[ib])
            {
                return qstrwhatlist[ia];
            }
        }
    }
    return QString("");
}


void StringAI::clearstring()
{
    m_strtemplist.clear();
}
void StringAI::addstring(const char *pchar)
{
    m_strtemplist.append(pchar);
}
void StringAI::addocrstring(void *pocr)
{
    //QEasyORC *apocr = (QEasyORC *)pocr;
    //addstring(apocr->getreslultstring().toStdString().c_str());
}
QString StringAI::getstring(int inum)
{
    if(inum>=0&&inum<m_strtemplist.size())
        return m_strtemplist[inum];
    else
        return m_strtemplist[m_strtemplist.size()-1];
}

