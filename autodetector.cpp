#include "autodetector.h"

#define SAFE_RELEASE(obj)   \
    if(obj != NULL) {   \
        obj->Release(); \
        obj = NULL; \
    }

QString AutoDetector::s;

AutoDetector::AutoDetector(QObject *parent) :
    QThread(parent)
{
    t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(checkAudioOutput()));
    t->start(500);

	smallSoundDetector = new QTimer(this);
	smallSoundDetector->setSingleShot(true);
	connect(smallSoundDetector, SIGNAL(timeout()), this, SLOT(checkSmallSound()));

	current_num = -1;
	detected = -1;

	pSessionManager = NULL;
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

	if (current_num == -1)
		current_num = list.size();
	
	detected = list.size();
	if (current_num != detected && smallSoundDetector->isActive() == false)
        smallSoundDetector->start(1500);

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
