
#include <QMap>
#include"imagebase.h"
#include"CONST_COLOR_ISO_AtanAngle.H"
#include"CONST_COLOR_ISO_SqrtX.H"
#include"backimagemanager.h"
#include"shape.h"
#include"imagetype.h"
#include<QApplication>
#include<QClipboard>
#include<QFileDialog>


ImageBase::ImageBase()
    :QImage(),
      m_ishow(0),
      m_mode(QPainter::CompositionMode_SourceOver)
{
}

ImageBase::ImageBase(const QImage & image)
    :QImage(image),
      m_ishow(0),
      m_mode(QPainter::CompositionMode_SourceOver)
{
}

ImageBase::ImageBase(const QSize &size, Format format)
    :QImage(size,format),
      m_ishow(0),
      m_mode(QPainter::CompositionMode_SourceOver)

{
}

void ImageBase::draw(QPainter &painter)
{
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawImage(0, 0,*this);
 //   painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
}
void ImageBase::drawx(QPainter &painter,
                      double dmovx,
                      double dmovy,
                      double dangle,
                      double dzoomx,
                      double dzoomy)
{
    painter.save();
    painter.translate(dmovx, dmovy);
    painter.rotate(dangle);
    painter.scale(dzoomx, dzoomy);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawImage(0, 0,*this);
    painter.restore();
}

thrddouble DotISO_FAST(int R1,int G1,int B1)
{
    thrddouble dotiso;
    int max[3];
    int temp;
    char Smax[3];
    char stemp;
    max[0]=R1;
    max[1]=B1;
    max[2]=G1;
    Smax[0]='R';
    Smax[1]='B';
    Smax[2]='G';
    if(max[1]>max[0])
    {
        temp=max[0];
        max[0]=max[1];
        max[1]=temp;

        stemp=Smax[0];
        Smax[0]=Smax[1];
        Smax[1]=stemp;
    }
    if(max[2]>max[1])
    {
        temp=max[1];
        max[1]=max[2];
        max[2]=temp;

        stemp=Smax[1];
        Smax[1]=Smax[2];
        Smax[2]=stemp;
    }
    if(max[1]>max[0])
    {
        temp=max[0];
        max[0]=max[1];
        max[1]=temp;

        stemp=Smax[0];
        Smax[0]=Smax[1];
        Smax[1]=stemp;
    }
    int iH = (max[0]+max[1]+max[2]);
    double H=intx0_5(iH) +intx0_062(iH) +intx0_015(iH);

    int snum=(max[1]*max[1]+max[2]*max[2]+max[0]*max[0]-max[0]*max[2]-max[1]*max[2]-max[1]*max[0]);
    double S=ISOsqrtX[snum];
    int S1=(((max[0])<<1)-(max[1]+max[2]));
    int S2=(max[1]-max[2]);
    double a=ISOAtanAngle[S1][S2];

    a=(a-30)>0?(a-30):0;
    if(Smax[0]== 'R')
    {
        if(Smax[1]=='B')
            a=a+180;
        else
            a=300-a;//360+(360-a);
    }
    else
    {
        if(Smax[0]== 'B')
        {
            if(Smax[1]=='G')
                a=a+60;//120+a;
            else
                a=180-a;//360+120-a;
        }
        else
        {
            if(Smax[0]=='G')
            {
                if(Smax[1]=='R')
                    a=a+300;//240+a;
                else
                    a=60-a;//360+240-a;
            }
            else
            {
                //ASSERT(0);
                //AfxMessageBox("!!!!!!!!");
            }
        }
    }
    dotiso.varH=H ;
    dotiso.varS=S ;
    dotiso.varO=a ;
    return dotiso;
}
void ImageBase::ImageGray()
{
        for(int x(0); x < width(); x++)
        {
            for(int y(0); y <  height(); y++)
            {
              setPixel(x, y, qGray(pixel(x, y)));
            }
        }
}


