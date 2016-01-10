#include "minipauzer.h"
#include "ui_minipauzer.h"

MiniPauzer::MiniPauzer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MiniPauzer)
{
    this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint |
                         Qt::WindowMinimizeButtonHint);

    player = Player::getInstance();

    detector = new AutoDetector(this);
    creator = new LibraryCreator(this);

    queueModel = new PlaylistQueueModel(this);
    masterModel = new MasterModel(this);

    connect(masterModel, SIGNAL(playlistPlayed(QString,int)), queueModel, SLOT(playPlaylist(QString,int)));
    connect(masterModel, SIGNAL(playlistQueued(QString,int)), queueModel, SLOT(queuePlaylist(QString,int)));

    widget = new ProcessWidget(this);
    widget->setModal(true);

    connect(creator, SIGNAL(finished()), this, SLOT(processFinished()));
    connect(widget, SIGNAL(processCanceled()), this, SLOT(processTerminated()));

    ui->setupUi(this);

    layoutSetup();

    //connect player and volume button
    connect(ui->btn_Volume, SIGNAL(volumeChanged(float)), player, SLOT(setVolume(float)));

    //loading data to set up pauzer
    loadData();

    changeStyle();

    createMenu();

    detector->start();
    if (ui->btn_AutoDetector->isChecked())
        detector->resume();
    else
        detector->pause();

    //Playing signals
    connect(player, SIGNAL(changePlaying(bool)), ui->btn_Play, SLOT(setChecked(bool)));

    //Drag signals
    connect(ui->titleBar, SIGNAL(titleBarDragged(QPoint)), this, SLOT(titleBardragged(QPoint)));

    //connect slider and stream
    connect(player, SIGNAL(posChanged(int)), ui->sliderBar, SLOT(setCurrentPos(int)));
    connect(ui->sliderBar, SIGNAL(valueChanged(int)), player, SLOT(setPosition(int)));

    //New song signals
    connect(player, SIGNAL(songLength(int)), ui->sliderBar, SLOT(setMaxLength(int)));
    connect(player, SIGNAL(songLength(int)), this, SLOT(updateLabelMaxLen(int)));
    connect(player, SIGNAL(songTitle(QString)), ui->lbl_Title, SLOT(setText(QString)));
    connect(player, SIGNAL(songArtist(QString)), ui->lbl_Artist, SLOT(setText(QString)));
    connect(player, SIGNAL(songAlbum(QString)), ui->lbl_Album, SLOT(setText(QString)));
    connect(player, SIGNAL(songCover(QPixmap)), this, SLOT(setCoverArt(QPixmap)));
    connect(player, SIGNAL(endOfPlaylistNoRepeat()), this, SLOT(endOfPlaylistStop()));

    //connect current time label
    connect(player, SIGNAL(posChanged(int)), this, SLOT(updateLabelCurTime(int)));

    //emit audio detection
    connect(detector, SIGNAL(audioDetected(int)), this, SLOT(detectAudio(int)));

    //connect end Of Queue to pause button
    connect(queueModel, SIGNAL(switchPlayerState(bool)), this, SLOT(switchPlayerState(bool)));

    isDetectorOn = false;
    isProcessCanceled = false;

    qsrand(QTime::currentTime().msecsSinceStartOfDay());

    tableView->setModel(masterModel);

    if (Manager::LoadSongToMaster())
    {
        masterModel->initializeModel();
        queueModel->initializeModel();
    }

    changePanel(Panel::MASTER);
}

void MiniPauzer::createThumbBar()
{
    thumbnailToolBar = new QWinThumbnailToolBar(this);
    thumbnailToolBar->setWindow(this->windowHandle());

    playToolButton = new QWinThumbnailToolButton(thumbnailToolBar);
    playToolButton->setEnabled(true);
    playToolButton->setToolTip(tr("Play/Pause"));
    playToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(playToolButton, SIGNAL(clicked()), this, SLOT(on_btn_Play_clicked()));

    forwardToolButton = new QWinThumbnailToolButton(thumbnailToolBar);
    forwardToolButton->setEnabled(true);
    forwardToolButton->setToolTip(tr("Next track"));
    forwardToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    connect(forwardToolButton, SIGNAL(clicked()), this, SLOT(on_btn_Next_clicked()));

    backwardToolButton = new QWinThumbnailToolButton(thumbnailToolBar);
    backwardToolButton->setEnabled(true);
    backwardToolButton->setToolTip(tr("Previous track"));
    backwardToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    connect(backwardToolButton, SIGNAL(clicked()), this, SLOT(on_btn_Prev_clicked()));

    thumbnailToolBar->addButton(backwardToolButton);
    thumbnailToolBar->addButton(playToolButton);
    thumbnailToolBar->addButton(forwardToolButton);
}

