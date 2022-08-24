#ifndef QEASYORC_H
#define QEASYORC_H

#include <QStringList>
#include "fastmatch.h"


class findobject;

typedef struct levelnode
{
    int s_ilevel;
    int s_inode;
}levelnode;
typedef struct levelvalenode
{
    int s_ilevel;
    int s_inode;
    double s_dvalue;
}levelvaluenode;

class levelnodes
{
    QList<levelvaluenode> s_nodes;
    int m_searchsum;
public:
    void setsearchnum(int inum)
    {
        m_searchsum = inum;
    }
    void addnode(levelvaluenode &node)
    {
        int isize = s_nodes.size();
        if(isize<m_searchsum)
        {
            for(int i=0;i<isize;i++)
            {
                if(node.s_dvalue>s_nodes[i].s_dvalue)
                {
                    s_nodes.insert(i,node);
                    return;
                }
            }
            s_nodes.insert(isize,node);
        }
        else
        {
            for(int i=0;i<isize;i++)
            {
                if(node.s_dvalue>s_nodes[i].s_dvalue)
                {
                    s_nodes.insert(i,node);
                    s_nodes.removeAt(s_nodes.size()-1);
                    return;
                }
            }
            s_nodes.insert(isize,node);
            s_nodes.removeAt(s_nodes.size()-1);
         }
    }
    QList<levelvaluenode> &getnodes()
    {
        return s_nodes;
    }
};

class QEasyORC: public fastmatch
{
public:
    QEasyORC();
    ~QEasyORC();
    void setshow(int ishow);
    virtual void setrect(int ix,int iy,int iw,int ih);
    virtual void drawshape(QPainter &painter,QPalette &pal);
    void setorcareasnum(int inum);
    void setorcareas(int inum,int ix,int iy,int iw,int ih);
    void setorcthre(int ithre);
    void setb2w(int ib2w);
    void setspecshow(int ishow);
    void imagemodelshow();
    void imagematchshow();
    void imagecompareshow(int itype);
    void setshowpos(int ix,int iy);
    void modelmethod(int itype);

    void match72_matchimg();
    void match72_matchpat();

	void setgrid(int iw,int igrid);

    void levelmodel();
    void savelevelmodel();
    void SelectModel(int ilevle,int inum);

    int imagefastmapsize(int ilevel,int inum);
    int SelectMapModel(int ilevel,int inum,int i0);

    void selectmodel72(const char *pfilename);

    void SelectNameModel(int ilevel,const char *pfilename);

    void fontorc();
    void fontorc_level(int ilevel);
    void fontorc_levelex(int ilevel);
    void fontorc_level2();
    void checkorc_level3();

    void fontsplit(void *pimage);
    void exfontsplit(void *pimage);
    void stringsplit(void *pimage);
    void areasorc(void *pimage);
    void setdebug(int idebugrect,int idebugfont);
    void setsplitimage(int ithre,int ixor,int iyor,int ixand,int iyand);
    void setsplitobject(int idistance,int isearchtype,int ibrow,int iminarea,int ibgedge);
    void setsplitgrid(int iw,int ih,int igridnum);
    void setsplitobjectoffset(int ix0,int ix1,int iy0,int iy1);
    void setsplitobjectbg(int ibgedge,int ibgmethod);

    void autolearn(const char *pfilename);
    void autolearnex(const char *pfilename);
    void autolearnobj(const char *pfilename );
    void setlearngridwh(int igridwh);
    void autolearnmass(const char *pfilename );

    void learnmass_36(const char *pfilename );
    void checklearn(const char *pfilename );
    void checkmatch(const char *pfilename );

    void setimagetype(int itype);

	void string_exnum(int inum);
	void string_autolearnmass(const char *pstring);

    void setmatchvalid(double dthre);
    void setusingobject(int iusing);

    void match72();

    bool matchlevelnode01();
    bool matchlevelnode2();
    void fontorc_levelnode(int ilevel);

    int resultnodesize(int ilevel,int iareanum);
    void selectresultnode(int ilevel,int iareanum,int inum0);
    void resultnodereset(int iareasnum);
    void setresultnode(int inum,levelnode anode);

