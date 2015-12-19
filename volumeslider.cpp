#include "volumeslider.h"
#include "ui_volumeslider.h"

VolumeSlider::VolumeSlider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VolumeSlider)
{
    ui->setupUi(this);
    layout = new QGridLayout(this);
    this->setLayout(layout);
    layout->addWidget(ui->volumeSlider, 0, 0, -1, -1, Qt::AlignCenter);

    this->setStyleSheet("background-color: #3d3d3d;"
                        "border: 0px;");
    ui->volumeSlider->setMaximum(MAX_VOL);
    ui->volumeSlider->setTracking(true);

    connect(ui->volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(emitVolumeSignal(int)));
}

VolumeSlider::~VolumeSlider()
{
    delete ui;
    delete layout;
}

void VolumeSlider::setSize(int w, int h)
{
    this->setFixedSize(w, h);
    layout->setAlignment(ui->volumeSlider, Qt::AlignCenter);
    ui->volumeSlider->setFixedHeight(h - 36);
}

void VolumeSlider::setPos(const QPoint &pos, int width)
{
    this->move(pos - QPoint(this->width() / 2 - width / 2, this->height() + 5));
}

void VolumeSlider::setCur(int cur)
{
    ui->volumeSlider->setSliderPosition(cur);
}

void VolumeSlider::enterEvent(QEvent *e)
{
    this->show();
}

void VolumeSlider::leaveEvent(QEvent *e)
{
    QWidget *temp = static_cast<QWidget *>(this->parent());
    QPoint cur;
    cur.setX(QCursor::pos().x() - this->mapToGlobal(this->rect().topLeft()).x());
    cur.setY(QCursor::pos().y() - this->mapToGlobal(this->rect().topLeft()).y());
    if (!(cur.x() > this->rect().left()
          && cur.x() < this->rect().right()
          && cur.y() > this->rect().bottom()))
        this->hide();
}

void VolumeSlider::emitVolumeSignal(int cur)
{
    float volume = (float)cur / (float)MAX_VOL;

    if (ui->volumeSlider->underMouse())
        emit volumeDragged(volume);
}
