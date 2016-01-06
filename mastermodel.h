#ifndef MASTERMODEL_H
#define MASTERMODEL_H

#include <QAbstractTableModel>
#include <QMetaType>
#include <QMimeData>

#include "manager.h"
#include "librarycreator.h"

class MasterModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Mode { ALBUM, ARTIST, SONG };
    static const int AlbumCol = 3;
    static const int ArtistCol = 1;
    static const int SongCol = 6;

    MasterModel(QObject* parent = 0);

    void initializeModel();

    void setMode(const Mode& _mode);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QStringList mimeTypes() const;
    QMimeData* mimeData(const QModelIndexList & indexes) const;
public slots:
    void setAlbumList(QMap<QString, QMap<int, QString>> _albumList);
    void setArtistList(QStringList _artistList);
    void setSongList(QMap<QString, QMap<int, QString>> _songList);
private:
    Mode mode;
    int count;

    QMap<QString, QMap<int, QString>> albumList;
    QMap<QString, QMap<int, QString>> songList;
    QStringList artistList;
};

#endif // MASTERMODEL_H
