#include "stdafx.h"
#include "TrayIconHelper.h"


TrayIconHelper::TrayIconHelper()
{
	nid = { 0 };
}


TrayIconHelper::~TrayIconHelper()
{
	Shell_NotifyIconW(NIM_DELETE, &nid);
}

bool TrayIconHelper::destroy()
{
	return Shell_NotifyIconW(NIM_DELETE, &nid);
}

bool TrayIconHelper::init(HWND hWnd, UINT uID, UINT callbackMessage, HICON icon)
{
	nid.cbSize = sizeof(nid);
	nid.hWnd = hWnd;
	nid.uID = uID; // It conflict with GUID
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	//nid.uFlags = NIF_ICON | NIF_TIP | NIF_GUID  | NIF_SHOWTIP;
	nid.uVersion = NOTIFYICON_VERSION_4;
	nid.hIcon = icon;
	nid.uCallbackMessage = callbackMessage;
	if (nid.hIcon == nullptr)
	{
		return false;
	}

	return Shell_NotifyIconW(NIM_ADD, &nid);
}

bool TrayIconHelper::setToolTip(const std::wstring &toolTip)
{
	wcscpy_s(nid.szTip, CONSTANTS::TRAY_ICON_MAX_BALLOON_MSG_LENGTH, toolTip.c_str());
	return Shell_NotifyIconW(NIM_MODIFY, &nid);
}

bool TrayIconHelper::showBaloonMessage(const std::wstring &title, const std::wstring & message, HICON icon)
{
	wcscpy_s(nid.szInfo, CONSTANTS::TRAY_ICON_MAX_BALLOON_MSG_LENGTH, message.c_str());
	wcscpy_s(nid.szInfoTitle, CONSTANTS::TRAY_ICON_MAX_BALLOON_TITLE_LENGTH, title.c_str());
	nid.hBalloonIcon = icon;
	nid.uFlags = nid.uFlags | NIF_INFO;
	nid.dwInfoFlags = NIIF_INFO;
	return Shell_NotifyIconW(NIM_MODIFY, &nid);
}

bool TrayIconHelper::setToolTip(const wchar_t *toolTip)
{
	if (toolTip) {
		wcscpy_s(nid.szTip, CONSTANTS::TRAY_ICON_MAX_BALLOON_MSG_LENGTH, toolTip);
		nid.uFlags = nid.uFlags | NIF_SHOWTIP;
		return Shell_NotifyIconW(NIM_MODIFY, &nid);
	}
	return false;
}

bool TrayIconHelper::showBaloonMessage(const wchar_t *title, const wchar_t *message, HICON icon)
{
	if(message)
	wcscpy_s(nid.szInfo, CONSTANTS::TRAY_ICON_MAX_BALLOON_MSG_LENGTH, message);
	if(title)
	wcscpy_s(nid.szInfoTitle, CONSTANTS::TRAY_ICON_MAX_BALLOON_TITLE_LENGTH, title);
	nid.hBalloonIcon = icon;
	nid.uFlags = nid.uFlags | NIF_INFO;
	nid.dwInfoFlags = NIIF_INFO;
	return Shell_NotifyIconW(NIM_MODIFY, &nid);
}
