/****************************************************************************
** Meta object code from reading C++ file 'pdfreportgenerator.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../pdfreportgenerator.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pdfreportgenerator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN18PdfReportGeneratorE_t {};
} // unnamed namespace

template <> constexpr inline auto PdfReportGenerator::qt_create_metaobjectdata<qt_meta_tag_ZN18PdfReportGeneratorE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "PdfReportGenerator",
        "reportGenerated",
        "",
        "filePath",
        "reportFailed",
        "reason",
        "generateBasicReport",
        "chipName",
        "testMode",
        "totalTests",
        "passedTests",
        "failedTests",
        "messages"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'reportGenerated'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'reportFailed'
        QtMocHelpers::SignalData<void(const QString &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 },
        }}),
        // Method 'generateBasicReport'
        QtMocHelpers::MethodData<bool(const QString &, const QString &, const QString &, int, int, int, const QStringList &)>(6, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 7 }, { QMetaType::QString, 8 }, { QMetaType::Int, 9 },
            { QMetaType::Int, 10 }, { QMetaType::Int, 11 }, { QMetaType::QStringList, 12 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<PdfReportGenerator, qt_meta_tag_ZN18PdfReportGeneratorE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject PdfReportGenerator::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18PdfReportGeneratorE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18PdfReportGeneratorE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN18PdfReportGeneratorE_t>.metaTypes,
    nullptr
} };

void PdfReportGenerator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<PdfReportGenerator *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->reportGenerated((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->reportFailed((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: { bool _r = _t->generateBasicReport((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[6])),(*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[7])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (PdfReportGenerator::*)(const QString & )>(_a, &PdfReportGenerator::reportGenerated, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (PdfReportGenerator::*)(const QString & )>(_a, &PdfReportGenerator::reportFailed, 1))
            return;
    }
}

const QMetaObject *PdfReportGenerator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PdfReportGenerator::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18PdfReportGeneratorE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int PdfReportGenerator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void PdfReportGenerator::reportGenerated(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void PdfReportGenerator::reportFailed(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}
QT_WARNING_POP
