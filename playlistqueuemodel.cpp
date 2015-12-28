#include "playlistqueuemodel.h"

PlaylistQueueModel::PlaylistQueueModel(QObject *parent) :
    QAbstractListModel(parent)
{
    currentPlaylist = -1;
}

PlaylistQueueModel::~PlaylistQueueModel()
{
    for (int i = 0; i < list.size(); i++)
        delete list.at(i);
}

void PlaylistQueueModel::initializeModel()
{
    beginResetModel();

    Playlist *master = new Playlist();
    master->setName("Master");
    master->setPlaylist(Playlist::Type::MASTER, 0);
    list.append(master);

    currentPlaylist = 0;

    Player::getInstance()->changeToPlaylist(master);

    endResetModel();
}

void PlaylistQueueModel::clearModel()
{

}

int PlaylistQueueModel::rowCount(const QModelIndex &parent) const
{
    return list.size();
}

QVariant PlaylistQueueModel::data(const QModelIndex &index, int role) const
{
    // Check that the index is valid and within the correct range first:
    if (!index.isValid())
        return QVariant();

    if (index.row() >= list.size())
        return QVariant();

    if (role == Qt::DisplayRole) {
        return QVariant(list.at(index.row())->getName());
    } else {
        return QVariant();
    }
}

