/****************************************************************************
** Meta object code from reading C++ file 'minipauzer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stable.h"
#include "../minipauzer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'minipauzer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MiniPauzer_t {
    QByteArrayData data[15];
    char stringdata0[229];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MiniPauzer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MiniPauzer_t qt_meta_stringdata_MiniPauzer = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MiniPauzer"
QT_MOC_LITERAL(1, 11, 18), // "updateLabelCurTime"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 4), // "time"
QT_MOC_LITERAL(4, 36, 17), // "updateLabelMaxLen"
QT_MOC_LITERAL(5, 54, 5), // "legth"
QT_MOC_LITERAL(6, 60, 13), // "getFolderList"
QT_MOC_LITERAL(7, 74, 4), // "list"
QT_MOC_LITERAL(8, 79, 17), // "releaseButtonPlay"
QT_MOC_LITERAL(9, 97, 19), // "on_btn_Play_clicked"
QT_MOC_LITERAL(10, 117, 26), // "on_sliderBar_sliderPressed"
QT_MOC_LITERAL(11, 144, 27), // "on_sliderBar_sliderReleased"
QT_MOC_LITERAL(12, 172, 28), // "on_btn_ChooseFolders_clicked"
QT_MOC_LITERAL(13, 201, 17), // "on_detected_audio"
QT_MOC_LITERAL(14, 219, 9) // "audio_num"

    },
    "MiniPauzer\0updateLabelCurTime\0\0time\0"
    "updateLabelMaxLen\0legth\0getFolderList\0"
    "list\0releaseButtonPlay\0on_btn_Play_clicked\0"
    "on_sliderBar_sliderPressed\0"
    "on_sliderBar_sliderReleased\0"
    "on_btn_ChooseFolders_clicked\0"
    "on_detected_audio\0audio_num"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MiniPauzer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x08 /* Private */,
       4,    1,   62,    2, 0x08 /* Private */,
       6,    1,   65,    2, 0x08 /* Private */,
       8,    0,   68,    2, 0x08 /* Private */,
       9,    0,   69,    2, 0x08 /* Private */,
      10,    0,   70,    2, 0x08 /* Private */,
      11,    0,   71,    2, 0x08 /* Private */,
      12,    0,   72,    2, 0x08 /* Private */,
      13,    1,   73,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::QStringList,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   14,

       0        // eod
};

void MiniPauzer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MiniPauzer *_t = static_cast<MiniPauzer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateLabelCurTime((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->updateLabelMaxLen((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->getFolderList((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 3: _t->releaseButtonPlay(); break;
        case 4: _t->on_btn_Play_clicked(); break;
        case 5: _t->on_sliderBar_sliderPressed(); break;
        case 6: _t->on_sliderBar_sliderReleased(); break;
        case 7: _t->on_btn_ChooseFolders_clicked(); break;
        case 8: _t->on_detected_audio((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject MiniPauzer::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MiniPauzer.data,
      qt_meta_data_MiniPauzer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MiniPauzer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MiniPauzer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MiniPauzer.stringdata0))
        return static_cast<void*>(const_cast< MiniPauzer*>(this));
    return QWidget::qt_metacast(_clname);
}

int MiniPauzer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
