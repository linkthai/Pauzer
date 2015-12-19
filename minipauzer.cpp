#include "minipauzer.h"
#include "ui_minipauzer.h"

MiniPauzer::MiniPauzer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MiniPauzer)
{    
    player = Player::getInstance();
    detector = new AutoDetector(this);

    ui->setupUi(this);

    layoutSetup();

    //loading data to set up pauzer
    loadData();

    changeStyle();

	buttonPlayClickTimer = new QTimer(this);
	buttonPlayClickTimer->setSingleShot(true);
	connect(buttonPlayClickTimer, SIGNAL(timeout()), this, SLOT(releaseButtonPlay()));

    detector->start();
    if (ui->btn_AutoDetector->isChecked())
        detector->resume();
    else
        detector->pause();

    //Playing signals
    connect(player, SIGNAL(changePlaying(bool)), ui->btn_Play, SLOT(setChecked(bool)));

    //Drag signals
    connect(ui->titleBar, SIGNAL(titleBarDragged(QPoint)), this, SLOT(on_titleBar_dragged(QPoint)));

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

    //connect player and volume button
    connect(ui->btn_Volume, SIGNAL(volumeChanged(float)), player, SLOT(setVolume(float)));

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

void MiniPauzer::layoutSetup()
{
    //initialize layout
    mainGrid = new QVBoxLayout(this);

    grd_Player = new QGridLayout(this);
    grd_PlayerFull = new QGridLayout(this);
    grd_SongInfo = new QVBoxLayout(this);
    grd_SongProgress = new QHBoxLayout(this);
    grd_SongButton = new QHBoxLayout(this);
    grd_SongSetting = new QHBoxLayout(this);

    grbx_Player = new QGroupBox(this);
    grbx_PlayerFull = new QGroupBox(this);
    grbx_SongInfo = new QGroupBox(this);
    grbx_SongProgress = new QGroupBox(this);
    grbx_SongButton = new QGroupBox(this);
    grbx_SongSetting = new QGroupBox(this);

    grd_Manager = new QHBoxLayout(this);
    grbx_Manager = new QGroupBox(this);

    this->setLayout(mainGrid);
    mainGrid->setMargin(0);
    mainGrid->setContentsMargins(0, 0, 0, 0);
    mainGrid->setSpacing(0);

    //titlebar layout
    const int title_button_size = 65;
    const int title_bar_height = 40;

    ui->titleBar->setLayout(ui->rightTitleBarLayout);
    ui->rightTitleBarLayout->setContentsMargins(0, 0, 0, 0);
    ui->btn_Close->setMinimumWidth(title_button_size);
    ui->btn_Close->setMaximumWidth(title_button_size);
    ui->btn_Maximize->setMinimumWidth(title_button_size);
    ui->btn_Maximize->setMaximumWidth(title_button_size);
    ui->btn_Minimize->setMinimumWidth(title_button_size);
    ui->btn_Minimize->setMaximumWidth(title_button_size);
    ui->btn_Minimize->setMinimumWidth(title_button_size);
    ui->btn_Minimize->setMaximumWidth(title_button_size);
    ui->btn_Up->setMinimumWidth(title_button_size);
    ui->btn_Up->setMaximumWidth(title_button_size);
    ui->btn_Down->setMinimumWidth(title_button_size);
    ui->btn_Down->setMaximumWidth(title_button_size);

    ui->rightTitleBarLayout->setAlignment(Qt::AlignRight);
    ui->titleBar->setMaximumHeight(title_bar_height);
    ui->titleBar->setMinimumHeight(title_bar_height);

    mainGrid->addWidget(ui->titleBar, 0, 0);
    mainGrid->setAlignment(ui->titleBar, Qt::AlignTop);

    //Manager layout
    grbx_Manager->setLayout(grd_Manager);
    grbx_Manager->setStyleSheet("border:0; background-color: #bdc3c7");
    mainGrid->addWidget(grbx_Manager, Qt::AlignTop);

    //Player layout
    grbx_Player->setLayout(grd_Player);
    grbx_Player->setStyleSheet("border:0;");
    grbx_Player->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    grbx_PlayerFull->setLayout(grd_PlayerFull);
    grbx_PlayerFull->setStyleSheet("border:0;");
    grbx_PlayerFull->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

    grbx_SongInfo->setLayout(grd_SongInfo);
    grbx_SongInfo->setStyleSheet("border:0;");
    grbx_SongInfo->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    grbx_SongProgress->setLayout(grd_SongProgress);
    grbx_SongProgress->setStyleSheet("border:0;");
    grbx_SongProgress->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    grbx_SongButton->setLayout(grd_SongButton);
    grbx_SongButton->setStyleSheet("border:0;");
    grbx_SongButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    grbx_SongSetting->setLayout(grd_SongSetting);
    grbx_SongSetting->setStyleSheet("border:0;");
    grbx_SongSetting->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

    mainGrid->addWidget(grbx_Player);
    mainGrid->setAlignment(grd_Player, Qt::AlignBottom);
    mainGrid->addWidget(grbx_PlayerFull);
    mainGrid->setAlignment(grd_PlayerFull, Qt::AlignBottom);
    grd_Player->setSpacing(0);
    grd_Player->setContentsMargins(0, 0, 0, 0);
    grd_PlayerFull->setSpacing(0);
    grd_PlayerFull->setContentsMargins(0, 0, 0, 0);

    grd_SongInfo->addWidget(ui->lbl_Title, 2);
    grd_SongInfo->addWidget(ui->lbl_Artist, 1);
    grd_SongInfo->addWidget(ui->lbl_Album, 1);
    ui->lbl_Title->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    grd_SongProgress->addWidget(ui->lbl_CurTime);
    grd_SongProgress->addWidget(ui->sliderBar);
    grd_SongProgress->addWidget(ui->lbl_songLen);

    grd_SongButton->addWidget(ui->btn_Prev);
    grd_SongButton->addWidget(ui->btn_Play);
    grd_SongButton->addWidget(ui->btn_Next);

    grd_SongSetting->setDirection(QBoxLayout::RightToLeft);
    grd_SongSetting->setAlignment(Qt::AlignRight);
    grd_SongSetting->addWidget(ui->btn_Volume, Qt::AlignRight);
    grd_SongSetting->addWidget(ui->btn_Repeat, Qt::AlignRight);
    grd_SongSetting->addWidget(ui->btn_Shuffle, Qt::AlignRight);
    grd_SongSetting->addWidget(ui->btn_AutoDetector, Qt::AlignRight);
    grd_SongSetting->addWidget(ui->btn_ChooseFolders, Qt::AlignRight);


}

void MiniPauzer::loadData()
{
    //code for load some data
    //get previous state of pauzer
    //pretend we did that
    changeState(State::MINI);

    ui->btn_AutoDetector->setChecked(true);

    float volume = 1;
    ui->btn_Volume->setVolume(1);
}

void MiniPauzer::changeState(MiniPauzer::State _state)
{
    state = _state;

    //Change layout
    if (state == State::FULL)
    {        
        ui->btn_Down->show();
        ui->btn_Maximize->show();
        ui->btn_Up->hide();

        grbx_Manager->show();

        grbx_Player->hide();
        grbx_PlayerFull->show();

        this->resize(1000, 800);

        grbx_PlayerFull->setFixedHeight(150);

        //add cover art
        grd_PlayerFull->addWidget(ui->coverArt, 0, 0, -1, 1);

        ui->coverArt->setMargin(15);
        ui->coverArt->setFixedHeight(grbx_PlayerFull->height());
        ui->coverArt->setFixedWidth(ui->coverArt->geometry().height());

        //add progress bar
        grd_PlayerFull->addWidget(grbx_SongProgress, 0, 1, 1, -1);
        grd_SongProgress->setContentsMargins(0, 15, 15, 5);
        grd_SongProgress->setSpacing(15);

        //add title, artist and album labels
        grbx_SongInfo->setMinimumWidth(300);
        grd_PlayerFull->addWidget(grbx_SongInfo, 1, 1, -1, 1);
        grd_SongInfo->setContentsMargins(0, 0, 15, 18);
        ui->lbl_Title->setContentsMargins(0, 0, 0, 0);
        ui->lbl_Artist->setContentsMargins(0, 0, 0, 0);
        grd_SongInfo->setSpacing(10);

        //add player button
        grd_PlayerFull->addWidget(grbx_SongButton, 1, 2, 1, 1);
        grbx_SongButton->setFixedWidth(400);
        grd_PlayerFull->setAlignment(grbx_SongButton, Qt::AlignCenter);
        grd_SongButton->setSpacing(20);

        //add player setting
        grd_PlayerFull->addWidget(grbx_SongSetting, 2, 2, 1, 1);
        grd_SongSetting->setMargin(15);
        grd_SongSetting->setSpacing(25);

    }
    else //state == State::MINI
    {

        ui->btn_Up->show();
        ui->btn_Maximize->hide();
        ui->btn_Down->hide();

        grbx_Manager->hide();

        grbx_PlayerFull->hide();
        grbx_Player->show();

        grbx_Player->setFixedHeight(210);

        //add cover art
        grd_Player->addWidget(ui->coverArt, 0, 0, -1, 1);

        ui->coverArt->setMargin(0);
        ui->coverArt->setFixedHeight(grbx_Player->height());
        ui->coverArt->setFixedWidth(ui->coverArt->geometry().height());

        //add title, artist and album labels
        grd_Player->addWidget(grbx_SongInfo, 0, 1, 1, -1);
        grd_SongInfo->setContentsMargins(15, 5, 15, 10);
        ui->lbl_Artist->setContentsMargins(0, -4, 0, 0);
        grd_SongInfo->setSpacing(0);


        //add progress bar
        grd_Player->addWidget(grbx_SongProgress, 1, 1, 1, -1);
        grd_SongProgress->setContentsMargins(15, 5, 15, 5);
        grd_SongProgress->setSpacing(10);

        //add player button
        grd_Player->addWidget(grbx_SongButton, 2, 1, 1, -1);
        grbx_SongButton->setFixedWidth(300);
        grd_Player->setAlignment(grbx_SongButton, Qt::AlignCenter);
        grd_SongButton->setContentsMargins(0, 5, 0, 5);
        grd_SongSetting->setSpacing(10);

        //add player setting
        grd_Player->addWidget(grbx_SongSetting, 3, 1, 1, -1);
        grd_SongSetting->setContentsMargins(15, 5, 20, 15);
        grd_SongSetting->setSpacing(20);

        this->resize(600, 250);

    }
}

void MiniPauzer::changeStyle()
{
    QRect rec = QApplication::desktop()->screenGeometry();
    //this->setWindowState(Qt::m);

    //this->resize(QSize(rec.width(), rec.height()));

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

    ui->btn_Up->setAttribute(Qt::WA_NoMousePropagation);
    ui->btn_Up->setIcon(QPixmap(":/icons/Up.png"));
    ui->btn_Up->setIconSize(QSize(15, 15));

    ui->btn_Down->setAttribute(Qt::WA_NoMousePropagation);
    ui->btn_Down->setIcon(QPixmap(":/icons/Down.png"));
    ui->btn_Down->setIconSize(QSize(15, 15));

    ui->coverArt->setScaledContents(true);
    QPixmap pixmap(":/resources/cover.png");
    ui->coverArt->setPixmap(pixmap);

    ui->lbl_Title->setMinimumHeight(20);
    ui->lbl_Artist->setMinimumHeight(18);
    ui->lbl_Album->setMinimumHeight(16);
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

void MiniPauzer::endOfPlaylistStop()
{
    isDetectorOn = false;
    player->pause();
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

void MiniPauzer::on_btn_Up_clicked()
{
    changeState(State::FULL);
}

void MiniPauzer::on_btn_Down_clicked()
{
    changeState(State::MINI);
}

void MiniPauzer::on_btn_Maximize_clicked()
{
    this->setWindowState(Qt::WindowFullScreen);
}

void MiniPauzer::on_btn_AutoDetector_toggled(bool checked)
{
    if (detector != NULL)
    {
        if (checked == 0)
        {
            detector->pause();
        }
        else
        {
            detector->resume();
        }
    }
}

void MiniPauzer::on_btn_Shuffle_toggled(bool checked)
{
    if (player != NULL)
    {
        ui->btn_Shuffle->setEnabled(false);
        if (checked == 0)
        {
            player->setShuffle(false);
        }
        else
        {
            player->setShuffle(true);
        }

        ui->btn_Shuffle->setEnabled(true);
    }
}

void MiniPauzer::on_btn_Repeat_toggled(bool checked)
{
    if (player != NULL)
    {
        player->setRepeat(checked);
    }
}

void MiniPauzer::on_btn_Volume_toggled(bool checked)
{
}
