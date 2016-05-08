/****************************************************************************
** Meta object code from reading C++ file 'mykinect2.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mykinect2.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mykinect2.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MyKinect2_t {
    QByteArrayData data[12];
    char stringdata0[159];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyKinect2_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyKinect2_t qt_meta_stringdata_MyKinect2 = {
    {
QT_MOC_LITERAL(0, 0, 9), // "MyKinect2"
QT_MOC_LITERAL(1, 10, 23), // "signal_changecolorframe"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 23), // "signal_changedepthframe"
QT_MOC_LITERAL(4, 59, 20), // "signal_changeskelpos"
QT_MOC_LITERAL(5, 80, 23), // "InitializeDefaultSensor"
QT_MOC_LITERAL(6, 104, 7), // "HRESULT"
QT_MOC_LITERAL(7, 112, 6), // "Update"
QT_MOC_LITERAL(8, 119, 11), // "ProcessBody"
QT_MOC_LITERAL(9, 131, 10), // "nBodyCount"
QT_MOC_LITERAL(10, 142, 7), // "IBody**"
QT_MOC_LITERAL(11, 150, 8) // "ppBodies"

    },
    "MyKinect2\0signal_changecolorframe\0\0"
    "signal_changedepthframe\0signal_changeskelpos\0"
    "InitializeDefaultSensor\0HRESULT\0Update\0"
    "ProcessBody\0nBodyCount\0IBody**\0ppBodies"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyKinect2[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,
       3,    0,   45,    2, 0x06 /* Public */,
       4,    0,   46,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   47,    2, 0x08 /* Private */,
       7,    0,   48,    2, 0x08 /* Private */,
       8,    2,   49,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    0x80000000 | 6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 10,    9,   11,

       0        // eod
};

void MyKinect2::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MyKinect2 *_t = static_cast<MyKinect2 *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signal_changecolorframe(); break;
        case 1: _t->signal_changedepthframe(); break;
        case 2: _t->signal_changeskelpos(); break;
        case 3: { HRESULT _r = _t->InitializeDefaultSensor();
            if (_a[0]) *reinterpret_cast< HRESULT*>(_a[0]) = _r; }  break;
        case 4: _t->Update(); break;
        case 5: _t->ProcessBody((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< IBody**(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MyKinect2::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyKinect2::signal_changecolorframe)) {
                *result = 0;
            }
        }
        {
            typedef void (MyKinect2::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyKinect2::signal_changedepthframe)) {
                *result = 1;
            }
        }
        {
            typedef void (MyKinect2::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyKinect2::signal_changeskelpos)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject MyKinect2::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_MyKinect2.data,
      qt_meta_data_MyKinect2,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MyKinect2::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyKinect2::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MyKinect2.stringdata0))
        return static_cast<void*>(const_cast< MyKinect2*>(this));
    return QThread::qt_metacast(_clname);
}

int MyKinect2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void MyKinect2::signal_changecolorframe()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void MyKinect2::signal_changedepthframe()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void MyKinect2::signal_changeskelpos()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
