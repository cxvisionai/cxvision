

#include "arrowi.h"

#include <qmath.h>
#include <QPen>
#include <QPainter>

//! [0]
Arrowi::Arrowi(DiagramItem *startItem, DiagramItem *endItem, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    myStartItem = startItem;
    myEndItem = endItem;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    myColor = Qt::black;
    setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}
//! [0]

//! [1]
QRectF Arrowi::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}
//! [1]

//! [2]
QPainterPath Arrowi::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(ArrowiHead);
    return path;
}
//! [2]

//! [3]
void Arrowi::updatePosition()
{
    QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
    setLine(line);
}
//! [3]

//! [4]
void Arrowi::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *)
{
    if (myStartItem->collidesWithItem(myEndItem))
        return;

    QPen myPen = pen();
    myPen.setColor(myColor);
    qreal ArrowiSize = 20;
    painter->setPen(myPen);
    painter->setBrush(myColor);
//! [4] //! [5]

    QLineF centerLine(myStartItem->pos(), myEndItem->pos());
    QPolygonF endPolygon = myEndItem->polygon();
    QPointF p1 = endPolygon.first() + myEndItem->pos();
    QPointF p2;
    QPointF intersectPoint;
    QLineF polyLine;
    for (int i = 1; i < endPolygon.count(); ++i) {
        p2 = endPolygon.at(i) + myEndItem->pos();
        polyLine = QLineF(p1, p2);
        QLineF::IntersectType intersectType =
            polyLine.intersect(centerLine, &intersectPoint);
        if (intersectType == QLineF::BoundedIntersection)
            break;
        p1 = p2;
    }

    setLine(QLineF(intersectPoint, myStartItem->pos()));
//! [5] //! [6]

    double angle = std::atan2(-line().dy(), line().dx());

    QPointF ArrowiP1 = line().p1() + QPointF(sin(angle + M_PI / 3) * ArrowiSize,
                                    cos(angle + M_PI / 3) * ArrowiSize);
    QPointF ArrowiP2 = line().p1() + QPointF(sin(angle + M_PI - M_PI / 3) * ArrowiSize,
                                    cos(angle + M_PI - M_PI / 3) * ArrowiSize);

    ArrowiHead.clear();
    ArrowiHead << line().p1() << ArrowiP1 << ArrowiP2;
//! [6] //! [7]
     painter->drawLine(line());
//    QRect rect1(QPoint(line().x1(),line().y1()),QPoint(line().x2(),line().y2()));
//    int startAngle = 20 * 16;
//    int arcLength = 120 * 16;
//      painter->drawArc(rect1, startAngle, arcLength);

    //  QPainterPath path;
    //  path.moveTo(line().x1(), line().y1());
    //  path.cubicTo(line().x1(), line().y1(), (line().x1()+line().x2())/2, (line().y1()+line().y2())/2,line().x2(), line().y2());
    //  painter->drawPath(path);
    painter->drawPolygon(ArrowiHead);
    if (isSelected()) {
        painter->setPen(QPen(myColor, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }
}
//! [7]
void Arrowi::SetDiagramModelConnectName(const QString strmodelname)
{

    setData(2,strmodelname);
    m_strModelConnectname = strmodelname;
}
