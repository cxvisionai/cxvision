
QT += multimedia multimediawidgets

INCLUDEPATH  += $$PWD
HEADERS = \
    $$PWD/camera.h \
    $$PWD/imagesettings.h \
    $$PWD/videosettings.h

SOURCES = \
    $$PWD/camera.cpp \
    $$PWD/imagesettings.cpp \
    $$PWD/videosettings.cpp

FORMS += \
    $$PWD/camera.ui \
    $$PWD/videosettings.ui \
    $$PWD/imagesettings.ui

RESOURCES += $$PWD/camera.qrc


# include(../../multimedia/shared/shared.pri)
