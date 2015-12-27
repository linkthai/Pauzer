#ifndef PLAYLISTLISTITEM_H
#define PLAYLISTLISTITEM_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QListWidgetItem>
#include <QStyledItemDelegate>

class PlaylistListItem : public QListWidgetItem
{
public:
    explicit PlaylistListItem(QListWidget *parent = 0);

signals:

public slots:
};

#endif // PLAYLISTLISTITEM_H
