#pragma once
#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

class SoundAPIHelper
{
public:
	SoundAPIHelper();
	virtual ~SoundAPIHelper();

	void init();
	void unInit();

	float getMasterVolumeLevelDB();
	float getMasterVolumeLevelScalar();

	HRESULT setMasterVolumeLevelDB(float value);
	HRESULT setMasterVolumeLevelScalar(float value);
private:
	bool initialized;
	IAudioEndpointVolume *endpointVolume;
	
};