void ImageBase::ImageThre(int ithre)
{
    QRgb pixel0;
        for(int x(0); x < width(); x++)
        {
            for(int y(0); y <  height(); y++)
            {
                QRgb pixelCur =  pixel(x, y);

                if(qRed(pixelCur)>ithre
                ||qGreen(pixelCur)>ithre
                ||qBlue(pixelCur)>ithre)
                    pixel0 = qRgb(255, 255, 255);
                else
                    pixel0 = qRgb(0, 0, 0);
              setPixel(x, y, pixel0);
            }
        }
}
void ImageBase::Image_5Blur_Gap_mud_thre_BW(int ithre,int increase,int igap,int ifindBorW)
{

    QRgb pixelCur,pixel_1,pixel_2,pixel1,pixel2;
    QRgb pixel0;
    //Gausblur
    for(int y(1); y <  height(); y++)
    {
       {
            pixelCur =  pixel(0, y);
            pixel_1 =  pixel(1, y);
            pixel_2 =  pixel(2, y);
            pixel1 =  pixel(1, y);
            pixel2 =  pixel(2, y);
            int ir0 = qRed(pixelCur);int ig0 = qGreen(pixelCur);int ib0 = qBlue(pixelCur);
            int ir_1 = qRed(pixel_1);int ig_1 = qGreen(pixel_1);int ib_1 = qBlue(pixel_1);
            int ir_2 = qRed(pixel_2);int ig_2 = qGreen(pixel_2);int ib_2 = qBlue(pixel_2);
            int ir1 = qRed(pixel1);int ig1 = qGreen(pixel1);int ib1 = qBlue(pixel1);
            int ir2 = qRed(pixel2);int ig2 = qGreen(pixel2);int ib2 = qBlue(pixel2);
            int ir = intx0_125(ir_2)+intx0_25(ir_1)+intx0_25(ir0)+intx0_062(ir0)+intx0_25(ir1)+intx0_125(ir2);
            int ig = intx0_125(ig_2)+intx0_25(ig_1)+intx0_25(ig0)+intx0_062(ig0)+intx0_25(ig1)+intx0_125(ig2);
            int ib = intx0_125(ib_2)+intx0_25(ib_1)+intx0_25(ib0)+intx0_062(ib0)+intx0_25(ib1)+intx0_125(ib2);
            ir = ir >255? 255:ir;
            ig = ig >255? 255:ig;
            ib = ib >255? 255:ib;
            pixel0 = qRgb(ir, ig, ib);
            setPixel(0, y-1, pixel0);
       }
       {
            pixelCur =  pixel(1, y);
            pixel_1 =  pixel(0, y);
            pixel_2 =  pixel(0, y);
            pixel1 =  pixel(2, y);
            pixel2 =  pixel(3, y);
            int ir0 = qRed(pixelCur);int ig0 = qGreen(pixelCur);int ib0 = qBlue(pixelCur);
            int ir_1 = qRed(pixel_1);int ig_1 = qGreen(pixel_1);int ib_1 = qBlue(pixel_1);
            int ir_2 = qRed(pixel_2);int ig_2 = qGreen(pixel_2);int ib_2 = qBlue(pixel_2);
            int ir1 = qRed(pixel1);int ig1 = qGreen(pixel1);int ib1 = qBlue(pixel1);
            int ir2 = qRed(pixel2);int ig2 = qGreen(pixel2);int ib2 = qBlue(pixel2);
            int ir = intx0_125(ir_2)+intx0_25(ir_1)+intx0_25(ir0)+intx0_062(ir0)+intx0_25(ir1)+intx0_125(ir2);
            int ig = intx0_125(ig_2)+intx0_25(ig_1)+intx0_25(ig0)+intx0_062(ig0)+intx0_25(ig1)+intx0_125(ig2);
            int ib = intx0_125(ib_2)+intx0_25(ib_1)+intx0_25(ib0)+intx0_062(ib0)+intx0_25(ib1)+intx0_125(ib2);
            ir = ir >255? 255:ir;
            ig = ig >255? 255:ig;
            ib = ib >255? 255:ib;
            pixel0 = qRgb(ir, ig, ib);
            setPixel(1, y-1, pixel0);
       }
        {
             pixelCur =  pixel(width()-2, y);
             pixel_1 =  pixel(width()-3, y);
             pixel_2 =  pixel(width()-4, y);
             pixel1 =  pixel(width()-1, y);
             pixel2 =  pixel(width()-1, y);
             int ir0 = qRed(pixelCur);int ig0 = qGreen(pixelCur);int ib0 = qBlue(pixelCur);
             int ir_1 = qRed(pixel_1);int ig_1 = qGreen(pixel_1);int ib_1 = qBlue(pixel_1);
             int ir_2 = qRed(pixel_2);int ig_2 = qGreen(pixel_2);int ib_2 = qBlue(pixel_2);
             int ir1 = qRed(pixel1);int ig1 = qGreen(pixel1);int ib1 = qBlue(pixel1);
             int ir2 = qRed(pixel2);int ig2 = qGreen(pixel2);int ib2 = qBlue(pixel2);
             int ir = intx0_125(ir_2)+intx0_25(ir_1)+intx0_25(ir0)+intx0_062(ir0)+intx0_25(ir1)+intx0_125(ir2);
             int ig = intx0_125(ig_2)+intx0_25(ig_1)+intx0_25(ig0)+intx0_062(ig0)+intx0_25(ig1)+intx0_125(ig2);
             int ib = intx0_125(ib_2)+intx0_25(ib_1)+intx0_25(ib0)+intx0_062(ib0)+intx0_25(ib1)+intx0_125(ib2);
             ir = ir >255? 255:ir;
             ig = ig >255? 255:ig;
             ib = ib >255? 255:ib;
             pixel0 = qRgb(ir, ig, ib);
             setPixel(width()-2, y-1, pixel0);
        }
        {
             pixelCur =  pixel(width()-1, y);
             pixel_1 =  pixel(width()-2, y);
             pixel_2 =  pixel(width()-3, y);
             pixel1 =  pixel(width()-2, y);
             pixel2 =  pixel(width()-3, y);
             int ir0 = qRed(pixelCur);int ig0 = qGreen(pixelCur);int ib0 = qBlue(pixelCur);
             int ir_1 = qRed(pixel_1);int ig_1 = qGreen(pixel_1);int ib_1 = qBlue(pixel_1);
             int ir_2 = qRed(pixel_2);int ig_2 = qGreen(pixel_2);int ib_2 = qBlue(pixel_2);
             int ir1 = qRed(pixel1);int ig1 = qGreen(pixel1);int ib1 = qBlue(pixel1);
             int ir2 = qRed(pixel2);int ig2 = qGreen(pixel2);int ib2 = qBlue(pixel2);
             int ir = intx0_125(ir_2)+intx0_25(ir_1)+intx0_25(ir0)+intx0_062(ir0)+intx0_25(ir1)+intx0_125(ir2);
             int ig = intx0_125(ig_2)+intx0_25(ig_1)+intx0_25(ig0)+intx0_062(ig0)+intx0_25(ig1)+intx0_125(ig2);
             int ib = intx0_125(ib_2)+intx0_25(ib_1)+intx0_25(ib0)+intx0_062(ib0)+intx0_25(ib1)+intx0_125(ib2);
             ir = ir >255? 255:ir;
             ig = ig >255? 255:ig;
             ib = ib >255? 255:ib;
             pixel0 = qRgb(ir, ig, ib);
             setPixel(width()-1, y-1, pixel0);
        }
       for(int x(2); x < width()-2; x++)
       {
            pixelCur =  pixel(x, y);
            pixel_1 =  pixel(x-1, y);
            pixel_2 =  pixel(x-2, y);
            pixel1 =  pixel(x+1, y);
            pixel2 =  pixel(x+2, y);
            int ir0 = qRed(pixelCur);int ig0 = qGreen(pixelCur);int ib0 = qBlue(pixelCur);
            int ir_1 = qRed(pixel_1);int ig_1 = qGreen(pixel_1);int ib_1 = qBlue(pixel_1);
            int ir_2 = qRed(pixel_2);int ig_2 = qGreen(pixel_2);int ib_2 = qBlue(pixel_2);
            int ir1 = qRed(pixel1);int ig1 = qGreen(pixel1);int ib1 = qBlue(pixel1);
            int ir2 = qRed(pixel2);int ig2 = qGreen(pixel2);int ib2 = qBlue(pixel2);


            int ir = intx0_125(ir_2)+intx0_25(ir_1)+intx0_25(ir0)+intx0_062(ir0)+intx0_25(ir1)+intx0_125(ir2);
            int ig = intx0_125(ig_2)+intx0_25(ig_1)+intx0_25(ig0)+intx0_062(ig0)+intx0_25(ig1)+intx0_125(ig2);
            int ib = intx0_125(ib_2)+intx0_25(ib_1)+intx0_25(ib0)+intx0_062(ib0)+intx0_25(ib1)+intx0_125(ib2);

            ir = ir >255? 255:ir;
            ig = ig >255? 255:ig;
            ib = ib >255? 255:ib;

            QRgb pixel0 = qRgb(ir, ig, ib);


            setPixel(x, y-1, pixel0);
        }
    }

    QRgb pixelx;
    //GAP MUD THRE
    if(0==ifindBorW)
    {
        for(int y(height()-2); y >0  ; y--)
        {
            for(int x(0); x < width()-igap; x++)
            {
                QRgb pixel1 =  pixel(x, y);
                QRgb pixel2 =  pixel(x+igap, y);
                int ir = qRed(pixel2) - qRed(pixel1);
                int ig = qGreen(pixel2) - qGreen(pixel1);
                int ib = qBlue(pixel2) - qBlue(pixel1);
                if(ir>ithre||ig>ithre||ib>ithre)
                //if(qGray(pixel2)-qGray(pixel1)>ithre)
                    pixelx = qRgb(255, 255, 255);
                else
                    pixelx = qRgb(0, 0, 0);
               // ir = ir > ithre ? 255:0;
               // ig = ig > ithre ? 255:0;
               // ib = ib > ithre ? 255:0;
                setPixel(x, y+1, pixelx);
            }
        }
    }
    else if(1==ifindBorW)
    {
        for(int y(height()-2); y >0  ; y--)
        {
            for(int x(0); x < width()-igap; x++)
            {
                QRgb pixel1 =  pixel(x, y);
                QRgb pixel2 =  pixel(x+igap, y);
                int ir = qRed(pixel1) - qRed(pixel2);
                int ig = qGreen(pixel1) - qGreen(pixel2);
                int ib = qBlue(pixel1) - qBlue(pixel2);
                if(ir>ithre||ig>ithre||ib>ithre)
                //if(qGray(pixel1)-qGray(pixel2))
                    pixelx = qRgb(255, 255, 255);
                else
                    pixelx = qRgb(0, 0, 0);
               // ir = ir > ithre ? 255:0;
               // ig = ig > ithre ? 255:0;
               // ib = ib > ithre ? 255:0;
                setPixel(x, y+1, pixelx);
            }
        }

    }


}
void ImageBase::ImageClear(int ibit)
{
    if(0==ibit)
    {
        for(int x(0); x < width(); x++)
        {
            for(int y(0); y <  height(); y++)
            {
               QRgb  pixel0 = qRgb(0, 0, 0);
                setPixel(x, y, pixel0);
            }
        }
    }
    else
    {
        for(int x(0); x < width(); x++)
        {
            for(int y(0); y <  height(); y++)
            {
                QRgb pixel0 = qRgb(255, 255, 255);
                setPixel(x, y, pixel0);
            }
        }
    }
}
void ImageBase::ImageBar(int itimes,int ifade)
{
    int ibarsize = 0;
//  2x2 3x3
//    H
//   GBI
//  FA0CJ
//   EDK
//    L
    for(int i=0;i<itimes;i++)
    {
        for(int x(1); x < width()-1; x++)
        {
            for(int y(1); y <  height()-1; y++)
            {
                QRgb pixelCur =  pixel(x, y);
                QRgb pixelR =  pixel(x-1, y);
                QRgb pixelL =  pixel(x, y-1);
                QRgb pixelU =  pixel(x+1, y);
                QRgb pixelD =  pixel(x, y+1);
                int ir = (qRed(pixelCur)+qRed(pixelR)+qRed(pixelL)+qRed(pixelU)+qRed(pixelD))/5;
                int ig = (qGreen(pixelCur)+qGreen(pixelR)+qGreen(pixelL)+qGreen(pixelU)+qGreen(pixelD))/5;
                int ib = (qBlue(pixelCur)+qBlue(pixelR)+qBlue(pixelL)+qBlue(pixelU)+qBlue(pixelD))/5;

                QRgb pixel0 = qRgb(ir, ig, ib);

                setPixel(x-1, y-1, pixel0);
            }
        }
    }
}
void ImageBase::Image_ColorThreshold(int ir,int ig,int ib,int ihgap,int isgap,int iogap)
{
    double d_h_0,d_s_0,d_o_0;

    DOTISO_MAC(ir,ig,ib,d_h_0,d_s_0,d_o_0);

    int r, g, b;
    double d_h,d_s,d_o;
    thrddouble dotiso;

    for (int i = 0; i < width(); i++)
    {
        for (int j = 0; j < height(); j++)
        {
            QRgb pixelCur = pixel(i, j);
            r=qRed(pixelCur);
            g=qGreen(pixelCur);
            b=qBlue(pixelCur);
            dotiso=DotISO_FAST(r,g,b);

            d_h=dotiso.varH -d_h_0;
            d_s=dotiso.varS -d_s_0;
            d_o=dotiso.varO -d_o_0;
            d_h=d_h>0?d_h:-d_h;
            d_s=d_s>0?d_s:-d_s;
            d_o=d_o>0?d_o:-d_o;
            d_o=360-d_o>d_o?d_o:360-d_o;
            if(d_h>ihgap||d_s>isgap||d_o>iogap)
                setPixel(i, j, qRgb(255,255,255));
            else
                setPixel(i, j, qRgb(0,0,0));
        }
    }
}
void ImageBase::ImageMulDiv(double dvalue)
{
    QRgb pixel0;
        for(int x(0); x < width(); x++)
        {
            for(int y(0); y <  height(); y++)
            {
               QRgb pixelCur =  pixel(x, y);
               float fr = qRed(pixelCur);
               float fg = qGreen(pixelCur);
               float fb = qBlue(pixelCur);
               int ir = fr *dvalue;
               int ig = fg *dvalue;
               int ib = fb *dvalue;
               pixel0 = qRgb(ir, ig, ib);

               setPixel(x, y, pixel0);
            }
        }
}
void ImageBase::SetMode(int imode)
{
    switch (imode)
    {
        case 0:m_mode=QPainter::CompositionMode_SourceOver;break;
        case 1:m_mode=QPainter::CompositionMode_DestinationOver;break;
        case 2:m_mode=QPainter::CompositionMode_Clear;break;
        case 3:m_mode=QPainter::CompositionMode_Source;break;
        case 4:m_mode=QPainter::CompositionMode_Destination;break;
        case 5:m_mode=QPainter::CompositionMode_SourceIn;break;
        case 6:m_mode=QPainter::CompositionMode_DestinationIn;break;
        case 7:m_mode=QPainter::CompositionMode_SourceOut;break;
        case 8:m_mode=QPainter::CompositionMode_DestinationOut;break;
        case 9:m_mode=QPainter::CompositionMode_SourceAtop;break;
        case 10:m_mode=QPainter::CompositionMode_DestinationAtop;break;
        case 11:m_mode=QPainter::CompositionMode_Xor;break;

        //svg 1.2 blend modes
        case 12:m_mode=QPainter::CompositionMode_Plus;break;
        case 13:m_mode=QPainter::CompositionMode_Multiply;break;
        case 14:m_mode=QPainter::CompositionMode_Screen;break;
        case 15:m_mode=QPainter::CompositionMode_Overlay;break;
        case 16:m_mode=QPainter::CompositionMode_Darken;break;
        case 17:m_mode=QPainter::CompositionMode_Lighten;break;
        case 18:m_mode=QPainter::CompositionMode_ColorDodge;break;
        case 19:m_mode=QPainter::CompositionMode_ColorBurn;break;
        case 20:m_mode=QPainter::CompositionMode_HardLight;break;
        case 21:m_mode=QPainter::CompositionMode_SoftLight;break;
        case 22:m_mode=QPainter::CompositionMode_Difference;break;
        case 23:m_mode=QPainter::CompositionMode_Exclusion;break;

        // ROPs
        case 24:m_mode=QPainter::RasterOp_SourceOrDestination;break;
        case 25:m_mode=QPainter::RasterOp_SourceAndDestination;break;
        case 26:m_mode=QPainter::RasterOp_SourceXorDestination;break;
        case 27:m_mode=QPainter::RasterOp_NotSourceAndNotDestination;break;
        case 28:m_mode=QPainter::RasterOp_NotSourceOrNotDestination;break;
        case 29:m_mode=QPainter::RasterOp_NotSourceXorDestination;break;
        case 30:m_mode=QPainter::RasterOp_NotSource;break;
        case 31:m_mode=QPainter::RasterOp_NotSourceAndDestination;break;
        case 32:m_mode=QPainter::RasterOp_SourceAndNotDestination;break;
        case 33:m_mode=QPainter::RasterOp_NotSourceOrDestination;break;
        case 34:m_mode=QPainter::RasterOp_SourceOrNotDestination;break;
        case 35:m_mode=QPainter::RasterOp_ClearDestination;break;
        case 36:m_mode=QPainter::RasterOp_SetDestination;break;
        case 37:m_mode=QPainter::RasterOp_NotDestination;break;
        default:        break;
    }

}
void ImageBase::ImageCopy(QImage &image)
{
   int iw1 = image.width();
   int ih1 = image.height();
   if(width()< m_ix0 + iw1
    || height() < m_iy0 + ih1)
        return;//error process

    QPainter painter(&image);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(m_ix0, m_iy0, *this);
    painter.end();
}
void ImageBase::ImageAnd(void *paimage)
{
    ImageBase *pimage = (ImageBase*)paimage;
    int iw1 = pimage->width();
    int ih1 = pimage->height();
    int iw0 = width();
    int ih0 = height();
    if(iw1!=iw0
     ||ih1!=ih0)
    {
        QSize imagesize(iw1,ih1);
        QImage aimage(imagesize, QImage::Format_ARGB32_Premultiplied);
        *this = aimage;
    }
    QPainter painter(this);
    painter.setCompositionMode(m_mode);
    painter.drawImage(0, 0, *pimage);
    painter.end();
   /*
    QPainter::CompositionMode mode = currentMode();

    QPainter painter(&resultImage);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(resultImage.rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, destinationImage);
    painter.setCompositionMode(mode);
    painter.drawImage(0, 0, sourceImage);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    painter.fillRect(resultImage.rect(), Qt::white);
    painter.end();
    *
    *
    *
    QPainter painter(&resultImage);
    painter.setCompositionMode(QPainter::case 0:m_mode=QPainter::CompositionMode_Source);
    painter.fillRect(resultImage.rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::case 0:m_mode=QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, destinationImage);
    painter.setCompositionMode(mode);
    painter.drawImage(0, 0, sourceImage);
    painter.setCompositionMode(QPainter::case 0:m_mode=QPainter::CompositionMode_DestinationOver);
    painter.fillRect(resultImage.rect(), Qt::white);
    painter.end();
    */
}
//void ImageBase::ImageXOR(ImageBase*pimage){}
//void ImageBase::ImageAND(ImageBase*pimage){}
//void ImageBase::ImageNOT(ImageBase*pimage){}
void ImageBase::ImageMoveAndX(int ix)
{
    QRgb pixel0;
    int iv = ix >0 ? 1:-1;
    for(int i=0;i<ix;i++)
    for(int x(0); x < width(); x++)
        {
            for(int y(0); y <  height(); y++)
            {
                   QRgb pixelCur1 =  pixel(x, y);
                   QRgb pixelCur2 =  pixel(x+iv, y);
                   pixel0 = pixelCur1 & pixelCur2;
                   setPixel(x, y, pixel0);
            }
        }
}
void ImageBase::ImageMoveAndY(int ix)
{
    QRgb pixel0;
    int iv = ix >0 ? 1:-1;

    for(int i=0;i<ix;i++)
    for(int x(0); x < width(); x++)
        {
            for(int y(0); y <  height(); y++)
            {
                   QRgb pixelCur1 =  pixel(x, y);
                   QRgb pixelCur2 =  pixel(x, y+iv);
                   pixel0 = pixelCur1 & pixelCur2;
                   setPixel(x, y, pixel0);
            }
        }
}
void ImageBase::ImageMoveOrX(int ix)
{
    QRgb pixel0;
    int iv = ix >0 ? 1:-1;

    for(int i=0;i<ix;i++)
    for(int x(0); x < width(); x++)
        {
            for(int y(0); y <  height(); y++)
            {
                   QRgb pixelCur1 =  pixel(x, y);
                   QRgb pixelCur2 =  pixel(x+iv, y);
                   pixel0 = pixelCur1 | pixelCur2;
                   setPixel(x, y, pixel0);
            }
        }
}
void ImageBase::ImageMoveOrY(int ix)
{
    QRgb pixel0;
    int iv = ix >0 ? 1:-1;
    for(int i=0;i<ix;i++)
    for(int x(0); x < width(); x++)
        {
            for(int y(0); y <  height(); y++)
            {
                   QRgb pixelCur1 =  pixel(x, y);
                   QRgb pixelCur2 =  pixel(x, y+iv);
                   pixel0 = pixelCur1 | pixelCur2;
                   setPixel(x, y, pixel0);
            }
        }
}

void ImageBase::Image_Or(int ix)
{

        ImageMoveOrX(ix);
        ImageMoveOrY(ix);
        ImageMoveOrX(-ix);
        ImageMoveOrY(-ix);

}
void ImageBase::Image_And(int ix)
{

    ImageMoveAndX(-ix);
        ImageMoveAndY(-ix);
        ImageMoveAndX(ix);
        ImageMoveAndY(ix);

}



