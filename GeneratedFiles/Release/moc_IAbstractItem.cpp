/****************************************************************************
** Meta object code from reading C++ file 'IAbstractItem.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../IAbstractItem.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'IAbstractItem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_IAbstractItem_t {
    QByteArrayData data[17];
    char stringdata0[153];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IAbstractItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IAbstractItem_t qt_meta_stringdata_IAbstractItem = {
    {
QT_MOC_LITERAL(0, 0, 13), // "IAbstractItem"
QT_MOC_LITERAL(1, 14, 5), // "dirty"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 6), // "setPen"
QT_MOC_LITERAL(4, 28, 3), // "pen"
QT_MOC_LITERAL(5, 32, 8), // "setBrush"
QT_MOC_LITERAL(6, 41, 5), // "brush"
QT_MOC_LITERAL(7, 47, 8), // "setAngle"
QT_MOC_LITERAL(8, 56, 5), // "angle"
QT_MOC_LITERAL(9, 62, 18), // "setShearHorizontal"
QT_MOC_LITERAL(10, 81, 15), // "shearHorizontal"
QT_MOC_LITERAL(11, 97, 16), // "setShearVertical"
QT_MOC_LITERAL(12, 114, 13), // "shearVertical"
QT_MOC_LITERAL(13, 128, 8), // "setShear"
QT_MOC_LITERAL(14, 137, 11), // "setLocation"
QT_MOC_LITERAL(15, 149, 1), // "x"
QT_MOC_LITERAL(16, 151, 1) // "y"

    },
    "IAbstractItem\0dirty\0\0setPen\0pen\0"
    "setBrush\0brush\0setAngle\0angle\0"
    "setShearHorizontal\0shearHorizontal\0"
    "setShearVertical\0shearVertical\0setShear\0"
    "setLocation\0x\0y"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IAbstractItem[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       7,   80, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   55,    2, 0x0a /* Public */,
       5,    1,   58,    2, 0x0a /* Public */,
       7,    1,   61,    2, 0x0a /* Public */,
       9,    1,   64,    2, 0x0a /* Public */,
      11,    1,   67,    2, 0x0a /* Public */,
      13,    2,   70,    2, 0x0a /* Public */,
      14,    2,   75,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QPen,    4,
    QMetaType::Void, QMetaType::QBrush,    6,
    QMetaType::Void, QMetaType::Double,    8,
    QMetaType::Void, QMetaType::Double,   10,
    QMetaType::Void, QMetaType::Double,   12,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   10,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   15,   16,

 // properties: name, type, flags
       6, QMetaType::QBrush, 0x00095103,
       4, QMetaType::QPen, 0x00095103,
       8, QMetaType::Double, 0x00095103,
      10, QMetaType::Double, 0x00095103,
      12, QMetaType::Double, 0x00095103,
      15, QMetaType::Int, 0x00095103,
      16, QMetaType::Int, 0x00095103,

       0        // eod
};

void IAbstractItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IAbstractItem *_t = static_cast<IAbstractItem *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->dirty(); break;
        case 1: _t->setPen((*reinterpret_cast< const QPen(*)>(_a[1]))); break;
        case 2: _t->setBrush((*reinterpret_cast< const QBrush(*)>(_a[1]))); break;
        case 3: _t->setAngle((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->setShearHorizontal((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->setShearVertical((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->setShear((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 7: _t->setLocation((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (IAbstractItem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IAbstractItem::dirty)) {
                *result = 0;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        IAbstractItem *_t = static_cast<IAbstractItem *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QBrush*>(_v) = _t->brush(); break;
        case 1: *reinterpret_cast< QPen*>(_v) = _t->pen(); break;
        case 2: *reinterpret_cast< double*>(_v) = _t->angle(); break;
        case 3: *reinterpret_cast< double*>(_v) = _t->shearHorizontal(); break;
        case 4: *reinterpret_cast< double*>(_v) = _t->shearVertical(); break;
        case 5: *reinterpret_cast< int*>(_v) = _t->x(); break;
        case 6: *reinterpret_cast< int*>(_v) = _t->y(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        IAbstractItem *_t = static_cast<IAbstractItem *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setBrush(*reinterpret_cast< QBrush*>(_v)); break;
        case 1: _t->setPen(*reinterpret_cast< QPen*>(_v)); break;
        case 2: _t->setAngle(*reinterpret_cast< double*>(_v)); break;
        case 3: _t->setShearHorizontal(*reinterpret_cast< double*>(_v)); break;
        case 4: _t->setShearVertical(*reinterpret_cast< double*>(_v)); break;
        case 5: _t->setX(*reinterpret_cast< int*>(_v)); break;
        case 6: _t->setY(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject IAbstractItem::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_IAbstractItem.data,
      qt_meta_data_IAbstractItem,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *IAbstractItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IAbstractItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_IAbstractItem.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "IIdInterface"))
        return static_cast< IIdInterface*>(this);
    return QObject::qt_metacast(_clname);
}

int IAbstractItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 7;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void IAbstractItem::dirty()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
