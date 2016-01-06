#ifndef PLAYLISTQUEUEMODEL_H
#define PLAYLISTQUEUEMODEL_H

#include <QAbstractListModel>
#include <QVariant>
#include <QMimeData>

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
    void clearQueueButMaster();
    int getCurrentPlaylistNum() const;
    bool removePlaylist(const int &row, bool removeMaster = false);
    bool movePlaylist(const int &from, const int &to);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
    bool moveRows(const QModelIndex & sourceParent, int sourceRow, int count,
                  const QModelIndex & destinationParent, int destinationChild);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    Qt::DropActions supportedDropActions() const;
    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData * data, Qt::DropAction action,
                     int row, int column, const QModelIndex & parent);
private:
    QList<Playlist *> list;
signals:
    void playlistRemoved();
    void endOfQueue();
};

#endif // PLAYLISTQUEUEMODEL_H
