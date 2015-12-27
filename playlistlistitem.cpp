#include "playlistlistitem.h"

PlaylistListItem::PlaylistListItem(Playlist::Type type, int id, QString name, QListWidget *parent)
    : QListWidgetItem(parent)
{
    QIcon icon;

    switch (type)
    {
    case Playlist::Type::ALBUM:

        icon.addPixmap(QPixmap(":/icons/Pauzer_Icon.png"), QIcon::Normal);
        icon.addPixmap(QPixmap(":/icons/Pauzer_Icon.png"), QIcon::Selected);

        break;
    default:        

        icon.addPixmap(QPixmap(":/icons/Pauzer.png"), QIcon::Normal);
        icon.addPixmap(QPixmap(":/icons/Pauzer.png"), QIcon::Selected);

        break;
    }
    this->setIcon(icon);
    this->setText(name);
    this->setFont(QFont("Segoe UI", 12));
}