void ImageBase::ImageIncrease(int inum)
{
    QRgb pixel0;
        for(int x(0); x < width(); x++)
        {
            for(int y(0); y <  height(); y++)
            {
               QRgb pixelCur =  pixel(x, y);

               pixel0 = qRgb(qRed(pixelCur)+inum, qGreen(pixelCur)+inum, qBlue(pixelCur)+inum);

               setPixel(x, y, pixel0);
            }
        }
}
void ImageBase::ImageDecrease(int inum)
{
    QRgb pixel0;
        for(int x(0); x < width(); x++)
        {
            for(int y(0); y <  height(); y++)
            {
               QRgb pixelCur =  pixel(x, y);

               pixel0 = qRgb(qRed(pixelCur)-inum, qGreen(pixelCur)-inum, qBlue(pixelCur)-inum);

               setPixel(x, y, pixel0);
            }
        }

}
void ImageBase::ImageFilp()
{
    QRgb pixel0;
    int ihalfh = (height()-1) /2;

    for(int x(0); x < width(); x++)
        {
            for(int y(0); y <  ihalfh; y++)
            {
               QRgb pixel0 =  pixel(x, y);
               QRgb pixel1 =  pixel(x, height()-1-y);

               setPixel(x, y, pixel1);
               setPixel(x, height()-1-y, pixel0);
            }
        }
}
void ImageBase::ImageMirror()
{
    QRgb pixel0;
    int ihalfw = width() /2;

    for(int x(0); x < ihalfw; x++)
        {
            for(int y(0); y <  height(); y++)
            {
               QRgb pixel0 =  pixel(x, y);
               QRgb pixel1 =  pixel(width()-x, y);

               setPixel(x, y, pixel1);
               setPixel(width()-x, y, pixel0);
            }
        }
}

void ImageBase::SetROI(int ix,int iy,int iw,int ih)
{
    int iw0 = width();
    int ih0 = height();
    if(ix>iw0||iy>ih0)
        return;
    m_ix0 = ix;
    m_iy0 = iy;
    if(ix+iw>=iw0)
        m_iw = iw0 - ix - 1;
    else
        m_iw = iw;
    if(iy+ih>=ih0)
        m_ih = ih0 - iy - 1;
    else
        m_ih = ih;
}
int ImageBase::PixelCompare(QRgb &pixel1,QRgb &pixel2,
                            int ihgap,int isgap,int iogap)
{

    int max[3];
    int temp;
    char Smax[3];
    char stemp;

    max[0] =qRed(pixel1) - qRed(pixel2);
    max[1] =qGreen(pixel1) - qGreen(pixel2);
    max[2] =qBlue(pixel1) - qBlue(pixel2);

    max[0] = max[0] >0 ?max[0]:-max[0];
    max[1] = max[1] >0 ?max[1]:-max[1];
    max[2] = max[2] >0 ?max[2]:-max[2];

    Smax[0]='R';
    Smax[1]='B';
    Smax[2]='G';
    if(max[1]>max[0])
    {
        temp=max[0];
        max[0]=max[1];
        max[1]=temp;

        stemp=Smax[0];
        Smax[0]=Smax[1];
        Smax[1]=stemp;
    }
    if(max[2]>max[1])
    {
        temp=max[1];
        max[1]=max[2];
        max[2]=temp;

        stemp=Smax[1];
        Smax[1]=Smax[2];
        Smax[2]=stemp;
    }
    if(max[1]>max[0])
    {
        temp=max[0];
        max[0]=max[1];
        max[1]=temp;

        stemp=Smax[0];
        Smax[0]=Smax[1];
        Smax[1]=stemp;
    }

    int h= max[0]+max[1]+max[2];//iH = r+g+b;
    //snum=(max[1]*max[1]+max[2]*max[2]+max[0]*max[0]-max[0]*max[2]-max[1]*max[2]-max[1]*max[0]);
    int s=(((max[0])<<1)-(max[1]+max[2]));//ISOsqrtX[snum];
    int S1=(((max[0])<<1)-(max[1]+max[2]));
    int S2=(max[1]-max[2]);
    int a=ISOAtanAngle[S1][S2];
    a=(a-30)>0?(a-30):0;
    switch(Smax[0])
    {
    case 'R':
        a = (Smax[1]=='B')?a+180:300-a;
        break;
    case 'B':
        a = (Smax[1]=='G')? a+60:180-a;
        break;
    case 'G':
        a = (Smax[1]=='R')? a+300:60-a;
        break;
    }
   // a2 = 360+a;

    if((a<iogap)
        &&(h<ihgap)
        &&(s<isgap))
        return 1;
    else
        return 0;
}

void ImageBase::ROIImageThre(int ithre)
{

    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;

    QRgb pixel0;
        for(int x(m_ix0); x < m_ix0+m_iw; x++)
        {
            for(int y(m_iy0); y <  m_iy0+m_ih; y++)
            {
                QRgb pixelCur =  pixel(x, y);

                if(qRed(pixelCur)>ithre
                ||qGreen(pixelCur)>ithre
                ||qBlue(pixelCur)>ithre)
                    pixel0 = qRgb(255, 255, 255);
                else
                    pixel0 = qRgb(0, 0, 0);
              setPixel(x , y , pixel0);
            }
        }
}
void ImageBase::ROIImageRGBThre(int iRthre,int iGthre,int iBthre,int iandor)
{
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;

    QRgb pixel0;
    int ir,ig,ib;
        for(int x(m_ix0); x < m_ix0+m_iw; x++)
        {
            for(int y(m_iy0); y <  m_iy0+m_ih; y++)
            {
                QRgb pixelCur =  pixel(x, y);

                if(qRed(pixelCur)>iRthre)
                    ir =255;
                else
                    ir = 0;
                if(qGreen(pixelCur)>iGthre)
                    ig =255;
                else
                    ig = 0;
                if(qBlue(pixelCur)>iBthre)
                    ib =255;
                else
                    ib = 0;
              if(0==iandor)
              {
                  pixel0 = qRgb(ir, ig, ib);
                  setPixel(x , y , pixel0);

              }
              else if(1==iandor)
              {

                  int ivalue = ir | ig | ib;
                  pixel0 = qRgb(ivalue, ivalue, ivalue);
                  setPixel(x , y , pixel0);
              }
              else if(2==iandor)
              {

                  int ivalue = ir & ig & ib;
                  pixel0 = qRgb(ivalue, ivalue, ivalue);
                  setPixel(x , y , pixel0);
              }
            }
        }
}



