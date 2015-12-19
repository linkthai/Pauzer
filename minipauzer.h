#ifndef MINIPAUZER_H
#define MINIPAUZER_H

#include "player.h"
#include "manager.h"
#include "folderdialog.h"
#include "autodetector.h"

namespace Ui {
class MiniPauzer;
}

class MiniPauzer : public QWidget
{
    Q_OBJECT

private:
    enum class State { FULL, MINI };

    State state;
    Ui::MiniPauzer *ui;
    Manager *manager;
    AutoDetector *detector;
    QTimer *buttonPlayClickTimer;

    bool isPlaying;
    bool isDetectorOn;
    bool isManuallyPlayed;
    bool isAutoPauseAllowed;
    bool isButtonPlayClickAllowed;

    QVBoxLayout *mainGrid;

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

    void layoutSetup();
    void loadData();
    void changeStyle();
    void changeState(State _state);

public:
    explicit MiniPauzer(QWidget *parent = 0);
    ~MiniPauzer();

    //PS: THIS IS TEMPORARY UNTIL SWITCH PLAYER TO SINGLETON PATTERN
    Player *player;

private slots:
    void updateLabelCurTime(int time);
    void updateLabelMaxLen(int legth);

    void getFolderList(QStringList list);

    void releaseButtonPlay();

    void on_btn_Play_clicked();

    void on_sliderBar_sliderPressed();

    void on_sliderBar_sliderReleased();

    void on_btn_ChooseFolders_clicked();

	void on_detected_audio(int audio_num);

    void on_btn_Prev_clicked();

    void on_btn_Next_clicked();

    void endOfPlaylistStop();

    void on_new_pixmap(const QPixmap &pixmap);

    void on_btn_Close_clicked();

    void on_btn_Minimize_clicked();

    void on_titleBar_dragged(const QPoint &newPoint);

    void on_btn_Up_clicked();
    void on_btn_Down_clicked();
    void on_btn_Maximize_clicked();

    void on_btn_AutoDetector_toggled(bool checked);

    void on_btn_Shuffle_toggled(bool checked);

    void on_btn_Repeat_toggled(bool checked);
    void on_btn_Volume_toggled(bool checked);
};

#endif // MINIPAUZER_H
