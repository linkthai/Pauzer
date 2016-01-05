#include "mastermodel.h"

MasterModel::MasterModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    qRegisterMetaType<QMap<QString,QMap<int,QString> >>("MyQMapType");

    mode = Mode::ALBUM;
    count = 0;

}

void MasterModel::initializeModel()
{
//    albumList.clear();

//    albumList = Manager::parser.GetAllAlbums();

    LibraryCreator *albumCreator = new LibraryCreator(this);
    albumCreator->setIndex(1);
    albumCreator->start();

    connect(albumCreator, SIGNAL(createdAlbumList(QMap<QString,QMap<int,QString> >)),
            this, SLOT(setAlbumList(QMap<QString,QMap<int,QString> >)));

    LibraryCreator *artistCreator = new LibraryCreator(this);
    artistCreator->setIndex(2);
    artistCreator->start();

    connect(artistCreator, SIGNAL(createdArtistList(QStringList)),
            this, SLOT(setArtistList(QStringList)));

    LibraryCreator *songCreator = new LibraryCreator(this);
    songCreator->setIndex(3);
    songCreator->start();

    connect(songCreator, SIGNAL(createdSongList(QMap<QString,QMap<int,QString> >)),
            this, SLOT(setSongList(QMap<QString,QMap<int,QString> >)));

    albumCreator->wait();
    artistCreator->wait();
    songCreator->wait();

    delete albumCreator;
    delete artistCreator;
    delete songCreator;
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
    resetInternalData();

    endResetModel();

}

void MasterModel::setAlbumList(QMap<QString, QMap<int, QString> > _albumList)
{
    albumList = _albumList;
}

void MasterModel::setArtistList(QStringList _artistList)
{
    artistList = _artistList;
}

void MasterModel::setSongList(QMap<QString, QMap<int, QString> > _songList)
{
    songList = _songList;
}

int MasterModel::rowCount(const QModelIndex &parent) const
{
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
                return albumList["Album_name"][row];
                break;
            case 1:
                return albumList["Artist_name"][row];
                break;
            case 2:
                QString year = albumList["Year"][row];
                if (year != "0")
                    return year;
                else
                    return QVariant();
                break;
            }
        }
            break;
        case Mode::ARTIST:
            return artistList.at(row);
            break;
        case Mode::SONG:
        {
            if (col < 0 || col >= SongCol)
                return QVariant();

            switch(col)
            {
            case 0:
                return songList["Title"][row];
                break;
            case 1:
                return songList["Artist"][row];
                break;
            case 2:
                return songList["Album"][row];
                break;
            case 3:
                return songList["Genre"][row];
                break;
            case 4:
                return songList["Year"][row];
                break;
            case 5:
                return songList["Duration"][row];
                break;
            }
        }
            break;
        }
    }
    return QVariant();
}



