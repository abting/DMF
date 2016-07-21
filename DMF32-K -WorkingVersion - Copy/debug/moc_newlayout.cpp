/****************************************************************************
** Meta object code from reading C++ file 'newlayout.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../newlayout.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'newlayout.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_newlayout_t {
    QByteArrayData data[7];
    char stringdata0[133];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_newlayout_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_newlayout_t qt_meta_stringdata_newlayout = {
    {
QT_MOC_LITERAL(0, 0, 9), // "newlayout"
QT_MOC_LITERAL(1, 10, 22), // "on_enterButton_clicked"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 23), // "on_cancelButton_clicked"
QT_MOC_LITERAL(4, 58, 35), // "on_loadContactPadInfoButton_c..."
QT_MOC_LITERAL(5, 94, 30), // "on_defaultNumberingBox_clicked"
QT_MOC_LITERAL(6, 125, 7) // "checked"

    },
    "newlayout\0on_enterButton_clicked\0\0"
    "on_cancelButton_clicked\0"
    "on_loadContactPadInfoButton_clicked\0"
    "on_defaultNumberingBox_clicked\0checked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_newlayout[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    0,   36,    2, 0x08 /* Private */,
       5,    1,   37,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::QString,
    QMetaType::Void, QMetaType::Bool,    6,

       0        // eod
};

void newlayout::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        newlayout *_t = static_cast<newlayout *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_enterButton_clicked(); break;
        case 1: _t->on_cancelButton_clicked(); break;
        case 2: { QString _r = _t->on_loadContactPadInfoButton_clicked();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 3: _t->on_defaultNumberingBox_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject newlayout::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_newlayout.data,
      qt_meta_data_newlayout,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *newlayout::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *newlayout::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_newlayout.stringdata0))
        return static_cast<void*>(const_cast< newlayout*>(this));
    return QDialog::qt_metacast(_clname);
}

int newlayout::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