void ImageBase::ROIImage_7Blur_Gap_mud_thre_BW(int ithre,int ireserve,int igap,int ifindBorW)
{
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;


    QRgb pixelCur,pixel_1,pixel_2,pixel_3,pixel1,pixel2,pixel3;
    QRgb pixel0;

    int tx0 = m_ix0-6>0?m_ix0-6:3;
    int tx1 =  m_ix0+m_iw+6<width()?m_ix0+m_iw+3:width()-3-1;
    //Gausblur
    for(int y(m_iy0+1); y < m_iy0+ m_ih; y++)
    {

       for(int x(tx0); x < tx1; x++)
       {

            pixelCur =  pixel(x, y);
            pixel_1 =  pixel(x-1, y);
            pixel_2 =  pixel(x-2, y);
            pixel_3 =  pixel(x-3, y);
            pixel1 =  pixel(x+1, y);
            pixel2 =  pixel(x+2, y);
            pixel3 =  pixel(x+3, y);
            int ir0 = qRed(pixelCur);int ig0 = qGreen(pixelCur);int ib0 = qBlue(pixelCur);
            int ir_1 = qRed(pixel_1);int ig_1 = qGreen(pixel_1);int ib_1 = qBlue(pixel_1);
            int ir_2 = qRed(pixel_2);int ig_2 = qGreen(pixel_2);int ib_2 = qBlue(pixel_2);
            int ir_3 = qRed(pixel_3);int ig_3 = qGreen(pixel_3);int ib_3 = qBlue(pixel_3);
            int ir1 = qRed(pixel1);int ig1 = qGreen(pixel1);int ib1 = qBlue(pixel1);
            int ir2 = qRed(pixel2);int ig2 = qGreen(pixel2);int ib2 = qBlue(pixel2);
            int ir3 = qRed(pixel3);int ig3 = qGreen(pixel3);int ib3 = qBlue(pixel3);

  /*
#define  intx0_5(i) (i>>1)
#define  intx0_25(i) (i>>2)
#define  intx0_125(i) (i>>3)
#define  intx0_062(i) (i>>4)
#define  intx0_031(i) (i>>5)
#define  intx0_015(i) (i>>6)
#define  intx0_007(i) (i>>7)
#define  intx0_003(i) (i>>8)
*/
//            0.05,0.11,0.19,0.30,0.19,0.11,0.05

            int ir =intx0_062(ir_3)+ intx0_125(ir_2)+intx0_25(ir_1)+intx0_25(ir0)+intx0_062(ir0)+intx0_25(ir1)+intx0_125(ir2)+intx0_062(ir3);
            int ig =intx0_062(ig_3)+ intx0_125(ig_2)+intx0_25(ig_1)+intx0_25(ig0)+intx0_062(ig0)+intx0_25(ig1)+intx0_125(ig2)+intx0_062(ig3);
            int ib =intx0_062(ib_3)+ intx0_125(ib_2)+intx0_25(ib_1)+intx0_25(ib0)+intx0_062(ib0)+intx0_25(ib1)+intx0_125(ib2)+intx0_062(ib3);

            ir = ir >255? 255:ir;
            ig = ig >255? 255:ig;
            ib = ib >255? 255:ib;

            QRgb pixel0 = qRgb(ir, ig, ib);


            setPixel(x, y-1, pixel0);
        }
    }

    QRgb pixelx;
    QRgb pixelx0;
    QRgb pixelx1;
    if(0==ireserve)
    {
        pixelx0= qRgb(0, 0, 0);
        pixelx1= qRgb(255, 255, 255);
    }
    else if(0==ireserve)
    {
        pixelx0= qRgb(255, 255, 255);
        pixelx1= qRgb(0, 0, 0);
    }

    //GAP MUD THRE
    if(0==ifindBorW)
    {
        int tmpx0 = m_ix0>0?m_ix0:0;
        int tmpx1 =  m_ix0+m_iw+igap<width()?m_ix0+m_iw+igap:width()-igap-1;
        for(int y(m_iy0+m_ih); y >m_iy0  ; y--)
        {
            for(int x(tmpx0); x < tmpx1; x++)
            {
                QRgb pixel1 =  pixel(x, y);
                QRgb pixel2 =  pixel(x+igap, y);
                int ir = qRed(pixel2) - qRed(pixel1);
                int ig = qGreen(pixel2) - qGreen(pixel1);
                int ib = qBlue(pixel2) - qBlue(pixel1);
                if(ir>ithre||ig>ithre||ib>ithre)
                // if(qGray(pixel2)-qGray(pixel1)>ithre)
                    pixelx = pixelx1;
                else
                    pixelx = pixelx0;
               // ir = ir > ithre ? 255:0;
               // ig = ig > ithre ? 255:0;
               // ib = ib > ithre ? 255:0;
                setPixel(x, y+1, pixelx);
            }
        }
    }
    else if(1==ifindBorW)
    {
        int tmpx0 = m_ix0>0?m_ix0:0;
        int tmpx1 =  m_ix0+m_iw+igap<width()?m_ix0+m_iw+igap:width()-igap-1;

        for(int y(m_iy0+m_ih); y >m_iy0  ; y--)
        {
            for(int x(tmpx0); x < tmpx1; x++)
            {
                QRgb pixel1 =  pixel(x, y);
                QRgb pixel2 =  pixel(x+igap, y);
                int ir = qRed(pixel1) - qRed(pixel2);
                int ig = qGreen(pixel1) - qGreen(pixel2);
                int ib = qBlue(pixel1) - qBlue(pixel2);
                if(ir>ithre||ig>ithre||ib>ithre)
                //if(qGray(pixel1)-qGray(pixel2))
                    pixelx = pixelx1;
                else
                    pixelx = pixelx0;
               // ir = ir > ithre ? 255:0;
               // ig = ig > ithre ? 255:0;
               // ib = ib > ithre ? 255:0;
                setPixel(x, y+1, pixelx);
            }
        }

    }


}
void ImageBase::ROIImage_5Blur_Gap_mud_thre_BW(int ithre,int ireserve,int igap,int ifindBorW)
{
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;


    QRgb pixelCur,pixel_1,pixel_2,pixel1,pixel2;
    QRgb pixel0;
    //Gausblur
    int tx0 = m_ix0-6>0?m_ix0-6:3;
    int tx1 =  m_ix0+m_iw+6<width()?m_ix0+m_iw+3:width()-3-1;
    for(int y(m_iy0); y < m_iy0+ m_ih; y++)
    {
       for(int x(tx0); x < tx1; x++)
       {
            pixelCur =  pixel(x, y);
            pixel_1 =  pixel(x-1, y);
            pixel_2 =  pixel(x-2, y);
            pixel1 =  pixel(x+1, y);
            pixel2 =  pixel(x+2, y);
            int ir0 = qRed(pixelCur);int ig0 = qGreen(pixelCur);int ib0 = qBlue(pixelCur);
            int ir_1 = qRed(pixel_1);int ig_1 = qGreen(pixel_1);int ib_1 = qBlue(pixel_1);
            int ir_2 = qRed(pixel_2);int ig_2 = qGreen(pixel_2);int ib_2 = qBlue(pixel_2);
            int ir1 = qRed(pixel1);int ig1 = qGreen(pixel1);int ib1 = qBlue(pixel1);
            int ir2 = qRed(pixel2);int ig2 = qGreen(pixel2);int ib2 = qBlue(pixel2);


            int ir = intx0_125(ir_2)+intx0_25(ir_1)+intx0_25(ir0)+intx0_062(ir0)+intx0_25(ir1)+intx0_125(ir2);
            int ig = intx0_125(ig_2)+intx0_25(ig_1)+intx0_25(ig0)+intx0_062(ig0)+intx0_25(ig1)+intx0_125(ig2);
            int ib = intx0_125(ib_2)+intx0_25(ib_1)+intx0_25(ib0)+intx0_062(ib0)+intx0_25(ib1)+intx0_125(ib2);

            ir = ir >255? 255:ir;
            ig = ig >255? 255:ig;
            ib = ib >255? 255:ib;

            QRgb pixel0 = qRgb(ir, ig, ib);


            setPixel(x, y-1, pixel0);
        }
    }

    QRgb pixelx;

    QRgb pixelx0;
    QRgb pixelx1;
    if(0==ireserve)
    {
        pixelx0= qRgb(0, 0, 0);
        pixelx1= qRgb(255, 255, 255);
    }
    else if(1==ireserve)
    {
        pixelx0= qRgb(255, 255, 255);
        pixelx1= qRgb(0, 0, 0);
    }
    //GAP MUD THRE
    if(0==ifindBorW)
    {

        int tmpx0 = m_ix0>0?m_ix0:0;
        int tmpx1 =  m_ix0+m_iw+igap<width()?m_ix0+m_iw+igap:width()-igap-1;

        for(int y(m_iy0+ m_ih); y >m_iy0/*+2 20210225*/ ; y--)
        {
            for(int x(tmpx0/*+2 20210225*/); x < tmpx1/*-igap 20210225*/; x++)
            {
                QRgb pixel1 =  pixel(x, y);
                QRgb pixel2 =  pixel(x+igap, y);
                int ir = qRed(pixel2) - qRed(pixel1);
                int ig = qGreen(pixel2) - qGreen(pixel1);
                int ib = qBlue(pixel2) - qBlue(pixel1);
                if(ir>ithre||ig>ithre||ib>ithre)
               // if(qGray(pixel2)-qGray(pixel1)>ithre)
                    pixelx = pixelx1;
                else
                    pixelx = pixelx0;
               // ir = ir > ithre ? 255:0;
               // ig = ig > ithre ? 255:0;
               // ib = ib > ithre ? 255:0;
                setPixel(x, y+1, pixelx);
            }
        }
    }
    else if(1==ifindBorW)
    {

        int tmpx0 = m_ix0>0?m_ix0:0;
        int tmpx1 =  m_ix0+m_iw+igap<width()?m_ix0+m_iw+igap:width()-igap-1;

        for(int y(m_iy0+m_ih); y >m_iy0/*+2 20210225*/  ; y--)
        {
            for(int x(tmpx0); x <tmpx1/*-igap 20210225*/; x++)
            {
                QRgb pixel1 =  pixel(x, y);
                QRgb pixel2 =  pixel(x+igap, y);
                int ir = qRed(pixel1) - qRed(pixel2);
                int ig = qGreen(pixel1) - qGreen(pixel2);
                int ib = qBlue(pixel1) - qBlue(pixel2);
                if(ir>ithre||ig>ithre||ib>ithre)
                //if(qGray(pixel1)-qGray(pixel2))
                    pixelx = pixelx1;
                else
                    pixelx = pixelx0;
               // ir = ir > ithre ? 255:0;
               // ig = ig > ithre ? 255:0;
               // ib = ib > ithre ? 255:0;
                setPixel(x, y+1, pixelx);
            }
        }

    }


}
void ImageBase::ROIImage_7Blur_Gap_mud_thre_BW_H(int ithre,int ireserve,int igap,int ifindBorW)
{
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;


    QRgb pixelCur,pixel_1,pixel_2,pixel_3,pixel1,pixel2,pixel3;
    QRgb pixel0;
    //Gausblur
    for(int x(m_ix0+1); x < m_ix0+m_iw; x++)
    {
        for(int y(m_iy0+3); y < m_iy0+ m_ih-3; y++)
        {
            pixelCur =  pixel(x, y);
            pixel_1 =  pixel(x, y-1);
            pixel_2 =  pixel(x, y-2);
            pixel_3 =  pixel(x, y-3);
            pixel1 =  pixel(x, y+1);
            pixel2 =  pixel(x, y+2);
            pixel3 =  pixel(x, y+3);
            int ir0 = qRed(pixelCur);int ig0 = qGreen(pixelCur);int ib0 = qBlue(pixelCur);
            int ir_1 = qRed(pixel_1);int ig_1 = qGreen(pixel_1);int ib_1 = qBlue(pixel_1);
            int ir_2 = qRed(pixel_2);int ig_2 = qGreen(pixel_2);int ib_2 = qBlue(pixel_2);
            int ir_3 = qRed(pixel_3);int ig_3 = qGreen(pixel_3);int ib_3 = qBlue(pixel_3);
            int ir1 = qRed(pixel1);int ig1 = qGreen(pixel1);int ib1 = qBlue(pixel1);
            int ir2 = qRed(pixel2);int ig2 = qGreen(pixel2);int ib2 = qBlue(pixel2);
            int ir3 = qRed(pixel3);int ig3 = qGreen(pixel3);int ib3 = qBlue(pixel3);

  /*
#define  intx0_5(i) (i>>1)
#define  intx0_25(i) (i>>2)
#define  intx0_125(i) (i>>3)
#define  intx0_062(i) (i>>4)
#define  intx0_031(i) (i>>5)
#define  intx0_015(i) (i>>6)
#define  intx0_007(i) (i>>7)
#define  intx0_003(i) (i>>8)
*/
//            0.05,0.11,0.19,0.30,0.19,0.11,0.05

            int ir =intx0_062(ir_3)+ intx0_125(ir_2)+intx0_25(ir_1)+intx0_25(ir0)+intx0_062(ir0)+intx0_25(ir1)+intx0_125(ir2)+intx0_062(ir3);
            int ig =intx0_062(ig_3)+ intx0_125(ig_2)+intx0_25(ig_1)+intx0_25(ig0)+intx0_062(ig0)+intx0_25(ig1)+intx0_125(ig2)+intx0_062(ig3);
            int ib =intx0_062(ib_3)+ intx0_125(ib_2)+intx0_25(ib_1)+intx0_25(ib0)+intx0_062(ib0)+intx0_25(ib1)+intx0_125(ib2)+intx0_062(ib3);

            ir = ir >255? 255:ir;
            ig = ig >255? 255:ig;
            ib = ib >255? 255:ib;

            QRgb pixel0 = qRgb(ir, ig, ib);


            setPixel(x-1, y, pixel0);
        }
    }

    QRgb pixelx;
    QRgb pixelx0;
    QRgb pixelx1;
    if(0==ireserve)
    {
        pixelx0= qRgb(0, 0, 0);
        pixelx1= qRgb(255, 255, 255);
    }
    else if(1==ireserve)
    {
        pixelx0= qRgb(255, 255, 255);
        pixelx1= qRgb(0, 0, 0);
    }
    //GAP MUD THRE
    if(0==ifindBorW)
    {
        for(int x(m_ix0+m_iw-2); x >m_ix0+2  ; x--)
        {
            for(int y(m_iy0+2); y < m_iy0+m_ih-igap-igap; y++)
            {
                QRgb pixel1 =  pixel(x, y);
                QRgb pixel2 =  pixel(x, y+igap);
                int ir = qRed(pixel2) - qRed(pixel1);
                int ig = qGreen(pixel2) - qGreen(pixel1);
                int ib = qBlue(pixel2) - qBlue(pixel1);
                if(ir>ithre||ig>ithre||ib>ithre)
                // if(qGray(pixel2)-qGray(pixel1)>ithre)
                    pixelx = pixelx1;
                else
                    pixelx = pixelx0;
               // ir = ir > ithre ? 255:0;
               // ig = ig > ithre ? 255:0;
               // ib = ib > ithre ? 255:0;
                setPixel(x+1, y, pixelx);
            }
        }
    }
    else if(1==ifindBorW)
    {
        for(int x(m_ix0+m_iw-2); x >m_ix0  ; x--)
        {
            for(int y(m_iy0+2); y < m_iy0+m_ih-igap-igap; y++)
            {
                QRgb pixel1 =  pixel(x, y);
                QRgb pixel2 =  pixel(x, y+igap);
                int ir = qRed(pixel1) - qRed(pixel2);
                int ig = qGreen(pixel1) - qGreen(pixel2);
                int ib = qBlue(pixel1) - qBlue(pixel2);
                if(ir>ithre||ig>ithre||ib>ithre)
                //if(qGray(pixel1)-qGray(pixel2))
                    pixelx = pixelx1;
                else
                    pixelx = pixelx0;
               // ir = ir > ithre ? 255:0;
               // ig = ig > ithre ? 255:0;
               // ib = ib > ithre ? 255:0;
                setPixel(x+1, y, pixelx);
            }
        }

    }
}
void ImageBase::ROIImage_5Blur_Gap_mud_thre_BW_H(int ithre,int increase,int igap,int ifindBorW)
{
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;


    QRgb pixelCur,pixel_1,pixel_2,pixel1,pixel2;
    QRgb pixel0;
    //Gausblur
    for(int x(m_ix0+1); x < m_ix0+ m_iw; x++)
    {

       for(int y(m_iy0+2); y < m_iy0+m_ih-2; y++)
       {
            pixelCur =  pixel(x, y);
            pixel_1 =  pixel(x, y-1);
            pixel_2 =  pixel(x, y-2);
            pixel1 =  pixel(x, y+1);
            pixel2 =  pixel(x, y+2);
            int ir0 = qRed(pixelCur);int ig0 = qGreen(pixelCur);int ib0 = qBlue(pixelCur);
            int ir_1 = qRed(pixel_1);int ig_1 = qGreen(pixel_1);int ib_1 = qBlue(pixel_1);
            int ir_2 = qRed(pixel_2);int ig_2 = qGreen(pixel_2);int ib_2 = qBlue(pixel_2);
            int ir1 = qRed(pixel1);int ig1 = qGreen(pixel1);int ib1 = qBlue(pixel1);
            int ir2 = qRed(pixel2);int ig2 = qGreen(pixel2);int ib2 = qBlue(pixel2);


            int ir = intx0_125(ir_2)+intx0_25(ir_1)+intx0_25(ir0)+intx0_062(ir0)+intx0_25(ir1)+intx0_125(ir2);
            int ig = intx0_125(ig_2)+intx0_25(ig_1)+intx0_25(ig0)+intx0_062(ig0)+intx0_25(ig1)+intx0_125(ig2);
            int ib = intx0_125(ib_2)+intx0_25(ib_1)+intx0_25(ib0)+intx0_062(ib0)+intx0_25(ib1)+intx0_125(ib2);

            ir = ir >255? 255:ir;
            ig = ig >255? 255:ig;
            ib = ib >255? 255:ib;

            QRgb pixel0 = qRgb(ir, ig, ib);


            setPixel(x-1, y, pixel0);
        }
    }

    QRgb pixelx;
    //GAP MUD THRE
    if(0==ifindBorW)
    {
        for(int x(m_ix0+m_iw-2); x >m_ix0  ; x--)
        {
            for(int y(m_iy0); y < m_iy0+m_ih-igap; y++)
            {
                QRgb pixel1 =  pixel(x, y);
                QRgb pixel2 =  pixel(x, y+igap);
                int ir = qRed(pixel2) - qRed(pixel1);
                int ig = qGreen(pixel2) - qGreen(pixel1);
                int ib = qBlue(pixel2) - qBlue(pixel1);
                if(ir>ithre||ig>ithre||ib>ithre)
               // if(qGray(pixel2)-qGray(pixel1)>ithre)
                    pixelx = qRgb(255, 255, 255);
                else
                    pixelx = qRgb(0, 0, 0);
               // ir = ir > ithre ? 255:0;
               // ig = ig > ithre ? 255:0;
               // ib = ib > ithre ? 255:0;
                setPixel(x+1, y, pixelx);
            }
        }
    }
    else if(1==ifindBorW)
    {
        for(int x(m_ix0+m_iw-2); x >m_ix0  ; x--)
        {
            for(int y(m_iy0); y < m_iy0+m_ih-igap; y++)
            {
                QRgb pixel1 =  pixel(x, y);
                QRgb pixel2 =  pixel(x, y+igap);
                int ir = qRed(pixel1) - qRed(pixel2);
                int ig = qGreen(pixel1) - qGreen(pixel2);
                int ib = qBlue(pixel1) - qBlue(pixel2);
                if(ir>ithre||ig>ithre||ib>ithre)
                //if(qGray(pixel1)-qGray(pixel2))
                    pixelx = qRgb(255, 255, 255);
                else
                    pixelx = qRgb(0, 0, 0);
               // ir = ir > ithre ? 255:0;
               // ig = ig > ithre ? 255:0;
               // ib = ib > ithre ? 255:0;
                setPixel(x+1, y, pixelx);
            }
        }

    }


}

void ImageBase::ROIImageClear(int ibit)
{
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;

        for(int x(m_ix0); x < m_ix0+ m_iw; x++)
        {
            for(int y(m_iy0); y <  m_iy0+m_ih; y++)
            {
               QRgb  pixel0 = qRgb(ibit, ibit, ibit);
                setPixel(x, y, pixel0);
            }
        }
}
void ImageBase::ROIImageColor(QRgb pixel0)
{
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;

    for(int x(m_ix0); x < m_ix0+m_iw; x++)
    {
        for(int y(m_iy0); y <  m_iy0+m_ih; y++)
        {
            setPixel(x, y, pixel0);
        }
    }
}