MiniPauzer::~MiniPauzer()
{
    QMap<QString, float> list;

    list.insert("FullSizeWidth", FullSize.width());
    list.insert("FullSizeHeight", FullSize.height());

    QPoint pos = this->mapToGlobal(MiniPos);

    list.insert("MiniPosX", MiniPos.x());
    list.insert("MiniPosY", MiniPos.y());

    pos = this->mapToGlobal(FullPos);

    list.insert("FullPosX", FullPos.x());
    list.insert("FullPosY", FullPos.y());

    if (state == State::FULL)
    {
        list.insert("State_Full", 1);
    }
    else
    {
        list.insert("State_Full", 0);
    }

    if (windowState() == Qt::WindowMaximized)
    {
        list.insert("Maximized", 1);
    }
    else
    {
        list.insert("Maximized", 0);
    }

    list.insert("Volume", ui->btn_Volume->getVolume());

    if (ui->btn_AutoDetector->isChecked())
    {
        list.insert("Detector", 1);
    }
    else
    {
        list.insert("Detector", 0);
    }

    if (ui->btn_Shuffle->isChecked())
    {
        list.insert("Shuffle", 1);
    }
    else
    {
        list.insert("Shuffle", 0);
    }

    if (ui->btn_Repeat->isChecked())
    {
        list.insert("Repeat", 1);
    }
    else
    {
        list.insert("Repeat", 0);
    }

    if (ui->btn_Lock->isChecked())
    {
        list.insert("Lock", 1);
    }
    else
    {
        list.insert("Lock", 0);
    }

    Manager::setting.setList(list);
    Manager::setting.WriteSetting();

    delete ui;
    delete player;
    delete detector;
    delete queueModel;
    delete widget;
    delete buttonPlayClickTimer;
}

