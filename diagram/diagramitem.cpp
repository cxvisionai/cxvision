
#include "diagramitem.h"
#include "arrow.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>

//! [0]
DiagramItem::DiagramItem(DiagramType diagramType, QMenu *contextMenu,
             QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent)
{
    myDiagramType = diagramType;
    myContextMenu = contextMenu;

    QPainterPath path;
    switch (myDiagramType) {
        case StartEnd:
            path.moveTo(200, 50);
            path.arcTo(150, 0, 50, 50, 0, 90);
            path.arcTo(50, 0, 50, 50, 90, 90);
            path.arcTo(50, 50, 50, 50, 180, 90);
            path.arcTo(150, 50, 50, 50, 270, 90);
            path.lineTo(200, 25);
            myPolygon = path.toFillPolygon();
            break;
    case Relation:
        path.addEllipse(QRectF(QPointF(-50, -50),QPointF(50, 50)));

        myPolygon = path.toFillPolygon();
    break;
        case Node:
            path.addEllipse(QRectF(QPointF(-50, -25),QPointF(50, 25)));

            myPolygon = path.toFillPolygon();
        break;
        case Conditional:
            myPolygon << QPointF(-50, 0) << QPointF(0, 50)
                      << QPointF(50, 0) << QPointF(0, -50)
                      << QPointF(-50, 0);
            break;
        case Step:
            myPolygon << QPointF(-50, -50) << QPointF(50, -50)
                      << QPointF(50, 50) << QPointF(-50, 50)
                      << QPointF(-50, -50);
            break;
        case String:
        myPolygon << QPointF(-50, -25) << QPointF(50, -25)
                  << QPointF(50, 25) << QPointF(-50, 25)
                  << QPointF(-50, -25);
            break;

        default:
            myPolygon << QPointF(-120, -80) << QPointF(-70, 80)
                      << QPointF(120, 80) << QPointF(70, -80)
                      << QPointF(-120, -80);
            break;
    }
    setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    m_strModelname = "model name";
}
//! [0]

//! [1]
void DiagramItem::removeArrow(Arrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}
//! [1]

//! [2]
void DiagramItem::removeArrows()
{
    foreach (Arrow *arrow, arrows) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}
//! [2]

//! [3]
void DiagramItem::addArrow(Arrow *arrow)
{
    arrows.append(arrow);
}
//! [3]

//! [4]
QPixmap DiagramItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    painter.drawPolyline(myPolygon);

    return pixmap;
}
//! [4]

//! [5]
void DiagramItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    //scene()->clearSelection();
    //setSelected(true);
    //myContextMenu->exec(event->screenPos());
}
//! [5]

//! [6]
QVariant DiagramItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (Arrow *arrow, arrows) {
            arrow->updatePosition();
        }
    }

    return value;
}
//! [6]

//! [6]
void DiagramItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                        QWidget *widget)
{
    QGraphicsPolygonItem::paint(painter,option,widget);
    QRect rect(-40,-30,100,100);
    painter->drawText(rect, Qt::AlignTop|Qt::AlignLeft, m_strModelname);
}
void DiagramItem::SetDiagramModelName(const QString strmodelname)
{
    setData(1,strmodelname);
    m_strModelname = strmodelname;
}
QString DiagramItem::GetDiagramModelName()const
{

    return m_strModelname;
}
