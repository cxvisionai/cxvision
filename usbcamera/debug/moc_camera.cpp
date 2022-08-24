/****************************************************************************
** Meta object code from reading C++ file 'camera.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../camera.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'camera.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Camera_t {
    QByteArrayData data[46];
    char stringdata0[645];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Camera_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Camera_t qt_meta_stringdata_Camera = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Camera"
QT_MOC_LITERAL(1, 7, 9), // "setCamera"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 11), // "QCameraInfo"
QT_MOC_LITERAL(4, 30, 10), // "cameraInfo"
QT_MOC_LITERAL(5, 41, 11), // "startCamera"
QT_MOC_LITERAL(6, 53, 10), // "stopCamera"
QT_MOC_LITERAL(7, 64, 6), // "record"
QT_MOC_LITERAL(8, 71, 5), // "pause"
QT_MOC_LITERAL(9, 77, 4), // "stop"
QT_MOC_LITERAL(10, 82, 8), // "setMuted"
QT_MOC_LITERAL(11, 91, 10), // "toggleLock"
QT_MOC_LITERAL(12, 102, 9), // "takeImage"
QT_MOC_LITERAL(13, 112, 19), // "displayCaptureError"
QT_MOC_LITERAL(14, 132, 26), // "QCameraImageCapture::Error"
QT_MOC_LITERAL(15, 159, 11), // "errorString"
QT_MOC_LITERAL(16, 171, 24), // "configureCaptureSettings"
QT_MOC_LITERAL(17, 196, 22), // "configureVideoSettings"
QT_MOC_LITERAL(18, 219, 22), // "configureImageSettings"
QT_MOC_LITERAL(19, 242, 20), // "displayRecorderError"
QT_MOC_LITERAL(20, 263, 18), // "displayCameraError"
QT_MOC_LITERAL(21, 282, 18), // "updateCameraDevice"
QT_MOC_LITERAL(22, 301, 8), // "QAction*"
QT_MOC_LITERAL(23, 310, 6), // "action"
QT_MOC_LITERAL(24, 317, 17), // "updateCameraState"
QT_MOC_LITERAL(25, 335, 14), // "QCamera::State"
QT_MOC_LITERAL(26, 350, 17), // "updateCaptureMode"
QT_MOC_LITERAL(27, 368, 19), // "updateRecorderState"
QT_MOC_LITERAL(28, 388, 21), // "QMediaRecorder::State"
QT_MOC_LITERAL(29, 410, 5), // "state"
QT_MOC_LITERAL(30, 416, 23), // "setExposureCompensation"
QT_MOC_LITERAL(31, 440, 5), // "index"
QT_MOC_LITERAL(32, 446, 16), // "updateRecordTime"
QT_MOC_LITERAL(33, 463, 20), // "processCapturedImage"
QT_MOC_LITERAL(34, 484, 9), // "requestId"
QT_MOC_LITERAL(35, 494, 3), // "img"
QT_MOC_LITERAL(36, 498, 16), // "updateLockStatus"
QT_MOC_LITERAL(37, 515, 19), // "QCamera::LockStatus"
QT_MOC_LITERAL(38, 535, 25), // "QCamera::LockChangeReason"
QT_MOC_LITERAL(39, 561, 17), // "displayViewfinder"
QT_MOC_LITERAL(40, 579, 20), // "displayCapturedImage"
QT_MOC_LITERAL(41, 600, 15), // "readyForCapture"
QT_MOC_LITERAL(42, 616, 5), // "ready"
QT_MOC_LITERAL(43, 622, 10), // "imageSaved"
QT_MOC_LITERAL(44, 633, 2), // "id"
QT_MOC_LITERAL(45, 636, 8) // "fileName"

    },
    "Camera\0setCamera\0\0QCameraInfo\0cameraInfo\0"
    "startCamera\0stopCamera\0record\0pause\0"
    "stop\0setMuted\0toggleLock\0takeImage\0"
    "displayCaptureError\0QCameraImageCapture::Error\0"
    "errorString\0configureCaptureSettings\0"
    "configureVideoSettings\0configureImageSettings\0"
    "displayRecorderError\0displayCameraError\0"
    "updateCameraDevice\0QAction*\0action\0"
    "updateCameraState\0QCamera::State\0"
    "updateCaptureMode\0updateRecorderState\0"
    "QMediaRecorder::State\0state\0"
    "setExposureCompensation\0index\0"
    "updateRecordTime\0processCapturedImage\0"
    "requestId\0img\0updateLockStatus\0"
    "QCamera::LockStatus\0QCamera::LockChangeReason\0"
    "displayViewfinder\0displayCapturedImage\0"
    "readyForCapture\0ready\0imageSaved\0id\0"
    "fileName"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Camera[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      27,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,  149,    2, 0x08 /* Private */,
       5,    0,  152,    2, 0x08 /* Private */,
       6,    0,  153,    2, 0x08 /* Private */,
       7,    0,  154,    2, 0x08 /* Private */,
       8,    0,  155,    2, 0x08 /* Private */,
       9,    0,  156,    2, 0x08 /* Private */,
      10,    1,  157,    2, 0x08 /* Private */,
      11,    0,  160,    2, 0x08 /* Private */,
      12,    0,  161,    2, 0x08 /* Private */,
      13,    3,  162,    2, 0x08 /* Private */,
      16,    0,  169,    2, 0x08 /* Private */,
      17,    0,  170,    2, 0x08 /* Private */,
      18,    0,  171,    2, 0x08 /* Private */,
      19,    0,  172,    2, 0x08 /* Private */,
      20,    0,  173,    2, 0x08 /* Private */,
      21,    1,  174,    2, 0x08 /* Private */,
      24,    1,  177,    2, 0x08 /* Private */,
      26,    0,  180,    2, 0x08 /* Private */,
      27,    1,  181,    2, 0x08 /* Private */,
      30,    1,  184,    2, 0x08 /* Private */,
      32,    0,  187,    2, 0x08 /* Private */,
      33,    2,  188,    2, 0x08 /* Private */,
      36,    2,  193,    2, 0x08 /* Private */,
      39,    0,  198,    2, 0x08 /* Private */,
      40,    0,  199,    2, 0x08 /* Private */,
      41,    1,  200,    2, 0x08 /* Private */,
      43,    2,  203,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 14, QMetaType::QString,    2,    2,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void, 0x80000000 | 25,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 28,   29,
    QMetaType::Void, QMetaType::Int,   31,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QImage,   34,   35,
    QMetaType::Void, 0x80000000 | 37, 0x80000000 | 38,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   42,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   44,   45,

       0        // eod
};

