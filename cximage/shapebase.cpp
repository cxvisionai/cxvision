#include "shapebase.h"
#include "backimagemanager.h"
#include <QPainter>

ShapeBase::ShapeBase():
    m_ishow(1),
    m_shapetype(ShapeType::Path),
    m_antialiased(false),
    m_transformed(false),
    m_pen(QPen(QColor(0,100,0))),
    m_brush(QBrush(QColor(0,250,0))),
    m_dminpercent(0.01),
    m_ishowlines(1)
{

}
ShapeBase::~ShapeBase()
{

}
int ShapeBase::show()
{
    return m_ishow;
}

void ShapeBase::setshow(int ishow)
{
    m_ishow = ishow;
}

//! [3]
void ShapeBase::setShape(int ashape)
{
    switch (ashape)
    {
        case 0: m_shapetype = ShapeBase::Line;break;
        case 1: m_shapetype = ShapeBase::Points;break;
        case 2: m_shapetype = ShapeBase::Polyline;break;
        case 3: m_shapetype = ShapeBase::Polygon;break;
        case 4: m_shapetype = ShapeBase::Rect;break;
        case 5: m_shapetype = ShapeBase::RoundedRect;break;
        case 6: m_shapetype = ShapeBase::Ellipse;break;
        case 7: m_shapetype = ShapeBase::Arc;break;
        case 8: m_shapetype = ShapeBase::Chord;break;
        case 9: m_shapetype = ShapeBase::Pie;break;
        case 10: m_shapetype = ShapeBase::Path;break;
        case 11: m_shapetype = ShapeBase::Text;break;
        case 12: m_shapetype = ShapeBase::Pixmap;break;
        default: m_shapetype = ShapeBase::Path;break;
    }
}

void ShapeBase::setpenw(int iw)
{
    m_pen.setWidthF(iw);
}
//! [3]

void ShapeBase::setPercent(double dvalue)
{
    m_dminpercent = dvalue;
}

//! [4]
void ShapeBase::setPen(int ir, int ig ,int ib)
{
    QPen apen(QColor(ir,ig,ib));
    m_pen = apen;
}
void ShapeBase::setPen(QColor qcolor)
{
    QPen apen(qcolor);
    m_pen = apen;
}
//! [4]

//! [5]
void ShapeBase::setBrush(int ir,int ig, int ib)
{
    QBrush abrush(QColor(ir,ig,ib));
    m_brush = abrush;
}
//! [5]

//! [6]
void ShapeBase::setAntialiased(int bantialiased)
{
    m_antialiased = bantialiased>0?true:false;
}
//! [6]

//! [7]
void ShapeBase::setTransformed(int btransformed)
{
    m_transformed = btransformed>0?true:false;
}
//

