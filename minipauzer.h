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
    void on_btn_Play_clicked();

    void on_sliderBar_sliderPressed();

    void on_sliderBar_sliderReleased();

    void updateLabelCurTime(int time);
    void updateLabelMaxLen(int legth);

    void on_btn_ChooseFolders_clicked();

    void getFolderList(QStringList list);

private:
    Ui::MiniPauzer *ui;
    Manager *manager;
    Player *player;
    AutoDetector *detector;
};

#endif // MINIPAUZER_H
