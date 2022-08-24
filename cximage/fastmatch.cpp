
#include "fastmatch.h"
#include "backimagemanager.h"



int fastmatch::m_curfastmatchnum = 0;
fastmatch::fastmatch():findline(),
    m_matchimage(0),
    m_imaxmatchnum(10),
    m_iminfindnum(-1),
    m_imatchthre(5),
    m_ispecshow(-1),
    m_iB2W(0),
    m_imatchoffset(1),
    m_dminscore(0.4),
    m_prelationmatch(0),
    m_irelationresultnum(0),
    m_drelationzoomx(1),
    m_drelationzoomy(1),
    m_danglegap(2),
    m_ixclustergap(5),
    m_iyclustergap(5),
    m_iangleclustergap(5),
    m_istyle(0),
    m_rootgridA(0)
{
    setcolor(0,0,255);
    QString strname = QString("fmatch%1").arg(m_curfastmatchnum);
    setname(strname.toStdString().c_str());
    m_curfastmatchnum = m_curfastmatchnum + 1;
    m_matchrects.setPen(Qt::blue);
    m_matchrects.setshow(2);
    m_imatchrectnum = 1;
    QRect rect(20,20,300,200);
    m_matchrects.addrect(rect);

    int icurmodule = BackImageManager::GetCurMode();
    g_pmodelimage = BackImageManager::GetModelImage(icurmodule);
    m_rootgridA = BackImageManager::GetRootGridA(icurmodule);

    m_pgrid = new QGrid;
    m_pgrid->setgrid(30,30,12,12,30,30);

}
void fastmatch::setgrid(int iw,int igrid)
{
    m_pgrid->setgrid(iw,iw,igrid,igrid,iw,iw);
}
fastmatch::~fastmatch()
{
    delete m_pgrid;
}
void fastmatch::setcomparegap(int igap)
{
    findline::setcomparegap(igap);
}
void fastmatch::setfindnum(int ifindnum)
{
    m_imaxmatchnum = ifindnum;
}
void fastmatch::setb2w(int ib2w)
{
    m_iB2W = ib2w;
}
void fastmatch::setrect(int ix, int iy, int iw, int ih)
{
    if(show()&0x20)
        m_matchrects.getrect(0).setRect(ix,iy,iw,ih);
    findline::setrect(ix,iy,iw,ih);
}
void fastmatch::setshow(int ishow)
{
    QShape::setshow(ishow);
}
void fastmatch::SetWHgap(int wgap,int hgap)
{
    findline::SetWHgap(wgap,hgap);
}
void fastmatch::measure(void *pimage)
{
    findline::measure(pimage);
}
void fastmatch::setlinesamplerate(double dsamplerate)
{
    findline::setlinesamplerate(dsamplerate);
}
void fastmatch::setlinegap(int igap)
{
    findline::setlinegap(igap);
}
void fastmatch::setmethod(int imethod)
{
    findline::setmethod(imethod);
}
void fastmatch::setthre(int ithre)
{
    findline::setthre(ithre);
}
void fastmatch::setmatchthre(int ithre)
{
    m_imatchthre = ithre;
}
void fastmatch::setfindsetting(int ifindset)
{
    findline::setfindsetting(ifindset);
}
void fastmatch::setfilter(int ifilterborw,int ifiltermin,int ifiltermax)
{
    findline::setfilter(ifilterborw,ifiltermin,ifiltermax);
}
void fastmatch::setselectedgenum(int iedgenum)
{
    findline::setselectedgenum(iedgenum);
}
QVector<PointsShape> &fastmatch::getmodels_l12()
{
    return m_models_l12;
}
void fastmatch::setspecshow(int ishow)
{
    m_ispecshow = ishow;
    m_resultrects.setspecshow(ishow);
    m_matchrects.setspecshow(ishow);
}
void fastmatch::drawshape(QPainter &painter,QPalette &pal)
{
    if(show()&0x20)
    {
        m_pgrid->setshow(0x04);
        m_pgrid->drawshape(painter);
    }
    if(show()&0x08)
    {
        drawpattern(painter);

    }
    if(show()&0x10)
    {
        m_modelpoints_sample1.setshow(8);
        m_modelpoints_sample1.drawshape(painter);
    }
    if(show()&0x02)
    {
        m_resultrects.setshow(2);
        m_resultrects.drawshape(painter);
    }
    if(show()&0x04)
    {
        m_resultrects.setshow(4);
        m_resultrects.drawshape(painter);
    }
    else if(show()&0x40)
    {
           /*int irsize = m_rotateresults.size();
           for(int i=0;i<irsize;i=i+4)
           {
              PointsShape apoints= m_rotateshaperesults[i];
              apoints.setshow(16);
              apoints.drawshape(painter);
           }
           */
           int iclustersize = m_clusters.size();
           for(int ic=0;ic<iclustersize;ic++)
           {
                int id=m_clusters[ic][0];
                PointsShape apoints= m_rotateshaperesults[id];
                apoints.setshow(16);
                apoints.drawshape(painter);
           }


    }
    else if(show()&0x80)
    {
        int isize = m_models_rotate.size();
        for(int iz=0;iz<isize;iz++)
        {
            if(-1==m_ispecshow||iz==m_ispecshow)
            {
                m_models_rotate[iz].setshow(8);
                m_models_rotate[iz].drawshape(painter);
                m_models_rotaterects[iz].setshow(16);
                m_models_rotaterects[iz].drawshape(painter);
            }
        }
    }
    else
        findline::drawshape(painter,pal);
    painter.save();
    painter.setPen(Qt::darkBlue);
    QBrush abrush(Qt::transparent, Qt::NoBrush);
    painter.setBrush(abrush);
    painter.drawRect(m_matchrect);
   painter.restore();

    m_matchrects.drawshape(painter);
}
void fastmatch::setcolorstyle(int istyle)
{
    m_istyle=istyle;
}
void fastmatch::drawshapex(QPainter &painter,
                           QPalette &pal,
                      double dmovx,
                      double dmovy,
                      double dangle,
                      double dzoomx,
                      double dzoomy)
{
    if(show()&0x20)
    {
        m_pgrid->setshow(0x04);
        m_pgrid->drawshape(painter);
    }
    if(show()&0x08)
    {
        drawpattern(painter);
        if(nullptr!=m_rootgridA)
            m_rootgridA->drawgrid(painter);

    }
    if(show()&0x10)
    {
        m_modelpoints_sample1.setshow(8);
        m_modelpoints_sample1.drawshape(painter);
    }
    if(show()&0x02)
    {
        m_resultrects.setshow(2);
        m_resultrects.drawshapex(painter,
                                dmovx,
                                dmovy,
                                dangle,
                                dzoomx,
                                dzoomy);
    }
    if(show()&0x04)
    {
        m_resultrects.setshow(4);
        m_resultrects.drawshapex(painter,
                                 dmovx,
                                 dmovy,
                                 dangle,
                                 dzoomx,
                                 dzoomy);
    }
    else if(show()&0x40)
    {
        /*int irsize = m_rotateresults.size();
        for(int i=0;i<irsize;i=i+4)
        {
           PointsShape apoints= m_rotateshaperesults[i];
           apoints.setshow(16);
           apoints.drawshape(painter);
        }
        */
        int iclustersize = m_clusters.size();
        for(int ic=0;ic<iclustersize;ic++)
        {
             int id=m_clusters[ic][0];
             PointsShape apoints= m_rotateshaperesults[id];
             apoints.setshow(16);
             apoints.drawshape(painter);
        }
    }
    else if(show()&0x80)
    {
        int isize = m_models_rotate.size();
        for(int iz=0;iz<isize;iz++)
        {
            if(-1==m_ispecshow||iz==m_ispecshow)
            {
                m_models_rotate[iz].setshow(8);
                m_models_rotate[iz].drawshape(painter);
                m_models_rotaterects[iz].setshow(16);
                m_models_rotaterects[iz].drawshape(painter);
            }
        }
    }
    else
        findline::drawshape(painter,pal);
    painter.save();
    if(0==m_istyle)
        painter.setPen(Qt::darkBlue);
    else if(1==m_istyle)
        painter.setPen(Qt::red);
    else if(2==m_istyle)
        painter.setPen(Qt::green);
    else
        painter.setPen(Qt::blue);
        //);
    QBrush abrush(Qt::transparent, Qt::NoBrush);
    painter.setBrush(abrush);
    QRect amatchrect;
    amatchrect.setX(m_matchrect.x()*dzoomx+dmovx);
    amatchrect.setY(m_matchrect.y()*dzoomy+dmovy);
    amatchrect.setWidth(m_matchrect.width()*dzoomx+dmovx);
    amatchrect.setHeight(m_matchrect.height()*dzoomy+dmovy);

    painter.drawRect(amatchrect);
   painter.restore();
    if(0)
    {
        m_matchrects.drawshapex(painter,
                                dmovx,
                                dmovy,
                                dangle,
                                dzoomx,
                                dzoomy);
    }
}


void fastmatch::Learn(ImageBase &image)
{
    edgepattern(image);
    modelzeroposition();
}
void fastmatch::modelzeroposition()
{
    findline::patternzeroposition();
}
void fastmatch::rotatemodelzeroposition()
{
    int isize = m_models_rotate.size();
    for(int iz=0;iz<isize;iz++)
    {
        QRectF arect1 = m_models_rotate[iz].boundingRect();
        m_models_rotate[iz].Move(-arect1.x(),-arect1.y());
        m_models_rotaterects[iz].Move(-arect1.x(),-arect1.y());
    }
}
void fastmatch::learn(void *pimage)
{
    ImageBase *pgetimage = (ImageBase*)pimage;
    Learn(*pgetimage);
}

