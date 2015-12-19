#ifndef VOLUMESLIDER_H
#define VOLUMESLIDER_H

#include <QWidget>
#include <QLayout>

namespace Ui {
class VolumeSlider;
}

class VolumeSlider : public QWidget
{
    Q_OBJECT

private:
    Ui::VolumeSlider *ui;
    QGridLayout *layout;

public:
    static const int MAX_VOL = 100;

    explicit VolumeSlider(QWidget *parent = 0);
    ~VolumeSlider();
    void setSize(int w, int h);
    void setPos(const QPoint &pos, int width);
    void setCur(int cur);
protected:
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
signals:
    void volumeDragged(float vol);
public slots:
    void emitVolumeSignal(int cur);
};

#endif // VOLUMESLIDER_H
