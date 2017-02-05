/****************************************************************************
** Meta object code from reading C++ file 'scenemodifier.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../scenemodifier.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scenemodifier.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SceneModifier_t {
    QByteArrayData data[7];
    char stringdata0[66];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SceneModifier_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SceneModifier_t qt_meta_stringdata_SceneModifier = {
    {
QT_MOC_LITERAL(0, 0, 13), // "SceneModifier"
QT_MOC_LITERAL(1, 14, 12), // "enableCuboid"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 7), // "enabled"
QT_MOC_LITERAL(4, 36, 6), // "update"
QT_MOC_LITERAL(5, 43, 13), // "updateSubstep"
QT_MOC_LITERAL(6, 57, 8) // "training"

    },
    "SceneModifier\0enableCuboid\0\0enabled\0"
    "update\0updateSubstep\0training"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SceneModifier[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x0a /* Public */,
       4,    0,   32,    2, 0x0a /* Public */,
       5,    1,   33,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    6,

       0        // eod
};

void SceneModifier::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SceneModifier *_t = static_cast<SceneModifier *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->enableCuboid((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->update(); break;
        case 2: _t->updateSubstep((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject SceneModifier::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SceneModifier.data,
      qt_meta_data_SceneModifier,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SceneModifier::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SceneModifier::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SceneModifier.stringdata0))
        return static_cast<void*>(const_cast< SceneModifier*>(this));
    return QObject::qt_metacast(_clname);
}

int SceneModifier::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
