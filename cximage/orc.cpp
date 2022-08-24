
#include<QFileInfo>
#include<QDir>

#include<QTime>
#include<QCoreApplication>
#include<QApplication>
#include<QClipboard>
#include<QEventLoop>

#include "orc.h"
#include "findobject.h"
#include "backimagemanager.h"


QEasyORC::QEasyORC():fastmatch(),
    m_igridw(12),//10
    m_igridh(12),
    m_idebugrectsnum(-1),
    m_idebugfontnum(-1),
    m_image_thre(110),
    m_findobj_distance(6),
    m_findobj_searchtype(444),
    m_findobj_brow(1),
    m_findobj_minarea(0),
    m_findobj_maxarea(10000),
    m_findobj_minw(0),
    m_findobj_maxw(9999),
    m_findobj_minh(0),
    m_findobj_maxh(9999),
    m_findobj_bgedge(2),
    m_dmatchthre(0.5),
    m_icompareobject(3),
    m_ix_or(0),
    m_iy_or(0),
    m_ix_and(0),
    m_iy_and(0),
    m_idraw_x(-1),
    m_idraw_y(-1),
    m_idraw_map(0),
    m_igridwh(12),
    m_exnum(999),
    m_findobj_ioffsetx0(0),
    m_findobj_ioffsetx1(0),
    m_findobj_ioffsety0(0),
    m_findobj_ioffsety1(0),
    m_imagetype(3),
    m_resultnodesearchsum(3)
{
  setname("OCR");
  fastmatch::setfindnum(1);
  setmatchthre(3);
  fastmatch::setthre(8);
  fastmatch::setcomparegap(2);
  fastmatch::setmethod(0);
  fastmatch::setlinegap(1);
  fastmatch::SetWHgap(1,1);
  fastmatch::setlinesamplerate(0.002);
  fastmatch::setfilter(0,0,10000);
  fastmatch::setfindnum(1);
  fastmatch::setmatchthre(3);
  fastmatch::setfindnum(1);

  clearmodel();
  loadfontmodel();
  levelmodel();
  setlevelstring();


  QShape::setrect(30,30,200,200);
  int icurmodule = BackImageManager::GetCurMode();
  g_pbackimage = BackImageManager::GetBackImage(icurmodule);
  g_pbackobjectimage = BackImageManager::GetBackObjectImage(icurmodule);
  g_pbackfindobject = BackImageManager::Getbackfindobject(icurmodule);

  m_pimagegrid  = new QGrid;
  m_pimagegrid->setshow(8);
  m_pimagegrid->setgrid(5,5,64,64,5,5);
}


QEasyORC::~QEasyORC()
{
    delete m_pimagegrid;
    mapclear();
}

void QEasyORC::setrect(int ix, int iy, int iw, int ih)
{
   QShape::setrect(ix,iy,iw,ih);
}

QString QEasyORC::char2string(QString pchar)
{
    if(QString(pchar)==" "){return QString("space");}
    else if(QString(pchar)=="!"){return QString("exclam");}
    else if(QString(pchar)=="\""){return QString("quotedbl");}
    else if(QString(pchar)=="#"){return QString("numbersign");}
    else if(QString(pchar)=="$"){return QString("dollar");}
    else if(QString(pchar)=="%"){return QString("percent");}
    else if(QString(pchar)=="&"){return QString("ampersand");}
    else if(QString(pchar)=="'"){return QString("apostrophe");}
    else if(QString(pchar)=="'"){return QString("quoteright");}
    else if(QString(pchar)=="("){return QString("parenleft");}
    else if(QString(pchar)==")"){return QString("parenright");}
    else if(QString(pchar)=="*"){return QString("asterisk");}
    else if(QString(pchar)=="+"){return QString("plus");}
    else if(QString(pchar)==","){return QString("comma");}
    else if(QString(pchar)=="-"){return QString("minus");}
    else if(QString(pchar)=="."){return QString("period");}
    else if(QString(pchar)=="/"){return QString("slash");}
    else if(QString(pchar)=="0"){return QString("0");}
    else if(QString(pchar)=="1"){return QString("1");}
    else if(QString(pchar)=="2"){return QString("2");}
    else if(QString(pchar)=="3"){return QString("3");}
    else if(QString(pchar)=="4"){return QString("4");}
    else if(QString(pchar)=="5"){return QString("5");}
    else if(QString(pchar)=="6"){return QString("6");}
    else if(QString(pchar)=="7"){return QString("7");}
    else if(QString(pchar)=="8"){return QString("8");}
    else if(QString(pchar)=="9"){return QString("9");}
    else if(QString(pchar)==":"){return QString("colon");}
    else if(QString(pchar)==";"){return QString("semicolon");}
    else if(QString(pchar)=="<"){return QString("less");}
    else if(QString(pchar)=="="){return QString("equal");}
    else if(QString(pchar)==">"){return QString("greater");}
    else if(QString(pchar)=="?"){return QString("question");}
    else if(QString(pchar)=="@"){return QString("at");}
    else if(QString(pchar)=="A"){return QString("A");}
    else if(QString(pchar)=="B"){return QString("B");}
    else if(QString(pchar)=="C"){return QString("C");}
    else if(QString(pchar)=="D"){return QString("D");}
    else if(QString(pchar)=="E"){return QString("E");}
    else if(QString(pchar)=="F"){return QString("F");}
    else if(QString(pchar)=="G"){return QString("G");}
    else if(QString(pchar)=="H"){return QString("H");}
    else if(QString(pchar)=="I"){return QString("I");}
    else if(QString(pchar)=="J"){return QString("J");}
    else if(QString(pchar)=="K"){return QString("K");}
    else if(QString(pchar)=="L"){return QString("L");}
    else if(QString(pchar)=="M"){return QString("M");}
    else if(QString(pchar)=="N"){return QString("N");}
    else if(QString(pchar)=="O"){return QString("O");}
    else if(QString(pchar)=="P"){return QString("P");}
    else if(QString(pchar)=="Q"){return QString("Q");}
    else if(QString(pchar)=="R"){return QString("R");}
    else if(QString(pchar)=="S"){return QString("S");}
    else if(QString(pchar)=="T"){return QString("T");}
    else if(QString(pchar)=="U"){return QString("U");}
    else if(QString(pchar)=="V"){return QString("V");}
    else if(QString(pchar)=="W"){return QString("W");}
    else if(QString(pchar)=="X"){return QString("X");}
    else if(QString(pchar)=="Y"){return QString("Y");}
    else if(QString(pchar)=="Z"){return QString("Z");}
    else if(QString(pchar)=="["){return QString("bracketleft");}
    else if(QString(pchar)=="\\"){return QString("backslash");}
    else if(QString(pchar)=="]"){return QString("bracketright");}
    else if(QString(pchar)=="^"){return QString("asciicircum");}
    else if(QString(pchar)=="_"){return QString("underscore");}
    else if(QString(pchar)=="`"){return QString("grave");}
    else if(QString(pchar)=="`"){return QString("quoteleft");}
    else if(QString(pchar)=="a"){return QString("a");}
    else if(QString(pchar)=="b"){return QString("b");}
    else if(QString(pchar)=="c"){return QString("c");}
    else if(QString(pchar)=="d"){return QString("d");}
    else if(QString(pchar)=="e"){return QString("e");}
    else if(QString(pchar)=="f"){return QString("f");}
    else if(QString(pchar)=="g"){return QString("g");}
    else if(QString(pchar)=="h"){return QString("h");}
    else if(QString(pchar)=="i"){return QString("i");}
    else if(QString(pchar)=="j"){return QString("j");}
    else if(QString(pchar)=="k"){return QString("k");}
    else if(QString(pchar)=="l"){return QString("l");}
    else if(QString(pchar)=="m"){return QString("m");}
    else if(QString(pchar)=="n"){return QString("n");}
    else if(QString(pchar)=="o"){return QString("o");}
    else if(QString(pchar)=="p"){return QString("p");}
    else if(QString(pchar)=="q"){return QString("q");}
    else if(QString(pchar)=="r"){return QString("r");}
    else if(QString(pchar)=="s"){return QString("s");}
    else if(QString(pchar)=="t"){return QString("t");}
    else if(QString(pchar)=="u"){return QString("u");}
    else if(QString(pchar)=="v"){return QString("v");}
    else if(QString(pchar)=="w"){return QString("w");}
    else if(QString(pchar)=="x"){return QString("x");}
    else if(QString(pchar)=="y"){return QString("y");}
    else if(QString(pchar)=="z"){return QString("z");}
    else if(QString(pchar)=="{"){return QString("braceleft");}
    else if(QString(pchar)=="|"){return QString("bar");}
    else if(QString(pchar)=="}"){return QString("braceright");}
    else if(QString(pchar)=="~"){return QString("asciitilde");}
    else return QString(pchar);
}
QString QEasyORC::string2char(const QString &strchar)
{
    if(strchar==QString("space")){return QString(" ");}
    else if(strchar==QString("exclam")){return QString("!");}
    else if(strchar==QString("quotedbl")){return QString("\"");}
    else if(strchar==QString("numbersign")){return QString("#");}
    else if(strchar==QString("dollar")){return QString("$");}
    else if(strchar==QString("percent")){return QString("%");}
    else if(strchar==QString("ampersand")){return QString("&");}
    else if(strchar==QString("apostrophe")){return QString("'");}
    else if(strchar==QString("quoteright")){return QString("'");}
    else if(strchar==QString("parenleft")){return QString("(");}
    else if(strchar==QString("parenright")){return QString(")");}
    else if(strchar==QString("asterisk")){return QString("*");}
    else if(strchar==QString("plus")){return QString("+");}
    else if(strchar==QString("comma")){return QString(",");}
    else if(strchar==QString("minus")){return QString("-");}
    else if(strchar==QString("period")){return QString(".");}
    else if(strchar==QString("slash")){return QString("/");}
    else if(strchar==QString("0")){return QString("0");}
    else if(strchar==QString("1")){return QString("1");}
    else if(strchar==QString("2")){return QString("2");}
    else if(strchar==QString("3")){return QString("3");}
    else if(strchar==QString("4")){return QString("4");}
    else if(strchar==QString("5")){return QString("5");}
    else if(strchar==QString("6")){return QString("6");}
    else if(strchar==QString("7")){return QString("7");}
    else if(strchar==QString("8")){return QString("8");}
    else if(strchar==QString("9")){return QString("9");}
    else if(strchar==QString("colon")){return QString(":");}
    else if(strchar==QString("semicolon")){return QString(";");}
    else if(strchar==QString("less")){return QString("<");}
    else if(strchar==QString("equal")){return QString("=");}
    else if(strchar==QString("greater")){return QString(">");}
    else if(strchar==QString("question")){return QString("?");}
    else if(strchar==QString("at")){return QString("@");}
    else if(strchar==QString("A")){return QString("A");}
    else if(strchar==QString("B")){return QString("B");}
    else if(strchar==QString("C")){return QString("C");}
    else if(strchar==QString("D")){return QString("D");}
    else if(strchar==QString("E")){return QString("E");}
    else if(strchar==QString("F")){return QString("F");}
    else if(strchar==QString("G")){return QString("G");}
    else if(strchar==QString("H")){return QString("H");}
    else if(strchar==QString("I")){return QString("I");}
    else if(strchar==QString("J")){return QString("J");}
    else if(strchar==QString("K")){return QString("K");}
    else if(strchar==QString("L")){return QString("L");}
    else if(strchar==QString("M")){return QString("M");}
    else if(strchar==QString("N")){return QString("N");}
    else if(strchar==QString("O")){return QString("O");}
    else if(strchar==QString("P")){return QString("P");}
    else if(strchar==QString("Q")){return QString("Q");}
    else if(strchar==QString("R")){return QString("R");}
    else if(strchar==QString("S")){return QString("S");}
    else if(strchar==QString("T")){return QString("T");}
    else if(strchar==QString("U")){return QString("U");}
    else if(strchar==QString("V")){return QString("V");}
    else if(strchar==QString("W")){return QString("W");}
    else if(strchar==QString("X")){return QString("X");}
    else if(strchar==QString("Y")){return QString("Y");}
    else if(strchar==QString("Z")){return QString("Z");}
    else if(strchar==QString("bracketleft")){return QString("[");}
    else if(strchar==QString("backslash")){return QString("\\");}
    else if(strchar==QString("bracketright")){return QString("]");}
    else if(strchar==QString("asciicircum")){return QString("^");}
    else if(strchar==QString("underscore")){return QString("_");}
    else if(strchar==QString("grave")){return QString("`");}
    else if(strchar==QString("quoteleft")){return QString("`");}
    else if(strchar==QString("a")){return QString("a");}
    else if(strchar==QString("b")){return QString("b");}
    else if(strchar==QString("c")){return QString("c");}
    else if(strchar==QString("d")){return QString("d");}
    else if(strchar==QString("e")){return QString("e");}
    else if(strchar==QString("f")){return QString("f");}
    else if(strchar==QString("g")){return QString("g");}
    else if(strchar==QString("h")){return QString("h");}
    else if(strchar==QString("i")){return QString("i");}
    else if(strchar==QString("j")){return QString("j");}
    else if(strchar==QString("k")){return QString("k");}
    else if(strchar==QString("l")){return QString("l");}
    else if(strchar==QString("m")){return QString("m");}
    else if(strchar==QString("n")){return QString("n");}
    else if(strchar==QString("o")){return QString("o");}
    else if(strchar==QString("p")){return QString("p");}
    else if(strchar==QString("q")){return QString("q");}
    else if(strchar==QString("r")){return QString("r");}
    else if(strchar==QString("s")){return QString("s");}
    else if(strchar==QString("t")){return QString("t");}
    else if(strchar==QString("u")){return QString("u");}
    else if(strchar==QString("v")){return QString("v");}
    else if(strchar==QString("w")){return QString("w");}
    else if(strchar==QString("x")){return QString("x");}
    else if(strchar==QString("y")){return QString("y");}
    else if(strchar==QString("z")){return QString("z");}
    else if(strchar==QString("braceleft")){return QString("{");}
    else if(strchar==QString("bar")){return QString("|");}
    else if(strchar==QString("braceright")){return QString("}");}
    else if(strchar==QString("asciitilde")){return QString("~");}
    else return QString(strchar);
}
void QEasyORC::mapclear()
{
    int isize = m_pgrids_l72.size();
    for(int i=0;i<isize;i++)
    {
        QGrid *pgrid = m_pgrids_l72[i];
        if(pgrid)
            delete pgrid;
    }
    m_pgrids_l72.clear();
 //
    isize = m_pgrids_l36.size();
    for(int i=0;i<isize;i++)
    {
        QGrid *pgrid = m_pgrids_l36[i];
        if(pgrid)
            delete pgrid;
    }
    m_pgrids_l36.clear();

//
    isize = m_pgrids_l12.size();
    for(int i=0;i<isize;i++)
    {
        QGrid *pgrid = m_pgrids_l12[i];
        if(pgrid)
            delete pgrid;
    }
    m_pgrids_l12.clear();

    //
         isize = m_pgrids_l6.size();
        for(int i=0;i<isize;i++)
        {
            QGrid *pgrid = m_pgrids_l6[i];
            if(pgrid)
                delete pgrid;
        }
        m_pgrids_l6.clear();
    //

    isize = m_pgrids_l3.size();
    for(int i=0;i<isize;i++)
    {
        QGrid *pgrid = m_pgrids_l3[i];
        if(pgrid)
            delete pgrid;
    }
    m_pgrids_l3.clear();

}
void QEasyORC::mapgrid()
{
    int ilevel = m_ilevle;
    mapclear();

    int ihnumx = 0;
    int ihnumy = 0;
    int il4size = imagefastmodelsize(4);
    for(int i=0;i<il4size;i++)
    {
        QGrid *pgrid = new QGrid;
        pgrid->setshow(8);
        pgrid->setroi(2050 + ihnumy * 72,72*ihnumx+30,72,72);
        if(50*ihnumx+50 > 2000)
        {
            ihnumx = 0;
            ihnumy = ihnumy + 1;
        }
        else
        {
            ihnumx = ihnumx + 1;
        }
        pgrid->setgrid(2,2,72,72,2,2);
        pgrid->SetModelWH(72,72);

        SelectModel(4,i);
        pgrid->SetFastModel(*getcurimagemodel());
        m_pgrids_l72.push_back(pgrid);
    }


    ihnumx = 0;
    ihnumy = 0;
    int il3size = imagefastmodelsize(3);
    for(int i=0;i<il3size;i++)
    {
        QGrid *pgrid = new QGrid;
        pgrid->setshow(8);
        pgrid->setroi(1650 + ihnumy * 72,72*ihnumx+30,72,72);
        if(50*ihnumx+50 > 3000)
        {
            ihnumx = 0;
             ihnumy = ihnumy + 1;
        }
        else
        {
            ihnumx = ihnumx + 1;
        }
        pgrid->setgrid(2,2,36,36,2,2);
        pgrid->SetModelWH(36,36);

        SelectModel(3,i);
        pgrid->SetFastModel(*getcurimagemodel());
        m_pgrids_l36.push_back(pgrid);
    }

    ihnumx = 0;
    ihnumy = 0;
    int il2size = imagefastmodelsize(2);
    for(int i=0;i<il2size;i++)
    {
        QGrid *pgrid = new QGrid;
        pgrid->setshow(8);
        pgrid->setroi(900 + ihnumy * 30,30*ihnumx+30,20,20);

        if(50*ihnumx+50 > 1500)
        {
            ihnumx = 0;
             ihnumy = ihnumy + 1;
        }
        else
        {
            ihnumx = ihnumx + 1;
        }
        pgrid->setgrid(2,2,12,12,2,2);
        pgrid->SetModelWH(12,12);

        SelectModel(2,i);
        pgrid->SetFastModel(*getcurimagemodel());
        m_pgrids_l12.push_back(pgrid);
    }
    //
    ihnumy = 0;
    ihnumx = 0;
    int il1size = imagefastmodelsize(1);
    for(int i=0;i<il1size;i++)
    {
        QGrid *pgrid = new QGrid;
        pgrid->setshow(8);
        pgrid->setroi(120 + ihnumy * 30,30*ihnumx+30,20,20);
        if(30*ihnumx+30 > 700)
        {
            ihnumx = 0;
             ihnumy = ihnumy + 1;
        }
        else
        {
            ihnumx = ihnumx + 1;
        }
        pgrid->setgrid(4,4,6,6,4,4);
        pgrid->SetModelWH(6,6);

        SelectModel(1,i);
        pgrid->SetFastModel(*getcurimagemodel());
        m_pgrids_l6.push_back(pgrid);
    }
    //
    int il0size = imagefastmodelsize(0);
    ihnumy = 0;
    ihnumx = 0;
    for(int i=0;i<il0size;i++)
    {
        QGrid *pgrid = new QGrid;
        pgrid->setshow(8);
        pgrid->setroi(20+ihnumy*30,30*ihnumx+30,20,20);
        if(30*ihnumx+30 > 700)
        {
            ihnumx = 0;
             ihnumy = ihnumy + 1;
        }
        else
        {
            ihnumx = ihnumx + 1;
        }
        pgrid->setgrid(8,8,3,3,8,8);
        pgrid->SetModelWH(3,3);

        SelectModel(0,i);
        pgrid->SetFastModel(*getcurimagemodel());
        m_pgrids_l3.push_back(pgrid);
    }

    m_ilevle = ilevel;

}
QString QEasyORC::filenametoORCstring(QString strbase)
{
    QString strkey ;
    QString strkey2 ;
    QString strkey3 ;
    QString strkey4 ;
    QStringList strsplit = strbase.split('_');
    QString split0,split1,split2,split3,split4;
    if(strsplit.size()>0)split0 = strsplit[0];
    if(strsplit.size()>1)split1 = strsplit[1];
    if(strsplit.size()>2)split2 = strsplit[2];
    if(strsplit.size()>3)split3 = strsplit[3];
    if(strsplit.size()>4)split4 = strsplit[4];
    QString strshow ;
    if(split0=="")
    {
         if(split1== ""){strkey = QString("_");}
         else if(split1== "1"){strkey = QString("\/");}
         else if(split1== "2"){strkey = QString("{");}
         else if(split1== "3"){strkey = QString("}");}
         else if(split1== "4"){strkey = QString("\\");}
         else if(split1== "5"){strkey = QString(">");}
         else if(split1== "6"){strkey = QString("<");}
         else if(split1== "7"){strkey = QString("@");}
         else if(split1== "8"){strkey = QString("!");}
         else if(split1== "9"){strkey = QString(":");}
         else if(split1== "10"){strkey = QString("\"");}
         else if(split1== "11"){strkey = QString("\'");}
         else if(split1== "12"){strkey = QString("~");}
         else if(split1== "13"){strkey = QString("#");}
         else if(split1== "14"){strkey = QString("$");}
         else if(split1== "15"){strkey = QString("%");}
         else if(split1== "16"){strkey = QString("^");}
         else if(split1== "17"){strkey = QString("&");}
         else if(split1== "18"){strkey = QString("*");}
         else if(split1== "19"){strkey = QString("(");}
         else if(split1== "20"){strkey = QString(")");}
         else if(split1== "21"){strkey = QString("-");}
         else if(split1== "22"){strkey = QString("+");}
         else if(split1== "23"){strkey = QString("=");}
         else if(split1== "24"){strkey = QString("?");}
         else if(split1== "25"){strkey = QString(".");}
         else {strkey = string2char(split1);}
         //else {strkey = strbase;}
         strshow = strkey + split2;
    }
    else
    {
        if(strsplit.size()<=2)
        {
            strkey = string2char(split0);
           // strkey = split0 ;
            strshow = strkey + split1;
        }
        else if(strsplit.size()==3)
        {
            strkey = string2char(split0);
            strkey2 = string2char(split1);
             strshow = strkey + strkey2 + split2;
        }
        else if(strsplit.size()==4)
        {
            strkey = string2char(split0);
            strkey2 = string2char(split1);
            strkey3 = string2char(split2);
             strshow = strkey + strkey2 + strkey3 + split3;
        }
        else if(strsplit.size()>=5)
        {
            strkey = string2char(split0);
            strkey2 = string2char(split1);
            strkey3 = string2char(split2);
            strkey4 = string2char(split3);
            strshow = strkey + strkey2 + strkey3 + strkey4 + split4;
        }
    }
    return strshow;
}
void QEasyORC::loadfontmodel()
{
    fastmatch::clearmodels_l12();
    fastmatch::clearmodels_l36();
    fastmatch::clearmodels_l72();

    fastmatch::imagemodesclear_l12();
    fastmatch::imagemodesclear_l36();
    fastmatch::imagemodesclear_l72();

    m_filenamelist_l12.clear();
    m_fontlist_l12.clear();

    m_filenamelist_l36.clear();
    m_fontlist_l36.clear();

    m_filenamelist_l72.clear();
    m_fontlist_l72.clear();

    //
    QString filenameto=QString("*.imp");

    QString path12 = QString("./model/12x12/");
    QStringList files12 = QDir(path12).entryList(QStringList(filenameto),QDir::Files | QDir::NoSymLinks);

    QString path36 = QString("./model/36x36/");
    QStringList files36 = QDir(path36).entryList(QStringList(filenameto),QDir::Files | QDir::NoSymLinks);

    QString path72 = QString("./model/72x72/");
    QStringList files72 = QDir(path72).entryList(QStringList(filenameto),QDir::Files | QDir::NoSymLinks);

    for(int i=0;i<files12.size();++i)
    {
        QString filename = files12[i];
        QFileInfo afile(filename);
        //QString getfilename_pat = afile.completeBaseName() + QString(".pat");
        QString getfilename_imp = afile.completeBaseName() + QString(".imp");
        //QString strfilepat =  QString("./model/")+getfilename_pat;
        QString strfileimp = path12 +getfilename_imp;
        QString strbase = afile.completeBaseName();

        //fastmatch::addmodels(strfilepat.toStdString().c_str());
        fastmatch::addimagemodels_l12(strfileimp.toStdString().c_str());

        m_filenamelist_l12.append(strbase);
        QString strshow = filenametoORCstring(strbase);
        m_fontlist_l12.append(strshow);
    }

    for(int i=0;i<files36.size();++i)
    {
        QString filename = files36[i];
        QFileInfo afile(filename);
        //QString getfilename_pat = afile.completeBaseName() + QString(".pat");
        QString getfilename_imp = afile.completeBaseName() + QString(".imp");
        //QString strfilepat =  QString("./model/")+getfilename_pat;
        QString strfileimp = path36 +getfilename_imp;
        QString strbase = afile.completeBaseName();

        //fastmatch::addmodels(strfilepat.toStdString().c_str());
        fastmatch::addimagemodels_l36(strfileimp.toStdString().c_str());

        m_filenamelist_l36.append(strbase);
        QString strshow = filenametoORCstring(strbase);
        m_fontlist_l36.append(strshow);
    }

    for(int i=0;i<files72.size();++i)
    {
        QString filename = files72[i];
        QFileInfo afile(filename);
        //QString getfilename_pat = afile.completeBaseName() + QString(".pat");
        QString getfilename_imp = afile.completeBaseName() + QString(".imp");
        //QString strfilepat =  QString("./model/")+getfilename_pat;
        QString strfileimp = path72 +getfilename_imp;
        QString strbase = afile.completeBaseName();

        //fastmatch::addmodels(strfilepat.toStdString().c_str());
        fastmatch::addimagemodels_l72(strfileimp.toStdString().c_str());

        m_filenamelist_l72.append(strbase);
        QString strshow = filenametoORCstring(strbase);
        m_fontlist_l72.append(strshow);
    }

}
void QEasyORC::setimagetype(int itype)
{
    m_imagetype = itype;
}
void QEasyORC::setshow(int ishow)
{
   fastmatch::setshow(ishow);
}
void QEasyORC::setshowpos(int ix,int iy)
{
    m_idraw_x = ix;
    m_idraw_y = iy;
}
void QEasyORC::drawshape(QPainter &painter,QPalette &pal)
{
    if(m_idebugfontnum!=-1
            &&m_idebugfontnum<getmodels_l12().size())
    {
        getmodels_l12()[m_idebugfontnum].drawshape(painter);
    }
    //
    fastmatch::drawshape(painter,pal);
    if(1==m_idraw_map
       ||-1==m_idraw_map)
    {
        for(int i=0;i<m_pgrids_l3.size();i++)
        {
            m_pgrids_l3[i]->drawshape(painter);
        }

        for(int i=0;i<m_pgrids_l6.size();i++)
        {
            m_pgrids_l6[i]->drawshape(painter);
        }

        for(int i=0;i<m_pgrids_l12.size();i++)
        {
            m_pgrids_l12[i]->drawshape(painter);
        }

        for(int i=0;i<m_pgrids_l36.size();i++)
        {
            m_pgrids_l36[i]->drawshape(painter);
        }

        for(int i=0;i<m_pgrids_l72.size();i++)
        {
            m_pgrids_l72[i]->drawshape(painter);
        }
    }
    else if(2==m_idraw_map
            ||-1==m_idraw_map
            ||-2==m_idraw_map)
    {
        if(0==m_ilevle)
        {
            int isize = getlevel3_6map().size();
            int jsize = getlevel6_12map().size();
            int ksize = getlevel12_36map().size();
            int lsize = getlevel36_72map().size();
            if(m_idebugfontnum<isize&&m_idebugfontnum>=0)
            {
                  m_pgrids_l3[m_idebugfontnum]->drawshape(painter);
                  for(int i=0;i<isize;i++)
                  {
                      if(getlevel3_6map()[i]==m_idebugfontnum)
                      {
                          m_pgrids_l6[i]->drawshape(painter);
                          for(int j=0;j<jsize;j++)
                          {
                              if(getlevel6_12map()[j]==i)
                              {
                                  m_pgrids_l12[j]->drawshape(painter);
                                  for(int k=0;k<ksize;k++)
                                  {
                                      if(getlevel12_36map()[k]==j)
                                      {
                                          m_pgrids_l36[k]->drawshape(painter);
                                          for(int l=0;l<lsize;l++)
                                              if(getlevel36_72map()[l]==k)
                                                  m_pgrids_l72[l]->drawshape(painter);

                                      }
                                  }
                              }
                          }
                      }
                  }
            }
        }
        else if(1==m_ilevle)
        {
            int jsize = getlevel6_12map().size();
            int ksize = getlevel12_36map().size();
            int lsize = getlevel36_72map().size();

            int idebugnum1 = getlevel3_6map()[m_idebugfontnum];
            m_pgrids_l3[idebugnum1]->drawshape(painter);

            if(m_idebugfontnum<jsize&&m_idebugfontnum>=0)
            {
                m_pgrids_l6[m_idebugfontnum]->drawshape(painter);
                for(int j=0;j<jsize;j++)
                {
                    if(getlevel6_12map()[j]==m_idebugfontnum)
                    {
                        m_pgrids_l12[j]->drawshape(painter);
                        for(int k=0;k<ksize;k++)
                        {
                            if(getlevel12_36map()[k]==j)
                            {
                                m_pgrids_l36[k]->drawshape(painter);
                                for(int l=0;l<lsize;l++)
                                {
                                    if(getlevel36_72map()[l]==k)
                                        m_pgrids_l72[l]->drawshape(painter);

                                }
                            }

                        }
                    }
                }
            }
        }
        else if(2==m_ilevle)
        {
            int jsize = getlevel6_12map().size();
            int ksize = getlevel12_36map().size();
            int lsize = getlevel36_72map().size();



            int idebugnum2 = getlevel6_12map()[m_idebugfontnum];
            m_pgrids_l6[idebugnum2]->drawshape(painter);
            int idebugnum1 = getlevel3_6map()[idebugnum2];
            m_pgrids_l3[idebugnum1]->drawshape(painter);

             if(m_idebugfontnum<jsize&&m_idebugfontnum>=0)
            {
                int ishownum = getlevel6_12map()[m_idebugfontnum];
                for(int j=0;j<jsize;j++)
                {
                    if(getlevel6_12map()[j]==ishownum)
                    {
                        m_pgrids_l12[j]->drawshape(painter);
                        for(int k=0;k<ksize;k++)
                        {
                            if(getlevel12_36map()[k]==j)
                            {
                                m_pgrids_l36[k]->drawshape(painter);
                                for(int l=0;l<lsize;l++)
                                    if(getlevel36_72map()[l]==k)
                                        m_pgrids_l72[l]->drawshape(painter);

                            }
                        }
                    }
                }

            }
        }
        else if(3==m_ilevle)
        {
            int ksize = getlevel12_36map().size();
            int lsize = getlevel36_72map().size();

            int idebugnum3 = getlevel12_36map()[m_idebugfontnum];
            m_pgrids_l12[idebugnum3]->drawshape(painter);
            int idebugnum2 = getlevel6_12map()[idebugnum3];
            m_pgrids_l6[idebugnum2]->drawshape(painter);
            int idebugnum1 = getlevel3_6map()[idebugnum2];
            m_pgrids_l3[idebugnum1]->drawshape(painter);



            if(m_idebugfontnum<ksize&&m_idebugfontnum>=0)
            {
                int ishownum = getlevel12_36map()[m_idebugfontnum];
                for(int k=0;k<ksize;k++)
                {
                    if(getlevel6_12map()[k]==ishownum)
                    {
                        m_pgrids_l36[k]->drawshape(painter);
                        for(int l=0;l<lsize;l++)
                            if(getlevel36_72map()[l]==k)
                                m_pgrids_l72[l]->drawshape(painter);

                    }
                }

            }
        }

    }
    else if(3==m_idraw_map
            ||-2==m_idraw_map)
    {
        m_pimagegrid->drawshape(painter);
    }
    else if(4==m_idraw_map
            &&m_idraw_x>=0
            &&m_idraw_y>=0)
    {
        int imodelnum = 0;
         for (int y = 0; y < 800; y += 20)
        {
           for (int x = 0; x < 600; x += 40)
            {
                painter.save();
                painter.translate(x+m_idraw_x, y+m_idraw_y);
                QRect arect(0,0,40,40);
                if(imodelnum<m_fontlist_l12.size())
                painter.drawText(arect,
                                 Qt::AlignCenter,
                                 m_fontlist_l12[imodelnum]);
                imodelnum++;
                painter.restore();
            }
        }
    }
    else if(103==m_idraw_map
            ||-100==m_idraw_map)
    {
        int iarea = m_reslutnodeslistgrid3x3.size();

        for(int ia=0;ia<iarea;ia++)
            {
                 if(-1==m_idebugrectsnum
                         ||ia==m_idebugrectsnum)
                 {
                     int inodesize = m_reslutnodeslistgrid3x3[ia].getnodes().size();

                     for(int in=0;in<inodesize;in++)
                      {
                             int icurnum = m_reslutnodeslistgrid3x3[ia].getnodes()[in].s_inode;
                             m_pgrids_l3[icurnum]->drawshape(painter);
                      }

                 }
            }


    }
    else if(106==m_idraw_map
            ||-100==m_idraw_map)
    {
        int iarea = m_reslutnodeslistgrid6x6.size();

        for(int ia=0;ia<iarea;ia++)
            {
                if(-1==m_idebugrectsnum
                        ||ia==m_idebugrectsnum)
                {
                    int inodesize = m_reslutnodeslistgrid6x6[ia].getnodes().size();
                    for(int in=0;in<inodesize;in++)
                    {
                        int icurnum = m_reslutnodeslistgrid6x6[ia].getnodes()[in].s_inode;
                        m_pgrids_l6[icurnum]->drawshape(painter);
                    }
                }
            }

    }
    else if(112==m_idraw_map
            ||-100==m_idraw_map)
    {
        int iarea = m_reslutnodeslistgrid12x12.size();

        for(int ia=0;ia<iarea;ia++)
            {
                if(-1==m_idebugrectsnum
                ||ia==m_idebugrectsnum)
                {
                    int inodesize = m_reslutnodeslistgrid12x12[ia].getnodes().size();
                    for(int in=0;in<inodesize;in++)
                    {
                        int icurnum = m_reslutnodeslistgrid12x12[ia].getnodes()[in].s_inode;
                        m_pgrids_l12[icurnum]->drawshape(painter);
                    }
                }
            }

    }
    else if(-1000==m_idraw_map)
    {
        if(m_idebugrectsnum!=-1)
        {
            int icurnum = m_reslutnodeslistgrid3x3[m_idebugrectsnum].getnodes()[0].s_inode;
            m_pgrids_l3[icurnum]->drawshape(painter);
             icurnum = m_reslutnodeslistgrid6x6[m_idebugrectsnum].getnodes()[0].s_inode;
            m_pgrids_l6[icurnum]->drawshape(painter);
             icurnum = m_reslutnodeslistgrid12x12[m_idebugrectsnum].getnodes()[0].s_inode;
            m_pgrids_l12[icurnum]->drawshape(painter);

        }
    }
   // painter.translate(0, 0);

}
void QEasyORC::setshowmap(int ishow,int ilevel,int idebugfont)
{
    m_ilevle = ilevel;
    m_idraw_map = ishow;
    m_idebugfontnum = idebugfont;
}
void QEasyORC::setorcareasnum(int inum)
{
    fastmatch::setmatchrectnum(inum);
}
void QEasyORC::setorcareas(int inum, int ix, int iy, int iw, int ih)
{
    fastmatch::setmultimatchrect(inum,ix,iy,iw,ih);
}
void QEasyORC::setorcthre(int ithre)
{
    fastmatch::setmatchthre(ithre);
}
void QEasyORC::setb2w(int ib2w)
{
    fastmatch::setb2w(ib2w);
}

