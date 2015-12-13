#include "minipauzer.h"
#include "ui_minipauzer.h"

MiniPauzer::MiniPauzer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MiniPauzer)
{
    ui->setupUi(this);
    ui->coverArt->setScaledContents(true);
    QPixmap pixmap(":/resources/cover.png");
    ui->coverArt->setPixmap(pixmap);

    player = new Player(this);
    detector = new AutoDetector(this);

	buttonPlayClickTimer = new QTimer(this);
	buttonPlayClickTimer->setSingleShot(true);
	connect(buttonPlayClickTimer, SIGNAL(timeout()), this, SLOT(releaseButtonPlay()));

    detector->start();
    if (ui->btn_AutoDetector->isChecked())
        detector->resume();
    else
        detector->start();

	//Playing signals
    connect(player, SIGNAL(changePlaying(bool)), ui->btn_Play, SLOT(setChecked(bool)));

    //connect slider and stream
    connect(player, SIGNAL(posChanged(int)), ui->sliderBar, SLOT(setCurrentPos(int)));
    connect(ui->sliderBar, SIGNAL(valueChanged(int)), player, SLOT(setPosition(int)));

    //New song signals
    connect(player, SIGNAL(songLength(int)), ui->sliderBar, SLOT(setMaxLength(int)));
    connect(player, SIGNAL(songLength(int)), this, SLOT(updateLabelMaxLen(int)));
    connect(player, SIGNAL(songTitle(QString)), ui->lbl_Title, SLOT(setText(QString)));
    connect(player, SIGNAL(songArtist(QString)), ui->lbl_Artist, SLOT(setText(QString)));
    connect(player, SIGNAL(songAlbum(QString)), ui->lbl_Album, SLOT(setText(QString)));
    connect(player, SIGNAL(songCover(QPixmap)), ui->coverArt, SLOT(setPixmap(QPixmap)));
    connect(player, SIGNAL(endOfPlaylistNoRepeat()), this, SLOT(endOfPlaylistStop()));

    //connect current time label
    connect(player, SIGNAL(posChanged(int)), this, SLOT(updateLabelCurTime(int)));

    //emit audio detection
    connect(detector, SIGNAL(audioDetected(int)), this, SLOT(on_detected_audio(int)));

	isPlaying = false;
    isDetectorOn = false;
	isManuallyPlayed = false;
	isAutoPauseAllowed = false;

	isButtonPlayClickAllowed = true;

    qsrand(QTime::currentTime().second());
}

MiniPauzer::~MiniPauzer()
{
    delete ui;
    delete player;
    detector->exit();
    delete detector;
	delete buttonPlayClickTimer;
}

void MiniPauzer::on_btn_Play_clicked()
{
	if (isButtonPlayClickAllowed == true)
	{
		isButtonPlayClickAllowed = false;

		buttonPlayClickTimer->start(500);

		if (player->getPlaying() == false)
		{
			isManuallyPlayed = true;
			isDetectorOn = true;
			player->play();
		}
		else
		{
			isDetectorOn = false;
			player->pause();
		}
	}
}

void MiniPauzer::on_sliderBar_sliderPressed()
{
    ui->sliderBar->setDragging(true);
}

void MiniPauzer::on_sliderBar_sliderReleased()
{
    ui->sliderBar->setDragging(false);
}

void MiniPauzer::updateLabelCurTime(int time)
{
    int hour = time / 3600;
    time = time % 3600;
    int minute = time / 60;
    time = time % 60;
    int sec = time;

    QTime curTime;
    curTime.setHMS(hour, minute, sec);
    QString text;

    if (hour == 0)
        text = curTime.toString("m:ss");
    else
        text = curTime.toString("h:mm:ss");

    ui->lbl_CurTime->setText(text);
}

void MiniPauzer::updateLabelMaxLen(int legth)
{
    int hour = legth / 3600;
    legth = legth % 3600;
    int minute = legth / 60;
    legth = legth % 60;
    int sec = legth;

    QTime maxLen;
    maxLen.setHMS(hour, minute, sec);
    QString text;

    if (hour == 0)
        text = maxLen.toString("m:ss");
    else
        text = maxLen.toString("h:mm:ss");

    ui->lbl_songLen->setText(text);
}

void MiniPauzer::on_btn_ChooseFolders_clicked()
{
    FolderDialog *dialog = new FolderDialog();
    dialog->setModal(true);

    connect(dialog, SIGNAL(createdFolderList(QStringList)), this, SLOT(getFolderList(QStringList)));

    dialog->exec();
    delete dialog;
}

void MiniPauzer::getFolderList(QStringList list)
{
    Manager::CreateMaster(list);

    player->changeToPlaylist(0);
}

void MiniPauzer::on_detected_audio(int audio_num)
{
	if (isDetectorOn)
	{
		if (audio_num <= 1 && isAutoPauseAllowed == false)
			isAutoPauseAllowed = true;

		if (isManuallyPlayed == true && isAutoPauseAllowed == true)
		{
			if (audio_num > 1)
				isAutoPauseAllowed = false;
			isManuallyPlayed = false;
		}

		if (player->getPlaying() == true)
		{
			if (audio_num > 1 && isAutoPauseAllowed == true)
			{
				player->pause();
			}
		}
		else
		{
			if (audio_num == 0)
			{
				player->play();
			}
		}
	}

}

void MiniPauzer::releaseButtonPlay()
{
	isButtonPlayClickAllowed = true;
}

void MiniPauzer::on_btn_Prev_clicked()
{
    player->prevSong();
}

void MiniPauzer::on_btn_Next_clicked()
{
    player->nextSong();
}

void MiniPauzer::on_btn_Shuffle_stateChanged(int state)
{
    if (state == 0)
    {
        player->setShuffle(false);
    }
    else
    {
        player->setShuffle(true);
    }
}

void MiniPauzer::on_btn_Repeat_stateChanged(int state)
{
    player->setRepeat(state);
}

void MiniPauzer::endOfPlaylistStop()
{
    isDetectorOn = false;
    player->pause();
}


void MiniPauzer::on_btn_AutoDetector_stateChanged(int state)
{
    if (state == 0)
    {
        detector->pause();
    }
    else
    {
        detector->resume();
    }
}
