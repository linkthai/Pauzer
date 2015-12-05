#ifndef PLAYER_H
#define PLAYER_H

#include <stable.h>

void CALLBACK PauseAfterFadeOut(HSYNC handle, DWORD channel, DWORD data, void *user);

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = 0);
    ~Player();
private:
    unsigned long channel;
    QTimer *t;
    bool changeToSong(int songNum);
signals:
    void posChanged(int time);
    void songLength(int length);
public slots:
    void play();
    void pause();
    void setPosition(int cur);
    void signalUpdate();
};

#endif // PLAYER_H