void QEasyORC::setspecshow(int ishow)
{
    fastmatch::setspecshow(ishow);
}
void QEasyORC::stringsplit(void *pimage)
{
    ImageBase *pgetimage = (ImageBase*)pimage;
    StringSplit(*pgetimage);
}
void QEasyORC::fontsplit(void *pimage)
{
    ImageBase *pgetimage = (ImageBase*)pimage;
    FontSplit(*pgetimage);
}
void QEasyORC::exfontsplit(void *pimage)
{
    ImageBase *pgetimage = (ImageBase*)pimage;
    ExFontSplit(*pgetimage);
}

void QEasyORC::areasorc(void *pimage)
{
    ImageBase *pgetimage = (ImageBase*)pimage;
    AreasORC(*pgetimage);
}
void QEasyORC::setdebug(int idebugrect,int idebugfont)
{
    m_idebugrectsnum = idebugrect;
    m_idebugfontnum = idebugfont;
}
void QEasyORC::setsplitimage(int ithre,int ixor,int iyor,int ixand,int iyand)
{
    m_image_thre = ithre;
    m_ix_or = ixor;
    m_iy_or = iyor;
    m_ix_and = ixand;
    m_iy_and = iyand;

}
void QEasyORC::setsplitobjectbg(int ibgedge,int ibgmethod)
{
    m_findobj_bgedge = ibgedge;
    m_findobj_bgmethod = ibgmethod;
}
void QEasyORC::setsplitobject(int idistance,int isearchtype,int ibrow,int iminarea,int ibgedge)
{
        m_findobj_distance = idistance;
        m_findobj_searchtype = isearchtype;
        m_findobj_brow = ibrow;
        m_findobj_minarea = iminarea;
        m_findobj_bgedge = ibgedge;
}
void QEasyORC::setsplitobjectoffset(int ix0,int ix1,int iy0,int iy1)
{
    m_findobj_ioffsetx0 = ix0;
    m_findobj_ioffsetx1 = ix1;
    m_findobj_ioffsety0 = iy0;
    m_findobj_ioffsety1 = iy1;
}

void QEasyORC::setsplitgrid(int iw,int ih,int igridnum)
{
    g_pbackfindobject->setobjectgrid(iw,ih,igridnum);
}
void QEasyORC::StringSplit(ImageBase &image)
{
    QRect arect = QShape::rect();

    g_pbackimage->SetROI(arect.x(),arect.y(),arect.width(),arect.height());
    image.SetROI(arect.x(),arect.y(),arect.width(),arect.height());
    image.SetMode(3);
    image.ROItoROI(g_pbackimage);

    g_pbackimage->ROIImageMoveOrX(m_ix_or);
    g_pbackimage->ROIImageMoveOrY(m_iy_or);
    g_pbackimage->ROIImageMoveAndX(m_ix_and);
    g_pbackimage->ROIImageMoveAndY(m_iy_and);
    g_pbackimage->ROIImageThre(m_image_thre);

    g_pbackfindobject->setrect(arect.x(),arect.y(),arect.width(),arect.height());

    g_pbackfindobject->setdistance(m_findobj_distance);
    g_pbackfindobject->setsearchtype(m_findobj_searchtype);
    g_pbackfindobject->setbrow(m_findobj_brow);

    g_pbackfindobject->setminmaxarea(m_findobj_minarea,m_findobj_maxarea);
    g_pbackfindobject->setminmaxwh(m_findobj_minw,m_findobj_maxw,m_findobj_minh,m_findobj_maxh);

    g_pbackfindobject->measure(g_pbackimage);

    int ix0 = g_pbackfindobject->getresultx(0);
    int iy0 = g_pbackfindobject->getresulty(0);

    int ih0 = g_pbackfindobject->getresulth(0);
    int iw0 = g_pbackfindobject->getresultw(0);

    ix0 = ix0 - 2 >=0?ix0-2:0;
    iy0 = iy0 - 2 >=0?iy0-2:0;

    setrect(ix0,iy0,iw0+4,ih0+4);

}

void QEasyORC::FontSplit(ImageBase &image)
{
    QRect arect = QShape::rect();

    g_pbackimage->SetROI(arect.x(),arect.y(),arect.width(),arect.height());
    image.SetROI(arect.x(),arect.y(),arect.width(),arect.height());
    image.SetMode(3);
    image.ROItoROI(g_pbackimage);


    g_pbackimage->ROIImageMoveOrX(m_ix_or);
    g_pbackimage->ROIImageMoveOrY(m_iy_or);
    g_pbackimage->ROIImageMoveAndX(m_ix_and);
    g_pbackimage->ROIImageMoveAndY(m_iy_and);
    if(0!=m_image_thre)
        g_pbackimage->ROIImageThre(m_image_thre);

    g_pbackfindobject->setrect(arect.x(),arect.y(),arect.width(),arect.height());

    g_pbackfindobject->setdistance(m_findobj_distance);
    g_pbackfindobject->setsearchtype(m_findobj_searchtype);
    g_pbackfindobject->setbrow(m_findobj_brow);

    g_pbackfindobject->setminmaxarea(m_findobj_minarea,m_findobj_maxarea);
    g_pbackfindobject->setminmaxwh(m_findobj_minw,m_findobj_maxw,m_findobj_minh,m_findobj_maxh);

    g_pbackfindobject->setoffset(m_findobj_ioffsetx0,m_findobj_ioffsetx1,m_findobj_ioffsety0,m_findobj_ioffsety1);

    g_pbackfindobject->measure(g_pbackimage);
    g_pbackfindobject->setbackground(m_findobj_bgedge,m_findobj_bgmethod);

    g_pbackfindobject->resultsrectfilter();
    g_pbackfindobject->objectgrid(&image);
}
void QEasyORC::ExFontSplit(ImageBase &image)
{
    QRect arect = QShape::rect();

    g_pbackimage->SetROI(arect.x(),arect.y(),arect.width(),arect.height());
    image.SetROI(arect.x(),arect.y(),arect.width(),arect.height());
    image.SetMode(3);
    image.ROItoROI(g_pbackimage);


    g_pbackimage->ROIImageMoveOrX(m_ix_or);
    g_pbackimage->ROIImageMoveOrY(m_iy_or);
    g_pbackimage->ROIImageMoveAndX(m_ix_and);
    g_pbackimage->ROIImageMoveAndY(m_iy_and);
    if(0!=m_image_thre)
        g_pbackimage->ROIImageThre(m_image_thre);
    g_pbackfindobject->setrect(arect.x(),arect.y(),arect.width(),arect.height());

    g_pbackfindobject->setdistance(m_findobj_distance);
    g_pbackfindobject->setsearchtype(m_findobj_searchtype);
    g_pbackfindobject->setbrow(m_findobj_brow);

    g_pbackfindobject->setminmaxarea(m_findobj_minarea,m_findobj_maxarea);
    g_pbackfindobject->setminmaxwh(m_findobj_minw,m_findobj_maxw,m_findobj_minh,m_findobj_maxh);

    g_pbackfindobject->setoffset(m_findobj_ioffsetx0,m_findobj_ioffsetx1,m_findobj_ioffsety0,m_findobj_ioffsety1);

    g_pbackfindobject->measure(g_pbackimage);
    g_pbackfindobject->setbackground(m_findobj_bgedge,m_findobj_bgmethod);

    //g_pbackfindobject->resultsrectfilter();
    g_pbackfindobject->objectgrid(&image);
}


void QEasyORC::AreasORC(ImageBase &image)
{
    RectsShape arects = fastmatch::getmatchrects();

   m_resultstrlist.clear();
   m_resultstring.clear();

   int iareasnum =  arects.size();
   if(iareasnum<=0)
       return;

   for(int ia=0;ia<iareasnum;ia++)
   {
       if(-1==m_idebugrectsnum
         ||ia==m_idebugrectsnum)
       {
           int isize = m_fontlist_l12.size();
           int ix = arects.getrect(ia).x();
           int iy = arects.getrect(ia).y();
           int iw = arects.getrect(ia).width();
           int ih = arects.getrect(ia).height();
           fastmatch::setmatchrect(ix,iy,iw,ih);
           double dmaxvalue = 0;
           int iresultfont = 0;
           for(int i=0;i<isize;i++)
           {
               if(-1==m_idebugfontnum
                 ||i==m_idebugfontnum)
               {
                   fastmatch::modelstocurrent_l12(i);
                   fastmatch::imagemodelstocurrent_l12(i);
                   fastmatch::Match(image);
                   double dvalue = fastmatch::getmaxresult();
                   double dimagevalue = 0;
                   if(dvalue>0.5)
                   {
                       fastmatch::imagematch(-1,1);
                       dimagevalue = fastmatch::getimagemodelreslut();
                       if(dimagevalue>dmaxvalue)
                       {
                           dmaxvalue = dimagevalue;
                           iresultfont = i;
                       }
                       if(-1==m_idebugfontnum
                          &&100==dimagevalue)
                           break;
                   }
               }
           }
           m_resultstrlist.push_back(m_fontlist_l12[iresultfont] );
           m_resultstring.append(m_fontlist_l12[iresultfont] );


       }
   }
//   if(-1!=m_idebugrectsnum&&-1!=m_idebugfontnum)
//        fastmatch::imagemodelcompareshow();
    QShape::setname(m_resultstring.toStdString().c_str());
}
void QEasyORC::imagemodelshow()
{
    fastmatch::imagemodelshow();
}

