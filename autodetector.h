#ifndef AUTODETECTOR_H
#define AUTODETECTOR_H

#include <QThread>
#include <QTimer>
#include <mmdeviceapi.h>
#include <audiopolicy.h>
#include <endpointvolume.h>

class AutoDetector : public QThread
{
    Q_OBJECT
private:
    QTimer *t;
	QTimer *smallSoundDetector;
	int current_num;
	int detected;
	IAudioSessionManager2* pSessionManager;
	HRESULT CreateSessionManager(IAudioSessionManager2** ppSessionManager);
signals:
    void audioDetected(int audio_num);
public:
    AutoDetector(QObject *parent = 0);
	~AutoDetector();
    void run();
    static void emitSignals(QString str);
    static QString s;
private slots:
    void checkAudioOutput();
	void checkSmallSound();
};

#endif // AUTODETECTOR_H