void fastmatch::savemodelfile(const char * pchar)
{
    findline::savepatternfile(pchar);
}
void fastmatch::loadmodelfile(const char * pchar)
{
    findline::loadpatternfile(pchar);
    QRectF arect1 = findline::patternboundingrect();
    m_imodelwith = arect1.width();
    m_imodelheigh = arect1.height();
}
void fastmatch::setrotateangle(double dangle)
{
    m_danglegap=dangle;
}
void fastmatch::loadrotatemodelfile(const char * pchar)
{
    //red(white 1) gap blue(black 0) model
    findline::loadpatternfile(pchar);
    QRectF arect1 = findline::patternboundingrect();
    m_imodelwith = arect1.width();
    m_imodelheigh = arect1.height();

    m_models_rotate.clear();//5 degree

    m_models_rotaterects.clear();//4 points

    int ispiltlsize1 = 360;
    int ianglecur =0;
    PointsShape amodelpoints;
    PointsShape arectpoints;
    PointsShape brectpoints;
    PointsShape bmodelrect;
    QPointF apoint(0,0);
    QPointF bpoint(m_imodelwith,0);
    QPointF cpoint(m_imodelwith,m_imodelheigh);
    QPointF dpoint(0,m_imodelheigh);
    arectpoints.addpoint(apoint);
    arectpoints.addpoint(bpoint);
    arectpoints.addpoint(cpoint);
    arectpoints.addpoint(dpoint);

    for(int i=0;i<360;i++)
    {
        ianglecur=i;
        amodelpoints=findline::getpattern();
        brectpoints=arectpoints;
       // QFont afont("Fixedsys", 16);
       // QString astr = QString("%1").arg(i);
        bmodelrect=brectpoints;
       // bmodelrect.addText(0,-8,afont,astr);
        amodelpoints.Rotate(ianglecur);
        bmodelrect.Rotate(ianglecur);

        m_models_rotate.append(amodelpoints);
        m_models_rotaterects.append(bmodelrect);
    }

  rotatemodelzeroposition();
}

void fastmatch::clearmodels_l12()
{
    m_models_l12.clear();
}
void fastmatch::addmodels_l12(const char * pchar)
{
    findline::loadpatternfile(pchar);
    m_models_l12.push_back(findline::getpattern());
}
void fastmatch::clearmodels_l36()
{
    m_models_l36.clear();
}
void fastmatch::addmodels_l36(const char * pchar)
{
    findline::loadpatternfile(pchar);
    m_models_l36.push_back(findline::getpattern());
}
void fastmatch::clearmodels_l72()
{
    m_models_l72.clear();
}
void fastmatch::addmodels_l72(const char * pchar)
{
    findline::loadpatternfile(pchar);
    m_models_l72.push_back(findline::getpattern());
}
void fastmatch::clearmodels_rotate()
{
    m_models_rotate.clear();
}
void fastmatch::addmodels_rotate(const char * pchar)
{
    findline::loadpatternfile(pchar);
    m_models_rotate.push_back(findline::getpattern());
}


void fastmatch::setcurmodels(int inum)
{
    if(inum<m_models_l12.size())
        findline::setpattern(m_models_l12[inum]);
}
void fastmatch::setcurimagemodels(int inum)
{
    m_pgrid->SetUnit(12,12);
    m_pgrid->UnitGrid();

    m_pgrid->ModelGridMethod_Gauss();
    m_imagefastmodel = m_pgrid->getfastmodel();

    if(inum<m_imagefastmodels_l12.size())
       m_imagefastmodels_l12[inum] = m_imagefastmodel;
}
void fastmatch::modelstocurrent_l72(int i)
{
    if(i<m_models_l72.size())
        findline::setpattern(m_models_l72[i]);
}
void fastmatch::modelstocurrent_l36(int i)
{
    if(i<m_models_l36.size())
        findline::setpattern(m_models_l36[i]);
}
void fastmatch::modelstocurrent_l12(int i)
{
    if(i<m_models_l12.size())
        findline::setpattern(m_models_l12[i]);
}
void fastmatch::modelstocurrent_l3(int i)
{
    if(i<m_models_l3.size())
        findline::setpattern(m_models_l3[i]);
}
void fastmatch::modelstocurrent_l6(int i)
{
    if(i<m_models_l6.size())
       findline::setpattern(m_models_l6[i]);
}
void fastmatch::patternrootgrid(double itype, double drate, double ilevel)
{
    findline::patternrootgrid(itype,drate,ilevel);
}
void fastmatch::patterntranform(int igap,int itype,int isgap,int iline)
{
     findline::patterntranform(igap,itype,isgap, iline);
     findline::patternzeroposition();
}
void fastmatch::patternzoom(double dx,double dy,double igap,double itype)
{
     findline::patternzoom(dx,dy,igap,itype);
     findline::patternzeroposition();
}
void fastmatch::modelrotate(double dangle)
{
     findline::patternrotate(dangle);
}
void fastmatch::modelzoom(double dx,double dy)
{
     findline::modelzoom(dx,dy);
}
void fastmatch::setmodelwh(int iw,int ih)
{
    QRectF rectf = findline::patternboundingrect();
    int iorgw = rectf.width();
    int iorgh = rectf.height();
    double dw = (iw*1.0)/(iorgw*1.0);
    double dh = (ih*1.0)/(iorgh*1.0);
    modelzoom(dw,dh);
}
void fastmatch::setmatchrect(int ix,int iy,int iw,int ih)
{
    if(ix>=0&&iy>=0&&iw>0&&ih>0)
    m_matchrect = QRect(ix,iy,iw,ih);
  //  m_matchrects.setrect(0,ix,iy,iw,ih);
}

