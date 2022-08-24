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

#ifndef SHAPE_H
#define SHAPE_H

#include <QWidget>

class QShape
{
public:
    enum Type { Rectangle = 0, Circle =1, Triangle =2,Font = 3 };

    explicit QShape(Type type = Rectangle, const QColor &color = Qt::red, const QRect &rect = QRect());

   virtual void drawshape(QPainter &painter,QPalette &pal) const;

   virtual void setrect(int ix,int iy,int iw,int ih);
   virtual void setshowlines(int ilines){m_ishowlines=ilines;}
    void drawshapex(QPainter &painter,QPalette &pal,double dmovx,double dmovy,
                    double dangle,double dzoomx,double dzoomy) const;

    void setshow(int ishow);
    void settype(int itype);
    void setname(const char *pname);
    void setcolor(int ir,int ig ,int ib);
    void setfont(int isize);

    Type type() const;
    QString name() const;
    QRect rect() const;
    QRect resizeHandlez(double dzoomx,double dzoomy) const;
    QRect resizeHandle() const;
    QRect resizeHandlex(double dmovx,double dmovy,
                        double dangle,double dzoomx,double dzoomy) const;

    QColor color() const;
    int show() const;
    void shapesetroi(void *pshape);
    static QString typeToString(Type type);
    static Type stringToType(const QString &s, bool *ok = 0);
    static const QSize minSize;

private:
    Type m_type;
    QRect m_rect;
    QColor m_color;
    QString m_name;
    int m_ipenwidth;
    int m_ishow;
    int m_ishowlines;
    int m_ifontsize;
    friend class visionmanager;
};



#endif // SHAPE_H
