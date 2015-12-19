#ifndef VOLUMEBUTTON_H
#define VOLUMEBUTTON_H

#include <QStyleOptionButton>
#include "volumeslider.h"
#include "player.h"

class VolumeButton : public QPushButton
{
    Q_OBJECT
private:
    VolumeSlider *dialog;
    float volume;
    float real_volume;
public:
    explicit VolumeButton(QWidget *parent = 0);
    ~VolumeButton();
    float getVolume();
protected:
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
public slots:
    void volumeToggled(bool checked);
    void setVolume(float vol);
signals:
    void volumeChanged(float vol);

};

#endif // VOLUMEBUTTON_H
