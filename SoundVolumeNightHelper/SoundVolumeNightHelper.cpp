// SoundVolumeNightHelper.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "SoundVolumeNightHelper.h"

static const int MAX_LOADSTRING = 100;

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

#pragma region My functions
HWND mainhWnd;
IniHelper iniHelper;
TimerHelper timer;
static UINT timerInterval = 60000;//1000 * 60 * 10;
SoundAPIHelper soundManager;
TrayIconHelper trayIcon;
LRESULT timer_proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
HMENU  hPopupMenu = nullptr; // Popup Menu descriptor
void createPopupMenu();
INT_PTR CALLBACK trayMenuProc(const HWND &handle, const UINT &message, const WPARAM &wParam, const LPARAM &lParam);

#define IDT_TIMER1 WM_USER+1 // Timer ID
#define WM_TRAYICON WM_USER+2
#define ID_TRAY     WM_USER+3
#ifdef WORD_TIME_ENABLED
static WORD minuteDelta = 2;
static WORD hour = 23;
static WORD minutes = 42;
#else
static int minuteDelta = 2;
static int time = 0;
#endif
static float nightVolume = 0.52f;
static float volumeEpsilon = 0.01f;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void MsgBoxFromHRESULT(LONG result)
{
	wchar_t buf[MAX_PATH] = { 0 };
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, L"", result, NULL, buf, MAX_PATH, nullptr);
	MessageBoxW(NULL, buf,
		szWindowClass,
		MB_OK | MB_ICONWARNING);
}


void MsgBoxLastError()
{
	wchar_t buf[MAX_PATH];
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, MAX_PATH, nullptr);
	MessageBoxW(NULL, buf,
		szWindowClass,
		MB_OK | MB_ICONWARNING);
}

void setMenuEnabled(HMENU hMenu, UINT item, bool status)
{
	MENUITEMINFOW info;
	info.fMask = MIIM_STATE;
	info.cbSize = sizeof(MENUITEMINFOW);
	if (GetMenuItemInfoW(hMenu, item, FALSE, &info)) {
		info.fMask = MIIM_STATE;
		if (status) {
			info.fState = MFS_ENABLED;
		} else {
			info.fState = MFS_DISABLED;
		}
		SetMenuItemInfo(hMenu, item, FALSE, &info);
	} else {
		MsgBoxLastError();
	}
}

void createPopupMenu() {
	if(hPopupMenu) return;
	hPopupMenu = LoadMenuW(hInst, MAKEINTRESOURCEW(IDR_MENU1));
	if (!hPopupMenu) {
		MsgBoxLastError();
		return;
	}
	if (IsAutoRunExists(szWindowClass)) {
#ifdef _DEBUG
		OutputDebugStringW(L"Autorun enabled\n");
#endif
		setMenuEnabled(hPopupMenu, ID_AUTOSTART_ADDTOAUTOSTART, false);
		setMenuEnabled(hPopupMenu, ID_AUTOSTART_REMOVEFROMAUTOSTART, true);
	} else {
#ifdef _DEBUG
		OutputDebugStringW(L"Autorun disabled\n");
#endif
		setMenuEnabled(hPopupMenu, ID_AUTOSTART_ADDTOAUTOSTART, true);
		setMenuEnabled(hPopupMenu, ID_AUTOSTART_REMOVEFROMAUTOSTART, false);
	}
}

