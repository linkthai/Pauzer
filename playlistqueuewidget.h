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

private:
    Ui::PlaylistQueueWidget *ui;
    QLabel *grbx_item;
    PlaylistQueueModel *model;

private slots:
    void on_btn_Play_clicked();

    void on_btn_Clear_clicked();

    void on_btn_Delete_clicked();

    void on_btn_Up_clicked();

    void on_btn_Down_clicked();

    void modelScrollTo(const QModelIndex & parent, int start, int end);

    void changeNextPlaylist();
    void changePrevPlaylist();

    void playCurrent();

};

#endif // PLAYLISTQUEUEWIDGET_H
