#ifndef PLAYER_H
#define PLAYER_H

#include "playlist.h"

void CALLBACK PauseAfterFadeOut(HSYNC handle, DWORD channel, DWORD data, void *user);

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
signals:
    void posChanged(int time);
    void songLength(int length);
    void songTitle(QString str);
    void songArtist(QString str);
    void songAlbum(QString str);
	void changePlaying(bool isPlaying);
    void testText(QString str);
public slots:
    void play();
    void pause();
    void nextSong();
    void prevSong();
    void setPosition(int cur);
    void signalUpdate();
    void changeToSong(int songNum);
};

#endif // PLAYER_H
