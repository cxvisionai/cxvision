#ifndef FASTMATCH_H
#define FASTMATCH_H
#include <map>
#include <vector>
#include <QMap>
#include "shape.h"

#include "shapebase.h"
#include "imagebase.h"
#include "findline.h"
#include "grid.h"


class QGrid;
typedef QList<int> QCluster;
using namespace std;
class fastmatch:public findline
{
public:
    fastmatch();
    ~fastmatch();

    void setshow(int ishow);

    virtual void setrect(int ix,int iy,int iw,int ih);
    virtual void drawshape(QPainter &painter,QPalette &pal);

    void drawshapex(QPainter &painter,QPalette &pal,double dmovx,double dmovy,
                    double dangle,double dzoomx,double dzoomy);

    //findline
    void SetWHgap(int wgap=2,int hgap=2);
    void measure(void *pimage);
    void setlinesamplerate(double dsamplerate);
    void setlinegap(int igap);
    void setmethod(int imethod);
    void setthre(int ithre);
    void setgamarate(int igama);
    void setfindsetting(int ifindset);
    void setfilter(int ifilterborw,int ifiltermin,int ifiltermax);//21 w ,22 b
    void setselectedgenum(int iedgenum);

    //match
    void learn(void *pimage);
    void setcomparegap(int igap);
    void savemodelfile(const char * pchar);
    void loadmodelfile(const char * pchar);
    void loadrotatemodelfile(const char * pchar);

    void setrotateangle(double danglel1);

    void patternrootgrid(double itype,double drate,double ilevel);

    void patternzoom(double dx,double dy,double igap,double itype);

    void patterntranform(int igap,int itype,int isgap,int iline);
    void modelrotate(double dangle);
    void modelzoom(double dx,double dy);
    void setmodelwh(int iw,int ih);
    void modelzeroposition();
    void rotatemodelzeroposition();
    void Match(ImageBase &image);
    void match(void *pimage);
    void MultiMatch(ImageBase &image);
    void multimatch(void *pimage);



    void rotatematch(void *pimage);
    void setclustergap(int ixclustergap,int iyclustergap,int iangleclustergap);

    void savematchroi(const char *pfilename);

    void imagelearn(int ithre1,int iandor);
    void imagelearnex(int ithre1,int iandor,int igrid);
    void imagelearnmass(int ithre1,int iandor,int igridwh);
    void imagelearncheck(int iimagetype,int iandor,int igridwh);

    void imagematch(int ithre1,int iandor,int igrid = 12,int ineedthre = 1);
    void imagematchex( int igrid);
    void savematchimagemodel(const char *pfilename);

    void loadfastimagemodel(const char *pfilename);
    void savefastimagemodel(const char *pfilename);
    void savefastimagepatmodel(const char *pfilename);

    void SaveMatchROI(ImageBase &image,const char *pfilename);
    void MatchImageLearn(ImageBase &aimage,int ithre1,int iandor);
    void MatchImageLearnEx(ImageBase &aimage,int ithre1,int iandor,int igrid);
    void MatchImageLearnMass(ImageBase &aimage,int ithre1,int iandor,int igrid);
    void MatchImageCheck(ImageBase &aimage,int iimagetype,int iandor,int igrid);

    void imagematch_grid(int ithre1,int iandor,int igrid);

    void MatchImageMatch(ImageBase &aimage,int ithre1,int iandor,int igrid=12,int ineedthre=1);
    void MatchImageExMatch(ImageBase &aimage ,int igrid=12);
    void MatchGrid(QGrid *pgrid);

    void samplemodel(int inum);// 1/inum
    void setmatchrectnum(int inum);
    void setmatchrect(int ix,int iy,int iw,int ih);

    void setmultimatchrect(int inum,int ix,int iy,int iw,int ih);
    void setmatchthre(int ithre);
    void setfindnum(int ifindnum);
    double getresultnum(int inum);
    int getresultcentx(int inum);
    int getresultcenty(int inum);