void QEasyORC::imagematchshow()
{
    fastmatch::imagematchshow();
}
void QEasyORC::imagecompareshow(int itype)
{
    fastmatch::imagemodelcompareshow(itype);
}
void QEasyORC::autolearn(const char *pfilename )
{
        QRect arect = g_pbackfindobject->getgrid(m_idebugrectsnum);
        int ix = arect.x();
        int iy = arect.y();
        int iw = arect.width();
        int ih = arect.height();

        QRect arectrecover = QShape::rect();

        fastmatch::setmatchrect(ix,iy,iw,ih);
        findline::setrect(ix,iy,iw,ih);
        fastmatch::setthre(8);
        fastmatch::setcomparegap(2);
        fastmatch::setmethod(0);
        fastmatch::setlinegap(1);
        fastmatch::SetWHgap(1,1);
        fastmatch::setlinesamplerate(0.002);
        fastmatch::setfilter(0,0,10000);
        fastmatch::learn(g_pbackobjectimage);
       fastmatch::setmatchrect(ix,iy,iw,ih);
    fastmatch::setfindnum(1);
    fastmatch::setmatchthre(3);
    fastmatch::setfindnum(1);
    fastmatch::setmatchthre(3);
    fastmatch::match(g_pbackobjectimage);
    fastmatch::imagelearn(-1,1);
    QString strfilename = pfilename;

     QRegExp rxnum("(\\d+)");
     QStringList listother = strfilename.split(rxnum);

     QRegExp rxother("(\\D+)");
     QStringList listnum = strfilename.split(rxother);

     QString strkey = strfilename ;
     QString strkey2;
     QString strkey3;
     QString strlast  ;
    if(listother.size()>1)
    {
        QString qstr = listother[0];
        if(qstr!=""&&qstr.size()==1)
            strkey = qstr;//strfilename.mid(0,1) ;
        else if(qstr.size()==2)
        {
            strkey = qstr.mid(0,1);//strfilename.mid(0,1) ;
            strkey2 = qstr.mid(1,1);
        }
        else if(qstr.size()==3)
        {
            strkey = qstr.mid(0,1);//strfilename.mid(0,1) ;
            strkey2 = qstr.mid(1,1);
            strkey3 = qstr.mid(2,1);
        }

        if(listnum.size()<2)
        {
            strkey = listnum[0].mid(0,1);
            strlast = listnum[0].mid(1);
        }
        else
        if(listnum[1]!="")
            strlast = listnum[1];//strfilename.mid(1) ;
    }
    QString strbase;
    QString charstring = char2string(strkey);
    if(!strkey2.isEmpty())
    {
        charstring = charstring + QString("_") + char2string(strkey2);
    }
    if(!strkey3.isEmpty())
    {
        charstring = charstring + QString("_") + char2string(strkey3);
    }
    //strbase = QString("_") + charstring;
    strbase = charstring;
if(0)
{
    if(strkey == QString("\/")){strbase=QString( "_1");}
    else if(strkey == QString("{")){strbase=QString( "_2");}
    else if(strkey == QString("}")){strbase=QString( "_3");}
    else if(strkey == QString("\\")){strbase=QString( "_4");}
    else if(strkey == QString(">")){strbase=QString( "_5");}
    else if(strkey == QString("<")){strbase=QString( "_6");}
    else if(strkey == QString("@")){strbase=QString( "_7");}
    else if(strkey == QString("!")){strbase=QString( "_8");}
    else if(strkey == QString(":")){strbase=QString( "_9");}
    else if(strkey == QString("\"")){strbase=QString( "_10");}
    else if(strkey == QString("\'")){strbase=QString( "_11");}
    else if(strkey == QString("~")){strbase=QString( "_12");}
    else if(strkey == QString("#")){strbase=QString( "_13");}
    else if(strkey == QString("$")){strbase=QString( "_14");}
    else if(strkey == QString("%")){strbase=QString( "_15");}
    else if(strkey == QString("^")){strbase=QString( "_16");}
    else if(strkey == QString("&")){strbase=QString( "_17");}
    else if(strkey == QString("*")){strbase=QString( "_18");}
    else if(strkey == QString("(")){strbase=QString( "_19");}
    else if(strkey == QString(")")){strbase=QString( "_20");}
    else if(strkey == QString("-")){strbase=QString( "_21");}
    else if(strkey == QString("+")){strbase=QString( "_22");}
    else if(strkey == QString("=")){strbase=QString( "_23");}
    else if(strkey == QString("?")){strbase=QString( "_24");}
    else if(strkey == QString(".")){strbase=QString( "_25");}
    else {strbase = strkey ;}
}
     QString qstrsavepat = QString("./model/12x12/")+strbase +QString("_")+ strlast + QString(".pat");
     QString qstrsaveimp = QString("./model/12x12/")+strbase +QString("_")+ strlast + QString(".imp");
    QString qstrbase = strbase +QString("_")+ strlast ;


    //fastmatch::savemodelfile(qstrsavepat.toStdString().c_str());
    fastmatch::savefastimagemodel(qstrsaveimp.toStdString().c_str());
    fastmatch::savefastimagepatmodel(qstrsavepat.toStdString().c_str());

    int ifontsize = m_fontlist_l12.size();
    for(int in = 0;in<ifontsize;in++)
    {

       QString qstr = m_fontlist_l12[in];
       if(strfilename==qstr)
       {
           fastmatch::setcurmodels(in);
           fastmatch::setcurimagemodels(in);
           break;
       }
       if(in==ifontsize-1)
       {
           //fastmatch::addmodels(qstrsavepat.toStdString().c_str());
           fastmatch::addimagemodels_l12(qstrsaveimp.toStdString().c_str());


            m_filenamelist_l12.append(qstrbase);
            m_fontlist_l12.append(strfilename);

       }
    }

    ix = arectrecover.x();
    iy = arectrecover.y();
    iw = arectrecover.width();
    ih = arectrecover.height();

    QShape::setrect(ix,iy,iw,ih);

    levelmodel();

}
void QEasyORC::autolearnex(const char *pfilename )
{
        QRect arect = g_pbackfindobject->getgridex(m_idebugrectsnum);

        int iobjw = g_pbackfindobject->getresultw(m_idebugrectsnum);
        int iobjh = g_pbackfindobject->getresulth(m_idebugrectsnum);
        int imaxlen = iobjw>iobjh?iobjw:iobjh;
        int igrid = fastmatch::GetRectGridLevel(imaxlen);

        int ix = arect.x();
        int iy = arect.y();
        int iw = arect.width();
        int ih = arect.height();

        QRect arectrecover = QShape::rect();

        fastmatch::setmatchrect(ix,iy,iw,ih);
        findline::setrect(ix,iy,iw,ih);
        fastmatch::setthre(8);
        fastmatch::setcomparegap(2);
        fastmatch::setmethod(0);
        fastmatch::setlinegap(1);
        fastmatch::SetWHgap(1,1);
        fastmatch::setlinesamplerate(0.002);
        fastmatch::setfilter(0,0,10000);
        fastmatch::learn(g_pbackobjectimage);
       fastmatch::setmatchrect(ix,iy,iw,ih);
    fastmatch::setfindnum(1);
    fastmatch::setmatchthre(3);
    fastmatch::setfindnum(1);
    fastmatch::setmatchthre(3);
    fastmatch::match(g_pbackobjectimage);

    fastmatch::imagelearnex(-1,1,igrid);

    //QRect arectx =  fastmatch::getresultrect(0);
    //int iwx = arectx.width();
    //int ihx = arectx.height();
    //int imaxlenx = iwx>ihx?iwx:ihx;
    //m_pimagegrid->SetModelWH(imaxlenx,imaxlenx);
    //m_pimagegrid->GridFastModel(fastmatch::getgrid());//test
    //return;
    //

    QGrid *pgrid= fastmatch::getgrid();
    QString strgrid = pgrid->GetGridString();

    QString strfilename = pfilename;

     QRegExp rxnum("(\\d+)");
     QStringList listother = strfilename.split(rxnum);

     QRegExp rxother("(\\D+)");
     QStringList listnum = strfilename.split(rxother);

     QString strkey = strfilename ;
     QString strkey2;
     QString strkey3;
     QString strkey4;
     QString strkey5;
     QString strlast  ;
    if(listother.size()>1)
    {
        QString qstr = listother[0];
        if(qstr!=""&&qstr.size()==1)
            strkey = qstr;//strfilename.mid(0,1) ;
        else if(qstr.size()==2)
        {
            strkey = qstr.mid(0,1);//strfilename.mid(0,1) ;
            strkey2 = qstr.mid(1,1);
        }
        else if(qstr.size()==3)
        {
            strkey = qstr.mid(0,1);//strfilename.mid(0,1) ;
            strkey2 = qstr.mid(1,1);
            strkey3 = qstr.mid(2,1);
        }
        else if(qstr.size()==4)
        {
            strkey = qstr.mid(0,1);//strfilename.mid(0,1) ;
            strkey2 = qstr.mid(1,1);
            strkey3 = qstr.mid(2,1);
            strkey4 = qstr.mid(3,1);
        }
        else if(qstr.size()==5)
        {
            strkey = qstr.mid(0,1);//strfilename.mid(0,1) ;
            strkey2 = qstr.mid(1,1);
            strkey3 = qstr.mid(2,1);
            strkey4 = qstr.mid(3,1);
            strkey5 = qstr.mid(4,1);

        }

        if(listnum.size()<2)
        {
            strkey = listnum[0].mid(0,1);
            strlast = listnum[0].mid(1);
        }
        else
        if(listnum[1]!="")
            strlast = listnum[1];//strfilename.mid(1) ;
    }
    QString strbase;
    QString charstring = char2string(strkey);
    if(!strkey2.isEmpty())
    {
        charstring = charstring + QString("_") + char2string(strkey2);
    }
    if(!strkey3.isEmpty())
    {
        charstring = charstring + QString("_") + char2string(strkey3);
    }
    if(!strkey4.isEmpty())
    {
        charstring = charstring + QString("_") + char2string(strkey4);
    }
    if(!strkey5.isEmpty())
    {
        charstring = charstring + QString("_") + char2string(strkey5);
    }

    //strbase = QString("_") + charstring;
    strbase = charstring;
    QString qstrsavepat = QString("./model/")+strgrid+QString("/")+strbase +QString("_")+ strlast + QString(".pat");
    QString qstrsaveimp = QString("./model/")+strgrid+QString("/")+strbase +QString("_")+ strlast + QString(".imp");
    QString qstrbase = strbase +QString("_")+ strlast ;
   //fastmatch::savemodelfile(qstrsavepat.toStdString().c_str());
    fastmatch::savefastimagemodel(qstrsaveimp.toStdString().c_str());
    fastmatch::savefastimagepatmodel(qstrsavepat.toStdString().c_str());

    int ifontsize = m_fontlist_l12.size();
    for(int in = 0;in<ifontsize;in++)
    {
       QString qstr = m_fontlist_l12[in];
       if(strfilename==qstr)
       {

           fastmatch::setcurmodels(in);
           fastmatch::setcurimagemodels(in);
           break;
       }
       if(in==ifontsize-1)
       {
           //fastmatch::addmodels(qstrsavepat.toStdString().c_str());
           fastmatch::addimagemodels_l12(qstrsaveimp.toStdString().c_str());

            m_filenamelist_l12.append(qstrbase);
            m_fontlist_l12.append(strfilename);
       }
    }

    ix = arectrecover.x();
    iy = arectrecover.y();
    iw = arectrecover.width();
    ih = arectrecover.height();

    QShape::setrect(ix,iy,iw,ih);

    levelmodel();

}
void QEasyORC::autolearnobj(const char *pfilename )
{
    QRect arect = g_pbackfindobject->getgridex(m_idebugrectsnum);//getgrid(ia);

    int iobjw = g_pbackfindobject->getresultw(m_idebugrectsnum);
    int iobjh = g_pbackfindobject->getresulth(m_idebugrectsnum);
    int imaxlen = iobjw>iobjh?iobjw:iobjh;
    int igrid_org = fastmatch::GetRectGridLevel(imaxlen);

    int ix = arect.x();
    int iy = arect.y();
    int iw = arect.width();
    int ih = arect.height();
    //fastmatch::setmatchrect(ix,iy,iw,ih);
   // g_pbackobjectimage->SetROI(ix,iy,iw,ih);

  //  g_pbackimage->SetROI(ix,iy,iw,ih);
  //  g_pbackobjectimage->SetMode(3);
  //  g_pbackobjectimage->ROItoROI(g_pbackimage);

  //  g_pbackimage->ROIColorTable();
  //  g_pbackimage->ROIColorTableBlur(0,-1);
  //  g_pbackimage->ROIColorTableEasyThre(1);
    g_pbackimage->SetROI(ix,iy,iw,ih);
    m_pimagegrid->ROIImagetoModel(*g_pbackimage);
    m_pimagegrid->ZeroModel();

    m_pimagegrid->ReGrid(igrid_org,igrid_org);
    if(0)//test
    {
        m_pimagegrid->SetUnit(igrid_org,igrid_org);
        m_pimagegrid->UnitGrid();
    }

    QGrid *pgrid= m_pimagegrid;
    QString strgrid = pgrid->GetGridString();

    QString strfilename = pfilename;

     QRegExp rxnum("(\\d+)");
     QStringList listother = strfilename.split(rxnum);

     QRegExp rxother("(\\D+)");
     QStringList listnum = strfilename.split(rxother);

     QString strkey = strfilename ;
     QString strkey2;
     QString strkey3;
     QString strkey4;
     QString strkey5;
     QString strlast  ;
    if(listother.size()>1)
    {
        QString qstr = listother[0];
        if(qstr!=""&&qstr.size()==1)
            strkey = qstr;//strfilename.mid(0,1) ;
        else if(qstr.size()==2)
        {
            strkey = qstr.mid(0,1);//strfilename.mid(0,1) ;
            strkey2 = qstr.mid(1,1);
        }
        else if(qstr.size()==3)
        {
            strkey = qstr.mid(0,1);//strfilename.mid(0,1) ;
            strkey2 = qstr.mid(1,1);
            strkey3 = qstr.mid(2,1);
        }
        else if(qstr.size()==4)
        {
            strkey = qstr.mid(0,1);//strfilename.mid(0,1) ;
            strkey2 = qstr.mid(1,1);
            strkey3 = qstr.mid(2,1);
            strkey4 = qstr.mid(3,1);
        }
        else if(qstr.size()==5)
        {
            strkey = qstr.mid(0,1);//strfilename.mid(0,1) ;
            strkey2 = qstr.mid(1,1);
            strkey3 = qstr.mid(2,1);
            strkey4 = qstr.mid(3,1);
            strkey5 = qstr.mid(4,1);

        }

        if(listnum.size()<2)
        {
            strkey = listnum[0].mid(0,1);
            strlast = listnum[0].mid(1);
        }
        else
        if(listnum[1]!="")
            strlast = listnum[1];//strfilename.mid(1) ;
    }
    QString strbase;
    QString charstring = char2string(strkey);
    if(!strkey2.isEmpty())
    {
        charstring = charstring + QString("_") + char2string(strkey2);
    }
    if(!strkey3.isEmpty())
    {
        charstring = charstring + QString("_") + char2string(strkey3);
    }
    if(!strkey4.isEmpty())
    {
        charstring = charstring + QString("_") + char2string(strkey4);
    }
    if(!strkey5.isEmpty())
    {
        charstring = charstring + QString("_") + char2string(strkey5);
    }

    //strbase = QString("_") + charstring;
    strbase = charstring;
    QString qstrsavepat = QString("./model/")+strgrid+QString("/")+strbase +QString("_")+ strlast + QString(".pat");
    QString qstrsaveimp = QString("./model/")+strgrid+QString("/")+strbase +QString("_")+ strlast + QString(".imp");
    QString qstrbase = strbase +QString("_")+ strlast ;

    bool bsaveok = false ;
    int isavenum = 0;
    while(!bsaveok)
    {
        QString strlast1 = QString("%1").arg(isavenum);
        qstrsaveimp = QString("./model/")+strgrid+QString("/")+strbase +QString("_")+ strlast1 + QString(".imp");
        isavenum = isavenum + 1;
        QFileInfo afileinf(qstrsaveimp);
        if(!afileinf.exists())
        {
            m_pimagegrid->savemapmodel(qstrsaveimp.toStdString().c_str());
            bsaveok = true;
        }
    }
   //fastmatch::savemodelfile(qstrsavepat.toStdString().c_str());
    //fastmatch::savefastimagemodel(qstrsaveimp.toStdString().c_str());
    //fastmatch::savefastimagepatmodel(qstrsavepat.toStdString().c_str());

    clearmodel();
    loadfontmodel();
    levelmodel();
    setlevelstring();

}

void QEasyORC::setlearngridwh(int igridwh)
{
    m_igridwh = igridwh;
}

void QEasyORC::string_exnum(int inum)
{
	m_exnum =inum;
}

void QEasyORC::string_autolearnmass(const char *pstring)
{
	QString qstr(pstring);
	int istrnum = qstr.size();
	int igetobj = g_pbackfindobject->getresultobjsnum();
	if(istrnum==igetobj)
	{
		for(int i=0;i<istrnum;i++)
		{ 
			 m_idebugrectsnum = i;
			 QString qchar(pstring[i]);
			 QString qname = qchar + QString("%1").arg(m_exnum);
			 learnmass_36(qname.toStdString().c_str());
			//artisticSleep(200);
		}	
	clearmodel();
	loadfontmodel();
	levelmodel();
	setlevelstring();
		
		
		 
		mapgrid();	
		setspecshow(-1);
        //setshowmap(-1);
		
	}
}
void QEasyORC::autolearnmass(const char *pfilename )
{
    QRect arect = g_pbackfindobject->getgridex(m_idebugrectsnum);

    int iobjw = g_pbackfindobject->getresultw(m_idebugrectsnum);
    int iobjh = g_pbackfindobject->getresulth(m_idebugrectsnum);
    int imaxlen = iobjw>iobjh?iobjw:iobjh;
    int igrid = fastmatch::GetRectGridLevel(imaxlen);

    int ix = arect.x();
    int iy = arect.y();
    int iw = arect.width();
    int ih = arect.height();
        QRect arectrecover = QShape::rect();

        fastmatch::setmatchrect(ix,iy,iw,ih);
        findline::setrect(ix,iy,iw,ih);
        fastmatch::setthre(8);
        fastmatch::setcomparegap(2);
        fastmatch::setmethod(0);
        fastmatch::setlinegap(1);
        fastmatch::SetWHgap(1,1);
        fastmatch::setlinesamplerate(0.002);
        fastmatch::setfilter(0,0,10000);
        fastmatch::learn(g_pbackobjectimage);
       fastmatch::setmatchrect(ix,iy,iw,ih);
    fastmatch::setfindnum(1);
    fastmatch::setmatchthre(3);
    fastmatch::setfindnum(1);
    fastmatch::setmatchthre(3);
    fastmatch::match(g_pbackobjectimage);

    fastmatch::imagelearnmass(-1,1,igrid);

    QString qsavedir;
    if(12==igrid)
        qsavedir = QString("./model/12x12/");
    else if(24==igrid)
        qsavedir = QString("./model/24x24/");
    else if(36==igrid)
        qsavedir = QString("./model/36x36/");
     else if(72==igrid)
        qsavedir = QString("./model/72x72/");
     else if(144==igrid)
        qsavedir = QString("./model/144x144/");

     QString strfilename = pfilename;

     QRegExp rxnum("(\\d+)");
     QStringList listother = strfilename.split(rxnum);

     QRegExp rxother("(\\D+)");
     QStringList listnum = strfilename.split(rxother);

     QString strkey = strfilename ;
     QString strkey2;
     QString strkey3;
     QString strlast  ;
    if(listother.size()>1)
    {
        QString qstr = listother[0];
        if(qstr!=""&&qstr.size()==1)
            strkey = qstr;//strfilename.mid(0,1) ;
        else if(qstr.size()==2)
        {
            strkey = qstr.mid(0,1);//strfilename.mid(0,1) ;
            strkey2 = qstr.mid(1,1);
        }
        else if(qstr.size()==3)
        {
            strkey = qstr.mid(0,1);//strfilename.mid(0,1) ;
            strkey2 = qstr.mid(1,1);
            strkey3 = qstr.mid(2,1);
        }

        if(listnum.size()<2)
        {
            strkey = listnum[0].mid(0,1);
            strlast = listnum[0].mid(1);
        }
        else
        if(listnum[1]!="")
            strlast = listnum[1];//strfilename.mid(1) ;
    }
    QString strbase;
    QString charstring = char2string(strkey);
    if(!strkey2.isEmpty())
    {
        charstring = charstring + QString("_") + char2string(strkey2);
    }
    if(!strkey3.isEmpty())
    {
        charstring = charstring + QString("_") + char2string(strkey3);
    }
    //strbase = QString("_") + charstring;
    strbase = charstring;

    QString qstrsavepat = qsavedir +strbase +QString("_")+ strlast + QString(".pat");
     QString qstrsaveimp = qsavedir +strbase +QString("_")+ strlast + QString(".imp");
    QString qstrbase = strbase +QString("_")+ strlast ;


    //fastmatch::savemodelfile(qstrsavepat.toStdString().c_str());
    fastmatch::savefastimagemodel(qstrsaveimp.toStdString().c_str());
    fastmatch::savefastimagepatmodel(qstrsavepat.toStdString().c_str());
 

 

    return;

    int ifontsize = m_fontlist_l12.size();
    for(int in = 0;in<ifontsize;in++)
    {

       QString qstr = m_fontlist_l12[in];
       if(strfilename==qstr)
       {
           fastmatch::setcurmodels(in);
           fastmatch::setcurimagemodels(in);
           break;
       }
       if(in==ifontsize-1)
       {
           //fastmatch::addmodels(qstrsavepat.toStdString().c_str());
           fastmatch::addimagemodels_l12(qstrsaveimp.toStdString().c_str());


            m_filenamelist_l12.append(qstrbase);
            m_fontlist_l12.append(strfilename);

       }
    }

    ix = arectrecover.x();
    iy = arectrecover.y();
    iw = arectrecover.width();
    ih = arectrecover.height();

    QShape::setrect(ix,iy,iw,ih);

    levelmodel();

}

