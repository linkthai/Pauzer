#ifndef LIBRARYCREATOR_H
#define LIBRARYCREATOR_H

#include <QThread>
#include "manager.h"
#include "master.h"

class LibraryCreator : public QThread
{
    Q_OBJECT
public:
    LibraryCreator(QObject *parent = 0);
    void setList(QStringList folders);
    void setIndex(int i);
    void run();
private:
    QStringList list;
    int index;
signals:
    void createdAlbumList(QMap<QString, QMap<int, QString>> albumList);
    void createdArtistList(QStringList artistList);
    void createdSongList(QMap<QString, QMap<int, QString>> songList);
};

#endif // LIBRARYCREATOR_H
