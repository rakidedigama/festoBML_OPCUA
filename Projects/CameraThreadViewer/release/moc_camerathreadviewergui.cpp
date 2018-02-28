/****************************************************************************
** Meta object code from reading C++ file 'camerathreadviewergui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../camerathreadviewergui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'camerathreadviewergui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ClickableLabel_t {
    QByteArrayData data[3];
    char stringdata0[30];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ClickableLabel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ClickableLabel_t qt_meta_stringdata_ClickableLabel = {
    {
QT_MOC_LITERAL(0, 0, 14), // "ClickableLabel"
QT_MOC_LITERAL(1, 15, 13), // "doubleClicked"
QT_MOC_LITERAL(2, 29, 0) // ""

    },
    "ClickableLabel\0doubleClicked\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ClickableLabel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void ClickableLabel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ClickableLabel *_t = static_cast<ClickableLabel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->doubleClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ClickableLabel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ClickableLabel::doubleClicked)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject ClickableLabel::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_ClickableLabel.data,
      qt_meta_data_ClickableLabel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ClickableLabel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ClickableLabel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ClickableLabel.stringdata0))
        return static_cast<void*>(const_cast< ClickableLabel*>(this));
    return QLabel::qt_metacast(_clname);
}

int ClickableLabel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ClickableLabel::doubleClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_CameraThreadViewerGUI_t {
    QByteArrayData data[40];
    char stringdata0[570];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CameraThreadViewerGUI_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CameraThreadViewerGUI_t qt_meta_stringdata_CameraThreadViewerGUI = {
    {
QT_MOC_LITERAL(0, 0, 21), // "CameraThreadViewerGUI"
QT_MOC_LITERAL(1, 22, 13), // "doLightsCalib"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 17), // "cancelLightsCalib"
QT_MOC_LITERAL(4, 55, 17), // "updateImageViewer"
QT_MOC_LITERAL(5, 73, 11), // "std::string"
QT_MOC_LITERAL(6, 85, 18), // "startButtonPressed"
QT_MOC_LITERAL(7, 104, 17), // "stopButtonPressed"
QT_MOC_LITERAL(8, 122, 18), // "pauseButtonPressed"
QT_MOC_LITERAL(9, 141, 15), // "rawRadioChecked"
QT_MOC_LITERAL(10, 157, 15), // "rGBRadioChecked"
QT_MOC_LITERAL(11, 173, 13), // "rRadioChecked"
QT_MOC_LITERAL(12, 187, 13), // "gRadioChecked"
QT_MOC_LITERAL(13, 201, 13), // "bRadioChecked"
QT_MOC_LITERAL(14, 215, 9), // "FPSUpdate"
QT_MOC_LITERAL(15, 225, 16), // "selectSaveFolder"
QT_MOC_LITERAL(16, 242, 9), // "startSave"
QT_MOC_LITERAL(17, 252, 7), // "endSave"
QT_MOC_LITERAL(18, 260, 11), // "blinkSaving"
QT_MOC_LITERAL(19, 272, 11), // "saveCounter"
QT_MOC_LITERAL(20, 284, 7), // "snapMsg"
QT_MOC_LITERAL(21, 292, 11), // "snapPressed"
QT_MOC_LITERAL(22, 304, 21), // "saveFolderDoubleClick"
QT_MOC_LITERAL(23, 326, 17), // "saveFolderTimeout"
QT_MOC_LITERAL(24, 344, 25), // "saveFolderLineEditChanged"
QT_MOC_LITERAL(25, 370, 16), // "focusTestPressed"
QT_MOC_LITERAL(26, 387, 10), // "focusingOn"
QT_MOC_LITERAL(27, 398, 9), // "bestFocus"
QT_MOC_LITERAL(28, 408, 9), // "imageSize"
QT_MOC_LITERAL(29, 418, 7), // "darkImg"
QT_MOC_LITERAL(30, 426, 5), // "error"
QT_MOC_LITERAL(31, 432, 18), // "lightsCalibPressed"
QT_MOC_LITERAL(32, 451, 13), // "lightsCalibOn"
QT_MOC_LITERAL(33, 465, 19), // "pbLightsTestPressed"
QT_MOC_LITERAL(34, 485, 12), // "doTimedFocus"
QT_MOC_LITERAL(35, 498, 10), // "newMessage"
QT_MOC_LITERAL(36, 509, 15), // "newTwincatValue"
QT_MOC_LITERAL(37, 525, 16), // "gardaPageRequest"
QT_MOC_LITERAL(38, 542, 17), // "gardaPageDownload"
QT_MOC_LITERAL(39, 560, 9) // "zAxisMove"

    },
    "CameraThreadViewerGUI\0doLightsCalib\0"
    "\0cancelLightsCalib\0updateImageViewer\0"
    "std::string\0startButtonPressed\0"
    "stopButtonPressed\0pauseButtonPressed\0"
    "rawRadioChecked\0rGBRadioChecked\0"
    "rRadioChecked\0gRadioChecked\0bRadioChecked\0"
    "FPSUpdate\0selectSaveFolder\0startSave\0"
    "endSave\0blinkSaving\0saveCounter\0snapMsg\0"
    "snapPressed\0saveFolderDoubleClick\0"
    "saveFolderTimeout\0saveFolderLineEditChanged\0"
    "focusTestPressed\0focusingOn\0bestFocus\0"
    "imageSize\0darkImg\0error\0lightsCalibPressed\0"
    "lightsCalibOn\0pbLightsTestPressed\0"
    "doTimedFocus\0newMessage\0newTwincatValue\0"
    "gardaPageRequest\0gardaPageDownload\0"
    "zAxisMove"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CameraThreadViewerGUI[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      37,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  199,    2, 0x06 /* Public */,
       3,    0,  202,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,  203,    2, 0x0a /* Public */,
       6,    0,  206,    2, 0x0a /* Public */,
       7,    0,  207,    2, 0x0a /* Public */,
       8,    0,  208,    2, 0x0a /* Public */,
       9,    1,  209,    2, 0x0a /* Public */,
      10,    1,  212,    2, 0x0a /* Public */,
      11,    1,  215,    2, 0x0a /* Public */,
      12,    1,  218,    2, 0x0a /* Public */,
      13,    1,  221,    2, 0x0a /* Public */,
      14,    0,  224,    2, 0x0a /* Public */,
      15,    0,  225,    2, 0x0a /* Public */,
      16,    0,  226,    2, 0x0a /* Public */,
      17,    0,  227,    2, 0x0a /* Public */,
      18,    0,  228,    2, 0x0a /* Public */,
      19,    0,  229,    2, 0x0a /* Public */,
      20,    1,  230,    2, 0x0a /* Public */,
      21,    0,  233,    2, 0x0a /* Public */,
      22,    0,  234,    2, 0x08 /* Private */,
      23,    0,  235,    2, 0x08 /* Private */,
      24,    1,  236,    2, 0x08 /* Private */,
      25,    0,  239,    2, 0x08 /* Private */,
      26,    1,  240,    2, 0x08 /* Private */,
      27,    1,  243,    2, 0x08 /* Private */,
      28,    3,  246,    2, 0x08 /* Private */,
      29,    0,  253,    2, 0x08 /* Private */,
      30,    1,  254,    2, 0x08 /* Private */,
      31,    0,  257,    2, 0x08 /* Private */,
      32,    1,  258,    2, 0x08 /* Private */,
      33,    0,  261,    2, 0x08 /* Private */,
      34,    0,  262,    2, 0x08 /* Private */,
      35,    2,  263,    2, 0x08 /* Private */,
      36,    2,  268,    2, 0x08 /* Private */,
      37,    0,  273,    2, 0x08 /* Private */,
      38,    0,  274,    2, 0x08 /* Private */,
      39,    0,  275,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::UInt,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::UInt,    2,
    QMetaType::Void, QMetaType::UInt, QMetaType::UInt, QMetaType::QString,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QByteArray,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::Short,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CameraThreadViewerGUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CameraThreadViewerGUI *_t = static_cast<CameraThreadViewerGUI *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->doLightsCalib((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 1: _t->cancelLightsCalib(); break;
        case 2: _t->updateImageViewer((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        case 3: _t->startButtonPressed(); break;
        case 4: _t->stopButtonPressed(); break;
        case 5: _t->pauseButtonPressed(); break;
        case 6: _t->rawRadioChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->rGBRadioChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->rRadioChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->gRadioChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->bRadioChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->FPSUpdate(); break;
        case 12: _t->selectSaveFolder(); break;
        case 13: _t->startSave(); break;
        case 14: _t->endSave(); break;
        case 15: _t->blinkSaving(); break;
        case 16: _t->saveCounter(); break;
        case 17: _t->snapMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 18: _t->snapPressed(); break;
        case 19: _t->saveFolderDoubleClick(); break;
        case 20: _t->saveFolderTimeout(); break;
        case 21: _t->saveFolderLineEditChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 22: _t->focusTestPressed(); break;
        case 23: _t->focusingOn((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 24: _t->bestFocus((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 25: _t->imageSize((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 26: _t->darkImg(); break;
        case 27: _t->error((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 28: _t->lightsCalibPressed(); break;
        case 29: _t->lightsCalibOn((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 30: _t->pbLightsTestPressed(); break;
        case 31: _t->doTimedFocus(); break;
        case 32: _t->newMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 33: _t->newTwincatValue((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< qint16(*)>(_a[2]))); break;
        case 34: _t->gardaPageRequest(); break;
        case 35: _t->gardaPageDownload(); break;
        case 36: _t->zAxisMove(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CameraThreadViewerGUI::*_t)(unsigned  );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CameraThreadViewerGUI::doLightsCalib)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CameraThreadViewerGUI::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CameraThreadViewerGUI::cancelLightsCalib)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject CameraThreadViewerGUI::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CameraThreadViewerGUI.data,
      qt_meta_data_CameraThreadViewerGUI,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CameraThreadViewerGUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CameraThreadViewerGUI::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CameraThreadViewerGUI.stringdata0))
        return static_cast<void*>(const_cast< CameraThreadViewerGUI*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int CameraThreadViewerGUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 37)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 37;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 37)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 37;
    }
    return _id;
}

// SIGNAL 0
void CameraThreadViewerGUI::doLightsCalib(unsigned  _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CameraThreadViewerGUI::cancelLightsCalib()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
