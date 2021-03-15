TEMPLATE = app
TARGET = cxvisionaiz
QT +=  serialport testlib widgets multimedia widgets#webenginewidgets
CONFIG += c++11

DEFINES += USE_CAM
#include(editlib\lib.pri)


qtHaveModule(opengl): QT += opengl
#######
#CONFIG += pillow_ssl


#pillow_zlib: DEFINES += PILLOW_ZLIB

#DEFINES += CXVISIONPILLOW
#DEFINES +=CXVISIONIMAGEVIEW
#DEFINES +=CXPYTHON

CONFIG(debug, debug|release) {

unix: LIBS += -LD:\Qt\Qt5.11.0\cxvisionai\pillow-master/lib -lpillowcored
 #     LIBS += -LD:\Qt\Qt5.11.0\cxvisionai\cxvision/cxpython2.7/lib/python2.7cv -llibcxpythonlib
}
CONFIG(release, debug|release) {
unix: LIBS += -LD:\Qt\Qt5.11.0\cxvisionai\pillow-master/lib -lpillowcore

}
#LIBS += -L/usr/local/lib -lX11 -lXtst -lXinerama -lxkbcommon -ldl -lutil
#         -LD:\Qt\Qt5.11.0\cxvisionai\cxpython2.7/lib/python2.7cv/ -llibcxpythonlib
LIBS += -LD:\MVS\Development\Libraries\win32\ -lMvCameraControl
INCLUDEPATH += /usr/local/include \
    D:\Qt\Qt5.11.0\cxvisionaiz\xdotool
INCLUDEPATH +=D:\Qt\Qt5.11.0\cxvisionai\cxpython2.7\Include
INCLUDEPATH += D:\Qt\Qt5.11.0\cxvisionaiz\editlib
INCLUDEPATH += D:\Qt\Qt5.11.0\cxvisionaiz\editlib\document
INCLUDEPATH += D:\Qt\Qt5.11.0\cxvisionaiz\editlib\snippets
INCLUDEPATH += D:\Qt\Qt5.11.0\cxvisionaiz\editlib\widgets
INCLUDEPATH += D:\Qt\Qt5.11.0\cxvisionaiz\editlib\.build
INCLUDEPATH += D:\Qt\Qt5.11.0\cxvisionaiz\editlib\qnfa
INCLUDEPATH += D:\Qt\Qt5.11.0\cxvisionaiz\spectrum\3rdparty\fftreal
INCLUDEPATH += D:\Qt\Qt5.11.0\cxvisionaiz\spectrum
####

INCLUDEPATH +=  D:\Qt\Qt5.11.0\cxvisionaiz \
                D:\Qt\Qt5.11.0\cxvisionaiz\CxGen \
                D:\Qt\Qt5.11.0\cxvisionaiz\pillow-master\pillowcore \
                D:\Qt\Qt5.11.0\cxvisionaiz\cxparser \
                D:\Qt\Qt5.11.0\cxvisionaiz\cxpython \
                D:\Qt\Qt5.11.0\cxvisionaiz\AIFSM \
                D:\Qt\Qt5.11.0\cxvisionaiz\AI \
                D:\Qt\Qt5.11.0\cxvisionaiz\diagram \
                D:\Qt\Qt5.11.0\cxvisionaiz\cam \
                D:\Qt\Qt5.11.0\cxvisionaiz\hk \
                D:\MVS\Development\Includes