void readConfig()
{
	wchar_t ourPath[MAX_PATH];
	const DWORD result = ::GetModuleFileNameW(
		nullptr,    // retrieve path of current process .EXE
		ourPath,
		MAX_PATH
	);
	std::wostringstream fileNameStream;
	fileNameStream << std::wstring(ourPath) << std::wstring(L".ini");
	std::wstring fileName = fileNameStream.str();
	std::wstring appName(szWindowClass);
	iniHelper.setFile(fileName);
	nightVolume = iniHelper.getPrivateProfileFloat(appName, L"NightVolume", nightVolume);
#ifdef _DEBUG
	OutputDebugStringW(fileName.c_str());
	std::wostringstream stringStream;
	stringStream << L"NightVolume=" << nightVolume << std::endl;
	OutputDebugStringW(stringStream.str().c_str());
#endif
	volumeEpsilon = iniHelper.getPrivateProfileFloat(appName, L"VolumeEpsilon", volumeEpsilon);
	timerInterval = iniHelper.getPrivateProfileInt(appName, L"TimerIntervalInMsec", timerInterval);
#ifdef WORD_TIME_ENABLED
	static WORD minuteDelta = 2;
	static WORD hour = 23;
	static WORD minutes = 42;
#else
	int hour = iniHelper.getPrivateProfileInt(appName, L"Hour", 23);
	int min = iniHelper.getPrivateProfileInt(appName, L"Minute", 0);
	minuteDelta = iniHelper.getPrivateProfileInt(appName, L"MinuteDelta", 2);
	time = hour*60+min;
#endif
}
#pragma endregion

#pragma region REGISTRY functions
void AddToAutoRun()
{
	wchar_t ourPath[MAX_PATH];
	const DWORD result = ::GetModuleFileNameW(
		nullptr,    // retrieve path of current process .EXE
		ourPath,
		MAX_PATH
	);
	if (result == 0)
	{
		// Error
		const DWORD error = ::GetLastError();
		/*MessageBoxW(NULL,
			LoadStringFromResource(IDS_STRING_ERR_CantGetPathToExe, hInst).c_str(),
			szWindowClass,
			MB_ICONWARNING);*/
		return;
	}
	const LSTATUS status = AddToAutoRun(szWindowClass, ourPath);
	if (status != ERROR_SUCCESS) {
		MsgBoxFromHRESULT(result);
	}
}

void RemoveFromAutorun()
{
	const LSTATUS status = RemoveFromAutorun(szWindowClass);
	if (status != ERROR_SUCCESS) {
		MsgBoxFromHRESULT(status);
	}
}
#pragma endregion

bool showTaskBar()
{
	const HICON icon = LoadIconW(hInst, MAKEINTRESOURCE(IDI_SOUNDVOLUMENIGHTHELPER));
	return trayIcon.init(mainhWnd, ID_TRAY, WM_TRAYICON, icon);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SOUNDVOLUMENIGHTHELPER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SOUNDVOLUMENIGHTHELPER));

    MSG msg;
#ifndef _DEBUG
	ShowWindow(mainhWnd, SW_HIDE);
#endif
	readConfig();
	timer.setHWND(mainhWnd);
	timer.setIDEvent(IDT_TIMER1);
	timer.start(timerInterval);
	soundManager.init();
	//
	const bool status = showTaskBar();
	if (!status) {
		return -1;
	}
	trayIcon.setToolTip(szTitle);

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
	timer.stop();
	soundManager.unInit();
	trayIcon.destroy();
    return static_cast<int>(msg.wParam);
}