void MiniPauzer::layoutSetup()
{
    QRect rec = QApplication::desktop()->screenGeometry();

    //initialize layout
    mainGrid = new QVBoxLayout(this);

    titleBarLayout = new QHBoxLayout();
    rightTitleBarLayout = new QHBoxLayout();
    leftTitleBarLayout = new QHBoxLayout();

    grd_Player = new QGridLayout();
    grd_PlayerFull = new QGridLayout();
    grd_SongInfo = new QVBoxLayout();
    grd_SongProgress = new QHBoxLayout();
    grd_SongButton = new QHBoxLayout();
    grd_SongSetting = new QHBoxLayout();

    grbx_Player = new QGroupBox(this);
    grbx_PlayerFull = new QGroupBox(this);
    grbx_SongInfo = new QGroupBox(this);
    grbx_SongProgress = new QGroupBox(this);
    grbx_SongButton = new QGroupBox(this);
    grbx_SongSetting = new QGroupBox(this);

    grd_Manager = new QHBoxLayout();
    grbx_Manager = new QGroupBox(this);

    grd_LeftPanel = new QVBoxLayout();
    grbx_LeftPanel = new QGroupBox(this);

    grd_MidPanel = new QVBoxLayout();
    grbx_MidPanel = new QGroupBox(this);

    grd_PanelInfo = new QGridLayout();
    grbx_PanelInfo = new QGroupBox(this);

    tableView = new MasterTableView();

    queuePanel = new PlaylistQueueWidget(queueModel);

    this->setLayout(mainGrid);
    mainGrid->setMargin(0);
    mainGrid->setContentsMargins(0, 0, 0, 0);
    mainGrid->setSpacing(0);

    grip = new QSizeGrip(this);

    //titlebar layout
    const int title_button_size = 65;
    const int title_bar_height = 40;

    ui->titleBar->setLayout(titleBarLayout);
    ui->titleBar->setContentsMargins(0, 0, 0, 0);

    ui->btn_Close->setMinimumWidth(title_button_size);
    ui->btn_Close->setMaximumWidth(title_button_size);
    ui->btn_Maximize->setMinimumWidth(title_button_size);
    ui->btn_Maximize->setMaximumWidth(title_button_size);
    ui->btn_Minimize->setMinimumWidth(title_button_size);
    ui->btn_Minimize->setMaximumWidth(title_button_size);
    ui->btn_Up->setMinimumWidth(title_button_size);
    ui->btn_Up->setMaximumWidth(title_button_size);
    ui->btn_Down->setMinimumWidth(title_button_size);
    ui->btn_Down->setMaximumWidth(title_button_size);
    ui->btn_Menu->setMinimumWidth(title_button_size);
    ui->btn_Menu->setMaximumWidth(title_button_size);
    ui->btn_Menu->setMinimumHeight(title_bar_height);

    rightTitleBarLayout->addWidget(ui->btn_Minimize);
    rightTitleBarLayout->addWidget(ui->btn_Up);
    rightTitleBarLayout->addWidget(ui->btn_Down);
    rightTitleBarLayout->addWidget(ui->btn_Maximize);
    rightTitleBarLayout->addWidget(ui->btn_Close);
    rightTitleBarLayout->setAlignment(Qt::AlignRight);
    rightTitleBarLayout->setContentsMargins(0, 0, 0, 0);
    rightTitleBarLayout->setMargin(0);

    leftTitleBarLayout->addWidget(ui->btn_Menu);
    leftTitleBarLayout->setAlignment(Qt::AlignLeft);
    leftTitleBarLayout->setContentsMargins(0, 0, 0, 0);
    leftTitleBarLayout->setMargin(0);

    titleBarLayout->addLayout(leftTitleBarLayout);
    titleBarLayout->addLayout(rightTitleBarLayout);
    titleBarLayout->setContentsMargins(0, 0, 0, 0);
    titleBarLayout->setMargin(0);

    ui->titleBar->setMaximumHeight(title_bar_height);
    ui->titleBar->setMinimumHeight(title_bar_height);

    mainGrid->addWidget(ui->titleBar, 0, 0);
    mainGrid->setAlignment(ui->titleBar, Qt::AlignTop);

    //Manager layout
    grbx_Manager->setLayout(grd_Manager);
    grbx_Manager->setStyleSheet("border:0;");
    grbx_Manager->setContentsMargins(0, 0, 0, 0);
    grd_Manager->setMargin(0);
    grd_Manager->setContentsMargins(0, 0, 0, 0);
    mainGrid->addWidget(grbx_Manager, Qt::AlignTop);

    grd_Manager->setDirection(QBoxLayout::LeftToRight);
    grd_Manager->setSpacing(0);

    //---------LeftPanel--------------------
    grd_Manager->addWidget(grbx_LeftPanel);
    grd_Manager->setAlignment(grbx_LeftPanel, Qt::AlignLeft);
    grbx_LeftPanel->setLayout(grd_LeftPanel);
    grbx_LeftPanel->setFixedWidth(220);
    grbx_LeftPanel->setContentsMargins(0, 0, 0, 0);
    grd_LeftPanel->setMargin(0);
    grd_LeftPanel->setSpacing(10);

    grd_LeftPanel->setDirection(QBoxLayout::TopToBottom);

    grd_LeftPanel->addSpacing(20);
    grd_LeftPanel->addWidget(ui->lbl_Icon, 0, Qt::AlignTop | Qt::AlignCenter);
    ui->lbl_Icon->setFixedSize(150, 150);
    grd_LeftPanel->addSpacing(10);

    ui->spr_Icon->setFrameShape(QFrame::HLine);
    ui->spr_Icon->setFrameShadow(QFrame::Sunken);
    ui->spr_Icon->setFixedHeight(1);
    ui->spr_Icon->setContentsMargins(10, 0, 10, 0);
    grd_LeftPanel->addWidget(ui->spr_Icon);

    grd_LeftPanel->addWidget(ui->btn_Master);
    ui->btn_Master->setFixedHeight(50);
    grd_LeftPanel->addStretch(3);

    //----------MidPanel-----------------------
    grd_Manager->addWidget(grbx_MidPanel);
    grbx_MidPanel->setLayout(grd_MidPanel);
    grbx_MidPanel->setContentsMargins(0, 0, 0 , 0);
    grd_MidPanel->setMargin(0);

    grd_MidPanel->addWidget(grbx_PanelInfo);
    grbx_PanelInfo->setLayout(grd_PanelInfo);
    grbx_PanelInfo->setFixedHeight(100);
    grbx_PanelInfo->setStyleSheet("background-color: transparent");

    //Panel Info
    grd_PanelInfo->addWidget(ui->btn_Albums, 0, 0, -1, 1);
    grd_PanelInfo->addWidget(ui->btn_Artists, 0, 1, -1, 1);
    grd_PanelInfo->addWidget(ui->btn_Songs, 0, 2, -1, 1);

    grd_MidPanel->addWidget(tableView);

    //------------PlaylistQueuePanel------------
    grd_Manager->addWidget(queuePanel);
    grd_Manager->setAlignment(grbx_LeftPanel, Qt::AlignRight);
    queuePanel->setFixedWidth(300);
    queuePanel->setContentsMargins(0, 0, 0, 0);

    queuePanel->setStyleSheet("QWidget {"
                              "border: 0px;"
                              "background-color: #262626;"
                              "}");

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
    grd_SongSetting->addWidget(ui->btn_Lock, Qt::AlignRight);
    grd_SongSetting->addWidget(ui->btn_Shuffle, Qt::AlignRight);
    grd_SongSetting->addWidget(ui->btn_AutoDetector, Qt::AlignRight);

    grd_PlayerFull->setMargin(5);
    grd_PlayerFull->addWidget(grip, 2, 3, 1, 1);
    grd_PlayerFull->setAlignment(grip, Qt::AlignBottom | Qt::AlignRight);

}

