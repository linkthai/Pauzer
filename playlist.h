#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <stable.h>

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
    void nextSong(bool isShuffling, bool isPlaylistRepeating);
    void prevSong(bool isShuffling, bool isPlaylistRepeating);
    void reShuffle();
signals:
    void changeCurrentSong(int currentSong);
public slots:
};

#endif // PLAYLIST_H
