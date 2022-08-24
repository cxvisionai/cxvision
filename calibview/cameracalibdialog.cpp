/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
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

#include "cameracalibdialog.h"

#include <QGraphicsView>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>
#include "cximage/visionmanager.h"
CameraCalibDialog::CameraCalibDialog(visionmanager *pmanager,QGraphicsView *pview)
    : QGraphicsView(pview)
    ,m_pmanager(pmanager)
{
    m_scene.setSceneRect(0, 0, 690, 470);

    double d2calx=m_pmanager->GetParserValue(QString("d2calx"));
    double d2caly=m_pmanager->GetParserValue(QString("d2caly"));

    double d3calx=m_pmanager->GetParserValue(QString("d3calx"));
    double d3caly=m_pmanager->GetParserValue(QString("d3caly"));

    double d6calx=m_pmanager->GetParserValue(QString("d6calx"));
    double d6caly=m_pmanager->GetParserValue(QString("d6caly"));

    double d7calx=m_pmanager->GetParserValue(QString("d7calx"));
    double d7caly=m_pmanager->GetParserValue(QString("d7caly"));

    double d4Rcalx=m_pmanager->GetParserValue(QString("d4Rcalx"));
    double d4Rcaly=m_pmanager->GetParserValue(QString("d4Rcaly"));

    double d4Lcalx=m_pmanager->GetParserValue(QString("d4Lcalx"));
    double d4Lcaly=m_pmanager->GetParserValue(QString("d4Lcaly"));

    double d5Rcalx=m_pmanager->GetParserValue(QString("d5Rcalx"));
    double d5Rcaly=m_pmanager->GetParserValue(QString("d5Rcaly"));

    double d5Lcalx=m_pmanager->GetParserValue(QString("d5Lcalx"));
    double d5Lcaly=m_pmanager->GetParserValue(QString("d5Lcaly"));

    double d8Rcalx=m_pmanager->GetParserValue(QString("d8Rcalx"));
    double d8Rcaly=m_pmanager->GetParserValue(QString("d8Rcaly"));

    double d8Lcalx=m_pmanager->GetParserValue(QString("d8Lcalx"));
    double d8Lcaly=m_pmanager->GetParserValue(QString("d8Lcaly"));

    double d9Rcalx=m_pmanager->GetParserValue(QString("d9Rcalx"));
    double d9Rcaly=m_pmanager->GetParserValue(QString("d9Rcaly"));

    double d9Lcalx=m_pmanager->GetParserValue(QString("d9Lcalx"));
    double d9Lcaly=m_pmanager->GetParserValue(QString("d9Lcaly"));



    m_cal2x = new QDoubleSpinBox;
    m_cal2xname = new QLabel;
    m_cal2y = new QDoubleSpinBox;
    m_cal2yname = new QLabel;

    m_cal3x = new QDoubleSpinBox;
    m_cal3xname = new QLabel;
    m_cal3y = new QDoubleSpinBox;
    m_cal3yname = new QLabel;

    m_cal6x = new QDoubleSpinBox;
    m_cal6xname = new QLabel;
    m_cal6y = new QDoubleSpinBox;
    m_cal6yname = new QLabel;

    m_cal7x = new QDoubleSpinBox;
    m_cal7xname = new QLabel;
    m_cal7y = new QDoubleSpinBox;
    m_cal7yname = new QLabel;









m_cal4Rx = new QDoubleSpinBox;
m_cal4Rxname = new QLabel;
m_cal4Ry = new QDoubleSpinBox;
m_cal4Ryname = new QLabel;

m_cal4Lx = new QDoubleSpinBox;
m_cal4Lxname = new QLabel;
m_cal4Ly = new QDoubleSpinBox;
m_cal4Lyname = new QLabel;


m_cal5Lx = new QDoubleSpinBox;
m_cal5Lxname = new QLabel;
m_cal5Ly = new QDoubleSpinBox;
m_cal5Lyname = new QLabel;

m_cal5Rx = new QDoubleSpinBox;
m_cal5Rxname = new QLabel;
m_cal5Ry = new QDoubleSpinBox;
m_cal5Ryname = new QLabel;



m_cal8Rx = new QDoubleSpinBox;
m_cal8Rxname = new QLabel;
m_cal8Ry = new QDoubleSpinBox;
m_cal8Ryname = new QLabel;

m_cal8Lx = new QDoubleSpinBox;
m_cal8Lxname = new QLabel;
m_cal8Ly = new QDoubleSpinBox;
m_cal8Lyname = new QLabel;

m_cal9Rx = new QDoubleSpinBox;
m_cal9Rxname = new QLabel;
m_cal9Ry = new QDoubleSpinBox;
m_cal9Ryname = new QLabel;

m_cal9Lx = new QDoubleSpinBox;
m_cal9Lxname = new QLabel;
m_cal9Ly = new QDoubleSpinBox;
m_cal9Lyname = new QLabel;




int  ix = 90;
int  iy = 289;
int  ilab = 17;
         m_cal2x->move(ix,iy);
         m_cal2x->setRange(-9999, 9999);
         m_cal2x->setSingleStep(0.2);
         m_cal2x->setValue(d2calx);
         m_cal2xname->move(ix,iy+ilab);
         m_cal2xname->setText("2x");
 ix=ix+80;
         m_cal2y->move(ix,iy);
         m_cal2y->setRange(-9999, 9999);
         m_cal2y->setSingleStep(0.2);
         m_cal2y->setValue(d2caly);
         m_cal2yname->move(ix,iy+ilab);
         m_cal2yname->setText("2y");
 iy=iy+30;
    m_cal2Button = new QPushButton(tr("2号位置拍照标定"));
    m_cal2Button->move(ix,iy);

         m_scene.addWidget(m_cal2x);
         m_scene.addWidget(m_cal2xname);
         m_scene.addWidget(m_cal2y);
         m_scene.addWidget(m_cal2yname);
         m_scene.addWidget(m_cal2Button);



         ix = 520;
         iy = 289;
         ilab = 17;
                  m_cal3x->move(ix,iy);
                  m_cal3x->setRange(-9999, 9999);
                  m_cal3x->setSingleStep(0.2);
                  m_cal3x->setValue(d3calx);
                  m_cal3xname->move(ix,iy+ilab);
                  m_cal3xname->setText("3x");
          ix=ix+80;
                  m_cal3y->move(ix,iy);
                  m_cal3y->setRange(-9999, 9999);
                  m_cal3y->setSingleStep(0.2);
                  m_cal3y->setValue(d3caly);
                  m_cal3yname->move(ix,iy+ilab);
                  m_cal3yname->setText("3y");
          iy=iy+30;
             m_cal3Button = new QPushButton(tr("3号位置拍照标定"));
             m_cal3Button->move(ix,iy);

                  m_scene.addWidget(m_cal3x);
                  m_scene.addWidget(m_cal3xname);
                  m_scene.addWidget(m_cal3y);
                  m_scene.addWidget(m_cal3yname);
                  m_scene.addWidget(m_cal3Button);

                  ix = 520;
                  iy = 70;
                  ilab = 17;
                           m_cal6x->move(ix,iy);
                           m_cal6x->setRange(-9999, 9999);
                           m_cal6x->setSingleStep(0.2);
                           m_cal6x->setValue(d6calx);
                           m_cal6xname->move(ix,iy+ilab);
                           m_cal6xname->setText("6x");
                   ix=ix+80;
                           m_cal6y->move(ix,iy);
                           m_cal6y->setRange(-9999, 9999);
                           m_cal6y->setSingleStep(0.2);
                           m_cal6y->setValue(d6caly);
                           m_cal6yname->move(ix,iy+ilab);
                           m_cal6yname->setText("6y");
                   iy=iy+30;
                      m_cal6Button = new QPushButton(tr("6号位置拍照标定"));
                      m_cal6Button->move(ix,iy);

                           m_scene.addWidget(m_cal6x);
                           m_scene.addWidget(m_cal6xname);
                           m_scene.addWidget(m_cal6y);
                           m_scene.addWidget(m_cal6yname);
                           m_scene.addWidget(m_cal6Button);


                           ix = 90;
                           iy = 70;
                           ilab = 17;
        m_cal7x->move(ix,iy);
        m_cal7x->setRange(-9999, 9999);
        m_cal7x->setSingleStep(0.2);
        m_cal7x->setValue(d7calx);
        m_cal7xname->move(ix,iy+ilab);
        m_cal7xname->setText("7x");
ix=ix+80;
        m_cal7y->move(ix,iy);
        m_cal7y->setRange(-9999, 9999);
        m_cal7y->setSingleStep(0.2);
        m_cal7y->setValue(d7caly);
        m_cal7yname->move(ix,iy+ilab);
        m_cal7yname->setText("7y");
iy=iy+30;
   m_cal7Button = new QPushButton(tr("7号位置拍照标定"));
   m_cal7Button->move(ix,iy);

        m_scene.addWidget(m_cal7x);
        m_scene.addWidget(m_cal7xname);
        m_scene.addWidget(m_cal7y);
        m_scene.addWidget(m_cal7yname);
        m_scene.addWidget(m_cal7Button);


{
            ix = 30;
            iy = 130;
            ilab = 17;
                     m_cal9Rx->move(ix,iy);
                     m_cal9Rx->setRange(-9999, 9999);
                     m_cal9Rx->setSingleStep(0.2);
                     m_cal9Rx->setValue(d9Rcalx);
                     m_cal9Rxname->move(ix,iy+ilab);
                     m_cal9Rxname->setText("9Rx");
             ix=ix+90;
                     m_cal9Ry->move(ix,iy);
                     m_cal9Ry->setRange(-9999, 9999);
                     m_cal9Ry->setSingleStep(0.2);
                     m_cal9Ry->setValue(d9Rcaly);
                     m_cal9Ryname->move(ix,iy+ilab);
                     m_cal9Ryname->setText("9Ry");
             iy=iy+30;
                m_cal9RButton = new QPushButton(tr("9R号位置拍照标定"));
                m_cal9RButton->move(ix,iy);

                     m_scene.addWidget(m_cal9Rx);
                     m_scene.addWidget(m_cal9Rxname);
                     m_scene.addWidget(m_cal9Ry);
                     m_scene.addWidget(m_cal9Ryname);
                     m_scene.addWidget(m_cal9RButton);


                     ix = 200;
                           iy = 130;
                           ilab = 17;
                                    m_cal8Rx->move(ix,iy);
                                    m_cal8Rx->setRange(-9999, 9999);
                                    m_cal8Rx->setSingleStep(0.2);
                                    m_cal8Rx->setValue(d8Rcalx);
                                    m_cal8Rxname->move(ix,iy+ilab);
                                    m_cal8Rxname->setText("8Rx");

                            ix=ix+90;
                                    m_cal8Ry->move(ix,iy);
                                    m_cal8Ry->setRange(-9999, 9999);
                                    m_cal8Ry->setSingleStep(0.2);
                                    m_cal8Ry->setValue(d8Rcaly);
                                    m_cal8Ryname->move(ix,iy+ilab);
                                    m_cal8Ryname->setText("8Ry");
                           ix=ix-30; iy=iy+30;
                                     m_cal8RButton = new QPushButton(tr("8R号位置拍照标定"));
                                    m_cal8RButton->move(ix,iy);

                                    m_scene.addWidget(m_cal8Rx);
                                    m_scene.addWidget(m_cal8Rxname);
                                    m_scene.addWidget(m_cal8Ry);
                                    m_scene.addWidget(m_cal8Ryname);
                                    m_scene.addWidget(m_cal8RButton);




        }

        {
            ix = 360;
               iy = 130;
               ilab = 17;
                        m_cal9Lx->move(ix,iy);
                        m_cal9Lx->setRange(-9999, 9999);
                        m_cal9Lx->setSingleStep(0.2);
                        m_cal9Lx->setValue(d9Lcalx);
                        m_cal9Lxname->move(ix,iy+ilab);
                        m_cal9Lxname->setText("9Lx");
                ix=ix+90;
                        m_cal9Ly->move(ix,iy);
                        m_cal9Ly->setRange(-9999, 9999);
                        m_cal9Ly->setSingleStep(0.2);
                        m_cal9Ly->setValue(d9Lcaly);
                        m_cal9Lyname->move(ix,iy+ilab);
                        m_cal9Lyname->setText("9Ly");
                iy=iy+30;
                   m_cal9LButton = new QPushButton(tr("9L号位置拍照标定"));
                   m_cal9LButton->move(ix,iy);

                        m_scene.addWidget(m_cal9Lx);
                        m_scene.addWidget(m_cal9Lxname);
                        m_scene.addWidget(m_cal9Ly);
                        m_scene.addWidget(m_cal9Lyname);
                        m_scene.addWidget(m_cal9LButton);

                        ix = 530;
                              iy = 130;
                              ilab = 17;
                                       m_cal8Lx->move(ix,iy);
                                       m_cal8Lx->setRange(-9999, 9999);
                                       m_cal8Lx->setSingleStep(0.2);
                                       m_cal8Lx->setValue(d8Lcalx);
                                       m_cal8Lxname->move(ix,iy+ilab);
                                       m_cal8Lxname->setText("8Lx");

                               ix=ix+90;
                                       m_cal8Ly->move(ix,iy);
                                       m_cal8Ly->setRange(-9999, 9999);
                                       m_cal8Ly->setSingleStep(0.2);
                                       m_cal8Ly->setValue(d8Lcaly);
                                       m_cal8Lyname->move(ix,iy+ilab);
                                       m_cal8Lyname->setText("8Ly");
                              ix=ix-30; iy=iy+30;
                                        m_cal8LButton = new QPushButton(tr("8L号位置拍照标定"));
                                       m_cal8LButton->move(ix,iy);

                                       m_scene.addWidget(m_cal8Lx);
                                       m_scene.addWidget(m_cal8Lxname);
                                       m_scene.addWidget(m_cal8Ly);
                                       m_scene.addWidget(m_cal8Lyname);
                                       m_scene.addWidget(m_cal8LButton);

                }

        {
                    ix = 10;
                    iy = 210;
                    ilab = 17;
                             m_cal4Rx->move(ix,iy);
                             m_cal4Rx->setRange(-9999, 9999);
                             m_cal4Rx->setSingleStep(0.2);
                             m_cal4Rx->setValue(d4Rcalx);
                             m_cal4Rxname->move(ix,iy+ilab);
                             m_cal4Rxname->setText("4Rx");
                     ix=ix+90;
                             m_cal4Ry->move(ix,iy);
                             m_cal4Ry->setRange(-9999, 9999);
                             m_cal4Ry->setSingleStep(0.2);
                             m_cal4Ry->setValue(d4Rcaly);
                             m_cal4Ryname->move(ix,iy+ilab);
                             m_cal4Ryname->setText("4Ry");
                     iy=iy+30;
                        m_cal4RButton = new QPushButton(tr("4R号位置拍照标定"));
                        m_cal4RButton->move(ix,iy);

                             m_scene.addWidget(m_cal4Rx);
                             m_scene.addWidget(m_cal4Rxname);
                             m_scene.addWidget(m_cal4Ry);
                             m_scene.addWidget(m_cal4Ryname);
                             m_scene.addWidget(m_cal4RButton);


                             ix = 180;
                                   iy =210;
                                   ilab = 17;
                                            m_cal5Rx->move(ix,iy);
                                            m_cal5Rx->setRange(-9999, 9999);
                                            m_cal5Rx->setSingleStep(0.2);
                                            m_cal5Rx->setValue(d5Rcalx);
                                            m_cal5Rxname->move(ix,iy+ilab);
                                            m_cal5Rxname->setText("5Rx");

                                    ix=ix+90;
                                            m_cal5Ry->move(ix,iy);
                                            m_cal5Ry->setRange(-9999, 9999);
                                            m_cal5Ry->setSingleStep(0.2);
                                            m_cal5Ry->setValue(d5Rcaly);
                                            m_cal5Ryname->move(ix,iy+ilab);
                                            m_cal5Ryname->setText("5Ry");
                                   ix=ix-30; iy=iy+30;
                                             m_cal5RButton = new QPushButton(tr("5R号位置拍照标定"));
                                            m_cal5RButton->move(ix,iy);

                                            m_scene.addWidget(m_cal5Rx);
                                            m_scene.addWidget(m_cal5Rxname);
                                            m_scene.addWidget(m_cal5Ry);
                                            m_scene.addWidget(m_cal5Ryname);
                                            m_scene.addWidget(m_cal5RButton);




                }

                {
                    ix = 330;
                       iy = 210;
                       ilab = 17;
                                m_cal4Lx->move(ix,iy);
                                m_cal4Lx->setRange(-9999, 9999);
                                m_cal4Lx->setSingleStep(0.2);
                                m_cal4Lx->setValue(d4Lcalx);
                                m_cal4Lxname->move(ix,iy+ilab);
                                m_cal4Lxname->setText("4Lx");
                        ix=ix+90;
                                m_cal4Ly->move(ix,iy);
                                m_cal4Ly->setRange(-9999, 9999);
                                m_cal4Ly->setSingleStep(0.2);
                                m_cal4Ly->setValue(d4Lcaly);
                                m_cal4Lyname->move(ix,iy+ilab);
                                m_cal4Lyname->setText("4Ly");
                        iy=iy+30;
                           m_cal4LButton = new QPushButton(tr("4L号位置拍照标定"));
                           m_cal4LButton->move(ix,iy);

                                m_scene.addWidget(m_cal4Lx);
                                m_scene.addWidget(m_cal4Lxname);
                                m_scene.addWidget(m_cal4Ly);
                                m_scene.addWidget(m_cal4Lyname);
                                m_scene.addWidget(m_cal4LButton);

                                ix = 500;
                                      iy = 210;
                                      ilab = 17;
                                               m_cal5Lx->move(ix,iy);
                                               m_cal5Lx->setRange(-9999, 9999);
                                               m_cal5Lx->setSingleStep(0.2);
                                               m_cal5Lx->setValue(d5Lcalx);
                                               m_cal5Lxname->move(ix,iy+ilab);
                                               m_cal5Lxname->setText("5Lx");

                                       ix=ix+90;
                                               m_cal5Ly->move(ix,iy);
                                               m_cal5Ly->setRange(-9999, 9999);
                                               m_cal5Ly->setSingleStep(0.2);
                                               m_cal5Ly->setValue(d5Lcaly);
                                               m_cal5Lyname->move(ix,iy+ilab);
                                               m_cal5Lyname->setText("5Ly");
                                      ix=ix-30; iy=iy+30;
                                                m_cal5LButton = new QPushButton(tr("5L号位置拍照标定"));
                                               m_cal5LButton->move(ix,iy);

                                               m_scene.addWidget(m_cal5Lx);
                                               m_scene.addWidget(m_cal5Lxname);
                                               m_scene.addWidget(m_cal5Ly);
                                               m_scene.addWidget(m_cal5Lyname);
                                               m_scene.addWidget(m_cal5LButton);

                        }



m_resultlabel = new QLabel;
m_resultlabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
m_resultlabel->setText("标定状态                 ");
m_resultlabel->setAlignment(Qt::AlignBottom | Qt::AlignRight);
m_resultlabel->move(20,20);
m_scene.addWidget(m_resultlabel);

        m_saveButton = new QPushButton(tr("保存"));

         m_saveButton->move(550,400);

         m_scene.addWidget(m_saveButton);


         connect(m_saveButton, &QAbstractButton::clicked, this, &CameraCalibDialog::save);

         connect(m_cal2Button, &QAbstractButton::clicked, this, &CameraCalibDialog::calpos2);
         connect(m_cal3Button, &QAbstractButton::clicked, this, &CameraCalibDialog::calpos3);
         connect(m_cal6Button, &QAbstractButton::clicked, this, &CameraCalibDialog::calpos6);
         connect(m_cal7Button, &QAbstractButton::clicked, this, &CameraCalibDialog::calpos7);

         connect(m_cal4LButton, &QAbstractButton::clicked, this, &CameraCalibDialog::calpos4L);
         connect(m_cal4RButton, &QAbstractButton::clicked, this, &CameraCalibDialog::calpos4R);
         connect(m_cal5LButton, &QAbstractButton::clicked, this, &CameraCalibDialog::calpos5L);
         connect(m_cal5RButton, &QAbstractButton::clicked, this, &CameraCalibDialog::calpos5R);

         connect(m_cal8LButton, &QAbstractButton::clicked, this, &CameraCalibDialog::calpos8L);
         connect(m_cal8RButton, &QAbstractButton::clicked, this, &CameraCalibDialog::calpos8R);
         connect(m_cal9LButton, &QAbstractButton::clicked, this, &CameraCalibDialog::calpos9L);
         connect(m_cal9RButton, &QAbstractButton::clicked, this, &CameraCalibDialog::calpos9R);






//! [3]

//! [4]
     setScene(&m_scene);
 //   view.setRenderHint(QPainter::Antialiasing);
    setBackgroundBrush(QPixmap(":/images/cal.png"));
//! [4] //! [5]
  //  view.setCacheMode(QGraphicsView::CacheBackground);
  //  view.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  //  view.setDragMode(QGraphicsView::ScrollHandDrag);
//! [5] //! [6]
  //  view.setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Colliding Mice"));
    resize(720, 480);
    show();
}
/*
double d45Rcalx=m_pmanager->GetParserValue(QString("d45Rcalx"));
double d45Rcaly=m_pmanager->GetParserValue(QString("d45Rcaly"));
double d45Rcalr=m_pmanager->GetParserValue(QString("d45Rcalr"));

double d45Lcalx=m_pmanager->GetParserValue(QString("d45Lcalx"));
double d45Lcaly=m_pmanager->GetParserValue(QString("d45Lcaly"));
double d45Lcalr=m_pmanager->GetParserValue(QString("d45Lcalr"));

double d89Lcalx=m_pmanager->GetParserValue(QString("d89Lcalx"));
double d89Lcaly=m_pmanager->GetParserValue(QString("d89Lcaly"));
double d89Lcalr=m_pmanager->GetParserValue(QString("d89Lcalr"));

double d89Rcalx=m_pmanager->GetParserValue(QString("d89Rcalx"));
double d89Rcaly=m_pmanager->GetParserValue(QString("d89Rcaly"));
double d89Rcalr=m_pmanager->GetParserValue(QString("d89Rcalr"));
*/
void CameraCalibDialog::save()
{
 /*   m_pmanager->SetParserValue(QString("d45Rcalx"),m_cal45Rx->value());
    m_pmanager->SetParserValue(QString("d45Rcaly"),m_cal45Ry->value());
    m_pmanager->SetParserValue(QString("d45Rcalr"),m_cal45Ra->value());

    m_pmanager->SetParserValue(QString("d45Lcalx"),m_cal45Lx->value());
    m_pmanager->SetParserValue(QString("d45Lcaly"),m_cal45Ly->value());
    m_pmanager->SetParserValue(QString("d45Lcalr"),m_cal45La->value());

    m_pmanager->SetParserValue(QString("d89Rcalx"),m_cal89Rx->value());
    m_pmanager->SetParserValue(QString("d89Rcaly"),m_cal89Ry->value());
    m_pmanager->SetParserValue(QString("d89Rcalr"),m_cal89Ra->value());

    m_pmanager->SetParserValue(QString("d89Lcalx"),m_cal89Lx->value());
    m_pmanager->SetParserValue(QString("d89Lcaly"),m_cal89Ly->value());
    m_pmanager->SetParserValue(QString("d89Lcalr"),m_cal89La->value());







    m_pmanager->SetParserValue(QString("ds0Rcalx1"),m_calRx1->value());
    m_pmanager->SetParserValue(QString("ds0Rcaly1"),m_calRy1->value());
    m_pmanager->SetParserValue(QString("ds0Rcalr1"),m_calRa1->value());

    m_pmanager->SetParserValue(QString("ds0Rcalx2"),m_calRx2->value());
    m_pmanager->SetParserValue(QString("ds0Rcaly2"),m_calRy2->value());
    m_pmanager->SetParserValue(QString("ds0Rcalr2"),m_calRa2->value());

    m_pmanager->SetParserValue(QString("ds0Rcalx3"),m_calRx3->value());
    m_pmanager->SetParserValue(QString("ds0Rcaly3"),m_calRy3->value());
    m_pmanager->SetParserValue(QString("ds0Rcalr3"),m_calRa3->value());

    m_pmanager->SetParserValue(QString("ds0Rcalx4"),m_calRx4->value());
    m_pmanager->SetParserValue(QString("ds0Rcaly4"),m_calRy4->value());
    m_pmanager->SetParserValue(QString("ds0Rcalr4"),m_calRa4->value());

    m_pmanager->SetParserValue(QString("ds0Rcalx5"),m_calRx5->value());
    m_pmanager->SetParserValue(QString("ds0Rcaly5"),m_calRy5->value());
    m_pmanager->SetParserValue(QString("ds0Rcalr5"),m_calRa5->value());

    m_pmanager->SetParserValue(QString("ds0Rcalx6"),m_calRx6->value());
    m_pmanager->SetParserValue(QString("ds0Rcaly6"),m_calRy6->value());
    m_pmanager->SetParserValue(QString("ds0Rcalr6"),m_calRa6->value());




    m_pmanager->SetParserValue(QString("ds1Lcalx1"),m_calLx1->value());
    m_pmanager->SetParserValue(QString("ds1Lcaly1"),m_calLy1->value());
    m_pmanager->SetParserValue(QString("ds1Lcalr1"),m_calLa1->value());

    m_pmanager->SetParserValue(QString("ds1Lcalx2"),m_calLx2->value());
    m_pmanager->SetParserValue(QString("ds1Lcaly2"),m_calLy2->value());
    m_pmanager->SetParserValue(QString("ds1Lcalr2"),m_calLa2->value());

    m_pmanager->SetParserValue(QString("ds1Lcalx3"),m_calLx3->value());
    m_pmanager->SetParserValue(QString("ds1Lcaly3"),m_calLy3->value());
    m_pmanager->SetParserValue(QString("ds1Lcalr3"),m_calLa3->value());

    m_pmanager->SetParserValue(QString("ds1Lcalx4"),m_calLx4->value());
    m_pmanager->SetParserValue(QString("ds1Lcaly4"),m_calLy4->value());
    m_pmanager->SetParserValue(QString("ds1Lcalr4"),m_calLa4->value());

    m_pmanager->SetParserValue(QString("ds1Lcalx5"),m_calLx5->value());
    m_pmanager->SetParserValue(QString("ds1Lcaly5"),m_calLy5->value());
    m_pmanager->SetParserValue(QString("ds1Lcalr5"),m_calLa5->value());

    m_pmanager->SetParserValue(QString("ds1Lcalx6"),m_calLx6->value());
    m_pmanager->SetParserValue(QString("ds1Lcaly6"),m_calLy6->value());
    m_pmanager->SetParserValue(QString("ds1Lcalr6"),m_calLa6->value());





*/

double drun=m_pmanager->GetParserValue(QString("drunRL"));
     m_pmanager->SetParserValue(QString("drunRL"),0);
    m_pmanager->savecalibfile();

    m_pmanager->SetParserValue(QString("drunRL"),drun);


   //
}

