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
#include <QDesktopWidget>
#include <QLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QSizeGrip>
#include <QDomDocument>
#include <QDomElement>
#include <QMenu>
#include <QAction>
#include <QFrame>
#include <QFileInfo>

#include <bass.h>

#include <fileref.h>
#include <mpegfile.h>
#include <tbytevector.h>
#include <id3v2tag.h>
#include <id3v2frame.h>
#include <id3v2header.h>
#include <tlist.h>
#include <tag.h>
#include <taglib.h>
#include <attachedpictureframe.h>

#include <iostream>
#include <string>

using namespace std;


#endif
