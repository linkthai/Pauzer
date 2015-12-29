#include "playlistqueuemodel.h"

PlaylistQueueModel::PlaylistQueueModel(QObject *parent) :
    QAbstractListModel(parent)
{
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
    master->setPlaylist(Playlist::Type::MASTER, 0);
    list.append(master);

    Player::getInstance()->changeToPlaylist(master);

    Playlist *temp = new Playlist();
    temp->setPlaylist(Playlist::Type::ALBUM, 1);
    list.append(temp);

    Playlist *sad = new Playlist();
    sad->setPlaylist(Playlist::Type::ALBUM, 2);
    list.append(sad);

    sad = new Playlist();
    sad->setPlaylist(Playlist::Type::ALBUM, 3);
    list.append(sad);

    sad = new Playlist();
    sad->setPlaylist(Playlist::Type::ALBUM, 4);
    list.append(sad);

    sad = new Playlist();
    sad->setPlaylist(Playlist::Type::ALBUM, 5);
    list.append(sad);

    sad = new Playlist();
    sad->setPlaylist(Playlist::Type::ALBUM, 6);
    list.append(sad);

    endResetModel();
}

void PlaylistQueueModel::setCurrentPlaylist(const int &num, bool firstSong)
{
    beginResetModel();

    if (num < 0)
    {
        Player::getInstance()->changeToPlaylist(list.at(0), firstSong);
    }
    else
        if (num >= list.size())
        {
            emit endOfQueue();
            Player::getInstance()->changeToPlaylist(list.at(list.size() - 1), firstSong);
        }
        else
        {
            Player::getInstance()->changeToPlaylist(list.at(num), firstSong);
        }


    endResetModel();
}

void PlaylistQueueModel::clearQueue()
{
    //move master to the top (only in case master is allowed to be moved anywhere but top)
    //then set master to be the current playlist
    //pretty much just to make sure
    for (int i = 0; i < list.size(); i++)
    {
        if (list.at(i)->getType() == Playlist::Type::MASTER)
        {
            list.move(i, 0);

            Player::getInstance()->changeToPlaylist(list.front());

            break;
        }
    }

    while (list.size() != 1)
    {
        removeRow(1);
    }
}

bool PlaylistQueueModel::removePlaylist(const int& row)
{
    //return false if attempt to delete master
    if (list.at(row)->getType() == Playlist::Type::MASTER)
        return false;

    //check if playlist being deleted is currently being played
    if (row == getCurrentPlaylistNum())
    {
        if (row == list.size() - 1)
            Player::getInstance()->changeToPlaylist(list.at(row - 1));
        else
            Player::getInstance()->changeToPlaylist(list.at(row + 1));
    }

    removeRow(row);
}

bool PlaylistQueueModel::movePlaylist(const int& from, const int& to)
{
    QModelIndex index = QModelIndex();
    return moveRows(index, from, 1, index, to);
}

int PlaylistQueueModel::getCurrentPlaylistNum() const
{
    return list.indexOf(Player::getInstance()->getPlaylist());
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

    int row = index.row();

    if (row >= list.size())
        return QVariant();

    switch (role)
    {
    case Qt::DisplayRole:
        return list.at(row)->getName();
        break;
    case Qt::FontRole:
        return QFont("UTM Avo", 10);
        break;
    case Qt::BackgroundRole:
    {
        if (row == this->getCurrentPlaylistNum())
        {
            return QVariant(QColor("#61d169"));
        }
        else
        {
            return QVariant();
        }
        break;
    }
    case Qt::ForegroundRole:
    {
        if (row == getCurrentPlaylistNum())
        {
            return QVariant(QColor("white"));
        }
        else
        {
            return QVariant(QColor("lightgray"));
        }
        break;
    }
    case Qt::DecorationRole:
    {
        return QVariant(list.at(row)->getIcon());
    }
    default:
        return QVariant();
    }
}

bool PlaylistQueueModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);

    for (int i = 0; i < count; i++)
    {
        delete list.at(row);
        list.removeAt(row);
        if (row == getCurrentPlaylistNum())
            emit playlistRemoved();
    }

    endRemoveRows();

    return true;
}

bool PlaylistQueueModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count,
                                  const QModelIndex &destinationParent, int destinationChild)
{
    if (sourceRow < 0 || sourceRow >= list.size() ||
            destinationChild < 0 || destinationChild >= list.size())
        return false;

    if (sourceRow <= destinationChild && sourceRow + count - 1 >= destinationChild)
        return false;

    //With List: 0 1 2 3 4, move(3, 1) --> 0 3 1 2 4 or move(2, 4) --> 0 1 3 4 2 (no)
    //                                               or move(2, 3) --> 0 1 3 2 4 (yes)
    //With Data: 0 1 2 3 4, move(3, 1) --> 0 3 1 2 4 or move(2, 4) --> 0 1 3 2 4 (yes)
    //The sourceRow and destinationChild parameter is list-based
    //so we have to increase destinationChild in data by 1 in the case of moving down
    if (sourceRow > destinationChild)
    {
        beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1,
                      destinationParent, destinationChild);
    }
    else
    {
        beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1,
                      destinationParent, destinationChild + 1);
    }

    int src = sourceRow;
    int des = destinationChild;

    for (int i = 0; i < count; i++)
    {
        list.move(src, des);

        if (sourceRow > destinationChild)
        {
            src++;
            des++;
        }
    }

    endMoveRows();
}