void MiniPauzer::changePanel(MiniPauzer::Panel _panel)
{    
    panel = _panel;

    switch(panel)
    {
    case Panel::MASTER:
        ui->btn_Master->setChecked(true);
        ui->btn_Albums->setChecked(true);
        break;
    }
}

void MiniPauzer::loadData()
{
    //code for load some data
    //get previous state of pauzer
    //pretend we did that

    Manager::setting.ReadSetting();
    QMap<QString, float> list = Manager::setting.list();

    QRect rec = QApplication::desktop()->screenGeometry();

    //set size for mini pauzer
    miniWidth = 580;
    miniHeight = 225;

    FullSize.setWidth(list.value("FullSizeWidth", rec.width() * 7 / 10));
    FullSize.setHeight(list.value("FullSizeHeight", rec.height() * 8 / 10));

    MiniPos.setX(list.value("MiniPosX", (rec.width() - miniWidth) / 2));
    MiniPos.setY(list.value("MiniPosY", (rec.height() - miniHeight) / 2));

    FullPos.setX(list.value("FullPosX", (rec.width() - FullSize.width()) / 2));
    FullPos.setY(list.value("FullPosY", (rec.height() - FullSize.height()) / 2));

    if (list.value("State_Full") == 1)
    {
        if (list.value("Maximized") == 1)
            isFullScreen = true;
        else
            isFullScreen = false;

        changeState(State::FULL);
    }
    else
    {
        changeState(State::MINI);
    }

    ui->btn_AutoDetector->setChecked(true);
    ui->btn_Lock->setChecked(false);

    float volume = Manager::setting.list().value("Volume");
    ui->btn_Volume->setVolume(volume);

    if (Manager::setting.list().value("Detector") == 0)
    {
        ui->btn_AutoDetector->setChecked(false);
    }
    else
    {
        ui->btn_AutoDetector->setChecked(true);
    }

    if (Manager::setting.list().value("Shuffle") == 0)
    {
        ui->btn_Shuffle->setChecked(false);
    }
    else
    {
        ui->btn_Shuffle->setChecked(true);
    }

    if (Manager::setting.list().value("Repeat") == 0)
    {
        ui->btn_Repeat->setChecked(false);
    }
    else
    {
        ui->btn_Repeat->setChecked(true);
    }

    if (Manager::setting.list().value("Lock") == 0)
    {
        ui->btn_Lock->setChecked(false);
    }
    else
    {
        ui->btn_Lock->setChecked(true);
    }
}

