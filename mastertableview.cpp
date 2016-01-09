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

}