void fastmatch::setmatchrectnum(int inum)
{
    if(inum<=0)
        inum =1;
    m_matchrects.clear();
    if(inum>0)
    for(int i=0;i<inum;i++)
    {
        QRect arect(100,100,200,200);
        QString str = QString("%1").arg(i);
        m_matchrects.addrect(arect,str);
    }
}
QRect &fastmatch::getmatchrect()
{
    return m_matchrect;
}
RectsShape &fastmatch::getmatchrects()
{
    return m_matchrects;
}
QRect fastmatch::getresultrect(int inum)
{
    QRect arect0 = m_resultrects.getrect(inum);
    return arect0;
}
void fastmatch::setmultimatchrect(int inum,int ix,int iy,int iw,int ih)
{
    if(inum<m_matchrects.size())
    m_matchrects.setrect(inum,ix,iy,iw,ih);
}
void fastmatch::resultclear()
{
    m_iminfindnum = -1;
    m_reslutpoints.clear();
    m_reslutnums.clear();
}
void fastmatch::resulttolist(QPoint &apoint,int inum)
{
    if(inum>m_iminfindnum)
    {
        //have same area
        for (int i = 0; i < m_reslutnums.size(); ++i)
        {
            QPoint apoint0 = m_reslutpoints.at(i);
            int ivalue = m_reslutnums.at(i);
            int ix0 = apoint0.x() - m_imodelwith/2 ;
            int iy0 = apoint0.y() - m_imodelheigh/2;
            int ix1 = apoint0.x() + m_imodelwith/2;
            int iy1 = apoint0.y() + m_imodelheigh/2;
            if(ix0<apoint.x()
            &&ix1>apoint.x()
            &&iy0<apoint.y()
            &&iy1>apoint.y())
            {
                if(ivalue<inum)
                {
                    m_reslutpoints[i] = apoint;
                    m_reslutnums[i] = inum;
                    goto NextRun01;
                }
                else
                    return;
            }
       }

        if(m_iminfindnum!=-1
        &&m_reslutpoints.size()>=m_imaxmatchnum)
        {
            int inum = m_reslutnums.indexOf(m_iminfindnum);
            m_reslutnums.removeAt(inum);
            m_reslutpoints.removeAt(inum);
        }

        m_reslutpoints.push_back(apoint);
        m_reslutnums.push_back(inum);
    }
    else
        return;


    //
    //

NextRun01:
    //
    int imini = 9999;
    QPoint minipoint ;
    for (int i = 0; i < m_reslutnums.size(); ++i)
  {
        int ivalue = m_reslutnums.at(i);
        QPoint apoint1 = m_reslutpoints.at(i);
        if(ivalue<imini)
        {
            minipoint = apoint1;
            imini = ivalue;
        }
   }

    m_iminfindnum = imini;
    m_iminpointkey = minipoint;
}
void fastmatch::resultsort()
{
    int imini = 9999;
    int imaxi = 0;
    QPoint minpoint ;
    QPoint maxpoint ;

    int isize = m_reslutnums.size();
    if(isize<=0)
        return;

    int iprevalue =  m_reslutnums.at(0);
    QPoint prepoint = m_reslutpoints.at(0);
    int iprenum = 0;
    int inxtvalue =  m_reslutnums.at(0);
    QPoint nxtpoint = m_reslutpoints.at(0);
    int inxtnum = 0;

    for (int i = 1; i < m_reslutnums.size(); ++i)
    {
        iprevalue =  m_reslutnums.at(i-1);
        prepoint = m_reslutpoints.at(i-1);
        iprenum = i-1;

        inxtvalue =  m_reslutnums.at(i);
        nxtpoint = m_reslutpoints.at(i);
        inxtnum = i;

        if(iprevalue>inxtvalue)
        {
            m_reslutnums[iprenum] =inxtvalue;
            m_reslutpoints[iprenum] =nxtpoint;
            m_reslutnums[inxtnum] =iprevalue;
            m_reslutpoints[inxtnum] =prepoint;

            for(int j=i-1;j>0;j--)
            {
                iprevalue =  m_reslutnums.at(j-1);
                prepoint = m_reslutpoints.at(j-1);
                iprenum = j-1;

                inxtvalue = m_reslutnums.at(j);
                nxtpoint = m_reslutpoints.at(j);
                inxtnum = j;

                if(iprevalue>inxtvalue)
                {
                    m_reslutnums[iprenum] =inxtvalue;
                    m_reslutpoints[iprenum] =nxtpoint;
                    m_reslutnums[inxtnum] =iprevalue;
                    m_reslutpoints[inxtnum] =prepoint;
                }
            }
        }

   }

}
void fastmatch::clusterclear()
{

}
void fastmatch::resultcluster(int ixgap,int iygap,int ianglegap)
{
    int ipointsize = m_rotatereslutpoints.size();
    QPoint apoint;
    bool binsert = false;
    for(int ir=0;ir<ipointsize;ir++)
    {
        binsert = false;
        apoint = m_rotatereslutpoints[ir];
        int ix0 =apoint.x();
        int iy0 =apoint.y();
        double dangle0 = m_rotatereslutangles[ir];
        int iclustersize = m_clusters.size();
        for(int ic=0;ic<iclustersize;ic++)
        {
            if(false==binsert)
            {
                int icsize = m_clusters[ic].size();
                for(int in=0;in<icsize;in++)
                {
                    if(false==binsert)
                    {
                        int ipos=m_clusters[ic][in];
                        QPoint bpoint = m_rotatereslutpoints[ipos];
                        int ix1 =bpoint.x();
                        int iy1 =bpoint.y();
                        double dangle1 = m_rotatereslutangles[ipos];
                        int ixgap0 = ix1-ix0>0?ix1-ix0:ix0-ix1;
                        int iygap0 = iy1-iy0>0?iy1-iy0:iy0-iy1;
                        double danglegap0 = dangle1-dangle0>0?dangle1-dangle0:dangle0-dangle1;
                        if(ixgap0<ixgap
                          &&iygap0<iygap
                          &&danglegap0<ianglegap)
                        {
                            m_clusters[ic].append(ir);
                            binsert=true;
                        }
                    }
                }
            }
            if(true==binsert)
            {
                int icsize = m_clusters[ic].size();
                for(int in=0;in<icsize-1;in++)
                {
                        int ipos0=m_clusters[ic][in];
                        int ipos1=m_clusters[ic][in+1];
                        double dangle0 = m_rotatereslutangles[ipos0];
                        double dangle1 = m_rotatereslutangles[ipos1];
                        if(dangle0<dangle1)
                        {
                            m_clusters[ic][in]=ipos1;
                            m_clusters[ic][in+1]=ipos0;
                        }
                }
            }
        }
        if(false==binsert)
        {
            QCluster newcluster;
            newcluster.append(ir);
            m_clusters.append(newcluster);
        }
    }

}
void fastmatch::Match(ImageBase &image)
{
    m_matchimage = &image;
    resultclear();
    QPainterPath &path = findline::getpatternpath();
     MatchSample(image,path);
}
void fastmatch::match(void *pimage)
{
    ImageBase *pgetimage = (ImageBase*)pimage;

    Match(*pgetimage);
}
void fastmatch::loadfastimagemodel(const char *pfilename)
{
    m_pgrid->loadmapmodel(pfilename);
    m_imagefastmodel = m_pgrid->getfastmodel();
}
QVector<int> *fastmatch::getcurimagemodel()
{
    return &m_imagefastmodel;
}
void fastmatch::imagemodesclear_l12()
{
    m_imagefastmodels_l12.clear();
}
void fastmatch::addimagemodels_l12(const char *pfilename)
{
    m_pgrid->loadmapmodel(pfilename);

   // if(1)//test
   // {
   //     m_pgrid->SetUnit(12,12);
   //     m_pgrid->UnitGrid();
   // }
   // else
    {
        m_pgrid->ZeroModel();
        m_pgrid->ReGrid(12,12);
    }
    m_pgrid->Grid2PattenModel(findline::getconparegap());
    m_models_l12.push_back(m_pgrid->getpatmodel());

   easyobject aeobj = m_pgrid->ModelGridMethod_ObjectA();
   m_easyobjectmodels_l12.push_back(aeobj);

   m_pgrid->ModelGridMethod_Gauss();

    m_imagefastmodel = m_pgrid->getfastmodel();
    m_imagefastmodels_l12.push_back(m_imagefastmodel);

}
void fastmatch::imagemodesclear_l36()
{
    m_imagefastmodels_l36.clear();
}
void fastmatch::addimagemodels_l36(const char *pfilename)
{
    m_pgrid->loadmapmodel(pfilename);
    m_pgrid->ZeroModel();
    m_pgrid->ReGrid(36,36);
     if(0)//test
     {
        m_pgrid->SetUnit(36,36);
        m_pgrid->UnitGrid();
     }

    m_pgrid->Grid2PattenModel(findline::getconparegap());
    m_models_l36.push_back(m_pgrid->getpatmodel());

   easyobject aeobj = m_pgrid->ModelGridMethod_ObjectA();
   m_easyobjectmodels_l36.push_back(aeobj);

   m_pgrid->ModelGridMethod_Gauss();

    m_imagefastmodel = m_pgrid->getfastmodel();
    m_imagefastmodels_l36.push_back(m_imagefastmodel);

}
void fastmatch::imagemodesclear_l72()
{
    m_imagefastmodels_l72.clear();
}
void fastmatch::addimagemodels_l72(const char *pfilename)
{
    m_pgrid->loadmapmodel(pfilename);
    m_pgrid->ZeroModel();
    m_pgrid->ReGrid(72,72);
    if(0)//test
    {
        m_pgrid->SetUnit(72,72);
        m_pgrid->UnitGrid();
    }

    m_pgrid->Grid2PattenModel_org(findline::getconparegap());
    m_models_l72.push_back(m_pgrid->getpatmodel());

   easyobject aeobj = m_pgrid->ModelGridMethod_ObjectA();
   m_easyobjectmodels_l72.push_back(aeobj);

   m_pgrid->ModelGridMethod_Gauss();

    m_imagefastmodel = m_pgrid->getfastmodel();
    m_imagefastmodels_l72.push_back(m_imagefastmodel);

}


QGrid *fastmatch::getgrid()
{
    return m_pgrid;
}
bool fastmatch::modelcompare(QVector<int> &modela, QVector<int> &modelb)
{
    int isize0 = modela.size();
    int isize1 = modelb.size();
    if(isize0!=isize1)
        return false;
    for(int i=0;i<isize0;i++)
    {
        if(modela[i]!=modelb[i])
            return false;
    }
    return true;
}
QMap<int ,int > &fastmatch::getlevel3_6map()
{
   return m_mapl3_l6;
}
QMap<int ,int > &fastmatch::getlevel6_12map()
{
    return m_mapl6_l12;
}
QMap<int ,int > &fastmatch::getlevel12_36map()
{
    return m_mapl12_l36;
}
QMap<int ,int > &fastmatch::getlevel36_72map()
{
    return m_mapl36_l72;
}
void fastmatch::clearmodel()
{
    m_easyobjectmodels_l12.clear();//
    m_models_l12.clear();
    m_imagefastmodels_l12.clear();
    m_mapl12_l36.clear();

    m_easyobjectmodels_l6.clear();
    m_models_l6.clear();
    m_imagefastmodels_l6.clear();
    m_mapl6_l12.clear();

    m_easyobjectmodels_l3.clear();
    m_models_l3.clear();
    m_imagefastmodels_l3.clear();
    m_mapl3_l6.clear();
}
void fastmatch::list_duplicatesmodel_l12()
{
    m_duplicates_list_l12.clear();
    int isize = m_imagefastmodels_l12.size();

    for(int i=0;i<isize;i++)
    {
        m_duplicates_list_l12.push_back(0);
    }
    int iduplicatesnum = 1;
    for(int i=0;i<isize;i++)
    {
        if(0==m_duplicates_list_l12[i])
        {
            for(int j=0;j<isize;j++)
            {
                if(i!=j)
                {
                    if(modelcompare(m_imagefastmodels_l12[i],m_imagefastmodels_l12[j]))
                    {
                        int isetvalue = 0;
                        if(m_duplicates_list_l12[i]==0&&m_duplicates_list_l12[j]==0)
                        {
                            m_duplicates_list_l12[i] = iduplicatesnum;
                            m_duplicates_list_l12[j] = iduplicatesnum;
                            iduplicatesnum = iduplicatesnum + 1;
                        }
                        else if(m_duplicates_list_l12[i]!=0)
                        {
                            isetvalue = m_duplicates_list_l12[i];
                            m_duplicates_list_l12[j] = isetvalue;
                        }
                        else if(m_duplicates_list_l12[j]!=0)
                        {
                            isetvalue = m_duplicates_list_l12[j];
                            m_duplicates_list_l12[i] = isetvalue;
                        }
                    }
                }
            }
        }
    }

}
void fastmatch::list_duplicatesmodel_l36()
{
    m_duplicates_list_l36.clear();
    int isize = m_imagefastmodels_l36.size();

    for(int i=0;i<isize;i++)
    {
        m_duplicates_list_l36.push_back(0);
    }
    int iduplicatesnum = 1;
    for(int i=0;i<isize;i++)
    {
        if(0==m_duplicates_list_l36[i])
        {
            for(int j=0;j<isize;j++)
            {
                if(i!=j)
                {
                    if(modelcompare(m_imagefastmodels_l36[i],m_imagefastmodels_l36[j]))
                    {
                        int isetvalue = 0;
                        if(m_duplicates_list_l36[i]==0&&m_duplicates_list_l36[j]==0)
                        {
                            m_duplicates_list_l36[i] = iduplicatesnum;
                            m_duplicates_list_l36[j] = iduplicatesnum;
                            iduplicatesnum = iduplicatesnum + 1;
                        }
                        else if(m_duplicates_list_l36[i]!=0)
                        {
                            isetvalue = m_duplicates_list_l36[i];
                            m_duplicates_list_l36[j] = isetvalue;
                        }
                        else if(m_duplicates_list_l36[j]!=0)
                        {
                            isetvalue = m_duplicates_list_l36[j];
                            m_duplicates_list_l36[i] = isetvalue;
                        }
                    }
                }
            }
        }
    }

}
void fastmatch::list_duplicatesmodel_l72()
{
    m_duplicates_list_l72.clear();
    int isize = m_imagefastmodels_l72.size();

    for(int i=0;i<isize;i++)
    {
        m_duplicates_list_l72.push_back(0);
    }
    int iduplicatesnum = 1;
    for(int i=0;i<isize;i++)
    {
        if(0==m_duplicates_list_l72[i])
        {
            for(int j=0;j<isize;j++)
            {
                if(i!=j)
                {
                    if(modelcompare(m_imagefastmodels_l72[i],m_imagefastmodels_l72[j]))
                    {
                        int isetvalue = 0;
                        if(m_duplicates_list_l72[i]==0&&m_duplicates_list_l72[j]==0)
                        {
                            m_duplicates_list_l72[i] = iduplicatesnum;
                            m_duplicates_list_l72[j] = iduplicatesnum;
                            iduplicatesnum = iduplicatesnum + 1;
                        }
                        else if(m_duplicates_list_l72[i]!=0)
                        {
                            isetvalue = m_duplicates_list_l72[i];
                            m_duplicates_list_l72[j] = isetvalue;
                        }
                        else if(m_duplicates_list_l72[j]!=0)
                        {
                            isetvalue = m_duplicates_list_l72[j];
                            m_duplicates_list_l72[i] = isetvalue;
                        }
                    }
                }
            }
        }
    }

}