void ImageBase::ROIImageBar(int itimes,int ifade)
{
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;

    int ibarsize = 0;
//  2x2 3x3
//    H
//   GBI
//  FA0CJ
//   EDK
//    L
    for(int i=0;i<itimes;i++)
    {
        for(int x(m_ix0+1); x <m_ix0+m_iw-1; x++)
        {
            for(int y(m_iy0+1); y < m_iy0+m_ih-1; y++)
            {
                QRgb pixelCur =  pixel(x, y);
                QRgb pixelR =  pixel(x-1, y);
                QRgb pixelL =  pixel(x, y-1);
                QRgb pixelU =  pixel(x+1, y);
                QRgb pixelD =  pixel(x, y+1);
                int ir = (qRed(pixelCur)+qRed(pixelR)+qRed(pixelL)+qRed(pixelU)+qRed(pixelD))/5;
                int ig = (qGreen(pixelCur)+qGreen(pixelR)+qGreen(pixelL)+qGreen(pixelU)+qGreen(pixelD))/5;
                int ib = (qBlue(pixelCur)+qBlue(pixelR)+qBlue(pixelL)+qBlue(pixelU)+qBlue(pixelD))/5;

                QRgb pixel0 = qRgb(ir, ig, ib);

                setPixel(x-1, y-1, pixel0);
            }
        }
    }
}
void ImageBase::ROIImage_HSOThreshold(int iHbegin,int iHend,int iSbegin,int iSend,int iObegin,int iOend)
{
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;

    int x0 = m_ix0;
    int y0 = m_iy0;
    int iw = m_iw;
    int ih = m_ih;

    int ihtwoif = 0;
    int istwoif = 0;
    int iotwoif = 0;
    if(iSbegin>iSend)
    {
        istwoif = 1;
    }
    if(iHbegin>iHend)
    {
        ihtwoif = 1;
    }
    if(iObegin>iOend)
    {
        iotwoif = 1;
    }

    QRgb  cCur = pixel(0,0);
    int r, g, b;
    //double d_h,d_s,d_o;

    thrddouble dotiso;

    int max[3];
    int temp;
    char Smax[3];
    char stemp;
    int S1,S2;
    double a;
    double a2;/*360+a*/
    int iH;
    int h;
    int snum;
    double s;

    if(ihtwoif==0)
    {
        if(istwoif==0)
        {
            if(0==iotwoif)
            {
                for (int i = y0; i <y0+ih; i++) {
                    for (int j = x0; j <x0+ iw; j++) {
                        cCur =  pixel(j, i);

                        max[0] =qRed(cCur);
                        max[1] =qGreen(cCur);
                        max[2] =qBlue(cCur);

                        Smax[0]='R';
                        Smax[1]='B';
                        Smax[2]='G';
                        if(max[1]>max[0])
                        {
                            temp=max[0];
                            max[0]=max[1];
                            max[1]=temp;

                            stemp=Smax[0];
                            Smax[0]=Smax[1];
                            Smax[1]=stemp;
                        }
                        if(max[2]>max[1])
                        {
                            temp=max[1];
                            max[1]=max[2];
                            max[2]=temp;

                            stemp=Smax[1];
                            Smax[1]=Smax[2];
                            Smax[2]=stemp;
                        }
                        if(max[1]>max[0])
                        {
                            temp=max[0];
                            max[0]=max[1];
                            max[1]=temp;

                            stemp=Smax[0];
                            Smax[0]=Smax[1];
                            Smax[1]=stemp;
                        }

                        h= max[0]+max[1]+max[2];//iH = r+g+b;
                        //snum=(max[1]*max[1]+max[2]*max[2]+max[0]*max[0]-max[0]*max[2]-max[1]*max[2]-max[1]*max[0]);
                        s=(((max[0])<<1)-(max[1]+max[2]));//ISOsqrtX[snum];
                        S1=(((max[0])<<1)-(max[1]+max[2]));
                        S2=(max[1]-max[2]);
                        a=ISOAtanAngle[S1][S2];
                        a=(a-30)>0?(a-30):0;
                        switch(Smax[0])
                        {
                        case 'R':
                            a = (Smax[1]=='B')?a+180:300-a;
                            break;
                        case 'B':
                            a = (Smax[1]=='G')? a+60:180-a;
                            break;
                        case 'G':
                            a = (Smax[1]=='R')? a+300:60-a;
                            break;
                        }
                        a2 = 360+a;

                        if(((iObegin<=a&&iOend>=a)
                            ||(iObegin<=a2&&iOend>=a2))
                            &&(iHbegin<=h&&iHend>=h)
                            &&(iSbegin<=s&&iSend>=s))
                            setPixel(j, i, qRgb(255,255,255)); // RGBs are valued
                        else
                            setPixel(j, i, qRgb(0,0,0));
                    }
                }
            }
            else if(1==iotwoif)
            {
                for (int i = y0; i <y0+ih; i++) {
                    for (int j = x0; j <x0+ iw; j++) {
                        cCur =  pixel(j, i);
                        r=qRed(cCur);
                        g=qGreen(cCur);
                        b=qBlue(cCur);
                        dotiso=DotISO_FAST(r,g,b);
                        max[0]=r;
                        max[1]=b;
                        max[2]=g;
                        Smax[0]='R';
                        Smax[1]='B';
                        Smax[2]='G';
                        if(max[1]>max[0])
                        {
                            temp=max[0];
                            max[0]=max[1];
                            max[1]=temp;

                            stemp=Smax[0];
                            Smax[0]=Smax[1];
                            Smax[1]=stemp;
                        }
                        if(max[2]>max[1])
                        {
                            temp=max[1];
                            max[1]=max[2];
                            max[2]=temp;

                            stemp=Smax[1];
                            Smax[1]=Smax[2];
                            Smax[2]=stemp;
                        }
                        if(max[1]>max[0])
                        {
                            temp=max[0];
                            max[0]=max[1];
                            max[1]=temp;

                            stemp=Smax[0];
                            Smax[0]=Smax[1];
                            Smax[1]=stemp;
                        }

                        //iH = r+g+b;
                        h=r+g+b;//intx0_5(iH) +intx0_062(iH) +intx0_015(iH);
                        snum=(max[1]*max[1]+max[2]*max[2]+max[0]*max[0]-max[0]*max[2]-max[1]*max[2]-max[1]*max[0]);
                        s=ISOsqrtX[snum];

                        a=max[0]-max[2];

                        if((iOend<=a||iObegin>=a)
                            &&(iHbegin<=h&&iHend>=h)
                            &&(iSbegin<=s&&iSend>=s))
                            setPixel(j, i, qRgb(255,255,255));
                        else
                            setPixel(j, i, qRgb(0,0,0)); // RGBs are valued
                    }
                }
            }
        }
        else
        {
            if(0==iotwoif)
            {
                for (int i = y0; i <y0+ih; i++) {
                    for (int j = x0; j <x0+ iw; j++) {
                        cCur =  pixel(j, i);
                        r=qRed(cCur);
                        g=qGreen(cCur);
                        b=qBlue(cCur);
                        max[0]=r;
                        max[1]=b;
                        max[2]=g;
                        Smax[0]='R';
                        Smax[1]='B';
                        Smax[2]='G';
                        if(max[1]>max[0])
                        {
                            temp=max[0];
                            max[0]=max[1];
                            max[1]=temp;

                            stemp=Smax[0];
                            Smax[0]=Smax[1];
                            Smax[1]=stemp;
                        }
                        if(max[2]>max[1])
                        {
                            temp=max[1];
                            max[1]=max[2];
                            max[2]=temp;

                            stemp=Smax[1];
                            Smax[1]=Smax[2];
                            Smax[2]=stemp;
                        }
                        if(max[1]>max[0])
                        {
                            temp=max[0];
                            max[0]=max[1];
                            max[1]=temp;

                            stemp=Smax[0];
                            Smax[0]=Smax[1];
                            Smax[1]=stemp;
                        }
                        iH = r+g+b;
                        h=intx0_5(iH) +intx0_062(iH) +intx0_015(iH);
                        snum=(max[1]*max[1]+max[2]*max[2]+max[0]*max[0]-max[0]*max[2]-max[1]*max[2]-max[1]*max[0]);
                        s=ISOsqrtX[snum];
                        S1=(((max[0])<<1)-(max[1]+max[2]));
                        S2=(max[1]-max[2]);
                        a=ISOAtanAngle[S1][S2];
                        a=(a-30)>0?(a-30):0;
                        if(Smax[0]== 'R')
                        {
                            if(Smax[1]=='B')
                                a=a+180;
                            else
                                a=300-a;
                        }
                        else if(Smax[0]== 'B')
                        {
                            if(Smax[1]=='G')
                                a=a+60;
                            else
                                a=180-a;
                        }
                        else if(Smax[0]=='G')
                        {
                            if(Smax[1]=='R')
                                a=a+300;
                            else
                                a=60-a;
                        }
                        if((iObegin<=a&&iOend>=a)
                            &&(iHbegin<=h&&iHend>=h)
                            &&(iSbegin>=s||iSend<=s))
                            setPixel(j, i, qRgb(255,255,255)); // RGBs are valued
                        else
                            setPixel(j, i, qRgb(0,0,0));
                    }
                }
            }
            else if(1==iotwoif)
            {
                for (int i = y0; i <y0+ih; i++) {
                    for (int j = x0; j <x0+ iw; j++) {
                        cCur =  pixel(j, i);
                        r=qRed(cCur);
                        g=qGreen(cCur);
                        b=qBlue(cCur);
                        dotiso=DotISO_FAST(r,g,b);
                        max[0]=r;
                        max[1]=b;
                        max[2]=g;
                        Smax[0]='R';
                        Smax[1]='B';
                        Smax[2]='G';
                        if(max[1]>max[0])
                        {
                            temp=max[0];
                            max[0]=max[1];
                            max[1]=temp;

                            stemp=Smax[0];
                            Smax[0]=Smax[1];
                            Smax[1]=stemp;
                        }
                        if(max[2]>max[1])
                        {
                            temp=max[1];
                            max[1]=max[2];
                            max[2]=temp;

                            stemp=Smax[1];
                            Smax[1]=Smax[2];
                            Smax[2]=stemp;
                        }
                        if(max[1]>max[0])
                        {
                            temp=max[0];
                            max[0]=max[1];
                            max[1]=temp;

                            stemp=Smax[0];
                            Smax[0]=Smax[1];
                            Smax[1]=stemp;
                        }
                        iH = r+g+b;
                        h=intx0_5(iH) +intx0_062(iH) +intx0_015(iH);
                        snum=(max[1]*max[1]+max[2]*max[2]+max[0]*max[0]-max[0]*max[2]-max[1]*max[2]-max[1]*max[0]);
                        s=ISOsqrtX[snum];
                        S1=(((max[0])<<1)-(max[1]+max[2]));
                        S2=(max[1]-max[2]);
                        a=ISOAtanAngle[S1][S2];
                        a=(a-30)>0?(a-30):0;
                        if(Smax[0]== 'R')
                        {
                            if(Smax[1]=='B')
                                a=a+180;
                            else
                                a=300-a;
                        }
                        else if(Smax[0]== 'B')
                        {
                            if(Smax[1]=='G')
                                a=a+60;
                            else
                                a=180-a;
                        }
                        else if(Smax[0]=='G')
                        {
                            if(Smax[1]=='R')
                                a=a+300;
                            else
                                a=60-a;
                        }
                        if((iOend>=a||iObegin<=a)
                            &&(iHbegin<=h&&iHend>=h)
                            &&(iSbegin>=s||iSend<=s))
                            setPixel(j, i, qRgb(255,255,255));
                        else
                            setPixel(j, i, qRgb(0,0,0)); // RGBs are valued
                    }
                }
            }

        }
    }
    else
    {
        if(0==istwoif)
        {
            if(0==iotwoif)
            {
                for (int i = y0; i <y0+ih; i++) {
                    for (int j = x0; j <x0+ iw; j++) {
                        cCur =  pixel(j, i);
                        r=qRed(cCur);
                        g=qGreen(cCur);
                        b=qBlue(cCur);
                        max[0]=r;
                        max[1]=b;
                        max[2]=g;
                        Smax[0]='R';
                        Smax[1]='B';
                        Smax[2]='G';
                        if(max[1]>max[0])
                        {
                            temp=max[0];
                            max[0]=max[1];
                            max[1]=temp;

                            stemp=Smax[0];
                            Smax[0]=Smax[1];
                            Smax[1]=stemp;
                        }
                        if(max[2]>max[1])
                        {
                            temp=max[1];
                            max[1]=max[2];
                            max[2]=temp;

                            stemp=Smax[1];
                            Smax[1]=Smax[2];
                            Smax[2]=stemp;
                        }
                        if(max[1]>max[0])
                        {
                            temp=max[0];
                            max[0]=max[1];
                            max[1]=temp;

                            stemp=Smax[0];
                            Smax[0]=Smax[1];
                            Smax[1]=stemp;
                        }
                        iH = r+g+b;
                        h=intx0_5(iH) +intx0_062(iH) +intx0_015(iH);
                        snum=(max[1]*max[1]+max[2]*max[2]+max[0]*max[0]-max[0]*max[2]-max[1]*max[2]-max[1]*max[0]);
                        s=ISOsqrtX[snum];
                        S1=(((max[0])<<1)-(max[1]+max[2]));
                        S2=(max[1]-max[2]);
                        a=ISOAtanAngle[S1][S2];
                        a=(a-30)>0?(a-30):0;
                        if(Smax[0]== 'R')
                        {
                            if(Smax[1]=='B')
                                a=a+180;
                            else
                                a=300-a;
                        }
                        else if(Smax[0]== 'B')
                        {
                            if(Smax[1]=='G')
                                a=a+60;
                            else
                                a=180-a;
                        }
                        else if(Smax[0]=='G')
                        {
                            if(Smax[1]=='R')
                                a=a+300;
                            else
                                a=60-a;
                        }
                        if((iObegin<=a&&iOend>=a)
                            &&( h>=iHbegin||h<=iHend)//iHbegin>=h||iHend<=h
                            &&(iSbegin<=s&&iSend>=s))
                            setPixel(j, i, qRgb(255,255,255)); // RGBs are valued
                        else
                            setPixel(j, i, qRgb(0,0,0));
                    }
                }
            }
            else if(1==iotwoif)
            {
                for (int i = y0; i <y0+ih; i++) {
                    for (int j = x0; j <x0+ iw; j++) {
                        cCur =  pixel(j, i);
                        r=qRed(cCur);
                        g=qGreen(cCur);
                        b=qBlue(cCur);
                        dotiso=DotISO_FAST(r,g,b);
                        max[0]=r;
                        max[1]=b;
                        max[2]=g;
                        Smax[0]='R';
                        Smax[1]='B';
                        Smax[2]='G';
                        if(max[1]>max[0])
                        {
                            temp=max[0];
                            max[0]=max[1];
                            max[1]=temp;

                            stemp=Smax[0];
                            Smax[0]=Smax[1];
                            Smax[1]=stemp;
                        }
                        if(max[2]>max[1])
                        {
                            temp=max[1];
                            max[1]=max[2];
                            max[2]=temp;

                            stemp=Smax[1];
                            Smax[1]=Smax[2];
                            Smax[2]=stemp;
                        }
                        if(max[1]>max[0])
                        {
                            temp=max[0];
                            max[0]=max[1];
                            max[1]=temp;

                            stemp=Smax[0];
                            Smax[0]=Smax[1];
                            Smax[1]=stemp;
                        }
                        iH = r+g+b;
                        h=intx0_5(iH) +intx0_062(iH) +intx0_015(iH);
                        snum=(max[1]*max[1]+max[2]*max[2]+max[0]*max[0]-max[0]*max[2]-max[1]*max[2]-max[1]*max[0]);
                        s=ISOsqrtX[snum];
                        S1=(((max[0])<<1)-(max[1]+max[2]));
                        S2=(max[1]-max[2]);
                        a=ISOAtanAngle[S1][S2];
                        a=(a-30)>0?(a-30):0;
                        if(Smax[0]== 'R')
                        {
                            if(Smax[1]=='B')
                                a=a+180;
                            else
                                a=300-a;
                        }
                        else if(Smax[0]== 'B')
                        {
                            if(Smax[1]=='G')
                                a=a+60;
                            else
                                a=180-a;
                        }
                        else if(Smax[0]=='G')
                        {
                            if(Smax[1]=='R')
                                a=a+300;
                            else
                                a=60-a;
                        }
                        if((iOend>=a||iObegin<=a)
                            &&(iHbegin>=h||iHend<=h)
                            &&(iSbegin<=s&&iSend>=s))
                            setPixel(j, i, qRgb(255,255,255));
                        else
                            setPixel(j, i, qRgb(0,0,0)); // RGBs are valued
                    }
                }
            }
        }
        else
        {
            if(0==iotwoif)
            {
                for (int i = y0; i <y0+ih; i++) {
                    for (int j = x0; j <x0+ iw; j++) {
                        cCur =  pixel(j, i);
                        r=qRed(cCur);
                        g=qGreen(cCur);
                        b=qBlue(cCur);
                        max[0]=r;
                        max[1]=b;
                        max[2]=g;
                        Smax[0]='R';
                        Smax[1]='B';
                        Smax[2]='G';
                        if(max[1]>max[0])
                        {
                            temp=max[0];
                            max[0]=max[1];
                            max[1]=temp;

                            stemp=Smax[0];
                            Smax[0]=Smax[1];
                            Smax[1]=stemp;
                        }
                        if(max[2]>max[1])
                        {
                            temp=max[1];
                            max[1]=max[2];
                            max[2]=temp;

                            stemp=Smax[1];
                            Smax[1]=Smax[2];
                            Smax[2]=stemp;
                        }
                        if(max[1]>max[0])
                        {
                            temp=max[0];
                            max[0]=max[1];
                            max[1]=temp;

                            stemp=Smax[0];
                            Smax[0]=Smax[1];
                            Smax[1]=stemp;
                        }
                        iH = r+g+b;
                        h=intx0_5(iH) +intx0_062(iH) +intx0_015(iH);
                        snum=(max[1]*max[1]+max[2]*max[2]+max[0]*max[0]-max[0]*max[2]-max[1]*max[2]-max[1]*max[0]);
                        s=ISOsqrtX[snum];
                        S1=(((max[0])<<1)-(max[1]+max[2]));
                        S2=(max[1]-max[2]);
                        a=ISOAtanAngle[S1][S2];
                        a=(a-30)>0?(a-30):0;
                        if(Smax[0]== 'R')
                        {
                            if(Smax[1]=='B')
                                a=a+180;
                            else
                                a=300-a;
                        }
                        else if(Smax[0]== 'B')
                        {
                            if(Smax[1]=='G')
                                a=a+60;
                            else
                                a=180-a;
                        }
                        else if(Smax[0]=='G')
                        {
                            if(Smax[1]=='R')
                                a=a+300;
                            else
                                a=60-a;
                        }
                        if((iObegin<=a&&iOend>=a)
                            &&(iHbegin>=h||iHend<=h)
                            &&(iSbegin>=s||iSend<=s))
                            setPixel(j, i, qRgb(255,255,255)); // RGBs are valued
                        else
                            setPixel(j, i, qRgb(0,0,0));
                    }
                }
            }
            else if(1==iotwoif)
            {
                for (int i = y0; i <y0+ih; i++) {
                    for (int j = x0; j <x0+ iw; j++) {
                        cCur =  pixel(j, i);
                        r=qRed(cCur);
                        g=qGreen(cCur);
                        b=qBlue(cCur);
                        dotiso=DotISO_FAST(r,g,b);
                        max[0]=r;
                        max[1]=b;
                        max[2]=g;
                        Smax[0]='R';
                        Smax[1]='B';
                        Smax[2]='G';
                        if(max[1]>max[0])
                        {
                            temp=max[0];
                            max[0]=max[1];
                            max[1]=temp;

                            stemp=Smax[0];
                            Smax[0]=Smax[1];
                            Smax[1]=stemp;
                        }
                        if(max[2]>max[1])
                        {
                            temp=max[1];
                            max[1]=max[2];
                            max[2]=temp;

                            stemp=Smax[1];
                            Smax[1]=Smax[2];
                            Smax[2]=stemp;
                        }
                        if(max[1]>max[0])
                        {
                            temp=max[0];
                            max[0]=max[1];
                            max[1]=temp;

                            stemp=Smax[0];
                            Smax[0]=Smax[1];
                            Smax[1]=stemp;
                        }
                        iH = r+g+b;
                        h=intx0_5(iH) +intx0_062(iH) +intx0_015(iH);
                        snum=(max[1]*max[1]+max[2]*max[2]+max[0]*max[0]-max[0]*max[2]-max[1]*max[2]-max[1]*max[0]);
                        s=ISOsqrtX[snum];
                        S1=(((max[0])<<1)-(max[1]+max[2]));
                        S2=(max[1]-max[2]);
                        a=ISOAtanAngle[S1][S2];
                        a=(a-30)>0?(a-30):0;
                        if(Smax[0]== 'R')
                        {
                            if(Smax[1]=='B')
                                a=a+180;
                            else
                                a=300-a;
                        }
                        else if(Smax[0]== 'B')
                        {
                            if(Smax[1]=='G')
                                a=a+60;
                            else
                                a=180-a;
                        }
                        else if(Smax[0]=='G')
                        {
                            if(Smax[1]=='R')
                                a=a+300;
                            else
                                a=60-a;
                        }
                        if((iOend>=a||iObegin<=a)
                            &&(iHbegin>=h||iHend<=h)
                            &&(iSbegin>=s||iSend<=s))
                            setPixel(j, i, qRgb(255,255,255));
                        else
                            setPixel(j, i, qRgb(0,0,0)); // RGBs are valued
                    }
                }
            }

        }
    }
}

