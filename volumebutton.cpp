#include "volumebutton.h"

VolumeButton::VolumeButton(QWidget *parent) :
    QPushButton(parent)
{
    this->setMouseTracking(true);
    dialog = new VolumeSlider(this);
    dialog->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    dialog->setSize(60, 210);
    dialog->setMouseTracking(true);
    dialog->hide();

    volume = 1.0;
    real_volume = 1.0;

    connect(dialog, SIGNAL(volumeDragged(float)), this, SLOT(setVolume(float)));
    connect (this, SIGNAL(toggled(bool)), SLOT(volumeToggled(bool)));
}

VolumeButton::~VolumeButton()
{
    delete dialog;
}

void VolumeButton::setVolume(float vol)
{
    if (vol < 0 || vol > 1)
        return;

    volume = vol;
    real_volume = vol;

    if (volume == 0)
        this->setChecked(false);
    else
        this->setChecked(true);

    emit volumeChanged(volume);
}

float VolumeButton::getVolume()
{
    return volume;
}

void VolumeButton::leaveEvent(QEvent * e)
{
    QPoint cur;
    cur.setX(QCursor::pos().x() - this->mapToGlobal(this->rect().topLeft()).x());
    cur.setY(QCursor::pos().y() - this->mapToGlobal(this->rect().topLeft()).y());
    if (!(cur.x() > this->rect().left()
          && cur.x() < this->rect().right()
          && cur.y() < this->rect().top()))
        dialog->hide();
}

void VolumeButton::volumeToggled(bool checked)
{
    if (checked)
    {
        if (real_volume == 0)
            real_volume = 0.1;
        volume = real_volume;
        emit volumeChanged(volume);
        dialog->setCur(volume * dialog->MAX_VOL);
    }
    else
    {
        volume = 0;
        emit volumeChanged(volume);
        dialog->setCur(volume * dialog->MAX_VOL);
    }
}

void VolumeButton::enterEvent(QEvent * e)
{
    dialog->setCur(volume * dialog->MAX_VOL);
    dialog->setPos(this->mapToGlobal(QPoint(this->rect().topLeft())), this->width());
    dialog->show();
}

