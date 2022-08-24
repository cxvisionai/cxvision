#include "imagecodeparser.h"
#include "fastmatch.h"


int imagecodeparser::m_curimagecodeparsernum = 0;
imagecodeparser::imagecodeparser()
{
    QString strname = QString("code%1").arg(m_curimagecodeparsernum);
     setname(strname.toStdString().c_str());
     m_curimagecodeparsernum = m_curimagecodeparsernum + 1;
    m_irotate=0;
    setcolor(0,255,120);
    m_decoder.setDecoder(QZXing::DecoderFormat_Aztec |
                       QZXing::DecoderFormat_CODABAR |
                       QZXing::DecoderFormat_CODE_39 |
                       QZXing::DecoderFormat_CODE_93 |
                       QZXing::DecoderFormat_EAN_8 |
                       QZXing::DecoderFormat_ITF |
                       QZXing::DecoderFormat_MAXICODE |
                       QZXing::DecoderFormat_PDF_417 |
                       QZXing::DecoderFormat_RSS_14 |
                       QZXing::DecoderFormat_RSS_EXPANDED |
                       QZXing::DecoderFormat_UPC_A |
                       QZXing::DecoderFormat_UPC_E |
                       QZXing::DecoderFormat_UPC_EAN_EXTENSION |
                       QZXing::DecoderFormat_CODE_128_GS1 |
                       QZXing::DecoderFormat_QR_CODE |
                       QZXing::DecoderFormat_DATA_MATRIX |
                       QZXing::DecoderFormat_EAN_13 |
                       QZXing::DecoderFormat_CODE_128
                       );
    m_ifindcode=0;
    m_iovergap=30;
    m_bsmoothtransformation=false;

}
imagecodeparser::~imagecodeparser()
{

}
void imagecodeparser::setdecoder(int hint)
{
    if(hint>0)
    m_decoder.setDecoder(hint);
    else
    m_decoder.setDecoder(QZXing::DecoderFormat_Aztec |
                       QZXing::DecoderFormat_CODABAR |
                       QZXing::DecoderFormat_CODE_39 |
                       QZXing::DecoderFormat_CODE_93 |
                       QZXing::DecoderFormat_EAN_8 |
                       QZXing::DecoderFormat_ITF |
                       QZXing::DecoderFormat_MAXICODE |
                       QZXing::DecoderFormat_PDF_417 |
                       QZXing::DecoderFormat_RSS_14 |
                       QZXing::DecoderFormat_RSS_EXPANDED |
                       QZXing::DecoderFormat_UPC_A |
                       QZXing::DecoderFormat_UPC_E |
                       QZXing::DecoderFormat_UPC_EAN_EXTENSION |
                       QZXing::DecoderFormat_CODE_128_GS1 |
                       QZXing::DecoderFormat_QR_CODE |
                       QZXing::DecoderFormat_DATA_MATRIX |
                       QZXing::DecoderFormat_EAN_13 |
                       QZXing::DecoderFormat_CODE_128
                       );


}
void imagecodeparser::setsmoothtransformation(int ismooth)
{
    if(ismooth>0)
        m_bsmoothtransformation=true;
    else
        m_bsmoothtransformation=false;


}

void imagecodeparser::setshow(int ishow)
{
    QShape::setshow(ishow);
}
void imagecodeparser::setrect(int ix,int iy,int iw,int ih)
{
   QShape::setrect(ix,iy,iw,ih);
}
void imagecodeparser::drawshape(QPainter &painter,QPalette &pal)
{
    if(show()&0x02)
    {
       painter.setPen(Qt::green);
       painter.setBrush(Qt::NoBrush);
       painter.drawRect(m_cxrect);
    }
    QShape::drawshape(painter,pal);
}
void imagecodeparser::drawshapex(QPainter &painter,QPalette &pal,
                      double dmovx,double dmovy,
                      double dangle,
                      double dzoomx,double dzoomy)
{

    if(show()&0x02)
    {
        painter.setPen(Qt::green);
        painter.setBrush(Qt::NoBrush);
        double dvaluex = m_cxrect.x();
        double dvaluey = m_cxrect.y();
        double dvaluew = m_cxrect.width();
        double dvalueh = m_cxrect.height();

        dvaluex = dvaluex*dzoomx+dmovx;
        dvaluey = dvaluey*dzoomy+dmovy;
        dvaluew = dvaluew*dzoomx;
        dvalueh = dvalueh*dzoomy;

        QRect arect =QRect(dvaluex,dvaluey,dvaluew,dvalueh);

       painter.drawRect(arect);
    }
    QShape::drawshapex(painter,pal,dmovx,dmovy,
                             dangle,dzoomx, dzoomy);
}

