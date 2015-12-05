#include "player.h"

Player::Player(QObject *parent) :
    QObject(parent)
{
    if (!BASS_Init(-1, 44100, 0, NULL, NULL))
        qDebug() << "Cannot initialize device";
    t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(signalUpdate()));
    //endOfMusic = true;
}

Player::~Player()
{


}

bool Player::changeToSong(int songNum)
{
    QString filename = "C:\\1.mp3";

    if (!(channel = BASS_StreamCreateFile(false, filename.toLatin1(), 0, 0, NULL))
        && !(channel = BASS_MusicLoad(false, filename.toLatin1(), 0, 0, BASS_MUSIC_RAMP, 0)))
            return false;

    emit songLength(BASS_ChannelBytes2Seconds(channel, BASS_ChannelGetLength(channel, BASS_POS_BYTE)));

    return true;
}

void CALLBACK PauseAfterFadeOut(HSYNC handle, DWORD channel, DWORD data, void *user)
{
    if (data != 0)
        return;
    BASS_ChannelPause(channel);
}

void Player::play()
{
    //Check is channel is Active
    //If no then start the playlist, if yes then resume the song
    if (BASS_ChannelIsActive(channel) == BASS_ACTIVE_STOPPED)
    {
        BASS_ChannelStop(channel);
        if (changeToSong(0) == false)
        {
            qDebug() << "Can't play file";
        }
    }

    if (!BASS_ChannelPlay(channel, false))
        qDebug() << "Error resuming";
    else
    {
        t->start(100);
        //playing = true;
    }

    //Fade in music after starting or resuming
    BASS_ChannelSetAttribute(channel, BASS_ATTRIB_VOL, 0);
    BASS_ChannelSlideAttribute(channel, BASS_ATTRIB_VOL, 1.0, 750);
}

void Player::pause()
{
    //Fade out music
    BASS_ChannelSlideAttribute(channel, BASS_ATTRIB_VOL, 0, 750);

    //After fade out call function to pause
    BASS_ChannelSetSync(channel, BASS_SYNC_SLIDE | BASS_SYNC_ONETIME, 0, &PauseAfterFadeOut, 0);
    t->stop();
    //playing = false;
}

void Player::setPosition(int cur)
{
    BASS_ChannelSetPosition(channel, BASS_ChannelSeconds2Bytes(channel, cur), BASS_POS_BYTE);
}

void Player::signalUpdate()
{
    emit posChanged(BASS_ChannelBytes2Seconds(channel, BASS_ChannelGetPosition(channel, BASS_POS_BYTE)));

}
