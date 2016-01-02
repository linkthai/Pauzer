#ifndef MASTERMODEL_H
#define MASTERMODEL_H

#include <QAbstractTableModel>
#include "manager.h"

class MasterModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Mode { ALBUM, ARTIST, SONG };
    static const int AlbumCol = 3;
    static const int ArtistCol = 1;
    static const int SongCol = 6;

    MasterModel(QObject* parent = 0);

    void setMode(const Mode& _mode);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
private:
    Mode mode;
    int count;
};

#endif // MASTERMODEL_H