void QEasyORC::checklearn(const char *pfilename )
{

    if(m_idebugrectsnum<0)
        return;
    QRect arect = g_pbackfindobject->getgridex(m_idebugrectsnum);

    int imaxlen = g_pbackfindobject->getobjectgridw();
    int igrid = fastmatch::GetRectGridLevel(imaxlen);

    int ix = arect.x();
    int iy = arect.y();
    int iw = arect.width();
    int ih = arect.height();

    QRect arectrecover = QShape::rect();

        fastmatch::setmatchrect(ix,iy,iw,ih);
        findline::setrect(ix,iy,iw,ih);
        fastmatch::setthre(8);
        fastmatch::setcomparegap(2);
        fastmatch::setmethod(0);
        fastmatch::setlinegap(1);
        fastmatch::SetWHgap(1,1);
        fastmatch::setlinesamplerate(0.002);
        fastmatch::setfilter(0,0,10000);
        fastmatch::learn(g_pbackobjectimage);
        fastmatch::setmatchrect(ix,iy,iw,ih);
    fastmatch::setfindnum(1);
    fastmatch::setmatchthre(3);
    fastmatch::setfindnum(1);
    fastmatch::setmatchthre(3);
    fastmatch::match(g_pbackobjectimage);

    fastmatch::imagelearncheck(m_imagetype,1,igrid);

    QString qsavedir;
    if(12==igrid)
        qsavedir = QString("./model/12x12/");
    else if(24==igrid)
        qsavedir = QString("./model/24x24/");
    else if(36==igrid)
        qsavedir = QString("./model/36x36/");
     else if(72==igrid)
        qsavedir = QString("./model/72x72/");
    else if(144==igrid)
       qsavedir = QString("./model/144x144/");

     QString strfilename = pfilename;

     QRegExp rxnum("(\\d+)");
     QStringList listother = strfilename.split(rxnum);

     QRegExp rxother("(\\D+)");
     QStringList listnum = strfilename.split(rxother);

     QString strkey = strfilename ;
     QString strkey2;
     QString strkey3;
     QString strlast  ;
    if(listother.size()>1)
    {
        QString qstr = listother[0];
        if(qstr!=""&&qstr.size()==1)
            strkey = qstr;//strfilename.mid(0,1) ;
        else if(qstr.size()==2)
        {
            strkey = qstr.mid(0,1);//strfilename.mid(0,1) ;
            strkey2 = qstr.mid(1,1);
        }
        else if(qstr.size()==3)
        {
            strkey = qstr.mid(0,1);//strfilename.mid(0,1) ;
            strkey2 = qstr.mid(1,1);
            strkey3 = qstr.mid(2,1);
        }

        if(listnum.size()<2)
        {
            strkey = listnum[0].mid(0,1);
            strlast = listnum[0].mid(1);
        }
        else if(listnum[1]!="")
            strlast = listnum[1];//strfilename.mid(1) ;
    }
    QString strbase;
    QString charstring = char2string(strkey);
    if(!strkey2.isEmpty())
    {
        charstring = charstring + QString("_") + char2string(strkey2);
    }
    if(!strkey3.isEmpty())
    {
        charstring = charstring + QString("_") + char2string(strkey3);
    }

    strbase = charstring;

    QString qstrsavepat = qsavedir +strbase +QString("_")+ strlast + QString(".pat");
     QString qstrsaveimp = qsavedir +strbase +QString("_")+ strlast + QString(".imp");
    QString qstrbase = strbase +QString("_")+ strlast ;

    fastmatch::savefastimagemodel(qstrsaveimp.toStdString().c_str());
    fastmatch::savemodelfile(qstrsavepat.toStdString().c_str());
    //fastmatch::savefastimagepatmodel(qstrsavepat.toStdString().c_str());

    return;

    int ifontsize = m_fontlist_l12.size();
    for(int in = 0;in<ifontsize;in++)
    {

       QString qstr = m_fontlist_l12[in];
       if(strfilename==qstr)
       {
           fastmatch::setcurmodels(in);
           fastmatch::setcurimagemodels(in);
           break;
       }
       if(in==ifontsize-1)
       {
           //fastmatch::addmodels(qstrsavepat.toStdString().c_str());
           fastmatch::addimagemodels_l12(qstrsaveimp.toStdString().c_str());


            m_filenamelist_l12.append(qstrbase);
            m_fontlist_l12.append(strfilename);

       }
    }

    ix = arectrecover.x();
    iy = arectrecover.y();
    iw = arectrecover.width();
    ih = arectrecover.height();

    QShape::setrect(ix,iy,iw,ih);

    levelmodel();

}
void QEasyORC::checkmatch(const char *pfilename )
{
    if(m_idebugrectsnum<0)
        return;
    QRect arect = g_pbackfindobject->getgridex(m_idebugrectsnum);

    int imaxlen = g_pbackfindobject->getobjectgridw();
    int igrid = fastmatch::GetRectGridLevel(imaxlen);

    int ix = arect.x();
    int iy = arect.y();
    int iw = arect.width();
    int ih = arect.height();

    QRect arectrecover = QShape::rect();

    fastmatch::setmatchrect(ix,iy,iw,ih);
    findline::setrect(ix,iy,iw,ih);

    fastmatch::setthre(8);
    fastmatch::setcomparegap(2);
    fastmatch::setmethod(0);
    fastmatch::setlinegap(1);
    fastmatch::SetWHgap(1,1);
    fastmatch::setlinesamplerate(0.002);
    fastmatch::setfilter(0,0,10000);

if(1)
{

    QString qsavedir;
    if(12==igrid)
        qsavedir = QString("./model/12x12/");
    else if(24==igrid)
        qsavedir = QString("./model/24x24/");
    else if(36==igrid)
        qsavedir = QString("./model/36x36/");
     else if(72==igrid)
        qsavedir = QString("./model/72x72/");
    else if(144==igrid)
       qsavedir = QString("./model/144x144/");

     QString strfilename = pfilename;

     QRegExp rxnum("(\\d+)");
     QStringList listother = strfilename.split(rxnum);

     QRegExp rxother("(\\D+)");
     QStringList listnum = strfilename.split(rxother);

     QString strkey = strfilename ;
     QString strkey2;
     QString strkey3;
     QString strlast  ;
    if(listother.size()>1)
    {
        QString qstr = listother[0];
        if(qstr!=""&&qstr.size()==1)
            strkey = qstr;//strfilename.mid(0,1) ;
        else if(qstr.size()==2)
        {
            strkey = qstr.mid(0,1);//strfilename.mid(0,1) ;
            strkey2 = qstr.mid(1,1);
        }
        else if(qstr.size()==3)
        {
            strkey = qstr.mid(0,1);//strfilename.mid(0,1) ;
            strkey2 = qstr.mid(1,1);
            strkey3 = qstr.mid(2,1);
        }

        if(listnum.size()<2)
        {
            strkey = listnum[0].mid(0,1);
            strlast = listnum[0].mid(1);
        }
        else if(listnum[1]!="")
            strlast = listnum[1];//strfilename.mid(1) ;
    }
    QString strbase;
    QString charstring = char2string(strkey);
    if(!strkey2.isEmpty())
    {
        charstring = charstring + QString("_") + char2string(strkey2);
    }
    if(!strkey3.isEmpty())
    {
        charstring = charstring + QString("_") + char2string(strkey3);
    }

    strbase = charstring;

    QString qstrsavepat = qsavedir +strbase +QString("_")+ strlast + QString(".pat");
    QString qstrsaveimp = qsavedir +strbase +QString("_")+ strlast + QString(".imp");
    QString qstrbase = strbase +QString("_")+ strlast ;

    //fastmatch::savefastimagemodel(qstrsaveimp.toStdString().c_str());
    //fastmatch::savemodelfile(qstrsavepat.toStdString().c_str());

    fastmatch::loadmodelfile(qstrsavepat.toStdString().c_str());
    fastmatch::loadfastimagemodel(qstrsaveimp.toStdString().c_str());
}
//    if(0)
//    {
//        fastmatch::learn(g_pbackobjectimage);
//    }

    fastmatch::setmatchrect(ix,iy,iw,ih);
    fastmatch::setfindnum(1);
    fastmatch::setmatchthre(3);
    fastmatch::setfindnum(1);
    fastmatch::setmatchthre(3);
    fastmatch::match(g_pbackobjectimage);

    fastmatch::imagelearncheck(m_imagetype,1,igrid);
    //fastmatch::savefastimagepatmodel(qstrsavepat.toStdString().c_str());

    return;

}
void QEasyORC::match72()
{
    if(m_idebugrectsnum<0)
        return;
    QRect arect = g_pbackfindobject->getgridex(m_idebugrectsnum);

    int imaxlen = g_pbackfindobject->getobjectgridw();
    int igrid = fastmatch::GetRectGridLevel(imaxlen);

    int ix = arect.x();
    int iy = arect.y();
    int iw = arect.width();
    int ih = arect.height();

    QRect arectrecover = QShape::rect();

    if(1)
    {
        fastmatch::setmatchrect(ix ,iy ,iw ,ih );
        fastmatch::setfindnum(1);
        fastmatch::setmatchthre(3);
        fastmatch::setfindnum(1);
        fastmatch::setmatchthre(3);
        fastmatch::match(g_pbackobjectimage);
        double dvalue = fastmatch::getmaxresult();


        double dimagevalue = 0;
        fastmatch::imagelearncheck(m_imagetype,1,igrid);
        dimagevalue = fastmatch::getimagemodelreslut();
        m_dvalue = dvalue;
        m_dmaxvalue = dimagevalue;
    }

   // fastmatch::imagemodelcompareshow();
    QString strt = QString(" %1 ").arg(m_dvalue) + QString(" %1").arg(m_dmaxvalue);
    QShape::setname(strt.toStdString().c_str());


}
void QEasyORC::match72_matchpat()
{
    if(m_idebugrectsnum<0)
        return;
    QRect arect = g_pbackfindobject->getgridex(m_idebugrectsnum);

    int ix = arect.x();
    int iy = arect.y();
    int iw = arect.width();
    int ih = arect.height();

    QRect arectrecover = QShape::rect();

    if(1)
    {
        fastmatch::setmatchrect(ix ,iy ,iw ,ih );
        fastmatch::setfindnum(1);
        fastmatch::setmatchthre(3);
        fastmatch::setfindnum(1);
        fastmatch::setmatchthre(3);
        fastmatch::match(g_pbackobjectimage);
        double dvalue = fastmatch::getmaxresult();
        m_dvalue = dvalue;
    }

    QString strt = QString(" %1 ").arg(m_dvalue) ;
    QShape::setname(strt.toStdString().c_str());


}
void QEasyORC::match72_matchimg()
{
    int imaxlen = g_pbackfindobject->getobjectgridw();
    int igrid = fastmatch::GetRectGridLevel(imaxlen);
    double dimagevalue = 0;
    fastmatch::imagelearncheck(m_imagetype,1,igrid);
    dimagevalue = fastmatch::getimagemodelreslut();
    m_dmaxvalue = dimagevalue;

    QString strt = QString(" %1 %2").arg(m_dvalue).arg(m_dmaxvalue) ;
    QShape::setname(strt.toStdString().c_str());

}
void QEasyORC::learnmass_36(const char *pfilename )
{
	QRect arect = g_pbackfindobject->getgridex(m_idebugrectsnum);

	int iobjw = g_pbackfindobject->getresultw(m_idebugrectsnum);
	int iobjh = g_pbackfindobject->getresulth(m_idebugrectsnum);
	int imaxlen = iobjw>iobjh?iobjw:iobjh;
	int igrid = 36;// fastmatch::GetRectGridLevel(imaxlen);

	int ix = arect.x();
	int iy = arect.y();
	int iw = arect.width();
	int ih = arect.height();
	QRect arectrecover = QShape::rect();

	fastmatch::setmatchrect(ix,iy,iw,ih);
	findline::setrect(ix,iy,iw,ih);
	fastmatch::setthre(8);
    fastmatch::setcomparegap(2);
	fastmatch::setmethod(0);
	fastmatch::setlinegap(1);
	fastmatch::SetWHgap(1,1);
	fastmatch::setlinesamplerate(0.002);
	fastmatch::setfilter(0,0,10000);
	fastmatch::learn(g_pbackobjectimage);
	fastmatch::setmatchrect(ix,iy,iw,ih);
	fastmatch::setfindnum(1);
	fastmatch::setmatchthre(3);
	fastmatch::setfindnum(1);
	fastmatch::setmatchthre(3);
	fastmatch::match(g_pbackobjectimage);

	fastmatch::imagelearnmass(-1,1,igrid);

	QString qsavedir;
	if(12==igrid)
		qsavedir = QString("./model/12x12/");
	else if(24==igrid)
		qsavedir = QString("./model/24x24/");
	else if(36==igrid)
		qsavedir = QString("./model/36x36/");
    else if(72==igrid)
        qsavedir = QString("./model/72x72/");
    else if(144==igrid)
        qsavedir = QString("./model/144x144/");
	else
		qsavedir = QString("./model/12x12/");

	QString strfilename = pfilename;

	QRegExp rxnum("(\\d+)");
	QStringList listother = strfilename.split(rxnum);

	QRegExp rxother("(\\D+)");
	QStringList listnum = strfilename.split(rxother);

	QString strkey = strfilename ;
	QString strkey2;
	QString strkey3;
	QString strlast  ;
	if(listother.size()>1)
	{
		QString qstr = listother[0];
		if(qstr!=""&&qstr.size()==1)
			strkey = qstr;//strfilename.mid(0,1) ;
		else if(qstr.size()==2)
		{
			strkey = qstr.mid(0,1);//strfilename.mid(0,1) ;
			strkey2 = qstr.mid(1,1);
		}
		else if(qstr.size()==3)
		{
			strkey = qstr.mid(0,1);//strfilename.mid(0,1) ;
			strkey2 = qstr.mid(1,1);
			strkey3 = qstr.mid(2,1);
		}

		if(listnum.size()<2)
		{
			strkey = listnum[0].mid(0,1);
			strlast = listnum[0].mid(1);
		}
		else
			if(listnum[1]!="")
				strlast = listnum[1];//strfilename.mid(1) ;
	}
	QString strbase;
	QString charstring = char2string(strkey);
	if(!strkey2.isEmpty())
	{
		charstring = charstring + QString("_") + char2string(strkey2);
	}
	if(!strkey3.isEmpty())
	{
		charstring = charstring + QString("_") + char2string(strkey3);
	}
	//strbase = QString("_") + charstring;
	strbase = charstring;

	QString qstrsavepat = qsavedir +strbase +QString("_")+ strlast + QString(".pat");
	QString qstrsaveimp = qsavedir +strbase +QString("_")+ strlast + QString(".imp");
	QString qstrbase = strbase +QString("_")+ strlast ;


	//fastmatch::savemodelfile(qstrsavepat.toStdString().c_str());
	fastmatch::savefastimagemodel(qstrsaveimp.toStdString().c_str());
	fastmatch::savefastimagepatmodel(qstrsavepat.toStdString().c_str());




	return;

	int ifontsize = m_fontlist_l12.size();
	for(int in = 0;in<ifontsize;in++)
	{

		QString qstr = m_fontlist_l12[in];
		if(strfilename==qstr)
		{
			fastmatch::setcurmodels(in);
			fastmatch::setcurimagemodels(in);
			break;
		}
		if(in==ifontsize-1)
		{
			//fastmatch::addmodels(qstrsavepat.toStdString().c_str());
			fastmatch::addimagemodels_l12(qstrsaveimp.toStdString().c_str());


			m_filenamelist_l12.append(qstrbase);
			m_fontlist_l12.append(strfilename);

		}
	}

	ix = arectrecover.x();
	iy = arectrecover.y();
	iw = arectrecover.width();
	ih = arectrecover.height();

	QShape::setrect(ix,iy,iw,ih);

	levelmodel();

}

void QEasyORC::setlevelstring()
{
  int istrsize = m_fontlist_l12.size();
  int istrsize1 = m_fontlist_l36.size();

  int ijsize = getlevel6_12map().size();
  int iksize = getlevel12_36map().size();
  int ilsize = getlevel36_72map().size();

  int iaddmax = ijsize - istrsize;
  int iaddmax1 = iksize - istrsize1;
  for(int il=0;il<ilsize;il++)
  {
        int ilnum = getlevel36_72map()[il];
        if(ilnum >= istrsize1)
        {
            for(int iz=0;iz<iaddmax1;iz++)
            {
                if(ilnum == istrsize1+iz)
                {
                     m_fontlist_l36.push_back(m_fontlist_l72[il]);
                }
            }
        }
  }
  for(int ik=0;ik<iksize;ik++)
  {
     int iknum = getlevel12_36map()[ik];
     if(iknum >= istrsize)
     {
        for(int iz=0;iz<iaddmax;iz++)
        {
            if(iknum == istrsize+iz)
            {
               // m_fontlist_l12.push_back(QString("l36[%1]").arg(ik));
                 m_fontlist_l12.push_back(m_fontlist_l36[ik]);
            }
        }

     }
  }
}
void QEasyORC::levelmodel()
{
    list_duplicatesmodel_l12();
    list_duplicatesmodel_l36();
    list_duplicatesmodel_l72();

    levelmodels_l72tol36();
    levelmodels_l36tol12();
    levelmodels_l12tol6();
    levelmodels_l6tol3();

}
void QEasyORC::setgrid(int iw,int igrid)
{
	fastmatch::setgrid(iw,igrid);
}
void QEasyORC::savelevelmodel()
{
    savelevel0_l1();
}
void QEasyORC::setmatchvalid(double dthre)
{
    m_dmatchthre = dthre;
}
void QEasyORC::setusingobject(int iusing)
{
    m_icompareobject = iusing;
}
void QEasyORC::fontorc()
{
   // RectsShape arects = g_pbackfindobject->getresultrects();
m_ilevle = 2;
    m_resultstrlist.clear();
    m_resultstring.clear();
    double dvalue = 0;
    double dimagevalue = 0;
    double dmaxvalue = 0;
    int imodelobjectw = 0;
    int imodelobjectb = 0;
    int imatchobjectw = 0;
    int imatchobjectb = 0;

    int iresultfont = 0;
    int iareasnum =   g_pbackfindobject->getresultobjsnum();
    if(iareasnum<=0)
      return;

    for(int ia=0;ia<iareasnum;ia++)
    {
           if(-1==m_idebugrectsnum
             ||ia==m_idebugrectsnum)
           {
               int isize = m_fontlist_l12.size();
               QRect arect = g_pbackfindobject->getgrid(ia);
               int ix = arect.x();
               int iy = arect.y();
               int iw = arect.width();
               int ih = arect.height();
               fastmatch::setmatchrect(ix,iy,iw,ih);
               dmaxvalue = 0;
               iresultfont = 0;
               for(int i=0;i<isize;i++)
               {
                   if(-1==m_idebugfontnum
                     ||i==m_idebugfontnum)
                   {
                       fastmatch::modelstocurrent_l12(i);
                       fastmatch::imagemodelstocurrent_l12(i);
                       //fastmatch::objectmodelstocurrent(i);
                       imodelobjectw = fastmatch::getmodeleasyobjectw_l12(i);
                       imodelobjectb = fastmatch::getmodeleasyobjectb_l12(i);

                       fastmatch::Match(*g_pbackobjectimage);

                       dvalue = fastmatch::getmaxresult();

                       dimagevalue = 0;
                       if(dvalue>=m_dmatchthre)//0.6
                       {
                           fastmatch::imagematch(-1,1);
                           dimagevalue = fastmatch::getimagemodelreslut();

                           imatchobjectb = fastmatch::geteasyobjectb();
                           imatchobjectw = fastmatch::geteasyobjectw();
                           int iobj = 1;
                           if(m_icompareobject==0)
                            {
                             iobj = 1;
                           }
                           else if(m_icompareobject==1)
                           {
                            iobj = 0;
                             if(imatchobjectb==imodelobjectb
                             &&imatchobjectw==imodelobjectw)
                            {
                            iobj = 1;
                            }
                           }
                           else if(m_icompareobject==2)
                           {
                               iobj = 0;
                             if(imatchobjectb==imodelobjectb)
                              {
                              iobj = 1;
                              }
                           }
                           else if(m_icompareobject==3)
                           {
                               iobj = 0;

                               if(imatchobjectw==imodelobjectw)
                              {
                              iobj = 1;
                              }
                           }

                           if(dimagevalue>dmaxvalue
                           &&iobj>0)
                          {
                           dmaxvalue = dimagevalue;
                           iresultfont = i;
                          }
                           if(-1==m_idebugfontnum
                              &&100==dimagevalue)
                               break;

                       }
                   }
               }

               if(-1==m_idebugrectsnum
                &&-1==m_idebugfontnum)
               {
                   QRegExp rxnum("(\\d+)");
                   QStringList listother = m_fontlist_l12[iresultfont].split(rxnum);

                   QRegExp rxother("(\\D+)");
                   QStringList listnum = m_fontlist_l12[iresultfont].split(rxother);

                   QString strkey = m_fontlist_l12[iresultfont] ;

                   if(listother.size()>0)
                  {
                      if(listother[0]!="")
                          strkey = listother[0];
                      if(listnum.size()<2)
                      {
                          strkey = listnum[0].mid(0,1);
                      }


                  }
                   m_resultstrlist.push_back(strkey);
                   m_resultstring.append(strkey);

               }
               else
               {
                   QString strkey = m_fontlist_l12[iresultfont] ;
                   m_resultstrlist.push_back(strkey);
                   m_resultstring.append(strkey);

               }
           }
       }
       if(-1!=m_idebugrectsnum&&-1!=m_idebugfontnum)
       {
           QString astr = QString("  %1   %2 b(%3 %5)w(%4 %6)").arg(dvalue).arg(dimagevalue).arg(imatchobjectb).arg(imatchobjectw).arg(imodelobjectb).arg(imodelobjectw);
           QShape::setname(astr.toStdString().c_str());
           //fastmatch::imagemodelcompareshow();
       }
       else if(-1!=m_idebugrectsnum&&-1==m_idebugfontnum)
       {
           fastmatch::modelstocurrent_l12(iresultfont);
           fastmatch::imagemodelstocurrent_l12(iresultfont);


           fastmatch::Match(*g_pbackobjectimage);

           fastmatch::imagematch(-1,1);
           imodelobjectw = fastmatch::getmodeleasyobjectw_l12(iresultfont);
           imodelobjectb = fastmatch::getmodeleasyobjectb_l12(iresultfont);

           imatchobjectb = fastmatch::geteasyobjectb();
           imatchobjectw = fastmatch::geteasyobjectw();
           //fastmatch::imagemodelcompareshow();
           QString strt = QString(" %1 ").arg(iresultfont) + m_resultstring + QString(" %1").arg(dmaxvalue);
           strt = strt + QString(" b(%3 %5)w(%4 %6)").arg(imatchobjectb).arg(imatchobjectw).arg(imodelobjectb).arg(imodelobjectw);
           QShape::setname(strt.toStdString().c_str());
       }
       else
           QShape::setname(m_resultstring.toStdString().c_str());
}

