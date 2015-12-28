#ifndef PLAYLISTQUEUEWIDGET_H
#define PLAYLISTQUEUEWIDGET_H

#include <QWidget>
#include "playlistqueuemodel.h"

namespace Ui {
class PlaylistQueueWidget;
}

class PlaylistQueueWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlaylistQueueWidget(PlaylistQueueModel *_model, QWidget *parent = 0);
    ~PlaylistQueueWidget();
private:
    Ui::PlaylistQueueWidget *ui;
    QLabel *grbx_item;
    PlaylistQueueModel *model;

};

#endif // PLAYLISTQUEUEWIDGET_H
