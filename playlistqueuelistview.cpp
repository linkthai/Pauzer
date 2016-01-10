#include "playlistqueuelistview.h"

PlaylistQueueListView::PlaylistQueueListView(QWidget *parent):
    QListView(parent)
{
    this->setStyleSheet("QListView#view_Playlist {"
                                     "border: 1px solid #737373;"
                                     "border-radius: 4px;"
                                     "background-color: #1a1a1a;"
                                     "outline: 0px;"
                                     "}"
                                     "QListView::item {"
                                     "height: 60px;"
                                     "padding: 5px;"
                                     "}"
                                     "QListView::item:selected:active {"
                                     "background-color: #404040;"
                                     "color: white;"
                                     "}"
                                     "QListView::item:selected:!active {"
                                     "background-color: #4d4d4d;"
                                     "color: lightgray;"
                                     "}");
    this->setContentsMargins(10, 10, 10, 10);
    this->setIconSize(QSize(60, 60));

    this->setDragEnabled(true);
    this->viewport()->setAcceptDrops(true);
    this->setDropIndicatorShown(true);
    this->setAutoScroll(true);
    this->setAutoScrollMargin(100);
    this->verticalScrollBar()->setSingleStep(10);

    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    connect(this, SIGNAL(doubleClicked(QModelIndex)), SLOT(doubleClickPlaylist(QModelIndex)));
}

void PlaylistQueueListView::doubleClickPlaylist(const QModelIndex &index)
{
    PlaylistQueueModel *model = static_cast<PlaylistQueueModel *>(this->model());

    model->playPlaylist(index.row());
}