QVector<int> &fastmatch::getduplicateslist_l36()
{
    return m_duplicates_list_l36;
}
QVector<int> &fastmatch::getduplicateslist_l12()
{
    return m_duplicates_list_l12;
}

void fastmatch::modelmethod(int itype)
{
    //m_pgrid->ReGrid(72,72);
    m_pgrid->SetFastModel(m_imagefastmodel);
    m_pgrid->ReSetModelGrid();
//
    switch (itype) {
    case 0:
        m_pgrid->ReSetModelGrid();
        break;
    case 1:
        m_pgrid->ModelGridMethod_Gauss();
        break;
    case 2:
        m_pgrid->ModelGridMethod_Object();
        break;
    case 3:
        m_pgrid->ModelGridMethod_ObjectA();
        break;
    case 4:
        m_pgrid->ModelGridMethod_Inside();
        break;
    case 5:
        m_pgrid->ModelGridMethod_Outside();
        break;
    default:
        m_pgrid->ReSetModelGrid();
        break;
    }
    m_imagefastmodel = m_pgrid->getfastmodel();

}
void fastmatch::levelmodels_l72tol36()
{
    //model level 12 load from file so no need to clear in here


    int isize = m_imagefastmodels_l72.size();

    for(int i=0;i<isize;i++)
      {
          m_pgrid->SetModelWH(72,72);
          m_pgrid->SetFastModel(m_imagefastmodels_l72[i]);
          m_pgrid->ReSetModelGrid();
          m_pgrid->ZeroModel();
          m_pgrid->GridZoom(36,36);
          m_pgrid->SetUnit(36,36);
          //
         m_pgrid->Grid2PattenModel(findline::getconparegap());//m_icomparegap
         // m_pgrid->ZeroModel();

         easyobject aeobj = m_pgrid->ModelGridMethod_ObjectA();

         m_pgrid->ModelGridMethod_Gauss();

         m_imagefastmodel = m_pgrid->getfastmodel();

         int imsize = m_imagefastmodels_l36.size();
         int isame = -1;
         for(int im=0;im<imsize;im++)
         {
               if(modelcompare(m_imagefastmodels_l36[im],m_imagefastmodel))
               {
                   m_mapl36_l72[i] =  im;
                   isame = 1;
                   break;
               }
         }
         if(-1==isame)
         {
              m_imagefastmodels_l36.push_back(m_imagefastmodel);
              m_models_l36.push_back(m_pgrid->getpatmodel());
              m_easyobjectmodels_l36.push_back(aeobj);
              m_mapl36_l72[i] = m_imagefastmodels_l36.size() - 1;
         }
      }
}

void fastmatch::levelmodels_l36tol12()
{
    //model level 12 load from file so no need to clear in here


    int isize = m_imagefastmodels_l36.size();

    for(int i=0;i<isize;i++)
      {
          m_pgrid->SetModelWH(36,36);
          m_pgrid->SetFastModel(m_imagefastmodels_l36[i]);
          m_pgrid->ReSetModelGrid();
          m_pgrid->ZeroModel();
          m_pgrid->GridZoom(12,12);
          m_pgrid->SetUnit(12,12);
          //
         m_pgrid->Grid2PattenModel(findline::getconparegap());//m_icomparegap
         // m_pgrid->ZeroModel();

         easyobject aeobj = m_pgrid->ModelGridMethod_ObjectA();

         m_pgrid->ModelGridMethod_Gauss();

          m_imagefastmodel = m_pgrid->getfastmodel();

          int imsize = m_imagefastmodels_l12.size();
          int isame = -1;
          for(int im=0;im<imsize;im++)
          {
               if(modelcompare(m_imagefastmodels_l12[im],m_imagefastmodel))
               {
                   m_mapl12_l36[i] =  im;
                   isame = 1;
                   break;
               }
          }
          if(-1==isame)
          {
              m_imagefastmodels_l12.push_back(m_imagefastmodel);
              m_models_l12.push_back(m_pgrid->getpatmodel());
              m_easyobjectmodels_l12.push_back(aeobj);
              m_mapl12_l36[i] = m_imagefastmodels_l12.size() - 1 ;

          }
      }
}

void fastmatch::levelmodels_l12tol6()
{

//    m_imagefastmodels_l12_l2.clear();

    int isize = m_imagefastmodels_l12.size();
//    for(int i=0;i<isize;i++)
//    {
//        m_pgrid->SetModelWH(12,12);
//        m_pgrid->SetFastModel(m_imagefastmodels_l12[i]);
//        m_pgrid->ReSetModelGrid();
//        m_pgrid->ZeroModel();
//        m_pgrid->SetUnit(12,12);
//        m_imagefastmodel = m_pgrid->getfastmodel();
//        m_imagefastmodels_l12_l2.push_back(m_imagefastmodel);
//    }


      for(int i=0;i<isize;i++)
      {
          m_pgrid->SetModelWH(12,12);
          m_pgrid->SetFastModel(m_imagefastmodels_l12[i]);

          m_pgrid->ReSetModelGrid();
          m_pgrid->ZeroModel();
          m_pgrid->GridZoom(6,6);
          m_pgrid->SetUnit(6,6);
          //
          m_pgrid->Grid2PattenModel(findline::getconparegap());//m_icomparegap
          //m_pgrid->ZeroModel();

         easyobject aeobj = m_pgrid->ModelGridMethod_ObjectA();

         m_pgrid->ModelGridMethod_Gauss();

          m_imagefastmodel = m_pgrid->getfastmodel();

          int imsize = m_imagefastmodels_l6.size();
          int isame = -1;
          for(int im=0;im<imsize;im++)
          {
               if(modelcompare(m_imagefastmodels_l6[im],m_imagefastmodel))
               {
                   m_mapl6_l12[i] =  im;
                   isame = 1;
                   break;
               }
          }
          if(-1==isame)
          {
              m_imagefastmodels_l6.push_back(m_imagefastmodel);
              m_models_l6.push_back(m_pgrid->getpatmodel());
              m_easyobjectmodels_l6.push_back(aeobj);
              m_mapl6_l12[i] = m_imagefastmodels_l6.size() - 1 ;
          }
      }
}

void fastmatch::levelmodels_l6tol3()
{
      int isize = m_imagefastmodels_l6.size();
      for(int i=0;i<isize;i++)
      {
          m_pgrid->SetModelWH(6,6);
          m_pgrid->SetFastModel(m_imagefastmodels_l6[i]);

          m_pgrid->ReSetModelGrid();
          m_pgrid->ZeroModel();
          m_pgrid->GridZoom(3,3);
          m_pgrid->SetUnit(3,3);
          //
          m_pgrid->Grid2PattenModel(findline::getconparegap());//m_icomparegap
          //m_pgrid->ZeroModel();

          easyobject aeobj = m_pgrid->ModelGridMethod_ObjectA();

         m_pgrid->ModelGridMethod_Gauss();

          m_imagefastmodel = m_pgrid->getfastmodel();

          int imsize = m_imagefastmodels_l3.size();
          int isame = -1;
          for(int im=0;im<imsize;im++)
          {
               if(modelcompare(m_imagefastmodels_l3[im],m_imagefastmodel))
               {
                   m_mapl3_l6[i] =  im;
                   isame = 1;
                   break;
               }
          }
          if(-1==isame)
          {
              m_models_l3.push_back(m_pgrid->getpatmodel());
              m_imagefastmodels_l3.push_back(m_imagefastmodel);
              m_easyobjectmodels_l3.push_back(aeobj);
              m_mapl3_l6[i] = m_imagefastmodels_l3.size() - 1 ;
          }

      }
}
void fastmatch::savelevel0_l1()
{
    int isize0 = m_models_l3.size();
    for(int i=0;i<isize0;i++)
    {
        QString strfilename = QString("./model/3x3/_%1.pat").arg(i);
        m_models_l3[i].save(strfilename.toStdString().c_str());
    }
    int isize1 = m_models_l6.size();
    for(int i=0;i<isize1;i++)
    {
        QString strfilename = QString("./model/6x6/_%1.pat").arg(i);
        m_models_l6[i].save(strfilename.toStdString().c_str());
    }

    int isize = m_imagefastmodels_l3.size();
    for(int i=0;i<isize;i++)
    {
        m_pgrid->SetModelWH(3,3);
        m_pgrid->SetFastModel(m_imagefastmodels_l3[i]);
        m_pgrid->ReSetModelGrid();
        QString strfilename = QString("./model/3x3/_%1.imp").arg(i);
        m_pgrid->savemapmodel(strfilename.toStdString().c_str());
    }

    isize = m_imagefastmodels_l6.size();
    for(int i=0;i<isize;i++)
    {
        m_pgrid->SetModelWH(6,6);
        m_pgrid->SetFastModel(m_imagefastmodels_l6[i]);
        m_pgrid->ReSetModelGrid();
        QString strfilename = QString("./model/6x6/_%1.imp").arg(i);
        m_pgrid->savemapmodel(strfilename.toStdString().c_str());
    }

     isize = m_imagefastmodels_l12.size();
    for(int i=0;i<isize;i++)
    {
        m_pgrid->SetModelWH(12,12);
        m_pgrid->SetFastModel(m_imagefastmodels_l12[i]);
        m_pgrid->ReSetModelGrid();
        m_pgrid->Grid2PattenModel(2);

        QString strfilename = QString("./model/12x12/_%1.pat").arg(i);
        m_pgrid->savemodelfile(strfilename.toStdString().c_str());

        strfilename = QString("./model/12x12/_%1.imp").arg(i);
        m_pgrid->savemapmodel(strfilename.toStdString().c_str());
    }
}
void fastmatch::imagemodelstocurrent_l72(int i)
{
    m_pgrid->SetModelWH(72,72);

    if(i<m_imagefastmodels_l72.size())
        m_imagefastmodel = m_imagefastmodels_l72[i];
}
void fastmatch::imagemodelstocurrent_l36(int i)
{
    m_pgrid->SetModelWH(36,36);

    if(i<m_imagefastmodels_l36.size())
        m_imagefastmodel = m_imagefastmodels_l36[i];
}
void fastmatch::imagemodelstocurrent_l12(int i)
{
    m_pgrid->SetModelWH(12,12);

    if(i<m_imagefastmodels_l12.size())
        m_imagefastmodel = m_imagefastmodels_l12[i];
}

