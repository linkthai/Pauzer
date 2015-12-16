#ifndef QDRAGGABLEFRAME_H
#define QDRAGGABLEFRAME_H

#include <QStyleOptionFrame>

class QDraggableFrame: public QFrame
{
    Q_OBJECT
public:
    explicit QDraggableFrame(QWidget *parent = 0);
protected:
    void mousePressEvent( QMouseEvent *);
    void mouseMoveEvent( QMouseEvent *);
private:
    QPoint clickPos;
signals:
    void titleBarDragged(const QPoint &newPoint);
};

#endif // QDRAGGABLEFRAME_H
