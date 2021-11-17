#pragma once
#include <Windows.h>
#include <string>

#include "DSAudioEngine.h"

class Win32App {
public:
	Win32App(HINSTANCE hInstance);
	int Run(); //run application and return some exit status value

	void OnInit(int width = 900, int height = 900);//initilize application resources 

	std::wstring resources_path;
private:

	void OnDestroy(); //safely release application resources and control

	static Win32App* m_self;
	LRESULT CALLBACK AppProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static LRESULT CALLBACK CallAppProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		return m_self->AppProc(hwnd, msg, wparam, lparam);
	};

	int m_client_width;
	int m_client_height;

	HINSTANCE m_hIn;

	HWND m_hwnd;

	DSAudioEngine m_audio_engine;
	bool playing_sound = false;
};

