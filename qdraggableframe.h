#ifndef QDRAGGABLEFRAME_H
#define QDRAGGABLEFRAME_H

#include <QStyleOptionFrame>

class QDraggableFrame: public QFrame
{
    Q_OBJECT
public:
    explicit QDraggableFrame(QWidget *parent = 0);
private:
    QPoint mpos;
protected:
    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);
signals:
    void titleBarDragged(const QPoint &newPoint);
};

#endif // QDRAGGABLEFRAME_H
