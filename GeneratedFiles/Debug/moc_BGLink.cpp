/****************************************************************************
** Meta object code from reading C++ file 'BGLink.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../BGLink.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BGLink.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BGLink_t {
    QByteArrayData data[11];
    char stringdata0[82];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BGLink_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BGLink_t qt_meta_stringdata_BGLink = {
    {
QT_MOC_LITERAL(0, 0, 6), // "BGLink"
QT_MOC_LITERAL(1, 7, 12), // "setLinkIndex"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 4), // "from"
QT_MOC_LITERAL(4, 26, 2), // "to"
QT_MOC_LITERAL(5, 29, 9), // "setWeight"
QT_MOC_LITERAL(6, 39, 6), // "weight"
QT_MOC_LITERAL(7, 46, 9), // "fromIndex"
QT_MOC_LITERAL(8, 56, 7), // "toIndex"
QT_MOC_LITERAL(9, 64, 9), // "fromRange"
QT_MOC_LITERAL(10, 74, 7) // "toRange"

    },
    "BGLink\0setLinkIndex\0\0from\0to\0setWeight\0"
    "weight\0fromIndex\0toIndex\0fromRange\0"
    "toRange"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BGLink[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       5,   32, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x0a /* Public */,
       5,    1,   29,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::Int,    6,

 // properties: name, type, flags
       7, QMetaType::Int, 0x00095103,
       8, QMetaType::Int, 0x00095103,
       9, QMetaType::Int, 0x00095001,
      10, QMetaType::Int, 0x00095001,
       6, QMetaType::Int, 0x00095103,

       0        // eod
};

void BGLink::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BGLink *_t = static_cast<BGLink *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setLinkIndex((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->setWeight((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        BGLink *_t = static_cast<BGLink *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->fromIndex(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->toIndex(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->fromRange(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->toRange(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->weight(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        BGLink *_t = static_cast<BGLink *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setFromIndex(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->setToIndex(*reinterpret_cast< int*>(_v)); break;
        case 4: _t->setWeight(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject BGLink::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BGLink.data,
      qt_meta_data_BGLink,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *BGLink::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BGLink::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BGLink.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QGraphicsPathItem"))
        return static_cast< QGraphicsPathItem*>(this);
    return QObject::qt_metacast(_clname);
}

int BGLink::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
