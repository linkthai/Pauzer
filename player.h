#ifndef PLAYER_H
#define PLAYER_H

#include "playlist.h"
#include "manager.h"
#include <stable.h>

void CALLBACK PauseAfterFadeOut(HSYNC handle, DWORD channel, DWORD data, void *user);
void CALLBACK EndOfPlayback(HSYNC handle, DWORD channel, DWORD data, void *user);

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = 0);
    ~Player();
	bool getPlaying();
    void changeToPlaylist(int playlistNum);
private:
	bool isPlaying;
    bool isShuffling;
    bool isChangingSong;
    unsigned long channel;
    QTimer *t;
    Playlist *playlist;
    enum Repeat {NO_REPEAT = 0, ONE_SONG = 1, PLAYLIST = 2};
    Repeat repeatMode;
signals:
    void posChanged(int time);
    void songLength(int length);
    void songTitle(QString str);
    void songArtist(QString str);
    void songAlbum(QString str);
    void songCover(const QPixmap &pic);
	void changePlaying(bool isPlaying);
    void testText(QString str);
    void endOfPlaylistNoRepeat();
public slots:
    void play();
    void pause();
    void nextSong();
    void prevSong();
    void setPosition(int cur);
    void signalUpdate();
    void changeToSong(int songNum, bool isPlaylistRepeated);
    void setShuffle(bool state);
    void setRepeat(int state);
    void checkEndPlayback();
};

#endif // PLAYER_H
