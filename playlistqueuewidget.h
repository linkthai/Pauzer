#ifndef PLAYLISTQUEUEWIDGET_H
#define PLAYLISTQUEUEWIDGET_H

#include <QWidget>
#include <QScrollBar>
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
private slots:
    void on_btn_Play_clicked();

    void on_btn_Clear_clicked();

    void on_btn_Delete_clicked();

    void on_btn_Up_clicked();

    void on_btn_Down_clicked();

    void changeNextPlaylist();
    void changePrevPlaylist();

private:
    Ui::PlaylistQueueWidget *ui;
    QLabel *grbx_item;
    PlaylistQueueModel *model;

};

#endif // PLAYLISTQUEUEWIDGET_H
