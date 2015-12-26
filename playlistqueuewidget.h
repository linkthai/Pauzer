#ifndef PLAYLISTQUEUEWIDGET_H
#define PLAYLISTQUEUEWIDGET_H

#include <QWidget>
#include "playlistlistitem.h"

namespace Ui {
class PlaylistQueueWidget;
}

class PlaylistQueueWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlaylistQueueWidget(QWidget *parent = 0);
    ~PlaylistQueueWidget();

private:
    Ui::PlaylistQueueWidget *ui;
};

#endif // PLAYLISTQUEUEWIDGET_H
