/****************************************************************************
** Meta object code from reading C++ file 'loadlayout.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../loadlayout.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'loadlayout.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_loadlayout_t {
    QByteArrayData data[8];
    char stringdata0[158];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_loadlayout_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_loadlayout_t qt_meta_stringdata_loadlayout = {
    {
QT_MOC_LITERAL(0, 0, 10), // "loadlayout"
QT_MOC_LITERAL(1, 11, 27), // "on_openLayoutButton_clicked"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 31), // "on_openContactPadButton_clicked"
QT_MOC_LITERAL(4, 72, 22), // "on_enterButton_clicked"
QT_MOC_LITERAL(5, 95, 23), // "on_cancelButton_clicked"
QT_MOC_LITERAL(6, 119, 30), // "on_defaultNumberingBox_clicked"
QT_MOC_LITERAL(7, 150, 7) // "checked"

    },
    "loadlayout\0on_openLayoutButton_clicked\0"
    "\0on_openContactPadButton_clicked\0"
    "on_enterButton_clicked\0on_cancelButton_clicked\0"
    "on_defaultNumberingBox_clicked\0checked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_loadlayout[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x08 /* Private */,
       3,    0,   40,    2, 0x08 /* Private */,
       4,    0,   41,    2, 0x08 /* Private */,
       5,    0,   42,    2, 0x08 /* Private */,
       6,    1,   43,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    7,

       0        // eod
};

void loadlayout::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        loadlayout *_t = static_cast<loadlayout *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { QString _r = _t->on_openLayoutButton_clicked();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 1: { QString _r = _t->on_openContactPadButton_clicked();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 2: _t->on_enterButton_clicked(); break;
        case 3: _t->on_cancelButton_clicked(); break;
        case 4: _t->on_defaultNumberingBox_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject loadlayout::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_loadlayout.data,
      qt_meta_data_loadlayout,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *loadlayout::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *loadlayout::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_loadlayout.stringdata0))
        return static_cast<void*>(const_cast< loadlayout*>(this));
    return QDialog::qt_metacast(_clname);
}

int loadlayout::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