void ImageBase::ROIImage_ColorThreshold(int ir,int ig,int ib,int ihgap,int isgap,int iogap)
{
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;

    double d_h_0,d_s_0,d_o_0;

    DOTISO_MAC(ir,ig,ib,d_h_0,d_s_0,d_o_0);

    int r, g, b;
    double d_h,d_s,d_o;
    thrddouble dotiso;

    for (int i = m_ix0; i < m_ix0 + m_iw; i++)
    {
        for (int j = m_iy0; j < m_iy0 + m_ih; j++)
        {
            QRgb pixelCur = pixel(i, j);
            r=qRed(pixelCur);
            g=qGreen(pixelCur);
            b=qBlue(pixelCur);
            dotiso=DotISO_FAST(r,g,b);

            d_h=dotiso.varH -d_h_0;
            d_s=dotiso.varS -d_s_0;
            d_o=dotiso.varO -d_o_0;
            d_h=d_h>0?d_h:-d_h;
            d_s=d_s>0?d_s:-d_s;
            d_o=d_o>0?d_o:-d_o;
            d_o=360-d_o>d_o?d_o:360-d_o;
            if(d_h>ihgap||d_s>isgap||d_o>iogap)
                setPixel(i, j, qRgb(255,255,255));
            else
                setPixel(i, j, qRgb(0,0,0));
        }
    }
}
void ImageBase::ROIImageGray()
{
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;

        for(int x(m_ix0); x < m_ix0 + m_iw; x++)
        {
            for(int y(m_iy0); y < m_iy0 + m_ih; y++)
            {

               setPixel(x, y, qGray(pixel(x,y)));
            }
        }
}

void ImageBase::ROIImageMulDiv(double dvalue)
{
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;

    QRgb pixel0;
        for(int x(m_ix0); x < m_ix0 + m_iw; x++)
        {
            for(int y(m_iy0); y < m_iy0 + m_ih; y++)
            {
               QRgb pixelCur =  pixel(x, y);
               float fr = qRed(pixelCur);
               float fg = qGreen(pixelCur);
               float fb = qBlue(pixelCur);
               int ir = fr *dvalue;
               int ig = fg *dvalue;
               int ib = fb *dvalue;
               pixel0 = qRgb(ir, ig, ib);

               setPixel(x, y, pixel0);
            }
        }
}
void ImageBase::ROIImageOR(ImageBase*pimage,QPainter::CompositionMode mode)
{
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;

    QPainter painter(this);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(m_ix0, m_iy0, *pimage,m_ix0,m_iy0,m_iw,m_ih);
    painter.end();
}
//void ImageBase::ImageXOR(ImageBase*pimage){}
//void ImageBase::ImageAND(ImageBase*pimage){}
//void ImageBase::ImageNOT(ImageBase*pimage){}
void ImageBase::ROIImageMove(int ix,int iy)
{

    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;

}
void ImageBase::ROIImageIncrease(int inum)
{
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;

    QRgb pixel0;
        for(int x(m_ix0); x < m_ix0+m_iw; x++)
        {
            for(int y(m_iy0); y <  m_iy0 + m_ih; y++)
            {
               QRgb pixelCur =  pixel(x, y);

               pixel0 = qRgb(qRed(pixelCur)+inum, qGreen(pixelCur)+inum, qBlue(pixelCur)+inum);

               setPixel(x, y, pixel0);
            }
        }
}
void ImageBase::ROIImageDecrease(int inum)
{
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;

    QRgb pixel0;
    for(int x(m_ix0); x < m_ix0+m_iw; x++)
    {
        for(int y(m_iy0); y <  m_iy0 + m_ih; y++)
            {
               QRgb pixelCur =  pixel(x, y);

               pixel0 = qRgb(qRed(pixelCur)-inum, qGreen(pixelCur)-inum, qBlue(pixelCur)-inum);

               setPixel(x, y, pixel0);
            }
        }

}

void ImageBase::ROIImageMoveAndX(int ix)
{
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;

    QRgb pixel0;
    int iv = ix >0 ? 1:-1;
    for(int i=0;i<ix;i++)
    for(int x(m_ix0+1); x < width()-1; x++)
        {
            for(int y(m_iy0); y <  height(); y++)
            {
                   QRgb pixelCur1 =  pixel(x, y);
                   QRgb pixelCur2 =  pixel(x+iv, y);
                   pixel0 = pixelCur1 & pixelCur2;
                   setPixel(x, y, pixel0);
            }
        }
}
void ImageBase::ROIImageMoveAndY(int ix)
{
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;

    QRgb pixel0;
    int iv = ix >0 ? 1:-1;

    for(int i=0;i<ix;i++)
    for(int x(m_ix0+1); x < width()-1; x++)
        {
            for(int y(m_iy0); y <  height(); y++)
            {
                   QRgb pixelCur1 =  pixel(x, y);
                   QRgb pixelCur2 =  pixel(x, y+iv);
                   pixel0 = pixelCur1 & pixelCur2;
                   setPixel(x, y, pixel0);
            }
        }
}
void ImageBase::ROIImageMoveOrX(int ix)
{
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;


    QRgb pixel0;
    int iv = ix >0 ? 1:-1;

    for(int i=0;i<ix;i++)
    for(int x(m_ix0); x < width(); x++)
        {
            for(int y(m_iy0+1); y <  height()-1; y++)
            {
                   QRgb pixelCur1 =  pixel(x, y);
                   QRgb pixelCur2 =  pixel(x+iv, y);
                   pixel0 = pixelCur1 | pixelCur2;
                   setPixel(x, y, pixel0);
            }
        }
}
void ImageBase::ROIImageMoveOrY(int ix)
{
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;

    QRgb pixel0;
    int iv = ix >0 ? 1:-1;
    for(int i=0;i<ix;i++)
    for(int x(m_ix0); x < width(); x++)
        {
            for(int y(m_iy0+1); y <  height()-1; y++)
            {
                   QRgb pixelCur1 =  pixel(x, y);
                   QRgb pixelCur2 =  pixel(x, y+iv);
                   pixel0 = pixelCur1 | pixelCur2;
                   setPixel(x, y, pixel0);
            }
        }
}

