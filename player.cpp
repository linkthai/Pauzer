#include "player.h"

bool endOfPlayback;
bool pauseAllowed;
Player* Player::player = NULL;

Player::Player(QObject *parent) :
    QObject(parent)
{
    if (!BASS_Init(-1, 44100, 0, NULL, NULL))
        qDebug() << "Cannot initialize device";

    t = new QTimer(this);
    t->start(100);
    connect(t, SIGNAL(timeout()), this, SLOT(signalUpdate()));
    connect(t, SIGNAL(timeout()), this, SLOT(checkEndPlayback()));

    //endOfMusic = true;

	isPlaying = false;
    isShuffling = false;
    isChangingSong = false;
    isLocked = false;
    endOfPlayback = false;
    pauseAllowed = false;

    isRepeating = false;
    volume = 1.0;

    playlist = NULL;

    emit(posChanged(0));
}

Player::~Player()
{
    if (t != NULL)
        delete t;
}

void Player::changeToPlaylist(Playlist *_playlist, bool playFirstSong, bool masterReset)
{
    if (!masterReset)
        removeCurrentPlaylist();

    playlist = _playlist;

    connect(playlist, SIGNAL(changeCurrentSong(int)), this, SLOT(changeToSong(int)));
    connect(playlist, SIGNAL(nextPlaylist()), this, SIGNAL(nextPlaylist()));
    connect(playlist, SIGNAL(prevPlaylist()), this, SIGNAL(prevPlaylist()));

    if (playFirstSong)
        playlist->playFirstSong(isShuffling);
    else
        playlist->playLastSong(isShuffling);
}

void Player::changeToSong(int songNum)
{
    isChangingSong = true;
    QString filename;

    filename = Manager::master.Get(songNum);

    BASS_ChannelRemoveSync(channel, BASS_SYNC_END);

    BASS_ChannelStop(channel);
    BASS_StreamFree(channel);

    channel = BASS_StreamCreateFile(false, reinterpret_cast<const wchar_t*>(filename.constData()), 0, 0, NULL);   

    BASS_ChannelSetAttribute(channel, BASS_ATTRIB_VOL, volume);

    BASS_ChannelSetSync(channel, BASS_SYNC_END, 0, &EndOfPlayback, 0);

    QString title, artist, album;
    TagLib::MPEG::File f( reinterpret_cast<const wchar_t*>(filename.constData()) );
    if (!f.isValid())
        return;

    title = TStringToQString(f.tag()->title());
    artist = TStringToQString(f.tag()->artist());
    album = TStringToQString(f.tag()->album());

    Manager::CheckSongInfo(title, artist, album, filename);

    emit songTitle(title);
    emit songArtist(artist);
    emit songAlbum(album);

    TagLib::ID3v2::Tag *tag = f.ID3v2Tag();

    TagLib::ID3v2::FrameList framelist = tag->frameList("APIC");

    if (framelist.isEmpty())
    {
        QPixmap pixMap(":/resources/cover.png");
        emit songCover(pixMap);
    }
    else
    {
        TagLib::ID3v2::AttachedPictureFrame *pic = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(framelist.front());
        if (pic)
        {
            QImage coverArt;
            coverArt.loadFromData((const uchar *)pic->picture().data(), pic->picture().size());

            QPixmap pixMap = QPixmap::fromImage(coverArt);
            emit songCover(pixMap);

        }
        else
        {
            QPixmap pixMap(":/resources/cover.png");
            emit songCover(pixMap);
        }
    }

    emit songLength(BASS_ChannelBytes2Seconds(channel, BASS_ChannelGetLength(channel, BASS_POS_BYTE)));
    BASS_ChannelSetPosition(channel, 0, BASS_POS_BYTE);
    signalUpdate();

    if (isPlaying)
    {
        BASS_ChannelPlay(channel, false);
    }
    else
    {
        BASS_ChannelPause(channel);
    }

    isChangingSong = false;
}

void Player::setShuffle(bool state)
{
    if (state)
    {
        isShuffling = true;
        if (playlist)
            playlist->reShuffle(true);
    }
    else
    {
        isShuffling = false;
    }
}

void Player::setRepeat(bool state)
{
    if (state)
    {
        isRepeating = true;
    }
    else
    {
        isRepeating = false;
    }
}

void Player::setLock(bool state)
{
    if (state)
    {
        isLocked = true;
    }
    else
    {
        isLocked = false;
    }
}

void Player::setVolume(float vol)
{
    if (vol < 0 || vol > 1)
        return;

    volume = vol;

    if (BASS_ChannelIsActive(channel) == BASS_ACTIVE_STOPPED)
        return;

    BASS_ChannelSlideAttribute(
        channel,
        BASS_ATTRIB_VOL,
        volume,
        500
    );
}

float Player::getVolume()
{
    float vol;

    BASS_ChannelGetAttribute(
        channel,
        BASS_ATTRIB_VOL,
        &vol
    );

    return vol;
}

Playlist *Player::getPlaylist()
{
    return playlist;
}

void Player::checkEndPlayback()
{
    if (endOfPlayback)
    {
        if (isRepeating)
        {
            setPosition(0);
            BASS_ChannelPlay(channel, false);
        }
        else
        {
            this->nextSong();
        }
        endOfPlayback = false;
    }
}

void Player::removeCurrentPlaylist()
{
    if (playlist != NULL && playlist->getInit())
    {
        playlist->disconnect();
    }
}

bool Player::getShuffle()
{
    return isShuffling;
}

void CALLBACK PauseAfterFadeOut(HSYNC handle, DWORD channel, DWORD data, void *user)
{
    if (pauseAllowed)
    {
        BASS_ChannelPause(channel);
    }
}

void CALLBACK EndOfPlayback(HSYNC handle, DWORD channel, DWORD data, void *user)
{
    endOfPlayback = true;
}

void Player::play()
{    
    pauseAllowed = false;

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
    BASS_ChannelSlideAttribute(channel, BASS_ATTRIB_VOL, volume, 500);
}

void Player::pause()
{
    pauseAllowed = true;
	isPlaying = false;
    emit changePlaying(isPlaying);

    //Fade out music
    BASS_ChannelSlideAttribute(channel, BASS_ATTRIB_VOL, 0.f, 500);

    //After fade out call function to pause
    BASS_ChannelSetSync(channel, BASS_SYNC_SLIDE | BASS_SYNC_ONETIME, 0.f, &PauseAfterFadeOut, 0);
    //playing = false;
}

void Player::nextSong()
{
    if (playlist)
    {
        BASS_ChannelStop(channel);
        playlist->nextSong(isShuffling, isLocked);
    }
}

void Player::prevSong()
{
    if (playlist)
    {
        BASS_ChannelStop(channel);
        playlist->prevSong(isShuffling, isLocked);
    }
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
