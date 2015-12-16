#include "qdraggableframe.h"

QDraggableFrame::QDraggableFrame(QWidget *parent) :
    QFrame(parent)
{
}

void QDraggableFrame::mousePressEvent(QMouseEvent *event){
    clickPos = event->pos();
}

void QDraggableFrame::mouseMoveEvent(QMouseEvent *event){
    if (event->buttons() && Qt::LeftButton) {
        QPoint diff = event->globalPos() - clickPos;
        QPoint newpos = this->pos() + diff;

        emit titleBarDragged(newpos);
    }
}