void CameraCalibDialog::calpos2()
{
    m_pmanager->cameracalib("calpos2");
    double dresult=m_pmanager->GetParserValue(QString("result"));
    if(dresult>0)
    {
        m_resultlabel->setText("位置2标定失败");
    }
    else
    {

        m_resultlabel->setText("位置2标定成功");
    }

}
void CameraCalibDialog::calpos3()
{
    m_pmanager->cameracalib("calpos3");
    double dresult=m_pmanager->GetParserValue(QString("result"));
    if(dresult>0)
    {
        m_resultlabel->setText("位置3标定失败");
    }
    else
    {

        m_resultlabel->setText("位置3标定成功");
    }
}
void CameraCalibDialog::calpos6()
{
    m_pmanager->cameracalib("calpos6");
    double dresult=m_pmanager->GetParserValue(QString("result"));
    if(dresult>0)
    {
        m_resultlabel->setText("位置6标定失败");
    }
    else
    {

        m_resultlabel->setText("位置6标定成功");
    }
}
void CameraCalibDialog::calpos7()
{
    m_pmanager->cameracalib("calpos7");
    double dresult=m_pmanager->GetParserValue(QString("result"));
    if(dresult>0)
    {
        m_resultlabel->setText("位置7标定失败");
    }
    else
    {

        m_resultlabel->setText("位置7标定成功");
    }
}
void CameraCalibDialog::calpos4L()
{
    m_pmanager->cameracalib("calpos4L");
    double dresult=m_pmanager->GetParserValue(QString("result"));
    if(dresult>0)
    {
        m_resultlabel->setText("位置4L标定失败");
    }
    else
    {

        m_resultlabel->setText("位置4L标定成功");
    }
}
void CameraCalibDialog::calpos5L()
{
    m_pmanager->cameracalib("calpos5L");
    double dresult=m_pmanager->GetParserValue(QString("result"));
    if(dresult>0)
    {
        m_resultlabel->setText("位置5L标定失败");
    }
    else
    {

        m_resultlabel->setText("位置5L标定成功");
    }

}
void CameraCalibDialog::calpos4R()
{
    m_pmanager->cameracalib("calpos4R");
    double dresult=m_pmanager->GetParserValue(QString("result"));
    if(dresult>0)
    {
        m_resultlabel->setText("位置4R标定失败");
    }
    else
    {

        m_resultlabel->setText("位置4R标定成功");
    }

}
void CameraCalibDialog::calpos5R()
{
    m_pmanager->cameracalib("calpos5R");
    double dresult=m_pmanager->GetParserValue(QString("result"));
    if(dresult>0)
    {
        m_resultlabel->setText("位置5R标定失败");
    }
    else
    {

        m_resultlabel->setText("位置5R标定成功");
    }}
