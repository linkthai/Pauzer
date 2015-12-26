#include "playlistmodel.h"

PlaylistModel::PlaylistModel(QObject *parent) : QAbstractListModel(parent)
{

}

int PlaylistModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    else
        return list.size();
}