void fastmatch::imagemodelstocurrent_l3(int i)
{
    m_pgrid->SetModelWH(3,3);
    if(i<m_imagefastmodels_l3.size())
        m_imagefastmodel = m_imagefastmodels_l3[i];
}
void fastmatch::imagemodelstocurrent_l6(int i)
{
    m_pgrid->SetModelWH(6,6);

    if(i<m_imagefastmodels_l6.size())
        m_imagefastmodel = m_imagefastmodels_l6[i];
}

int fastmatch::imagefastmodelsize(int ilevel)
{
    switch (ilevel)
    {
    case 0:
        return m_imagefastmodels_l3.size();
        break;
    case 1:
        return m_imagefastmodels_l6.size();
        break;
    case 2:
        return m_imagefastmodels_l12.size();
        break;
    case 3:
        return m_imagefastmodels_l36.size();
        break;
    case 4:
        return m_imagefastmodels_l72.size();
        break;

    default:
        return m_imagefastmodels_l12.size();
        break;
    }
    return 0;
}


void fastmatch::objectmodelstocurrent(int i)
{
    if(i<m_easyobjectmodels_l12.size())
        m_easyobject = m_easyobjectmodels_l12[i];
}
void fastmatch::savefastimagemodel(const char *pfilename)
{
    m_pgrid->ReSetModelGrid();

    m_pgrid->savemapmodel(pfilename);


}
void fastmatch::savefastimagepatmodel(const char *pfilename)
{
    m_pgrid->Grid2PattenModel(findline::getconparegap());
    m_pgrid->savemodelfile(pfilename);
}
void fastmatch::savematchroi(const char *pfilename)
{
    SaveMatchROI(*m_matchimage,pfilename);
}
void fastmatch::savematchimagemodel(const char *pfilename)
{
    g_pmodelimage->SaveROI(pfilename);
}
void fastmatch::SaveMatchROI(ImageBase &image,const char *pfilename)
{
    int isize = m_resultrects.size();
    QRect arect = m_resultrects.getrect(isize - 1);

    image.SetROI(arect.x()-m_imatchoffset,arect.y(),arect.width(),arect.height());
    image.SaveROI(pfilename);
}
void fastmatch::imagelearn(int ithre1,int iandor)
{
    MatchImageLearn(*m_matchimage,ithre1,iandor);
}
void fastmatch::imagelearnmass(int ithre1,int iandor,int igridwh)
{
    MatchImageLearnMass(*m_matchimage,ithre1,iandor,igridwh);
}
void fastmatch::imagelearncheck(int iimagetype,int iandor,int igridwh)
{
    MatchImageCheck(*m_matchimage,iimagetype,iandor,igridwh);
}

void fastmatch::imagelearnex(int ithre1,int iandor,int igrid)
{
    MatchImageLearnEx(*m_matchimage,ithre1,iandor,igrid);
}
void fastmatch::MatchImageLearn(ImageBase &aimage,int ithre1,int iandor)
{
    int isize = m_resultrects.size();
    if(isize<=0)
        return;

    QRect arect0 = m_resultrects.getrect(isize - 1);
    //QRect arect = m_pgrid->CentRect(arect0);
    m_pgrid->SetUnit(12,12);
    QRect arect = m_pgrid->CentRect_Condition(arect0,3);

    aimage.SetROI(arect.x()-m_imatchoffset,arect.y(),arect.width(),arect.height());
    g_pmodelimage->SetROI(0,0,arect.width(),arect.height());
    aimage.SetMode(3);
    aimage.ROItoROI(g_pmodelimage);
    g_pmodelimage->ROIColorTable();
    g_pmodelimage->ROIColorTableBlur(0,ithre1);
    g_pmodelimage->ROIColorTableEasyThre(iandor);
    m_pgrid->ROIImagetoModel(*g_pmodelimage);
    m_pgrid->SetUnit(12,12);
    m_pgrid->UnitGrid();

    m_pgrid->ModelGridMethod_Gauss();
    m_imagefastmodel = m_pgrid->getfastmodel();
}
int fastmatch::GetRectGridLevel(int irectw)
{
    int igrid = 12;
    if(irectw<=12)
    {
        igrid =12;
    }
    else if(irectw>12&&irectw<=36)
    {
        igrid =36;
    }
    else if(irectw>36&&irectw<=72)
    {
        igrid =72;
    }
    else if(irectw>72&&irectw<=144)
    {
        igrid =144;
    }

    return igrid;
}
void fastmatch::MatchImageLearnEx(ImageBase &aimage,int ithre1,int iandor,int igrid)
{
    int isize = m_resultrects.size();
    if(isize<=0)
        return;

    QRect arect0 = m_resultrects.getrect(isize - 1);

   // int iw = arect0.width();
   // int ih = arect0.height();
   // int imaxlen = iw>ih?iw:ih;
   // {
        m_pgrid->SetUnit(igrid,igrid);//(12,12);
        QRect arect = m_pgrid->CentRect_Condition(arect0,3);
        aimage.SetROI(arect.x()-m_imatchoffset,arect.y(),arect.width(),arect.height());
        g_pmodelimage->SetROI(0,0,arect.width(),arect.height());
        aimage.SetMode(3);
        aimage.ROItoROI(g_pmodelimage);
        g_pmodelimage->ROIColorTable();
        g_pmodelimage->ROIColorTableBlur(0,ithre1);
        g_pmodelimage->ROIColorTableEasyThre(iandor);
        m_pgrid->ROIImagetoModel(*g_pmodelimage);
        m_pgrid->SetUnit(igrid,igrid);//(12,12);
        m_pgrid->UnitGrid();

        m_pgrid->ModelGridMethod_Gauss();
        m_imagefastmodel = m_pgrid->getfastmodel();
    //}
}
void fastmatch::MatchImageLearnMass(ImageBase &aimage,int ithre1,int iandor,int igrid)
{
    int isize = m_resultrects.size();
    if(isize<=0)
        return;
    m_pgrid->setgrid(10,10,igrid,igrid,10,10);
    m_pgrid->SetModelWH(igrid,igrid);
 /*   QRect arect0 = m_resultrects.getrect(isize - 1);
    //QRect arect = m_pgrid->CentRect(arect0);
    m_pgrid->SetUnit(igridwh,igridwh);
    QRect arect = m_pgrid->CentRect_Condition(arect0,3);

    aimage.SetROI(arect.x()-m_imatchoffset,arect.y(),arect.width(),arect.height());
    g_pmodelimage->SetROI(0,0,arect.width(),arect.height());
    aimage.SetMode(3);
    aimage.ROItoROI(g_pmodelimage);
    g_pmodelimage->ROIColorTable();
    g_pmodelimage->ROIColorTableBlur(0,ithre1);
    g_pmodelimage->ROIColorTableEasyThre(iandor);
    m_pgrid->ROIImagetoModel(*g_pmodelimage);
    m_pgrid->SetUnit(igridwh,igridwh);
    m_pgrid->UnitGrid();

    m_pgrid->ModelGridMethod_Gauss();
    m_imagefastmodel = m_pgrid->getfastmodel();
    */

    QRect arect0 = m_resultrects.getrect(isize - 1);

    m_pgrid->SetUnit(igrid,igrid);//(12,12);
    QRect arect = m_pgrid->CentRect_Condition(arect0,3);
    aimage.SetROI(arect.x()-m_imatchoffset,arect.y(),arect.width(),arect.height());
    g_pmodelimage->SetROI(0,0,arect.width(),arect.height());
    aimage.SetMode(3);//linux 0 ,win 3
    aimage.ROItoROI(g_pmodelimage);

    g_pmodelimage->ROIColorTable();
    g_pmodelimage->ROIColorTableBlur(0,ithre1);
    g_pmodelimage->ROIColorTableEasyThre(iandor);

    m_pgrid->ROIImagetoModel(*g_pmodelimage);
    m_pgrid->SetUnit(igrid,igrid);//(12,12);
    m_pgrid->UnitGrid();

    m_pgrid->ModelGridMethod_Gauss();
    m_imagefastmodel = m_pgrid->getfastmodel();

}
void fastmatch::MatchImageCheck(ImageBase &aimage,int iimagetype,int iandor,int igrid)
{
    int isize = m_resultrects.size();
    if(isize<=0)
        return;

    m_pgrid->setgrid(10,10,igrid,igrid,10,10);
    m_pgrid->SetModelWH(igrid,igrid);

    QRect arect0 = m_resultrects.getrect(isize - 1);

    m_pgrid->SetUnit(igrid,igrid);

    //zero image roi
    g_pmodelimage->SetROI(0,0,igrid,igrid);
    g_pmodelimage->ROIImageClear(0);

    aimage.SetROI(arect0.x()-m_imatchoffset,arect0.y(),arect0.width(),arect0.height());
    g_pmodelimage->SetROI(0,0,arect0.width(),arect0.height());
    aimage.SetMode(iimagetype);//linux 0 ,win 30
    aimage.ROItoROI(g_pmodelimage);

    g_pmodelimage->SetROI(0,0,igrid,igrid);
    m_pgrid->ROIImagetoModel(*g_pmodelimage);

//    m_pgrid->ROIImagetoModel_gray(*g_pmodelimage);
//    m_pgrid->SetUnit(igrid,igrid);//(12,12);
//    m_pgrid->EdgeGrid();
//    amatch.setthre(21);
//    amatch.setcompgap(1);
//    amatch.setmethod(0);
//    amatch.setlinegap(3);
//    amatch.setwhgap(1,1);
//    amatch.setlinesample(0.004);
//    amatch.setfilter(21,0,10000);
//    amatch.learn(aimage3);
//    amatch.savemodel("ready.pat");
//here !!!
//    !!!
//    m_pgrid->ModelGridMethod_Gauss();
//    m_imagefastmodel = m_pgrid->getfastmodel();

    m_imagefastmatchlist = m_pgrid->getfastmodel();

    isize = m_imagefastmodel.size();
    int ingsize = 0;
    int ioksize = 0;
    for(int i=0;i<isize;i++)
    {
       int ia = m_imagefastmatchlist[i];
       int io = m_imagefastmodel[i];
       //
       if(ia!=io&&io==1)
       {
           if(ia>0)
                ingsize = ingsize + ia;
           else
                ingsize = ingsize - ia;
       }
       else if(ia==1&&io!=1)
       {
           if(io>0)
                ingsize = ingsize + io;
           else
                ingsize = ingsize - io;
       }
       else if(io==ia
               &&io==1)
           ioksize++;
    }
    m_imagemodelresult_OK = ioksize;
    m_imagemodelresult_NG = ingsize;
}

