#ifndef AUTODETECTOR_H
#define AUTODETECTOR_H

#include <QThread>
#include <QTimer>

class AutoDetector : public QThread
{
    Q_OBJECT
private:
    QTimer *t;
signals:
    void audioDetected(QString str);
public:
    AutoDetector(QObject *parent = 0);
    void run();

private slots:
    void checkAudioOutput();
};

#endif // AUTODETECTOR_H
