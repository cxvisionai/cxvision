
#ifndef STRINGAI_H
#define STRINGAI_H

#include <QMap>

class StringAI
{

    QStringList m_keywordfilterlist;
    QStringList m_filefilterlist;
    QStringList m_strneedstringlist;

    QMap<QString, QStringList> m_searchmap;

    QMap<QString,int> m_urlmap;// <url,level>
    QStringList m_searchedkeys;// <url,level>

    QMultiMap<QString,int> m_urlmap_seed;// <url,level>

    QMultiMap<QString,int> m_urlmap_tendcy;// <url,level>

    QStringList m_corelist;

    QStringList m_freqlists;
    QStringList m_strkey;

    QStringList m_strtemplist;

    int m_ibufsize;
    int m_isearchlevel;
public:
    StringAI();
    ~StringAI();
    void clearstring();
    void addstring(const char *pchar);
    void addocrstring(void *pocr);
    QString getstring(int inum);

    void addsearchkey(const char *pkeychar);
    void clearkeys();
    void updateurltree(const char *purl);

    void addrelation(const char *pkeyandlabel);
    void addurl(const char *pkeyandlabel);

    void setrelationlabel(const char *pkeyandlabel);
    void seturllabel(const char *pkeyandlabel);

    void setsearchlabel(const QString &qkeystr,const QString &qstr);
    void setsearchurl(const QString &qkeystr,const QString &qstrurl);


    void update();
    QString runstep();
    void clear();



    void addneedstringlist(const char *str);
    void addextfilter(const char *str);
    void addkeywordfilter(const char *str);
    void clearfilter();

    void relationkeyscan(const QString &strkey,int ilevel,int iendlevel);

    void makecorelist();
    QStringList prelist();
    void runfilter();



    QString getlocationstring(const QString &strfile);
    void urltreemkdir(const QString &astr);
    void saverelationcreate(const QString &qkeystr,const QString &qstr);
    void saveurltreecreate(const QString &strurl);
    void saveurltreecreate(const QString &qkeystr,const QString &strurl);

    void saveurltreelistcreate(const QStringList &strurl);

    void loadurltreefiletonode(const QString &qstr);
    void loadrelationtreefiletonode(const QString &qstr);


    QStringList parserstring_what(QString qstr);
    QStringList parserstring_who(QString qstr);
    QStringList parserstring_why(QString qstr);
    QStringList parserstring_where(QString qstr);
    QStringList parserstring_when(QString qstr);


    QStringList parserstring_same(QString qstr);
    QStringList parserstring_how(QString qstr);

    //string split who what when where why how
    void parsersting_easy(QString qstr);
    //
    QString cxvisionaiparserstring(QString qstrwho,QString qstrwhat,QString qstrwhen=QString(""),QString qstrwhere=QString(""),QString qstrwhy=QString(""));

};


#endif