void Camera::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Camera *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setCamera((*reinterpret_cast< const QCameraInfo(*)>(_a[1]))); break;
        case 1: _t->startCamera(); break;
        case 2: _t->stopCamera(); break;
        case 3: _t->record(); break;
        case 4: _t->pause(); break;
        case 5: _t->stop(); break;
        case 6: _t->setMuted((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->toggleLock(); break;
        case 8: _t->takeImage(); break;
        case 9: _t->displayCaptureError((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QCameraImageCapture::Error(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 10: _t->configureCaptureSettings(); break;
        case 11: _t->configureVideoSettings(); break;
        case 12: _t->configureImageSettings(); break;
        case 13: _t->displayRecorderError(); break;
        case 14: _t->displayCameraError(); break;
        case 15: _t->updateCameraDevice((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 16: _t->updateCameraState((*reinterpret_cast< QCamera::State(*)>(_a[1]))); break;
        case 17: _t->updateCaptureMode(); break;
        case 18: _t->updateRecorderState((*reinterpret_cast< QMediaRecorder::State(*)>(_a[1]))); break;
        case 19: _t->setExposureCompensation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->updateRecordTime(); break;
        case 21: _t->processCapturedImage((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QImage(*)>(_a[2]))); break;
        case 22: _t->updateLockStatus((*reinterpret_cast< QCamera::LockStatus(*)>(_a[1])),(*reinterpret_cast< QCamera::LockChangeReason(*)>(_a[2]))); break;
        case 23: _t->displayViewfinder(); break;
        case 24: _t->displayCapturedImage(); break;
        case 25: _t->readyForCapture((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 26: _t->imageSaved((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCameraImageCapture::Error >(); break;
            }
            break;
        case 16:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCamera::State >(); break;
            }
            break;
        case 18:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMediaRecorder::State >(); break;
            }
            break;
        case 22:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCamera::LockChangeReason >(); break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCamera::LockStatus >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Camera::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_Camera.data,
    qt_meta_data_Camera,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Camera::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Camera::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Camera.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int Camera::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 27)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 27;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 27)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 27;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