void fastmatch::imagematch(int ithre1,int iandor,int igrid,int ineedthre)
{
    MatchImageMatch(*m_matchimage,ithre1,iandor,igrid,ineedthre);
}

void fastmatch::imagematch_grid(int ithre1,int iandor,int igrid)
{
    MatchImageMatch(*m_matchimage,ithre1,iandor,igrid);
}

void fastmatch::imagematchex(int igrid)
{
    MatchImageExMatch(*m_matchimage ,igrid);
}

void fastmatch::MatchImageMatch(ImageBase &aimage,int ithre1,int iandor,int igrid,int ineedthre)
{
    int isize = m_resultrects.size();
    if(isize<=0)
        return;
    QRect arect0 = m_resultrects.getrect(isize - 1);
    //QRect arect = m_pgrid->CentRect(arect0);

    //QRect arect = m_pgrid->CentRect_Condition(arect0,3);
    aimage.SetROI(arect0.x()-m_imatchoffset,arect0.y(),arect0.width(),arect0.height());
    g_pmodelimage->SetROI(0,0,arect0.width(),arect0.height());
    aimage.SetMode(3);
    aimage.ROItoROI(g_pmodelimage);

    if(1==ineedthre)
    {
        g_pmodelimage->ROIColorTable();
        g_pmodelimage->ROIColorTableBlur(0,ithre1);
        g_pmodelimage->ROIColorTableEasyThre(iandor);
    }
    m_pgrid->ROIImagetoModel(*g_pmodelimage);
    m_pgrid->ZeroModel();
    m_pgrid->ReGrid(igrid,igrid);

    if(0)//test1
    {
        m_pgrid->SetUnit(igrid,igrid);
        m_pgrid->UnitGrid();
    }
    m_easyobject = m_pgrid->ModelGridMethod_ObjectA();

    m_pgrid->ModelGridMethod_Gauss();
    m_imagefastmatchlist = m_pgrid->getfastmodel();

    isize = m_imagefastmodel.size();
    int ingsize = 0;
    int ioksize = 0;
    for(int i=0;i<isize;i++)
    {
       int ia = m_imagefastmatchlist[i];
       int io = m_imagefastmodel[i];

       //
       if(ia!=io&&io==1)
       {
           if(ia>0)
                ingsize = ingsize + ia;
           else
                ingsize = ingsize - ia;
       }
       else if(ia==1&&io!=1)
       {
           if(io>0)
                ingsize = ingsize + io;
           else
                ingsize = ingsize - io;
       }
       else if(io==ia
               &&io==1)
           ioksize++;
    }
    m_imagemodelresult_OK = ioksize;
    m_imagemodelresult_NG = ingsize;
}

void fastmatch::MatchImageExMatch(ImageBase &aimage, int igrid)
{
    int isize = m_resultrects.size();
    if(isize<=0)
        return;
    QRect arect0 = m_resultrects.getrect(isize - 1);
    //QRect arect = m_pgrid->CentRect(arect0);
    m_pgrid->SetUnit(igrid,igrid);
    QRect arect;
    switch (igrid) {
    case 3:
        arect = QRect(arect0.x()+1,arect0.y()+1,3,3);
        break;
    case 6:
        arect = QRect(arect0.x()+1,arect0.y()+1,6,6);
        break;
    case 12:
        arect = m_pgrid->CentRect_Condition(arect0,3);
        break;
    default:
        break;
    }
    aimage.SetROI(arect.x()-m_imatchoffset,arect.y(),arect.width(),arect.height());
    g_pmodelimage->SetROI(0,0,arect.width(),arect.height());
    aimage.SetMode(3);
    aimage.ROItoROI(g_pmodelimage);
//    g_pmodelimage->ROIColorTable();
//    g_pmodelimage->ROIColorTableBlur(0,ithre1);
//    g_pmodelimage->ROIColorTableEasyThre(iandor);
    m_pgrid->ROIImagetoModel(*g_pmodelimage);

    m_easyobject = m_pgrid->ModelGridMethod_ObjectA();

    m_pgrid->ModelGridMethod_Gauss();
    m_imagefastmatchlist = m_pgrid->getfastmodel();

    isize = m_imagefastmodel.size();
    int ingsize = 0;
    int ioksize = 0;
    for(int i=0;i<isize;i++)
    {
       int ia = m_imagefastmatchlist[i];
       int io = m_imagefastmodel[i];

       //
       if(ia!=io&&io==1)
       {
           if(ia>0)
                ingsize = ingsize + ia;
           else
                ingsize = ingsize - ia;
       }
       else if(ia==1&&io!=1)
       {
           if(io>0)
                ingsize = ingsize + io;
           else
                ingsize = ingsize - io;
       }
       else if(io==ia
               &&io==1)
           ioksize++;
    }
    m_imagemodelresult_OK = ioksize;
    m_imagemodelresult_NG = ingsize;
}
void fastmatch::MatchGrid(QGrid *pgrid)
{
    m_pgrid->GridFastModel(pgrid);

    m_easyobject = m_pgrid->ModelGridMethod_ObjectA();

    m_pgrid->ModelGridMethod_Gauss();
    m_imagefastmatchlist = m_pgrid->getfastmodel();

    int isize = m_imagefastmodel.size();
    int ingsize = 0;
    int ioksize = 0;
    for(int i=0;i<isize;i++)
    {
       int ia = m_imagefastmatchlist[i];
       int io = m_imagefastmodel[i];

       //
       if(ia!=io&&io==1)
       {
           if(ia>0)
                ingsize = ingsize + ia;
           else
                ingsize = ingsize - ia;
       }
       else if(ia==1&&io!=1)
       {
           if(io>0)
                ingsize = ingsize + io;
           else
                ingsize = ingsize - io;
       }
       else if(io==ia
               &&io==1)
           ioksize++;
    }
    m_imagemodelresult_OK = ioksize;
    m_imagemodelresult_NG = ingsize;
}

double fastmatch::getimagemodelreslut()
{
    if(0==m_imagemodelresult_NG
     &&0!=m_imagemodelresult_OK)
        return 100;
    if(0==m_imagemodelresult_OK
     &&0!=m_imagemodelresult_NG)
        return 0;
    if(0==m_imagemodelresult_OK
     &&0==m_imagemodelresult_NG)
        return 0;

    int itotalsize = m_imagefastmodel.size();
    double dresult = (m_imagemodelresult_OK*1.0)/(1.0*m_imagemodelresult_NG);
    return dresult;
}

double fastmatch::getimagemodelreslut_check_1()
{
    if(0==m_imagemodelresult_NG
     &&0!=m_imagemodelresult_OK)
        return 100;
    if(0==m_imagemodelresult_OK
     &&0!=m_imagemodelresult_NG)
        return 0;
    if(0==m_imagemodelresult_OK
     &&0==m_imagemodelresult_NG)
        return 0;

    int itotalsize = m_imagefastmodel.size();
    double dresult = (m_imagemodelresult_OK*1.0)/(1.0*m_imagemodelresult_NG);
    return dresult;
}
void fastmatch::imagemodelcomparegrid(int itype)
{
    int imatchsize = m_imagefastmatchlist.size();
    if(imatchsize<=0)
        return;
    int isize = m_imagefastmodel.size();
    if(imatchsize!=isize)
        return;

    switch (itype)
    {
    case 0:
        for(int i=0;i<isize;i++)
        {
           int ia = m_imagefastmatchlist[i];
           int io = m_imagefastmodel[i];

           if(io==1&&ia!=io)
           {
              m_pgrid->setfastlistvalue(i,-1);
           }
           else if(ia==1&&io!=1)
           {
               m_pgrid->setfastlistvalue(i,-2);
           }
           else if(io==1&&ia!=1)
           {
               m_pgrid->setfastlistvalue(i,-3);
           }
           else if(io==1&&io==ia)
              m_pgrid->setfastlistvalue(i,1);
           else
              m_pgrid->setfastlistvalue(i,0);
        }

        break;
    case 1:
        for(int i=0;i<isize;i++)
        {
           int ia = m_imagefastmatchlist[i];
           int io = m_imagefastmodel[i];

           if(io==0&&ia!=io)
              m_pgrid->setfastlistvalue(i,-1);
           else if(ia==1&&io!=0)
               m_pgrid->setfastlistvalue(i,0);
           else if(ia==1&&io==0)
               m_pgrid->setfastlistvalue(i,-1);
           else if(ia==0&&io!=0)
           {
               if(io==-1)
                    m_pgrid->setfastlistvalue(i,0);
                else
                    m_pgrid->setfastlistvalue(i,io);
           }
           else
              m_pgrid->setfastlistvalue(i,0);
        }

        break;
    case 2:

        break;


    default:

        break;
    }


}
void fastmatch::imagemodelcompareshow(int itype)
{
    int imatchsize = m_imagefastmatchlist.size();
    if(imatchsize<=0)
        return;
    int isize = m_imagefastmodel.size();
    if(imatchsize!=isize)
        return;
    switch (itype)
    {
    case 0:
        for(int i=0;i<isize;i++)
        {
           int ia = m_imagefastmatchlist[i];
           int io = m_imagefastmodel[i];

           if(io==1&&ia!=io)
           {
              m_pgrid->setfastlistvalue(i,-1);
           }
           else if(ia==1&&io!=1)
           {
               m_pgrid->setfastlistvalue(i,-2);
           }
           else if(io==1&&ia!=1)
           {
               m_pgrid->setfastlistvalue(i,-3);
           }
           else if(io==1&&io==ia)
              m_pgrid->setfastlistvalue(i,1);
           else
              m_pgrid->setfastlistvalue(i,0);
        }

        break;
    case 1:
        for(int i=0;i<isize;i++)
        {
           int ia = m_imagefastmatchlist[i];
           int io = m_imagefastmodel[i];

           if(io==0&&ia!=io)
              m_pgrid->setfastlistvalue(i,-1);
           else if(ia==1&&io!=0)
               m_pgrid->setfastlistvalue(i,0);
           else if(ia==1&&io==0)
               m_pgrid->setfastlistvalue(i,-1);
           else if(ia==0&&io!=0)
           {
               if(io==-1)
                    m_pgrid->setfastlistvalue(i,0);
                else
                    m_pgrid->setfastlistvalue(i,io);
           }
           else
              m_pgrid->setfastlistvalue(i,0);
        }

        break;
    case 2:

        break;


    default:

        break;
    }
}
double fastmatch::imagegridresult(int itype)
{
    switch (itype) {
    case 0:

        break;
    case 1:

        break;


    default:

        break;
    }
}
void fastmatch::imagemodelshow()
{
    m_pgrid->SetFastModel(m_imagefastmodel);
    int isize = m_imagefastmodel.size();
    if(isize<=0)
        return;
    for(int i=0;i<isize;i++)
    {
       int io = m_imagefastmodel[i];
       m_pgrid->setfastlistvalue(i,io);
    }
}

