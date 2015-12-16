#include "qdraggableframe.h"

QDraggableFrame::QDraggableFrame(QWidget *parent) :
    QFrame(parent)
{


}

void QDraggableFrame::mousePressEvent(QMouseEvent *event){
    mpos = event->pos();
}

void QDraggableFrame::mouseMoveEvent(QMouseEvent *event){
    if (event->buttons() && Qt::LeftButton) {
        QPoint diff = event->pos() - mpos;
        QPoint newpos = this->pos() + diff;

        emit titleBarDragged(newpos);
    }
}
