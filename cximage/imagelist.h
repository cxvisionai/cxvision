#ifndef IMAGELIST_H
#define IMAGELIST_H
#include "imagebase.h"
#include <QPainter>
#include <QWidget>


class ImageList
{

public:
    ImageList();
    ~ImageList();
    void setimagelist(int inum);

    QList<ImageBase>m_imagelist;
    ImageBase * getimage(int inum);

    ImageBase * nextimage();

    int m_ishow;
    int m_imagnum;
    int m_icurimgnum;

    int getcurimgenum(){return m_icurimgnum;}
    int nextcurimgenum();
    void setcurimgenum(int icurnum){m_icurimgnum=icurnum;}

    void settoimage(void *pimage);
    int size(){return m_imagelist.size();}
    void setshow(int ishow){m_ishow = ishow;}
    int getshow(){return m_ishow;}
    void draw(QPainter &painter);
    void drawx(QPainter &painter,
               double dmovx,
               double dmovy,
               double dangle,
               double dzoomx,
               double dzoomy);
    void save();
    void load();
    void clearimage();
};

#endif
