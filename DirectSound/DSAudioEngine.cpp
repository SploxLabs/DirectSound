#include "DSAudioEngine.h"

DSAudioEngine::DSAudioEngine() :
	main_window(0),
	direct_sound(0),
	primary_buffer(0),
	secondary_buffer(0)
{
}

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
	//release the primary buffer
	secondary_buffer->Release();
	//release the direct sound interface pointer
	direct_sound->Release();

}

void DSAudioEngine::LoadSound(std::wstring fname)
{
	FILE* file_ptr;
	std::wstring error_message;
	bool acceptable = true;
	/* Open the .wav file in binary mode */
	if (_wfopen_s(&file_ptr, fname.c_str(), L"rb")) {
		error_message = L"Failed to open: " + fname + L"\n";
		OutputDebugStringW(error_message.c_str());
		acceptable = false;
	}

	/* Build Test Header */
	//"What is an acceptable .wav file"
	WaveHeaderType test_header;
	test_header.chunkId[0] = 'R';
	test_header.chunkId[1] = 'I';
	test_header.chunkId[2] = 'F';
	test_header.chunkId[3] = 'F';
	test_header.format[0] = 'W';
	test_header.format[1] = 'A';
	test_header.format[2] = 'V';
	test_header.format[3] = 'E';
	test_header.subChunkId[0] = 'f';
	test_header.subChunkId[1] = 'm';
	test_header.subChunkId[2] = 't';
	test_header.subChunkId[3] = ' ';
	test_header.audioFormat = WAVE_FORMAT_PCM;
	test_header.numChannels = 2;
	test_header.sampleRate = 44100;
	test_header.bitsPerSample = 16;
	test_header.dataChunkId[0] = 'd';
	test_header.dataChunkId[1] = 'a';
	test_header.dataChunkId[2] = 't';
	test_header.dataChunkId[3] = 'a';

	/* Read the .wav file header */
	WaveHeaderType wav_file_header;
	if (fread(&wav_file_header, sizeof(wav_file_header), 1, file_ptr) != 1) {
		error_message = L"Failed to read header of: " + fname + L"\n";
		OutputDebugStringW(error_message.c_str());
		acceptable = false;
	}

	/* Compare File Headers to see if file is acceptable */
	
	//Check Chunk ID
	if (wav_file_header.chunkId != test_header.chunkId) {
		error_message = L"File: " + fname + L" has unacceptable: Chunk ID\n";
		OutputDebugStringW(error_message.c_str());
		acceptable = false;
	}

	//Check Format
	if (wav_file_header.format != test_header.format) {
		error_message = L"File: " + fname + L" has unacceptable: format\n";
		OutputDebugStringW(error_message.c_str());
		acceptable = false;
	}

	//Check Sub Chunk ID
	if (wav_file_header.subChunkId != test_header.subChunkId) {
		error_message = L"File: " + fname + L" has unacceptable: Sub Chunk ID\n";
		OutputDebugStringW(error_message.c_str());
		acceptable = false;
	}

	//Check Audio Format
	if (wav_file_header.audioFormat != test_header.audioFormat) {
		error_message = L"File: " + fname + L" has unacceptable: Audio Format\n";
		OutputDebugStringW(error_message.c_str());
		acceptable = false;
	}

	//Check Stero/mono
	if (wav_file_header.numChannels != test_header.numChannels) {
		error_message = L"File: " + fname + L" has unacceptable: Number of Channels\n";
		OutputDebugStringW(error_message.c_str());
		acceptable = false;
	}

	//Check SampleRate
	if (wav_file_header.sampleRate != test_header.sampleRate) {
		error_message = L"File: " + fname + L" has unacceptable: Sample Rate\n";
		OutputDebugStringW(error_message.c_str());
		acceptable = false;
	}

	//Check Bit Depth
	if (wav_file_header.bitsPerSample != test_header.bitsPerSample) {
		error_message = L"File: " + fname + L" has unacceptable: Bit Depth\n";
		OutputDebugStringW(error_message.c_str());
		acceptable = false;
	}

	//Check Data Chunk ID
	if (wav_file_header.dataChunkId != test_header.dataChunkId) {
		error_message = L"File: " + fname + L" has unacceptable: Data Chunk ID\n";
		OutputDebugStringW(error_message.c_str());
		acceptable = false;
	}

	//we exit with error here since we could have multiple errors reported above,
	// save time from "ERROR" fix "ERROR" fix "ERROR" fix, now it will be
	// "ERROR, ERROR, ERROR", fix errors, done!
	if (!acceptable) { exit(533); } 

	/* Create Secondary Buffer */
	WAVEFORMATEX wav_format;
	wav_format.wFormatTag = WAVE_FORMAT_PCM;
	wav_format.nSamplesPerSec = 44100;
	wav_format.wBitsPerSample = 16;
	wav_format.nChannels = 2;
	wav_format.nBlockAlign = (wav_format.wBitsPerSample / 8) * wav_format.nChannels;
	wav_format.nAvgBytesPerSec = wav_format.nSamplesPerSec * wav_format.nBlockAlign;
	wav_format.cbSize = 0;

	DSBUFFERDESC buffer_desc;
	buffer_desc.dwSize = sizeof(DSBUFFERDESC);
	buffer_desc.dwFlags = DSBCAPS_CTRLVOLUME;
	buffer_desc.dwBufferBytes = wav_file_header.dataSize;
	buffer_desc.dwReserved = 0;
	buffer_desc.lpwfxFormat = &wav_format;
	buffer_desc.guid3DAlgorithm = GUID_NULL;

	IDirectSoundBuffer* tmp_buffer;

	ThrowIfFailed(direct_sound->CreateSoundBuffer(&buffer_desc, &tmp_buffer, NULL));
	ThrowIfFailed(tmp_buffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondary_buffer));
	
	tmp_buffer->Release();
}

void DSAudioEngine::PlayLoadedSound()
{
}
