#include "sliderbar.h"

void SliderBar::mousePressEvent(QMouseEvent *event)
{
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);

    if (event->button() == Qt::LeftButton &&
            sr.contains(event->pos()) == false)
    {
        int newVal;
        if (orientation() == Qt::Vertical)
        {
            double halfHandleHeight = (0.5 * sr.height()) + 0.5;
            int adaptedPosY = height() - event->y();
            if ( adaptedPosY < halfHandleHeight )
                  adaptedPosY = halfHandleHeight;
            if ( adaptedPosY > height() - halfHandleHeight )
                  adaptedPosY = height() - halfHandleHeight;
            double newHeight = (height() - halfHandleHeight) - halfHandleHeight;
            double normalizedPosition = (adaptedPosY - halfHandleHeight)  / newHeight ;

            newVal = minimum() + (maximum()-minimum()) * normalizedPosition;
        }
        else
        {
            double halfHandleWidth = (0.5 * sr.width()) + 0.5; // Correct rounding
            int adaptedPosX = event->x();
            if ( adaptedPosX < halfHandleWidth )
                adaptedPosX = halfHandleWidth;
            if ( adaptedPosX > width() - halfHandleWidth )
                adaptedPosX = width() - halfHandleWidth;
            // get new dimensions accounting for slider handle width
            double newWidth = (width() - halfHandleWidth) - halfHandleWidth;
            double normalizedPosition = (adaptedPosX - halfHandleWidth)  / newWidth ;

            newVal = minimum() + ((maximum()-minimum()) * normalizedPosition);
        }

        if (invertedAppearance() == true)
            setValue( maximum() - newVal );
        else
            setValue(newVal);

        event->accept();
    }
    QSlider::mousePressEvent(event);
}

SliderBar::SliderBar(QWidget *parent) :
    QSlider(parent)
{
    isDragging = false;
    this->setTracking(false);
}

void SliderBar::setDragging(bool drag)
{
    if (isDragging != drag)
        isDragging = drag;
}

bool SliderBar::getDragging()
{
    return isDragging;
}

void SliderBar::setCurrentPos(int time)
{
    if (isDragging == true)
        return;
    this->setSliderPosition(time);
}

void SliderBar::setMaxLength(int length)
{
    this->setMaximum(length);
}

