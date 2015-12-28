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
    ui->view_Playlist->setStyleSheet("QListView {"
                                     "border: 1px solid #737373;"
                                     "border-radius: 4px;"
                                     "background-color: #1a1a1a;"
                                     "outline: 0px;"
                                     "}"
                                     "QListView::item {"
                                     "background-color: transparent;"
                                     "color: white;"
                                     "height: 60px;"
                                     "margin: 2px;"
                                     "padding: 5px;"
                                     "border-radius: 2px;"
                                     "}"
                                     "QListView::item:selected {"
                                     "selection-color: #61d169;"
                                     "selection-background-color: white;"
                                     "background-color: #4d4d4d;"
                                     "}");
    ui->view_Playlist->setContentsMargins(10, 10, 10, 10);
    ui->view_Playlist->setDragDropMode(QAbstractItemView::DragDrop);
    ui->view_Playlist->setDefaultDropAction(Qt::MoveAction);
    ui->view_Playlist->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->view_Playlist->setIconSize(QSize(50, 50));
    ui->view_Playlist->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->view_Playlist->setModel(model);

    grd_main->addWidget(ui->grbx_Playlist);
    ui->grbx_Playlist->setStyleSheet("QGroupBox {"
                                     "border: 0px;"
                                     "background-color: transparent;"
                                     "}");
    ui->grbx_Playlist->setContentsMargins(5, 5, 5, 5);
    ui->grbx_Playlist->setFixedHeight(50);


}

PlaylistQueueWidget::~PlaylistQueueWidget()
{
    delete ui;
}
