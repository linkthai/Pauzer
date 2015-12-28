#ifndef PLAYLISTQUEUEMODEL_H
#define PLAYLISTQUEUEMODEL_H

#include <QAbstractListModel>
#include <QVariant>

#include "playlist.h"
#include "player.h"

class PlaylistQueueModel : public QAbstractListModel
{
    Q_OBJECT
public:
    PlaylistQueueModel(QObject* parent = 0);
    ~PlaylistQueueModel();

    void initializeModel();
    void clearModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
private:
    QList<Playlist *> list;
    int currentPlaylist;
};

#endif // PLAYLISTQUEUEMODEL_H
