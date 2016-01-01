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
    void setCurrentPlaylist(const int &num, bool firstSong = true);
    void clearQueue();
    int getCurrentPlaylistNum() const;
    bool removePlaylist(const int &row);
    bool movePlaylist(const int &from, const int &to);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
    bool moveRows(const QModelIndex & sourceParent, int sourceRow, int count,
                  const QModelIndex & destinationParent, int destinationChild);
private:
    QList<Playlist *> list;
signals:
    void playlistRemoved();
    void endOfQueue();
};

#endif // PLAYLISTQUEUEMODEL_H
