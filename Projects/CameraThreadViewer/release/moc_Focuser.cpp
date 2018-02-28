/****************************************************************************
** Meta object code from reading C++ file 'Focuser.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Focuser.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Focuser.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Focuser_t {
    QByteArrayData data[19];
    char stringdata0[196];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Focuser_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Focuser_t qt_meta_stringdata_Focuser = {
    {
QT_MOC_LITERAL(0, 0, 7), // "Focuser"
QT_MOC_LITERAL(1, 8, 14), // "calculateFocus"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 12), // "moveFirgelli"
QT_MOC_LITERAL(4, 37, 5), // "value"
QT_MOC_LITERAL(5, 43, 12), // "startImaging"
QT_MOC_LITERAL(6, 56, 7), // "focusOn"
QT_MOC_LITERAL(7, 64, 9), // "bestFocus"
QT_MOC_LITERAL(8, 74, 14), // "cancelFocusing"
QT_MOC_LITERAL(9, 89, 11), // "updateImage"
QT_MOC_LITERAL(10, 101, 11), // "std::string"
QT_MOC_LITERAL(11, 113, 13), // "setSaveFolder"
QT_MOC_LITERAL(12, 127, 8), // "setLaser"
QT_MOC_LITERAL(13, 136, 11), // "focusCamera"
QT_MOC_LITERAL(14, 148, 11), // "sendMessage"
QT_MOC_LITERAL(15, 160, 10), // "newMessage"
QT_MOC_LITERAL(16, 171, 4), // "type"
QT_MOC_LITERAL(17, 176, 4), // "data"
QT_MOC_LITERAL(18, 181, 14) // "grabberStarted"

    },
    "Focuser\0calculateFocus\0\0moveFirgelli\0"
    "value\0startImaging\0focusOn\0bestFocus\0"
    "cancelFocusing\0updateImage\0std::string\0"
    "setSaveFolder\0setLaser\0focusCamera\0"
    "sendMessage\0newMessage\0type\0data\0"
    "grabberStarted"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Focuser[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,
       3,    1,   80,    2, 0x06 /* Public */,
       5,    0,   83,    2, 0x06 /* Public */,
       6,    1,   84,    2, 0x06 /* Public */,
       7,    1,   87,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   90,    2, 0x0a /* Public */,
       9,    1,   91,    2, 0x0a /* Public */,
      11,    1,   94,    2, 0x0a /* Public */,
      12,    2,   97,    2, 0x0a /* Public */,
      13,    0,  102,    2, 0x0a /* Public */,
      14,    0,  103,    2, 0x0a /* Public */,
      15,    2,  104,    2, 0x0a /* Public */,
      18,    0,  109,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::UInt,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::UInt,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::Short,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QByteArray,   16,   17,
    QMetaType::Void,

       0        // eod
};

void Focuser::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Focuser *_t = static_cast<Focuser *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->calculateFocus(); break;
        case 1: _t->moveFirgelli((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 2: _t->startImaging(); break;
        case 3: _t->focusOn((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->bestFocus((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 5: _t->cancelFocusing(); break;
        case 6: _t->updateImage((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        case 7: _t->setSaveFolder((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->setLaser((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< qint16(*)>(_a[2]))); break;
        case 9: _t->focusCamera(); break;
        case 10: _t->sendMessage(); break;
        case 11: _t->newMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 12: _t->grabberStarted(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Focuser::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Focuser::calculateFocus)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Focuser::*_t)(uint );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Focuser::moveFirgelli)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Focuser::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Focuser::startImaging)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (Focuser::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Focuser::focusOn)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (Focuser::*_t)(uint );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Focuser::bestFocus)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject Focuser::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Focuser.data,
      qt_meta_data_Focuser,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Focuser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Focuser::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Focuser.stringdata0))
        return static_cast<void*>(const_cast< Focuser*>(this));
    return QObject::qt_metacast(_clname);
}

int Focuser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void Focuser::calculateFocus()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void Focuser::moveFirgelli(uint _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Focuser::startImaging()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void Focuser::focusOn(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Focuser::bestFocus(uint _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
