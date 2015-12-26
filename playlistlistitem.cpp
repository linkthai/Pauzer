#include "playlistlistitem.h"

PlaylistListItem::PlaylistListItem(QListWidget *parent): QListWidgetItem(parent)
{
    this->setIcon(QIcon(":/icons/Pauzer_Icon.png"));
    this->setText("Master");
    this->setFont(QFont("Segoe UI", 12));
}

