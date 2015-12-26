#ifndef PLAYLISTQUEUE_H
#define PLAYLISTQUEUE_H

#include <QObject>
#include <QList>
#include "playlist.h"

class PlaylistQueue : public QObject
{
    Q_OBJECT
private:
    QList<Playlist *> list;

    static PlaylistQueue *queue;
public:
    explicit PlaylistQueue(QObject *parent = 0);

    static PlaylistQueue* getInstance()
    {
        if (!queue)
            queue = new PlaylistQueue();

        return queue;
    }

signals:

public slots:
};

#endif // PLAYLISTQUEUE_H
