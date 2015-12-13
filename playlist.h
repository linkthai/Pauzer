#ifndef PLAYLIST_H
#define PLAYLIST_H

class Playlist : public QObject
{
    Q_OBJECT
private:
    QString listName;
    QList<int> songList;
    QList<int> shuffleList;
    int currentSong;
public:
    explicit Playlist(QObject *parent = 0);
    ~Playlist();
    QList<int> getSongList();
    QList<int> getShuffleList();
    void setPlaylist(int playlistNum, bool isShuffling);
    int getCurrentSong();
    void nextSong(bool isShuffling, int repeatMode);
    void prevSong(bool isShuffling, int repeatMode);
    void reShuffle(bool changeSong = false);
signals:
    void changeCurrentSong(int currentSong, bool isPlaylistRepeated);
public slots:
};

#endif // PLAYLIST_H
