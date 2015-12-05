#include "autodetector.h"
#include <bass.h>

AutoDetector::AutoDetector(QObject *parent) :
    QThread(parent)
{
    t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(checkAudioOutput()));
    t->start(500);
}

void AutoDetector::run()
{
    while(1)
    {
    }
}

void AutoDetector::checkAudioOutput()
{
    int count=1;
    QString str;
    BASS_DEVICEINFO info;
    for (int i = 0; BASS_RecordGetDeviceInfo(i, &info); i++)
        if (info.flags & BASS_DEVICE_ENABLED)
        {
            str = info.name;
        }

    emit audioDetected(str);
}

