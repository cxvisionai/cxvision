/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <qevent.h>
#include <QPainter>
#include <QTextStream>
#include <QUndoStack>
#include "visionmanager.h"
#include "shapecommands.h"

static const int resizeHandleWidth =6;

/******************************************************************************
** QShape
*/

const QSize QShape::minSize(30, 20);

QShape::QShape(Type type, const QColor &color, const QRect &rect)
    : m_type(type),
      m_rect(rect),
      m_color(color),
      m_ipenwidth(1),
      m_ishow(1),
      m_ifontsize(8)
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
    case 3:
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
QRect QShape::resizeHandlez(double dzoomx,double dzoomy) const
{
    QPoint br = m_rect.bottomRight();
    return QRect(br - QPoint(resizeHandleWidth/dzoomx, resizeHandleWidth/dzoomy), br);
}
QRect QShape::resizeHandlex(double dmovx,double dmovy,
                            double dangle,double dzoomx,double dzoomy) const
{
    double dvaluex = rect().x();
    double dvaluey = rect().y();
    double dvaluew = rect().width();
    double dvalueh = rect().height();

    dvaluex = dvaluex*dzoomx+dmovx;
    dvaluey = dvaluey*dzoomy+dmovy;
    dvaluew = dvaluew*dzoomx;
    dvalueh = dvalueh*dzoomy;
    QRect arect =QRect(dvaluex,dvaluey,dvaluew,dvalueh);
    QPoint br = arect.bottomRight();
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
        case Font:
            result = QLatin1String("font");
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
    if (s == QLatin1String("Font"))
        return Font;

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

                // paint the resize handle
                painter.setPen(pal.text().color());
                painter.setBrush(Qt::white);
                painter.drawRect(resizeHandle().adjusted(0, 0, -1, -1));

                // paint the QShape name
                painter.setBrush(pal.text());
                if (type() == QShape::Triangle)
                    arect.adjust(0, arect.height()/2, 0, 0);
                painter.drawText(arect, Qt::AlignCenter,name());
                break;
            case QShape::Circle:
                painter.setRenderHint(QPainter::Antialiasing);
                painter.drawEllipse(arect);
                painter.setRenderHint(QPainter::Antialiasing, false);

                // paint the resize handle
                painter.setPen(pal.text().color());
                painter.setBrush(Qt::white);
                painter.drawRect(resizeHandle().adjusted(0, 0, -1, -1));

                // paint the QShape name
                painter.setBrush(pal.text());
                if (type() == QShape::Triangle)
                    arect.adjust(0, arect.height()/2, 0, 0);
                painter.drawText(arect, Qt::AlignCenter,name());
                break;
            case QShape::Triangle:
                painter.setRenderHint(QPainter::Antialiasing);
                painter.drawPolygon(triangle(arect));
                painter.setRenderHint(QPainter::Antialiasing, false);

                // paint the resize handle
                painter.setPen(pal.text().color());
                painter.setBrush(Qt::white);
                painter.drawRect(resizeHandle().adjusted(0, 0, -1, -1));

                // paint the QShape name
                painter.setBrush(pal.text());
                if (type() == QShape::Triangle)
                    arect.adjust(0, arect.height()/2, 0, 0);
                painter.drawText(arect, Qt::AlignCenter,name());
                break;
            case QShape::Font:
                QFont font = painter.font();
                font.setPixelSize(m_ifontsize);
                painter.setFont(font);
                // paint the resize handle
                painter.setPen(pal.text().color());
                painter.setBrush(pal.text());
                painter.drawText(arect, Qt::AlignCenter,name());
                break;
        }


}
void QShape::drawshapex(QPainter &painter,QPalette &pal,double dmovx,double dmovy,
                        double dangle,double dzoomx,double dzoomy) const
{
        QPen pen = QColor(0,0,250);// pal.text().color();
        pen.setWidth(m_ipenwidth);
        painter.setPen(pen);
        painter.setBrush(gradient(color(),rect()));

        double dvaluex = rect().x();
        double dvaluey = rect().y();
        double dvaluew = rect().width();
        double dvalueh = rect().height();

        dvaluex = dvaluex*dzoomx+dmovx;
        dvaluey = dvaluey*dzoomy+dmovy;
        dvaluew = dvaluew*dzoomx;
        dvalueh = dvalueh*dzoomy;
        QRect arect =QRect(dvaluex,dvaluey,dvaluew,dvalueh);
        QRect fontrect(arect.x(),arect.y()-10,200,10);
        arect.adjust(1, 1, -resizeHandleWidth/2, -resizeHandleWidth/2);
        // paint the QShape
        switch (type())
        {
            case QShape::Rectangle:
                painter.drawRect(arect);

                // paint the resize handle
                painter.setPen(pal.text().color());
                painter.setBrush(Qt::white);
                painter.drawRect(resizeHandlex( dmovx, dmovy,
                                                dangle, dzoomx, dzoomy).adjusted(0, 0, -1, -1));

                // paint the QShape name
                painter.setPen(Qt::darkGreen);
                painter.setBrush(Qt::darkGreen);
                if (type() == QShape::Triangle)
                    arect.adjust(0, arect.height()/2, 0, 0);

                painter.drawText(fontrect, Qt::AlignLeft,name());

                break;
            case QShape::Circle:
                painter.setRenderHint(QPainter::Antialiasing);
                painter.drawEllipse(arect);
                painter.setRenderHint(QPainter::Antialiasing, false);

                // paint the resize handle
                painter.setPen(pal.text().color());
                painter.setBrush(Qt::white);
                painter.drawRect(resizeHandlex( dmovx, dmovy,
                                                dangle, dzoomx, dzoomy).adjusted(0, 0, -1, -1));

                // paint the QShape name
                painter.setPen(Qt::darkGreen);
                painter.setBrush(Qt::darkGreen);
                if (type() == QShape::Triangle)
                    arect.adjust(0, arect.height()/2, 0, 0);
                painter.drawText(arect, Qt::AlignCenter,name());



                break;
            case QShape::Triangle:
                painter.setRenderHint(QPainter::Antialiasing);
                painter.drawPolygon(triangle(arect));
                painter.setRenderHint(QPainter::Antialiasing, false);

                // paint the resize handle
                painter.setPen(pal.text().color());
                painter.setBrush(Qt::white);
                painter.drawRect(resizeHandlex( dmovx, dmovy,
                                                dangle, dzoomx, dzoomy).adjusted(0, 0, -1, -1));

                // paint the QShape name
                painter.setPen(Qt::darkGreen);
                painter.setBrush(Qt::darkGreen);
                if (type() == QShape::Triangle)
                    arect.adjust(0, arect.height()/2, 0, 0);
                painter.drawText(arect, Qt::AlignCenter,name());
                break;
            case QShape::Font:
                QFont font = painter.font();
                font.setPixelSize(m_ifontsize);
                painter.setFont(font);
                painter.setPen(Qt::darkGreen);
                painter.setBrush(Qt::darkGreen);
                painter.drawText(arect, Qt::AlignCenter,name());
                break;
        }


}
void QShape::setfont(int isize)
{
    m_ifontsize = isize;
}
void QShape::shapesetroi(void *pshape)
{
     QShape *ashp=(QShape*)pshape;
     if(nullptr!=ashp)
     {
         QRect arect = ashp->rect();
         setrect(arect.x(),arect.y(),arect.width(),arect.height());
     }
}
