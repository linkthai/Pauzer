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

    grbx_item = new QGroupBox(this);
    QGridLayout *grd_item = new QGridLayout();
    grbx_item->setLayout(grd_item);

}

PlaylistQueueWidget::~PlaylistQueueWidget()
{
    delete ui;
}

void PlaylistQueueWidget::createListFromQueue()
{
    ui->view_Playlist->clear();

    PlaylistQueue *queue = PlaylistQueue::getInstance();

    QList<Playlist *> playlist = queue->getList();
    PlaylistListItem *item = NULL;
    Playlist *current = NULL;

    for (int i = 0; i < playlist.size(); i++)
    {
        current = playlist.at(i);
        item = new PlaylistListItem(current->getType(), current->getId(), current->getName());

        ui->view_Playlist->addItem(item);

        if (i == queue->getCurrentPlaylistNum())
        {
            ui->view_Playlist->setItemWidget(item, grbx_item);
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
                                             ""
                                             "}"
                                             "QListWidget::item:selected {"
                                             "selection-color: #61d169;"
                                             "background-color: #4d4d4d;"
                                             "}");
        }
    }

}
