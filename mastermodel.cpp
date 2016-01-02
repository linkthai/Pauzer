#include "mastermodel.h"

MasterModel::MasterModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    mode = Mode::ALBUM;
    count = 0;
}

void MasterModel::setMode(const MasterModel::Mode &_mode)
{
    beginResetModel();

    mode = _mode;
    switch (mode)
    {
    case Mode::ALBUM:
        count = Manager::parser.GetAllAlbumsCount();
        break;
    case Mode::ARTIST:
        count = Manager::parser.GetAllArtistsCount();
        break;
    case Mode::SONG:
        count = Manager::parser.GetAllSongsCount();
        break;
    }

    endResetModel();

}


int MasterModel::rowCount(const QModelIndex &parent) const
{
    if (Manager::master.GetCount() <= 0)
        return 0;

    return count;

}

int MasterModel::columnCount(const QModelIndex &parent) const
{
    if (Manager::master.GetCount() <= 0)
        return 0;

    switch(mode)
    {
    case Mode::ALBUM:
        return AlbumCol;
        break;
    case Mode::ARTIST:
        return ArtistCol;
        break;
    case Mode::SONG:
        return SongCol;
        break;
    }

}

QVariant MasterModel::data(const QModelIndex &index, int role) const
{
    if (Manager::master.GetCount() <= 0)
        return QVariant();

    if (!index.isValid())
        return QVariant();

    int row = index.row();
    int col = index.column();

    if (role == Qt::DisplayRole)
    {
        switch(mode)
        {
        case Mode::ALBUM:
        {
            if (col < 0 || col >= AlbumCol)
                return QVariant();

            switch(col)
            {
            case 0:
                return Manager::parser.GetAlbumNameByID(row);
                break;
            default:
                return "NULL";
                break;
            }
        }
            break;
        case Mode::ARTIST:
            return 1;
            break;
        case Mode::SONG:
            return 6;
            break;
        }
    }
    return QVariant();
}



