#include "playlist.h"
#include "manager.h"

Playlist::Playlist(QObject *parent) : QObject(parent)
{
    currentSong = 0;
    currentPos = 0;
    id = 0;
}

void Playlist::setPlaylist(Type _type, int playlistNum)
{
    songList.clear();

    type = _type;
    id = playlistNum;

    switch(type)
    {
    default:
        for (int i = 0; i < Manager::master.GetCount(); i++)
        {
            songList.append(i);
        }
        break;

    case Type::ALBUM:
        QList<int> album = Manager::parser.GetSongsByAlbum(playlistNum);
        for (int i = 0; i < 10; i++)
        {
            songList.append(i + 40);
        }
        this->setName(Manager::parser.GetAlbumNameByID(playlistNum));
        break;
    }

    reShuffle();
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
    if (isShuffling)
    {
        currentSong = shuffleList.at(0);
        emit changeCurrentSong(currentSong);
    }
    else
    {
        currentSong = songList.at(0);
        emit changeCurrentSong(currentSong);
    }
}

void Playlist::nextSong(bool isShuffling)
{
    int num;

    if (isShuffling)
    {
        num = shuffleList.indexOf(currentSong);
        if (num == shuffleList.size() - 1)
        {
            reShuffle();
            num = 0;
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
            num = 0;
        }
        else
        {
            num++;
        }
        currentSong = songList.at(num);
    }

    if (num == 0)
        emit changeCurrentSong(currentSong);
    else
        emit changeCurrentSong(currentSong);
}

void Playlist::prevSong(bool isShuffling)
{
    int num;

    if (isShuffling)
    {
        num = shuffleList.indexOf(currentSong);
        if (num == 0)
        {
            num = 0;
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
            num = songList.size() - 1;
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

    if (changeSong == true)
    {
        currentSong = shuffleList.at(0);
        emit changeCurrentSong(currentSong);
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

Playlist::Type Playlist::getType()
{
    return type;
}

int Playlist::getId()
{
    return id;
}

