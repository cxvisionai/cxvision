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

#include "robotCalibDialog.h"

#include <QGraphicsView>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>
#include "cximage/visionmanager.h"
RobotCalibDialog::RobotCalibDialog(visionmanager *pmanager,QGraphicsView *pview)
    : QGraphicsView(pview)
    ,m_pmanager(pmanager)
{
    m_scene.setSceneRect(0, 0, 630, 470);

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


double dlcalx1=m_pmanager->GetParserValue(QString("ds1Lcalx1"));
double dlcaly1=m_pmanager->GetParserValue(QString("ds1Lcaly1"));
double dlcalr1=m_pmanager->GetParserValue(QString("ds1Lcalr1"));

double dlcalx2=m_pmanager->GetParserValue(QString("ds1Lcalx2"));
double dlcaly2=m_pmanager->GetParserValue(QString("ds1Lcaly2"));
double dlcalr2=m_pmanager->GetParserValue(QString("ds1Lcalr2"));

double dlcalx3=m_pmanager->GetParserValue(QString("ds1Lcalx3"));
double dlcaly3=m_pmanager->GetParserValue(QString("ds1Lcaly3"));
double dlcalr3=m_pmanager->GetParserValue(QString("ds1Lcalr3"));

double dlcalx4=m_pmanager->GetParserValue(QString("ds1Lcalx4"));
double dlcaly4=m_pmanager->GetParserValue(QString("ds1Lcaly4"));
double dlcalr4=m_pmanager->GetParserValue(QString("ds1Lcalr4"));

double dlcalx5=m_pmanager->GetParserValue(QString("ds1Lcalx5"));
double dlcaly5=m_pmanager->GetParserValue(QString("ds1Lcaly5"));
double dlcalr5=m_pmanager->GetParserValue(QString("ds1Lcalr5"));

double dlcalx6=m_pmanager->GetParserValue(QString("ds1Lcalx6"));
double dlcaly6=m_pmanager->GetParserValue(QString("ds1Lcaly6"));
double dlcalr6=m_pmanager->GetParserValue(QString("ds1Lcalr6"));


double drcalx1=m_pmanager->GetParserValue(QString("ds0Rcalx1"));
double drcaly1=m_pmanager->GetParserValue(QString("ds0Rcaly1"));
double drcalr1=m_pmanager->GetParserValue(QString("ds0Rcalr1"));

double drcalx2=m_pmanager->GetParserValue(QString("ds0Rcalx2"));
double drcaly2=m_pmanager->GetParserValue(QString("ds0Rcaly2"));
double drcalr2=m_pmanager->GetParserValue(QString("ds0Rcalr2"));

double drcalx3=m_pmanager->GetParserValue(QString("ds0Rcalx3"));
double drcaly3=m_pmanager->GetParserValue(QString("ds0Rcaly3"));
double drcalr3=m_pmanager->GetParserValue(QString("ds0Rcalr3"));

double drcalx4=m_pmanager->GetParserValue(QString("ds0Rcalx4"));
double drcaly4=m_pmanager->GetParserValue(QString("ds0Rcaly4"));
double drcalr4=m_pmanager->GetParserValue(QString("ds0Rcalr4"));

double drcalx5=m_pmanager->GetParserValue(QString("ds0Rcalx5"));
double drcaly5=m_pmanager->GetParserValue(QString("ds0Rcaly5"));
double drcalr5=m_pmanager->GetParserValue(QString("ds0Rcalr5"));

double drcalx6=m_pmanager->GetParserValue(QString("ds0Rcalx6"));
double drcaly6=m_pmanager->GetParserValue(QString("ds0Rcaly6"));
double drcalr6=m_pmanager->GetParserValue(QString("ds0Rcalr6"));













m_cal45Rx = new QDoubleSpinBox;
m_cal45Rxname = new QLabel;
m_cal45Ry = new QDoubleSpinBox;
m_cal45Ryname = new QLabel;
m_cal45Ra = new QDoubleSpinBox;
m_cal45Raname = new QLabel;

m_cal45Lx = new QDoubleSpinBox;
m_cal45Lxname = new QLabel;
m_cal45Ly = new QDoubleSpinBox;
m_cal45Lyname = new QLabel;
m_cal45La = new QDoubleSpinBox;
m_cal45Laname = new QLabel;

m_cal89Rx = new QDoubleSpinBox;
m_cal89Rxname = new QLabel;
m_cal89Ry = new QDoubleSpinBox;
m_cal89Ryname = new QLabel;
m_cal89Ra = new QDoubleSpinBox;
m_cal89Raname = new QLabel;

m_cal89Lx = new QDoubleSpinBox;
m_cal89Lxname = new QLabel;
m_cal89Ly = new QDoubleSpinBox;
m_cal89Lyname = new QLabel;
m_cal89La = new QDoubleSpinBox;
m_cal89Laname = new QLabel;


{

    m_calLx1 = new QDoubleSpinBox;
    m_calLxname1 = new QLabel;
    m_calLy1 = new QDoubleSpinBox;
    m_calLyname1 = new QLabel;
    m_calLa1 = new QDoubleSpinBox;
    m_calLaname1 = new QLabel;

    m_calLx2 = new QDoubleSpinBox;
    m_calLxname2 = new QLabel;
    m_calLy2 = new QDoubleSpinBox;
    m_calLyname2 = new QLabel;
    m_calLa2 = new QDoubleSpinBox;
    m_calLaname2 = new QLabel;

    m_calLx3 = new QDoubleSpinBox;
    m_calLxname3 = new QLabel;
    m_calLy3 = new QDoubleSpinBox;
    m_calLyname3 = new QLabel;
    m_calLa3 = new QDoubleSpinBox;
    m_calLaname3 = new QLabel;

    m_calLx4 = new QDoubleSpinBox;
    m_calLxname4 = new QLabel;
    m_calLy4 = new QDoubleSpinBox;
    m_calLyname4 = new QLabel;
    m_calLa4 = new QDoubleSpinBox;
    m_calLaname4 = new QLabel;

    m_calLx5 = new QDoubleSpinBox;
    m_calLxname5 = new QLabel;
    m_calLy5 = new QDoubleSpinBox;
    m_calLyname5 = new QLabel;
    m_calLa5 = new QDoubleSpinBox;
    m_calLaname5 = new QLabel;

    m_calLx6 = new QDoubleSpinBox;
    m_calLxname6 = new QLabel;
    m_calLy6 = new QDoubleSpinBox;
    m_calLyname6 = new QLabel;
    m_calLa6 = new QDoubleSpinBox;
    m_calLaname6 = new QLabel;

}

{

    m_calRx1 = new QDoubleSpinBox;
    m_calRxname1 = new QLabel;
    m_calRy1 = new QDoubleSpinBox;
    m_calRyname1 = new QLabel;
    m_calRa1 = new QDoubleSpinBox;
    m_calRaname1 = new QLabel;

    m_calRx2 = new QDoubleSpinBox;
    m_calRxname2 = new QLabel;
    m_calRy2 = new QDoubleSpinBox;
    m_calRyname2 = new QLabel;
    m_calRa2 = new QDoubleSpinBox;
    m_calRaname2 = new QLabel;

    m_calRx3 = new QDoubleSpinBox;
    m_calRxname3 = new QLabel;
    m_calRy3 = new QDoubleSpinBox;
    m_calRyname3 = new QLabel;
    m_calRa3 = new QDoubleSpinBox;
    m_calRaname3 = new QLabel;

    m_calRx4 = new QDoubleSpinBox;
    m_calRxname4 = new QLabel;
    m_calRy4 = new QDoubleSpinBox;
    m_calRyname4 = new QLabel;
    m_calRa4 = new QDoubleSpinBox;
    m_calRaname4 = new QLabel;

    m_calRx5 = new QDoubleSpinBox;
    m_calRxname5 = new QLabel;
    m_calRy5 = new QDoubleSpinBox;
    m_calRyname5 = new QLabel;
    m_calRa5 = new QDoubleSpinBox;
    m_calRaname5 = new QLabel;

    m_calRx6 = new QDoubleSpinBox;
    m_calRxname6 = new QLabel;
    m_calRy6 = new QDoubleSpinBox;
    m_calRyname6 = new QLabel;
    m_calRa6 = new QDoubleSpinBox;
    m_calRaname6 = new QLabel;


}




      int  ix = 120;
      int  iy = 269;
      int  ilab = 17;

        m_cal45Rx->move(ix,iy);
        m_cal45Rx->setRange(-9999, 9999);
        m_cal45Rx->setSingleStep(0.2);
        m_cal45Rx->setValue(d45Rcalx);
        m_cal45Rxname->move(ix,iy+ilab);
        m_cal45Rxname->setText("45Rx");
ix=ix+80;
        m_cal45Ry->move(ix,iy);
        m_cal45Ry->setRange(-9999, 9999);
        m_cal45Ry->setSingleStep(0.2);
        m_cal45Ry->setValue(d45Rcaly);
        m_cal45Ryname->move(ix,iy+ilab);
        m_cal45Ryname->setText("45Ry");
ix=ix+80;
        m_cal45Ra->move(ix,iy);
        m_cal45Ra->setRange(-9999, 9999);
        m_cal45Ra->setSingleStep(0.2);
        m_cal45Ra->setValue(d45Rcalr);
        m_cal45Raname->move(ix,iy+ilab);
        m_cal45Raname->setText("45Ra");

        m_scene.addWidget(m_cal45Rx);
        m_scene.addWidget(m_cal45Rxname);
        m_scene.addWidget(m_cal45Ry);
        m_scene.addWidget(m_cal45Ryname);
        m_scene.addWidget(m_cal45Ra);
        m_scene.addWidget(m_cal45Raname);
 ix=ix+100;
        m_cal45Lx->move(ix,iy);
        m_cal45Lx->setRange(-9999, 9999);
        m_cal45Lx->setSingleStep(0.2);
        m_cal45Lx->setValue(d45Lcalx);
        m_cal45Lxname->move(ix,iy+ilab);
        m_cal45Lxname->setText("45Lx");
ix=ix+80;
        m_cal45Ly->move(ix,iy);
        m_cal45Ly->setRange(-9999, 9999);
        m_cal45Ly->setSingleStep(0.2);
        m_cal45Ly->setValue(d45Lcaly);
        m_cal45Lyname->move(ix,iy+ilab);
        m_cal45Lyname->setText("45Ly");
ix=ix+80;
        m_cal45La->move(ix,iy);
        m_cal45La->setRange(-9999, 9999);
        m_cal45La->setSingleStep(0.2);
        m_cal45La->setValue(d45Lcalr);
        m_cal45Laname->move(ix,iy+ilab);
        m_cal45Laname->setText("45La");

        m_scene.addWidget(m_cal45Lx);
        m_scene.addWidget(m_cal45Lxname);
        m_scene.addWidget(m_cal45Ly);
        m_scene.addWidget(m_cal45Lyname);
        m_scene.addWidget(m_cal45La);
        m_scene.addWidget(m_cal45Laname);



        ix = 130;
        iy = 210;
        ilab = -10;

         m_cal89Rx->move(ix,iy);
         m_cal89Rx->setRange(-9999, 9999);
         m_cal89Rx->setSingleStep(0.2);
         m_cal89Rx->setValue(d89Rcalx);
         m_cal89Rxname->move(ix,iy+ilab);
         m_cal89Rxname->setText("89Rx");
 ix=ix+80;
         m_cal89Ry->move(ix,iy);
         m_cal89Ry->setRange(-9999, 9999);
         m_cal89Ry->setSingleStep(0.2);
         m_cal89Ry->setValue(d89Rcaly);
         m_cal89Ryname->move(ix,iy+ilab);
         m_cal89Ryname->setText("89Ry");
 ix=ix+80;
         m_cal89Ra->move(ix,iy);
         m_cal89Ra->setRange(-9999, 9999);
         m_cal89Ra->setSingleStep(0.2);
         m_cal89Ra->setValue(d89Rcalr);
         m_cal89Raname->move(ix,iy+ilab);
         m_cal89Raname->setText("89Ra");

         m_scene.addWidget(m_cal89Rx);
         m_scene.addWidget(m_cal89Rxname);
         m_scene.addWidget(m_cal89Ry);
         m_scene.addWidget(m_cal89Ryname);
         m_scene.addWidget(m_cal89Ra);
         m_scene.addWidget(m_cal89Raname);
  ix=ix+100;
         m_cal89Lx->move(ix,iy);
         m_cal89Lx->setRange(-9999, 9999);
         m_cal89Lx->setSingleStep(0.2);
         m_cal89Lx->setValue(d89Lcalx);
         m_cal89Lxname->move(ix,iy+ilab);
         m_cal89Lxname->setText("89Lx");
 ix=ix+80;
         m_cal89Ly->move(ix,iy);
         m_cal89Ly->setRange(-9999, 9999);
         m_cal89Ly->setSingleStep(0.2);
         m_cal89Ly->setValue(d89Lcaly);
         m_cal89Lyname->move(ix,iy+ilab);
         m_cal89Lyname->setText("89Ly");
 ix=ix+80;
         m_cal89La->move(ix,iy);
         m_cal89La->setRange(-9999, 9999);
         m_cal89La->setSingleStep(0.2);
         m_cal89La->setValue(d89Lcalr);
         m_cal89Laname->move(ix,iy+ilab);
         m_cal89Laname->setText("89La");

         m_scene.addWidget(m_cal89Lx);
         m_scene.addWidget(m_cal89Lxname);
         m_scene.addWidget(m_cal89Ly);
         m_scene.addWidget(m_cal89Lyname);
         m_scene.addWidget(m_cal89La);
         m_scene.addWidget(m_cal89Laname);



         {

             ix =180;
             iy = 455;
             ilab = -15;

             m_calRx1->move(ix,iy);
             m_calRx1->setRange(-9999, 9999);
             m_calRx1->setSingleStep(0.2);
             m_calRx1->setValue(drcalx1);
             m_calRxname1->move(ix+ilab,iy+3);
             m_calRxname1->setText("Rx1");
             ix=ix+100;
             m_calRy1->move(ix,iy);
             m_calRy1->setRange(-9999, 9999);
             m_calRy1->setSingleStep(0.2);
             m_calRy1->setValue(drcaly1);
             m_calRyname1->move(ix+ilab,iy+3);
             m_calRyname1->setText("Ry1");
             ix=ix+100;
             m_calRa1->move(ix,iy);
             m_calRa1->setRange(-9999, 9999);
             m_calRa1->setSingleStep(0.2);
             m_calRa1->setValue(drcalr1);
             m_calRaname1->move(ix+ilab,iy+3);
             m_calRaname1->setText("Ra1");

             m_scene.addWidget(m_calRx1);
             m_scene.addWidget(m_calRxname1);
             m_scene.addWidget(m_calRy1);
             m_scene.addWidget(m_calRyname1);
             m_scene.addWidget(m_calRa1);
             m_scene.addWidget(m_calRaname1);

             ix =180;
             iy=iy-30;
             m_calRx2->move(ix,iy);
             m_calRx2->setRange(-9999, 9999);
             m_calRx2->setSingleStep(0.2);
             m_calRx2->setValue(drcalx2);
             m_calRxname2->move(ix+ilab,iy+3);
             m_calRxname2->setText("Rx2");
             ix=ix+100;
             m_calRy2->move(ix,iy);
             m_calRy2->setRange(-9999, 9999);
             m_calRy2->setSingleStep(0.2);
             m_calRy2->setValue(drcaly2);
             m_calRyname2->move(ix+ilab,iy+3);
             m_calRyname2->setText("Ry2");
             ix=ix+100;
             m_calRa2->move(ix,iy);
             m_calRa2->setRange(-9999, 9999);
             m_calRa2->setSingleStep(0.2);
             m_calRa2->setValue(drcalr2);
             m_calRaname2->move(ix+ilab,iy+3);
             m_calRaname2->setText("Ra2");

             m_scene.addWidget(m_calRx2);
             m_scene.addWidget(m_calRxname2);
             m_scene.addWidget(m_calRy2);
             m_scene.addWidget(m_calRyname2);
             m_scene.addWidget(m_calRa2);
             m_scene.addWidget(m_calRaname2);


             ix =180;
             iy=iy-30;
             m_calRx3->move(ix,iy);
             m_calRx3->setRange(-9999, 9999);
             m_calRx3->setSingleStep(0.2);
             m_calRx3->setValue(drcalx3);
             m_calRxname3->move(ix+ilab,iy+3);
             m_calRxname3->setText("Rx3");
             ix=ix+100;
             m_calRy3->move(ix,iy);
             m_calRy3->setRange(-9999, 9999);
             m_calRy3->setSingleStep(0.2);
             m_calRy3->setValue(drcaly3);
             m_calRyname3->move(ix+ilab,iy+3);
             m_calRyname3->setText("Ry3");
             ix=ix+100;
             m_calRa3->move(ix,iy);
             m_calRa3->setRange(-9999, 9999);
             m_calRa3->setSingleStep(0.2);
             m_calRa3->setValue(drcalr3);
             m_calRaname3->move(ix+ilab,iy+3);
             m_calRaname3->setText("Ra3");

             m_scene.addWidget(m_calRx3);
             m_scene.addWidget(m_calRxname3);
             m_scene.addWidget(m_calRy3);
             m_scene.addWidget(m_calRyname3);
             m_scene.addWidget(m_calRa3);
             m_scene.addWidget(m_calRaname3);



             ix =180;
             iy=iy-30;
             m_calRx4->move(ix,iy);
             m_calRx4->setRange(-9999, 9999);
             m_calRx4->setSingleStep(0.2);
             m_calRx4->setValue(drcalx4);
             m_calRxname4->move(ix+ilab,iy+3);
             m_calRxname4->setText("Rx4");
             ix=ix+100;
             m_calRy4->move(ix,iy);
             m_calRy4->setRange(-9999, 9999);
             m_calRy4->setSingleStep(0.2);
             m_calRy4->setValue(drcaly4);
             m_calRyname4->move(ix+ilab,iy+3);
             m_calRyname4->setText("Ry4");
             ix=ix+100;
             m_calRa4->move(ix,iy);
             m_calRa4->setRange(-9999, 9999);
             m_calRa4->setSingleStep(0.2);
             m_calRa4->setValue(drcalr4);
             m_calRaname4->move(ix+ilab,iy+3);
             m_calRaname4->setText("Ra4");

             m_scene.addWidget(m_calRx4);
             m_scene.addWidget(m_calRxname4);
             m_scene.addWidget(m_calRy4);
             m_scene.addWidget(m_calRyname4);
             m_scene.addWidget(m_calRa4);
             m_scene.addWidget(m_calRaname4);


             ix =180;
             iy=iy-30;
             m_calRx5->move(ix,iy);
             m_calRx5->setRange(-9999, 9999);
             m_calRx5->setSingleStep(0.2);
             m_calRx5->setValue(drcalx5);
             m_calRxname5->move(ix+ilab,iy+3);
             m_calRxname5->setText("Rx5");
             ix=ix+100;
             m_calRy5->move(ix,iy);
             m_calRy5->setRange(-9999, 9999);
             m_calRy5->setSingleStep(0.2);
             m_calRy5->setValue(drcaly5);
             m_calRyname5->move(ix+ilab,iy+3);
             m_calRyname5->setText("Ry5");
             ix=ix+100;
             m_calRa5->move(ix,iy);
             m_calRa5->setRange(-9999, 9999);
             m_calRa5->setSingleStep(0.2);
             m_calRa5->setValue(drcalr5);
             m_calRaname5->move(ix+ilab,iy+3);
             m_calRaname5->setText("Ra5");

             m_scene.addWidget(m_calRx5);
             m_scene.addWidget(m_calRxname5);
             m_scene.addWidget(m_calRy5);
             m_scene.addWidget(m_calRyname5);
             m_scene.addWidget(m_calRa5);
             m_scene.addWidget(m_calRaname5);


             ix =180;
             iy=iy-30;
             m_calRx6->move(ix,iy);
             m_calRx6->setRange(-9999, 9999);
             m_calRx6->setSingleStep(0.2);
             m_calRx6->setValue(drcalx6);
             m_calRxname6->move(ix+ilab,iy+3);
             m_calRxname6->setText("Rx6");
             ix=ix+100;
             m_calRy6->move(ix,iy);
             m_calRy6->setRange(-9999, 9999);
             m_calRy6->setSingleStep(0.2);
             m_calRy6->setValue(drcaly6);
             m_calRyname6->move(ix+ilab,iy+3);
             m_calRyname6->setText("Ry6");
             ix=ix+100;
             m_calRa6->move(ix,iy);
             m_calRa6->setRange(-9999, 9999);
             m_calRa6->setSingleStep(0.2);
             m_calRa6->setValue(drcalr6);
             m_calRaname6->move(ix+ilab,iy+3);
             m_calRaname6->setText("Ra6");

             m_scene.addWidget(m_calRx6);
             m_scene.addWidget(m_calRxname6);
             m_scene.addWidget(m_calRy6);
             m_scene.addWidget(m_calRyname6);
             m_scene.addWidget(m_calRa6);
             m_scene.addWidget(m_calRaname6);




         }



         {

             ix =180;
             iy = 178;
             ilab = -15;

             m_calLx1->move(ix,iy);
             m_calLx1->setRange(-9999, 9999);
             m_calLx1->setSingleStep(0.2);
             m_calLx1->setValue(dlcalx1);
             m_calLxname1->move(ix+ilab,iy+3);
             m_calLxname1->setText("Lx1");
             ix=ix+100;
             m_calLy1->move(ix,iy);
             m_calLy1->setRange(-9999, 9999);
             m_calLy1->setSingleStep(0.2);
             m_calLy1->setValue(dlcaly1);
             m_calLyname1->move(ix+ilab,iy+3);
             m_calLyname1->setText("Ly1");
             ix=ix+100;
             m_calLa1->move(ix,iy);
             m_calLa1->setRange(-9999, 9999);
             m_calLa1->setSingleStep(0.2);
             m_calLa1->setValue(dlcalr1);
             m_calLaname1->move(ix+ilab,iy+3);
             m_calLaname1->setText("La1");

             m_scene.addWidget(m_calLx1);
             m_scene.addWidget(m_calLxname1);
             m_scene.addWidget(m_calLy1);
             m_scene.addWidget(m_calLyname1);
             m_scene.addWidget(m_calLa1);
             m_scene.addWidget(m_calLaname1);

             ix =180;
             iy=iy-30;
             m_calLx2->move(ix,iy);
             m_calLx2->setRange(-9999, 9999);
             m_calLx2->setSingleStep(0.2);
             m_calLx2->setValue(dlcalx2);
             m_calLxname2->move(ix+ilab,iy+3);
             m_calLxname2->setText("Lx2");
             ix=ix+100;
             m_calLy2->move(ix,iy);
             m_calLy2->setRange(-9999, 9999);
             m_calLy2->setSingleStep(0.2);
             m_calLy2->setValue(dlcaly2);
             m_calLyname2->move(ix+ilab,iy+3);
             m_calLyname2->setText("Ly2");
             ix=ix+100;
             m_calLa2->move(ix,iy);
             m_calLa2->setRange(-9999, 9999);
             m_calLa2->setSingleStep(0.2);
             m_calLa2->setValue(dlcalr2);
             m_calLaname2->move(ix+ilab,iy+3);
             m_calLaname2->setText("La2");

             m_scene.addWidget(m_calLx2);
             m_scene.addWidget(m_calLxname2);
             m_scene.addWidget(m_calLy2);
             m_scene.addWidget(m_calLyname2);
             m_scene.addWidget(m_calLa2);
             m_scene.addWidget(m_calLaname2);


             ix =180;
             iy=iy-30;
             m_calLx3->move(ix,iy);
             m_calLx3->setRange(-9999, 9999);
             m_calLx3->setSingleStep(0.2);
             m_calLx3->setValue(dlcalx3);
             m_calLxname3->move(ix+ilab,iy+3);
             m_calLxname3->setText("Lx3");
             ix=ix+100;
             m_calLy3->move(ix,iy);
             m_calLy3->setRange(-9999, 9999);
             m_calLy3->setSingleStep(0.2);
             m_calLy3->setValue(dlcaly3);
             m_calLyname3->move(ix+ilab,iy+3);
             m_calLyname3->setText("Ly3");
             ix=ix+100;
             m_calLa3->move(ix,iy);
             m_calLa3->setRange(-9999, 9999);
             m_calLa3->setSingleStep(0.2);
             m_calLa3->setValue(dlcalr3);
             m_calLaname3->move(ix+ilab,iy+3);
             m_calLaname3->setText("La3");

             m_scene.addWidget(m_calLx3);
             m_scene.addWidget(m_calLxname3);
             m_scene.addWidget(m_calLy3);
             m_scene.addWidget(m_calLyname3);
             m_scene.addWidget(m_calLa3);
             m_scene.addWidget(m_calLaname3);



             ix =180;
             iy=iy-30;
             m_calLx4->move(ix,iy);
             m_calLx4->setRange(-9999, 9999);
             m_calLx4->setSingleStep(0.2);
             m_calLx4->setValue(dlcalx4);
             m_calLxname4->move(ix+ilab,iy+3);
             m_calLxname4->setText("Lx4");
             ix=ix+100;
             m_calLy4->move(ix,iy);
             m_calLy4->setRange(-9999, 9999);
             m_calLy4->setSingleStep(0.2);
             m_calLy4->setValue(dlcaly4);
             m_calLyname4->move(ix+ilab,iy+3);
             m_calLyname4->setText("Ly4");
             ix=ix+100;
             m_calLa4->move(ix,iy);
             m_calLa4->setRange(-9999, 9999);
             m_calLa4->setSingleStep(0.2);
             m_calLa4->setValue(dlcalr4);
             m_calLaname4->move(ix+ilab,iy+3);
             m_calLaname4->setText("La4");

             m_scene.addWidget(m_calLx4);
             m_scene.addWidget(m_calLxname4);
             m_scene.addWidget(m_calLy4);
             m_scene.addWidget(m_calLyname4);
             m_scene.addWidget(m_calLa4);
             m_scene.addWidget(m_calLaname4);


             ix =180;
             iy=iy-30;
             m_calLx5->move(ix,iy);
             m_calLx5->setRange(-9999, 9999);
             m_calLx5->setSingleStep(0.2);
             m_calLx5->setValue(dlcalx5);
             m_calLxname5->move(ix+ilab,iy+3);
             m_calLxname5->setText("Lx5");
             ix=ix+100;
             m_calLy5->move(ix,iy);
             m_calLy5->setRange(-9999, 9999);
             m_calLy5->setSingleStep(0.2);
             m_calLy5->setValue(dlcaly5);
             m_calLyname5->move(ix+ilab,iy+3);
             m_calLyname5->setText("Ly5");
             ix=ix+100;
             m_calLa5->move(ix,iy);
             m_calLa5->setRange(-9999, 9999);
             m_calLa5->setSingleStep(0.2);
             m_calLa5->setValue(dlcalr5);
             m_calLaname5->move(ix+ilab,iy+3);
             m_calLaname5->setText("La5");

             m_scene.addWidget(m_calLx5);
             m_scene.addWidget(m_calLxname5);
             m_scene.addWidget(m_calLy5);
             m_scene.addWidget(m_calLyname5);
             m_scene.addWidget(m_calLa5);
             m_scene.addWidget(m_calLaname5);


             ix =180;
             iy=iy-30;
             m_calLx6->move(ix,iy);
             m_calLx6->setRange(-9999, 9999);
             m_calLx6->setSingleStep(0.2);
             m_calLx6->setValue(dlcalx6);
             m_calLxname6->move(ix+ilab,iy+3);
             m_calLxname6->setText("Lx6");
             ix=ix+100;
             m_calLy6->move(ix,iy);
             m_calLy6->setRange(-9999, 9999);
             m_calLy6->setSingleStep(0.2);
             m_calLy6->setValue(dlcaly6);
             m_calLyname6->move(ix+ilab,iy+3);
             m_calLyname6->setText("Ly6");
             ix=ix+100;
             m_calLa6->move(ix,iy);
             m_calLa6->setRange(-9999, 9999);
             m_calLa6->setSingleStep(0.2);
             m_calLa6->setValue(dlcalr6);
             m_calLaname6->move(ix+ilab,iy+3);
             m_calLaname6->setText("La6");

             m_scene.addWidget(m_calLx6);
             m_scene.addWidget(m_calLxname6);
             m_scene.addWidget(m_calLy6);
             m_scene.addWidget(m_calLyname6);
             m_scene.addWidget(m_calLa6);
             m_scene.addWidget(m_calLaname6);




         }







         m_saveButton = new QPushButton(tr("保存"));

         m_saveButton->move(550,430);

         m_scene.addWidget(m_saveButton);


         connect(m_saveButton, &QAbstractButton::clicked, this, &RobotCalibDialog::save);

//! [3]

//! [4]
     setScene(&m_scene);
 //   view.setRenderHint(QPainter::Antialiasing);
    setBackgroundBrush(QPixmap(":/images/rc.png"));
//! [4] //! [5]
  //  view.setCacheMode(QGraphicsView::CacheBackground);
  //  view.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  //  view.setDragMode(QGraphicsView::ScrollHandDrag);
//! [5] //! [6]
  //  view.setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Colliding Mice"));
    resize(640, 480);
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
void RobotCalibDialog::save()
{
    m_pmanager->SetParserValue(QString("d45Rcalx"),m_cal45Rx->value());
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






double drun=m_pmanager->GetParserValue(QString("drunRL"));
     m_pmanager->SetParserValue(QString("drunRL"),0);
    m_pmanager->savecalibfile();

    m_pmanager->SetParserValue(QString("drunRL"),drun);



   //
}