void ImageBase::ROIImageAnd(int ix)
{
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;


    ROIImageMoveAndY(-ix);
    ROIImageMoveAndX(-ix);
    ROIImageMoveAndY(ix);
    ROIImageMoveAndX(ix);

}

void ImageBase::ROIImageOr(int ix)
{
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;


    ROIImageMoveOrY(-ix);
    ROIImageMoveOrX(-ix);
    ROIImageMoveOrY(ix);
    ROIImageMoveOrX(ix);

}


void ImageBase::ROIImageFilp()
{
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;

    QRgb pixel0;
    int ihalfh = m_ih /2;

    for(int x(m_ix0); x < m_iw; x++)
        {
            for(int y(m_iy0); y <  ihalfh; y++)
            {
               QRgb pixel0 =  pixel(x, y);
               QRgb pixel1 =  pixel(x, height()-y);

               setPixel(x, y, pixel1);
               setPixel(x, height()-y, pixel0);
            }
        }
}
void ImageBase::ROIImageMirror()
{
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;

    QRgb pixel0;
    int ihalfw = m_iw /2;

    for(int x(m_ix0); x < ihalfw; x++)
        {
            for(int y(m_iy0); y <  m_ih; y++)
            {
               QRgb pixel0 =  pixel(x, y);
               QRgb pixel1 =  pixel(width()-x, y);

               setPixel(x, y, pixel1);
               setPixel(width()-x, y, pixel0);
            }
        }
}
void ImageBase::ROIImageColorGray()
{
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }
    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;


    for(int x(m_ix0); x < m_ix0 + m_iw; x++)
        {
            for(int y(m_iy0); y < m_iy0 + m_ih; y++)
            {
               QRgb pixel0 =  pixel(x, y);
               int ivalue = qRed(pixel0) + qGreen(pixel0) +qBlue(pixel0);
               ivalue = ivalue /3;
               setPixel(x, y, QRgb(ivalue));
            }
        }
}

QPainterPath ImageBase::ImageTransfer(QPainterPath &path,double dx,double dy,double dangle,double dscalex,double dscaley)
{

    QPainter painter(this);
    painter.setClipPath(path);
    painter.translate(-dx, -dy);
    painter.rotate(dangle);
    painter.scale(dscalex, dscaley);
    return painter.clipPath();
}
void ImageBase::ImageDrawPath(QPainterPath &path,int ix1,int iy1,QColor acolor)
{
    int icount = path.elementCount();
    for(int i=0;i<icount;i++)
    {
         QPainterPath::Element aele = path.elementAt(i);
         setPixelColor(aele.x+ix1,aele.y+iy1,acolor);
    }
}

void ImageBase::ImageCentRotate(double dangle)
{

    QTransform matrix;
    matrix.translate(0, 0);
    matrix.scale(1, 1);
    matrix.rotate(dangle);

     QImage aimageroi1=this->copy(0,0,iw(),ih());
    QImage aimageroi2=aimageroi1.transformed(matrix);
    QPainter painter(&aimageroi2);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, *this);
    painter.end();
}

void ImageBase::ImageFont(const char *pchar)
{

    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;
    if(width()<=0||height()<=0)
        return;
    QString qfontstring(pchar);
    QStringList astlsit = qfontstring.split(",");//name,pos.x,pos.y,size,r,g,b
    QString qname;
    QString qposx;int iposx;
    QString qposy;int iposy;
    QString qposw;int iposw;
    QString qposh;int iposh;
    QString qsize;int ifontsize;
    QString qrvalue;int irvalue;
    QString qgvalue;int igvalue;
    QString qbvalue;int ibvalue;
    if(astlsit.size()>0)
        qname = astlsit[0];
    if(astlsit.size()>1)
    {
        qposx = astlsit[1];
        iposx = qposx.toInt();
    }
    if(astlsit.size()>2)
    {
        qposy = astlsit[2];
        iposy = qposy.toInt();
    }
    if(astlsit.size()>3)
    {
        qposw = astlsit[3];
        iposw = qposw.toInt();
    }
    if(astlsit.size()>4)
    {
        qposh = astlsit[4];
        iposh = qposh.toInt();
    }
    if(astlsit.size()>5)
    {
        qsize = astlsit[5];
        ifontsize = qsize.toInt();
    }
    if(astlsit.size()>6)
    {
        qrvalue = astlsit[6];
        irvalue = qrvalue.toInt();
    }
    if(astlsit.size()>7)
    {
        qgvalue = astlsit[7];
        igvalue = qgvalue.toInt();
    }
    if(astlsit.size()>8)
    {
        qbvalue = astlsit[8];
        ibvalue = qbvalue.toInt();
    }
    QPainter painter(this);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    QFont font = painter.font();
    QColor qcolor(irvalue,igvalue,ibvalue);
    font.setPixelSize(ifontsize);
    painter.setFont(font);
    painter.setPen(qcolor);
    painter.setBrush(Qt::white);
    QRect arect;
    arect.setRect(iposx,iposy,iposw,iposh);
    painter.drawText(arect, Qt::AlignCenter,qname);

    painter.drawImage(0, 0,*this);
    painter.end();
}
void ImageBase::ImageFontSet(const char *pchar)
{

    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;
    if(width()<=0||height()<=0)
        return;
    QString qfontstring(pchar);
    QStringList astlsit = qfontstring.split(",");//name,pos.x,pos.y,size,r,g,b
    QString qname;
    QString qposx;int iposx;
    QString qposy;int iposy;
    QString qposw;int iposw;
    QString qposh;int iposh;
    QString qsize;int ifontsize;
    QString qrvalue;int irvalue;
    QString qgvalue;int igvalue;
    QString qbvalue;int ibvalue;

    if(astlsit.size()>0)
    {
        qposx = astlsit[0];
        m_ifontposx = qposx.toInt();
    }
    if(astlsit.size()>1)
    {
        qposy = astlsit[1];
        m_ifontposy = qposy.toInt();
    }
    if(astlsit.size()>2)
    {
        qposw = astlsit[2];
        m_ifontposw = qposw.toInt();
    }
    if(astlsit.size()>3)
    {
        qposh = astlsit[3];
        m_ifontposh = qposh.toInt();
    }
    if(astlsit.size()>4)
    {
        qsize = astlsit[4];
        m_ifontsize = qsize.toInt();
    }
    if(astlsit.size()>5)
    {
        qrvalue = astlsit[5];
        m_ifontrvalue = qrvalue.toInt();
    }
    if(astlsit.size()>6)
    {
        qgvalue = astlsit[6];
        m_ifontgvalue = qgvalue.toInt();
    }
    if(astlsit.size()>7)
    {
        qbvalue = astlsit[7];
        m_ifontbvalue = qbvalue.toInt();
    }

}
 void ImageBase::ImageFontValue(double dvalue,double dtype)
 {
     if(dtype==1.0)
     {
         int ivalue=dvalue;
         m_qfontname=QString("%1").arg(ivalue);
     }
     else
         m_qfontname=QString("%1").arg(dvalue);

     QPainter painter(this);
     painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
     QFont font = painter.font();
     QColor qcolor(m_ifontrvalue,m_ifontgvalue,m_ifontbvalue);
     font.setPixelSize(m_ifontsize);
     painter.setFont(font);
     painter.setPen(qcolor);
     painter.setBrush(Qt::white);
     QRect arect;
     arect.setRect(m_ifontposx,m_ifontposy,m_ifontposw,m_ifontposh);
     painter.drawText(arect, Qt::AlignCenter,m_qfontname);

     painter.drawImage(0, 0,*this);
     painter.end();
 }
void ImageBase::ImageZoom(double dscalex,double dscaley)
{

    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;
    if(width()<=0||height()<=0)
        return;
    QPainter painter(this);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    painter.scale(dscalex, dscaley);
    painter.drawImage(0, 0,*this);
    painter.end();
}
void ImageBase::ImageZoomAnd(ImageBase *pimage,int ix,int iy,double dscalex,double dscaley)
{

    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;
    if(width()<=0||height()<=0)
        return;
    QPainter painter(this);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    painter.scale(dscalex, dscaley);
    int icurposx = (ix*1.0) / (dscalex*1.0);
    int icurposy = (iy*1.0) / (dscaley*1.0);
    painter.drawImage(icurposx, icurposy,*pimage);
    painter.end();
}

void ImageBase::ImageZoomA(int ix,int iy,double dscalex,double dscaley)
{
    if(width()<=0||height()<=0)
        return;
    QPainter painter(this);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    painter.scale(dscalex, dscaley);

    int icurposx = (ix*1.0) / (dscalex*1.0);
    int icurposy = (iy*1.0) / (dscaley*1.0);
    painter.drawImage(icurposx, icurposy,*this);
    painter.end();
}

void ImageBase::ImagePen(int ix,int iy )
{
    setPixel( ix, iy, Qt::red);
}
void ImageBase::ROIImageCopy(int ix1,int iy1,int iw1,int ih1)
{

    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;
    if(width()< ix0() + iw()
       || height() < iy0() + ih())
        return;//error process


    if(width()< ix1 + iw1
       || height() < iy1 + ih1)
        return;//error process

    double dscalex = (1.0*iw1)/(iw()*1.0);
    double dscaley = (1.0*ih1)/(ih()*1.0);

    int icurposx = (ix1*1.0) / (dscalex*1.0);
    int icurposy = (iy1*1.0) / (dscaley*1.0);


    QPainter painter(this);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.scale(dscalex, dscaley);
    painter.drawImage(icurposx,icurposy,*this,ix0(),iy0());
    painter.end();
}
void ImageBase::SavePath(const char *pfilename)
{
    /*
BMP
GIF
JPG
JPEG
PNG
PBM
PGM
PPM
XBM
XPM
*/
    save(QString(pfilename),"JPEG");
}


