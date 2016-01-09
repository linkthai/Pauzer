#include "playlist.h"
#include "manager.h"

Playlist::Playlist(QObject *parent) : QObject(parent)
{
    currentSong = -1;
    currentPos = 0;
    id = 0;
    isInit = false;
}

void Playlist::setPlaylist(Type _type, int playlistNum)
{
    songList.clear();

    type = _type;
    id = playlistNum;

    QList<int> list;

    switch(type)
    {
    default:
        for (int i = 0; i < Manager::master.GetCount(); i++)
        {
            songList.append(i);
        }
        this->setName("Master");
        break;

    case Type::ALBUM:
        list = Manager::parser.GetSongsByAlbum(playlistNum);
        for (int i = 0; i < list.size(); i++)
        {
            songList.append(list.at(i));
        }
        this->setName(Manager::parser.GetAlbumNameByID(playlistNum));
        break;
    case Type::ARTIST:
    {
        QList<int> albums;
        albums = Manager::parser.GetAlbumsByArtist(playlistNum);

        for (int i = 0; i < albums.size(); i++)
        {
            list = Manager::parser.GetSongsByAlbum(albums.at(i));
            for (int j = 0; j < list.size(); j++)
            {
                songList.append(list.at(j));
            }
        }
        this->setName(Manager::parser.GetArtistNameByID(playlistNum));
        break;
    }
    case Type::SONG:
    {
        songList.append(playlistNum);

        QString filename = Manager::master.Get(playlistNum);
        TagLib::MPEG::File f( reinterpret_cast<const wchar_t*>(filename.constData()) );
        QString title = TStringToQString(f.ID3v2Tag()->title());

        if (title == "")
        {
            QStringList name = filename.split("/");
            title = name.value(name.length() - 1);
            name = title.split(".");
            title = name.value(0);
        }
        title = title.simplified();

        this->setName(title);
        break;
    }
    }

    this->setIcon();
    isInit = true;
}

Playlist::~Playlist()
{

}

QList<int> Playlist::getSongList()
{
    return songList;
}

QList<int> Playlist::getShuffleList()
{
    return shuffleList;
}

int Playlist::getCurrentSong()
{
    return currentSong;
}

void Playlist::playFirstSong(bool isShuffling)
{
    currentSong = -1;
    if (isShuffling)
    {
        reShuffle();
        currentSong = shuffleList.at(0);
        emit changeCurrentSong(currentSong);
    }
    else
    {
        currentSong = songList.at(0);
        emit changeCurrentSong(currentSong);
    }
}

void Playlist::playLastSong(bool isShuffling)
{
    currentSong = -1;
    if (isShuffling)
    {
        reShuffle();
        currentSong = shuffleList.at(shuffleList.size() - 1);
        emit changeCurrentSong(currentSong);
    }
    else
    {
        currentSong = songList.at(songList.size() - 1);
        emit changeCurrentSong(currentSong);
    }
}

void Playlist::nextSong(bool isShuffling, bool isLocked)
{
    int num;

    if (isShuffling)
    {
        num = shuffleList.indexOf(currentSong);
        if (num == shuffleList.size() - 1)
        {
            if (isLocked)
            {
                reShuffle(true);
                num = 0;
            }
            else
            {
                emit nextPlaylist();
                return;
            }
        }
        else
        {
            num++;
        }
        currentSong = shuffleList.at(num);
    }
    else
    {
        num = songList.indexOf(currentSong);
        if (num == songList.size() - 1)
        {
            if (isLocked)
            {
                num = 0;
            }
            else
            {
                emit nextPlaylist();
                return;
            }
        }
        else
        {
            num++;
        }
        currentSong = songList.at(num);
    }

    emit changeCurrentSong(currentSong);
}

void Playlist::prevSong(bool isShuffling, bool isLocked)
{
    int num;

    if (isShuffling)
    {
        num = shuffleList.indexOf(currentSong);
        if (num == 0)
        {
            if (isLocked)
            {
                num = shuffleList.size() - 1;
            }
            else
            {
                emit prevPlaylist();
                return;
            }
        }
        else
        {
            num--;
        }
        currentSong = shuffleList.at(num);
    }
    else
    {
        num = songList.indexOf(currentSong);
        if (num == 0)
        {
            if (isLocked)
            {
                num = songList.size() - 1;
            }
            else
            {
                emit prevPlaylist();
                return;
            }
        }
        else
        {
            num--;
        }
        currentSong = songList.at(num);
    }

    emit changeCurrentSong(currentSong);
}

void Playlist::reShuffle(bool changeSong)
{
    int lastSongId = -1;
    if (shuffleList.size() > 1)
        lastSongId = shuffleList.at(shuffleList.size() - 1);

    shuffleList.clear();

    QList<int> tempList;
    for (int i = 0; i < songList.size(); i++)
    {
        tempList.append(songList.at(i));
    }

    while(tempList.size() != 0)
    {
        int rand = qrand() % tempList.size();
        shuffleList.append(tempList.at(rand));
        tempList.removeAt(rand);
    }

    //if the playlist is playing
    if (!changeSong)
    {
        if (currentSong != -1)
        {
            shuffleList.move(shuffleList.indexOf(currentSong), 0);
        }
    }
    else
    {
        if (lastSongId == shuffleList.at(0))
        {
            int rand = qrand() % (shuffleList.size() - 1);
            shuffleList.move(0, rand + 1);
        }
    }
}

void Playlist::setName(QString _name)
{
    if (_name == "")
        return;

    name = _name;
}

QString Playlist::getName()
{
    return name;
}

void Playlist::setIcon()
{
    QString filename;
    bool coverFound = false;
    int i = 0;

    if (type == Playlist::Type::MASTER)
    {
        QPixmap pixMap(":/icons/Pauzer_White.png");
        icon.addPixmap(pixMap, QIcon::Normal);
        icon.addPixmap(pixMap, QIcon::Selected);

        coverFound = true;
    }

    while (coverFound == false && i < songList.size())
    {
        filename = Manager::master.Get(songList.at(i));
        TagLib::MPEG::File f(reinterpret_cast<const wchar_t*>(filename.constData()));

        TagLib::ID3v2::Tag *tag = f.ID3v2Tag();

        if (tag->isEmpty())
        {
            i++;
            continue;
        }

        TagLib::ID3v2::FrameList framelist = tag->frameList("APIC");

        if (framelist.isEmpty())
        {
            i++;
            continue;
        }
        else
        {
            TagLib::ID3v2::AttachedPictureFrame *pic = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(framelist.front());
            if (pic)
            {
                QImage coverArt;
                coverArt.loadFromData((const uchar *)pic->picture().data(), pic->picture().size());

                QPixmap pixMap = QPixmap::fromImage(coverArt);
                icon.addPixmap(pixMap, QIcon::Normal);
                icon.addPixmap(pixMap, QIcon::Selected);
                coverFound = true;
            }
            else
            {
                i++;
                continue;
            }
        }
    }

    if (coverFound == false)
    {
        QPixmap pixMap(":/resources/cover.png");
        icon.addPixmap(pixMap, QIcon::Normal);
        icon.addPixmap(pixMap, QIcon::Selected);
    }
}

QIcon Playlist::getIcon()
{
    return icon;
}

Playlist::Type Playlist::getType()
{
    return type;
}

int Playlist::getId()
{
    return id;
}

bool Playlist::getInit()
{
    return isInit;
}

