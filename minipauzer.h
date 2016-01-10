#ifndef MINIPAUZER_H
#define MINIPAUZER_H

#include <QGraphicsDropShadowEffect>

#include "player.h"
#include "manager.h"
#include "folderdialog.h"
#include "autodetector.h"
#include "librarycreator.h"
#include "processwidget.h"
#include "playlistqueuewidget.h"
#include "mastermodel.h"
#include "mastertableview.h"

namespace Ui {
class MiniPauzer;
}

class MiniPauzer : public QWidget
{
    Q_OBJECT

private:
    int miniWidth;
    int miniHeight;

    enum class State { FULL, MINI };
    enum class Panel { MASTER };

    ProcessWidget *widget;
    FolderDialog *dialog;

    State state;
    Panel panel;
    Ui::MiniPauzer *ui;
    AutoDetector *detector;
    LibraryCreator *creator;
    QTimer *buttonPlayClickTimer;

    PlaylistQueueModel *queueModel;
    MasterModel *masterModel;

    Player *player;

    bool isDetectorOn;
    bool isProcessCanceled;

    QVBoxLayout *mainGrid;

    QHBoxLayout *titleBarLayout;
    QHBoxLayout *rightTitleBarLayout;
    QHBoxLayout *leftTitleBarLayout;

    QGridLayout *grd_Player;
    QGridLayout *grd_PlayerFull;
    QVBoxLayout *grd_SongInfo;
    QHBoxLayout *grd_SongProgress;
    QHBoxLayout *grd_SongButton;
    QHBoxLayout *grd_SongSetting;

    QGroupBox *grbx_Player;
    QGroupBox *grbx_PlayerFull;
    QGroupBox *grbx_SongInfo;
    QGroupBox *grbx_SongProgress;
    QGroupBox *grbx_SongButton;
    QGroupBox *grbx_SongSetting;

    QHBoxLayout *grd_Manager;
    QGroupBox *grbx_Manager;

    QVBoxLayout *grd_LeftPanel;
    QGroupBox *grbx_LeftPanel;

    QVBoxLayout *grd_MidPanel;
    QGroupBox *grbx_MidPanel;

    QGridLayout *grd_PanelInfo;
    QGroupBox *grbx_PanelInfo;

    MasterTableView *tableView;

    PlaylistQueueWidget *queuePanel;

    QSizeGrip *grip;

    QPoint MiniPos;
    QPoint FullPos;
    QSize FullSize;
    bool isFullScreen;

    QMenu *menu;
    QAction *openFoldersAct;
    QAction *settingsAct;
    QAction *aboutAct;

    void layoutSetup();
    void loadData();
    void changeStyle();
    void createMenu();
    void changePanel(Panel _panel);
    void changeState(State _state);

    QWinThumbnailToolBar* thumbnailToolBar;
    QWinThumbnailToolButton *playToolButton;
    QWinThumbnailToolButton *forwardToolButton;
    QWinThumbnailToolButton *backwardToolButton;

protected:
    void resizeEvent(QResizeEvent * event);

public:
    explicit MiniPauzer(QWidget *parent = 0);
    ~MiniPauzer();
    void createThumbBar();

private slots:
    void updateLabelCurTime(int time);
    void updateLabelMaxLen(int length);

    void getFolderList(QStringList list);

    void on_btn_Play_clicked();

    void on_sliderBar_sliderPressed();

    void on_sliderBar_sliderReleased();

    void openFolders();
    void openAbout();

    void detectAudio(int audio_num);

    void on_btn_Prev_clicked();

    void on_btn_Next_clicked();

    void endOfPlaylistStop();

    void setCoverArt(const QPixmap &pixmap);

    void on_btn_Close_clicked();

    void on_btn_Minimize_clicked();

    void titleBardragged(const QPoint &newPoint);

    void on_btn_Up_clicked();
    void on_btn_Down_clicked();
    void on_btn_Maximize_clicked();

    void on_btn_AutoDetector_toggled(bool checked);

    void on_btn_Shuffle_toggled(bool checked);

    void on_btn_Repeat_toggled(bool checked);

    void processFinished();
    void processTerminated();
    void on_btn_Albums_toggled(bool checked);
    void on_btn_Artists_toggled(bool checked);
    void on_btn_Songs_toggled(bool checked);

    void switchPlayerState(bool PauseToPlay);
    void on_btn_Lock_toggled(bool checked);
};

#endif // MINIPAUZER_H
