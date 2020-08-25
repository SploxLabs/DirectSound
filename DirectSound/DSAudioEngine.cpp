#include "DSAudioEngine.h"

DSAudioEngine::DSAudioEngine() :
	direct_sound(0),
	primary_buffer(0),
	secondary_buffer(0)
{}

DSAudioEngine::~DSAudioEngine() {
	Destroy();
}

void DSAudioEngine::Init(HWND window) {
	//Initailze the direct sound interface pointer for the def sound device
	
	ThrowIfFailed(DirectSoundCreate8(NULL, &direct_sound, NULL));
	//Set the cooperative level to priority so the format of the sound buffer can be modified
	ThrowIfFailed(direct_sound->SetCooperativeLevel(window, DSSCL_PRIORITY));
	
	/* Create the Primary Buffer*/
	DSBUFFERDESC buffer_desc;
	buffer_desc.dwSize = sizeof(DSBUFFERDESC);
	buffer_desc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	buffer_desc.dwBufferBytes = 0;
	buffer_desc.dwReserved = 0;
	buffer_desc.lpwfxFormat = NULL;
	buffer_desc.guid3DAlgorithm = GUID_NULL;

	/* Create Wav Format From CD Quality .WAV files */
	WAVEFORMATEX wav_format;
	wav_format.wFormatTag = WAVE_FORMAT_PCM; 
	wav_format.nChannels = 2;
	wav_format.nSamplesPerSec = 44100;
	wav_format.wBitsPerSample = 16;
	wav_format.nBlockAlign = (wav_format.wBitsPerSample / 8) * wav_format.nChannels;
	wav_format.nAvgBytesPerSec = wav_format.nSamplesPerSec * wav_format.nBlockAlign;
	wav_format.cbSize = 0;

	/* Set The Primary Buffer Format */
	ThrowIfFailed(primary_buffer->SetFormat(&wav_format));


}

void DSAudioEngine::Destroy()
{
	//release the primary buffer
	primary_buffer->Release();
	//release the direct sound interface pointer
	direct_sound->Release();
}

void DSAudioEngine::LoadSound(std::wstring fname)
{
	FILE* pfile;
	/* Open the .wav file in binary mode */
	if (!_wfopen_s(&pfile, fname.c_str(), L"rb")) { 
		throw std::runtime_error("file failed to open"); 
	}

	/* Read the .wav file header */
	WaveHeaderType wav_file_header;
	if (fread(&wav_file_header, sizeof(WaveHeaderType), 1, pfile) != 1) {
		throw std::runtime_error("failed to read .wav file header");
	}

	/* Check File Properties from header */
	const char test[4] { 'R', 'I', 'F', 'F' };
	if (wav_file_header.chunkId != test) {
		throw std::runtime_error(".wav file chunk ID is not RIFF");
	}






}
