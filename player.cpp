#include "player.h"

Player::Player(QObject *parent) :
    QObject(parent)
{
    if (!BASS_Init(-1, 44100, 0, NULL, NULL))
        qDebug() << "Cannot initialize device";


    t = new QTimer(this);
    t->start(500);
    connect(t, SIGNAL(timeout()), this, SLOT(signalUpdate()));

    //endOfMusic = true;

	isPlaying = false;
    isShuffling = false;
    isChangingSong = false;

    playlist = new Playlist(this);
    connect(playlist, SIGNAL(changeCurrentSong(int)), this, SLOT(changeToSong(int)));

    emit(posChanged(0));
}

Player::~Player()
{
    delete playlist;

    if (t != NULL)
        delete t;
}

void Player::changeToPlaylist(int playlistNum)
{
    playlist->setPlaylist(0, TRUE);

}

void Player::changeToSong(int songNum)
{
    isChangingSong = true;
    QString filename;

    switch(songNum)
    {
    case 1:
        filename = "C:/1.mp3";
        break;
    case 2:
        filename = "C:/2.mp3";
        break;
    case 3:
        filename = "C:/3.mp3";
        break;
    default:
        filename = "C:/4.mp3";
        break;
    }

    BASS_ChannelStop(channel);
    BASS_StreamFree(channel);

    channel = BASS_StreamCreateFile(false, filename.toLatin1(), 0, 0, NULL);

    if (isPlaying)
    {
        BASS_ChannelPlay(channel, false);
    }
    else
    {
        BASS_ChannelPause(channel);
    }

    QString text;
    TagLib::FileRef f(QFile::encodeName(filename).constData());

    text = TStringToQString(f.tag()->title());
    emit songTitle(text);

    text = TStringToQString(f.tag()->artist());
    emit songArtist(text);

    text = TStringToQString(f.tag()->album());
    emit songAlbum(text);

    emit songLength(BASS_ChannelBytes2Seconds(channel, BASS_ChannelGetLength(channel, BASS_POS_BYTE)));
    BASS_ChannelSetPosition(channel, 0, BASS_POS_BYTE);
    signalUpdate();
    isChangingSong = false;
}

void CALLBACK PauseAfterFadeOut(HSYNC handle, DWORD channel, DWORD data, void *user)
{
    BASS_ChannelPause(channel);
}

void Player::play()
{
    //Check is channel is Active
    //If no then start the playlist, if yes then resume the song
    if (BASS_ChannelIsActive(channel) == BASS_ACTIVE_STOPPED)
    {
        BASS_ChannelStop(channel);
    }

    if (!BASS_ChannelPlay(channel, false))
        qDebug() << "Error resuming";
    else
    {
        isPlaying = true;
    }

	emit changePlaying(isPlaying);

    //Fade in music after starting or resuming
    BASS_ChannelSetAttribute(channel, BASS_ATTRIB_VOL, 0);
    BASS_ChannelSlideAttribute(channel, BASS_ATTRIB_VOL, 1.0, 750);
}

void Player::pause()
{
	isPlaying = false;
	emit changePlaying(isPlaying);

    //Fade out music
    BASS_ChannelSlideAttribute(channel, BASS_ATTRIB_VOL, 0.f, 750);

    //After fade out call function to pause
    BASS_ChannelSetSync(channel, BASS_SYNC_SLIDE | BASS_SYNC_ONETIME, 0.f, &PauseAfterFadeOut, 0);
    //playing = false;
}

void Player::nextSong()
{
    playlist->nextSong(TRUE, FALSE);
}

void Player::prevSong()
{
    playlist->prevSong(TRUE,FALSE);
}

bool Player::getPlaying()
{
    return isPlaying;
}

void Player::setPosition(int cur)
{
    if (isChangingSong == false)
        BASS_ChannelSetPosition(channel, BASS_ChannelSeconds2Bytes(channel, cur), BASS_POS_BYTE);
}

void Player::signalUpdate()
{
    emit posChanged(BASS_ChannelBytes2Seconds(channel, BASS_ChannelGetPosition(channel, BASS_POS_BYTE)));

}