#pragma region default init WInAPI window
//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SOUNDVOLUMENIGHTHELPER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SOUNDVOLUMENIGHTHELPER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   mainhWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!mainhWnd)
   {
      return FALSE;
   }

   ShowWindow(mainhWnd, nCmdShow);
   UpdateWindow(mainhWnd);

   return TRUE;
}
#pragma endregion

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND — обработать меню приложения
//  WM_PAINT — отрисовать главное окно
//  WM_DESTROY — отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static UINT s_uTaskbarRestart;
    switch (message)
    {
    case WM_COMMAND:
        {
            const int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
#pragma region Tray Icon commands
			case ID_TRAYMENU_EXIT:
				DestroyWindow(hWnd);
				//Also we can call: SendMessage(hwnd, WM_DESTROY, 0, 0);
				break;
			case ID_AUTOSTART_ADDTOAUTOSTART:
				AddToAutoRun();
				setMenuEnabled(hPopupMenu, ID_AUTOSTART_ADDTOAUTOSTART, false);
				setMenuEnabled(hPopupMenu, ID_AUTOSTART_REMOVEFROMAUTOSTART, true);
				break;
			case ID_AUTOSTART_REMOVEFROMAUTOSTART:
				RemoveFromAutorun();
				setMenuEnabled(hPopupMenu, ID_AUTOSTART_ADDTOAUTOSTART, true);
				setMenuEnabled(hPopupMenu, ID_AUTOSTART_REMOVEFROMAUTOSTART, false);
				break;
			case ID_TRAYMENU_ABOUT: {
				const std::wstring about = LoadStringFromResource(IDS_STRING_About, hInst);
				MessageBoxW(NULL, about.c_str(),
					szTitle, MB_OK | MB_ICONINFORMATION);
			}
			break;
#pragma endregion
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		DestroyMenu(hPopupMenu);
        PostQuitMessage(0);
        break;
	case WM_TIMER:
		return timer_proc(hWnd, message, wParam, lParam);
		break;
	case WM_TRAYICON:
		return trayMenuProc(hWnd, message, wParam, lParam);
		break;
	case WM_CONTEXTMENU:
	case WM_CREATE:
		s_uTaskbarRestart = RegisterWindowMessage(TEXT("TaskbarCreated"));
		createPopupMenu();
#ifndef _DEBUG
		trayIcon.showBaloonMessage(szTitle, L"Hello world", NULL);
#endif
		break;
    default:
		// TaskBar created after crash.
		if(message == s_uTaskbarRestart) {
			showTaskBar();
		}
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

LRESULT timer_proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (wParam == IDT_TIMER1)
	{
		SYSTEMTIME lt = { 0 };
		GetLocalTime(&lt);
#ifdef WORD_TIME_ENABLED
		if ((hour == lt.wHour && absS(lt.wMinute - minutes) < minuteDelta) ||
			(hour == (lt.wHour - 1) && (60 - lt.wMinute) < minuteDelta)
			)
#else
		const int currentTime = static_cast<int>(lt.wHour) * 60 + static_cast<int>(lt.wMinute);
		if(abs(currentTime-time) < minuteDelta)
#endif
		{
			//It happend!
			const float currentVolume = soundManager.getMasterVolumeLevelScalar();
			if (nightVolume < currentVolume + volumeEpsilon) {
				soundManager.setMasterVolumeLevelScalar(nightVolume - volumeEpsilon);
			}
		}
	}
	return 0;
}

inline void ShowContextMenu(HWND hWnd)
{
#ifdef _DEBUG
	OutputDebugStringW(L"ShowContextMenu\n");
#endif
	POINT curPoint;
	GetCursorPos(&curPoint);

	// should SetForegroundWindow according
	// to original poster so the popup shows on top
	SetForegroundWindow(hWnd);
	TrackPopupMenu(
		// Fix Bug if menu loded from resources.
		GetSubMenu(hPopupMenu, 0),
		TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		curPoint.x,
		curPoint.y,
		0,
		hWnd,
		nullptr
	);
}

INT_PTR CALLBACK trayMenuProc(const HWND &hWnd, const UINT &message, const WPARAM &wParam, const LPARAM &lParam)
{
#ifdef _DEBUG
	OutputDebugStringW(L"trayMenuProc\n");
	std::wostringstream st;
	st << "message=" << message - WM_USER << L" "<< std::hex<< message<< std::endl
		<<L"lParam="<<std::hex<<lParam<< std::endl;
	std::wstring s = st.str();
	OutputDebugStringW(s.c_str());
#endif
	switch (message)
	{
	case WM_TRAYICON: {
		const int wm = LOWORD(lParam);
		switch (wm)
		{
		case WM_LBUTTONDBLCLK:
			break;
		case WM_RBUTTONDOWN:
		case WM_CONTEXTMENU:
			ShowContextMenu(mainhWnd);
			break;
		default:
			return DefWindowProcW(hWnd, message, wParam, lParam);
		}
	}
		break;
	default:
#ifdef _DEBUG
		OutputDebugStringW(L"default\n");
#endif
		return DefWindowProcW(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}

