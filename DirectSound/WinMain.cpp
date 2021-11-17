#include "Win32App.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

	Win32App app(hInstance);
	app.OnInit();
	return app.Run();
};