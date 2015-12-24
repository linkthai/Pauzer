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
private:
    bool isPlaying;
    bool isShuffling;
    bool isRepeating;
    bool isChangingSong;
    unsigned long channel;
    float volume;
    QTimer *t;
    Playlist *playlist;

    static Player* player;
public:
    explicit Player(QObject *parent = 0);
    ~Player();
    bool getPlaying();
    float getVolume();

    static Player* getInstance()
    {
        if (!player)
            player = new Player();

        return player;
    }

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
    void changeToPlaylist(int playlistNum);
    void setShuffle(bool state);
    void setRepeat(bool state);
    void setVolume(float vol);
    void checkEndPlayback();
};

#endif // PLAYER_H
