#pragma once
#include <windows.h>
#include <Shellapi.h>
#include <cstdlib>
#include <string>
#ifdef _DEBUG
#include <sstream>
#endif
#include <wchar.h>
#include <Strsafe.h>

namespace CONSTANTS {
	static const size_t TRAY_ICON_MAX_BALLOON_TITLE_LENGTH = 64;
	static const size_t TRAY_ICON_MAX_BALLOON_MSG_LENGTH = 256;
	static const size_t TRAY_ICON_MAX_TOOLTIP_LENGTH = 64;
}

class TrayIconHelper
{
public:
	TrayIconHelper();
	virtual ~TrayIconHelper();

	bool init(HWND hWnd, UINT uID, UINT callbackMessage, HICON icon);
	bool destroy();
	bool setToolTip(const std::wstring &toolTip);
	bool showBaloonMessage(const std::wstring &title, const std::wstring &message, HICON icon = NULL);
	bool setToolTip(const wchar_t *toolTip);
	bool showBaloonMessage(const wchar_t *title, const wchar_t *message, HICON icon = NULL);

private:
	NOTIFYICONDATAW nid;
};

