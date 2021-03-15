
#ifndef STRINGAI_H
#define STRINGAI_H

#include <QMap>

class StringAI
{
    QStringList m_corelist;

    QStringList m_keywordfilterlist;
    QStringList m_filefilterlist;
    QStringList m_strneedstringlist;
public:
    QString getlocationstring(const QString &strfile);
    void urltreemkdir(const QString &astr);
    void urltreecreate(const QString &strurl);
    void urltreelistcreate(const QStringList &strurl);
    QStringList loadurltreefiletobuff(const QString &str);
    QStringList loadrelationtreefiletobuff(const QString &qstr);

    void addneedstringlist(const QString &str);
    void addextfilter(const QString &str);
    void addkeywordfilter(const QString &str);
    void clearfilter();

    QString runstep();
    QStringList prelist();


    QMultiMap<QString, QString> m_relationmap;


    void updaterelationlist();
};


#endif
