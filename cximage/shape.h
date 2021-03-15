#ifndef SHAPE_H
#define SHAPE_H

#include <QWidget>

class QShape
{
public:
    enum Type { Rectangle = 0, Circle =1, Triangle =2 };

    explicit QShape(Type type = Rectangle, const QColor &color = Qt::red, const QRect &rect = QRect());

   virtual void drawshape(QPainter &painter,QPalette &pal) const;
   virtual void setrect(int ix,int iy,int iw,int ih);

    void setshow(int ishow);
    void settype(int itype);
    void setname(const char *pname);
    void setcolor(int ir,int ig ,int ib);

    Type type() const;
    QString name() const;
    QRect rect() const;
    QRect resizeHandle() const;
    QColor color() const;
    int show() const;

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

};



#endif // SHAPE_H