HEADERS +=  \
    hk/mainwindow.h \
    hk/CamRun.h \
    hk/MyCamera.h \
    hk/qt_hik.hpp \
    cximage/imagebase.h \
    cximage/CONST_COLOR_ISO_AtanAngle.H \
    cximage/CONST_COLOR_ISO_SqrtX.H \
    cximage/CONST_PointDistancetable.H \
    cximage/ImageParserClass.h \
    cximage/shape.h \
    cximage/shapecommands.h \
    cximage/visionmanager.h \
    cximage/CONST_float_vect.H \
    cximage/shapebase.h \
    cximage/backimagemanager.h \
    cxparser/muParserTreeNode.h \
    cxparser/muParserTokenReader.h \
    cxparser/muParserToken.h \
    cxparser/muParserTest.h \
    cxparser/muParserStrClassMap.h \
    cxparser/muParserStack.h \
    cxparser/muParserRun.h \
    cxparser/muParserInt.h \
    cxparser/muParserFixes.h \
    cxparser/muParserError.h \
    cxparser/muParserDLL.h \
    cxparser/muParserDef.h \
    cxparser/muparserClassReader.h \
    cxparser/muParserClassFunctionReader.h \
    cxparser/muParserClass.h \
    cxparser/muParserCallback.h \
    cxparser/muParserBytecode.h \
    cxparser/muParserBase.h \
    cxparser/muParser.h \
    cxparser/LeakWatcher.h \
    cxpython/cxpythontest.h \
    cxpython/ffpython.h \
    p2p/dialog.h \
    p2p/connection.h \
    p2p/connection_serial.h \
    p2p/connection_msg.h \
    p2p/settingsdialog.h \
    p2p/dialog.h \
    p2p/codeeditor.h \
    p2p/codeeditor.h \
    download/textprogressbar.h \
    download/downloadmanager.h \
    AIFSM/AIControl.h \
    AIFSM/Control.h \
    AIFSM/FStateApproach.h \
    AIFSM/FuSM.h \
    AIFSM/FuSMAIControl.h \
    AIFSM/FuSMMachine.h \
    AIFSM/FuSMState.h \
    AIFSM/AIControl.h \
    AI/stringai.h \
    diagram/arrow.h \
    diagram/diagramitem.h \
    diagram/diagramscene.h \
    diagram/diagramtextitem.h \
    diagram/diagramtab.h \
    diagram/arrowi.h \
    p2p/norwegianwoodstyle.h \
    p2p/CRC/crc32.c.h \
    p2p/CRC/zconf.h \
    p2p/CRC/zutil.h \
    diagram/diagrammainwindow.h





SOURCES += \
    hk/mainwindow.cpp \
    hk/CamRun.cpp \
    hk/qt_hik.cpp \
    hk/MyCamera.cpp \
    cximage/imagebase.cpp \
    cximage/ImageParserClass.cpp \
    cximage/shape.cpp \
    cximage/shapecommands.cpp \
    cximage/visionmanager.cpp \
    cximage/shapebase.cpp \
    cximage/backimagemanager.cpp \
    main.cpp \
    p2p/dialog.cpp \
    p2p/connection.cpp \
    p2p/connection_serial.cpp \
    p2p/connection_msg.cpp \
    p2p/settingsdialog.cpp \
    p2p/codeeditor.cpp \
    cxparser/muParserTreeNode.cpp \
    cxparser/muparsertokenreader.cpp \
    cxparser/muparsertest.cpp \
    cxparser/muParserStrClassMap.cpp \
    cxparser/muParserRun.cpp \
    cxparser/muParserInt.cpp \
    cxparser/muParserError.cpp \
    cxparser/muparserClassReader.cpp \
    cxparser/muParserClassFunctionReader.cpp \
    cxparser/muParserClass.cpp \
    cxparser/muParserCallback.cpp \
    cxparser/muParserBytecode.cpp \
    cxparser/muParserBase.cpp \
    cxparser/muParser.cpp \
    download/textprogressbar.cpp \
    download/downloadmanager.cpp \
    AIFSM/FStateApproach.cpp \
    AIFSM/FuSMAIControl.cpp \
    AIFSM/FuSMMachine.cpp \
    AIFSM/FuSMState.cpp \
    AIFSM/AIControl.cpp \
    AI/stringai.cpp \
    diagram/arrow.cpp \
    diagram/diagramitem.cpp \
    diagram/diagramscene.cpp \
    diagram/diagramtextitem.cpp \
    diagram/diagramtab.cpp \
    diagram/arrowi.cpp \
    p2p/norwegianwoodstyle.cpp \
    diagram/diagrammainwindow.cpp

FORMS += \
    hk/mainwindow.ui \
    certificateerrordialog.ui \
    passworddialog.ui \
    downloadmanagerwidget.ui \
    downloadwidget.ui \
    p2p/settingsdialog.ui

RESOURCES += data/simplebrowser.qrc
RESOURCES += jquery.qrc
RESOURCES += diagramscene.qrc
RESOURCES += spectrum/spectrum.qrc

#LIBS += -LD:\Qt\Qt5.11.0\cxvisionai\cxvision/lib
#LIBS += -lfftreal

# install
target.path = D:\Qt\Qt5.11.0\cxvisionaiz
INSTALLS += target

DISTFILES += \
    ../pillow-master/config.pri

SUBDIRS += \
    download/downloadmanager.pro
