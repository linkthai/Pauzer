#ifndef PLAYLISTQUEUE_H
#define PLAYLISTQUEUE_H

#include <QObject>
#include <QList>
#include "playlist.h"
#include "player.h"

class PlaylistQueue : public QObject
{
    Q_OBJECT
private:
    QList<Playlist *> list;
    int currentPlaylist;

    static PlaylistQueue *queue;
public:
    explicit PlaylistQueue(QObject *parent = 0);
    ~PlaylistQueue();

    static PlaylistQueue* getInstance()
    {
        if (!queue)
            queue = new PlaylistQueue();

        return queue;
    }

    QList<Playlist *> getList();
    int getCurrentPlaylistNum();

    Playlist *getCurrentPlaylist();
    void setPlaylistToPlayer(int playlistNum = 0);
    void clearList();

signals:

public slots:
};

#endif // PLAYLISTQUEUE_H
