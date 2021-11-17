
#ifndef DSAudioEngine_H
#define DSAudioEngine_H

/////////////
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
#include <dsound.h> //direct sound, 
#include <windef.h> 
#include <string>
#include <stdio.h>
#include "HrDebugger.h"

class DSAudioEngine
{
public:
	explicit DSAudioEngine();
	~DSAudioEngine();

	void Init(HWND window);
	void Destroy();

	void LoadSound(std::string fname);

	void PlayLoadedSound();
	void StopPlayingLoadedSound();

private:
	HWND main_window;

	IDirectSound8* direct_sound;
	IDirectSoundBuffer* primary_buffer;
	IDirectSoundBuffer8* secondary_buffer;

	struct WaveHeaderType {
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};
};

#endif