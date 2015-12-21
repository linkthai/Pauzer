#include "playlist.h"
#include "manager.h"
#include <QMessageBox>

Playlist::Playlist(QObject *parent) : QObject(parent)
{
    currentSong = 0;
}

void Playlist::setPlaylist(int playlistNum, bool isShuffling)
{
    songList.clear();

    for (int i = 0; i < Manager::master.GetCount(); i++)
    {
        songList.append(i);
    }

    reShuffle();

    if (isShuffling)
    {
        currentSong = shuffleList.at(0);
    }
    else
    {
        currentSong = songList.at(0);
    }
    emit changeCurrentSong(currentSong, false);
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
        emit changeCurrentSong(currentSong, true);
    else
        emit changeCurrentSong(currentSong, false);
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

    emit changeCurrentSong(currentSong, false);
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
        emit changeCurrentSong(currentSong, false);
    }
}

