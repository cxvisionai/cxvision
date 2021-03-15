#include <qevent.h>
#include <QPainter>
#include <QTextStream>
#include <QUndoStack>
#include "visionmanager.h"
#include "shapecommands.h"

static const int resizeHandleWidth = 6;

/******************************************************************************
** QShape
*/

const QSize QShape::minSize(30, 20);

QShape::QShape(Type type, const QColor &color, const QRect &rect)
    : m_type(type),
      m_rect(rect),
      m_color(color),
      m_ipenwidth(1),
      m_ishow(1)
{
}
void QShape::setshow(int ishow)
{
    m_ishow = ishow;
}

void QShape::settype(int itype)
{
    switch (itype)
    {
    case 0:
            m_type=QShape::Rectangle;
        break;
    case 1:
            m_type=QShape::Circle;
        break;
    case 2:
            m_type=QShape::Triangle;
        break;
    default:
            m_type=QShape::Rectangle;
        break;
    }

}
void QShape::setname(const char *pname)
{
    m_name = QString(pname);
}
void QShape::setrect(int ix,int iy,int iw,int ih)
{
    m_rect = QRect(ix,iy,iw,ih);
}
void QShape::setcolor(int ir,int ig ,int ib)
{
    m_color =QColor(ir,ig,ib);
}

QShape::Type QShape::type() const
{
    return m_type;
}

QRect QShape::rect() const
{
    return m_rect;
}

QColor QShape::color() const
{
    return m_color;
}

QString QShape::name() const
{
    return m_name;
}
int QShape::show() const
{
    return m_ishow;
}
QRect QShape::resizeHandle() const
{
    QPoint br = m_rect.bottomRight();
    return QRect(br - QPoint(resizeHandleWidth, resizeHandleWidth), br);
}

QString QShape::typeToString(Type type)
{
    QString result;

    switch (type) {
        case Rectangle:
            result = QLatin1String("rect");
            break;
        case Circle:
            result = QLatin1String("circle");
            break;
        case Triangle:
            result = QLatin1String("triangle");
            break;
    }

    return result;
}

QShape::Type QShape::stringToType(const QString &s, bool *ok)
{
    if (ok != 0)
        *ok = true;

    if (s == QLatin1String("Rectangle"))
        return Rectangle;
    if (s == QLatin1String("Circle"))
        return Circle;
    if (s == QLatin1String("Triangle"))
        return Triangle;

    if (ok != 0)
        *ok = false;
    return Rectangle;
}
static QGradient gradient(const QColor &color, const QRect &rect)
{
    QColor c = color;
    c.setAlpha(40);
    QLinearGradient result(rect.topLeft(), rect.bottomRight());
    result.setColorAt(0, c.dark(40));
    result.setColorAt(0.5, c.light(10));
    result.setColorAt(1, c.dark(40));
    return result;
}
static QPolygon triangle(const QRect &rect)
{
    QPolygon result(3);
    result.setPoint(0, rect.center().x(), rect.top());
    result.setPoint(1, rect.right(), rect.bottom());
    result.setPoint(2, rect.left(), rect.bottom());
    return result;
}
void QShape::drawshape(QPainter &painter,QPalette &pal) const
{
        QPen pen = QColor(0,150,0);// pal.text().color();
        pen.setWidth(m_ipenwidth);
        painter.setPen(pen);
        painter.setBrush(gradient(color(),rect()));

        QRect arect = rect();
        arect.adjust(1, 1, -resizeHandleWidth/2, -resizeHandleWidth/2);

        // paint the QShape
        switch (type())
        {
            case QShape::Rectangle:
                painter.drawRect(arect);
                break;
            case QShape::Circle:
                painter.setRenderHint(QPainter::Antialiasing);
                painter.drawEllipse(arect);
                painter.setRenderHint(QPainter::Antialiasing, false);
                break;
            case QShape::Triangle:
                painter.setRenderHint(QPainter::Antialiasing);
                painter.drawPolygon(triangle(arect));
                painter.setRenderHint(QPainter::Antialiasing, false);
                break;
        }

        // paint the resize handle
        painter.setPen(pal.text().color());
        painter.setBrush(Qt::white);
        painter.drawRect(resizeHandle().adjusted(0, 0, -1, -1));

        // paint the QShape name
        painter.setBrush(pal.text());
        if (type() == QShape::Triangle)
            arect.adjust(0, arect.height()/2, 0, 0);
        painter.drawText(arect, Qt::AlignCenter,name());

}