    void setminscore(double dscore);

    double getmaxresult();
    double getimagemodelreslut();
    double getimagemodelreslut_check_1();

    int getmodeleasyobjectw_l72(int inum);
    int getmodeleasyobjectb_l72(int inum);

    int getmodeleasyobjectw_l36(int inum);
    int getmodeleasyobjectb_l36(int inum);

    int getmodeleasyobjectw_l12(int inum);
    int getmodeleasyobjectb_l12(int inum);

    int getmodeleasyobjectw_l6(int inum);
    int getmodeleasyobjectb_l6(int inum);

    int getmodeleasyobjectw_l3(int inum);
    int getmodeleasyobjectb_l3(int inum);

    int geteasyobjectb();
    int geteasyobjectw();

    void imagemodelcompareshow(int itype);
    void imagemodelcomparegrid(int itype);

    double imagegridresult(int itype);

    void imagemodelshow();
    void imagematchshow();

    void clearmodels_l12();
    void addmodels_l12(const char * pchar);

    void clearmodels_l36();
    void addmodels_l36(const char * pchar);

    void clearmodels_l72();
    void addmodels_l72(const char * pchar);

    void modelstocurrent_l72(int i);
    void modelstocurrent_l36(int i);
    void modelstocurrent_l12(int i);
    void modelstocurrent_l3(int i);
    void modelstocurrent_l6(int i);

    void imagemodesclear_l12();
    void addimagemodels_l12(const char *pchar);

    void imagemodesclear_l36();
    void addimagemodels_l36(const char *pchar);

    void imagemodesclear_l72();
    void addimagemodels_l72(const char *pchar);

    void clearmodels_rotate();
    void addmodels_rotate(const char * pchar);

    int GetRectGridLevel(int irectw);
    QVector<int> *getcurimagemodel();
    bool modelcompare(QVector<int> &modela, QVector<int> &modelb);

    void clearmodel();
    void list_duplicatesmodel_l12();
    void list_duplicatesmodel_l36();
    void list_duplicatesmodel_l72();

    void levelmodels_l72tol36();
    void levelmodels_l36tol12();
    void levelmodels_l12tol6();
    void levelmodels_l6tol3();

    int imagefastmodelsize(int ilevel);
    void imagemodelstocurrent_l72(int i);
    void imagemodelstocurrent_l36(int i);
    void imagemodelstocurrent_l12(int i);
    void imagemodelstocurrent_l3(int i);
    void imagemodelstocurrent_l6(int i);

    void objectmodelstocurrent(int i);

    void setcurmodels(int inum);
    void setcurimagemodels(int inum);

    void setspecshow(int ishow);
    void setb2w(int ib2w);
    void modelmethod(int itype);
    RectsShape &getmatchrects();
    QRect &getmatchrect();
    QRect getresultrect(int inum);
    QVector<PointsShape> &getmodels_l12();
    QGrid * getgrid();
    void setgrid(int iw,int igrid);
    QMap<int ,int > &getlevel3_6map();
    QMap<int ,int > &getlevel6_12map();
    QMap<int ,int > &getlevel12_36map();
    QMap<int ,int > &getlevel36_72map();

    QVector<int> &getduplicateslist_l72();
    QVector<int> &getduplicateslist_l36();
    QVector<int> &getduplicateslist_l12();
    void savelevel0_l1();
private:
    int m_istyle;
    ImageBase * g_pmodelimage;
    ImageBase * m_matchimage;


    QVector<int> m_imagefastmodel;

    QVector<PointsShape> m_models_l72;

    QVector<PointsShape> m_models_l36;

    QVector<PointsShape> m_models_l12;

    QVector<PointsShape> m_models_l3;

    QVector<PointsShape> m_models_l6;

typedef QVector<int> IMAGEFASTMODEL;
    QVector<IMAGEFASTMODEL> m_imagefastmodels_l72;

    QVector<IMAGEFASTMODEL> m_imagefastmodels_l36;

    QVector<IMAGEFASTMODEL> m_imagefastmodels_l12;