void fastmatch::imagematchshow()
{
    m_pgrid->SetFastModel(m_imagefastmodel);
    int imatchsize = m_imagefastmatchlist.size();
    if(imatchsize<=0)
        return;
    int isize = m_imagefastmodel.size();
    if(isize!=imatchsize)
        return;
    for(int i=0;i<isize;i++)
    {
       int io = m_imagefastmatchlist[i];
       m_pgrid->setfastlistvalue(i,io);
    }
}
void fastmatch::setminscore(double dminscore)
{
    m_dminscore = dminscore;
}
void fastmatch::MatchSample(ImageBase &image,QPainterPath &path)
{
    int ithre = m_imatchthre;
    int icurmodule = BackImageManager::GetCurMode();
    ImageBase *pimage = BackImageManager::GetTransferImage(icurmodule);
    int ix0 = m_matchrect.x();
    int iy0 = m_matchrect.y();
    int ix1 = m_matchrect.x() + m_matchrect.width();
    int iy1 = m_matchrect.y() + m_matchrect.height();

    if(image.width()<= ix1
       || image.height() <= iy1)
        return;//error process
    m_iminfindnum = -1;
    int icount = path.elementCount();
    QRgb pixel0,pixel1;
    int icalnum = 0;
    int icalng = 0;

    QPainterPath::Element aele;

    int igapx =1;
    int igapy =1;
    QRectF arect1 = path.boundingRect();
    iy1 = iy1 - arect1.height();
    ix1 = ix1 - arect1.width();
    int ix =0;
    int iy =0;
    int iw = findline::patternboundingrect().width();
    int ih = findline::patternboundingrect().height();
    int itotalsize = findline::getpattern().size();

    if(m_dminscore>1&&m_dminscore<0)
        m_dminscore = 0.4;
    int iminfindngnum = (1-m_dminscore) *itotalsize/2;
    int iminfindoknum =  m_dminscore *itotalsize/2;



    for(iy=iy0;iy<iy1;)
    {
        for(ix=ix0;ix<ix1;)
        {
            int imovx =ix;
            int imovy =iy;

            icalnum = 0;
            icalng = 0;
            for(int i=0;i<icount-1;i++)
            {
                aele = path.elementAt(i);
                pixel0 = image.pixel(aele.x+imovx,aele.y+imovy);
                i++;
                aele = path.elementAt(i);
                pixel1 = image.pixel(aele.x+imovx,aele.y+imovy);

                if(0==m_iB2W)
                {
                    int ir = qRed(pixel0) - qRed(pixel1);
                    int ig = qGreen(pixel0) - qGreen(pixel1);
                    int ib = qBlue(pixel0) - qBlue(pixel1);
                    if(ir>ithre||ig>ithre||ib>ithre)
                    {
                        icalnum++;
                    }
                    else
                    {
                        icalng++;
                        if(icalng>iminfindngnum)
                            goto NextStep_1;
                    }
                }
                else if(1==m_iB2W)
                {
                    int ir = qRed(pixel1) - qRed(pixel0);
                    int ig = qGreen(pixel1) - qGreen(pixel0);
                    int ib = qBlue(pixel1) - qBlue(pixel0);
                    if(ir>ithre||ig>ithre||ib>ithre)
                    {
                        icalnum++;
                    }
                    else
                    {
                        icalng++;

                        if(icalng>iminfindngnum)
                            goto NextStep_1;
                    }
                }
            }

NextStep_1:
            ix += igapx;
            if(icalnum>m_iminfindnum
                    &&icalnum>iminfindoknum)
            {
                QPoint apoint(ix,iy);
                resulttolist(apoint,icalnum);
            }

        }
        iy += igapy;
    }

    resultsort();
    m_resultrects.clear();
    int icountresult = m_reslutnums.size();
    for(int i=0;i<icountresult;i++)
    {
        int ivalue =  m_reslutnums.at(i);
        QPoint apoint = m_reslutpoints.at(i);
        double dpercent = (2.0*ivalue) /(1.0 *itotalsize);
        QRect arect(apoint.x(),apoint.y(),iw,ih);
        QString astr = QString("%1").arg(dpercent);
        m_resultrects.addrect(arect,astr);
    }

}


