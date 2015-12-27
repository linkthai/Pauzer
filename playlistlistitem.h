#ifndef PLAYLISTLISTITEM_H
#define PLAYLISTLISTITEM_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QListWidgetItem>
#include <QStyledItemDelegate>

#include "playlist.h"

class PlaylistListItem : public QListWidgetItem
{
public:
    explicit PlaylistListItem(Playlist::Type type, int id, QString name, QListWidget *parent = 0);

signals:

public slots:
};

#endif // PLAYLISTLISTITEM_H
