#include "playlistqueue.h"

PlaylistQueue::PlaylistQueue(QObject *parent) : QObject(parent)
{
    Playlist *master = new Playlist();
    master->setPlaylist(Playlist::Type::MASTER, 0);

    list.append(master);
}

