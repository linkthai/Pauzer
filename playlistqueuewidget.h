#ifndef PLAYLISTQUEUEWIDGET_H
#define PLAYLISTQUEUEWIDGET_H

#include <QWidget>
#include "playlistlistitem.h"
#include "playlistqueue.h"

namespace Ui {
class PlaylistQueueWidget;
}

class PlaylistQueueWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlaylistQueueWidget(QWidget *parent = 0);
    ~PlaylistQueueWidget();

    void createListFromQueue();

private:
    Ui::PlaylistQueueWidget *ui;
    QGroupBox *grbx_item;
};

#endif // PLAYLISTQUEUEWIDGET_H
