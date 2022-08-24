/********************************************************************************
** Form generated from reading UI file 'videosettings.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEOSETTINGS_H
#define UI_VIDEOSETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VideoSettingsUi
{
public:
    QGridLayout *gridLayout_4;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QComboBox *audioCodecBox;
    QLabel *label_5;
    QComboBox *audioSampleRateBox;
    QLabel *label_3;
    QSlider *audioQualitySlider;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QLabel *label_8;
    QComboBox *videoResolutionBox;
    QLabel *label_9;
    QComboBox *videoFramerateBox;
    QLabel *label_6;
    QComboBox *videoCodecBox;
    QLabel *label_7;
    QSlider *videoQualitySlider;
    QLabel *label_4;
    QComboBox *containerFormatBox;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *VideoSettingsUi)
    {
        if (VideoSettingsUi->objectName().isEmpty())
            VideoSettingsUi->setObjectName(QString::fromUtf8("VideoSettingsUi"));
        VideoSettingsUi->resize(561, 369);
        gridLayout_4 = new QGridLayout(VideoSettingsUi);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        scrollArea = new QScrollArea(VideoSettingsUi);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 543, 250));
        gridLayout_3 = new QGridLayout(scrollAreaWidgetContents);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        groupBox = new QGroupBox(scrollAreaWidgetContents);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 2);

        audioCodecBox = new QComboBox(groupBox);
        audioCodecBox->setObjectName(QString::fromUtf8("audioCodecBox"));

        gridLayout->addWidget(audioCodecBox, 1, 0, 1, 2);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 2, 0, 1, 2);

        audioSampleRateBox = new QComboBox(groupBox);
        audioSampleRateBox->setObjectName(QString::fromUtf8("audioSampleRateBox"));

        gridLayout->addWidget(audioSampleRateBox, 3, 0, 1, 2);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 4, 0, 1, 1);

        audioQualitySlider = new QSlider(groupBox);
        audioQualitySlider->setObjectName(QString::fromUtf8("audioQualitySlider"));
        audioQualitySlider->setMaximum(4);
        audioQualitySlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(audioQualitySlider, 4, 1, 1, 1);


        gridLayout_3->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(scrollAreaWidgetContents);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_2->addWidget(label_8, 0, 0, 1, 2);

        videoResolutionBox = new QComboBox(groupBox_2);
        videoResolutionBox->setObjectName(QString::fromUtf8("videoResolutionBox"));

        gridLayout_2->addWidget(videoResolutionBox, 1, 0, 1, 2);

        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_2->addWidget(label_9, 2, 0, 1, 2);

        videoFramerateBox = new QComboBox(groupBox_2);
        videoFramerateBox->setObjectName(QString::fromUtf8("videoFramerateBox"));

        gridLayout_2->addWidget(videoFramerateBox, 3, 0, 1, 2);

        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 4, 0, 1, 2);

        videoCodecBox = new QComboBox(groupBox_2);
        videoCodecBox->setObjectName(QString::fromUtf8("videoCodecBox"));

        gridLayout_2->addWidget(videoCodecBox, 5, 0, 1, 2);

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 6, 0, 1, 1);

        videoQualitySlider = new QSlider(groupBox_2);
        videoQualitySlider->setObjectName(QString::fromUtf8("videoQualitySlider"));
        videoQualitySlider->setMaximum(4);
        videoQualitySlider->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(videoQualitySlider, 6, 1, 1, 1);


        gridLayout_3->addWidget(groupBox_2, 0, 1, 3, 1);

        label_4 = new QLabel(scrollAreaWidgetContents);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_3->addWidget(label_4, 1, 0, 1, 1);

        containerFormatBox = new QComboBox(scrollAreaWidgetContents);
        containerFormatBox->setObjectName(QString::fromUtf8("containerFormatBox"));

        gridLayout_3->addWidget(containerFormatBox, 2, 0, 1, 1);

        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout_4->addWidget(scrollArea, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 14, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer, 1, 0, 1, 1);

        buttonBox = new QDialogButtonBox(VideoSettingsUi);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout_4->addWidget(buttonBox, 2, 0, 1, 1);


        retranslateUi(VideoSettingsUi);
        QObject::connect(buttonBox, SIGNAL(accepted()), VideoSettingsUi, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), VideoSettingsUi, SLOT(reject()));

        QMetaObject::connectSlotsByName(VideoSettingsUi);
    } // setupUi

    void retranslateUi(QDialog *VideoSettingsUi)
    {
        VideoSettingsUi->setWindowTitle(QCoreApplication::translate("VideoSettingsUi", "Video Settings", nullptr));
        groupBox->setTitle(QCoreApplication::translate("VideoSettingsUi", "Audio", nullptr));
        label_2->setText(QCoreApplication::translate("VideoSettingsUi", "Audio Codec:", nullptr));
        label_5->setText(QCoreApplication::translate("VideoSettingsUi", "Sample Rate:", nullptr));
        label_3->setText(QCoreApplication::translate("VideoSettingsUi", "Quality:", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("VideoSettingsUi", "Video", nullptr));
        label_8->setText(QCoreApplication::translate("VideoSettingsUi", "Resolution:", nullptr));
        label_9->setText(QCoreApplication::translate("VideoSettingsUi", "Framerate:", nullptr));
        label_6->setText(QCoreApplication::translate("VideoSettingsUi", "Video Codec:", nullptr));
        label_7->setText(QCoreApplication::translate("VideoSettingsUi", "Quality:", nullptr));
        label_4->setText(QCoreApplication::translate("VideoSettingsUi", "Container Format:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VideoSettingsUi: public Ui_VideoSettingsUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEOSETTINGS_H