//
void ShapeBase::drawshape(QPainter &painter)
{
    static const QPoint points[4] = {
        QPoint(10, 80),
        QPoint(20, 10),
        QPoint(80, 30),
        QPoint(90, 70)
    };

    QRect rect(10, 20, 80, 60);

    QPainterPath path;
    path.moveTo(20, 80);
    path.lineTo(20, 30);
    path.cubicTo(80, 0, 50, 50, 80, 80);

    int startAngle = 20 * 16;
    int arcLength = 120 * 16;
//! [8]

//! [9]
    painter.setPen(m_pen);
    painter.setBrush(m_brush);
    if (m_antialiased)
        painter.setRenderHint(QPainter::Antialiasing, true);
//! [9]

//! [10]
    for (int x = 0; x <400; x += 100)
    {
        for (int y = 0; y < 400; y += 100)
        {
            painter.save();
            painter.translate(x, y);

            if (m_transformed)
            {
                painter.translate(50, 50);
                painter.rotate(60.0);
                painter.scale(0.6, 0.9);
                painter.translate(-50, -50);
            }


//! [12]
            switch (m_shapetype)
            {
                case Line:
                    painter.drawLine(rect.bottomLeft(), rect.topRight());
                    break;
                case Points:
                    painter.drawPoints(points, 4);
                    break;
                case Polyline:
                    painter.drawPolyline(points, 4);
                    break;
                case Polygon:
                    painter.drawPolygon(points, 4);
                    break;
                case Rect:
                    painter.drawRect(rect);
                    break;
                case RoundedRect:
                    painter.drawRoundedRect(rect, 25, 25, Qt::RelativeSize);
                    break;
                case Ellipse:
                    painter.drawEllipse(rect);
                    break;
                case Arc:
                    painter.drawArc(rect, startAngle, arcLength);
                    break;
                case Chord:
                    painter.drawChord(rect, startAngle, arcLength);
                    break;
                case Pie:
                    painter.drawPie(rect, startAngle, arcLength);
                    break;
                case Path:
                    if(1==m_ishow)
                        painter.drawPath(path);
                    else if(2==m_ishow)
                    {
                        int icount = 1/m_dminpercent ;
                        for(int i=0;i<icount;i++)
                        {
                           QPointF fpoint = path.pointAtPercent(m_dminpercent*i);
                            painter.drawPoint(fpoint);
                        }
                    }
                    else if(3==m_ishow)
                    {

                        int icount = 1/m_dminpercent ;
                        for(int i=0;i<icount;i++)
                        {
                           QPointF fpoint = path.pointAtPercent(m_dminpercent*i);
                            painter.drawPoint(fpoint);
                        }

                        painter.setPen(QPen(QColor(255,0,0)));
                        icount = path.elementCount();
                        for(int i=0;i<icount;i++)
                        {
                             QPainterPath::Element aele = path.elementAt(i);
                             painter.drawPoint(aele.x,aele.y);
                        }
                        painter.setPen(m_pen);
                    }
                    break;
                case Text:
                    painter.drawText(rect,
                                     Qt::AlignCenter,
                                     ("cxvision"));
                    break;
                case Pixmap:
                    painter.drawPixmap(10, 10, m_pixmap);
            }
//! [12] //! [13]
            painter.restore();
        }
    }

    painter.setRenderHint(QPainter::Antialiasing, false);
    //painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    //painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}


LineShape::LineShape()
{
    m_line.setLine(100,100,200,200);

   m_path.moveTo(m_line.x1(),m_line.y1());
   m_path.lineTo(m_line.x2(),m_line.y2());
}

void LineShape::setline(int ix0,int iy0,int ix1,int iy1)
{
    m_line.setLine(ix0,iy0,ix1,iy1);

    QPainterPath path;
    path.moveTo(m_line.x1(),m_line.y1());
    path.lineTo(m_line.x2(),m_line.y2());
    m_path = path;
}
/*
void LineShape::Move(int ix,int iy)
{
    m_line.setLine(m_line.x1()+ix,m_line.y1()+iy,m_line.x2()+ix,m_line.y2()+iy);
    for(int i=0;i<m_path.elementCount();i++)
    {
       QPointF apoint = m_path.elementAt(i);
       m_path.setElementPositionAt(i,apoint.x()+ix,apoint.y()+iy);
    }
}*/
LineShape::~LineShape()
{
}

void LineShape::copy(LineShape &aline)
{
    m_line = aline.m_line;
    m_path = aline.m_path;
}
LineShape& LineShape::operator=(const  LineShape& aline)
{
    m_line = aline.m_line;
    m_path = aline.m_path;
}
void LineShape::setshow(int ishow)
{
    ShapeBase::setshow(ishow);
}
void LineShape::setpenw(int iw)
{
    m_pen.setWidthF(iw);
}
void LineShape::drawshape(QPainter &painter)
{
//! [9]
    painter.setPen(m_pen);
    painter.setBrush(m_brush);
    if (m_antialiased)
        painter.setRenderHint(QPainter::Antialiasing, true);
//! [9]

//! [10]
            if (m_transformed)
            {
                painter.translate(50, 50);
                painter.rotate(60.0);
                painter.scale(0.6, 0.9);
                painter.translate(-50, -50);
            }
            if(1==m_ishow)
                painter.drawPath(m_path);
            else if(2==m_ishow)
            {
                int icount = 1/m_dminpercent ;
                for(int i=0;i<icount;i++)
                {
                   QPointF fpoint = m_path.pointAtPercent(m_dminpercent*i);
                    painter.drawPoint(fpoint);
                }
            }
            else if(3==m_ishow)
            {

                int icount = 1/m_dminpercent ;
                for(int i=0;i<icount;i++)
                {
                   QPointF fpoint = m_path.pointAtPercent(m_dminpercent*i);
                    painter.drawPoint(fpoint);
                }

                painter.setPen(QPen(QColor(255,0,0)));
                icount = m_path.elementCount();
                for(int i=0;i<icount;i++)
                {
                     QPainterPath::Element aele = m_path.elementAt(i);
                     painter.drawPoint(aele.x,aele.y);
                }
                painter.setPen(m_pen);
            }


 }

void LineShape::linecopy(QImage &srcImage,QImage &desImage)
{
    int icount = 1/m_dminpercent ;
    for(int i=0;i<icount;i++)
    {
       QPointF fpoint = m_path.pointAtPercent(m_dminpercent*i);
       desImage.setPixel(fpoint.x(),fpoint.y(),srcImage.pixel(fpoint.x(),fpoint.y()));
    }
}
void LineShape::linecopyex(QImage &srcImage,QImage &desImage,int ix,int iy)
{
    int icount = 1/m_dminpercent ;
    if(0==ix)
    {
        for(int i=0;i<icount;i++)
        {
           QPointF fpoint = m_path.pointAtPercent(m_dminpercent*i);
           desImage.setPixel(i,iy,srcImage.pixel(fpoint.x(),fpoint.y()));
        }
    }
    else if(0==iy)
    {
        for(int i=0;i<icount;i++)
        {
           QPointF fpoint = m_path.pointAtPercent(m_dminpercent*i);
           desImage.setPixel(ix,i,srcImage.pixel(fpoint.x(),fpoint.y()));
        }
    }
}
QPointF LineShape::getlinepoint(int inum)
{
    return m_path.pointAtPercent(m_dminpercent*inum);
}
int LineShape::getlinesize()
{
    int icount = 1/m_dminpercent ;
    return icount;
}
void LineShape::Move(int ix,int iy)
{
    int icurmodule = BackImageManager::GetCurMode();
    ImageBase *pimage = BackImageManager::GetTransferImage(icurmodule);

    m_path = pimage->ImageTransfer(m_path,ix,iy);
}
void LineShape::Rotate(double dangle)
{
    int icurmodule = BackImageManager::GetCurMode();
    ImageBase *pimage = BackImageManager::GetTransferImage(icurmodule);

    m_path = pimage->ImageTransfer(m_path,0,0,dangle);

}
void LineShape::Zoom(double dx0,double dy0)
{
    int icurmodule = BackImageManager::GetCurMode();
    ImageBase *pimage = BackImageManager::GetTransferImage(icurmodule);

    m_path = pimage->ImageTransfer(m_path,0,0,0.0,dx0,dy0);

}
void PointsShape::setshow(int ishow)
{
    ShapeBase::setshow(ishow);
}
void PointsShape::addapoint(double apointx,double apointy)
{
    if(m_path.elementCount()==0)
        m_path.moveTo(apointx,apointy);
    else
        m_path.lineTo(apointx,apointy);
}
void PointsShape::addpoint(qreal &apointx,qreal &apointy)
{
    if(m_path.elementCount()==0)
        m_path.moveTo(apointx,apointy);
    else
        m_path.lineTo(apointx,apointy);
}

void PointsShape::addpointa(qreal &apointx,qreal &apointy)
{
    m_path.moveTo(apointx,apointy);
}
void PointsShape::addpointb(qreal &apointx,qreal &apointy)
{
    m_path.lineTo(apointx,apointy);
}
QPainterPath &PointsShape::getpath()
{
    return m_path;
}

void PointsShape::addpoints(PointsShape &points)
{
    if(points.size())
    m_path.addPath(points.getpath());
}
QRectF PointsShape::boundingRect()
{
    return m_path.boundingRect();
}
QRectF  PointsShape::controlPointRect()
{
    return m_path.controlPointRect();
}
void PointsShape::addpoint(QPoint &apoint)
{
    if(m_path.elementCount()==0)
        m_path.moveTo(apoint.x(),apoint.y());
    else
        m_path.lineTo(apoint.x(),apoint.y());
}
void PointsShape::addpoint(QPointF &apoint)
{
    if(m_path.elementCount()==0)
        m_path.moveTo(apoint.x(),apoint.y());
    else
        m_path.lineTo(apoint.x(),apoint.y());
}
void PointsShape::addText(qreal x, qreal y,QFont &f,QString &text)
{
    m_path.addText(x, y, f, text);
}

void PointsShape::clear()
{
    QPainterPath path;
    m_path = path;
}
int PointsShape::size()
{
    return m_path.elementCount();
}
void PointsShape::drawshape(QPainter &painter)
{
//! [9]
    painter.setPen(m_pen);
    painter.setBrush(m_brush);
//    if (m_antialiased)
//        painter.setRenderHint(QPainter::Antialiasing, true);
//! [9]

//! [10]
//            if (m_transformed)
//            {
//                painter.translate(50, 50);
//                painter.rotate(60.0);
//                painter.scale(0.6, 0.9);
//                painter.translate(-50, -50);
//            }

            if(1==m_ishow)
            {// draw dot
                int icount = m_path.elementCount();
                for(int i=0;i<icount;i++)
                {
                     QPainterPath::Element aele = m_path.elementAt(i);
                     painter.drawPoint(aele.x,aele.y);
                }
            }
            else if(2==m_ishow)
            {// draw x
                int icount = m_path.elementCount();
                for(int i=0;i<icount;i++)
                {
                     QPainterPath::Element aele = m_path.elementAt(i);
                     painter.drawLine(aele.x-3,aele.y-3,aele.x+3,aele.y+3);
                     painter.drawLine(aele.x-3,aele.y+3,aele.x+3,aele.y-3);
                }
            }
            else if(16==m_ishow)
            {//四边形
                painter.setPen(QPen(QColor(0,255,0)));
                int icount = m_path.elementCount();
                for(int i=0;i<icount-4;i=i+4)
                {
                     QPainterPath::Element aele1 = m_path.elementAt(i);
                     QPainterPath::Element aele2 = m_path.elementAt(i+1);
                     QPainterPath::Element aele3 = m_path.elementAt(i+2);
                     QPainterPath::Element aele4 = m_path.elementAt(i+3);
                     painter.drawLine(aele1.x,aele1.y,aele2.x,aele2.y);
                     painter.drawLine(aele2.x,aele2.y,aele3.x,aele3.y);
                     painter.drawLine(aele3.x,aele3.y,aele4.x,aele4.y);
                     painter.drawLine(aele4.x,aele4.y,aele1.x,aele1.y);
                 }
            }
            else if(3==m_ishow)
                painter.drawPath(m_path);
            else if(4==m_ishow)
            {
                int icount = 1/m_dminpercent ;
                for(int i=0;i<icount;i++)
                {
                   QPointF fpoint = m_path.pointAtPercent(m_dminpercent*i);
                    painter.drawPoint(fpoint);
                }

                painter.setPen(QPen(QColor(255,0,0)));
                icount = m_path.elementCount();
                for(int i=0;i<icount;i++)
                {
                     QPainterPath::Element aele = m_path.elementAt(i);
                     painter.drawLine(aele.x-3,aele.y-3,aele.x+3,aele.y+3);
                     painter.drawLine(aele.x-3,aele.y+3,aele.x+3,aele.y-3);
                     //painter.drawPoint(aele.x,aele.y);
                }
                painter.setPen(m_pen);
            }           
            else if(8==m_ishow)
            {
               // int icount = 1/m_dminpercent ;
              //  for(int i=0;i<icount;i++)
               // {
               //    QPointF fpoint = m_path.pointAtPercent(m_dminpercent*i);
               //     painter.drawPoint(fpoint);
               // }
                painter.save();
               // painter.setPen(QPen(QColor(25,155,25)));
               // painter.drawPath(m_path);

                int icount = m_path.elementCount();

                for(int i=0;i<icount-1;i++)
                {
                    QPainterPath::Element aele = m_path.elementAt(i);

                    painter.setPen(QPen(QColor(255,0,0)));
                    painter.drawPoint(aele);
                    i++;
                    aele = m_path.elementAt(i);
                    painter.setPen(QPen(QColor(0,0,255)));
                    painter.drawPoint(aele);
                }
               painter.restore();
            }
 }
void PointsShape::drawshapex(QPainter &painter,
                             double dmovx,
                             double dmovy,
                             double dangle,
                             double dzoomx,
                             double dzoomy)
{
//! [9]
    painter.setPen(m_pen);
    painter.setBrush(m_brush);

            if(1==m_ishow)
            {// draw dot
                int icount = m_path.elementCount();
                for(int i=0;i<icount;i++)
                {
                     QPainterPath::Element aele = m_path.elementAt(i);


                     double dvaluex = aele.x;
                     double dvaluey = aele.y;
                     dvaluex = dvaluex*dzoomx+dmovx;
                     dvaluey = dvaluey*dzoomy+dmovy;

                     painter.drawPoint(dvaluex,dvaluey);
                }
            }
            else if(2==m_ishow)
            {// draw x
                int icount = m_path.elementCount();
                for(int i=0;i<icount;i++)
                {
                     QPainterPath::Element aele = m_path.elementAt(i);

                     double dvaluex = aele.x;
                     double dvaluey = aele.y;
                     dvaluex = dvaluex*dzoomx+dmovx;
                     dvaluey = dvaluey*dzoomy+dmovy;
                     painter.drawLine(dvaluex-3,dvaluey-3,dvaluex+3,dvaluey+3);
                     painter.drawLine(dvaluex-3,dvaluey+3,dvaluex+3,dvaluey-3);
                }
            }
            else if(16==m_ishow)
            {//四边形
                painter.setPen(QPen(QColor(0,255,0)));
                int icount = m_path.elementCount();
                for(int i=0;i<icount-4;i=i+4)
                {
                     QPainterPath::Element aele1 = m_path.elementAt(i);
                     QPainterPath::Element aele2 = m_path.elementAt(i+1);
                     QPainterPath::Element aele3 = m_path.elementAt(i+2);
                     QPainterPath::Element aele4 = m_path.elementAt(i+3);
                     painter.drawLine(aele1.x,aele1.y,aele2.x,aele2.y);
                     painter.drawLine(aele2.x,aele2.y,aele3.x,aele3.y);
                     painter.drawLine(aele3.x,aele3.y,aele4.x,aele4.y);
                     painter.drawLine(aele4.x,aele4.y,aele1.x,aele1.y);
                 }
            }
            else if(3==m_ishow)
                painter.drawPath(m_path);
            else if(4==m_ishow)
            {
                int icount = 1/m_dminpercent ;
                for(int i=0;i<icount;i++)
                {
                   QPointF fpoint = m_path.pointAtPercent(m_dminpercent*i);
                    painter.drawPoint(fpoint);
                }

                painter.setPen(QPen(QColor(255,0,0)));
                icount = m_path.elementCount();
                for(int i=0;i<icount;i++)
                {
                     QPainterPath::Element aele = m_path.elementAt(i);
                     painter.drawLine(aele.x-3,aele.y-3,aele.x+3,aele.y+3);
                     painter.drawLine(aele.x-3,aele.y+3,aele.x+3,aele.y-3);
                     //painter.drawPoint(aele.x,aele.y);
                }
                painter.setPen(m_pen);
            }
            else if(8==m_ishow)
            {
                painter.save();

                int icount = m_path.elementCount();

                for(int i=0;i<icount-1;i++)
                {
                    QPainterPath::Element aele = m_path.elementAt(i);

                    double dvaluex = aele.x;
                    double dvaluey = aele.y;
                    dvaluex = dvaluex*dzoomx+dmovx;
                    dvaluey = dvaluey*dzoomy+dmovy;

                    painter.setPen(QPen(QColor(255,0,0)));
                    painter.drawPoint(QPointF(dvaluex,dvaluey));
                    i++;
                    aele = m_path.elementAt(i);

                    dvaluex = aele.x;
                    dvaluey = aele.y;
                    dvaluex = dvaluex*dzoomx+dmovx;
                    dvaluey = dvaluey*dzoomy+dmovy;

                    painter.setPen(QPen(QColor(0,0,255)));
                    painter.drawPoint(QPointF(dvaluex,dvaluey));
                }
               painter.restore();
            }
 }




void PointsShape::save(const char * pchar)
{

    int isize = m_path.elementCount();
    if(isize<=0)
        return;
    FILE    *rf = fopen(pchar, "w+");
   if ( rf == nullptr)
       return;
   rewind(rf);
   QPainterPath::Element aele = m_path.elementAt(0);
    int ix =aele.x;
    int iy =aele.y;
   fprintf(rf,"%d,",ix);
   fprintf(rf,"%d",iy);

   for(int i=1;i<isize;i++)
   {
       aele = m_path.elementAt(i);
       ix =aele.x;
       iy =aele.y;
       fprintf(rf,",");
       fprintf(rf,"%d,",ix);
       fprintf(rf,"%d",iy);
   }

   fclose(rf);
}
void PointsShape::load(const char * pchar)
{
    clear();
    FILE    *rf = fopen(pchar, "rb");
    if(nullptr==rf)
        return;
    fseek(rf,0,SEEK_END);
    int filesize = ftell(rf);
    char *pcharget =new char[filesize+10];
    memset(pcharget,0,filesize+10);
    rewind(rf);
    fread((char*)(pcharget),filesize,1,rf);


    QString qstr = pcharget;
    QStringList strnumlist = qstr.split(",");
    for(int i=0;i<strnumlist.size()-3;i++)
    {
        qreal ix = strnumlist.at(i).toInt();
        i++;
        qreal iy = strnumlist.at(i).toInt();
        addpointa(ix,iy);
        i++;
        ix = strnumlist.at(i).toInt();
        i++;
        iy = strnumlist.at(i).toInt();
        addpointb(ix,iy);
    }


    delete []pcharget;
        fclose(rf);
}



void PointsShape::patterntokeys(QList <QPoint> &keypointsA,QList <QPoint> &keypointsA_,
                            QList <QPoint> &keypointsB,QList <QPoint> &keypointsB_,
                            int ibackgroundtype)
{//ibackgroundtype=0无背景=1黑背景=2白背景
    int icount = m_path.elementCount();
    for(int i=0;i<icount;)
    {
         QPainterPath::Element aele0 = m_path.elementAt(i);
         i++;
         QPainterPath::Element aele1 = m_path.elementAt(i);
         i++;
         qreal ix0 = aele0.x;
         qreal iy0 = aele0.y;
         qreal ix1 = aele1.x;
         qreal iy1 = aele1.y;

         qreal ix = aele0.x + aele1.x;
         qreal iy = aele0.y + aele1.y;
         if( ix0 == ix1 && iy0 > iy1 )
         {
             if(0==ibackgroundtype)
             keypointsA_.append(QPoint(ix/2,iy/2));
             else if(1==ibackgroundtype)
             keypointsA_.append(QPoint(ix0,iy0));
             else if(2==ibackgroundtype)
             keypointsA_.append(QPoint(ix1,iy1));
         }
         else if( ix0 == ix1 && iy0 < iy1 )
         {
             if(0==ibackgroundtype)
             keypointsA.append(QPoint(ix/2,iy/2));
             else if(1==ibackgroundtype)
             keypointsA.append(QPoint(ix1,iy1));
             else if(2==ibackgroundtype)
             keypointsA.append(QPoint(ix0,iy0));
         }
         else if(iy0 == iy1 && ix0 > ix1)
         {
             if(0==ibackgroundtype)
             keypointsB_.append(QPoint(ix/2,iy/2));
             else if(1==ibackgroundtype)
             keypointsB_.append(QPoint(ix1,iy1));
             else if(2==ibackgroundtype)
             keypointsB_.append(QPoint(ix0,iy0));
         }
         else if(iy0 == iy1 && ix0 < ix1)
         {
             if(0==ibackgroundtype)
             keypointsB.append(QPoint(ix/2,iy/2));
             else if(1==ibackgroundtype)
             keypointsB.append(QPoint(ix0,iy0));
             else if(2==ibackgroundtype)
             keypointsB.append(QPoint(ix1,iy1));
         }
    }

}
void PointsShape::keystopattern(QList <QPoint> &keypointsA,QList <QPoint> &keypointsA_,
                                QList <QPoint> &keypointsB,QList <QPoint> &keypointsB_,
                                int igap,int itype,int isgap,int iline)
{

    clear();
    int icount0 = keypointsA.size();
    int icount1 = keypointsA_.size();
    int icount2 =  keypointsB.size();
    int icount3 =  keypointsB_.size();
    if(1==itype)
    {
        for(int i=0;i<icount0;i++)
        {
            QPoint apoint = keypointsA.at(i);
            qreal ix0 = apoint.x();
            qreal iy0 = apoint.y()-igap;
            qreal ix1 = apoint.x();
            qreal iy1 = apoint.y()+igap;
            addpointa(ix0,iy0);
            addpointb(ix1,iy1);
        }
        for(int i=0;i<icount1;i++)
        {
            QPoint apoint = keypointsA_.at(i);
            qreal ix0 = apoint.x();
            qreal iy0 = apoint.y()+igap;
            qreal ix1 = apoint.x();
            qreal iy1 = apoint.y()-igap;
            addpointa(ix0,iy0);
            addpointb(ix1,iy1);
        }
        for(int i=0;i<icount2;i++)
        {
            QPoint apoint = keypointsB.at(i);
            qreal ix0 = apoint.x()-igap;
            qreal iy0 = apoint.y();
            qreal ix1 = apoint.x()+igap;
            qreal iy1 = apoint.y();
            addpointa(ix0,iy0);
            addpointb(ix1,iy1);
        }
        for(int i=0;i<icount3;i++)
        {
            QPoint apoint = keypointsB_.at(i);
            qreal ix0 = apoint.x()+igap;
            qreal iy0 = apoint.y();
            qreal ix1 = apoint.x()-igap;
            qreal iy1 = apoint.y();
            addpointa(ix0,iy0);
            addpointb(ix1,iy1);
        }
    }
    else if(2==itype)
    {
        for(int i=0;i<icount0;i++)
        {
            QPoint apoint = keypointsA.at(i);
            qreal ix0 = apoint.x();
            qreal iy0 = apoint.y()-igap;
            qreal ix1 = apoint.x();
            qreal iy1 = apoint.y()+igap;
            addpointa(ix0,iy0);
            addpointb(ix1,iy1);
            qreal ix2 = apoint.x();
            qreal iy2 = iy0-igap;
            qreal ix3 = apoint.x();
            qreal iy3 = iy1+igap;
            addpointa(ix2,iy2);
            addpointb(ix3,iy3);
        }
        for(int i=0;i<icount1;i++)
        {
            QPoint apoint = keypointsA_.at(i);
            qreal ix0 = apoint.x();
            qreal iy0 = apoint.y()+igap;
            qreal ix1 = apoint.x();
            qreal iy1 = apoint.y()-igap;
            addpointa(ix0,iy0);
            addpointb(ix1,iy1);
            qreal ix2 = apoint.x();
            qreal iy2 = iy0+igap;
            qreal ix3 = apoint.x();
            qreal iy3 = iy1-igap;
            addpointa(ix2,iy2);
            addpointb(ix3,iy3);
        }
        for(int i=0;i<icount2;i++)
        {
            QPoint apoint = keypointsB.at(i);
            qreal ix0 = apoint.x()-igap;
            qreal iy0 = apoint.y();
            qreal ix1 = apoint.x()+igap;
            qreal iy1 = apoint.y();
            addpointa(ix0,iy0);
            addpointb(ix1,iy1);
            qreal ix2 = ix0-igap;
            qreal iy2 = apoint.y();
            qreal ix3 = ix1+igap;
            qreal iy3 = apoint.y();
            addpointa(ix2,iy2);
            addpointb(ix3,iy3);
        }
        for(int i=0;i<icount3;i++)
        {
            QPoint apoint = keypointsB_.at(i);
            qreal ix0 = apoint.x()+igap;
            qreal iy0 = apoint.y();
            qreal ix1 = apoint.x()-igap;
            qreal iy1 = apoint.y();
            addpointa(ix0,iy0);
            addpointb(ix1,iy1);
            qreal ix2 = ix0+igap;
            qreal iy2 = apoint.y();
            qreal ix3 = ix1-igap;
            qreal iy3 = apoint.y();
            addpointa(ix2,iy2);
            addpointb(ix3,iy3);
        }
    }
    else if(100==itype)
    {
        for(int i=0;i<icount0;i++)
        {
            QPoint apoint = keypointsA.at(i);
            qreal ix0 = apoint.x();
            qreal iy0 = apoint.y()-igap;
            qreal ix1 = apoint.x();
            qreal iy1 = apoint.y()+isgap;//+igap;
            addpointa(ix0,iy0);
            addpointb(ix1,iy1);
            qreal ix2 = apoint.x();
            qreal iy2 = iy0-igap;
            qreal ix3 = apoint.x();
            qreal iy3 = iy1;//+igap;
            addpointa(ix2,iy2);
            addpointb(ix3,iy3);
        }
        for(int i=0;i<icount1;i++)
        {
            QPoint apoint = keypointsA_.at(i);
            qreal ix0 = apoint.x();
            qreal iy0 = apoint.y()+igap;
            qreal ix1 = apoint.x();
            qreal iy1 = apoint.y()-isgap;//-igap;
            addpointa(ix0,iy0);
            addpointb(ix1,iy1);
            qreal ix2 = apoint.x();
            qreal iy2 = iy0+igap;
            qreal ix3 = apoint.x();
            qreal iy3 = iy1;//-igap;
            addpointa(ix2,iy2);
            addpointb(ix3,iy3);
        }
        for(int i=0;i<icount2;i++)
        {
            QPoint apoint = keypointsB.at(i);
            qreal ix0 = apoint.x()-igap;
            qreal iy0 = apoint.y();
            qreal ix1 = apoint.x()+isgap;//+igap;
            qreal iy1 = apoint.y();
            addpointa(ix0,iy0);
            addpointb(ix1,iy1);
            qreal ix2 = ix0-igap;
            qreal iy2 = apoint.y();
            qreal ix3 = ix1;//+igap;
            qreal iy3 = apoint.y();
            addpointa(ix2,iy2);
            addpointb(ix3,iy3);
        }
        for(int i=0;i<icount3;i++)
        {
            QPoint apoint = keypointsB_.at(i);
            qreal ix0 = apoint.x()+igap;
            qreal iy0 = apoint.y();
            qreal ix1 = apoint.x()-isgap;//-igap;
            qreal iy1 = apoint.y();
            addpointa(ix0,iy0);
            addpointb(ix1,iy1);
            qreal ix2 = ix0+igap;
            qreal iy2 = apoint.y();
            qreal ix3 = ix1;//-igap;
            qreal iy3 = apoint.y();
            addpointa(ix2,iy2);
            addpointb(ix3,iy3);
        }

    }
    else if(200==itype)
    {
        for(int i=0;i<icount0;i++)
        {
            QPoint apoint = keypointsA.at(i);
            qreal ix0 = apoint.x();
            qreal iy0 = apoint.y()-isgap;//-igap;
            qreal ix1 = apoint.x();
            qreal iy1 = apoint.y()+igap;
            addpointa(ix0,iy0);
            addpointb(ix1,iy1);
            qreal ix2 = apoint.x();
            qreal iy2 = iy0;//-igap;
            qreal ix3 = apoint.x();
            qreal iy3 = iy1+igap;
            addpointa(ix2,iy2);
            addpointb(ix3,iy3);
        }
        for(int i=0;i<icount1;i++)
        {
            QPoint apoint = keypointsA_.at(i);
            qreal ix0 = apoint.x();
            qreal iy0 = apoint.y()+isgap;//+igap;
            qreal ix1 = apoint.x();
            qreal iy1 = apoint.y()-igap;
            addpointa(ix0,iy0);
            addpointb(ix1,iy1);
            qreal ix2 = apoint.x();
            qreal iy2 = iy0;//+igap;
            qreal ix3 = apoint.x();
            qreal iy3 = iy1-igap;
            addpointa(ix2,iy2);
            addpointb(ix3,iy3);
        }
        for(int i=0;i<icount2;i++)
        {
            QPoint apoint = keypointsB.at(i);
            qreal ix0 = apoint.x()-isgap;//-igap;
            qreal iy0 = apoint.y();
            qreal ix1 = apoint.x()+igap;
            qreal iy1 = apoint.y();
            addpointa(ix0,iy0);
            addpointb(ix1,iy1);
            qreal ix2 = ix0;//-igap;
            qreal iy2 = apoint.y();
            qreal ix3 = ix1+igap;
            qreal iy3 = apoint.y();
            addpointa(ix2,iy2);
            addpointb(ix3,iy3);
        }
        for(int i=0;i<icount3;i++)
        {
            QPoint apoint = keypointsB_.at(i);
            qreal ix0 = apoint.x()+isgap;//+igap;
            qreal iy0 = apoint.y();
            qreal ix1 = apoint.x()-igap;
            qreal iy1 = apoint.y();
            addpointa(ix0,iy0);
            addpointb(ix1,iy1);
            qreal ix2 = ix0;//+igap;
            qreal iy2 = apoint.y();
            qreal ix3 = ix1-igap;
            qreal iy3 = apoint.y();
            addpointa(ix2,iy2);
            addpointb(ix3,iy3);
        }
    }
    else if(1000==itype)
    {
        for(int i=0;i<icount0;i++)
        {
            QPoint apoint = keypointsA.at(i);
            qreal ix0 = apoint.x();
            qreal iy0 = apoint.y()-igap;
            qreal ix1 = apoint.x();
            qreal iy1 = apoint.y()+isgap;//+igap;
            addpointa(ix0,iy0);
            addpointb(ix1,iy1);
            for(int j=0;j<iline;j++)
            {
                iy0 = iy0-igap;
                addpointa(ix0,iy0);
                addpointb(ix1,iy1);
            }
        }
        for(int i=0;i<icount1;i++)
        {
            QPoint apoint = keypointsA_.at(i);
            qreal ix0 = apoint.x();
            qreal iy0 = apoint.y()+igap;
            qreal ix1 = apoint.x();
            qreal iy1 = apoint.y()-isgap;//-igap;
            addpointa(ix0,iy0);
            addpointb(ix1,iy1);
            for(int j=0;j<iline;j++)
            {
                iy0 = iy0+igap;
                addpointa(ix0,iy0);
                addpointb(ix1,iy1);
            }
        }
        for(int i=0;i<icount2;i++)
        {
            QPoint apoint = keypointsB.at(i);
            qreal ix0 = apoint.x()-igap;
            qreal iy0 = apoint.y();
            qreal ix1 = apoint.x()+isgap;//+igap;
            qreal iy1 = apoint.y();
            addpointa(ix0,iy0);
            addpointb(ix1,iy1);
            for(int j=0;j<iline;j++)
            {
                ix0 = ix0-igap;
                addpointa(ix0,iy0);
                addpointb(ix1,iy1);
            }
        }
        for(int i=0;i<icount3;i++)
        {
            QPoint apoint = keypointsB_.at(i);
            qreal ix0 = apoint.x()+igap;
            qreal iy0 = apoint.y();
            qreal ix1 = apoint.x()-isgap;//-igap;
            qreal iy1 = apoint.y();
            addpointa(ix0,iy0);
            addpointb(ix1,iy1);
            for(int j=0;j<iline;j++)
            {
                ix0 = ix0+igap;
                addpointa(ix0,iy0);
                addpointb(ix1,iy1);
            }
        }

    }
    else if(2000==itype)
    {
        for(int i=0;i<icount0;i++)
        {
            QPoint apoint = keypointsA.at(i);
            qreal ix0 = apoint.x();
            qreal iy0 = apoint.y();//-igap;
            qreal ix1 = apoint.x();
            qreal iy1 = apoint.y()+igap;
            addpointa(ix0,iy0);
            addpointb(ix1,iy1);

            for(int j=0;j<iline;j++)
            {
                iy1 = iy1+igap;
                addpointa(ix0,iy0);
                addpointb(ix1,iy1);
            }
        }
        for(int i=0;i<icount1;i++)
        {
            QPoint apoint = keypointsA_.at(i);
            qreal ix0 = apoint.x();
            qreal iy0 = apoint.y();//+igap;
            qreal ix1 = apoint.x();
            qreal iy1 = apoint.y()-igap;
            addpointa(ix0,iy0);
            addpointb(ix1,iy1);
            for(int j=0;j<iline;j++)
            {
                iy1 = iy1-igap;
                addpointa(ix0,iy0);
                addpointb(ix1,iy1);
            }
        }
        for(int i=0;i<icount2;i++)
        {
            QPoint apoint = keypointsB.at(i);
            qreal ix0 = apoint.x();//-igap;
            qreal iy0 = apoint.y();
            qreal ix1 = apoint.x()+igap;
            qreal iy1 = apoint.y();
            addpointa(ix0,iy0);
            addpointb(ix1,iy1);
            for(int j=0;j<iline;j++)
            {
                ix1 = ix1+igap;
                addpointa(ix0,iy0);
                addpointb(ix1,iy1);
            }
        }
        for(int i=0;i<icount3;i++)
        {
            QPoint apoint = keypointsB_.at(i);
            qreal ix0 = apoint.x();//+igap;
            qreal iy0 = apoint.y();
            qreal ix1 = apoint.x()-igap;
            qreal iy1 = apoint.y();
            addpointa(ix0,iy0);
            addpointb(ix1,iy1);
            for(int j=0;j<iline;j++)
            {
                ix1 = ix1-igap;
                addpointa(ix0,iy0);
                addpointb(ix1,iy1);
            }
        }

    }

}
void PointsShape::keyszoom(QList <QPoint> &keypointsA,QList <QPoint> &keypointsA_,
                   QList <QPoint> &keypointsB,QList <QPoint> &keypointsB_,
                    double dxz,double dyz)
{//模型缩小忽略重复点
    QList <QPoint> keypointsA1;
    QList <QPoint> keypointsA_1;
    QList <QPoint> keypointsB1;
    QList <QPoint> keypointsB_1;
    int icount0 = keypointsA.size();
    int icount1 = keypointsA_.size();
    int icount2 =  keypointsB.size();
    int icount3 =  keypointsB_.size();

    for(int i=0;i<icount0;i++)
    {
       QPoint apoint = keypointsA.at(i);
       qreal ix0 = apoint.x()*dxz;
       qreal iy0 = apoint.y()*dyz;
       if(!keypointsA1.contains(QPoint(ix0,iy0)))
       keypointsA1.append(QPoint(ix0,iy0));
    }
    for(int i=0;i<icount1;i++)
    {
       QPoint apoint = keypointsA_.at(i);
       qreal ix0 = apoint.x()*dxz;
       qreal iy0 = apoint.y()*dyz;
       if(!keypointsA_1.contains(QPoint(ix0,iy0)))
       keypointsA_1.append(QPoint(ix0,iy0));
    }
    for(int i=0;i<icount2;i++)
    {
       QPoint apoint = keypointsB.at(i);
       qreal ix0 = apoint.x()*dxz;
       qreal iy0 = apoint.y()*dyz;
       if(!keypointsB1.contains(QPoint(ix0,iy0)))
       keypointsB1.append(QPoint(ix0,iy0));
    }
    for(int i=0;i<icount3;i++)
    {
       QPoint apoint = keypointsB_.at(i);
       qreal ix0 = apoint.x()*dxz;
       qreal iy0 = apoint.y()*dyz;
       if(!keypointsB_1.contains(QPoint(ix0,iy0)))
       keypointsB_1.append(QPoint(ix0,iy0));
    }
  /*
    keypointsA1 = keypointsA;
    keypointsA_1 =  keypointsA_ ;
    keypointsB1 = keypointsB;
    keypointsB_1 = keypointsB_;
  */
        keypointsA.clear();
        keypointsA_.clear();
        keypointsB.clear();
        keypointsB_.clear();

    keypointsA = keypointsA1;
    keypointsA_ = keypointsA_1;
    keypointsB = keypointsB1;
    keypointsB_ = keypointsB_1;


}
void PointsShape::patternzoom(double dxz,double dyz,int igap,int itype,int iline)
{
    if(0==itype)
    {
        QList <QPoint> keypointsA;
        QList <QPoint> keypointsA_;
        QList <QPoint> keypointsB;
        QList <QPoint> keypointsB_;
        patterntokeys(keypointsA,keypointsA_,keypointsB,keypointsB_);
        keyszoom(keypointsA,keypointsA_,keypointsB,keypointsB_,dxz,dyz);
        keystopattern(keypointsA,keypointsA_,keypointsB,keypointsB_,igap);
    }
    else if(1==itype)
    {//黑色线和形体模型缩小（底色为白）
        QList <QPoint> keypointsA;
        QList <QPoint> keypointsA_;
        QList <QPoint> keypointsB;
        QList <QPoint> keypointsB_;
        patterntokeys(keypointsA,keypointsA_,keypointsB,keypointsB_,1);
        keyszoom(keypointsA,keypointsA_,keypointsB,keypointsB_,dxz,dyz);
        keystopattern(keypointsA,keypointsA_,keypointsB,keypointsB_,igap,1000,0,iline);
    }
    else if(2==itype)
    {//白色线和形体模型缩小（底色为黑）
        QList <QPoint> keypointsA;
        QList <QPoint> keypointsA_;
        QList <QPoint> keypointsB;
        QList <QPoint> keypointsB_;
        patterntokeys(keypointsA,keypointsA_,keypointsB,keypointsB_,2);
        keyszoom(keypointsA,keypointsA_,keypointsB,keypointsB_,dxz,dyz);
        keystopattern(keypointsA,keypointsA_,keypointsB,keypointsB_,igap,2000,0,iline);
    }
 }
void PointsShape::keysrootgrid(int ibackgroundtype,
                   double drate,
                   int ilevel)
{//ibackgroundtype=0 无背景   =1黑背景  =2白背景
    int icurmodule = BackImageManager::GetCurMode();
    QRootGrid *arootA = BackImageManager::GetRootGridA(icurmodule);
    QRootGrid *arootA_ = BackImageManager::GetRootGridA_(icurmodule);
    QRootGrid *arootB = BackImageManager::GetRootGridB(icurmodule);
    QRootGrid *arootB_ = BackImageManager::GetRootGridB_(icurmodule);
    arootA->release();
    arootA_->release();
    arootB->release();
    arootB_->release();
    QList <QPoint> keypointsA;
    QList <QPoint> keypointsA_;
    QList <QPoint> keypointsB;
    QList <QPoint> keypointsB_;
    patterntokeys(keypointsA,keypointsA_,keypointsB,keypointsB_,ibackgroundtype);
   // double drate =0.25;
   // int ilevel = 3;
    double dxz0 = 1;
    double dyz0 = 1;
    int icount0 = keypointsA.size();
    for(int i=0;i<icount0;i++)
    {
       QPoint apoint = keypointsA.at(i);
       QList <QPoint> rootpoints;
       dxz0 = 1;
       dyz0 = 1;
       for(int il=0;il<ilevel;il++)
       {
           qreal ix0 = apoint.x()*dxz0;
           qreal iy0 = apoint.y()*dyz0;
           rootpoints.push_front(QPoint(ix0,iy0));
           dxz0 = dxz0 * drate;
           dyz0 = dyz0 * drate;
       }
       arootA->addrootpointlist(rootpoints,1);
    }
    //
    dxz0 = 1;
    dyz0 = 1;
    icount0 = keypointsA_.size();
    for(int i=0;i<icount0;i++)
    {
       QPoint apoint = keypointsA_.at(i);
       QList <QPoint> rootpoints;
       dxz0 = 1;
       dyz0 = 1;
       for(int il=0;il<ilevel;il++)
       {
           qreal ix0 = apoint.x()*dxz0;
           qreal iy0 = apoint.y()*dyz0;
           rootpoints.push_front(QPoint(ix0,iy0));
           dxz0 = dxz0 * drate;
           dyz0 = dyz0 * drate;
       }
       arootA->addrootpointlist(rootpoints,2);
    }
    //
    dxz0 = 1;
    dyz0 = 1;
    icount0 = keypointsB.size();
    for(int i=0;i<icount0;i++)
    {
       QPoint apoint = keypointsB.at(i);
       QList <QPoint> rootpoints;
       dxz0 = 1;
       dyz0 = 1;
       for(int il=0;il<ilevel;il++)
       {
           qreal ix0 = apoint.x()*dxz0;
           qreal iy0 = apoint.y()*dyz0;
           rootpoints.push_front(QPoint(ix0,iy0));
           dxz0 = dxz0 * drate;
           dyz0 = dyz0 * drate;
       }
       arootA->addrootpointlist(rootpoints,4);
    }
    //
    dxz0 = 1;
    dyz0 = 1;
    icount0 = keypointsB_.size();
    for(int i=0;i<icount0;i++)
    {
       QPoint apoint = keypointsB_.at(i);
       QList <QPoint> rootpoints;
       dxz0 = 1;
       dyz0 = 1;
       for(int il=0;il<ilevel;il++)
       {
           qreal ix0 = apoint.x()*dxz0;
           qreal iy0 = apoint.y()*dyz0;
           rootpoints.push_front(QPoint(ix0,iy0));
           dxz0 = dxz0 * drate;
           dyz0 = dyz0 * drate;
       }
       arootA->addrootpointlist(rootpoints,8);
    }



}
void QRootGrid::gridclasstype()
{
// int m_itype;
    //A=1,A_=2,B=4,B_=8
    //背景 白 目标 黑
    //背景 黑 目标 白
    //A 横向 上白下黑
    //A_ 横向 上黑下白
    //B 纵向 左白右黑
    //B_ 纵向 左黑右白

}
void QRootGrid::drawgrid(QPainter &painter)
{

    QRootGrid *pgrid = this;
    int isize = m_plist.size();
    for(int i=0;i<isize;i++)
    {
        QPoint apoint = m_plist[i];
        QList <QPoint> rootpoints;
        apoint.setX(apoint.x()+100*m_ilevel);
        apoint.setY(apoint.y()+100*m_ilevel);
        if(1==m_glist[i].m_itype)
            painter.setPen(Qt::darkYellow);
        else if(2==m_glist[i].m_itype)
            painter.setPen(Qt::darkMagenta);
        else if(4==m_glist[i].m_itype)
            painter.setPen(Qt::darkCyan);
        else if(8==m_glist[i].m_itype)
            painter.setPen(Qt::darkBlue);
        else if((1&2)==m_glist[i].m_itype)
            painter.setPen(Qt::darkGreen);
        else if((1&8)==m_glist[i].m_itype)
            painter.setPen(Qt::darkRed);
        else if((1&4)==m_glist[i].m_itype)
            painter.setPen(Qt::yellow);
        else if((2&4)==m_glist[i].m_itype)
            painter.setPen(Qt::magenta);
        else if((2&8)==m_glist[i].m_itype)
            painter.setPen(Qt::cyan);
        else if((4&8)==m_glist[i].m_itype)
            painter.setPen(Qt::blue);
        else if((1&2&4)==m_glist[i].m_itype)
            painter.setPen(Qt::green);
        else if((1&4&8)==m_glist[i].m_itype)
            painter.setPen(Qt::red);
        else if((2&4&8)==m_glist[i].m_itype)
            painter.setPen(Qt::gray);
        painter.drawPoint(apoint);
    }
    for(int i=0;i<isize;i++)
    {
        m_glist[i].drawgrid(painter);
    }
}
void QRootGrid::drawlayer(QPainter &painter,int ilevel)
{
    int isize = m_plist.size();
    if(m_ilevel==ilevel)
    {
        QRootGrid *pgrid = this;
        for(int i=0;i<isize;i++)
        {
            QPoint apoint = m_plist[i];
            QList <QPoint> rootpoints;
            apoint.setX(apoint.x()+100*m_ilevel);
            apoint.setY(apoint.y()+100*m_ilevel);
            if(1==m_glist[i].m_itype)
                painter.setPen(Qt::darkYellow);
            else if(2==m_glist[i].m_itype)
                painter.setPen(Qt::darkMagenta);
            else if(4==m_glist[i].m_itype)
                painter.setPen(Qt::darkCyan);
            else if(8==m_glist[i].m_itype)
                painter.setPen(Qt::darkBlue);
            else if((1&2)==m_glist[i].m_itype)
                painter.setPen(Qt::darkGreen);
            else if((1&8)==m_glist[i].m_itype)
                painter.setPen(Qt::darkRed);
            else if((1&4)==m_glist[i].m_itype)
                painter.setPen(Qt::yellow);
            else if((2&4)==m_glist[i].m_itype)
                painter.setPen(Qt::magenta);
            else if((2&8)==m_glist[i].m_itype)
                painter.setPen(Qt::cyan);
            else if((4&8)==m_glist[i].m_itype)
                painter.setPen(Qt::blue);
            else if((1&2&4)==m_glist[i].m_itype)
                painter.setPen(Qt::green);
            else if((1&4&8)==m_glist[i].m_itype)
                painter.setPen(Qt::red);
            else if((2&4&8)==m_glist[i].m_itype)
                painter.setPen(Qt::gray);
            painter.drawPoint(apoint);
        }
    }

    for(int i=0;i<isize;i++)
    {
        m_glist[i].drawgrid(painter);
    }
}


void QRootGrid::drawshape(QPainter &painter)
{
    if(1==m_ishow)
    {
        drawgrid(painter);
    }
}
QRootGrid::QRootGrid():
    m_ilevel(0),
    m_point(QPoint(0,0))
{

}
QRootGrid::~QRootGrid()
{
    release();
}

void QRootGrid::release()
{
    int isize = m_plist.size();
    for(int i=0;i<isize;i++)
    {
        m_glist[i].release();
    }
    m_glist.clear();
    m_plist.clear();
}

void QRootGrid::addrootpointlist(QList <QPoint> &rootpoints,int itype)
{
    int isize = rootpoints.size();
    QRootGrid *pgrid = this;
    for(int i=0;i<isize;i++)
    {
        QPoint apoint = rootpoints[i];
        QList <QPoint> alist = pgrid->m_plist;
        if(!alist.contains(apoint))
        {
              QRootGrid agrid;// = new QRootGrid;
              agrid.m_ilevel=i+1;
              agrid.m_point=apoint;
              agrid.m_itype =itype;
              pgrid->m_glist.push_back(agrid);
              pgrid->m_plist.push_back(apoint);
              pgrid = &(pgrid->m_glist[pgrid->m_glist.size()-1]);
        }
        else
        {
           int ilistsize = alist.size();
           int ifind =-1;
           for(int t=0;t<ilistsize;t++)
               if(apoint==alist[t])
                   ifind=t;
           if(-1!=ifind)
           {
               pgrid->m_itype =pgrid->m_itype&itype;
               pgrid = &(pgrid->m_glist[ifind]);
           }
        }

    }
}

void PointsShape::patterntranform(int igap,int itype,int isgap,int iline)
{//模型转换
    QList <QPoint> keypointsA;
    QList <QPoint> keypointsA_;
    QList <QPoint> keypointsB;
    QList <QPoint> keypointsB_;
    patterntokeys(keypointsA,keypointsA_,keypointsB,keypointsB_);
    keystopattern(keypointsA,keypointsA_,keypointsB,keypointsB_,igap,itype,isgap,iline);

}



void PointsShape::doublepattern(double igap,int idirect,PointsShape& apoints)
{//more and more furture in here

    double dx ;
    double dy  ;
    switch (idirect) {
        case 12:// 12 o'clock /up
        {
            int icount = m_path.elementCount();
            for(int i=0;i<icount;i++)
            {
                 QPainterPath::Element aele = m_path.elementAt(i);
                 dx = aele.x;
                 dy = aele.y + igap;
                 apoints.addpointa(dx,dy);
                 dx = aele.x;
                 dy = aele.y - igap;
                 apoints.addpointb(dx,dy);
            }
        }
            break;
        case 6:// 6 o'clock /down
        {
            int icount = m_path.elementCount();
            for(int i=0;i<icount;i++)
            {
                QPainterPath::Element aele = m_path.elementAt(i);
                dx = aele.x;
                dy = aele.y - igap;
                apoints.addpointa(dx,dy);
                dx = aele.x;
                dy = aele.y + igap;
                apoints.addpointb(dx,dy);
            }
        }
            break;
        case 3:// 3 o'clock /left
        {
            int icount = m_path.elementCount();
            for(int i=0;i<icount;i++)
            {
                 QPainterPath::Element aele = m_path.elementAt(i);
                 dx = aele.x - igap;
                 dy = aele.y;
                 apoints.addpointa(dx,dy);
                 dx = aele.x + igap;
                 dy = aele.y;
                 apoints.addpointb(dx,dy);
            }
        }
            break;
        case 9:// 6 o'clock /right
        {
            int icount = m_path.elementCount();
            for(int i=0;i<icount;i++)
            {
                QPainterPath::Element aele = m_path.elementAt(i);
                dx = aele.x + igap;
                dy = aele.y;
                apoints.addpointa(dx,dy);
                dx = aele.x - igap;
                dy = aele.y;
                apoints.addpointb(dx,dy);
            }
        }
            break;

    }

}
void PointsShape::onepattern(double igap,int idirect,PointsShape& apoints)
{//more and more furture in here

    double dx ;
    double dy  ;
    switch (idirect) {
        case 12:// 12 o'clock /up
        {
            int icount = m_path.elementCount();
            for(int i=0;i<icount;i++)
            {
                 QPainterPath::Element aele = m_path.elementAt(i);
                 dx = aele.x;
                 dy = aele.y;
                 apoints.addpointa(dx,dy);
                 dx = aele.x;
                 dy = aele.y - igap;
                 apoints.addpointb(dx,dy);
            }
        }
            break;
        case 6:// 6 o'clock /down
        {
            int icount = m_path.elementCount();
            for(int i=0;i<icount;i++)
            {
                QPainterPath::Element aele = m_path.elementAt(i);
                dx = aele.x;
                dy = aele.y;
                apoints.addpointa(dx,dy);
                dx = aele.x;
                dy = aele.y + igap;
                apoints.addpointb(dx,dy);
            }
        }
            break;
        case 3:// 3 o'clock /left
        {
            int icount = m_path.elementCount();
            for(int i=0;i<icount;i++)
            {
                 QPainterPath::Element aele = m_path.elementAt(i);
                 dx = aele.x;
                 dy = aele.y;
                 apoints.addpointa(dx,dy);
                 dx = aele.x + igap;
                 dy = aele.y;
                 apoints.addpointb(dx,dy);
            }
        }
            break;
        case 9:// 6 o'clock /right
        {
            int icount = m_path.elementCount();
            for(int i=0;i<icount;i++)
            {
                QPainterPath::Element aele = m_path.elementAt(i);
                dx = aele.x;
                dy = aele.y;
                apoints.addpointa(dx,dy);
                dx = aele.x - igap;
                dy = aele.y;
                apoints.addpointb(dx,dy);
            }
        }
            break;

    }

}

void PointsShape::doublesample(int isamplerate,PointsShape& apoints)
{

    double dx ;
    double dy  ;
    int icount = m_path.elementCount();
    int idoublepointscount = icount/2;
    for(int i=0;i<idoublepointscount;i++)
    {
        if(i%isamplerate==0)
        {
            int inum = i*2;
            QPainterPath::Element aele = m_path.elementAt(inum);
            dx = aele.x  ;
            dy = aele.y  ;
            apoints.addpointa(dx,dy);
            aele = m_path.elementAt(inum+1);
            dx = aele.x  ;
            dy = aele.y  ;
            apoints.addpointb(dx,dy);
        }
     }
}

void PointsShape::Move(int ix,int iy)
{
    int icurmodule = BackImageManager::GetCurMode();
    ImageBase *pimage = BackImageManager::GetTransferImage(icurmodule);

    m_path = pimage->ImageTransfer(m_path,ix,iy);
}
void PointsShape::Rotate(double dangle)
{
    int icurmodule = BackImageManager::GetCurMode();
    ImageBase *pimage = BackImageManager::GetTransferImage(icurmodule);

    m_path = pimage->ImageTransfer(m_path,0,0,dangle);

}
void PointsShape::calibration(double dx,double dy,double dangle)
{
    int icurmodule = BackImageManager::GetCurMode();
    ImageBase *pimage = BackImageManager::GetTransferImage(icurmodule);

    m_path = pimage->ImageTransfer(m_path,dx,dy,dangle);

}

double PointsShape::getx(int inum)
{
    int icount = m_path.elementCount();
    if(icount>inum&&inum>=0)
    {
        QPainterPath::Element aele = m_path.elementAt(inum);
        return aele.x;
    }
    else {
        return -99999;
    }
}
double PointsShape::gety(int inum)
{
    int icount = m_path.elementCount();
    if(icount>inum&&inum>=0)
    {
        QPainterPath::Element aele = m_path.elementAt(inum);
        return aele.y;
    }
    else {
        return -99999;
    }

}



void PointsShape::Zoom(double dx0,double dy0)
{
    int icurmodule = BackImageManager::GetCurMode();
    ImageBase *pimage = BackImageManager::GetTransferImage(icurmodule);

    m_path = pimage->ImageTransfer(m_path,0,0,0.0,dx0,dy0);

}

TwoPointsShape::TwoPointsShape():
m_insidewidth(2),
m_movoffset(0),
m_iheadtail(0)
{
    m_ishow = 4;
}
void TwoPointsShape::clear()
{
    m_linemap.clear();
    QPainterPath path;
    m_path = path;
}
void TwoPointsShape::makepath(int ivh)
{
    if(0==ivh)
    {
        QList<int> alist = m_linemap.keys();
        int isize = alist.size();
        for(int i=0;i<isize;i++)
        {
            int ix = alist[i];
            int iminy = m_linemap[ix].x();
            int imaxy = m_linemap[ix].y();
            int fcenty = (iminy + imaxy)/2;

            if(m_path.elementCount()==0)
                m_path.moveTo(ix,fcenty);
            else
                m_path.lineTo(ix,fcenty);
        }
    }
    else if(1==ivh)
    {
        QList<int> alist = m_linemap.keys();
        int isize = alist.size();
        for(int i=0;i<isize;i++)
        {
            int iy = alist[i];
            int iminx = m_linemap[iy].x();
            int imaxx = m_linemap[iy].y();
            int fcentx =  (iminx + imaxx)/2;

            if(m_path.elementCount()==0)
                m_path.moveTo(fcentx,iy);
            else
                m_path.lineTo(fcentx,iy);
        }
    }


}
void TwoPointsShape::addpoint(int ivaluea ,int ivalueb)
{
    QPoint resultpoint ;
    QPoint apoint = m_linemap[ivaluea];
    int imin =  apoint.x();
    int imax =  apoint.y();
    if(0==imin||ivalueb<imin)
        resultpoint.setX(ivalueb);
    else
        resultpoint.setX(imin);
    if(0==imax||ivalueb>imax)
        resultpoint.setY(ivalueb);
    else
        resultpoint.setY(imax);
    m_linemap[ivaluea] = resultpoint;

 }
int TwoPointsShape::size()
{
    QList<int> alist = m_linemap.keys();
    int isize = alist.size();
    return isize;
}
void TwoPointsShape::drawshape(QPainter &painter)
{
//! [9]
    painter.setPen(m_pen);
    painter.setBrush(m_brush);

    if(1==m_ishow)
    {// draw dot
        QList<int> alist = m_linemap.keys();
        int isize = alist.size();
        for(int i=0;i<isize;i++)
        {
            int ix = alist[i];
            int iminy = m_linemap[ix].x();
            int imaxy = m_linemap[ix].y();
            QPointF afpointa,afpointb;
            afpointa.setX(ix);
            afpointa.setY(iminy);
            afpointb.setX(ix);
            afpointb.setY(imaxy);
            painter.drawLine(afpointa,afpointb);
        }
    }
    else if(2==m_ishow)
    {
        // draw dot
         QList<int> alist = m_linemap.keys();
         int isize = alist.size();
         for(int i=0;i<isize;i++)
         {
             int iy = alist[i];
             int iminx = m_linemap[iy].x();
             int imaxx = m_linemap[iy].y();
             QPointF afpointa,afpointb;
             afpointa.setX(iminx);
             afpointa.setY(iy);
             afpointb.setX(imaxx);
             afpointb.setY(iy);
             painter.drawLine(afpointa,afpointb);
          }
    }
    else if(4==m_ishow)
    {
        int icount = m_path.elementCount();
        for(int i=0;i<icount;i++)
        {
             QPainterPath::Element aele = m_path.elementAt(i);
             painter.drawPoint(aele.x,aele.y);
        }
       // painter.drawPath(m_path);
    }
}
void TwoPointsShape::setedgeoi(int insidewidth,int offset,int iheadtail)
{
    m_insidewidth = insidewidth;
    m_movoffset = offset;
    m_iheadtail = iheadtail;
}
void TwoPointsShape::edgeimage(QImage &aImage,int itype)
{
    int inum = m_iheadtail>0?m_iheadtail:-m_iheadtail;

    switch (itype)
    {
    case 0:
    {
        int icount = m_path.elementCount();
        if(icount>inum)
        for(int i=inum;i<icount-inum;i++)
        {
             QPainterPath::Element aele = m_path.elementAt(i);
             for(int i=0;i<m_insidewidth;i++)
             {
                 aImage.setPixel(aele.x,aele.y+i+m_movoffset,qRgb(255,255,255));
             }
             for(int i=1;i<m_insidewidth;i++)
             {
                aImage.setPixel(aele.x,aele.y-i+m_movoffset,qRgb(0,0,0));
             }
        }
    }

    break;
    case 1:
    {
        int icount = m_path.elementCount();
        if(icount>inum)
        for(int i=inum;i<icount-inum;i++)
        {
             QPainterPath::Element aele = m_path.elementAt(i);
             for(int i=0;i<m_insidewidth;i++)
             {
                 aImage.setPixel(aele.x+m_movoffset+i,aele.y,qRgb(0,0,0));
             }
             for(int i=1;i<m_insidewidth;i++)
             {
                 aImage.setPixel(aele.x+m_movoffset-i,aele.y,qRgb(255,255,255));
             }
        }
    }
    break;
    case 2:
    {
        int icount = m_path.elementCount();
        if(icount>inum)
        for(int i=inum;i<icount-inum;i++)
        {
             QPainterPath::Element aele = m_path.elementAt(i);
             for(int i=0;i<m_insidewidth;i++)
             {
                aImage.setPixel(aele.x,aele.y+i+m_movoffset,qRgb(0,0,0));
             }
             for(int i=1;i<m_insidewidth;i++)
             {
                 aImage.setPixel(aele.x,aele.y-i+m_movoffset,qRgb(255,255,255));
             }
        }
    }


        break;
    case 3:
    {
        int icount = m_path.elementCount();
        if(icount>inum)
        for(int i=inum;i<icount-inum;i++)
        {
             QPainterPath::Element aele = m_path.elementAt(i);
             for(int i=0;i<m_insidewidth;i++)
             {
                 aImage.setPixel(aele.x+m_movoffset-i,aele.y,qRgb(0,0,0));
             }
             for(int i=1;i<m_insidewidth;i++)
             {
                 aImage.setPixel(aele.x+m_movoffset+i,aele.y,qRgb(255,255,255));
             }
        }
    }
        break;

    }
}
void TwoPointsShape::Move(int ix,int iy)
{
    int icurmodule = BackImageManager::GetCurMode();
    ImageBase *pimage = BackImageManager::GetTransferImage(icurmodule);

    m_path = pimage->ImageTransfer(m_path,ix,iy);
}
void TwoPointsShape::Rotate(double dangle)
{
    int icurmodule = BackImageManager::GetCurMode();
    ImageBase *pimage = BackImageManager::GetTransferImage(icurmodule);

    m_path = pimage->ImageTransfer(m_path,0,0,dangle);

}
void TwoPointsShape::Zoom(double dx0,double dy0)
{
    int icurmodule = BackImageManager::GetCurMode();
    ImageBase *pimage = BackImageManager::GetTransferImage(icurmodule);

    m_path = pimage->ImageTransfer(m_path,0,0,0.0,dx0,dy0);

}


RectsShape::RectsShape():
    m_ispecshow(-1)
{
   m_pen=QColor(0,255,100);
}
void RectsShape::addrect(QRect &arect)
{
   m_rects.push_back(arect);
}
void RectsShape::addrect(QRect &arect,QString &astring)
{
   m_rects.push_back(arect);
   m_strlist.push_back(astring);
}
QRect RectsShape::getrect(int inum)
{
    if(inum>=0&&inum<m_rects.size())
        return m_rects.at(inum);
    else
        return QRect(0,0,0,0);
}
void RectsShape::setrect(int inum,int ix,int iy,int iw,int ih)
{
    if(inum<m_rects.size())
    m_rects[inum] = QRect(ix,iy,iw,ih);
}
void RectsShape::setstring(int inum,const QString &str)
{
    if(inum<m_strlist.size())
    m_strlist[inum] = str;
}
void RectsShape::setangle(int inum,double dangle)
{

}
int RectsShape::size()
{
    return m_rects.size();
}
void RectsShape::clear()
{
    m_rects.clear();
    m_strlist.clear();
    m_angles.clear();
}

static QGradient gradient(const QColor &color, const QRect &rect)
{
    QColor c = color;
    c.setAlpha(255);
    QLinearGradient result(rect.topLeft(), rect.bottomRight());
    result.setColorAt(0, c.dark(10));
    result.setColorAt(0.5, c.light(10));
    result.setColorAt(1, c.dark(10));
    return result;
}

void RectsShape::setspecshow(int ishownum)
{
    m_ispecshow = ishownum;
}
void RectsShape::drawshape(QPainter &painter)
{
//! [9]
 //   painter.setPen(m_pen);
 //   painter.setBrush(m_brush);
 //   if (m_antialiased)
  //      painter.setRenderHint(QPainter::Antialiasing, true);
//! [9]

//! [10]
    painter.setPen(m_pen);
    painter.setBrush(Qt::NoBrush);

            if(1==m_ishow)
            {// draw rects
               //painter.drawPath(m_path);
               painter.setPen(m_pen);
               painter.drawRects(m_rects);
            }
            if(2==m_ishow)
            {
                int ishowfont = 1;
                if(m_rects.size()!=m_strlist.size())
                    ishowfont = 0;
                int icount = m_rects.size();
                for(int i=0;i<icount;i++)
                {
                    if(-1==m_ispecshow
                       ||i==m_ispecshow)
                    {
                        painter.setPen(m_pen);
                        painter.drawRect(m_rects[i]);
                        if(ishowfont)
                        {
                            painter.setPen(QColor(0,100,0));
                            painter.drawText(QRect(m_rects[i].x(),
                                                   m_rects[i].y()-20,
                                                   80,30),
                                             Qt::AlignTop & Qt::AlignRight,
                                             m_strlist[i]);
                        }
                    }

                }

            }
            if(4==m_ishow)
            {
                int ishowfont = 1;
                if(m_rects.size()!=m_strlist.size())
                    ishowfont = 0;
                int icount = m_rects.size();
               // for(int i=0;i<icount;i++)
                if(icount>0)
                {
                    painter.setPen(m_pen);
                    painter.drawRect(m_rects[icount-1]);
                    if(ishowfont)
                    {
                        painter.setPen(QColor(0,100,0));
                        painter.drawText(QRect(m_rects[icount-1].x(),
                                               m_rects[icount-1].y()-20,
                                               80,30),
                                         Qt::AlignTop & Qt::AlignRight,
                                         m_strlist[icount-1]);

                    }

                }

            }
}
void RectsShape::drawshapex(QPainter &painter,
                            double dmovx,
                            double dmovy,
                            double dangle,
                            double dzoomx,
                            double dzoomy)
{
    painter.setPen(m_pen);
    painter.setBrush(Qt::NoBrush);

            if(1==m_ishow)
            {// draw rects
               //painter.drawPath(m_path);
               painter.setPen(m_pen);
               RectVector arects=m_rects;
               for (int i=0;i<m_rects.size();i++)
               {
                   double dvaluex = arects[i].x();
                   double dvaluey = arects[i].y();
                   double dvaluew = arects[i].width();
                   double dvalueh = arects[i].height();

                   dvaluex = dvaluex*dzoomx+dmovx;
                   dvaluey = dvaluey*dzoomy+dmovy;
                   dvaluew = dvaluew*dzoomx;
                   dvalueh = dvalueh*dzoomy;

                   arects[i].setX(dvaluex);
                   arects[i].setY(dvaluey);
                   arects[i].setWidth(dvaluew);
                   arects[i].setHeight(dvalueh);
               }
               painter.drawRects(arects);
            }
            if(2==m_ishow)
            {
                int ishowfont = 1;
                if(m_rects.size()!=m_strlist.size())
                    ishowfont = 0;
                RectVector arects=m_rects;
                int icount = m_rects.size();
                for(int i=0;i<icount;i++)
                {
                    double dvaluex = arects[i].x();
                    double dvaluey = arects[i].y();
                    double dvaluew = arects[i].width();
                    double dvalueh = arects[i].height();

                    dvaluex = dvaluex*dzoomx+dmovx;
                    dvaluey = dvaluey*dzoomy+dmovy;
                    dvaluew = dvaluew*dzoomx;
                    dvalueh = dvalueh*dzoomy;

                    arects[i].setX(dvaluex);
                    arects[i].setY(dvaluey);
                    arects[i].setWidth(dvaluew);
                    arects[i].setHeight(dvalueh);
                    if(-1==m_ispecshow
                       ||i==m_ispecshow)
                    {
                        painter.setPen(m_pen);
                        painter.drawRect(arects[i]);
                        if(ishowfont)
                        {
                            painter.setPen(QColor(0,100,0));
                            painter.drawText(QRect(arects[i].x(),
                                                   arects[i].y()-15,
                                                   80,30),
                                             Qt::AlignTop & Qt::AlignRight,
                                             m_strlist[i]);
                        }
                    }

                }

            }
            if(4==m_ishow)
            {
                int ishowfont = 1;
                if(m_rects.size()!=m_strlist.size())
                    ishowfont = 0;
                RectVector arects=m_rects;
                int icount = m_rects.size();

                if(icount>0)
                {
                    double dvaluex = arects[icount-1].x();
                    double dvaluey = arects[icount-1].y();
                    double dvaluew = arects[icount-1].width();
                    double dvalueh = arects[icount-1].height();

                    dvaluex = dvaluex*dzoomx+dmovx;
                    dvaluey = dvaluey*dzoomy+dmovy;
                    dvaluew = dvaluew*dzoomx;
                    dvalueh = dvalueh*dzoomy;

                    arects[icount-1].setX(dvaluex);
                    arects[icount-1].setY(dvaluey);
                    arects[icount-1].setWidth(dvaluew);
                    arects[icount-1].setHeight(dvalueh);
                    painter.setPen(m_pen);
                    painter.drawRect(arects[icount-1]);
                    if(ishowfont)
                    {
                        painter.setPen(QColor(0,100,0));
                        painter.drawText(QRect(arects[icount-1].x(),
                                               arects[icount-1].y()-15,
                                               80,30),
                                         Qt::AlignTop & Qt::AlignRight,
                                         m_strlist[icount-1]);

                    }

                }

            }
}




QRect RectsShape::comb(QRect &rect1,QRect &rect2)
{

    int ix0= rect1.left();
    int iy0= rect1.top();
    int ix1= rect1.right();
    int iy1= rect1.bottom();

    int ix2= rect2.left();
    int iy2= rect2.top();
    int ix3= rect2.right();
    int iy3= rect2.bottom();

    int iminx = ix0<ix2?ix0:ix2;
    int imaxx = ix1>ix3?ix1:ix3;
    int iminy = iy0<iy2?iy0:iy2;
    int imaxy = iy1>iy3?iy1:iy3;

    return QRect(iminx,iminy,imaxx-iminx,imaxy-iminy);

}
QRect RectsShape::comb(RectVector &rects1)
{
    QRect arect = rects1[0];
    for(int i=1;i<rects1.size();i++)
    {
        arect = comb(rects1[i],arect);
    }
    return arect;
}
RectDirction RectsShape::pos(QRect &rect1,QRect &rect2)
{
    if(rect2.contains(rect1))
        return RectDirction::inside_dir;
    if(rect1.contains(rect2))
        return RectDirction::outside_dir;

    int ix0= rect1.left();
    int iy0= rect1.top();
    int ix1= rect1.right();
    int iy1= rect1.bottom();

    int ix2= rect2.left();
    int iy2= rect2.top();
    int ix3= rect2.right();
    int iy3= rect2.bottom();
// ix0<ix2  ix3<ix1
    //iy0>iy3

//
    if(ix0<=ix2&&ix3<=ix1+2&&iy0>iy2)
    {//scale for ocr
        return RectDirction::down_purex_dir;
    }
    else if(ix0<=ix2&&ix3<=ix1+2&&iy0>=iy3)
    {//scale for ocr
        return RectDirction::down_purex_dir;
    }
    else if(ix1<ix2&&iy1<iy2)
        return RectDirction::rightup_dir;
    else if(ix3<ix0&&iy1<iy2)
        return RectDirction::leftup_dir;
    else if(ix1<ix2&&iy3<iy0)
        return RectDirction::rightdown_dir;
    else if(ix3<ix0&&iy3<iy0)
        return RectDirction::leftdown_dir;
    else if(iy1<iy2)
        return RectDirction::up_dir;
    else if(iy3<=iy0)
    {
        return RectDirction::down_dir;
        // ix0<ix2  ix3<ix1
            //iy0>iy3
    }
    else if(ix1<ix2)
        return RectDirction::right_dir;
    else if(ix3<ix0)
        return RectDirction::left_dir;
    else
        return RectDirction::cross_dir;
}
void RectsShape::sort()
{
    RectVector sortrects;
    int icount = m_rects.size();
    RectDirction adir0,adir1;
    for(int i=0;i<icount;i++)
    {
        if(i==0)
            sortrects.append(m_rects[i]);
        else if(i==1)
        {
            adir0 = pos(m_rects[0],m_rects[1]);
            if(adir0==RectDirction::down_dir||adir0==RectDirction::right_dir)
            {
                sortrects.append(m_rects[1]);
            }
            else if(adir0==RectDirction::up_dir||adir0==RectDirction::left_dir)
            {
                sortrects.push_front(m_rects[1]);
            }
            else
                sortrects.push_front(m_rects[1]);
        }
        else if(i>1)
        {
            int sortcount = sortrects.size();
            for(int j=0;j<sortcount-1;j++)
            {
                adir0 = pos(m_rects[i],sortrects[j]);
                adir1 = pos(m_rects[i],sortrects[j+1]);
                if((adir0==RectDirction::up_dir
                        ||adir0==RectDirction::leftup_dir
                    ||adir0==RectDirction::rightup_dir
                    ||adir0==RectDirction::right_dir)
                 &&(adir1==RectDirction::up_dir
                    ||adir1==RectDirction::leftup_dir
                    ||adir1==RectDirction::rightup_dir
                    ||adir1==RectDirction::right_dir))
                {
                    sortrects.push_front(m_rects[i]);
                    break;
                }
                else if((adir0==RectDirction::left_dir
                        ||adir0==RectDirction::leftup_dir
                        ||adir0==RectDirction::leftdown_dir)
                 &&(adir1==RectDirction::left_dir
                    ||adir1==RectDirction::leftup_dir
                    ||adir1==RectDirction::leftdown_dir))
                {

                    if(j==sortcount-2)
                    {
                        sortrects.push_back(m_rects[i]);
                        break;
                    }
                    else
                        continue;
                }
                else if((adir0==RectDirction::up_dir
                         ||adir0==RectDirction::leftup_dir
                         ||adir0==RectDirction::rightup_dir)
                  &&(adir1==RectDirction::up_dir
                     ||adir1==RectDirction::leftup_dir
                     ||adir1==RectDirction::rightup_dir))
                {
                    if(j==sortcount-2)
                    {
                        sortrects.push_back(m_rects[i]);
                        break;
                    }
                    else
                        continue;
                }
                else if(adir0==RectDirction::inside_dir)
                {
                    break;
                }
                else if(adir0==RectDirction::outside_dir)
                {
                    sortrects[j] = m_rects[i];
                    break;
                }
                else if((adir0==RectDirction::up_dir
                    ||adir0==RectDirction::leftup_dir
                    ||adir0==RectDirction::rightup_dir)
                &&(adir1==RectDirction::down_dir
                   ||adir1==RectDirction::leftdown_dir
                   ||adir1==RectDirction::rightdown_dir))
                {
                    sortrects.insert(j+1,m_rects[i]);
                    break;
                }
                else if((adir0==RectDirction::left_dir
                         ||adir0==RectDirction::leftup_dir
                         ||adir0==RectDirction::leftdown_dir)
                    &&(adir1==RectDirction::right_dir
                       ||adir1==RectDirction::rightup_dir
                       ||adir1==RectDirction::rightdown_dir))
                    {
                        sortrects.insert(j+1,m_rects[i]);
                        break;
                    }
            }

        }
    }
    m_rects = sortrects;
}

void RectsShape::removecontains_c()
{
    QVector<int> aisetv;
    int isize = m_rects.size();
    for(int i=0;i<isize;i++)
    {
        aisetv.push_back(1);
    }

    for(int i=0;i<isize;i++)
    {
        int icompj = i;
        int bfindcontain = 1;
        while(bfindcontain)
        {
            bfindcontain = 0;
            icompj = icompj + 1;
            if(icompj<isize)
            {
                RectDirction rd = pos(m_rects[i],m_rects[icompj]);
                if(rd == RectDirction::outside_dir)
                  bfindcontain = 1;
                else if(rd== RectDirction::down_purex_dir)
                   bfindcontain = 2;
            }
            else
                bfindcontain = 0;
           if(1==bfindcontain)
           {
               aisetv[icompj] = 0;
           }
           else if(2==bfindcontain)
           {
               aisetv[i] = 2;
               aisetv[icompj] = 2;
           }
        }
    }
    RectVector arects;

    for(int i=0;i<isize;i++)
    {
        if(aisetv[i]!=0&&aisetv[i]!=2)
            arects.push_back(m_rects[i]);
        else if(i+1<isize)
        {
            if(aisetv[i]==2&&aisetv[i+1]==2)
            {
                int ixa0 = m_rects[i].x();
                int iya0 = m_rects[i].y();
                int ixb0 = ixa0+m_rects[i].width();
                int iyb0 = iya0+m_rects[i].height();

                int ixa1 = m_rects[i+1].x();
                int iya1 = m_rects[i+1].y();
                int ixb1 = ixa1+m_rects[i+1].width();
                int iyb1 = iya1+m_rects[i+1].height();

                int iminx = ixa0<ixa1?ixa0:ixa1;
                int iminy = iya0<iya1?iya0:iya1;

                int imaxx = ixb0>ixb1?ixb0:ixb1;
                int imaxy = iyb0>iyb1?iyb0:iyb1;

                int iwx = imaxx - iminx;
                int ihx = imaxy - iminy;
                QRect arectx(iminx,iminy,iwx,ihx);
                arects.push_back(arectx);
            }
        }
    }
    m_rects.clear();

    m_rects.clear();

    for(int i=0;i<arects.size();i++)
    {
        m_rects.push_back(arects[i]);
    }

}
void RectsShape::Move(int ix,int iy)
{
    int icount = m_rects.size();
    for(int i=0;i<icount;i++)
    {
        int ix1= m_rects[i].left();
        int iy1= m_rects[i].top();
        int ix2= m_rects[i].right();
        int iy2= m_rects[i].bottom();
        m_rects[i].setLeft(ix1+ix);
        m_rects[i].setTop(iy1+iy);
        m_rects[i].setRight(ix2+ix);
        m_rects[i].setBottom(iy2+iy);
    }

}
void RectsShape::Rotate(double iangle)
{

}
void RectsShape::Zoom(double dx,double dy)
{
    int icount = m_rects.size();
    for(int i=0;i<icount;i++)
    {
        int ix1= m_rects[i].left();
        int iy1= m_rects[i].top();
        int ix2= m_rects[i].right();
        int iy2= m_rects[i].bottom();
        m_rects[i].setLeft(ix1*dx);
        m_rects[i].setTop(iy1*dy);
        m_rects[i].setRight(ix2*dx);
        m_rects[i].setBottom(iy2*dy);
    }
}






QArea::QArea(RectType type ):
m_type(type)
{

}

QAreas::QAreas(RectsShape &rects)
{
    int isize = rects.size();
    for(int i=0;i<isize;i++)
    {
        QArea area;
        area.setid(i);
        area.settype(QArea::orgarea);
        m_areas.push_back(area);
    }
}
void QAreas::GenMap()
{
    /*
    0 r1x1 r1x2    r2x1 r2x2
    -------------------
ry1 |   |   |      |      |
    -------------------
ry2 |
    -------------------
    |
    -------------------
    |
    -------------------
    |
    |
    |

*/
}
void QAreas::regroup()
{
    int isize = m_areas.size();
    for(int i=0;i<isize;i++)
    {

     }
}
