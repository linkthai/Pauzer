#include "mastertableview.h"

MasterTableView::MasterTableView(QWidget* parent):
    QTableView(parent)
{
    QHeaderView *verticalHeader = this->verticalHeader();
    verticalHeader->setVisible(false);
    verticalHeader->sectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(45);

    QHeaderView *horizontalHeader = this->horizontalHeader();
    horizontalHeader->setVisible(false);
    horizontalHeader->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    horizontalHeader->setMinimumSectionSize(80);
    horizontalHeader->setMaximumSectionSize(260);

    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setTextElideMode(Qt::ElideRight);

    this->setDragDropMode(DragDropMode::DragOnly);
    this->setDragEnabled(true);
    this->setAcceptDrops(false);

    this->setShowGrid(false);
    this->setAutoScroll(false);
    this->setAlternatingRowColors(true);
    this->setStyleSheet("QTableView{"
                             "outline: 0;"
                             "padding: 10px;"
                             "background-color: #f9f9f9;"
                             "alternate-background-color: white;"
                             "}"
                             "QTableView::item{"
                             "color: #1a1a1a;"
                             "padding-left: 5px;"
                             "padding-right: 5px;"
                             "}"
                             "QTableView::item:selected{"
                             "background-color: #b0e8b4;"
                             "}");

    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(ShowContextMenu(const QPoint&)));
}

void MasterTableView::ShowContextMenu(const QPoint &pos)
{
    QPoint globalPos = this->mapToGlobal(pos);
    globalPos.setX(globalPos.x() + 5);
    globalPos.setY(globalPos.y() + 5);

    QMenu menu;

    QAction *play = new QAction(tr("&Play"), this);
    play->setText(tr("Play"));
    connect(play, SIGNAL(triggered()), this, SLOT(playPlaylist()));

    QAction *queue = new QAction(tr("&Queue"), this);
    queue->setText(tr("Queue"));
    connect(queue, SIGNAL(triggered()), this, SLOT(queuePlaylist()));

    menu.addAction(play);
    menu.addAction(queue);

    menu.setFont(QFont("Segoe UI", 10));
    menu.setFixedWidth(150);

    menu.setStyleSheet("QMenu{"
                       "margin: 2px;"
                       "background-color: white;"
                       "}"
                       "QMenu::item{"
                       "color: #1a1a1a;"
                       "}"
                       "QMenu::item:selected{"
                       "background-color: #b0e8b4;"
                       "}");

    menu.exec(globalPos);

    delete play;
    delete queue;
}

void MasterTableView::playPlaylist()
{
    MasterModel *model = static_cast<MasterModel *>(this->model());
    model->playPlaylist(this->selectedIndexes().front().row());
}

void MasterTableView::queuePlaylist()
{
    MasterModel *model = static_cast<MasterModel *>(this->model());
    model->queuePlaylist(this->selectedIndexes().front().row());
}