void MiniPauzer::changeState(MiniPauzer::State _state)
{
    QPoint p = this->pos();

    state = _state;

    //Change layout
    if (state == State::FULL)
    {
        if (p.x() != 0 && p.y() != 0)
        {
            MiniPos.setX(p.x());
            MiniPos.setY(p.y());
        }

        this->move(FullPos);
        this->resize(FullSize.width(), FullSize.height());

        ui->btn_Down->show();
        ui->btn_Maximize->show();
        ui->btn_Up->hide();

        grbx_Manager->show();

        grbx_Player->hide();
        grbx_PlayerFull->show();

        grbx_PlayerFull->setFixedHeight(150);

        //add cover art
        grd_PlayerFull->addWidget(ui->coverArt, 0, 0, -1, 1);

        ui->coverArt->setContentsMargins(10, 10, 20, 20);
        ui->coverArt->setFixedHeight(grbx_PlayerFull->height());
        ui->coverArt->setFixedWidth(ui->coverArt->geometry().height());

        //add progress bar
        grd_PlayerFull->addWidget(grbx_SongProgress, 0, 1, 1, -1);
        grd_SongProgress->setContentsMargins(0, 15, 15, 5);
        grd_SongProgress->setSpacing(15);

        //add title, artist and album labels
        grbx_SongInfo->setFixedWidth(400);
        grd_PlayerFull->addWidget(grbx_SongInfo, 1, 1, -1, 1);
        grd_SongInfo->setContentsMargins(0, 0, 15, 5);
        ui->lbl_Title->setContentsMargins(0, -5, 0, 0);
        grd_SongInfo->setSpacing(5);
        ui->lbl_Title->setFont(QFont("Segoe UI", 18));
        ui->lbl_Artist->setFont(QFont("Segoe UI", 10));
        ui->lbl_Album->setFont(QFont("Segoe UI", 9));

        //add player button
        grd_PlayerFull->addWidget(grbx_SongButton, 1, 2, 1, 1);
        grbx_SongButton->setFixedWidth(400);
        grd_PlayerFull->setAlignment(grbx_SongButton, Qt::AlignCenter);
        grd_SongButton->setSpacing(15);
        ui->btn_Prev->setFixedSize(25, 25);
        ui->btn_Next->setFixedSize(25, 25);
        ui->btn_Play->setFixedSize(30, 30);

        //add player setting
        grd_PlayerFull->addWidget(grbx_SongSetting, 2, 2, 1, 1);
        grd_SongSetting->setMargin(15);
        grd_SongSetting->setSpacing(25);
        ui->btn_Volume->setFixedSize(QSize(20, 20));
        ui->btn_Repeat->setFixedSize(QSize(20, 20));
        ui->btn_Shuffle->setFixedSize(QSize(20, 20));
        ui->btn_Lock->setFixedSize(QSize(20, 20));
        ui->btn_AutoDetector->setFixedSize(QSize(20, 20));

        if (isFullScreen)
            this->setWindowState(this->windowState() ^ Qt::WindowMaximized);

    }
    else //state == State::MINI
    {
        if (p.x() != 0 && p.y() != 0)
        {
            FullPos.setX(p.x());
            FullPos.setY(p.y());
        }

        if (isFullScreen)
            this->setWindowState(this->windowState() ^ Qt::WindowMaximized);

        ui->btn_Up->show();
        ui->btn_Maximize->hide();
        ui->btn_Down->hide();

        grbx_Manager->hide();

        grbx_PlayerFull->hide();
        grbx_Player->show();

        grbx_Player->setFixedHeight(miniHeight - ui->titleBar->geometry().height());

        //add cover art
        grd_Player->addWidget(ui->coverArt, 0, 0, -1, 1);

        ui->coverArt->setMargin(0);
        ui->coverArt->setContentsMargins(0, 0, 0, 0);
        ui->coverArt->setFixedHeight(grbx_Player->height());
        ui->coverArt->setFixedWidth(ui->coverArt->geometry().height());

        //add title, artist and album labels
        grd_Player->addWidget(grbx_SongInfo, 0, 1, 1, -1);
        grbx_SongInfo->setFixedWidth(miniWidth - ui->coverArt->height());
        grd_SongInfo->setContentsMargins(15, 5, 15, 5);
        ui->lbl_Artist->setContentsMargins(0, -4, 0, 0);
        grd_SongInfo->setSpacing(0);
        ui->lbl_Title->setFont(QFont("Segoe UI", 18));
        ui->lbl_Artist->setFont(QFont("Segoe UI", 9));
        ui->lbl_Album->setFont(QFont("Segoe UI", 8));

        //add progress bar
        grd_Player->addWidget(grbx_SongProgress, 1, 1, 1, -1);
        grd_SongProgress->setContentsMargins(15, 0, 15, 0);
        grd_SongProgress->setSpacing(10);

        //add player button
        grd_Player->addWidget(grbx_SongButton, 2, 1, 1, -1);
        grbx_SongButton->setFixedWidth(300);
        grd_Player->setAlignment(grbx_SongButton, Qt::AlignCenter);
        grd_SongButton->setContentsMargins(0, 5, 0, 5);
        grd_SongSetting->setSpacing(10);
        ui->btn_Prev->setFixedSize(18, 18);
        ui->btn_Next->setFixedSize(18, 18);
        ui->btn_Play->setFixedSize(24, 24);

        //add player setting
        grd_Player->addWidget(grbx_SongSetting, 3, 1, 1, -1);
        grd_SongSetting->setContentsMargins(15, 5, 20, 15);
        grd_SongSetting->setSpacing(20);
        ui->btn_Volume->setFixedSize(QSize(16, 16));
        ui->btn_Repeat->setFixedSize(QSize(16, 16));
        ui->btn_Shuffle->setFixedSize(QSize(16, 16));
        ui->btn_Lock->setFixedSize(QSize(16, 16));
        ui->btn_AutoDetector->setFixedSize(QSize(16, 16));

        this->resize(miniWidth, miniHeight);
        this->move(MiniPos);
    }
}

