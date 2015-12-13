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

public:
    explicit MiniPauzer(QWidget *parent = 0);
    ~MiniPauzer();

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

    void on_btn_Shuffle_stateChanged(int state);

    void endOfPlaylistStop();

    void on_btn_Repeat_stateChanged(int state);

    void on_btn_AutoDetector_stateChanged(int state);

private:
    Ui::MiniPauzer *ui;
    Manager *manager;
    Player *player;
    AutoDetector *detector;
	QTimer *buttonPlayClickTimer;

	bool isPlaying;
	bool isDetectorOn;
	bool isManuallyPlayed;
	bool isAutoPauseAllowed;
	bool isButtonPlayClickAllowed;
};

#endif // MINIPAUZER_H