void fastmatch::MultiMatch(ImageBase &image)
{
    resultclear();
    int isize = m_models_l12.size();
    for(int i=0;i<isize;i++)
    {
        modelstocurrent_l12(i);
        QPainterPath &path = findline::getpatternpath();
        MultiMatchSample(image,path);
    }
}
void fastmatch::multimatch(void *pimage)
{
    ImageBase *pgetimage = (ImageBase*)pimage;
    MultiMatch(*pgetimage);
}
void fastmatch::MultiMatchSample(ImageBase &image,QPainterPath &path)
{
    int ithre = m_imatchthre;
    int icurmodule = BackImageManager::GetCurMode();
    ImageBase *pimage = BackImageManager::GetTransferImage(icurmodule);

    int imatchnum = m_matchrects.size();
    for(int ik=0;ik<imatchnum;ik++)
    {
        int ix0 = m_matchrects.getrect(ik).x();
        int iy0 = m_matchrects.getrect(ik).y();
        int ix1 = m_matchrects.getrect(ik).x() + m_matchrects.getrect(ik).width();
        int iy1 = m_matchrects.getrect(ik).y() + m_matchrects.getrect(ik).height();

        if(image.width()< ix1
           || image.height() < iy1)
            return;//error process

        m_iminfindnum = -1;
        int icount = path.elementCount();

        QRgb pixel0,pixel1;

        int icalnum = 0;
        int icalng = 0;
        QPainterPath::Element aele;

        int igapx =1;
        int igapy =1;
        QRectF arect1 = path.boundingRect();
        iy1 = iy1 - arect1.height();
        ix1 = ix1 - arect1.width();
        //g_pmodelimage->ImageClear(0);
        int ix =0;
        int iy =0;

        int iw = findline::patternboundingrect().width();
        int ih = findline::patternboundingrect().height();
        int itotalsize = findline::getpattern().size();

        if(m_dminscore>1&&m_dminscore<0)
            m_dminscore = 0.4;
        int iminfindngnum = (1-m_dminscore) *itotalsize;
        int iminfindoknum =  m_dminscore *itotalsize;



        for(iy=iy0;iy<iy1;)
        {
            for(ix=ix0;ix<ix1;)
            {
                int imovx =ix;
                int imovy =iy;

                icalnum = 0;
                for(int i=0;i<icount-1;i++)
                {
                    aele = path.elementAt(i);
                    pixel0 = image.pixel(aele.x+imovx,aele.y+imovy);
                    i++;
                    aele = path.elementAt(i);
                    pixel1 = image.pixel(aele.x+imovx,aele.y+imovy);
                    //iresult =  qGray(pixel0) - qGray(pixel1)  ;

                    if(0==m_iB2W)
                    {
                        int ir = qRed(pixel0) - qRed(pixel1);
                        int ig = qGreen(pixel0) - qGreen(pixel1);
                        int ib = qBlue(pixel0) - qBlue(pixel1);
                        if(ir>ithre||ig>ithre||ib>ithre)
                        {
                            icalnum++;
                        }
                        else
                        {
                            icalng++;
                            if(icalng>iminfindngnum)
                                goto NextStep_2;
                        }
                    }
                    else if(1==m_iB2W)
                    {
                        int ir = qRed(pixel1) - qRed(pixel0);
                        int ig = qGreen(pixel1) - qGreen(pixel0);
                        int ib = qBlue(pixel1) - qBlue(pixel0);
                        if(ir>ithre||ig>ithre||ib>ithre)
                        {
                            icalnum++;
                        }
                        else
                        {
                            icalng++;
                            if(icalng>iminfindngnum)
                                goto NextStep_2;
                        }
                    }
                }
NextStep_2:
                ix += igapx;

                if(icalnum>m_iminfindnum
                        &&icalnum>iminfindoknum)
                {
                    QPoint apoint(ix,iy);
                    resulttolist(apoint,icalnum);
                }

            }
            iy += igapy;
        }

        resultsort();
        m_resultrects.clear();

        int icountresult = m_reslutnums.size();
        for(int i=0;i<icountresult;i++)
        {
            int ivalue =  m_reslutnums.at(i);
            QPoint apoint = m_reslutpoints.at(i);
             double dpercent = (2.0*ivalue) /(1.0 *itotalsize);
            QRect arect(apoint.x(),apoint.y(),iw,ih);
            QString astr = QString("%1").arg(dpercent);
            m_resultrects.addrect(arect,astr);
        }

    }

}
void fastmatch::RotateMatch(ImageBase &image)
{
    m_matchimage = &image;
    resultclear();
    m_rotateshaperesults.clear();
    m_rotatereslutpoints.clear();
    m_rotateresults.clear();
    m_rotatereslutangles.clear();
    m_clusters.clear();

    int isize = 360/m_danglegap;
    int icurangle =0;
    for(int i=0;i<isize;i++)
    {
        resultclear();
        QPainterPath &path = m_models_rotate[icurangle].getpath();
        PointsShape &arectpoints=m_models_rotaterects[icurangle];
        RotateMatchSample(image,path,arectpoints,icurangle);
        icurangle = icurangle + m_danglegap;
    }
    resultcluster(m_ixclustergap,m_iyclustergap,m_iangleclustergap);
}
void fastmatch::setclustergap(int ixclustergap,int iyclustergap,int iangleclustergap)
{
    m_ixclustergap = ixclustergap;
    m_iyclustergap = iyclustergap;
    iangleclustergap = m_iangleclustergap;
}
void fastmatch::rotatematch(void *pimage)
{
    ImageBase *pgetimage = (ImageBase*)pimage;
    RotateMatch(*pgetimage);
}
void fastmatch::RotateMatchSample(ImageBase &image,QPainterPath &path,PointsShape &modelrect,double dangle)
{
    int ithre = m_imatchthre;
    int icurmodule = BackImageManager::GetCurMode();
    ImageBase *pimage = BackImageManager::GetTransferImage(icurmodule);
    int ix0 = m_matchrect.x();
    int iy0 = m_matchrect.y();
    int ix1 = m_matchrect.x() + m_matchrect.width();
    int iy1 = m_matchrect.y() + m_matchrect.height();

    if(image.width()<= ix1
       || image.height() <= iy1)
        return;//error process
    m_iminfindnum = -1;
    int icount = path.elementCount();
    QRgb pixel0,pixel1;
    int icalnum = 0;
    int icalng = 0;

    QPainterPath::Element aele;

    int igapx =1;
    int igapy =1;
    QRectF arect1 = path.boundingRect();
    iy1 = iy1 - arect1.height();
    ix1 = ix1 - arect1.width();
    int ix =0;
    int iy =0;
    int iw = findline::patternboundingrect().width();
    int ih = findline::patternboundingrect().height();
    int itotalsize = findline::getpattern().size();

    if(m_dminscore>1&&m_dminscore<0)
        m_dminscore = 0.4;
    int iminfindngnum = (1-m_dminscore) *itotalsize/2;
    int iminfindoknum =  m_dminscore *itotalsize/2;



    for(iy=iy0;iy<iy1;)
    {
        for(ix=ix0;ix<ix1;)
        {
            int imovx =ix;
            int imovy =iy;

            icalnum = 0;
            icalng = 0;
            for(int i=0;i<icount-1;i++)
            {
                aele = path.elementAt(i);
                pixel0 = image.pixel(aele.x+imovx,aele.y+imovy);
                i++;
                aele = path.elementAt(i);
                pixel1 = image.pixel(aele.x+imovx,aele.y+imovy);

                if(0==m_iB2W)
                {
                    int ir = qRed(pixel0) - qRed(pixel1);
                    int ig = qGreen(pixel0) - qGreen(pixel1);
                    int ib = qBlue(pixel0) - qBlue(pixel1);
                    if(ir>ithre||ig>ithre||ib>ithre)
                    {
                        icalnum++;
                    }
                    else
                    {
                        icalng++;
                        if(icalng>iminfindngnum)
                            goto NextStep_1;
                    }
                }
                else if(1==m_iB2W)
                {
                    int ir = qRed(pixel1) - qRed(pixel0);
                    int ig = qGreen(pixel1) - qGreen(pixel0);
                    int ib = qBlue(pixel1) - qBlue(pixel0);
                    if(ir>ithre||ig>ithre||ib>ithre)
                    {
                        icalnum++;
                    }
                    else
                    {
                        icalng++;

                        if(icalng>iminfindngnum)
                            goto NextStep_1;
                    }
                }
            }

NextStep_1:
            ix += igapx;
            if(icalnum>m_iminfindnum
                    &&icalnum>iminfindoknum)
            {
                QPoint apoint(ix,iy);
                resulttolist(apoint,icalnum);
            }

        }
        iy += igapy;
    }

    resultsort();
    m_resultrects.clear();
    int icountresult = m_reslutnums.size();
    for(int i=0;i<icountresult;i++)
    {
        int ivalue =  m_reslutnums.at(i);
        QPoint apoint = m_reslutpoints.at(i);
        double dpercent = (2.0*ivalue) /(1.0 *itotalsize);
        QRect arect(apoint.x(),apoint.y(),iw,ih);
        QString astr = QString("%1").arg(dpercent);
        m_resultrects.addrect(arect,astr);
        PointsShape bmodelrect = modelrect;
        QFont afont("Fixedsys", 8);
        bmodelrect.addText(0,0,afont,astr);
        bmodelrect.Move(apoint.x(),apoint.y());
        m_rotateshaperesults.append(bmodelrect);
        m_rotatereslutpoints.append(apoint);
        m_rotateresults.append(dpercent);
        m_rotatereslutangles.append(dangle);

    }


}


double fastmatch::getresultnum(int inum)
{
    if(m_reslutnums.size()>inum)
        return m_reslutnums.at(inum);
    else
        return 0;
}
int fastmatch::getresultcentx(int inum)
{
    if(m_reslutpoints.size()>inum)
    {
        int iw = findline::patternboundingrect().width();
        return m_reslutpoints.at(inum).x()+(iw/2);
    }
    else
        return 0;
}
int fastmatch::getresultcenty(int inum)
{
    if(m_reslutpoints.size()>inum)
    {
        int ih = findline::patternboundingrect().height();
        return m_reslutpoints.at(inum).y()+(ih/2);

    }
    else
        return 0;
}
double fastmatch::getmaxresult()
{
    if(m_reslutnums.size()>0)
    {
        int itotalsize = findline::getpattern().size();
        double dpercent = (2.0*m_reslutnums.at(m_reslutnums.size()-1)) /(1.0 *itotalsize);
        return dpercent;
    }

    else
        return 0;

}
int fastmatch::geteasyobjectw()
{
    return m_easyobject.s_iwobjnum;
}
int fastmatch::geteasyobjectb()
{
    return m_easyobject.s_ibobjnum;
}

int fastmatch::getmodeleasyobjectw_l72(int i)
{
    return m_easyobjectmodels_l72[i].s_iwobjnum;
}
int fastmatch::getmodeleasyobjectb_l72(int i)
{
    return m_easyobjectmodels_l72[i].s_ibobjnum;
}


int fastmatch::getmodeleasyobjectw_l36(int i)
{
    return m_easyobjectmodels_l36[i].s_iwobjnum;
}
int fastmatch::getmodeleasyobjectb_l36(int i)
{
    return m_easyobjectmodels_l36[i].s_ibobjnum;
}

int fastmatch::getmodeleasyobjectw_l12(int i)
{
    return m_easyobjectmodels_l12[i].s_iwobjnum;
}
int fastmatch::getmodeleasyobjectb_l12(int i)
{
    return m_easyobjectmodels_l12[i].s_ibobjnum;
}

int fastmatch::getmodeleasyobjectw_l3(int i)
{
    return m_easyobjectmodels_l3[i].s_iwobjnum;
}
int fastmatch::getmodeleasyobjectb_l3(int i)
{
    return m_easyobjectmodels_l3[i].s_ibobjnum;
}
int fastmatch::getmodeleasyobjectw_l6(int i)
{
    return m_easyobjectmodels_l6[i].s_iwobjnum;
}
int fastmatch::getmodeleasyobjectb_l6(int i)
{
    return m_easyobjectmodels_l6[i].s_ibobjnum;
}


void fastmatch::setrelationrectfromresultnum(int inum)
{
    m_irelationresultnum = inum;
}
void fastmatch::setrelationrectfrom_matchresult(void *pmatch)
{
    m_prelationmatch = (fastmatch*)pmatch;
    if(0!=m_prelationmatch)
    {
        int inum = m_prelationmatch->m_resultrects.size();
        if(m_irelationresultnum<inum)
        {
            m_irelationrect = m_prelationmatch->m_resultrects.getrect(m_irelationresultnum);
        }
    }
}
void fastmatch::setrelationxy(int iprex1,int iprey1,int iendx1,int iendy1)
{
    m_irelationrect.setLeft(m_irelationrect.left() + iprex1);
    m_irelationrect.setTop(m_irelationrect.top() + iprey1);
    m_irelationrect.setRight(m_irelationrect.right() + iendx1);
    m_irelationrect.setBottom(m_irelationrect.bottom() + iendy1);


}
void fastmatch::setrelationzoom(double drelationzoomx,double drelationzoomy)
{
    m_irelationrect.setLeft((double)m_irelationrect.left() * drelationzoomx);
    m_irelationrect.setTop((double)m_irelationrect.top() * drelationzoomy);
    m_irelationrect.setRight((double)m_irelationrect.right() * drelationzoomx);
    m_irelationrect.setBottom((double)m_irelationrect.bottom() * drelationzoomy);
}
void fastmatch::setrelationtorect()
{
    if(m_irelationrect.x()>=0
            &&m_irelationrect.y()>=0
            &&m_irelationrect.width()>0
            &&m_irelationrect.height()>0)
     m_matchrect = m_irelationrect;
}
void fastmatch::shapesetroi(void *pshape)
{
   QShape::shapesetroi(pshape);
}
