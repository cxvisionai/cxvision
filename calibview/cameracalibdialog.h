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

#ifndef CAMERACALIBDIALOG_H
#define CAMERACALIBDIALOG_H

#include <QGraphicsView>
#include <QGraphicsScene>
QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QLabel;
class QDoubleSpinBox;
class QPushButton;
class visionmanager;
QT_END_NAMESPACE

class CameraCalibDialog : public QGraphicsView
{
    Q_OBJECT

public:
    CameraCalibDialog(visionmanager *pmanager = nullptr,QGraphicsView *pview = nullptr);

public slots:
    void save();
    void calpos2();
    void calpos3();
    void calpos6();
    void calpos7();
    void calpos4L();
    void calpos5L();
    void calpos4R();
    void calpos5R();
    void calpos9L();
    void calpos8L();
    void calpos9R();
    void calpos8R();

private:
    QDoubleSpinBox *m_cal4Rx;
    QLabel *m_cal4Rxname ;
    QDoubleSpinBox *m_cal4Ry;
    QLabel *m_cal4Ryname ;


    QDoubleSpinBox *m_cal4Lx;
    QLabel *m_cal4Lxname ;
    QDoubleSpinBox *m_cal4Ly;
    QLabel *m_cal4Lyname ;


    QDoubleSpinBox *m_cal5Rx;
    QLabel *m_cal5Rxname ;
    QDoubleSpinBox *m_cal5Ry;
    QLabel *m_cal5Ryname ;

    QDoubleSpinBox *m_cal5Lx;
    QLabel *m_cal5Lxname ;
    QDoubleSpinBox *m_cal5Ly;
    QLabel *m_cal5Lyname ;


    QDoubleSpinBox *m_cal8Rx;
    QLabel *m_cal8Rxname ;
    QDoubleSpinBox *m_cal8Ry;
    QLabel *m_cal8Ryname ;

    QDoubleSpinBox *m_cal8Lx;
    QLabel *m_cal8Lxname ;
    QDoubleSpinBox *m_cal8Ly;
    QLabel *m_cal8Lyname ;


    QDoubleSpinBox *m_cal9Rx;
    QLabel *m_cal9Rxname ;
    QDoubleSpinBox *m_cal9Ry;
    QLabel *m_cal9Ryname ;

    QDoubleSpinBox *m_cal9Lx;
    QLabel *m_cal9Lxname ;
    QDoubleSpinBox *m_cal9Ly;
    QLabel *m_cal9Lyname ;


    QDoubleSpinBox *m_cal2x;
    QLabel *m_cal2xname ;
    QDoubleSpinBox *m_cal2y;
    QLabel *m_cal2yname ;

    QDoubleSpinBox *m_cal3x;
    QLabel *m_cal3xname ;
    QDoubleSpinBox *m_cal3y;
    QLabel *m_cal3yname ;

    QDoubleSpinBox *m_cal6x;
    QLabel *m_cal6xname ;
    QDoubleSpinBox *m_cal6y;
    QLabel *m_cal6yname ;


    QDoubleSpinBox *m_cal7x;
    QLabel *m_cal7xname ;
    QDoubleSpinBox *m_cal7y;
    QLabel *m_cal7yname ;


    QLabel *m_resultlabel = nullptr;


    QPushButton *m_saveButton = nullptr;

    QPushButton *m_cal2Button = nullptr;
    QPushButton *m_cal3Button = nullptr;

    QPushButton *m_cal6Button = nullptr;
    QPushButton *m_cal7Button = nullptr;

    QPushButton *m_cal4LButton = nullptr;
    QPushButton *m_cal4RButton = nullptr;

    QPushButton *m_cal5LButton = nullptr;
    QPushButton *m_cal5RButton = nullptr;

    QPushButton *m_cal8LButton = nullptr;
    QPushButton *m_cal8RButton = nullptr;

    QPushButton *m_cal9LButton = nullptr;
    QPushButton *m_cal9RButton = nullptr;






    QPushButton *quitButton = nullptr;
    QDialogButtonBox *buttonBox = nullptr;

    QGraphicsScene m_scene;
    visionmanager *m_pmanager;

};

#endif