    QVector<IMAGEFASTMODEL> m_imagefastmodels_l6;

    QVector<IMAGEFASTMODEL> m_imagefastmodels_l3;


    QVector<int>m_duplicates_list_l72;
    QVector<int>m_duplicates_list_l36;
    QVector<int>m_duplicates_list_l12;

    // QVector<IMAGEFASTMODEL> m_imagefastmodels_l12_l2;

    QMap<int ,int > m_mapl3_l6;
    QMap<int ,int > m_mapl6_l12;
    QMap<int ,int > m_mapl12_l36;
    QMap<int ,int > m_mapl36_l72;

    QVector<easyobject> m_easyobjectmodels_l72;

    QVector<easyobject> m_easyobjectmodels_l36;

    QVector<easyobject> m_easyobjectmodels_l12;

    QVector<easyobject> m_easyobjectmodels_l6;

    QVector<easyobject> m_easyobjectmodels_l3;

    QVector<int> m_imagefastmatchlist;

    int m_imodelwith;
    int m_imodelheigh;

    easyobject m_cureasyobject;

    easyobject m_easyobject;

    int m_imagemodelresult_NG;
    int m_imagemodelresult_OK;

    int m_easyobjectw_ng;

    int m_easyobjectb_ng;

    PointsShape m_modelpoints_sample1;//  /2
    PointsShape m_modelpoints_sample2;//  /4
    PointsShape m_modelpoints_sample3;//  /8

    int m_imaxmatchnum;
    int m_imatchthre;
    int m_iB2W;
    int m_imatchoffset;

    RectsShape m_matchrects;
    QRect m_matchrect;

    int m_imatchrectnum;
    QList<QPoint> m_reslutpoints;
    QList<int> m_reslutnums;

    RectsShape m_resultrects;

    double m_danglegap;//5

    QVector<PointsShape> m_models_rotate;//1 degree
    QVector<PointsShape> m_models_rotaterects;//4 points
    QVector<PointsShape> m_rotateshaperesults;//4 points

    QList<QPoint> m_rotatereslutpoints;
    QList<double> m_rotateresults;//
    QList<double> m_rotatereslutangles;//

    QList<QCluster> m_clusters;

    int m_ixclustergap;
    int m_iyclustergap;
    int m_iangleclustergap;

    int m_iminfindnum;
    QPoint m_iminpointkey;
    int m_imaxfindnum;
    QPoint m_imaxpointkey;

    QGrid *m_pgrid;//12X12

    QRootGrid *m_rootgridA;

//  QGrid *m_pgrid_l0;//3X3
//  QGrid *m_pgrid_l1;//6X6

    void Learn(ImageBase &image);
    void resulttolist(QPoint &apoint,int inum);
    void resultclear();
    void resultsort();
    void clusterclear();
    void resultcluster(int ixgap,int iygap,int ianglegap);

    void MatchSample(ImageBase &image,QPainterPath &path);
    void MultiMatchSample(ImageBase &image,QPainterPath &path);
    void RotateMatch(ImageBase &image);
    void RotateMatchSample(ImageBase &image,QPainterPath &path,PointsShape &modelrect,double dangle);
    int m_ispecshow;
    double m_dminscore;

    //20201222
    static int m_curfastmatchnum;

    fastmatch * m_prelationmatch;

    int m_irelationresultnum;

    double m_drelationzoomx;

    double m_drelationzoomy;

    QRect m_irelationrect;

public:
    RectsShape *getresultrects(){return &m_resultrects;}

    void setrelationrectfromresultnum(int inum);
    void setrelationrectfrom_matchresult(void *pmatch);
    void setrelationxy(int iprex1,int iprey1,int iendx1,int iendy1);
    void setrelationzoom(double drelationzoomx,double drelationzoomy);
    void setrelationtorect();
    void setcolorstyle(int istyle);



    void MinModelLearn(ImageBase &image);

    void shapesetroi(void *pshape);
};





#endif
