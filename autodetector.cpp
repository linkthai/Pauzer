#include "autodetector.h"

#define SAFE_RELEASE(obj)   \
    if(obj != NULL) {   \
        obj->Release(); \
        obj = NULL; \
    }

AutoDetector::AutoDetector(QObject *parent) :
    QThread(parent)
{
    //Create a timer to check Windows audio outputs
    t = new QTimer(this);

    //SmallSoundDetector prevents triggering
	smallSoundDetector = new QTimer(this);
	smallSoundDetector->setSingleShot(true);
	connect(smallSoundDetector, SIGNAL(timeout()), this, SLOT(checkSmallSound()));

    //current number of audio
	current_num = -1;
    //detected number of audio
	detected = -1;

    //session manager
	pSessionManager = NULL;
    //create an instance of session managaer
    CreateSessionManager(&pSessionManager);
}

AutoDetector::~AutoDetector()
{
	SAFE_RELEASE(pSessionManager);
	delete t;
	delete smallSoundDetector;
}

void AutoDetector::run()
{
    while(1)
    {
    }
}

void AutoDetector::start()
{
    connect(t, SIGNAL(timeout()), this, SLOT(checkAudioOutput()));

}

void AutoDetector::pause()
{
    if (t->isActive())
        t->stop();
}

void AutoDetector::resume()
{
    if (!(t->isActive()))
        t->start(100);
}

void AutoDetector::checkSmallSound()
{
	if (current_num != detected)
		current_num = detected;
}

void AutoDetector::checkAudioOutput()
{
	QList<float> list;

	if (pSessionManager != NULL)
	{
		IAudioSessionEnumerator* sessionEnumerator = NULL;
		pSessionManager->GetSessionEnumerator(&sessionEnumerator);

		int count = NULL;
		sessionEnumerator->GetCount(&count);

		IAudioSessionControl* sessionControl = NULL;
		for (int i = 0; i < count; i++)
		{
			sessionEnumerator->GetSession(i, &sessionControl);

			AudioSessionState sessionState;
			sessionControl->GetState(&sessionState);

			if (sessionState == AudioSessionStateActive)
			{
				IAudioSessionControl2 *pAudioSessionControl2 = NULL;
				sessionControl->QueryInterface(__uuidof(IAudioSessionControl2),
					(void**)&pAudioSessionControl2);
				if (pAudioSessionControl2 != NULL && pAudioSessionControl2->IsSystemSoundsSession() == S_FALSE)
				{
					IAudioMeterInformation* pAudioMeterInfo = NULL;
					pAudioSessionControl2->QueryInterface(__uuidof(IAudioMeterInformation),
						(void**)&pAudioMeterInfo);

					float peak = 0;
					pAudioMeterInfo->GetPeakValue(&peak);
					
					if (peak < 0.001)
						peak = 0;

					if (peak != 0)
						list.append(peak);
				}

				SAFE_RELEASE(pAudioSessionControl2);
			}
		}

		SAFE_RELEASE(sessionControl);
		SAFE_RELEASE(sessionEnumerator);
	}

    //assign current_num
	if (current_num == -1)
		current_num = list.size();
	
    //detected always equal to actual current number of audio
	detected = list.size();

    //if the audio numbers continue for more than 1.5s undisrupted the new current_num value will change
	if (current_num != detected && smallSoundDetector->isActive() == false)
    {
        smallSoundDetector->start(1500);
    }

    //if the actual audio number change back to perceived current audio number, the timer will be stopped
    if (current_num == detected && smallSoundDetector->isActive())
    {
        smallSoundDetector->stop();
    }

	emit audioDetected(current_num);
}

HRESULT AutoDetector::CreateSessionManager(IAudioSessionManager2** ppSessionManager)
{
 
    HRESULT hr = S_OK;
    
    IMMDevice* pDevice = NULL;
    IMMDeviceEnumerator* pEnumerator = NULL;
    IAudioSessionManager2* pSessionManager = NULL;

    // Create the device enumerator.
    hr = CoCreateInstance(
        __uuidof(MMDeviceEnumerator),
        NULL, CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator),
        (void**)&pEnumerator);

    // Get the default audio device.
    hr = pEnumerator->GetDefaultAudioEndpoint(
                    eRender, eConsole, &pDevice);

    // Get the session manager.
    hr = pDevice->Activate(
        __uuidof(IAudioSessionManager2), CLSCTX_ALL,
        NULL, (void**)&pSessionManager);

    // Return the pointer to the caller.
    *(ppSessionManager) = pSessionManager;
    (*ppSessionManager)->AddRef();

    // Clean up.
    SAFE_RELEASE(pSessionManager);
    SAFE_RELEASE(pEnumerator);
    SAFE_RELEASE(pDevice);

    return hr;
}
