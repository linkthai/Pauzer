#ifndef PLAYLIST_H
#define PLAYLIST_H

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
    void nextSong(bool isShuffling);
    void prevSong(bool isShuffling);
    void reShuffle(bool changeSong = false);
private:
    QString listName;
    QList<int> songList;
    QList<int> shuffleList;
    int currentSong;
    Type type;
signals:
    void changeCurrentSong(int currentSong, bool isPlaylistRepeated);
public slots:
};

#endif // PLAYLIST_H