void QEasyORC::SelectModel(int ilevle,int inum)
{
    m_ilevle = ilevle;
    switch (ilevle) {
    case 0:
        fastmatch::modelstocurrent_l3(inum);
        fastmatch::imagemodelstocurrent_l3(inum);
        m_imodelobjectw = fastmatch::getmodeleasyobjectw_l3(inum);
        m_imodelobjectb = fastmatch::getmodeleasyobjectb_l3(inum);

        break;
    case 1:
        fastmatch::modelstocurrent_l6(inum);
        fastmatch::imagemodelstocurrent_l6(inum);
        m_imodelobjectw = fastmatch::getmodeleasyobjectw_l6(inum);
        m_imodelobjectb = fastmatch::getmodeleasyobjectb_l6(inum);

        break;
    case 2:
        fastmatch::modelstocurrent_l12(inum);
        fastmatch::imagemodelstocurrent_l12(inum);
        m_imodelobjectw = fastmatch::getmodeleasyobjectw_l12(inum);
        m_imodelobjectb = fastmatch::getmodeleasyobjectb_l12(inum);
        break;
    case 3:
        fastmatch::modelstocurrent_l36(inum);
        fastmatch::imagemodelstocurrent_l36(inum);
        m_imodelobjectw = fastmatch::getmodeleasyobjectw_l36(inum);
        m_imodelobjectb = fastmatch::getmodeleasyobjectb_l36(inum);
        break;
    case 4:
        fastmatch::modelstocurrent_l72(inum);
        fastmatch::imagemodelstocurrent_l72(inum);
        m_imodelobjectw = fastmatch::getmodeleasyobjectw_l72(inum);
        m_imodelobjectb = fastmatch::getmodeleasyobjectb_l72(inum);
        break;
    }
}

void QEasyORC::selectmodel72(const char *pfilename)
{
    SelectNameModel(4,pfilename);
}

void QEasyORC::SelectNameModel(int ilevel,const char *pfilename)
{
    m_ilevle = ilevel;

    int iselnum = 0;
    QString strname(pfilename);

    switch (ilevel)
    {

        case 2:
        {
            int isize = m_filenamelist_l12.size();
            for(int i=0;i<isize;i++)
            {
                if(strname == m_filenamelist_l12[i])
                {
                    iselnum = i;
                    break;
                }
            }
        }
        break;

        case 3:
        {
            int isize = m_filenamelist_l36.size();
            for(int i=0;i<isize;i++)
            {
                if(strname == m_filenamelist_l36[i])
                {
                    iselnum = i;
                    break;
                }
            }
        }
        break;


        case 4:
            {
                int isize = m_fontlist_l72.size();
                for(int i=0;i<isize;i++)
                {
                    if(strname == m_fontlist_l72[i])
                    {
                        iselnum = i;
                        break;
                    }
                }
            }
        break;
    }

    SelectModel(ilevel,iselnum);

}
void QEasyORC::modelmethod(int itype)
{
    fastmatch::modelmethod(itype);
}

void QEasyORC::fontorc_level(int ilevel)
{
     int iareasnum =   g_pbackfindobject->getresultobjsnum();

     m_resultstrlist.clear();
     m_resultstring.clear();
    if(iareasnum<=0)
      return;
    m_ilevle = ilevel;
    if(0==ilevel)
    {
        m_l3resultlist.clear();
    }
    else if(1==ilevel)
    {
        m_l6resultlist.clear();
    }
    else if(2==ilevel)
    {
        m_l12resultlist.clear();
    }
    double dvalue = 0;
    double dimagevalue = 0;
    double dmaxvalue = 0;
   // int imodelobjectw = 0;
   // int imodelobjectb = 0;
    int imatchobjectw = 0;
    int imatchobjectb = 0;

    int iresultfont = 0;

    for(int ia=0;ia<iareasnum;ia++)
    {
           if(-1==m_idebugrectsnum
             ||ia==m_idebugrectsnum)
           {
               int isize = imagefastmodelsize(ilevel);
               QRect arect = g_pbackfindobject->getgrid(ia);
               int ix = arect.x();
               int iy = arect.y();
               int iw = arect.width();
               int ih = arect.height();
               g_pbackobjectimage->SetROI(ix,iy,iw,ih);

               switch (ilevel)
               {
                   case 0:
                       fastmatch::setmatchrect(ix,iy,iw*0.25,ih*0.25);

                       g_pbackobjectimage->ROIColorTable();
                       g_pbackobjectimage->ROIColorTableBlur(0,-1);
                       g_pbackobjectimage->ROIColorTableEasyThre(1);
                       g_pbackobjectimage->ROIImageZoom(g_pbackimage,0.25,0.25);
                       break;
                   case 1:
                       fastmatch::setmatchrect(ix,iy,iw*0.5,ih*0.5);

                       g_pbackobjectimage->ROIColorTable();
                       g_pbackobjectimage->ROIColorTableBlur(0,-1);
                       g_pbackobjectimage->ROIColorTableEasyThre(1);
                       g_pbackobjectimage->ROIImageZoom(g_pbackimage,0.5,0.5);
                       break;
               default:
                   case 2:
                       fastmatch::setmatchrect(ix,iy,iw,ih);

                       g_pbackobjectimage->ROIColorTable();
                       g_pbackobjectimage->ROIColorTableBlur(0,-1);
                       g_pbackobjectimage->ROIColorTableEasyThre(1);
                       g_pbackobjectimage->ROIImageZoom(g_pbackimage,1,1);
                       break;
               }

               dmaxvalue = 0;
               int imaxnum = 0;
               iresultfont = 0;
               for(int i=0;i<isize;i++)
               {
                   if(-1==m_idebugfontnum
                     ||i==m_idebugfontnum)
                   {
                       SelectModel(ilevel,i);
                       fastmatch::Match(*g_pbackimage);//g_pbackimage;
                       dvalue = fastmatch::getmaxresult();
                       dimagevalue = 0;
                       if(dvalue>=dmaxvalue)
                       {
                           imaxnum = i;
                           dmaxvalue = dvalue;
                       }
                       if((0==ilevel&&-1!=m_idebugfontnum)
                          ||1==ilevel
                          ||2==ilevel)
                       {
                           if(dvalue>=m_dmatchthre)//0.6
                           {
                               switch (ilevel)
                               {
                                   case 0:
                                       fastmatch::imagematchex(3);
                                       break;
                                   case 1:
                                       fastmatch::imagematchex(6);
                                       break;
                                   case 2:
                                       fastmatch::imagematchex(12);
                                       break;
                               }
                               dimagevalue = fastmatch::getimagemodelreslut();

                               imatchobjectb = fastmatch::geteasyobjectb();
                               imatchobjectw = fastmatch::geteasyobjectw();
                               int iobj = 1;
                               if(m_icompareobject==0)
                                {
                                 iobj = 1;
                               }
                               else if(m_icompareobject==1)
                               {
                                iobj = 0;
                                 if(imatchobjectb==m_imodelobjectb
                                 &&imatchobjectw==m_imodelobjectw)
                                {
                                iobj = 1;
                                }
                               }
                               else if(m_icompareobject==2)
                               {
                                   iobj = 0;
                                 if(imatchobjectb==m_imodelobjectb)
                                  {
                                  iobj = 1;
                                  }
                               }
                               else if(m_icompareobject==3)
                               {
                                   iobj = 0;

                                   if(imatchobjectw==m_imodelobjectw)
                                  {
                                  iobj = 1;
                                  }
                               }

                               if(dimagevalue>dmaxvalue
                               &&iobj>0)
                              {
                               dmaxvalue = dimagevalue;
                               iresultfont = i;
                              }
                               if(-1==m_idebugfontnum
                                  &&100==dimagevalue)
                                   break;
                           }

                       }
                   }
               }

               if(-1==m_idebugfontnum
                  &&0==ilevel)
               {
                   SelectModel(ilevel,imaxnum);
                   fastmatch::Match(*g_pbackimage);//g_pbackimage;
                   dvalue = fastmatch::getmaxresult();
                   switch (ilevel)
                   {
                       case 0:
                           fastmatch::imagematchex(3);
                           break;
                       case 1:
                           fastmatch::imagematchex(6);
                           break;
                       case 2:
                           fastmatch::imagematchex(12);
                           break;
                       case 3:
	                       fastmatch::imagematchex(36);
						   break;
                   }
                   dimagevalue = fastmatch::getimagemodelreslut();

                   imatchobjectb = fastmatch::geteasyobjectb();
                   imatchobjectw = fastmatch::geteasyobjectw();
                   int iobj = 1;
                   if(m_icompareobject==0)
                    {
                     iobj = 1;
                   }
                   else if(m_icompareobject==1)
                   {
                    iobj = 0;
                     if(imatchobjectb==m_imodelobjectb
                     &&imatchobjectw==m_imodelobjectw)
                    {
                    iobj = 1;
                    }
                   }
                   else if(m_icompareobject==2)
                   {
                       iobj = 0;
                     if(imatchobjectb==m_imodelobjectb)
                      {
                      iobj = 1;
                      }
                   }
                   else if(m_icompareobject==3)
                   {
                       iobj = 0;

                       if(imatchobjectw==m_imodelobjectw)
                      {
                      iobj = 1;
                      }
                   }

                   if(dimagevalue>dmaxvalue
                   &&iobj>0)
                  {
                   dmaxvalue = dimagevalue;
                   iresultfont = imaxnum;
                  }
                   m_l3resultlist.push_back(imaxnum);
               }
               else if(1==ilevel)
               {
                   m_l6resultlist.push_back(iresultfont);
               }
               else if(2==ilevel)
                {
                    if(-1==m_idebugrectsnum
                     &&-1==m_idebugfontnum)
                    {
                        QRegExp rxnum("(\\d+)");
                        QStringList listother = m_fontlist_l12[iresultfont].split(rxnum);

                        QRegExp rxother("(\\D+)");
                        QStringList listnum = m_fontlist_l12[iresultfont].split(rxother);

                        QString strkey = m_fontlist_l12[iresultfont] ;
                       if(listother.size()>0)
                       {
                           if(listother[0]!="")
                               strkey = listother[0];
                           if(listnum.size()<2)
                           {
                               strkey = listnum[0].mid(0,1);
                           }
                       }
                        m_resultstrlist.push_back(strkey);
                        m_resultstring.append(strkey);

						m_l12resultlist.push_back(iresultfont);
                    }
                    else
                    {
                        QString strkey = m_fontlist_l12[iresultfont] ;
                        m_resultstrlist.push_back(strkey);
                        m_resultstring.append(strkey);
						m_l12resultlist.push_back(iresultfont);

                    }

                }
           }
    }
       if(-1!=m_idebugrectsnum&&-1!=m_idebugfontnum)
       {
           QString astr = QString("  %1   %2 b(%3 %5)w(%4 %6)").arg(dvalue).arg(dimagevalue).arg(imatchobjectb).arg(imatchobjectw).arg(m_imodelobjectb).arg(m_imodelobjectw);
           QShape::setname(astr.toStdString().c_str());
          // fastmatch::imagemodelcompareshow();
       }
       else if(-1!=m_idebugrectsnum&&-1==m_idebugfontnum)
       {
           //fastmatch::modelstocurrent(iresultfont);
           //fastmatch::imagemodelstocurrent_l12(iresultfont);
           ///
           //fastmatch::modelstocurrent(i);
           //fastmatch::imagemodelstocurrent_l12(i);
           //imodelobjectw = fastmatch::getmodeleasyobjectw(i);
           //imodelobjectb = fastmatch::getmodeleasyobjectb(i);
           SelectModel(ilevel,iresultfont);
           ///

           fastmatch::Match(*g_pbackobjectimage);

           switch (ilevel)
           {
               case 0:
                   fastmatch::imagematchex(3);
                   break;
               case 1:
                   fastmatch::imagematchex(6);
                   break;
               case 2:
                   fastmatch::imagematchex(12);
				   break;
			   case 3:
				   fastmatch::imagematchex(36);
				   break;
           }
           //imodelobjectw = fastmatch::getmodeleasyobjectw(iresultfont);
           //imodelobjectb = fastmatch::getmodeleasyobjectb(iresultfont);

           imatchobjectb = fastmatch::geteasyobjectb();
           imatchobjectw = fastmatch::geteasyobjectw();
           //fastmatch::imagemodelcompareshow();
           QString strt = QString(" %1 ").arg(iresultfont) + m_resultstring + QString(" %1").arg(dmaxvalue);
           strt = strt + QString(" b(%3 %5)w(%4 %6)").arg(imatchobjectb).arg(imatchobjectw).arg(m_imodelobjectb).arg(m_imodelobjectw);
           QShape::setname(strt.toStdString().c_str());
       }
       else
       {
           if(0==ilevel)
           {
               QString qshowstr;
               int isize = m_l3resultlist.size();
               for(int i=0;i<isize;i++)
               {
                   QString astrnum = QString("%1_").arg(m_l3resultlist[i]);
                   qshowstr = qshowstr + astrnum;
               }

               QShape::setname(qshowstr.toStdString().c_str());
           }
           else if(1==ilevel)
           {
               QString qshowstr;
               int isize = m_l6resultlist.size();
               for(int i=0;i<isize;i++)
               {
                   QString astrnum = QString("%1_").arg(m_l6resultlist[i]);
                   qshowstr = qshowstr + astrnum;
               }

               QShape::setname(qshowstr.toStdString().c_str());
           }
           else if(2==ilevel)
                QShape::setname(m_resultstring.toStdString().c_str());
       }
}
int QEasyORC::imagefastmapsize(int ilevel,int inum)
{
   int igetsize = 0;
   if(0==ilevel)
   {
      return imagefastmodelsize(ilevel);
   }
   else if(1==ilevel)
   {
       int il0num = m_l3resultlist[inum];
        int isize = getlevel3_6map().size();
        for(int i=0;i<isize;i++)
        {
            if(getlevel3_6map()[i]==il0num)
            {
                igetsize = igetsize + 1;
             }
         }
        return igetsize;
   }
   else if(2==ilevel)
   {
       int il1num = m_l6resultlist[inum];
        int isize = getlevel6_12map().size();
        for(int i=0;i<isize;i++)
        {
            if(getlevel6_12map()[i]==il1num)
            {
                igetsize = igetsize + 1;
            }
         }
        return igetsize;
   }
   else if(3==ilevel)
   {
	   int il1num = m_l12resultlist[inum];
	   int isize = getlevel12_36map().size();
	   for(int i=0;i<isize;i++)
	   {
		   if(getlevel12_36map()[i]==il1num)
		   {
			   igetsize = igetsize + 1;
		   }
	   }
	   return igetsize;
   }
    return 0;
}
int QEasyORC::SelectMapModel(int ilevel,int inum,int i0)
{ 
    m_ilevle = ilevel;
    int igetsize = 0;
    if(0==ilevel)
    {
        SelectModel(ilevel,i0);
        return i0;
    }
    else if(1==ilevel)
    {
         int il0num = m_l3resultlist[inum];
         int isize = getlevel3_6map().size();
         for(int i=0;i<isize;i++)
         {
             if(getlevel3_6map()[i]==il0num)
             {
                 if(igetsize == i0)
                 {
                     fastmatch::modelstocurrent_l6(i);
                     fastmatch::imagemodelstocurrent_l6(i);
                     m_imodelobjectw = fastmatch::getmodeleasyobjectw_l6(i);
                     m_imodelobjectb = fastmatch::getmodeleasyobjectb_l6(i);
                     return i;
                 }
                 igetsize = igetsize + 1;
             }
         }
    }
    else if(2==ilevel)
    {
         int il1num = m_l6resultlist[inum];
         int isize = getlevel6_12map().size();
         for(int i=0;i<isize;i++)
         {
             if(getlevel3_6map()[i]==il1num)
             {
                 if(igetsize == i0)
                 {
                     fastmatch::modelstocurrent_l12(i);
                     fastmatch::imagemodelstocurrent_l12(i);
                     m_imodelobjectw = fastmatch::getmodeleasyobjectw_l12(i);
                     m_imodelobjectb = fastmatch::getmodeleasyobjectb_l12(i);
                     return i;
                 }
                 igetsize = igetsize + 1;
             }
         }
	}
	else if(3==ilevel)
	{
		int il1num = m_l12resultlist[inum];
		int isize = getlevel12_36map().size();
		for(int i=0;i<isize;i++)
		{
			if(getlevel12_36map()[i]==il1num)
			{
				if(igetsize == i0)
				{
					fastmatch::modelstocurrent_l36(i);
					fastmatch::imagemodelstocurrent_l36(i);
					m_imodelobjectw = fastmatch::getmodeleasyobjectw_l36(i);
					m_imodelobjectb = fastmatch::getmodeleasyobjectb_l36(i);
					return i;
				}
				igetsize = igetsize + 1;
			}
		}
	}
///
    return 0;
}
int QEasyORC::resultnodesize(int ilevel,int iareanum)
{
    if(0==ilevel)
    {
       return imagefastmodelsize(ilevel);
    }
    else
    {
        levelnode anode =  m_reslutnodelist[iareanum];
        int iselectnum = 0;
        int isize = getlevel3_6map().size();
        int jsize = getlevel6_12map().size();
        int ksize = getlevel12_36map().size();
        if(0==anode.s_ilevel)
        {
            if(anode.s_inode<isize
             &&anode.s_inode>=0)
            {
                if(0==ilevel)
                {
                    return 1;
                }
                else if(1==ilevel)
                {
                    for(int i=0;i<isize;i++)
                    {
                          if(getlevel3_6map()[i]==anode.s_inode)
                          {
                              iselectnum = iselectnum + 1;
                          }
                     }
                    return iselectnum;
                }
                else if(2==ilevel)
                {
                    for(int i=0;i<isize;i++)
                    {
                          if(getlevel3_6map()[i]==anode.s_inode)
                          {
                              for(int j=0;j<jsize;j++)
                              {
                                  if(getlevel6_12map()[j]==i)
                                  {
                                      iselectnum = iselectnum + 1;
                                  }
                              }
                          }
                     }
                    return iselectnum;
                }
            }
            else
            {
                return imagefastmodelsize(ilevel);
            }
        }
        else if(1==anode.s_ilevel)
        {
            if(anode.s_inode<isize
             &&anode.s_inode>=0)
            {
                if(0==ilevel)
                {
                    return 1;
                }
                else if(1==ilevel)
                {
                    return 1;
                }
                else if(2==ilevel)
                {
                    for(int j=0;j<jsize;j++)
                    {
                        if(getlevel6_12map()[j]==anode.s_inode)
                        {
                            iselectnum = iselectnum + 1;
                        }
                    }
                    return iselectnum;
                 }
            }
            else
            {
                return imagefastmodelsize(ilevel);
            }

        }
        else if(2==anode.s_ilevel)
        {
            if(anode.s_inode<jsize
             &&anode.s_inode>=0)
            {   
				if(ilevel<3)
                    return 1;
				else if(3==ilevel)
				{
					for(int k=0;k<ksize;k++)
					{
						if(getlevel12_36map()[k]==anode.s_inode)
						{
							iselectnum = iselectnum + 1;
						}
					}
					return iselectnum;
				}
            }
            else
            {
                return imagefastmodelsize(ilevel);
            }
        }

    }

}
void QEasyORC::selectresultnode(int ilevel,int iareanum,int inum0)
{
  if(m_reslutnodelist.size()<=iareanum)
        return ;
  levelnode anode = m_reslutnodelist[iareanum];
  int iselectnum = 0;
  int isize = getlevel3_6map().size();
  int jsize = getlevel6_12map().size();
  int ksize = getlevel12_36map().size();
  if(0==anode.s_ilevel)
  {
      if(anode.s_inode<isize
      &&anode.s_inode>=0)
      {
          if(0==ilevel)
          {
              fastmatch::modelstocurrent_l3(inum0);
              fastmatch::imagemodelstocurrent_l3(inum0);
              m_imodelobjectw = fastmatch::getmodeleasyobjectw_l3(inum0);
              m_imodelobjectb = fastmatch::getmodeleasyobjectb_l3(inum0);
              m_selectnode.s_ilevel = 0;
              m_selectnode.s_inode = inum0;
              return ;
          }
          else if(1==ilevel)
          {
              for(int i=0;i<isize;i++)
              {
                    if(getlevel3_6map()[i]==anode.s_inode)
                    {
                        if(iselectnum==inum0)
                        {
                            fastmatch::modelstocurrent_l6(i);
                            fastmatch::imagemodelstocurrent_l6(i);
                            m_imodelobjectw = fastmatch::getmodeleasyobjectw_l6(i);
                            m_imodelobjectb = fastmatch::getmodeleasyobjectb_l6(i);
                            m_selectnode.s_ilevel = 1;
                            m_selectnode.s_inode = i;
                            return ;
                        }
                        iselectnum = iselectnum + 1;

                    }
               }
          }
          else if(2==ilevel)
          {
              for(int i=0;i<isize;i++)
              {
                    if(getlevel3_6map()[i]==anode.s_inode)
                    {
                        for(int j=0;j<jsize;j++)
                        {
                            if(getlevel6_12map()[j]==i)
                            {
                                if(iselectnum==inum0)
                                {
                                    fastmatch::modelstocurrent_l12(j);
                                    fastmatch::imagemodelstocurrent_l12(j);
                                    m_imodelobjectw = fastmatch::getmodeleasyobjectw_l12(j);
                                    m_imodelobjectb = fastmatch::getmodeleasyobjectb_l12(j);
                                    m_selectnode.s_ilevel = 2;
                                    m_selectnode.s_inode = j;
                                    return ;
                                }
                                iselectnum = iselectnum + 1;
                            }
                        }
                    }
               }
          }
      }
      else
      {
          if(0==ilevel)
          {
              fastmatch::modelstocurrent_l3(inum0);
              fastmatch::imagemodelstocurrent_l3(inum0);
              m_imodelobjectw = fastmatch::getmodeleasyobjectw_l3(inum0);
              m_imodelobjectb = fastmatch::getmodeleasyobjectb_l3(inum0);
              m_selectnode.s_ilevel = 0;
              m_selectnode.s_inode = inum0;
              return ;

          }
          else if(1==ilevel)
          {
              fastmatch::modelstocurrent_l6(inum0);
              fastmatch::imagemodelstocurrent_l6(inum0);
              m_imodelobjectw = fastmatch::getmodeleasyobjectw_l6(inum0);
              m_imodelobjectb = fastmatch::getmodeleasyobjectb_l6(inum0);
              m_selectnode.s_ilevel = 1;
              m_selectnode.s_inode = inum0;
              return ;

          }
          else if(2==ilevel)
          {
              fastmatch::modelstocurrent_l12(inum0);
              fastmatch::imagemodelstocurrent_l12(inum0);
              m_imodelobjectw = fastmatch::getmodeleasyobjectw_l12(inum0);
              m_imodelobjectb = fastmatch::getmodeleasyobjectb_l12(inum0);
              m_selectnode.s_ilevel = 2;
              m_selectnode.s_inode = inum0;
              return ;

          }
      }
  }
  else if(1==anode.s_ilevel)
  {
      if(anode.s_inode<isize
       &&anode.s_inode>=0)
      {
          if(0==ilevel)
          {
              fastmatch::modelstocurrent_l6(anode.s_inode);
              fastmatch::imagemodelstocurrent_l6(anode.s_inode);
              m_imodelobjectw = fastmatch::getmodeleasyobjectw_l6(anode.s_inode);
              m_imodelobjectb = fastmatch::getmodeleasyobjectb_l6(anode.s_inode);
              m_selectnode.s_ilevel = 1;
              m_selectnode.s_inode = anode.s_inode;
              return ;
          }
          else if(1==ilevel)
          {
              fastmatch::modelstocurrent_l6(anode.s_inode);
              fastmatch::imagemodelstocurrent_l6(anode.s_inode);
              m_imodelobjectw = fastmatch::getmodeleasyobjectw_l6(anode.s_inode);
              m_imodelobjectb = fastmatch::getmodeleasyobjectb_l6(anode.s_inode);
              m_selectnode.s_ilevel = 1;
              m_selectnode.s_inode = anode.s_inode;
              return ;
          }
          else if(2==ilevel)
          {
              for(int j=0;j<jsize;j++)
              {
                  if(getlevel6_12map()[j]==anode.s_inode)
                  {
                      if(iselectnum==inum0)
                      {
                          fastmatch::modelstocurrent_l12(j);
                          fastmatch::imagemodelstocurrent_l12(j);
                          m_imodelobjectw = fastmatch::getmodeleasyobjectw_l12(j);
                          m_imodelobjectb = fastmatch::getmodeleasyobjectb_l12(j);
                          m_selectnode.s_ilevel = 2;
                          m_selectnode.s_inode = j;
                          return ;
                      }
                      iselectnum = iselectnum + 1;
                  }
              }
           }
      }
      else
      {
          if(0==ilevel)
          {
                  fastmatch::modelstocurrent_l3(inum0);
                  fastmatch::imagemodelstocurrent_l3(inum0);
                  m_imodelobjectw = fastmatch::getmodeleasyobjectw_l3(inum0);
                  m_imodelobjectb = fastmatch::getmodeleasyobjectb_l3(inum0);
                  m_selectnode.s_ilevel = 0;
                  m_selectnode.s_inode = inum0;
                  return ;
          }
          else if(1==ilevel)
          {
                  fastmatch::modelstocurrent_l6(inum0);
                  fastmatch::imagemodelstocurrent_l6(inum0);
                  m_imodelobjectw = fastmatch::getmodeleasyobjectw_l6(inum0);
                  m_imodelobjectb = fastmatch::getmodeleasyobjectb_l6(inum0);
                  m_selectnode.s_ilevel = 1;
                  m_selectnode.s_inode = inum0;
                  return ;
          }
          else if(2==ilevel)
          {
                  fastmatch::modelstocurrent_l12(inum0);
                  fastmatch::imagemodelstocurrent_l12(inum0);
                  m_imodelobjectw = fastmatch::getmodeleasyobjectw_l12(inum0);
                  m_imodelobjectb = fastmatch::getmodeleasyobjectb_l12(inum0);
                  m_selectnode.s_ilevel = 2;
                  m_selectnode.s_inode = inum0;
                  return ;
           }
      }
  }
  else if(2==anode.s_ilevel)
  {
      if(anode.s_inode<jsize
       &&anode.s_inode>=0)
      {
			if(ilevel<3)
			{
          fastmatch::modelstocurrent_l12(anode.s_inode);
          fastmatch::imagemodelstocurrent_l12(anode.s_inode);
          m_imodelobjectw = fastmatch::getmodeleasyobjectw_l12(anode.s_inode);
          m_imodelobjectb = fastmatch::getmodeleasyobjectb_l12(anode.s_inode);
          m_selectnode.s_ilevel = 2;
          m_selectnode.s_inode = anode.s_inode;
          }
		  else if(3==ilevel)
			{
				for(int k=0;k<ksize;k++)
				{
					if(getlevel12_36map()[k]==anode.s_inode)
					{
						if(iselectnum==inum0)
						{
							fastmatch::modelstocurrent_l36(k);
							fastmatch::imagemodelstocurrent_l36(k);
							m_imodelobjectw = fastmatch::getmodeleasyobjectw_l36(k);
							m_imodelobjectb = fastmatch::getmodeleasyobjectb_l36(k);
							m_selectnode.s_ilevel = 3;
							m_selectnode.s_inode = k;
							return ;
						}
						iselectnum = iselectnum + 1;
					}
				}
			}
		
          
          return ;
      }
      else
      {

              if(0==ilevel)
              {
                  fastmatch::modelstocurrent_l3(inum0);
                  fastmatch::imagemodelstocurrent_l3(inum0);
                  m_imodelobjectw = fastmatch::getmodeleasyobjectw_l3(inum0);
                  m_imodelobjectb = fastmatch::getmodeleasyobjectb_l3(inum0);
                  m_selectnode.s_ilevel = 0;
                  m_selectnode.s_inode = inum0;
                  return ;

              }
              else if(1==ilevel)
              {
                  fastmatch::modelstocurrent_l6(inum0);
                  fastmatch::imagemodelstocurrent_l6(inum0);
                  m_imodelobjectw = fastmatch::getmodeleasyobjectw_l6(inum0);
                  m_imodelobjectb = fastmatch::getmodeleasyobjectb_l6(inum0);
                  m_selectnode.s_ilevel = 1;
                  m_selectnode.s_inode = inum0;
                  return ;

              }
              else if(2==ilevel)
              {
                  fastmatch::modelstocurrent_l12(inum0);
                  fastmatch::imagemodelstocurrent_l12(inum0);
                  m_imodelobjectw = fastmatch::getmodeleasyobjectw_l12(inum0);
                  m_imodelobjectb = fastmatch::getmodeleasyobjectb_l12(inum0);
                  m_selectnode.s_ilevel = 2;
                  m_selectnode.s_inode = inum0;
                  return ;

			  }
		
			
      }
  }

}
void QEasyORC::resultnodereset(int iareasnum)
{
    m_reslutnodelist.clear();
    for(int i=0;i<iareasnum;i++)
    {
        levelnode anode;
        anode.s_ilevel = 0;
        anode.s_inode = -1;
        m_reslutnodelist.push_back(anode);
    }
}
void QEasyORC::setresultnode(int inum,levelnode inode)
{
    m_reslutnodelist[inum]  = inode;
}




