#ifndef SLIDERBAR_H
#define SLIDERBAR_H

#include <QStyleOptionSlider>

class SliderBar : public QSlider
{
    Q_OBJECT
private:
    bool isDragging;
protected:
  void mousePressEvent ( QMouseEvent * event );
public:
    explicit SliderBar(QWidget *parent = 0);
    void setDragging(bool drag);
    bool getDragging();
public slots:
    void setCurrentPos(int time);
    void setMaxLength(int length);
};

#endif // SLIDERBAR_H
