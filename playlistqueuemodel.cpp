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

    clearQueue();

    Playlist *master = new Playlist();
    master->setPlaylist(Playlist::Type::MASTER, 0);
    list.append(master);

    Player::getInstance()->changeToPlaylist(master, true, true);

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
            setPlayerState(false);
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
    while (list.size() > 0)
    {
        removePlaylist(0, true);
    }
}

void PlaylistQueueModel::clearQueueButMaster()
{
    int index = 0;
    while (list.size() > 1)
    {
        if (!removePlaylist(index, false))
        {
            index = 1;
        }
    }
}

bool PlaylistQueueModel::removePlaylist(const int& row, bool removeMaster)
{
    //return false if attempt to delete master
    if (!removeMaster && list.at(row)->getType() == Playlist::Type::MASTER)
        return false;

    //check if playlist being deleted is currently being played
    if (row == getCurrentPlaylistNum())
    {
        if (!removeMaster)
        {
            if (row == list.size() - 1)
                Player::getInstance()->changeToPlaylist(list.at(row - 1));
            else
                Player::getInstance()->changeToPlaylist(list.at(row + 1));
        }
    }

    removeRow(row);

    return true;
}

bool PlaylistQueueModel::movePlaylist(const int& from, const int& to)
{
    QModelIndex index = QModelIndex();

    int endRow = to;
    if (endRow == -1 || endRow >= rowCount(index))
        endRow = rowCount(index) - 1;

    return moveRows(index, from, 1, index, endRow);
}

void PlaylistQueueModel::setPlayerState(bool PauseToPlay)
{
    emit switchPlayerState(PauseToPlay);
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

    if (row < 0 || row >= list.size())
        return QVariant();

    switch (role)
    {
    case Qt::DisplayRole:
        return list.at(row)->getName();
        break;
    case Qt::FontRole:
        return QFont("Segoe UI", 10);
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

bool PlaylistQueueModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0 || row > rowCount())
        return false;

    beginInsertRows(parent, row, row + count - 1);

    for (int i = 0; i < count; i++)
    {
        Playlist *playlist = new Playlist();
        list.insert(i + row, playlist);
    }

    endInsertRows();

    return true;
}

bool PlaylistQueueModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);

    for (int i = 0; i < count; i++)
    {
        delete list.at(row);
        list.removeAt(row);
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

Qt::ItemFlags PlaylistQueueModel::flags(const QModelIndex &index) const
{
    return (QAbstractListModel::flags(index) | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
}

Qt::DropActions PlaylistQueueModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList PlaylistQueueModel::mimeTypes() const
{
    QStringList types;
    types << "application/x-playlist" << "application/x-playlist-move";
    return types;
}

QMimeData* PlaylistQueueModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (QModelIndex index, indexes) {
        if (index.isValid()) {

            int num = index.row();

            stream << num;
        }
    }

    mimeData->setData("application/x-playlist-move", encodedData);
    return mimeData;
}

bool PlaylistQueueModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
                                      int row, int column, const QModelIndex &parent)
{
    if (!data->hasFormat("application/x-playlist") && !data->hasFormat("application/x-playlist-move"))
        return false;

    if (action == Qt::IgnoreAction)
        return true;

    if (column > 0)
        return false;

    beginResetModel();

    if (data->hasFormat("application/x-playlist"))
    {
        QByteArray encodedData = data->data("application/x-playlist");
        QDataStream stream(&encodedData, QIODevice::ReadOnly);

        QString type;
        int num;
        stream >> type >> num;

        if (!parent.isValid()) {
            if (row < 0)
                row = rowCount();
            else
                row = qMin(row, rowCount());
        } else {
            row = parent.row();
        }

        insertRow(row);

        if (type == "Album")
        {
            list[row]->setPlaylist(Playlist::Type::ALBUM, num);
        }
        else
            if (type == "Artist")
            {
                list[row]->setPlaylist(Playlist::Type::ARTIST, num);
            }
            else
            {
                list[row]->setPlaylist(Playlist::Type::SONG, num);
            }
    }
    else
    {
        QByteArray encodedData = data->data("application/x-playlist-move");
        QDataStream stream(&encodedData, QIODevice::ReadOnly);

        int num;
        stream >> num;

        if (!parent.isValid()) {
            if (row < 0)
                row = rowCount();
            else
                row = qMin(row, rowCount());
        } else {
            row = parent.row();
        }

        movePlaylist(num, row);
    }

    endResetModel();

    return true;
}

void PlaylistQueueModel::playPlaylist(QString type, int num)
{
    beginResetModel();

    int currentRow = getCurrentPlaylistNum();
    if (list[currentRow]->getType() == Playlist::Type::MASTER)
        currentRow++;

    insertRow(currentRow);

    if (type == "Album")
    {
        list[currentRow]->setPlaylist(Playlist::Type::ALBUM, num);
    }
    else
        if (type == "Artist")
        {
            list[currentRow]->setPlaylist(Playlist::Type::ARTIST, num);
        }
        else
        {
            list[currentRow]->setPlaylist(Playlist::Type::SONG, num);
        }

    setCurrentPlaylist(currentRow);
    emit playCurrent();

    endResetModel();
}

void PlaylistQueueModel::queuePlaylist(QString type, int num)
{
    beginResetModel();

    int newRow = rowCount();
    insertRow(newRow);

    if (type == "Album")
    {
        list[newRow]->setPlaylist(Playlist::Type::ALBUM, num);
    }
    else
        if (type == "Artist")
        {
            list[newRow]->setPlaylist(Playlist::Type::ARTIST, num);
        }
        else
        {
            list[newRow]->setPlaylist(Playlist::Type::SONG, num);
        }

    endResetModel();
}

void PlaylistQueueModel::playPlaylist(int row)
{
    setCurrentPlaylist(row);
    emit playCurrent();
}