    bool matchlevelnodelist3x3(int ia);
    bool matchlevelnodelist6x6(int ia);
    bool matchlevelnodelist12x12(int ia);
    bool matchlevelnodelist36x36(int ia);
    bool matchlevelnodelist72x72(int ia);
    void fontorc_levelnodelist();

    //int resultnodelistsize(int ilevel,int iareanum);
    //void selectresultnodelist(int ilevel,int iareanum,int inum0);


    void resultnodelistreset(int iareasnum);
    void setresultnodelist(int ilevel,int iareanum,levelnode anode);

    QString char2string(QString pchar);
    QString string2char(const QString &strchar);

    QString filenametoORCstring(QString strbase);

    QString getreslultstring();
    void clipboardresult();
    QString ABC2string(QString strget);
    void setlevelstring();
    void setshowmap(int ishow,int ilevel,int idebugfont);
    void mapclear();
    void mapgrid();
    void shownoderesult();
    void shownoderesultex();
    void shownodelistresult12x12();
    void setminscore(double dminscore);
    void stringresulthead(const char*pchar);
    void stringresulttail(const char*pchar);
    void shapesetroi(void *pshape);
private:
    int m_igridw;
    int m_igridh;
    void loadfontmodel();

    double m_dmatchthre;
    int m_icompareobject;

    QStringList m_fontlist_l12;
    QStringList m_filenamelist_l12;

    QStringList m_fontlist_l36;
    QStringList m_filenamelist_l36;

    QStringList m_fontlist_l72;
    QStringList m_filenamelist_l72;

    QStringList m_resultstrlist;
    QString m_resultstring;

    QList<int> m_l3resultlist;
    QList<int> m_l6resultlist;
	QList<int> m_l12resultlist;
	QList<int> m_l36resultlist;

    QList<levelnode> m_reslutnodelist;

    QList<levelnodes> m_reslutnodeslistgrid3x3;
    QList<levelnodes> m_reslutnodeslistgrid6x6;
    QList<levelnodes> m_reslutnodeslistgrid12x12;
    QList<levelnodes> m_reslutnodeslistgrid36x36;
    QList<levelnodes> m_reslutnodeslistgrid72x72;

    int m_resultnodesearchsum;

    int m_idebugrectsnum;
    int m_idebugfontnum;
    void AreasORC(ImageBase &image);
    void FontSplit(ImageBase &image);
    void ExFontSplit(ImageBase &image);
    void StringSplit(ImageBase &image);

    ImageBase * g_pbackimage;
    ImageBase * g_pbackobjectimage;
    findobject * g_pbackfindobject;
    int m_image_thre;
    int m_findobj_distance;
    int m_findobj_searchtype;
    int m_findobj_brow;
    int m_findobj_minarea;
    int m_findobj_maxarea;
    int m_findobj_minw;
    int m_findobj_maxw;
    int m_findobj_minh;
    int m_findobj_maxh;
    int m_findobj_bgedge;
    int m_findobj_bgmethod;

    int m_findobj_ioffsetx0;
    int m_findobj_ioffsetx1;
    int m_findobj_ioffsety0;
    int m_findobj_ioffsety1;

    int m_ix_or;
    int m_iy_or;
    int m_ix_and;
    int m_iy_and;

    int m_idraw_x;
    int m_idraw_y;

    int m_imodelobjectw;
    int m_imodelobjectb;

    int m_idraw_map;
    int m_ilevle;

    int m_imagetype;

    QList<QGrid*> m_pgrids_l72;
    QList<QGrid*> m_pgrids_l36;
    QList<QGrid*> m_pgrids_l12;
    QList<QGrid*> m_pgrids_l6;
    QList<QGrid*> m_pgrids_l3;
	QGrid *m_pimagegrid;

    double m_dvalue;
    double m_dmaxvalue;
    double m_iresultfont;
    double m_imaxnum;
    double m_iselmaxnum;
    levelnode m_selectnode;

    int m_igridwh;
    int m_exnum;

    QString m_qocrstring;
};








#endif
