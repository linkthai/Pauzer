#include "playlistqueuewidget.h"
#include "ui_playlistqueuewidget.h"
#include <QBoxLayout>
#include <QGroupBox>

PlaylistQueueWidget::PlaylistQueueWidget(PlaylistQueueModel *_model, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaylistQueueWidget)
{
    ui->setupUi(this);

    model = _model;
    connect(Player::getInstance(), SIGNAL(nextPlaylist()), this, SLOT(changeNextPlaylist()));
    connect(Player::getInstance(), SIGNAL(prevPlaylist()), this, SLOT(changePrevPlaylist()));

    connect(model, SIGNAL(playCurrent()), this, SLOT(playCurrent()));

    connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(modelScrollTo(QModelIndex,int,int)));

    QVBoxLayout *layout = new QVBoxLayout();
    this->setLayout(layout);

    layout->setMargin(0);

    QGroupBox *grbx_main = new QGroupBox();
    layout->addWidget(grbx_main);

    QVBoxLayout *grd_main = new QVBoxLayout();
    grbx_main->setLayout(grd_main);


    grd_main->setMargin(20);
    grd_main->setSpacing(10);

    grd_main->setDirection(QBoxLayout::TopToBottom);

    grd_main->addWidget(ui->view_Playlist);
    model->setParent(ui->view_Playlist);
    ui->view_Playlist->setModel(model);

    grd_main->addWidget(ui->grbx_Playlist);
    ui->grbx_Playlist->setStyleSheet("QGroupBox {"
                                     "border: 0px;"
                                     "background-color: transparent;"
                                     "}");
    ui->grbx_Playlist->setContentsMargins(5, 5, 5, 5);
    ui->grbx_Playlist->setFixedHeight(50);

    ui->btn_Play->setStyleSheet("QPushButton#btn_Play{"
                                "width: 30px;"
                                "height: 30px;"
                                "image: url(:/icons/Playlist_Play.png)"
                                "}"
                                "QPushButton#btn_Play:hover{"
                                "image: url(:/icons/Playlist_Play_Hover.png)"
                                "}");
    ui->btn_Play->setToolTip(tr("Start playlist"));

    ui->btn_Up->setStyleSheet("QPushButton#btn_Up{"
                                "width: 30px;"
                                "height: 30px;"
                                "image: url(:/icons/Playlist_Up.png)"
                                "}"
                                "QPushButton#btn_Up:hover{"
                                "image: url(:/icons/Playlist_Up_Hover.png)"
                                "}");
    ui->btn_Up->setToolTip(tr("Move up playlist"));

    ui->btn_Down->setStyleSheet("QPushButton#btn_Down{"
                                "width: 30px;"
                                "height: 30px;"
                                "image: url(:/icons/Playlist_Down.png)"
                                "}"
                                "QPushButton#btn_Down:hover{"
                                "image: url(:/icons/Playlist_Down_Hover.png)"
                                "}");
    ui->btn_Down->setToolTip(tr("Move down playlist"));


    ui->btn_Delete->setStyleSheet("QPushButton#btn_Delete{"
                                "width: 30px;"
                                "height: 30px;"
                                "image: url(:/icons/Playlist_Delete.png)"
                                "}"
                                "QPushButton#btn_Delete:hover{"
                                "image: url(:/icons/Playlist_Delete_Hover.png)"
                                "}");
    ui->btn_Delete->setToolTip(tr("Delete playlist"));


    ui->btn_Clear->setStyleSheet("QPushButton#btn_Clear{"
                                "width: 30px;"
                                "height: 30px;"
                                "image: url(:/icons/Playlist_Clear.png)"
                                "}"
                                "QPushButton#btn_Clear:hover{"
                                "image: url(:/icons/Playlist_Clear_Hover.png)"
                                "}");
    ui->btn_Clear->setToolTip(tr("Clear playlist"));



}

PlaylistQueueWidget::~PlaylistQueueWidget()
{
    delete ui;
}

void PlaylistQueueWidget::on_btn_Play_clicked()
{
    if (ui->view_Playlist->selectionModel()->selectedRows().size() == 0)
        return;

    int selected = ui->view_Playlist->selectionModel()->selectedRows().front().row();

    model->setCurrentPlaylist(selected);
    model->setPlayerState(true);
}

void PlaylistQueueWidget::on_btn_Clear_clicked()
{
    model->clearQueueButMaster();
}

void PlaylistQueueWidget::on_btn_Delete_clicked()
{
    if (ui->view_Playlist->selectionModel()->selectedRows().size() == 0)
        return;

    int selected = ui->view_Playlist->selectionModel()->selectedRows().front().row();

    model->removePlaylist(selected);
}

void PlaylistQueueWidget::on_btn_Up_clicked()
{
    if (ui->view_Playlist->selectionModel()->selectedRows().size() == 0)
        return;

    int selected = ui->view_Playlist->selectionModel()->selectedRows().front().row();

    model->movePlaylist(selected, selected - 1);
}

void PlaylistQueueWidget::on_btn_Down_clicked()
{
    if (ui->view_Playlist->selectionModel()->selectedRows().size() == 0)
        return;

    int selected = ui->view_Playlist->selectionModel()->selectedRows().front().row();

    model->movePlaylist(selected, selected + 1);
}

void PlaylistQueueWidget::modelScrollTo(const QModelIndex &parent, int start, int end)
{
    ui->view_Playlist->scrollToBottom();
}

void PlaylistQueueWidget::changeNextPlaylist()
{
    int current = model->getCurrentPlaylistNum();

    model->setCurrentPlaylist(current + 1, true);
}

void PlaylistQueueWidget::changePrevPlaylist()
{
    int current = model->getCurrentPlaylistNum();

    model->setCurrentPlaylist(current - 1, false);
}

void PlaylistQueueWidget::playCurrent()
{
    model->setPlayerState(true);
}
