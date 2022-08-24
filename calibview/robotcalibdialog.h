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

#ifndef CALIBDIALOG_H
#define CALIBDIALOG_H

#include <QGraphicsView>
#include <QGraphicsScene>
QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QLabel;
class QDoubleSpinBox;
class QPushButton;
class visionmanager;
QT_END_NAMESPACE

class RobotCalibDialog : public QGraphicsView
{
    Q_OBJECT

public:
    RobotCalibDialog(visionmanager *pmanager = nullptr,QGraphicsView *pview = nullptr);

public slots:
    void save();

private:
    QDoubleSpinBox *m_cal45Rx;
    QLabel *m_cal45Rxname ;
    QDoubleSpinBox *m_cal45Ry;
    QLabel *m_cal45Ryname ;
    QDoubleSpinBox *m_cal45Ra;
    QLabel *m_cal45Raname ;

    QDoubleSpinBox *m_cal45Lx;
    QLabel *m_cal45Lxname ;
    QDoubleSpinBox *m_cal45Ly;
    QLabel *m_cal45Lyname ;
    QDoubleSpinBox *m_cal45La;
    QLabel *m_cal45Laname ;

    QDoubleSpinBox *m_cal89Rx;
    QLabel *m_cal89Rxname ;
    QDoubleSpinBox *m_cal89Ry;
    QLabel *m_cal89Ryname ;
    QDoubleSpinBox *m_cal89Ra;
    QLabel *m_cal89Raname ;

    QDoubleSpinBox *m_cal89Lx;
    QLabel *m_cal89Lxname ;
    QDoubleSpinBox *m_cal89Ly;
    QLabel *m_cal89Lyname ;
    QDoubleSpinBox *m_cal89La;
    QLabel *m_cal89Laname ;



    QDoubleSpinBox *m_calLx1;
    QLabel *m_calLxname1 ;
    QDoubleSpinBox *m_calLy1;
    QLabel *m_calLyname1 ;
    QDoubleSpinBox *m_calLa1;
    QLabel *m_calLaname1 ;

    QDoubleSpinBox *m_calLx2;
    QLabel *m_calLxname2 ;
    QDoubleSpinBox *m_calLy2;
    QLabel *m_calLyname2 ;
    QDoubleSpinBox *m_calLa2;
    QLabel *m_calLaname2 ;

    QDoubleSpinBox *m_calLx3;
    QLabel *m_calLxname3 ;
    QDoubleSpinBox *m_calLy3;
    QLabel *m_calLyname3 ;
    QDoubleSpinBox *m_calLa3;
    QLabel *m_calLaname3 ;

    QDoubleSpinBox *m_calLx4;
    QLabel *m_calLxname4 ;
    QDoubleSpinBox *m_calLy4;
    QLabel *m_calLyname4 ;
    QDoubleSpinBox *m_calLa4;
    QLabel *m_calLaname4 ;

    QDoubleSpinBox *m_calLx5;
    QLabel *m_calLxname5 ;
    QDoubleSpinBox *m_calLy5;
    QLabel *m_calLyname5 ;
    QDoubleSpinBox *m_calLa5;
    QLabel *m_calLaname5 ;

    QDoubleSpinBox *m_calLx6;
    QLabel *m_calLxname6 ;
    QDoubleSpinBox *m_calLy6;
    QLabel *m_calLyname6 ;
    QDoubleSpinBox *m_calLa6;
    QLabel *m_calLaname6 ;


    QDoubleSpinBox *m_calRx1;
    QLabel *m_calRxname1 ;
    QDoubleSpinBox *m_calRy1;
    QLabel *m_calRyname1 ;
    QDoubleSpinBox *m_calRa1;
    QLabel *m_calRaname1 ;

    QDoubleSpinBox *m_calRx2;
    QLabel *m_calRxname2 ;
    QDoubleSpinBox *m_calRy2;
    QLabel *m_calRyname2 ;
    QDoubleSpinBox *m_calRa2;
    QLabel *m_calRaname2 ;

    QDoubleSpinBox *m_calRx3;
    QLabel *m_calRxname3 ;
    QDoubleSpinBox *m_calRy3;
    QLabel *m_calRyname3 ;
    QDoubleSpinBox *m_calRa3;
    QLabel *m_calRaname3 ;

    QDoubleSpinBox *m_calRx4;
    QLabel *m_calRxname4 ;
    QDoubleSpinBox *m_calRy4;
    QLabel *m_calRyname4 ;
    QDoubleSpinBox *m_calRa4;
    QLabel *m_calRaname4 ;

    QDoubleSpinBox *m_calRx5;
    QLabel *m_calRxname5 ;
    QDoubleSpinBox *m_calRy5;
    QLabel *m_calRyname5 ;
    QDoubleSpinBox *m_calRa5;
    QLabel *m_calRaname5 ;

    QDoubleSpinBox *m_calRx6;
    QLabel *m_calRxname6 ;
    QDoubleSpinBox *m_calRy6;
    QLabel *m_calRyname6 ;
    QDoubleSpinBox *m_calRa6;
    QLabel *m_calRaname6 ;




    QPushButton *m_saveButton = nullptr;
    QPushButton *quitButton = nullptr;
    QDialogButtonBox *buttonBox = nullptr;

    QGraphicsScene m_scene;
    visionmanager *m_pmanager;

};

#endif
