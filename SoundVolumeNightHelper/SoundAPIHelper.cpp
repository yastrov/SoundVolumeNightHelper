#include "stdafx.h"
#include "SoundAPIHelper.h"


SoundAPIHelper::SoundAPIHelper(): initialized(false)
{
}


SoundAPIHelper::~SoundAPIHelper()
{
	if (initialized) {
		unInit();
	}
}

void SoundAPIHelper::init()
{
	HRESULT hr;
	CoInitialize(NULL);
	IMMDeviceEnumerator *deviceEnumerator = NULL;
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
	IMMDevice *defaultDevice = NULL;

	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	deviceEnumerator->Release();
	deviceEnumerator = NULL;

	endpointVolume = NULL;
	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
	defaultDevice->Release();
	defaultDevice = NULL;
	initialized = true;
}

void SoundAPIHelper::unInit()
{
	endpointVolume->Release();
	CoUninitialize();
	initialized = false;
}

float SoundAPIHelper::getMasterVolumeLevelDB()
{
	float currentVolume = 0;
	HRESULT hr = endpointVolume->GetMasterVolumeLevel(&currentVolume);
	return currentVolume;
}

float SoundAPIHelper::getMasterVolumeLevelScalar()
{
	float currentVolume = 0;
	HRESULT hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
	return currentVolume;
}

HRESULT SoundAPIHelper::setMasterVolumeLevelDB(float value)
{
	return endpointVolume->SetMasterVolumeLevel(value, NULL);
}

HRESULT SoundAPIHelper::setMasterVolumeLevelScalar(float value)
{
	return endpointVolume->SetMasterVolumeLevelScalar(value, NULL);
}
