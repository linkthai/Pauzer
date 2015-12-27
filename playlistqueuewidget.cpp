#include "playlistqueuewidget.h"
#include "ui_playlistqueuewidget.h"
#include <QBoxLayout>
#include <QGroupBox>

PlaylistQueueWidget::PlaylistQueueWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaylistQueueWidget)
{
    ui->setupUi(this);

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
    ui->view_Playlist->setStyleSheet("QListWidget {"
                                     "border: 1px solid #737373;"
                                     "border-radius: 4px;"
                                     "background-color: #1a1a1a;"
                                     "outline: 0px;"
                                     "}"
                                     "QListWidget::item {"
                                     "background-color: transparent;"
                                     "color: white;"
                                     "height: 60px;"
                                     "margin: 2px;"
                                     "padding: 5px;"
                                     "border-radius: 2px;"
                                     "}"
                                     "QListWidget::item:selected {"
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

    grd_main->addWidget(ui->grbx_Playlist);
    ui->grbx_Playlist->setStyleSheet("QGroupBox {"
                                     "border: 0px;"
                                     "background-color: transparent;"
                                     "}");
    ui->grbx_Playlist->setContentsMargins(5, 5, 5, 5);
    ui->grbx_Playlist->setFixedHeight(50);

    grbx_item == NULL;

}

PlaylistQueueWidget::~PlaylistQueueWidget()
{
    delete ui;
}

void PlaylistQueueWidget::createListFromQueue()
{
    queue = PlaylistQueue::getInstance();
    ui->view_Playlist->clear();

    QList<Playlist *> playlist = queue->getList();
    PlaylistListItem *item = NULL;
    Playlist *current = NULL;

    for (int i = 0; i < playlist.size(); i++)
    {
        current = playlist.at(i);
        item = new PlaylistListItem(current->getType(), current->getId(), current->getName());

        ui->view_Playlist->addItem(item);
    }

}

void PlaylistQueueWidget::on_btn_Play_clicked()
{

    if (ui->view_Playlist->selectedItems().size() != 0)
    {
        PlaylistListItem *item = static_cast<PlaylistListItem *>(ui->view_Playlist->selectedItems().first());

        if (item)
        {
            queue = PlaylistQueue::getInstance();

            queue->setPlaylistToPlayer(ui->view_Playlist->row(item));
        }
    }
}

void PlaylistQueueWidget::changeCurrentPlaylist()
{
    PlaylistListItem *item = static_cast<PlaylistListItem *>
            (ui->view_Playlist->item(queue->getCurrentPlaylistNum()));
    Playlist *current = queue->getCurrentPlaylist();

    for (int i = 0; i < ui->view_Playlist->count(); i++)
        ui->view_Playlist->removeItemWidget(ui->view_Playlist->item(i));

    grbx_item = new QLabel(this);
    QGridLayout *grd_item = new QGridLayout();
    grbx_item->setLayout(grd_item);

    grbx_item->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    grbx_item->setStyleSheet("background-color: #61d169;"
                             "border: 2px solid #61d169;"
                             "color: white;");
    grbx_item->setFont(QFont("Segoe UI", 12));
    grbx_item->setText(current->getName());

    ui->view_Playlist->setItemWidget(item, grbx_item);
}
