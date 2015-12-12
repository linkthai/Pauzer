#ifndef STABLE
#define STABLE

#define QStringToTString(s) TagLib::String(s.utf8().data(), TagLib::String::UTF8)
#define TStringToQString(s) QString::fromUtf8(s.toCString(true))

#include <QtGlobal>
#include <QWidget>
#include <QThread>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QWidget>
#include <QSlider>
#include <QDir>
#include <QDialog>
#include <QFileSystemModel>
#include <QMessageBox>
#include <QFileDialog>
#include <QMouseEvent>
#include <bass.h>
#include <fileref.h>

#endif