void imagecodeparser::measure0(void *pimage)
{
    ImageBase *pgetimage = (ImageBase*)pimage;

    Measure0(*pgetimage);
}
void imagecodeparser::Measure0(ImageBase &image)
{
    m_ifindcode=0;
    QImage aimageroi = image.copy(rect());
    QTransform matrix;
    matrix.translate(0, 0);
    matrix.scale(1, 1);
    matrix.rotate(m_irotate);
    QImage aimageroi2=aimageroi.transformed(matrix);
    m_result = m_decoder.decodeImage(aimageroi2);//,-1,-1,m_bsmoothtransformation);

   m_decodedFormat = m_decoder.GetDecoderFormat();
   m_charSet_ = m_decoder.GetCharSet();
   m_cxrect = m_decoder.GetRect();
   QString strname = m_decodedFormat+QString(" ")+m_charSet_+QString(" %1:").arg(m_curimagecodeparsernum)+m_result;
    setname(strname.toStdString().c_str());
}

void imagecodeparser::measure(void *pimage)
{
    ImageBase *pgetimage = (ImageBase*)pimage;

    Measure(*pgetimage);
}
void imagecodeparser::Measure(ImageBase &image)
{
    m_ifindcode=0;
    QImage aimageroi = image.copy(rect());
    if(0==m_irotate)
    {
        m_result = m_decoder.decodeImage(aimageroi,-1,-1,m_bsmoothtransformation);

        if(m_result.size()>3)
             m_ifindcode=1;
    }
     else if(0!=m_irotate)
    {
        QTransform matrix;
        matrix.translate(0, 0);
        matrix.scale(1, 1);
        matrix.rotate(m_irotate);
        QImage aimageroi2=aimageroi.transformed(matrix);
        m_result = m_decoder.decodeImage(aimageroi2,-1,-1,m_bsmoothtransformation);

        if(m_result.size()>3)
             m_ifindcode=1;
        if(m_ifindcode<1)
        {
            QRect arect = rect();
            arect.setRect(arect.x()+1,arect.y(),arect.width(),arect.height());
            aimageroi = image.copy(arect);
            QTransform matrix;
            matrix.translate(0, 0);
            matrix.scale(1, 1);
            matrix.rotate(m_irotate);
            QImage aimageroi2=aimageroi.transformed(matrix);
            m_result = m_decoder.decodeImage(aimageroi2,-1,-1,m_bsmoothtransformation);

            if(m_result.size()>3)
                 m_ifindcode=1;
        }
        if(m_ifindcode<1)
        {
            QRect arect = rect();
            arect.setRect(arect.x()+2,arect.y(),arect.width(),arect.height());
            aimageroi = image.copy(arect);
            QTransform matrix;
            matrix.translate(0, 0);
            matrix.scale(1, 1);
            matrix.rotate(m_irotate);
            QImage aimageroi2=aimageroi.transformed(matrix);
            m_result = m_decoder.decodeImage(aimageroi2,-1,-1,m_bsmoothtransformation);

            if(m_result.size()>3)
                 m_ifindcode=1;
        }
        if(m_ifindcode<1)
        {
            QRect arect = rect();
            arect.setRect(arect.x()+3,arect.y(),arect.width(),arect.height());
            aimageroi = image.copy(arect);
            QTransform matrix;
            matrix.translate(0, 0);
            matrix.scale(1, 1);
            matrix.rotate(m_irotate);
            QImage aimageroi2=aimageroi.transformed(matrix);
            m_result = m_decoder.decodeImage(aimageroi2,-1,-1,m_bsmoothtransformation);

            if(m_result.size()>3)
                 m_ifindcode=1;
        }
        if(m_ifindcode<1)
        {
            QRect arect = rect();
            arect.setRect(arect.x()-1,arect.y(),arect.width(),arect.height());
            aimageroi = image.copy(arect);
            QTransform matrix;
            matrix.translate(0, 0);
            matrix.scale(1, 1);
            matrix.rotate(m_irotate);
            QImage aimageroi2=aimageroi.transformed(matrix);
            m_result = m_decoder.decodeImage(aimageroi2,-1,-1,m_bsmoothtransformation);

            if(m_result.size()>3)
                 m_ifindcode=1;
        }
        if(m_ifindcode<1)
        {
            QRect arect = rect();
            arect.setRect(arect.x()-2,arect.y(),arect.width(),arect.height());
            aimageroi = image.copy(arect);
            QTransform matrix;
            matrix.translate(0, 0);
            matrix.scale(1, 1);
            matrix.rotate(m_irotate);
            QImage aimageroi2=aimageroi.transformed(matrix);
            m_result = m_decoder.decodeImage(aimageroi2,-1,-1,m_bsmoothtransformation);

            if(m_result.size()>3)
                 m_ifindcode=1;
        }
        if(m_ifindcode<1)
        {
            QRect arect = rect();
            arect.setRect(arect.x()-3,arect.y(),arect.width(),arect.height());
            aimageroi = image.copy(arect);
            QTransform matrix;
            matrix.translate(0, 0);
            matrix.scale(1, 1);
            matrix.rotate(m_irotate);
            QImage aimageroi2=aimageroi.transformed(matrix);
            m_result = m_decoder.decodeImage(aimageroi2,-1,-1,m_bsmoothtransformation);

            if(m_result.size()>3)
                 m_ifindcode=1;
        }
        /////////////////////////////////////
        if(m_ifindcode<1)
        {
            QRect arect = rect();
            arect.setRect(arect.x()+m_iovergap,arect.y(),arect.width(),arect.height());
            aimageroi = image.copy(arect);
            QTransform matrix;
            matrix.translate(0, 0);
            matrix.scale(1, 1);
            matrix.rotate(m_irotate);
            QImage aimageroi2=aimageroi.transformed(matrix);
            m_result = m_decoder.decodeImage(aimageroi2,-1,-1,m_bsmoothtransformation);

            if(m_result.size()>3)
                 m_ifindcode=1;
        }
        if(m_ifindcode<1)
        {
            QRect arect = rect();
            arect.setRect(arect.x()-m_iovergap,arect.y(),arect.width(),arect.height());
            aimageroi = image.copy(arect);
            QTransform matrix;
            matrix.translate(0, 0);
            matrix.scale(1, 1);
            matrix.rotate(m_irotate);
            QImage aimageroi2=aimageroi.transformed(matrix);
            m_result = m_decoder.decodeImage(aimageroi2,-1,-1,m_bsmoothtransformation);

            if(m_result.size()>3)
                 m_ifindcode=1;
        }
        if(m_ifindcode<1)
        {
            QRect arect = rect();
            arect.setRect(arect.x(),arect.y()+m_iovergap,arect.width(),arect.height());
            aimageroi = image.copy(arect);
            QTransform matrix;
            matrix.translate(0, 0);
            matrix.scale(1, 1);
            matrix.rotate(m_irotate);
            QImage aimageroi2=aimageroi.transformed(matrix);
            m_result = m_decoder.decodeImage(aimageroi2,-1,-1,m_bsmoothtransformation);

            if(m_result.size()>3)
                 m_ifindcode=1;
        }
        if(m_ifindcode<1)
        {
            QRect arect = rect();
            arect.setRect(arect.x(),arect.y()-m_iovergap,arect.width(),arect.height());
            aimageroi = image.copy(arect);
            QTransform matrix;
            matrix.translate(0, 0);
            matrix.scale(1, 1);
            matrix.rotate(m_irotate);
            QImage aimageroi2=aimageroi.transformed(matrix);
            m_result = m_decoder.decodeImage(aimageroi2,-1,-1,m_bsmoothtransformation);

            if(m_result.size()>3)
                 m_ifindcode=1;
        }

        ///////////////////////////////////////////////
        //rotate
        if(m_ifindcode<1)
        {
            QRect arect = rect();
            arect.setRect(arect.x(),arect.y(),arect.width(),arect.height());
            aimageroi = image.copy(arect);
            QTransform matrix;
            matrix.translate(0, 0);
            matrix.scale(1, 1);
            matrix.rotate(m_irotate+1);
            QImage aimageroi2=aimageroi.transformed(matrix);
            m_result = m_decoder.decodeImage(aimageroi2,-1,-1,m_bsmoothtransformation);

            if(m_result.size()>3)
                 m_ifindcode=1;
        }
        if(m_ifindcode<1)
        {
            QRect arect = rect();
            arect.setRect(arect.x(),arect.y(),arect.width(),arect.height());
            aimageroi = image.copy(arect);
            QTransform matrix;
            matrix.translate(0, 0);
            matrix.scale(1, 1);
            matrix.rotate(m_irotate+2);
            QImage aimageroi2=aimageroi.transformed(matrix);
            m_result = m_decoder.decodeImage(aimageroi2,-1,-1,m_bsmoothtransformation);

            if(m_result.size()>3)
                 m_ifindcode=1;
        }
        if(m_ifindcode<1)
        {
            QRect arect = rect();
            arect.setRect(arect.x()+3,arect.y(),arect.width(),arect.height());
            aimageroi = image.copy(arect);
            QTransform matrix;
            matrix.translate(0, 0);
            matrix.scale(1, 1);
            matrix.rotate(m_irotate+3);
            QImage aimageroi2=aimageroi.transformed(matrix);
            m_result = m_decoder.decodeImage(aimageroi2,-1,-1,m_bsmoothtransformation);

            if(m_result.size()>3)
                 m_ifindcode=1;
        }
        if(m_ifindcode<1)
        {
            QRect arect = rect();
            arect.setRect(arect.x(),arect.y(),arect.width(),arect.height());
            aimageroi = image.copy(arect);
            QTransform matrix;
            matrix.translate(0, 0);
            matrix.scale(1, 1);
            matrix.rotate(m_irotate-1);
            QImage aimageroi2=aimageroi.transformed(matrix);
            m_result = m_decoder.decodeImage(aimageroi2,-1,-1,m_bsmoothtransformation);

            if(m_result.size()>3)
                 m_ifindcode=1;
        }
        if(m_ifindcode<1)
        {
            QRect arect = rect();
            arect.setRect(arect.x(),arect.y(),arect.width(),arect.height());
            aimageroi = image.copy(arect);
            QTransform matrix;
            matrix.translate(0, 0);
            matrix.scale(1, 1);
            matrix.rotate(m_irotate-2);
            QImage aimageroi2=aimageroi.transformed(matrix);
            m_result = m_decoder.decodeImage(aimageroi2,-1,-1,m_bsmoothtransformation);

            if(m_result.size()>3)
                 m_ifindcode=1;
        }
        if(m_ifindcode<1)
        {
            QRect arect = rect();
            arect.setRect(arect.x(),arect.y(),arect.width(),arect.height());
            aimageroi = image.copy(arect);
            QTransform matrix;
            matrix.translate(0, 0);
            matrix.scale(1, 1);
            matrix.rotate(m_irotate-3);
            QImage aimageroi2=aimageroi.transformed(matrix);
            m_result = m_decoder.decodeImage(aimageroi2,-1,-1,m_bsmoothtransformation);

            if(m_result.size()>3)
                 m_ifindcode=1;
        }


    }

   m_decodedFormat = m_decoder.GetDecoderFormat();
   m_charSet_ = m_decoder.GetCharSet();
   m_cxrect = m_decoder.GetRect();
   QString strname = m_decodedFormat+QString(" ")+m_charSet_+QString(" %1:").arg(m_curimagecodeparsernum)+m_result;
    setname(strname.toStdString().c_str());
}
int imagecodeparser::findcodex()
{
    if(m_result.size()<1)
        return 0;
    if(m_result.mid(0,1)!="*")
        return 0;
    else
        return 1;
}
int imagecodeparser::coderule(int isize)
{
    if(m_result.size()!=isize)
        return 0;
    else
        return 1;
}
void imagecodeparser::setrelationrectfromresultnum(int inum)
{
    m_irelationresultnum = inum;
}
void imagecodeparser::setrelationrectfrom_matchresult(void *pmatch)
{
    m_prelationmatch = (fastmatch*)pmatch;
    if(0!=m_prelationmatch)
    {
        int inum = m_prelationmatch->getresultrects()->size();
        if(m_irelationresultnum<inum)
        {
            m_irelationrect = m_prelationmatch->getresultrects()->getrect(m_irelationresultnum);
        }
    }
}
void imagecodeparser::setrelationxy(int iprex1,int iprey1,int iendx1,int iendy1)
{
    m_irelationrect.setLeft(m_irelationrect.left() + iprex1);
    m_irelationrect.setTop(m_irelationrect.top() + iprey1);
    m_irelationrect.setRight(m_irelationrect.right() + iendx1);
    m_irelationrect.setBottom(m_irelationrect.bottom() + iendy1);


}
void imagecodeparser::setrelationzoom(double drelationzoomx,double drelationzoomy)
{
    m_irelationrect.setLeft((double)m_irelationrect.left() * drelationzoomx);
    m_irelationrect.setTop((double)m_irelationrect.top() * drelationzoomy);
    m_irelationrect.setRight((double)m_irelationrect.right() * drelationzoomx);
    m_irelationrect.setBottom((double)m_irelationrect.bottom() * drelationzoomy);
}
void imagecodeparser::setrelationtorect()
{
    if(m_irelationrect.x()>=0
            &&m_irelationrect.y()>=0
            &&m_irelationrect.width()>0
            &&m_irelationrect.height()>0)
     QShape::setrect(m_irelationrect.x(),
                     m_irelationrect.y(),
                     m_irelationrect.width(),
                     m_irelationrect.height());
}
void imagecodeparser::shapesetroi(void *pshape)
{
    QShape::shapesetroi(pshape);
}
