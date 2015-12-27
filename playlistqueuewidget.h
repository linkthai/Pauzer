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
public slots:
    void changeCurrentPlaylist();
private slots:
    void on_btn_Play_clicked();
private:
    Ui::PlaylistQueueWidget *ui;
    QLabel *grbx_item;
    PlaylistQueue *queue;

};

#endif // PLAYLISTQUEUEWIDGET_H