void QEasyORC::fontorc_levelex(int ilevel)
{
    int iareasnum = g_pbackfindobject->getresultobjsnum();

    m_resultstrlist.clear();
    m_resultstring.clear();
    if(iareasnum<=0)
      return;
    m_ilevle = ilevel;
    if(0==ilevel)
    {
        resultnodereset(iareasnum);
        m_l3resultlist.clear();
    }
    else if(1==ilevel)
    {
        if(m_l3resultlist.size()<=0)
            return;
        if(m_l3resultlist.size()!=iareasnum)
            return;
        m_l6resultlist.clear();
    }
    else if(2==ilevel)
    {
        if(m_l6resultlist.size()<=0)
            return;
        if(m_l6resultlist.size()!=iareasnum)
            return;
	}
	else if(3==ilevel)
	{
		if(m_l12resultlist.size()<=0)
			return;
		if(m_l12resultlist.size()!=iareasnum)
			return;
	}
    double dvalue = 0;
    double dimagevalue = 0;
    double dmaxvalue = 0;
    int iselmaxnum = 0;
    int imatchobjectw = 0;
    int imatchobjectb = 0;

    int iresultfont = 0;

    for(int ia=0;ia<iareasnum;ia++)
    {
           if(-1==m_idebugrectsnum
             ||ia==m_idebugrectsnum)
           {
               int isize = imagefastmapsize(ilevel,ia);

               QRect arect = g_pbackfindobject->getgrid(ia);
               int ix = arect.x();
               int iy = arect.y();
               int iw = arect.width();
               int ih = arect.height();
               //fastmatch::setmatchrect(ix,iy,iw,ih);
               g_pbackobjectimage->SetROI(ix,iy,iw,ih);

               switch (ilevel)
               {
                   case 0:
                       fastmatch::setmatchrect(ix,iy,iw*0.25,ih*0.25);

                       g_pbackobjectimage->ROIColorTable();
                       g_pbackobjectimage->ROIColorTableBlur(0,-1);
                       g_pbackobjectimage->ROIColorTableEasyThre(1);

                       m_pimagegrid->ROIImagetoModel(*g_pbackobjectimage);

                       if(0)
                       {
                           m_pimagegrid->SetUnit(12,12);
                           m_pimagegrid->UnitGrid();
                       }
                       m_pimagegrid->ZeroModel();
                       m_pimagegrid->ReGrid(12,12);
                       m_pimagegrid->GridZoom(3,3);
                       m_pimagegrid->SetUnit(3,3);


                       break;
                   case 1:
                       fastmatch::setmatchrect(ix,iy,iw*0.5,ih*0.5);

                       g_pbackobjectimage->ROIColorTable();
                       g_pbackobjectimage->ROIColorTableBlur(0,-1);
                       g_pbackobjectimage->ROIColorTableEasyThre(1);

                       m_pimagegrid->ROIImagetoModel(*g_pbackobjectimage);

                       if(0)
                       {
                           m_pimagegrid->SetUnit(12,12);
                           m_pimagegrid->UnitGrid();
                       }
                       m_pimagegrid->ZeroModel();
                       m_pimagegrid->ReGrid(12,12);

                       m_pimagegrid->GridZoom(6,6);
                       m_pimagegrid->SetUnit(6,6);
                       break;
               default:
                   case 2:
                       fastmatch::setmatchrect(ix,iy,iw,ih);

                       g_pbackobjectimage->ROIColorTable();
                       g_pbackobjectimage->ROIColorTableBlur(0,-1);
                       g_pbackobjectimage->ROIColorTableEasyThre(1);

                      // m_pimagegrid->ROIImagetoModel(*g_pbackobjectimage);
                      // m_pimagegrid->SetUnit(12,12);
                      // m_pimagegrid->UnitGrid();
                      // m_pimagegrid->ZeroModel();

                       break;
               }

               dmaxvalue = 0;
               iselmaxnum = 0;
               int imaxnum = 0;
               iresultfont = 0;
               for(int i=0;i<isize;i++)
               {
                   int iselnum = SelectMapModel(ilevel,ia,i);
                   if(-1==m_idebugfontnum
                     ||iselnum==m_idebugfontnum)
                   {
                        MatchGrid(m_pimagegrid);
                        dimagevalue = fastmatch::getimagemodelreslut();

                        imatchobjectb = fastmatch::geteasyobjectb();
                        imatchobjectw = fastmatch::geteasyobjectw();
                        int iobj = 1;
                               if(m_icompareobject==0)
                               {
                                 iobj = 1;
                               }
                               else if(m_icompareobject==1)
                               {
                                    iobj = 0;
                                     if(imatchobjectb==m_imodelobjectb
                                     &&imatchobjectw==m_imodelobjectw)
                                    {
                                    iobj = 1;
                                    }
                               }
                               else if(m_icompareobject==2)
                               {
                                   iobj = 0;
                                 if(imatchobjectb==m_imodelobjectb)
                                  {
                                  iobj = 1;
                                  }
                               }
                               else if(m_icompareobject==3)
                               {
                                   iobj = 0;

                                   if(imatchobjectw==m_imodelobjectw)
                                  {
                                  iobj = 1;
                                  }
                               }

                               if(dimagevalue>dmaxvalue
                               &&iobj>0)
                              {
                               dmaxvalue = dimagevalue;
                               iresultfont = iselnum;
                               imaxnum = iselnum;
                               iselmaxnum = i;
                              }
                               if(-1==m_idebugfontnum
                                  &&100==dimagevalue)
                               {
                                   break;
                                }
                   }
                   if(iselnum==m_idebugfontnum)
                        break;
               }

               if(0==ilevel)
               {
                   m_l3resultlist.push_back(imaxnum);
               }
               else if(1==ilevel)
               {
                   m_l6resultlist.push_back(iresultfont);
               }
               else if(2==ilevel)
                {
                    if(-1==m_idebugrectsnum
                     &&-1==m_idebugfontnum)
                    {
                        QRegExp rxnum("(\\d+)");
                        QStringList listother = m_fontlist_l12[iresultfont].split(rxnum);

                        QRegExp rxother("(\\D+)");
                        QStringList listnum = m_fontlist_l12[iresultfont].split(rxother);

                        QString strkey = m_fontlist_l12[iresultfont] ;
                       if(listother.size()>0)
                       {
                           if(listother[0]!="")
                               strkey = listother[0];
                           if(listnum.size()<2)
                           {
                               strkey = listnum[0].mid(0,1);
                           }
                       }
                        m_resultstrlist.push_back(strkey);
                        m_resultstring.append(strkey);
                    }
                    else
                    {
                        QString strkey = m_fontlist_l12[iresultfont] ;
                        m_resultstrlist.push_back(strkey);
                        m_resultstring.append(strkey);
                    }

	                m_l12resultlist.push_back(iresultfont);
                }
           }
    }
       if(-1!=m_idebugrectsnum&&-1!=m_idebugfontnum)
       {
           QString astr = QString("  %1   %2 b(%3 %5)w(%4 %6)").arg(dvalue).arg(dimagevalue).arg(imatchobjectb).arg(imatchobjectw).arg(m_imodelobjectb).arg(m_imodelobjectw);
           QShape::setname(astr.toStdString().c_str());
          // fastmatch::imagemodelcompareshow();
       }
       else if(-1!=m_idebugrectsnum&&-1==m_idebugfontnum)
       {
            //SelectModel(ilevel,iresultfont);
            int iselnum = SelectMapModel(ilevel,m_idebugrectsnum,iselmaxnum);
            fastmatch::MatchGrid(m_pimagegrid);

           imatchobjectb = fastmatch::geteasyobjectb();
           imatchobjectw = fastmatch::geteasyobjectw();
           //fastmatch::imagemodelcompareshow();
           QString strt = QString(" %1 ").arg(iresultfont) + m_resultstring + QString(" %1").arg(iselnum);
           strt = strt + QString(" b(%3 %5)w(%4 %6)").arg(imatchobjectb).arg(imatchobjectw).arg(m_imodelobjectb).arg(m_imodelobjectw);
           QShape::setname(strt.toStdString().c_str());
       }
       else
       {
           if(0==ilevel)
           {
               QString qshowstr;
               int isize = m_l3resultlist.size();
               for(int i=0;i<isize;i++)
               {
                   QString astrnum = QString("%1|").arg(m_l3resultlist[i]);
                   qshowstr = qshowstr + astrnum;
               }

               QShape::setname(qshowstr.toStdString().c_str());
           }
           else if(1==ilevel)
           {
               QString qshowstr;
               int isize = m_l6resultlist.size();
               for(int i=0;i<isize;i++)
               {
                   QString astrnum = QString("%1|").arg(m_l6resultlist[i]);
                   qshowstr = qshowstr + astrnum;
               }
               QShape::setname(qshowstr.toStdString().c_str());
           }
           else if(2==ilevel)
                QShape::setname(m_resultstring.toStdString().c_str());
       }
}