void MiniPauzer::changeStyle()
{
    this->setStyleSheet("QWidget#MiniPauzer {"
                        "border: 3px solid green;"
                        "margin: 10px;"
                        "}");

    ui->titleBar->setStyleSheet("background-color: #2d2d2d");

    grbx_Player->setStyleSheet("border: 0px; background-color: #282828;");
    grbx_PlayerFull->setStyleSheet("QGroupBox{border-top: 2px solid #2fdd69;"
                                   "background-color: #282828;}");

    ui->lbl_Title->setStyleSheet("color: white;");
    ui->lbl_Artist->setStyleSheet("color: white;");
    ui->lbl_Album->setStyleSheet("color: #e8e8e8;");
    ui->lbl_CurTime->setStyleSheet("color: #e8e8e8;");
    ui->lbl_songLen->setStyleSheet("color: #e8e8e8;");

    ui->btn_Minimize->setAttribute(Qt::WA_NoMousePropagation);
    ui->btn_Minimize->setIcon(QPixmap(":/icons/Minimize.png"));
    ui->btn_Minimize->setIconSize(QSize(15, 15));

    if (this->isMaximized())
    {
        ui->btn_Maximize->setAttribute(Qt::WA_NoMousePropagation);
        ui->btn_Maximize->setIcon(QPixmap(":/icons/Restore.png"));
        ui->btn_Maximize->setIconSize(QSize(15, 15));
    }
    else
    {
        ui->btn_Maximize->setAttribute(Qt::WA_NoMousePropagation);
        ui->btn_Maximize->setIcon(QPixmap(":/icons/Maximize.png"));
        ui->btn_Maximize->setIconSize(QSize(15, 15));
    }

    ui->btn_Close->setAttribute(Qt::WA_NoMousePropagation);
    ui->btn_Close->setIcon(QPixmap(":/icons/Close.png"));
    ui->btn_Close->setIconSize(QSize(15, 15));

    ui->btn_Up->setAttribute(Qt::WA_NoMousePropagation);
    ui->btn_Up->setIcon(QPixmap(":/icons/Up.png"));
    ui->btn_Up->setIconSize(QSize(15, 15));

    ui->btn_Down->setAttribute(Qt::WA_NoMousePropagation);
    ui->btn_Down->setIcon(QPixmap(":/icons/Down.png"));
    ui->btn_Down->setIconSize(QSize(15, 15));

    ui->btn_Menu->setAttribute(Qt::WA_NoMousePropagation);

    ui->coverArt->setScaledContents(true);
    QPixmap pixmap(":/resources/cover.png");
    ui->coverArt->setPixmap(pixmap);

    ui->lbl_Title->setMinimumHeight(20);
    ui->lbl_Artist->setMinimumHeight(18);
    ui->lbl_Album->setMinimumHeight(16);

    ui->btn_Play->setToolTip(tr("Play/Pause"));
    ui->btn_Prev->setToolTip(tr("Previous Song"));
    ui->btn_Next->setToolTip(tr("Next Song"));
    ui->btn_AutoDetector->setToolTip(tr("Auto detector"));
    ui->btn_Lock->setToolTip(tr("Lock Playlist"));
    ui->btn_Repeat->setToolTip(tr("Repeat Song"));
    ui->btn_Shuffle->setToolTip(tr("Shuffle playlist"));
    ui->btn_Volume->setToolTip(tr("Volume"));

    grip->setStyleSheet("QSizeGrip {"
                        "width: 15px;"
                        "height: 15px;"
                        "image: url(:/icons/Grip.png); }"
                        "QSizeGrip:hover {"
                        "image: url(:/icons/Grip_Hover.png); }");

    grbx_LeftPanel->setStyleSheet("QGroupBox{"
                                  "border: 0px;"
                                  "background-color: #262626;"
                                  "}");

    ui->lbl_Icon->setPixmap(QPixmap(":/icons/Pauzer_Icon.png"));
    ui->lbl_Icon->setScaledContents(true);

    ui->spr_Icon->setStyleSheet("background-color: #404040;"
                                "margin: 10px;");

    ui->btn_Master->setAutoExclusive(true);
    ui->btn_Master->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->btn_Master->setIcon(QIcon(":/icons/Master.png"));
    ui->btn_Master->setIconSize(QSize(24, 24));
    ui->btn_Master->setFont(QFont("UTM Avo"));
    ui->btn_Master->setText(tr(" Master Playlist"));
    ui->btn_Master->setStyleSheet("QToolButton{"
                                  "text-align: left;"
                                  "border: 0px;"
                                  "padding-left: 20px;"
                                  "font-size: 18px;"
                                  "background-color: transparent;"
                                  "color: white;"
                                  "}"
                                  "QToolButton:hover{"
                                  "background-color: #595959;"
                                  "}"
                                  "QToolButton:checked{"
                                  "background-color: #3eba58;"
                                  "}"
                                  "QToolButton:checked:hover{"
                                  "background-color: #61d169;"
                                  "}");

//    ui->btn_User->setAutoExclusive(true);
//    ui->btn_User->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//    ui->btn_User->setIcon(QIcon(":/icons/Master.png"));
//    ui->btn_User->setIconSize(QSize(24, 24));
//    ui->btn_User->setFont(QFont("UTM Avo"));
//    ui->btn_User->setText(tr(" User Playlist"));
//    ui->btn_User->setStyleSheet("QToolButton{"
//                                  "text-align: left;"
//                                  "border: 0px;"
//                                  "padding-left: 20px;"
//                                  "font-size: 18px;"
//                                  "background-color: transparent;"
//                                  "color: white;"
//                                  "}"
//                                  "QToolButton:hover{"
//                                  "background-color: #595959;"
//                                  "}"
//                                  "QToolButton:checked{"
//                                  "background-color: #3eba58;"
//                                  "}"
//                                  "QToolButton:checked:hover{"
//                                  "background-color: #61d169;"
//                                  "}");

    grbx_MidPanel->setStyleSheet("QGroupBox{"
                                 "background-color: #1a1a1a;"
                                 "}");

    ui->btn_Albums->setAutoExclusive(true);
    ui->btn_Albums->setIcon(QIcon(":/icons/Albums.png"));
    ui->btn_Albums->setIconSize(QSize(40, 40));
    ui->btn_Albums->setText(tr("Albums"));
    ui->btn_Albums->setStyleSheet("QPushButton{"
                                  "margin: 10px;"
                                  "border-radius: 3px;"
                                  "color: white;"
                                  "background-color: transparent;"
                                  "}"
                                  "QPushButton:hover{"
                                  "border: 2px solid #61d169;"
                                  "background-color: #262626;"
                                  "}"
                                  "QPushButton:checked{"
                                  "color: white;"
                                  "background-color: #61d169;"
                                  "}");

    ui->btn_Artists->setAutoExclusive(true);
    ui->btn_Artists->setIcon(QIcon(":/icons/Artists.png"));
    ui->btn_Artists->setIconSize(QSize(40, 40));
    ui->btn_Artists->setText(tr("Artists"));
    ui->btn_Artists->setStyleSheet("QPushButton{"
                                   "margin: 10px;"
                                   "border-radius: 3px;"
                                  "color: white;"
                                  "background-color: transparent;"
                                  "}"
                                  "QPushButton:hover{"
                                   "border: 2px solid #61d169;"
                                   "background-color: #262626;"
                                  "}"
                                  "QPushButton:checked{"
                                  "color: white;"
                                  "background-color: #61d169;"
                                  "}");

    ui->btn_Songs->setAutoExclusive(true);
    ui->btn_Songs->setIcon(QIcon(":/icons/Songs.png"));
    ui->btn_Songs->setIconSize(QSize(40, 40));
    ui->btn_Songs->setText(tr("Songs"));
    ui->btn_Songs->setStyleSheet("QPushButton{"
                                 "margin: 10px;"
                                 "border-radius: 3px;"
                                  "color: white;"
                                  "background-color: transparent;"
                                  "}"
                                  "QPushButton:hover{"
                                  "border: 2px solid #61d169;"
                                  "background-color: #262626;"
                                  "}"
                                  "QPushButton:checked{"
                                  "color: white;"
                                  "background-color: #61d169;"
                                  "}");

}

