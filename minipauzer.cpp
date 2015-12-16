#include "minipauzer.h"
#include "ui_minipauzer.h"

MiniPauzer::MiniPauzer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MiniPauzer)
{
    ui->setupUi(this);

    changeStyle();

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

    //Drag signals
    connect(ui->titleBar, SIGNAL(titleBarDragged(QPoint)), this, SLOT(on_titleBar_dragged(QPoint)));

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
    connect(player, SIGNAL(songCover(QPixmap)), this, SLOT(on_new_pixmap(QPixmap)));
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

void MiniPauzer::changeStyle()
{
    this->setStyleSheet("background-color: #282828;");
    ui->titleBar->setStyleSheet("background-color: #2d2d2d");

    ui->lbl_Title->setStyleSheet("color: white;");
    ui->lbl_Artist->setStyleSheet("color: white;");
    ui->lbl_Album->setStyleSheet("color: #e8e8e8;");
    ui->lbl_CurTime->setStyleSheet("color: #e8e8e8;");
    ui->lbl_songLen->setStyleSheet("color: #e8e8e8;");

    ui->btn_Minimize->setAttribute(Qt::WA_NoMousePropagation);
    ui->btn_Minimize->setIcon(QPixmap(":/icons/Minimize.png"));
    ui->btn_Minimize->setIconSize(QSize(15, 15));

    ui->btn_Maximize->setAttribute(Qt::WA_NoMousePropagation);
    ui->btn_Maximize->setIcon(QPixmap(":/icons/Maximize.png"));
    ui->btn_Maximize->setIconSize(QSize(15, 15));

    ui->btn_Close->setAttribute(Qt::WA_NoMousePropagation);
    ui->btn_Close->setIcon(QPixmap(":/icons/Close.png"));
    ui->btn_Close->setIconSize(QSize(15, 15));


    ui->coverArt->setScaledContents(true);
    QPixmap pixmap(":/resources/cover.png");
    ui->coverArt->setPixmap(pixmap);

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
    ui->btn_Shuffle->setEnabled(false);
    if (state == 0)
    {
        player->setShuffle(false);
    }
    else
    {
        player->setShuffle(true);
    }

    ui->btn_Shuffle->setEnabled(true);
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

void MiniPauzer::on_new_pixmap(const QPixmap &pixmap)
{
    ui->coverArt->clear();
    ui->coverArt->setPixmap(pixmap);
}

void MiniPauzer::on_btn_Close_clicked()
{
    this->close();
}

void MiniPauzer::on_btn_Minimize_clicked()
{
    this->setWindowState(Qt::WindowMinimized);

}

void MiniPauzer::on_titleBar_dragged(const QPoint &newPoint)
{
    this->move(newPoint);
}