void QEasyORC::fontorc_level2()
{
   // RectsShape arects = g_pbackfindobject->getresultrects();

    int iareasnum =   g_pbackfindobject->getresultobjsnum();
    if(m_l6resultlist.size()<=0)
        return;
    if(m_l6resultlist.size()!=iareasnum)
        return;
    m_resultstrlist.clear();
    m_resultstring.clear();


    double dvalue = 0;
    double dimagevalue = 0;
    double dmaxvalue = 0;
    //int imodelobjectw = 0;
    //int imodelobjectb = 0;
    int imatchobjectw = 0;
    int imatchobjectb = 0;

    int iresultfont = 0;
    int iresultnum = 0;
    if(iareasnum<=0)
      return;

    for(int ia=0;ia<iareasnum;ia++)
    {
           if(-1==m_idebugrectsnum
             ||ia==m_idebugrectsnum)
           {
               //int isize = m_fontlist_l12.size();
               int isize = imagefastmapsize(2,ia);
               QRect arect = g_pbackfindobject->getgrid(ia);
               int ix = arect.x();
               int iy = arect.y();
               int iw = arect.width();
               int ih = arect.height();
               fastmatch::setmatchrect(ix,iy,iw,ih);
               dmaxvalue = 0;
               iresultfont = 0;
               iresultnum = 0;
               if(1==isize)
               {
                   int iselnum = SelectMapModel(2,ia,0);
                   dmaxvalue = 100;
                   iresultfont = iselnum;
                   iresultnum = 0;

               }
               else
               for(int i=0;i<isize;i++)
               {
                   int iselnum = SelectMapModel(2,ia,i);
                   if(-1==m_idebugfontnum
                     ||i==m_idebugfontnum)
                   {
                       //fastmatch::modelstocurrent(i);
                       //fastmatch::imagemodelstocurrent_l12(i);
                       //fastmatch::objectmodelstocurrent(i);
                       //imodelobjectw = fastmatch::getmodeleasyobjectw(i);
                       //imodelobjectb = fastmatch::getmodeleasyobjectb(i);

                       fastmatch::Match(*g_pbackobjectimage);

                       dvalue = fastmatch::getmaxresult();

                       dimagevalue = 0;
                       if(dvalue>=m_dmatchthre)//0.6
                       {
                           fastmatch::imagematch(-1,1);
                           dimagevalue = fastmatch::getimagemodelreslut();

                           imatchobjectb = fastmatch::geteasyobjectb();
                           imatchobjectw = fastmatch::geteasyobjectw();
                           int iobj = 1;
                           if(m_icompareobject==0)
                            {
                             iobj = 1;
                           }
                           else if(m_icompareobject==1)
                           {
                            iobj = 0;
                             if(imatchobjectb==m_imodelobjectb
                             &&imatchobjectw==m_imodelobjectw)
                            {
                            iobj = 1;
                            }
                           }
                           else if(m_icompareobject==2)
                           {
                               iobj = 0;
                             if(imatchobjectb==m_imodelobjectb)
                              {
                              iobj = 1;
                              }
                           }
                           else if(m_icompareobject==3)
                           {
                               iobj = 0;

                               if(imatchobjectw==m_imodelobjectw)
                              {
                              iobj = 1;
                              }
                           }

                           if(dimagevalue>dmaxvalue
                           &&iobj>0)
                          {
                           dmaxvalue = dimagevalue;
                           iresultfont = iselnum;
                           iresultnum = i;
                          }
                           if(-1==m_idebugfontnum
                              &&100==dimagevalue)
                               break;

                       }
                   }
               }

			   if(-1==m_idebugrectsnum
                &&-1==m_idebugfontnum)
               {
                   QRegExp rxnum("(\\d+)");
                   QStringList listother = m_fontlist_l12[iresultfont].split(rxnum);

                   QRegExp rxother("(\\D+)");
                   QStringList listnum = m_fontlist_l12[iresultfont].split(rxother);

                   QString strkey = m_fontlist_l12[iresultfont] ;
                  if(listother.size()>0)
                  {
                      if(listother[0]!="")
                          strkey = listother[0];
                      if(listnum.size()<2)
                      {
                          strkey = listnum[0].mid(0,1);
                      }


                  }
                   m_resultstrlist.push_back(strkey);
                   m_resultstring.append(strkey);

               }
               else
               {
                   QString strkey = m_fontlist_l12[iresultfont] ;
                   m_resultstrlist.push_back(strkey);
                   m_resultstring.append(strkey);

               }
           }
       }
       if(-1!=m_idebugrectsnum&&-1!=m_idebugfontnum)
       {
           QString astr = QString("  %1   %2 b(%3 %5)w(%4 %6)").arg(dvalue).arg(dimagevalue).arg(imatchobjectb).arg(imatchobjectw).arg(m_imodelobjectb).arg(m_imodelobjectw);
           QShape::setname(astr.toStdString().c_str());
           //fastmatch::imagemodelcompareshow();
       }
       else if(-1!=m_idebugrectsnum&&-1==m_idebugfontnum)
       {
           //fastmatch::modelstocurrent(iresultfont);
           //fastmatch::imagemodelstocurrent_l12(iresultfont);
           int iselnum = SelectMapModel(2,m_idebugrectsnum,iresultnum);

           fastmatch::Match(*g_pbackobjectimage);

           fastmatch::imagematch(-1,1);


           imatchobjectb = fastmatch::geteasyobjectb();
           imatchobjectw = fastmatch::geteasyobjectw();
           //fastmatch::imagemodelcompareshow();
           QString strt = QString(" %1 ").arg(iresultfont) + m_resultstring + QString(" %1").arg(dmaxvalue);
           strt = strt + QString(" b(%3 %5)w(%4 %6)").arg(imatchobjectb).arg(imatchobjectw).arg(m_imodelobjectb).arg(m_imodelobjectw);
           QShape::setname(strt.toStdString().c_str());
       }
       else
           QShape::setname(m_resultstring.toStdString().c_str());
}
void QEasyORC::checkorc_level3()
{ 
		int iareasnum =   g_pbackfindobject->getresultobjsnum();

        m_l12resultlist.clear();

        int isize = m_reslutnodelist.size();
        for(int it=0;it<isize ;it++)
        {
            if(2==m_reslutnodelist[it].s_ilevel)
            {
                m_l12resultlist.push_back(m_reslutnodelist[it].s_inode);
            }
            else
            {
                QShape::setname(QString("node result level != 2").toStdString().c_str());
                return;
            }
        }

        m_resultstrlist.clear();
		m_resultstring.clear();
 
 		double dvalue = 0;
		double dimagevalue = 0;
		double dmaxvalue = 0; 
		int imatchobjectw = 0;
		int imatchobjectb = 0;

		int iresultfont = 0;
		int iresultnum = 0;
		if(iareasnum<=0)
			return;

		for(int ia=0;ia<iareasnum;ia++)
		{
			if(-1==m_idebugrectsnum
				||ia==m_idebugrectsnum)
			{ 
				//int isize = imagefastmapsize(3,ia);
				int isize = resultnodesize(3,ia);
				QRect arect = g_pbackfindobject->getgrid(ia);
				int ix = arect.x();
				int iy = arect.y();
				int iw = arect.width();
				int ih = arect.height();
				fastmatch::setmatchrect(ix,iy,iw,ih);
				dmaxvalue = 0;
				iresultfont = 0;
				iresultnum = 0;
 				for(int i=0;i<isize;i++)
				{
                    selectresultnode(3,ia,i);
                    if(-1==m_idebugfontnum
                    ||i==m_idebugfontnum)
						{ 
							fastmatch::Match(*g_pbackobjectimage);

							dvalue = fastmatch::getmaxresult();

							dimagevalue = 0;
							if(dvalue>=m_dmatchthre)//0.6
							{
								fastmatch::imagematch_grid(-1,1,36);
								dimagevalue = fastmatch::getimagemodelreslut();

							   if(dimagevalue>dmaxvalue)
								{
									dmaxvalue = dimagevalue;

									m_iresultfont = m_selectnode.s_inode; 
                                     iresultfont = m_selectnode.s_inode;
									iresultnum = i;
								}
								if(-1==m_idebugfontnum
									&&100==dimagevalue)
									break;
							}
						}
				}

                    if(-1==m_idebugrectsnum
						&&-1==m_idebugfontnum)
					{
						QRegExp rxnum("(\\d+)");
                        QStringList listother = m_fontlist_l36[m_iresultfont].split(rxnum);

						QRegExp rxother("(\\D+)");
                        QStringList listnum = m_fontlist_l36[m_iresultfont].split(rxother);

                        QString strkey = m_fontlist_l36[m_iresultfont] ;
						if(listother.size()>0)
						{
							if(listother[0]!="")
								strkey = listother[0];
							if(listnum.size()<2)
							{
								strkey = listnum[0].mid(0,1);
							}
						}
						QString astr = QString("  %1   %2 ").arg(dvalue).arg(dimagevalue) ;
						m_resultstrlist.push_back(strkey +astr);
						m_resultstring.append(strkey+astr);

					}
					else
					{
						QString astr = QString("  %1   %2 ").arg(dvalue).arg(dimagevalue) ;

						QString strkey = m_filenamelist_l36[iresultfont] ;
						m_resultstrlist.push_back(strkey+astr);
						m_resultstring.append(strkey+astr);

					}
			}
		}
		if(-1!=m_idebugrectsnum&&-1!=m_idebugfontnum)
		{
			QString astr = QString("  %1   %2 ").arg(dvalue).arg(dimagevalue) ;
			QShape::setname(astr.toStdString().c_str());
            //fastmatch::imagemodelcompareshow();
		}
		else if(-1!=m_idebugrectsnum&&-1==m_idebugfontnum)
        {
            selectresultnode(3,m_idebugrectsnum,iresultnum);

            fastmatch::Match(*g_pbackobjectimage);

            fastmatch::imagematch_grid(-1,1,36);
  
            //fastmatch::imagemodelcompareshow();

            QString strt = QString(" %1 ").arg(iresultfont) + m_resultstring + QString(" %1").arg(dmaxvalue);
			 
			QShape::setname(strt.toStdString().c_str());
		}
		else
			QShape::setname(m_resultstring.toStdString().c_str());
}

bool QEasyORC::matchlevelnode01()
{
    MatchGrid(m_pimagegrid);
    double dimagevalue = fastmatch::getimagemodelreslut();

    int imatchobjectb = fastmatch::geteasyobjectb();
    int imatchobjectw = fastmatch::geteasyobjectw();
    int iobj = 1;
           if(m_icompareobject==0)
           {
             iobj = 1;
           }
           else if(m_icompareobject==1)
           {
                iobj = 0;
                 if(imatchobjectb==m_imodelobjectb
                 &&imatchobjectw==m_imodelobjectw)
                {
                iobj = 1;
                }
           }
           else if(m_icompareobject==2)
           {
               iobj = 0;
             if(imatchobjectb==m_imodelobjectb)
              {
              iobj = 1;
              }
           }
           else if(m_icompareobject==3)
           {
               iobj = 0;

               if(imatchobjectw==m_imodelobjectw)
              {
                  iobj = 1;
              }
           }

           if(dimagevalue>m_dmaxvalue
           &&iobj>0)
          {
               m_iselmaxnum = m_selectnode.s_inode;
               m_dmaxvalue = dimagevalue;
               m_iresultfont = m_selectnode.s_inode;
               m_imaxnum = m_selectnode.s_inode;
          }
           if(-1==m_idebugfontnum
              &&100==dimagevalue)
           {
               return false;
           }

           return true;

}
bool QEasyORC::matchlevelnode2()
{
    //fastmatch::Match(*g_pbackobjectimage);
    fastmatch::Match(*g_pbackimage);

    double dvalue = fastmatch::getmaxresult();
    double dimagevalue = 0;
    if(dvalue>=m_dmatchthre)//0.6
    {
        fastmatch::imagematch(-1,1,12,0);
        dimagevalue = fastmatch::getimagemodelreslut();
        int imatchobjectb = fastmatch::geteasyobjectb();
        int imatchobjectw = fastmatch::geteasyobjectw();
        int iobj = 1;
        if(m_icompareobject==0)
        {
          iobj = 1;
        }
        else if(m_icompareobject==1)
        {
            iobj = 0;
            if(imatchobjectb==m_imodelobjectb
            &&imatchobjectw==m_imodelobjectw)
            {
                iobj = 1;
            }
        }
        else if(m_icompareobject==2)
        {
            iobj = 0;
            if(imatchobjectb==m_imodelobjectb)
            {
                iobj = 1;
            }
        }
        else if(m_icompareobject==3)
        {
            iobj = 0;

            if(imatchobjectw==m_imodelobjectw)
            {
                iobj = 1;
            }
        }
        if(dimagevalue>m_dmaxvalue)
        {
            if(iobj>0)
            {
                m_dvalue = dvalue;
                m_dmaxvalue = dimagevalue;
                m_iresultfont = m_selectnode.s_inode;
                m_imaxnum = m_selectnode.s_inode;
            }
            else if(m_dmaxvalue<=0)
            {
                m_dvalue = dvalue;
                m_dmaxvalue = dimagevalue;
                m_iresultfont = m_selectnode.s_inode;
                m_imaxnum = m_selectnode.s_inode;
            }
       }
        if(-1==m_idebugfontnum
           &&100==dimagevalue)
        {
            return false;
        }

        return true;
    }
    else
         return true;
}
void QEasyORC::resultnodelistreset(int iareanum)
{
    m_reslutnodeslistgrid3x3.clear();
    m_reslutnodeslistgrid6x6.clear();
    m_reslutnodeslistgrid12x12.clear();
    m_reslutnodeslistgrid36x36.clear();
    m_reslutnodeslistgrid72x72.clear();

    for(int ia=0;ia<iareanum;ia++)
    {
        levelnodes anodes;
        anodes.setsearchnum(m_resultnodesearchsum);
        m_reslutnodeslistgrid3x3.push_back(anodes);
        m_reslutnodeslistgrid6x6.push_back(anodes);
        m_reslutnodeslistgrid12x12.push_back(anodes);

//        m_reslutnodeslistgrid36x36.push_back(anodes);
//        m_reslutnodeslistgrid72x72.push_back(anodes);

    }
}
bool QEasyORC::matchlevelnodelist3x3(int ia)
{
    int isize = imagefastmodelsize(0);
    double dimagevalue = 0;
    for(int i=0;i<isize;i++)
    {
        fastmatch::modelstocurrent_l3(i);
        fastmatch::imagemodelstocurrent_l3(i);
        MatchGrid(m_pimagegrid);
        dimagevalue = fastmatch::getimagemodelreslut();

        levelvalenode anode;
        anode.s_ilevel = 0;
        anode.s_inode = i;
        anode.s_dvalue = dimagevalue;

        m_reslutnodeslistgrid3x3[ia].addnode(anode);//m_resultnodesearchsum
        //if(100==dimagevalue)
        //{
        //    return true;
        //}
    }

    return true;
}
bool QEasyORC::matchlevelnodelist6x6(int ia)
{
    int isize = m_reslutnodeslistgrid3x3[ia].getnodes().size();
    double dimagevalue = 0;
    for(int i=0;i<isize;i++)
    {
        int inodenum = m_reslutnodeslistgrid3x3[ia].getnodes()[i].s_inode;

        for(int j=0;j<getlevel3_6map().size();j++)
        {
            if(getlevel3_6map()[j]==inodenum)
            {
                fastmatch::modelstocurrent_l6(j);
                fastmatch::imagemodelstocurrent_l6(j);
                MatchGrid(m_pimagegrid);
                dimagevalue = fastmatch::getimagemodelreslut();
                levelvalenode anode;
                anode.s_ilevel = 1;
                anode.s_inode = j;
                anode.s_dvalue = dimagevalue;

                m_reslutnodeslistgrid6x6[ia].addnode(anode);//m_resultnodesearchsum
                //if(100==dimagevalue)
                //{
                //    return true;
                //}

            }
        }
    }

    return true;
}
bool QEasyORC::matchlevelnodelist12x12(int ia)
{
    int isize = m_reslutnodeslistgrid6x6[ia].getnodes().size();
    double dimagevalue = 0;
    for(int i=0;i<isize;i++)
    {
        int inodenum = m_reslutnodeslistgrid6x6[ia].getnodes()[i].s_inode;

        for(int j=0;j<getlevel6_12map().size();j++)
        {
            if(getlevel6_12map()[j]==inodenum)
            {
                fastmatch::modelstocurrent_l12(j);
                fastmatch::imagemodelstocurrent_l12(j);
                //MatchGrid(m_pimagegrid);
                fastmatch::Match(*g_pbackimage);
                double dvalue = fastmatch::getmaxresult();
                double dimagevalue = 0;
                if(dvalue>=m_dmatchthre)//0.6
                {
                    fastmatch::imagematch(-1,1,12,0);
                    dimagevalue = fastmatch::getimagemodelreslut();

                    levelvalenode anode;
                    anode.s_ilevel = 2;
                    anode.s_inode = j;

                    anode.s_dvalue = dimagevalue;
                    m_reslutnodeslistgrid12x12[ia].addnode(anode);//m_resultnodesearchsum

                }
            }
        }
    }

    return true;
}
bool QEasyORC::matchlevelnodelist36x36(int ia)
{
    //fastmatch::Match(*g_pbackobjectimage);
    fastmatch::Match(*g_pbackimage);
    double dvalue = fastmatch::getmaxresult();
    double dimagevalue = 0;
    if(dvalue>=m_dmatchthre)//0.6
    {
        fastmatch::imagematch(-1,1,12,0);
        dimagevalue = fastmatch::getimagemodelreslut();
       if(dimagevalue>m_dmaxvalue)
       {
            m_dvalue = dvalue;
            m_dmaxvalue = dimagevalue;
            m_iresultfont = m_selectnode.s_inode;
            m_imaxnum = m_selectnode.s_inode;
       }
       if(-1==m_idebugfontnum
           &&100==dimagevalue)
       {
            return false;
       }
       return true;
    }
    else
       return true;
}

void QEasyORC::fontorc_levelnode(int ilevel)
{
    int iareasnum = g_pbackfindobject->getresultobjsnum();
    m_resultstrlist.clear();
    m_resultstring.clear();
    if(iareasnum<=1)
      return;
    m_ilevle = ilevel;

    m_dvalue = 0;
    m_dmaxvalue = 0;
    if(0==ilevel)
    {
        resultnodereset(iareasnum);
    }
    //double dvalue = 0;
    //double dimagevalue = 0;
    //double dmaxvalue = 0;
    //int iselmaxnum = 0;
    int imatchobjectw = 0;
    int imatchobjectb = 0;

    for(int ia=0;ia<iareasnum;ia++)
    {
           if(-1==m_idebugrectsnum
             ||ia==m_idebugrectsnum)
           {
               int isize = resultnodesize(ilevel,ia);

               QRect arect = g_pbackfindobject->getgridex(ia);//getgrid(ia);

               int iobjw = g_pbackfindobject->getresultw(ia);
               int iobjh = g_pbackfindobject->getresulth(ia);
               int imaxlen = iobjw>iobjh?iobjw:iobjh;
               int igrid_org = fastmatch::GetRectGridLevel(imaxlen);


               int ix = arect.x();
               int iy = arect.y();
               int iw = arect.width();
               int ih = arect.height();

               if(ix<0||iy<0||iw<=0||ih<=0)
                   continue;
               //fastmatch::setmatchrect(ix,iy,iw,ih);


               switch (ilevel)
               {
                   case 0:
                      fastmatch::setmatchrect(ix,iy,iw,ih);
                       g_pbackobjectimage->SetROI(ix,iy,iw,ih);
                       g_pbackimage->SetROI(ix,iy,iw,ih);
                       g_pbackobjectimage->SetMode(3);
                       g_pbackobjectimage->ROItoROI(g_pbackimage);


                       g_pbackimage->ROIColorTable();
                       g_pbackimage->ROIColorTableBlur(0,-1);
                       g_pbackimage->ROIColorTableEasyThre(1);


                       m_pimagegrid->ROIImagetoModel(*g_pbackimage);
                       m_pimagegrid->ZeroModel();
                       m_pimagegrid->ReGrid(igrid_org,igrid_org);

                       if(0)//test
                       {
                           m_pimagegrid->SetUnit(igrid_org,igrid_org);
                           //48 -> 12
                           m_pimagegrid->UnitGrid();
                       }
                       m_pimagegrid->GridZoom(3,3);
                       m_pimagegrid->SetUnit(3,3);
                       //12 ->3

                       break;
                   case 1:
                       fastmatch::setmatchrect(ix,iy,iw,ih);
                       g_pbackimage->SetROI(ix,iy,iw,ih);
                       m_pimagegrid->ROIImagetoModel(*g_pbackimage);
                       m_pimagegrid->ZeroModel();
                       m_pimagegrid->ReGrid(igrid_org,igrid_org);

                       if(0)//test
                       {
                           m_pimagegrid->SetUnit(igrid_org,igrid_org);
                           //48 -> 12
                           m_pimagegrid->UnitGrid();
                       }
                       m_pimagegrid->GridZoom(6,6);
                       m_pimagegrid->SetUnit(6,6);
                       //12 -> 6
                       break;
               default:
               case 2:
                        if(igrid_org==12)
                           fastmatch::setmatchrect(ix,iy,iw,ih);
                        else
                        {
                            g_pbackimage->SetROI(ix,iy,iw,ih);
                            m_pimagegrid->ROIImagetoModel(*g_pbackimage);

                            m_pimagegrid->ZeroModel();
                            m_pimagegrid->ReGrid(igrid_org,igrid_org);
                            if(0)//test
                            {
                                m_pimagegrid->SetUnit(igrid_org,igrid_org);
                                //48 -> 12
                                m_pimagegrid->UnitGrid();
                            }

                            m_pimagegrid->GridZoom(12,12);
                            m_pimagegrid->SetUnit(12,12);

                        }
                       break;
               case 3:
                       fastmatch::setmatchrect(ix,iy,iw,ih);

                   break;
               }

               m_dmaxvalue = 0;
               m_iselmaxnum = 0;
               m_iresultfont = 0;
               for(int i=0;i<isize;i++)
               {
                    if(-1==m_idebugfontnum)
                        selectresultnode(ilevel,ia,i);
                    else
                        SelectModel(ilevel,m_idebugfontnum);
                   //||m_selectnode.s_inode==m_idebugfontnum)
                   if(1)
                   {
                       if(0==ilevel||1==ilevel)
                       {
                           if(!matchlevelnode01())
                           {
                               setresultnode(ia,m_selectnode);
                               break;
                           }
                       }
                       else if(2==ilevel)
                       {
                           if(1==isize
                             &&-1==m_idebugfontnum)
                           {//only one node
                               m_dvalue = 1;
                               m_dmaxvalue = 99;
                               m_iresultfont = m_selectnode.s_inode;
                               m_imaxnum = m_selectnode.s_inode;
                               break;
                           }
                           else if(igrid_org!=12)
                           {
                               if(!matchlevelnode01())
                               {
                                   setresultnode(ia,m_selectnode);
                                   break;
                               }
                           }
                           else if(!matchlevelnode2())
                                break;

                       }
                   }

                   if(m_selectnode.s_inode==m_idebugfontnum)
                        break;
               }
               if(1==ilevel||0==ilevel)
               {
                   if(m_reslutnodelist[ia].s_inode==-1)
                   {
                       if(m_reslutnodelist[ia].s_ilevel==0)
                       {
                           if(ilevel==1)
                           {
                               m_reslutnodelist[ia].s_inode = -1;
                               m_reslutnodelist[ia].s_ilevel = 1;
                           }
                       }
                       else if(m_reslutnodelist[ia].s_ilevel==1)
                       {
                           if(ilevel==2)
                           {
                               m_reslutnodelist[ia].s_inode = -1;
                               m_reslutnodelist[ia].s_ilevel = 2;
                           }
                       }
                   }
               }
               if(2==ilevel)
                {
                    if(-1==m_idebugrectsnum
                     &&-1==m_idebugfontnum)
                    {

                        m_reslutnodelist[ia].s_inode = m_iresultfont;
                        m_reslutnodelist[ia].s_ilevel = 2;
                        int iresultfont = m_iresultfont;
                        QRegExp rxnum("(\\d+)");
                        QStringList listother = m_fontlist_l12[iresultfont].split(rxnum);

                        QRegExp rxother("(\\D+)");
                        QStringList listnum = m_fontlist_l12[iresultfont].split(rxother);

                        QString strkey = m_fontlist_l12[iresultfont] ;
                       if(listother.size()>0)
                       {
                           if(listother[0]!="")
                               strkey = listother[0];
                           if(listnum.size()<2)
                           {
                               strkey = listnum[0].mid(0,1);
                           }
                       }
                        m_resultstrlist.push_back(strkey);
                        m_resultstring.append(strkey);
                    }
                    else
                    {
                        m_reslutnodelist[ia].s_inode = m_iresultfont;
                        m_reslutnodelist[ia].s_ilevel = 2;
                        int iresultfont = m_iresultfont;
                        QString strkey = m_fontlist_l12[iresultfont] ;
                        m_resultstrlist.push_back(strkey);
                        m_resultstring.append(strkey);
                    }
                }
           }
    }
       if(-1!=m_idebugrectsnum&&-1!=m_idebugfontnum)
       {

           //fastmatch::Match(*g_pbackobjectimage);
           //double matresult = fastmatch::getmaxresult();

           imatchobjectb = fastmatch::geteasyobjectb();
           imatchobjectw = fastmatch::geteasyobjectw();
           QString astr = QString("  %1   %2 b(%3 %5)w(%4 %6)").arg(m_dvalue).arg(m_dmaxvalue).arg(imatchobjectb).arg(imatchobjectw).arg(m_imodelobjectb).arg(m_imodelobjectw);
           QShape::setname(astr.toStdString().c_str());
          // fastmatch::imagemodelcompareshow();
       }
       else if(-1!=m_idebugrectsnum&&-1==m_idebugfontnum)
       {
            if(m_idebugrectsnum>=iareasnum)
                return;
           selectresultnode(ilevel,m_idebugrectsnum,m_iselmaxnum);
         //  fastmatch::Match(*g_pbackobjectimage);
         //  double m_iresultfont = fastmatch::getmaxresult();
           if(0==ilevel||1==ilevel)
               fastmatch::MatchGrid(m_pimagegrid);
           else if(2==ilevel)
           {
               fastmatch::Match(*g_pbackobjectimage);
               m_dvalue = fastmatch::getmaxresult();
               fastmatch::imagematch(-1,1);
           }
           imatchobjectb = fastmatch::geteasyobjectb();
           imatchobjectw = fastmatch::geteasyobjectw();
           //fastmatch::imagemodelcompareshow();
           QString strt = QString(" %1 %2 ").arg(m_dvalue).arg(m_dmaxvalue) + m_resultstring + QString(" %1").arg(m_selectnode.s_inode);
           strt = strt + QString(" b(%3 %5)w(%4 %6) ").arg(imatchobjectb).arg(imatchobjectw).arg(m_imodelobjectb).arg(m_imodelobjectw);
           if(2==ilevel)
               strt = strt + m_fontlist_l12[m_iresultfont] ;
           QShape::setname(strt.toStdString().c_str());
       }
       else
       {
           QString qshowstr;
           int isize = m_reslutnodelist.size();
           for(int it=0;it<isize ;it++)
           {
               QString astrnum = QString("%3_%2(%1)").arg(m_reslutnodelist[it].s_ilevel).arg(m_reslutnodelist[it].s_inode).arg(it);
               qshowstr = qshowstr + astrnum;
           }
           QShape::setname(qshowstr.toStdString().c_str());
       }
}