void MiniPauzer::createMenu()
{
    menu = new QMenu(this);
    ui->btn_Menu->setMenu(menu);
    ui->btn_Menu->setPopupMode(QToolButton::InstantPopup);
    ui->btn_Menu->setArrowType(Qt::NoArrow);

    openFoldersAct = new QAction(tr("&Open Folders"), this);
    openFoldersAct->setIcon(QPixmap(":/icons/Menu_Folders.png"));
    openFoldersAct->setText(tr("Open Folders"));
    connect(openFoldersAct, SIGNAL(triggered()), this, SLOT(openFolders()));

    settingsAct = new QAction(tr("&Settings"), this);
    settingsAct->setIcon(QPixmap(":/icons/Menu_Settings.png"));
    settingsAct->setIconText(tr("Settings"));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setIcon(QPixmap(":/icons/Menu_About.png"));
    aboutAct->setIconText(tr("About"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(openAbout()));

    menu->addAction(openFoldersAct);
    menu->addAction(settingsAct);
    menu->addSeparator();
    menu->addAction(aboutAct);

    menu->setStyleSheet("QMenu {"
                        "background-color: #383838;"
                        "color: white;"
                        "margin: 2px;"
                        "padding: 4px;"
                        "}"
                        "QMenu::item {"
                        "padding-left: 35px;"
                        "padding-right: 35px;"
                        "padding-top: 5px;"
                        "padding-bottom: 5px;"
                        "margin-top: 5px;"
                        "margin-bottom: 5px;"
                        "text-align: center;"
                        "border: 1px solid transparent;"
                        "}"
                        "QMenu::item:selected {"
                        "background-color: #2fdd69;"
                        "}"
                        "QMenu::separator {"
                        "height: 1px;"
                        "background: gray;"
                        "margin-left: 2px;"
                        "margin-right: 2px;"
                        "}");
}

void MiniPauzer::resizeEvent(QResizeEvent *event)
{
    if (state == State::FULL && !isFullScreen)
    {
        FullSize.setWidth(this->width());
        FullSize.setHeight(this->height());
    }
}

void MiniPauzer::on_btn_Play_clicked()
{

    if (player->getPlaying() == false)
    {
        isDetectorOn = true;
        player->play();
        playToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
    else
    {
        isDetectorOn = false;
        player->pause();
        playToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}


void MiniPauzer::detectAudio(int audio_num)
{
    if (isDetectorOn)
    {
        if (player->getPlaying() == true)
        {
            if (audio_num > 1)
            {
                player->pause();
                playToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            }
        }
        else
        {
            if (audio_num == 0)
            {
                player->play();
                playToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            }
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

void MiniPauzer::updateLabelMaxLen(int length)
{
    int hour = length / 3600;
    length = length % 3600;
    int minute = length / 60;
    length = length % 60;
    int sec = length;

    QTime maxLen;
    maxLen.setHMS(hour, minute, sec);
    QString text;

    if (hour == 0)
        text = maxLen.toString("m:ss");
    else
        text = maxLen.toString("h:mm:ss");

    ui->lbl_songLen->setText(text);
}

void MiniPauzer::openFolders()
{
    dialog = new FolderDialog();
    dialog->setModal(true);

    connect(dialog, SIGNAL(createdFolderList(QStringList)), this, SLOT(getFolderList(QStringList)));

    dialog->exec();
    delete dialog;
}

void MiniPauzer::openAbout()
{
    QDialog *about = new QDialog();
    about->setModal(true);
    about->setFixedSize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout();
    about->setLayout(layout);

    QLabel *label = new QLabel();
    label->setText("Pauzer © 2016\n\n"
                   "GitHub: github.com/lynxerious/Pauzer\n\n"
                   "Created by:\n"
                   "Thái Việt Linh\n"
                   "Hà Mộng Long\n");
    label->setFont(QFont("Segoe UI", 10));
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label, Qt::AlignCenter);

    QPushButton *button = new QPushButton();
    button->setText(tr("Ok"));
    layout->addWidget(button, Qt::AlignCenter);
    button->setFixedHeight(40);

    connect(button, SIGNAL(clicked()), about, SLOT(close()));
    about->exec();

    delete about;
}

void MiniPauzer::getFolderList(QStringList list)
{
    creator->setList(list);
    creator->start();

    isProcessCanceled = false;

    widget->show();
}

void MiniPauzer::processFinished()
{
    //user canceled the process midway
    if (isProcessCanceled)
    {
        Manager::CreationProcessSuccess(false);
    }
    else //run successfully
    {
        Manager::CreationProcessSuccess(true);
        if (widget)
        {
            widget->close();
        }

        masterModel->initializeModel();
        queueModel->initializeModel();

        changePanel(Panel::MASTER);
    }
}

void MiniPauzer::processTerminated()
{
    isProcessCanceled = true;

    Manager::LoadSongToMaster();
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

void MiniPauzer::setCoverArt(const QPixmap &pixmap)
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

void MiniPauzer::titleBardragged(const QPoint &newPoint)
{
    this->move(newPoint);
    if (state == State::FULL)
    {
        FullPos = newPoint;
    }
    else
    {
        MiniPos = newPoint;
    }
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
    this->setWindowState(this->windowState() ^ Qt::WindowMaximized);

    if (this->isMaximized())
    {
        isFullScreen = true;

        ui->btn_Maximize->setAttribute(Qt::WA_NoMousePropagation);
        ui->btn_Maximize->setIcon(QPixmap(":/icons/Restore.png"));
        ui->btn_Maximize->setIconSize(QSize(15, 15));
    }
    else
    {
        isFullScreen = false;

        ui->btn_Maximize->setAttribute(Qt::WA_NoMousePropagation);
        ui->btn_Maximize->setIcon(QPixmap(":/icons/Maximize.png"));
        ui->btn_Maximize->setIconSize(QSize(15, 15));
    }
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

void MiniPauzer::on_btn_Lock_toggled(bool checked)
{
    if (player != NULL)
    {
        player->setLock(checked);
    }
}

void MiniPauzer::on_btn_Albums_toggled(bool checked)
{
    if (checked)
    {
        masterModel->setMode(MasterModel::Mode::ALBUM);
        tableView->resizeColumnsToContents();
        tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        tableView->horizontalHeader()->setStretchLastSection(true);
    }
}

void MiniPauzer::on_btn_Artists_toggled(bool checked)
{
    if (checked)
    {
        masterModel->setMode(MasterModel::Mode::ARTIST);
        tableView->resizeColumnsToContents();
        tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        tableView->horizontalHeader()->setStretchLastSection(true);
    }
}

void MiniPauzer::on_btn_Songs_toggled(bool checked)
{
    if (checked)
    {
        masterModel->setMode(MasterModel::Mode::SONG);
        tableView->resizeColumnsToContents();
        tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        tableView->horizontalHeader()->setStretchLastSection(false);
    }
}

void MiniPauzer::switchPlayerState(bool PauseToPlay)
{
    if (PauseToPlay)
    {
        if (!player->getPlaying())
        {
            ui->btn_Play->click();
        }
    }
    else
    {
        if (player->getPlaying())
        {
            ui->btn_Play->click();
        }
    }
}
