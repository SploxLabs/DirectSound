#include "Win32App.h"

Win32App* Win32App::m_self = nullptr;

Win32App::Win32App(HINSTANCE hInstance) : m_hIn(hInstance) {
    m_self = this;
}

int Win32App::Run() {
    ShowWindow(m_hwnd, SW_SHOW);

    MSG msg = {};
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            //do engine stuff, render graphics, change volumes based on time, etc...
        }
    }

    OnDestroy();
    return 0;
}

void Win32App::OnInit(int width, int height) {
    m_client_width = width;
    m_client_height = height;
    //define window class we are going to register and use
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc = CallAppProc;
    wcex.hInstance = m_hIn;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hIcon = LoadIcon(NULL, IDI_QUESTION);
    wcex.hIconSm = NULL;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"wcex";
    wcex.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClassEx(&wcex)) { exit(1); } //register class

    RECT window_rect = { 0, 0, static_cast<LONG>(m_client_width), static_cast<LONG>(m_client_height) }; //set window rect to our desired client size
    AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, FALSE); //adjust the rect to describe a window area 

    m_hwnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        wcex.lpszClassName,
        L"Hey Look a Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        window_rect.right - window_rect.left, window_rect.bottom - window_rect.top,
        nullptr,
        nullptr,
        m_hIn,
        nullptr);

    if (!m_hwnd) { exit(1); }

    m_audio_engine.Init(m_hwnd);

    m_audio_engine.LoadSound("test.wav");
}

void Win32App::OnDestroy()
{
}

LRESULT Win32App::AppProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_SIZE: {
    }
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
        return 0;
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
        return 0;
    case WM_MOUSEMOVE:
        return 0;

    case WM_KEYDOWN:
        switch (wparam)
        {
        case VK_SPACE:
        {
            playing_sound = !playing_sound;
            if (playing_sound) {
                m_audio_engine.PlayLoadedSound();
            }
            else {
                m_audio_engine.StopPlayingLoadedSound();
            }
        }
        case VK_ESCAPE:
            return 0;
        case VK_F1:
            break;
        case VK_F2:
            break;
        case VK_F3:
            break;
        case VK_F4:
            break;
        case VK_F5: {
            break;
        }
        case VK_F6: {
            break;
        }

        case VK_UP: {
            return 0;
        }

        case VK_DOWN: {
            return 0;
        }
        case VK_LEFT:
            return 0;
        case VK_RIGHT:
            break;

        case 0x30: //0 key
            break;
        case 0x31: //1 key
            break;
        case 0x32: //2 key
            break;
        case 0x33: //3 key
            break;
        case 0x34: //4 key
            break;
        case 0x35: //5 key
            break;
        case 0x36: //6 key
            break;
        case 0x37: //7 key
            break;
        case 0x38: //8 key
            break;
        case 0x39: //9 key
            break;
        case 0x41: //A key
            break;
        case 0x42: //B key
            break;
        case 0x43: //C key
             break;
        case 0x44: //D key
            break;
        case 0x45: //E key
            break;
        case 0x46: //F key
            break;
        case 0x47: //G key
            break;
        case 0x48: //H key
             break;
        case 0x49: //I key
            break;
        case 0x4A: //J key
            break;
        case 0x4B: //K key
             break;
        case 0x4C: //L key
            break;
        case 0x4D: //M key
            break;
        case 0x4E: //N key
            break;
        case 0x4F: //O key
            break;
        case 0x50: //P key
            break;
        case 0x51: //Q key
            break;
        case 0x52: //R key
            break;
        case 0x53: //S key
            break;
        case 0x54: //T key
            break;
        case 0x55: //U key
            break;
        case 0x56: //V key
            break;
        case 0x57: //W key
            break;
        case 0x58: //X key
            break;
        case 0x59: //Y key
            break;
        case 0x5A: //Z key
            break;
        case VK_RETURN: //ENTER key
            break;
        case VK_BACK: //BACKSPACE key
             break;
        case VK_OEM_COMMA: //For any country/region, the '+' key
            break;
        case VK_OEM_PLUS: //For any country/region, the '+' key
            break;
        case VK_OEM_PERIOD: //.> key
            break;
        case VK_OEM_MINUS: //.> key
            break;
        case VK_OEM_2: //.> key
            break;
        case VK_OEM_3: {
            break;
        }
        default:
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        break;
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}
