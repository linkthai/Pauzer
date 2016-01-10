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
    void playPlaylist(int row);
    void queuePlaylist(int row);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QStringList mimeTypes() const;
    QMimeData* mimeData(const QModelIndexList & indexes) const;
signals:
    void playlistPlayed(QString, int);
    void playlistQueued(QString, int);
public slots:
    void setAlbumList(QMap<QString, QStringList> _albumList);
    void setArtistList(QList<QPair<int, QString>> _artistList);
    void setSongList(QMap<QString, QMap<int, QString>> _songList);
private:
    Mode mode;
    int count;

    QMap<QString, QStringList> albumList;
    QMap<QString, QMap<int, QString>> songList;
    QList<QPair<int, QString>> artistList;
};

#endif // MASTERMODEL_H
