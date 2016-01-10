#ifndef PLAYLISTQUEUELISTVIEW_H
#define PLAYLISTQUEUELISTVIEW_H

#include <QListView>
#include <QScrollBar>

#include "playlistqueuemodel.h"

class PlaylistQueueListView : public QListView
{
    Q_OBJECT
public:
    PlaylistQueueListView(QWidget *parent = 0);
public slots:
    void doubleClickPlaylist(const QModelIndex & index);
};

#endif // PLAYLISTQUEUELISTVIEW_H
