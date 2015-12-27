#include "playlistqueue.h"

PlaylistQueue* PlaylistQueue::queue = NULL;

PlaylistQueue::PlaylistQueue(QObject *parent) : QObject(parent)
{
    Playlist *master = new Playlist();
    master->setName("Master");
    master->setPlaylist(Playlist::Type::MASTER, 0);

    list.append(master);

    Playlist *pharmacy = new Playlist();
    pharmacy->setPlaylist(Playlist::Type::ALBUM, 1);
    list.append(pharmacy);

    currentPlaylist = 0;
}

PlaylistQueue::~PlaylistQueue()
{
    while (list.size())
    {
        delete list.first();
        list.removeFirst();
    }
}

QList<Playlist *> PlaylistQueue::getList()
{
    return list;
}

int PlaylistQueue::getCurrentPlaylistNum()
{
    return currentPlaylist;
}

Playlist *PlaylistQueue::getCurrentPlaylist()
{
    return list.at(currentPlaylist);
}

void PlaylistQueue::setPlaylistToPlayer(int playlistNum)
{
    currentPlaylist = playlistNum;

    Player* player = Player::getInstance();
    player->changeToPlaylist();

    emit currentPlaylistChanged();
}

void PlaylistQueue::clearList()
{
    while (list.size())
    {
        delete list.first();
        list.removeFirst();
    }

    Playlist *master = new Playlist();
    master->setName("Master");
    master->setPlaylist(Playlist::Type::MASTER, 0);

    list.append(master);

    currentPlaylist = 0;
}