void CameraCalibDialog::calpos9L()
{
    m_pmanager->cameracalib("calpos9L");
    double dresult=m_pmanager->GetParserValue(QString("result"));
    if(dresult>0)
    {
        m_resultlabel->setText("位置9L标定失败");
    }
    else
    {

        m_resultlabel->setText("位置9L标定成功");
    }

}
void CameraCalibDialog::calpos8L()
{
    m_pmanager->cameracalib("calpos8L");
    double dresult=m_pmanager->GetParserValue(QString("result"));
    if(dresult>0)
    {
        m_resultlabel->setText("位置8L标定失败");
    }
    else
    {

        m_resultlabel->setText("位置8L标定成功");
    }

}
void CameraCalibDialog::calpos9R()
{
    m_pmanager->cameracalib("calpos9R");
    double dresult=m_pmanager->GetParserValue(QString("result"));
    if(dresult>0)
    {
        m_resultlabel->setText("位置9R标定失败");
    }
    else
    {

        m_resultlabel->setText("位置9R标定成功");
    }

}
void CameraCalibDialog::calpos8R()
{
    m_pmanager->cameracalib("calpos8R");
    double dresult=m_pmanager->GetParserValue(QString("result"));
    if(dresult>0)
    {
        m_resultlabel->setText("位置8R标定失败");
    }
    else
    {

        m_resultlabel->setText("位置8R标定成功");
    }

}