void ImageBase::LoadPath(const char *pfilename)
{
    load(QString(pfilename));
}
void ImageBase::LoadNum(int inum)
{
    QString strname = m_qfiledir + QString("/%1.bmp").arg(inum);
    load(strname);
}
void ImageBase::LoadNumJpg(int inum)
{
    QString strname = m_qfiledir + QString("/%1.jpg").arg(inum);
    load(strname);
}
void ImageBase::setimagepath(const char *pfilepath)
{
    m_qfiledir=QString(pfilepath);
}
void ImageBase::Load()
{
    QString fileName =
        QFileDialog::getOpenFileName(nullptr, QString("Open File"),
        QDir::currentPath(),
        QString("ALL Files (*.*)"));
    if (fileName.isEmpty())
        return;
    QFileInfo aqfi(fileName);
   m_qfiledir = aqfi.path();
    load(fileName);
}
void ImageBase::ROIImage()
{

    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;
    if(width()< ix0() + iw()
       || height() < iy0() + ih())
        return;//error process
    QSize imagesize(iw(),ih());
    QImage aimage(imagesize, QImage::Format_ARGB32_Premultiplied);

    QPainter painter(&aimage);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0,0,*this,ix0(),iy0(),iw(),ih());
    painter.end();

   *this = aimage;
}
void ImageBase::clipboardROIimage()
{
    QSize imagesize(iw(),ih());
    QImage aimage(imagesize, QImage::Format_ARGB32_Premultiplied);

    QPainter painter(&aimage);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawImage(0,0,*this,ix0(),iy0(),iw(),ih());
    painter.end();

    QApplication::clipboard()->setImage(aimage);

}
void ImageBase::SaveROI(const char *pfilename)
{

    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;
    if(width()< ix0() + iw()
       || height() < iy0() + ih())
        return;//error process
    QSize imagesize(iw(),ih());
    QImage aimage(imagesize, QImage::Format_ARGB32_Premultiplied);

    QPainter painter(&aimage);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0,0,*this,ix0(),iy0(),iw(),ih());
    painter.end();

    aimage.save(QString(pfilename));
}
void ImageBase::roitoroi(void *paimage)
{
    ImageBase *pimage = (ImageBase*)paimage;
    ROItoROI(pimage);
}
void ImageBase::ROItoROI(ImageBase *aimage)
{

    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;
    if(width()< ix0() + iw()
       || height() < iy0() + ih())
        return;//error process
    int ix1 = aimage->ix0();
    int iy1 = aimage->iy0();
    if(ix1<0||iy1<0)
        return;

    QPainter painter(aimage);
    painter.setCompositionMode(m_mode);
    painter.drawImage(ix1,iy1,*this,ix0(),iy0(),iw(),ih());
    painter.end();

}
void ImageBase::ROIColorTable()
{

    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;
    if(width()< ix0() + iw()
       || height() < iy0() + ih())
        return;//error process
    int icurmodule = BackImageManager::GetCurMode();
    qint64 *rtable = BackImageManager::GetRtable(icurmodule);
    qint64 *gtable = BackImageManager::GetGtable(icurmodule);
    qint64 *btable = BackImageManager::GetBtable(icurmodule);

    for(int i=0;i<=255;i++)
    {
        btable[i] = 0;
        gtable[i] = 0;
        rtable[i] = 0;
    }
//    memset(rtable,0,255*sizeof(qint64));
//    memset(gtable,0,255*sizeof(qint64));
//    memset(btable,0,255*sizeof(qint64));

    int r=0, g=0, b=0;

    int iby = m_iy0;
    int iey = m_iy0+m_ih;
    int ibx = m_ix0;
    int iex = m_ix0+m_iw;

    BackImageManager::g_table_rate = m_ih * m_iw;
    for (int j = iby; j < iey; j++)
    {
        for (int i = ibx; i < iex; i++)
        {
            QRgb pixel0 =  pixel(i, j);

            r =qRed(pixel0);
            g =qGreen(pixel0);
            b =qBlue(pixel0);;

            rtable[r] ++;
            gtable[g] ++;
            btable[b] ++;

         }
   }

}
void ImageBase::ROIColorTableBlur(int iGauss_Smoth,int ithre)
{
    if(iGauss_Smoth<0
       ||ithre<0)
        return;
    int icurmodule = BackImageManager::GetCurMode();
    qint64 *rtable = BackImageManager::GetRtable(icurmodule);
    qint64 *gtable = BackImageManager::GetGtable(icurmodule);
    qint64 *btable = BackImageManager::GetBtable(icurmodule);

    //m_iGauss_Smoth = iGauss_Smoth;
    //m_iSmoth_thre = ithre;
    double *fGaussWindow=NULL;
    switch(iGauss_Smoth)
    {
    case 0://more smoth
        fGaussWindow=(double*)GausPar_Smoth;
        break;
    case 1:
        fGaussWindow=(double*)GausPar;
        break;
    case 2:
        fGaussWindow=(double*)GausPar0;
        break;
    case 3:
        fGaussWindow=(double*)GausPar_moreSmoth;
        break;
    case 4:
        fGaussWindow=(double*)GausPar_average;
        break;
    default:
        fGaussWindow=(double*)GausPar;
        break;
    }
    int ipointsum=0;
    double dvalue =0;
    double dblurvalue[256];
    //r
    for (int i=0;i<=255;i++)
    {
        dblurvalue[i] = 0;
        for(int j=0;j<MATRIXNUM;j++)
        {
            ipointsum=i+GausMatix[j];
            if(ipointsum<0)
                ipointsum=-ipointsum;
            if(ipointsum>=255)
                ipointsum=255-ipointsum+254;

            dvalue= rtable[ipointsum] ;
            dblurvalue[i]+=fGaussWindow[j]*dvalue;
        }
    }

    for(int i=0;i<=255;i++)
    {
        rtable[i] = dblurvalue[i]-ithre;
    }
    //g
    for (int i=0;i<=255;i++)
    {
        dblurvalue[i] = 0;
        for(int j=0;j<MATRIXNUM;j++)
        {
            ipointsum=i+GausMatix[j];
            if(ipointsum<0)
                ipointsum=-ipointsum;
            if(ipointsum>=255)
                ipointsum=255-ipointsum+254;

            dvalue= gtable[ipointsum] ;
            dblurvalue[i]+=fGaussWindow[j]*dvalue;
        }
    }

    for(int i=0;i<=255;i++)
    {
        gtable[i] = dblurvalue[i]-ithre;
    }
    //b
    for (int i=0;i<=255;i++)
    {
        btable[i] = 0;
        for(int j=0;j<MATRIXNUM;j++)
        {
            ipointsum=i+GausMatix[j];
            if(ipointsum<0)
                ipointsum=-ipointsum;
            if(ipointsum>=255)
                ipointsum=255-ipointsum+254;

            dvalue= btable[ipointsum] ;
            dblurvalue[i]+=fGaussWindow[j]*dvalue;
        }
    }

    for(int i=0;i<=255;i++)
    {
        btable[i] = dblurvalue[i]-ithre;
    }
}
void ImageBase::ROIColorTableEasyThre(int iandor,int ioffset)
{
    int icurmodule = BackImageManager::GetCurMode();
    qint64 *rtable = BackImageManager::GetRtable(icurmodule);
    qint64 *gtable = BackImageManager::GetGtable(icurmodule);
    qint64 *btable = BackImageManager::GetBtable(icurmodule);

    int imax = 0;
    int imin = 0;
    int ir,ig,ib =0;
    //r
    imax = 0;
    imin = 0;
    for (int i=0;i<=255;i++)
        {
        if(rtable[i]>0)
            {
            imin = i;
            break;
            }
        }

    for(int i=255;i>=0;i--)
        {
        if(rtable[i]>0)
            {
            imax = i;
            break;
            }
        }

    ir = (imax+imin)/2+ioffset ;


    //g
    imax = 0;
    imin = 0;
    for (int i=0;i<=255;i++)
        {
        if(gtable[i]>0)
            {
            imin = i;
            break;
            }
        }

    for(int i=255;i>=0;i--)
        {
        if(gtable[i]>0)
            {
            imax = i;
            break;
            }
        }
    ig = (imax+imin)/2+ioffset ;
    //b
    imax = 0;
    imin = 0;
    for (int i=0;i<=255;i++)
        {
        if(btable[i]>0)
            {
            imin = i;
            break;
            }
        }

    for(int i=255;i>=0;i--)
        {
        if(btable[i]>0)
            {
            imax = i;
            break;
            }
        }
    ib = (imax+imin)/2+ioffset ;
   BackImageManager::g_r_table_thre = ir;
   BackImageManager::g_g_table_thre = ig;
   BackImageManager::g_b_table_thre = ib;
    ROIImageRGBThre(ir,ig,ib,iandor);
}


QRgb ImageBase::ROIBackground(int ileftgap,int irightgap,int iupgap,int idowngap,int imethod)
{

    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;

    if(width()< ix0() + iw()
       || height() < iy0() + ih())
        return QColor(0,0,0).rgb();//error process
    QMap<QRgb,int> backcolormap;
    if(imethod==0)//edge background
    {
        int ibeginx = m_ix0-ileftgap;
        ibeginx = ibeginx>=0?ibeginx:0;
        for(int x(ibeginx); x < m_ix0; x++)
        {
            for(int y(m_iy0); y < m_iy0 + m_ih; y++)
            {
                QRgb pixel0 =  pixel(x, y);
                backcolormap[pixel0]++;
            }
        }
        ibeginx = m_ix0+m_iw;
        ibeginx = ibeginx>=0?ibeginx:0;
        for(int x(ibeginx); x < m_ix0+m_iw +irightgap; x++)
        {
            for(int y(m_iy0 ); y < m_iy0 ; y++)
            {
                QRgb pixel0 =  pixel(x, y);
                backcolormap[pixel0]++;
            }
        }

        for(int x(m_ix0); x < m_ix0+m_iw; x++)
        {
            int ibeginy = m_iy0 - iupgap;
            ibeginy = ibeginy>=0?ibeginy:0;
            for(int y(ibeginy ); y < m_iy0; y++)
            {
                QRgb pixel0 =  pixel(x, y);
                backcolormap[pixel0]++;
            }
        }

        for(int x(m_ix0); x < m_ix0+m_iw; x++)
        {
            for(int y(m_iy0+ m_ih); y < m_iy0 + m_ih +idowngap; y++)
            {
                QRgb pixel0 =  pixel(x, y);
                backcolormap[pixel0]++;
            }
        }

    }
    else if(imethod==1)//roi background
    {
        int icentx = m_ix0 + m_iw/2;
        int icenty = m_iy0 + m_ih/2;

        int ibeginx = icentx-ileftgap;
        ibeginx = ibeginx>=0?ibeginx:0;
        for(int x(ibeginx); x < icentx + irightgap; x++)
        {
            int ibeginy = icenty -iupgap;
            ibeginy = ibeginy>=0?ibeginy:0;
            for(int y(ibeginy); y < icenty + idowngap; y++)
            {
                QRgb pixel0 =  pixel(x, y);
                backcolormap[pixel0]++;
            }
        }
    }
    int imaxvalue = 0;
     QRgb qcolor;
    QMap<QRgb, int>::const_iterator i = backcolormap.constBegin();
    while (i != backcolormap.constEnd())
    {
        if(i.value()>imaxvalue)
        {
            imaxvalue = i.value();
            qcolor = i.key();
        }
        ++i;
    }
    return qcolor;
}
void ImageBase::ROIImageZoom(ImageBase *pimage,double dratex,double dratey)
{

    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }


    QRgb pixel0 = qRgb(0,0,0);
    QRgb pixel1 = qRgb(255,255,255);
    int ix0 = m_ix0;
    int iy0 = m_iy0;

    for(int x(0); x <  m_iw; x++)
    {
        for(int y(0); y <  m_ih; y++)
        {
            pimage->setPixel(x+m_ix0 , y+m_iy0 , pixel0);
        }
    }

    for(int x(0); x <  m_iw; x++)
    {
        for(int y(0); y <  m_ih; y++)
        {
            QRgb pixelCur =  pixel(x+ix0, y+iy0);
            int inewx = x * dratex;
            int inewy = y * dratey;
            QRgb pixelnewCur =  pimage->pixel(inewx + ix0, inewy +iy0);

            if(qRed(pixelnewCur)==0
               &&qRed(pixelCur)!=0
               &&qBlue(pixelCur)!=0
               &&qGreen(pixelCur)!=0)
                pimage->setPixel(inewx + ix0, inewy +iy0, pixel1);
        }
    }

}

void ImageBase::ROIImageZoomEx(ImageBase *pimage,double dratex,double dratey)
{

    if(m_ix0<0)
        m_ix0=0;
    if(m_iy0<0)
        m_iy0=0;
    if(width()<m_ix0  +m_iw)
    {
        m_ix0 =width()-m_iw-1;
    }
    if(height() < m_iy0 + m_ih)
    {
        m_iy0=height()- m_ih-1;
    }


    QRgb pixel0 = qRgb(0,0,0);
    QRgb pixel1 = qRgb(255,255,255);
    int ix0 = m_ix0;
    int iy0 = m_iy0;

    for(int x(0); x <  m_iw; x++)
    {
        for(int y(0); y <  m_ih; y++)
        {
            pimage->setPixel(x+m_ix0 , y+m_iy0 , pixel0);
        }
    }
    if(0.5==dratex
      &&0.5==dratey)
    {
        for(int x(0); x <  m_iw; x++)
        {
            for(int y(0); y <  m_ih; y++)
            {
                    QRgb pixelCur =  pixel(x+ix0, y+iy0);
                    int inewx = x * 0.5;
                    int inewy = y * 0.5;
                    QRgb pixelnewCur =  pimage->pixel(inewx + ix0, inewy +iy0);

                    if(qRed(pixelnewCur)==0
                       &&qRed(pixelCur)!=0
                       &&qBlue(pixelCur)!=0
                       &&qGreen(pixelCur)!=0)
                        pimage->setPixel(inewx + ix0, inewy +iy0, pixel1);
            }
        }
    }
    else if(0.25==dratex
      &&0.25==dratey)
    {
        int ibeginx = m_iw/2;
        int ibeginy = m_ih/2;
        for(int x(0); x <  m_iw; x++)
        {
            for(int y(0); y <  m_ih; y++)
            {
                    QRgb pixelCur =  pixel(x+ix0, y+iy0);
                    int inewx = x * 0.5;
                    int inewy = y * 0.5;
                    QRgb pixelnewCur =  pimage->pixel(inewx +ibeginx+ ix0, inewy+ibeginy +iy0);
                    if(qRed(pixelnewCur)==0
                       &&qRed(pixelCur)!=0 )
                        pimage->setPixel(inewx +ibeginx+ ix0 , inewy +ibeginy+iy0, pixel1);
            }
        }
        int iw1 = m_iw/2;
        int ih1 = m_ih/2;
        for(int x(0); x <  iw1; x++)
        {
            for(int y(0); y < ih1; y++)
            {
                    QRgb pixelCur =  pimage->pixel(x+ibeginx+ix0, y+ibeginy+iy0);
                    int inewx = x * 0.5;
                    int inewy = y * 0.5;
                    QRgb pixelnewCur =  pimage->pixel(inewx  + ix0, inewy+ iy0);
                    if(qRed(pixelnewCur)==0
                       &&qRed(pixelCur)!=0 )
                        pimage->setPixel(inewx  + ix0 , inewy +iy0, pixel1);
            }
        }

    }

}
void ImageBase::shapesetroi(void *pshape)
{
     QShape *ashp=(QShape*)pshape;
     if(nullptr!=ashp)
     {
         QRect arect = ashp->rect();
         SetROI(arect.x(),arect.y(),arect.width(),arect.height());
     }
}
int ImageBase::tablevalue(int icolor)
{
    int icurmodule = BackImageManager::GetCurMode();
    qint64 *rtable = BackImageManager::GetRtable(icurmodule);
    qint64 *gtable = BackImageManager::GetGtable(icurmodule);
    qint64 *btable = BackImageManager::GetBtable(icurmodule);
    if(icolor<=255&&icolor>=0)
        return rtable[icolor];
}
