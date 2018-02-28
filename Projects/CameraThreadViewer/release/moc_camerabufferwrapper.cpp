/****************************************************************************
** Meta object code from reading C++ file 'camerabufferwrapper.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../camerabufferwrapper.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'camerabufferwrapper.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CameraBufferWrapper_t {
    QByteArrayData data[14];
    char stringdata0[139];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CameraBufferWrapper_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CameraBufferWrapper_t qt_meta_stringdata_CameraBufferWrapper = {
    {
QT_MOC_LITERAL(0, 0, 19), // "CameraBufferWrapper"
QT_MOC_LITERAL(1, 20, 17), // "newImageAvailable"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 11), // "std::string"
QT_MOC_LITERAL(4, 51, 8), // "savedImg"
QT_MOC_LITERAL(5, 60, 8), // "saveSnap"
QT_MOC_LITERAL(6, 69, 9), // "newRawImg"
QT_MOC_LITERAL(7, 79, 9), // "imageSize"
QT_MOC_LITERAL(8, 89, 3), // "src"
QT_MOC_LITERAL(9, 93, 7), // "darkImg"
QT_MOC_LITERAL(10, 101, 5), // "error"
QT_MOC_LITERAL(11, 107, 14), // "grabberStarted"
QT_MOC_LITERAL(12, 122, 8), // "firgelli"
QT_MOC_LITERAL(13, 131, 7) // "trigger"

    },
    "CameraBufferWrapper\0newImageAvailable\0"
    "\0std::string\0savedImg\0saveSnap\0newRawImg\0"
    "imageSize\0src\0darkImg\0error\0grabberStarted\0"
    "firgelli\0trigger"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CameraBufferWrapper[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       4,    0,   67,    2, 0x06 /* Public */,
       5,    1,   68,    2, 0x06 /* Public */,
       6,    1,   71,    2, 0x06 /* Public */,
       7,    3,   74,    2, 0x06 /* Public */,
       9,    0,   81,    2, 0x06 /* Public */,
      10,    1,   82,    2, 0x06 /* Public */,
      11,    0,   85,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    1,   86,    2, 0x0a /* Public */,
      13,    1,   89,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, QMetaType::UInt, QMetaType::UInt, QMetaType::QString,    2,    2,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void CameraBufferWrapper::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CameraBufferWrapper *_t = static_cast<CameraBufferWrapper *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newImageAvailable((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        case 1: _t->savedImg(); break;
        case 2: _t->saveSnap((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->newRawImg((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        case 4: _t->imageSize((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 5: _t->darkImg(); break;
        case 6: _t->error((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->grabberStarted(); break;
        case 8: _t->firgelli((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->trigger((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CameraBufferWrapper::*_t)(std::string );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CameraBufferWrapper::newImageAvailable)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CameraBufferWrapper::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CameraBufferWrapper::savedImg)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (CameraBufferWrapper::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CameraBufferWrapper::saveSnap)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (CameraBufferWrapper::*_t)(std::string );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CameraBufferWrapper::newRawImg)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (CameraBufferWrapper::*_t)(unsigned  , unsigned  , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CameraBufferWrapper::imageSize)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (CameraBufferWrapper::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CameraBufferWrapper::darkImg)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (CameraBufferWrapper::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CameraBufferWrapper::error)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (CameraBufferWrapper::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CameraBufferWrapper::grabberStarted)) {
                *result = 7;
                return;
            }
        }
    }
}

const QMetaObject CameraBufferWrapper::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CameraBufferWrapper.data,
      qt_meta_data_CameraBufferWrapper,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CameraBufferWrapper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CameraBufferWrapper::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CameraBufferWrapper.stringdata0))
        return static_cast<void*>(const_cast< CameraBufferWrapper*>(this));
    return QThread::qt_metacast(_clname);
}

int CameraBufferWrapper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void CameraBufferWrapper::newImageAvailable(std::string _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CameraBufferWrapper::savedImg()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void CameraBufferWrapper::saveSnap(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CameraBufferWrapper::newRawImg(std::string _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CameraBufferWrapper::imageSize(unsigned  _t1, unsigned  _t2, QString _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CameraBufferWrapper::darkImg()
{
    QMetaObject::activate(this, &staticMetaObject, 5, Q_NULLPTR);
}

// SIGNAL 6
void CameraBufferWrapper::error(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CameraBufferWrapper::grabberStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 7, Q_NULLPTR);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