void QEasyORC::fontorc_levelnodelist()
{
    int iareasnum = g_pbackfindobject->getresultobjsnum();
    m_resultstrlist.clear();
    m_resultstring.clear();
    if(iareasnum<=0
            )
      return;


    m_dvalue = 0;
    m_dmaxvalue = 0;
    resultnodelistreset(iareasnum);

    int imatchobjectw = 0;
    int imatchobjectb = 0;

    for(int ia=0;ia<iareasnum;ia++)
    {
           if(-1==m_idebugrectsnum
             ||ia==m_idebugrectsnum)
           {
               QRect arect = g_pbackfindobject->getgridex(ia);//getgrid(ia);

               int iobjw = g_pbackfindobject->getresultw(ia);
               int iobjh = g_pbackfindobject->getresulth(ia);
               int imaxlen = iobjw>iobjh?iobjw:iobjh;
               int igrid_org = fastmatch::GetRectGridLevel(imaxlen);

               int ix = arect.x();
               int iy = arect.y();
               int iw = arect.width();
               int ih = arect.height();

               if(ix<0||iy<0||iw<=0||ih<=0)
                   continue;

               //3x3
               fastmatch::setmatchrect(ix,iy,iw,ih);
                g_pbackobjectimage->SetROI(ix,iy,iw,ih);
                g_pbackimage->SetROI(ix,iy,iw,ih);
                g_pbackobjectimage->SetMode(3);
                g_pbackobjectimage->ROItoROI(g_pbackimage);

                g_pbackimage->ROIColorTable();
                g_pbackimage->ROIColorTableBlur(0,-1);
                g_pbackimage->ROIColorTableEasyThre(1);

                m_pimagegrid->ROIImagetoModel(*g_pbackimage);
                m_pimagegrid->ZeroModel();
                m_pimagegrid->ReGrid(igrid_org,igrid_org);

                m_pimagegrid->GridZoom(3,3);
                m_pimagegrid->SetUnit(3,3);

                matchlevelnodelist3x3(ia);

               //6x6
                fastmatch::setmatchrect(ix,iy,iw,ih);
                g_pbackimage->SetROI(ix,iy,iw,ih);
                m_pimagegrid->ROIImagetoModel(*g_pbackimage);
                m_pimagegrid->ZeroModel();
                m_pimagegrid->ReGrid(igrid_org,igrid_org);

                m_pimagegrid->GridZoom(6,6);
                m_pimagegrid->SetUnit(6,6);

                matchlevelnodelist6x6(ia);
                //12x12
                fastmatch::setmatchrect(ix,iy,iw,ih);
                if(0)
                {
                    g_pbackimage->SetROI(ix,iy,iw,ih);
                    m_pimagegrid->ROIImagetoModel(*g_pbackimage);

                    m_pimagegrid->ZeroModel();
                    m_pimagegrid->ReGrid(igrid_org,igrid_org);

                    m_pimagegrid->GridZoom(12,12);
                    m_pimagegrid->SetUnit(12,12);
                }
                matchlevelnodelist12x12(ia);
                //36x36


               m_dmaxvalue = 0;
               m_iselmaxnum = 0;
               m_iresultfont = 0;


              //if(2==ilevel)
               if(0)
               {
                    if(-1==m_idebugrectsnum
                     &&-1==m_idebugfontnum)
                    {

                        m_reslutnodelist[ia].s_inode = m_iresultfont;
                        m_reslutnodelist[ia].s_ilevel = 2;
                        int iresultfont = m_iresultfont;
                        QRegExp rxnum("(\\d+)");
                        QStringList listother = m_fontlist_l12[iresultfont].split(rxnum);

                        QRegExp rxother("(\\D+)");
                        QStringList listnum = m_fontlist_l12[iresultfont].split(rxother);

                        QString strkey = m_fontlist_l12[iresultfont] ;
                       if(listother.size()>0)
                       {
                           if(listother[0]!="")
                               strkey = listother[0];
                           if(listnum.size()<2)
                           {
                               strkey = listnum[0].mid(0,1);
                           }
                       }
                        m_resultstrlist.push_back(strkey);
                        m_resultstring.append(strkey);
                    }
                    else
                    {
                        m_reslutnodelist[ia].s_inode = m_iresultfont;
                        m_reslutnodelist[ia].s_ilevel = 2;
                        int iresultfont = m_iresultfont;
                        QString strkey = m_fontlist_l12[iresultfont] ;
                        m_resultstrlist.push_back(strkey);
                        m_resultstring.append(strkey);
                    }
                }
           }
    }
    if(0)
    {
        if(-1!=m_idebugrectsnum&&-1!=m_idebugfontnum)
        {
            //fastmatch::Match(*g_pbackobjectimage);
            //double matresult = fastmatch::getmaxresult();

            imatchobjectb = fastmatch::geteasyobjectb();
            imatchobjectw = fastmatch::geteasyobjectw();
            QString astr = QString("  %1   %2 b(%3 %5)w(%4 %6)").arg(m_dvalue).arg(m_dmaxvalue).arg(imatchobjectb).arg(imatchobjectw).arg(m_imodelobjectb).arg(m_imodelobjectw);
            QShape::setname(astr.toStdString().c_str());
           // fastmatch::imagemodelcompareshow();
        }
        else if(-1!=m_idebugrectsnum&&-1==m_idebugfontnum)
        {
             if(m_idebugrectsnum>=iareasnum)
                 return;
            //selectresultnode(ilevel,m_idebugrectsnum,m_iselmaxnum);
            //   if(0==ilevel||1==ilevel)
                fastmatch::MatchGrid(m_pimagegrid);
            //else if(2==ilevel)
            {
                fastmatch::Match(*g_pbackobjectimage);
                m_dvalue = fastmatch::getmaxresult();
                fastmatch::imagematch(-1,1);
            }
            imatchobjectb = fastmatch::geteasyobjectb();
            imatchobjectw = fastmatch::geteasyobjectw();
            //fastmatch::imagemodelcompareshow();
            QString strt = QString(" %1 %2 ").arg(m_dvalue).arg(m_dmaxvalue) + m_resultstring + QString(" %1").arg(m_selectnode.s_inode);
            strt = strt + QString(" b(%3 %5)w(%4 %6) ").arg(imatchobjectb).arg(imatchobjectw).arg(m_imodelobjectb).arg(m_imodelobjectw);
            //if(2==ilevel)
                strt = strt + m_fontlist_l12[m_iresultfont] ;
            QShape::setname(strt.toStdString().c_str());
        }
        else
        {
            QString qshowstr;
            int isize = m_reslutnodelist.size();
            for(int it=0;it<isize ;it++)
            {
                QString astrnum = QString("%3_%2(%1)").arg(m_reslutnodelist[it].s_ilevel).arg(m_reslutnodelist[it].s_inode).arg(it);
                qshowstr = qshowstr + astrnum;
            }
            QShape::setname(qshowstr.toStdString().c_str());
        }
    }

}

void QEasyORC::shownoderesult()
{
    QString qshowstr;
    int isize = m_reslutnodelist.size();
    QString strpatA("");
    QString strpatB("");

    QString strpata("");
    QString strpatb("");
    QString strpatc("");

    for(int i=0;i<isize;i++)
    {
        int iresultfont = m_reslutnodelist[i].s_inode;
        if(iresultfont<0)
            continue;

        //


        //
        QRegExp rxnum("(\\d+)");

        QStringList listother = m_fontlist_l12[iresultfont].split(rxnum);

        QRegExp rxother("(\\D+)");
        QStringList listnum = m_fontlist_l12[iresultfont].split(rxother);
        QString strkey = m_fontlist_l12[iresultfont];
       if(listother.size()>0)
       {
           if(listother[0]!="")
               strkey = listother[0];
           if(listnum.size()<2)
           {
               strkey = listnum[0].mid(0,1);
           }
           // ZH* *ZH ->ZH
           QStringList strpat = strkey.split("*");
           if(strpat.size()>1)
           {
               if((!strpat[0].isEmpty())||(!strpat[1].isEmpty()))
               {
                   if(strpatA.isEmpty())
                   {
                       strpatA = strpat[0];
                       strkey =  QString("");
                   }
                   else if(!strpatA.isEmpty())
                   {
                       strpatB = strpat[1];
                       {
                           //cn comb list 0 1 2.....
                           //swithc() case :
                           strkey = strpatB;
                       }
                       strpatA = QString("");
                       strpatB = QString("");

                   }
               }
           }
           // ZH| |ZH| |ZH ->ZH
           strpat = strkey.split("|");
           if(strpat.size()>1)
           {
               if((!strpat[0].isEmpty())||(!strpat[1].isEmpty()))
               {
                   if(strpata.isEmpty())
                   {
                       strpata = strpat[0];
                       strkey =  QString("");
                   }
                   else if((!strpata.isEmpty())&&strpatb.isEmpty())
                   {
                       strpatb = strpat[1];
                       strkey =  QString("");
                   }
                   else if((!strpata.isEmpty())&&(!strpatb.isEmpty())&&(strpatc.isEmpty()))
                   {
                       strpatc = strpat[1];
                       {
                           //cn comb list 0 1 2.....
                           //swithc() case :
                           strkey = strpatc;
                       }
                       strpata = QString("");
                       strpatb = QString("");
                       strpatc = QString("");

                   }
               }
           }

       }
       qshowstr = qshowstr + strkey;
    }

    QShape::setname(qshowstr.toStdString().c_str());
}
//
//chinese comb
void QEasyORC::shownoderesultex()
{
    QString qshowstr;
    int isize = m_reslutnodelist.size();
    QString strpatA("");
    QString strpatB("");

    QString strpata("");
    QString strpatb("");
    QString strpatc("");

    QStringList strlista;
    QStringList strlistb;
    QStringList strlistc;

    int il12size = getduplicateslist_l12().size();

    for(int i=0;i<isize;i++)
    {
        int iresultfont = m_reslutnodelist[i].s_inode;
        if(iresultfont<0)
            continue;
        //
        int igetvalue = getduplicateslist_l12()[iresultfont];
        if(igetvalue!=0)
        {
            strlista.clear();
            for(int j=0;j<il12size;j++)
            {
               if(igetvalue == getduplicateslist_l12()[j])
               {
                   QString strget = ABC2string(m_fontlist_l12[j]);
                   if(!strget.isEmpty())
                       strlista.push_back(strget);//.push_back(m_fontlist_l12[j]);
               }
            }
        }

        QRegExp rxnum("(\\d+)");
        QStringList listother = m_fontlist_l12[iresultfont].split(rxnum);

        QRegExp rxother("(\\D+)");
        QStringList listnum = m_fontlist_l12[iresultfont].split(rxother);
        QString strkey = m_fontlist_l12[iresultfont];
        if(listother.size()>0)
        {
               if(listother[0]!="")
                   strkey = listother[0];
               if(listnum.size()<2)
               {
                   strkey = listnum[0].mid(0,1);
               }
               // ZH* *ZH ->ZH
               QStringList strpat = strkey.split("*");
               if(strpat.size()>1)
               {
                   if((!strpat[0].isEmpty())||(!strpat[1].isEmpty()))
                   {
                       if(strpatA.isEmpty())
                       {
                           strpatA = strpat[0];
                           strkey =  QString("");
                       }
                       else if(!strpatA.isEmpty())
                       {
                           strpatB = strpat[1];
                           {
                               //cn comb list 0 1 2.....
                               //swithc() case :
                               if(strlista.size()!=0&&strlistb.size()==0)
                               {
                                   strkey = strpatA;
                               }
                               else if(strlista.size()==0&&strlistb.size()!=0)
                               {
                                   strkey = strpatB;
                               }
                               else if(strlista.size()!=0&&strlistb.size()!=0)
                               {
                                   for(int ia=0;ia<strlista.size();ia++)
                                   {
                                       for(int ib=0;ib<strlistb.size();ib++)
                                       {
                                           if(strlista[ia]==strlistb[ib])
                                           {
                                               strkey = strlista[ia];
                                               goto ENDIAIBLOOP;
                                           }
                                       }
                                   }
ENDIAIBLOOP:
                                strlista.clear();
                                strlistb.clear();

                               }
                               else
                                   strkey = strpatB;
                           }
                           strpatA = QString("");
                           strpatB = QString("");
                           strlista.clear();
                           strlistb.clear();
                       }
                   }
               }
               // ZH| |ZH| |ZH ->ZH
               strpat = strkey.split("|");
               if(strpat.size()>1)
               {
                   if((!strpat[0].isEmpty())||(!strpat[1].isEmpty()))
                   {
                       if(strpata.isEmpty())
                       {
                           strpata = strpat[0];
                           strkey =  QString("");
                       }
                       else if((!strpata.isEmpty())&&strpatb.isEmpty())
                       {
                           strpatb = strpat[1];
                           strkey =  QString("");
                       }
                       else if((!strpata.isEmpty())&&(!strpatb.isEmpty())&&(strpatc.isEmpty()))
                       {
                           strpatc = strpat[1];
                           {
                               //cn comb list 0 1 2.....
                               //swithc() case :
                               strkey = strpatc;
                           }
                           strpata = QString("");
                           strpatb = QString("");
                           strpatc = QString("");
                           strlista.clear();
                           strlistb.clear();
                           strlistc.clear();
                       }
                   }
               }
        }
        qshowstr = qshowstr + strkey;

        strlistb = strlista;
        strlista.clear();

        //
    }

    m_qocrstring = qshowstr;
    QShape::setname(qshowstr.toStdString().c_str());
}
//
//chinese comb
void QEasyORC::shownodelistresult12x12()
{
    //

    //
    QString qshowstr;
    int isize = m_reslutnodeslistgrid12x12.size();
    QString strpatA("");
    QString strpatB("");

    QString strpata("");
    QString strpatb("");
    QString strpatc("");

    QStringList strlista;
    QStringList strlistb;
    QStringList strlistc;



    int il12size = getduplicateslist_l12().size();

    for(int i=0;i<isize;i++)
    {
        if(m_idebugrectsnum==-1
          ||m_idebugrectsnum==i)
        {
            int inodesize = m_reslutnodeslistgrid12x12[i].getnodes().size();
            if(inodesize>0)
            {
                int iresultfont   = m_reslutnodeslistgrid12x12[i].getnodes()[0].s_inode;
                if(iresultfont<0)
                    continue;
                //
                int ilistsize =getduplicateslist_l12().size();
                if(ilistsize>iresultfont)
                {

                    int igetvalue = getduplicateslist_l12()[iresultfont];
                    if(igetvalue!=0)
                    {
                        strlista.clear();
                        for(int j=0;j<il12size;j++)
                        {
                           if(igetvalue == getduplicateslist_l12()[j])
                           {
                               QString strget = ABC2string(m_fontlist_l12[j]);
                               if(!strget.isEmpty())
                                   strlista.push_back(strget);//.push_back(m_fontlist_l12[j]);
                           }
                        }
                    }

                    QRegExp rxnum("(\\d+)");
                    QStringList listother = m_fontlist_l12[iresultfont].split(rxnum);

                    QRegExp rxother("(\\D+)");
                    QStringList listnum = m_fontlist_l12[iresultfont].split(rxother);
                    QString strkey = m_fontlist_l12[iresultfont];
                    if(listother.size()>0)
                    {
                           if(listother[0]!="")
                               strkey = listother[0];
                           if(listnum.size()<2)
                           {
                               strkey = listnum[0].mid(0,1);
                           }
                           // ZH* *ZH ->ZH
                           QStringList strpat = strkey.split("*");
                           if(strpat.size()>1)
                           {
                               if((!strpat[0].isEmpty())||(!strpat[1].isEmpty()))
                               {
                                   if(strpatA.isEmpty())
                                   {
                                       strpatA = strpat[0];
                                       strkey =  QString("");
                                   }
                                   else if(!strpatA.isEmpty())
                                   {
                                       strpatB = strpat[1];
                                       {
                                           //cn comb list 0 1 2.....
                                           //swithc() case :
                                           if(strlista.size()!=0&&strlistb.size()==0)
                                           {
                                               strkey = strpatA;
                                           }
                                           else if(strlista.size()==0&&strlistb.size()!=0)
                                           {
                                               strkey = strpatB;
                                           }
                                           else if(strlista.size()!=0&&strlistb.size()!=0)
                                           {
                                               for(int ia=0;ia<strlista.size();ia++)
                                               {
                                                   for(int ib=0;ib<strlistb.size();ib++)
                                                   {
                                                       if(strlista[ia]==strlistb[ib])
                                                       {
                                                           strkey = strlista[ia];
                                                           goto ENDIAIBLOOP;
                                                       }
                                                   }
                                               }
            ENDIAIBLOOP:
                                            strlista.clear();
                                            strlistb.clear();

                                           }
                                           else
                                               strkey = strpatB;
                                       }
                                       strpatA = QString("");
                                       strpatB = QString("");
                                       strlista.clear();
                                       strlistb.clear();
                                   }
                               }
                           }
                           // ZH| |ZH| |ZH ->ZH
                           strpat = strkey.split("|");
                           if(strpat.size()>1)
                           {
                               if((!strpat[0].isEmpty())||(!strpat[1].isEmpty()))
                               {
                                   if(strpata.isEmpty())
                                   {
                                       strpata = strpat[0];
                                       strkey =  QString("");
                                   }
                                   else if((!strpata.isEmpty())&&strpatb.isEmpty())
                                   {
                                       strpatb = strpat[1];
                                       strkey =  QString("");
                                   }
                                   else if((!strpata.isEmpty())&&(!strpatb.isEmpty())&&(strpatc.isEmpty()))
                                   {
                                       strpatc = strpat[1];
                                       {
                                           //cn comb list 0 1 2.....
                                           //swithc() case :
                                           strkey = strpatc;
                                       }
                                       strpata = QString("");
                                       strpatb = QString("");
                                       strpatc = QString("");
                                       strlista.clear();
                                       strlistb.clear();
                                       strlistc.clear();
                                   }
                               }
                           }
                    }
                    qshowstr = qshowstr + strkey;

                    strlistb = strlista;
                    strlista.clear();
                }
                else
                {
                    qshowstr = qshowstr + QString("?");
                }

            }
            else
            {
                qshowstr = qshowstr + QString("?");

            }
        }

        //
    }

    m_qocrstring = qshowstr;
    QShape::setname(qshowstr.toStdString().c_str());
}

QString QEasyORC::ABC2string(QString strget)
{
    QString strpatA;
    QString strpatB;

    QString strpata;
    QString strpatb;
    QString strpatc;

    QRegExp rxnum("(\\d+)");
    QStringList listother = strget.split(rxnum);

    QRegExp rxother("(\\D+)");
    QStringList listnum = strget.split(rxother);
    QString strkey = strget;

   if(listother.size()>0)
   {
       if(listother[0]!="")
           strkey = listother[0];
       if(listnum.size()<2)
       {
           strkey = listnum[0].mid(0,1);
       }
       // ZH* *ZH ->ZH
       QStringList strpat = strkey.split("*");
       if(strpat.size()>1)
       {
                if(!strpat[0].isEmpty())
                {
                    return strpat[0];
                }
                else if(!strpat[1].isEmpty())
                {
                    return strpat[1];
                }

       }
       // ZH| |ZH| |ZH ->ZH
       strpat = strkey.split("|");
       if(strpat.size()>1)
       {
           if(!strpat[0].isEmpty())
           {
               return strpat[0];
           }
           else if(!strpat[1].isEmpty())
           {
               return strpat[1];
           }
       }

   }
   return QString("");

}
QString QEasyORC::getreslultstring()
{
    return m_qocrstring;
}
void QEasyORC::stringresulthead(const char*pchar)
{
   m_qocrstring = QString(pchar) + m_qocrstring;
}
void QEasyORC::stringresulttail(const char*pchar)
{
   m_qocrstring =  m_qocrstring + QString(pchar);
}

void QEasyORC::clipboardresult()
{//QApplication::clipboard()
    QApplication::clipboard()->setText(m_qocrstring);
}

void QEasyORC::setminscore(double dminscore)
{
    fastmatch::setminscore(dminscore);
}
void QEasyORC::shapesetroi(void *pshape)
{
    QShape::shapesetroi(pshape);
}
