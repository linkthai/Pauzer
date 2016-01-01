#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QList>
#include <QIcon>

class Playlist : public QObject
{
    Q_OBJECT
public:
    enum class Type {MASTER = 0, ALBUM = 1, ARTIST = 2, SONG = 3, USER = 4, FOLDERS = 5};

    explicit Playlist(QObject *parent = 0);
    ~Playlist();
    QList<int> getSongList();
    QList<int> getShuffleList();
    void setPlaylist(Type _type, int playlistNum);
    int getCurrentSong();
    void playFirstSong(bool isShuffling);
    void playLastSong(bool isShuffling);
    void nextSong(bool isShuffling);
    void prevSong(bool isShuffling);
    void reShuffle(bool changeSong = false);

    void setName(QString _name);
    QString getName();

    void setIcon();
    QIcon getIcon();

    Type getType();
    int getId();

private:
    QString name;
    QList<int> songList;
    QList<int> shuffleList;
    int currentSong;
    int currentPos;
    Type type;
    int id;
    QIcon icon;
signals:
    void changeCurrentSong(int currentSong);
    void nextPlaylist();
    void prevPlaylist();
public slots:
};

#endif // PLAYLIST_H
