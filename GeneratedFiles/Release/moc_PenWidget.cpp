/****************************************************************************
** Meta object code from reading C++ file 'PenWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../PenWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PenWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PenWidget_t {
    QByteArrayData data[12];
    char stringdata0[116];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PenWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PenWidget_t qt_meta_stringdata_PenWidget = {
    {
QT_MOC_LITERAL(0, 0, 9), // "PenWidget"
QT_MOC_LITERAL(1, 10, 10), // "penChanged"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 3), // "pen"
QT_MOC_LITERAL(4, 26, 6), // "setPen"
QT_MOC_LITERAL(5, 33, 11), // "updateColor"
QT_MOC_LITERAL(6, 45, 5), // "index"
QT_MOC_LITERAL(7, 51, 11), // "updateWidth"
QT_MOC_LITERAL(8, 63, 5), // "value"
QT_MOC_LITERAL(9, 69, 15), // "updateLineStyle"
QT_MOC_LITERAL(10, 85, 14), // "updateCapStyle"
QT_MOC_LITERAL(11, 100, 15) // "updateJoinStyle"

    },
    "PenWidget\0penChanged\0\0pen\0setPen\0"
    "updateColor\0index\0updateWidth\0value\0"
    "updateLineStyle\0updateCapStyle\0"
    "updateJoinStyle"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PenWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   52,    2, 0x0a /* Public */,
       5,    1,   55,    2, 0x08 /* Private */,
       7,    1,   58,    2, 0x08 /* Private */,
       9,    1,   61,    2, 0x08 /* Private */,
      10,    1,   64,    2, 0x08 /* Private */,
      11,    1,   67,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QPen,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QPen,    3,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,

       0        // eod
};

void PenWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PenWidget *_t = static_cast<PenWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->penChanged((*reinterpret_cast< const QPen(*)>(_a[1]))); break;
        case 1: _t->setPen((*reinterpret_cast< const QPen(*)>(_a[1]))); break;
        case 2: _t->updateColor((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->updateWidth((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->updateLineStyle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->updateCapStyle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->updateJoinStyle((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PenWidget::*)(const QPen & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PenWidget::penChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject PenWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PenWidget.data,
      qt_meta_data_PenWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *PenWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PenWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PenWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int PenWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void PenWidget::penChanged(const QPen & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
