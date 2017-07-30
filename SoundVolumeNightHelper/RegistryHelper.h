#pragma once
#include <windows.h>
#include <Intsafe.h>
#include <Strsafe.h>

LSTATUS AddToAutoRun(const wchar_t *name, const wchar_t *path)
{
	wchar_t buf[MAX_PATH] = { 0 };
	HKEY key = nullptr;
	LSTATUS status = RegOpenKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &key);
	if (status != ERROR_SUCCESS) {
		return status;
	}
	swprintf_s(buf, MAX_PATH, L"\"%s\"", path);
	int size = 0;
	SizeTToInt(wcslen(buf) * sizeof(wchar_t) + sizeof(wchar_t), &size);
	status = RegSetValueExW(key, name, 0, REG_SZ, reinterpret_cast<LPBYTE>(buf), size);
	// Next command create subfolder for "Run".
	//HKEY k;
	//status = RegCreateKeyExW(key, name, 0, 0 ,REG_SZ, KEY_ALL_ACCESS | KEY_WOW64_64KEY,
	//	NULL,
	//	&k,
	//	NULL
	//	);
	//status = RegSetValueExW(k, name, 0, REG_SZ, reinterpret_cast<LPBYTE>(buf), size);
	//RegCloseKey(k);
	RegCloseKey(key);
	return status;
}

LSTATUS RemoveFromAutorun(const wchar_t *name)
{
	HKEY hkey = nullptr;
	LSTATUS status = RegOpenKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
		0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hkey);
	if (status != ERROR_SUCCESS) {
		return status;
	}
	//In next command we really find subfolder for Run:
	//result = RegDeleteKeyExW(hkey, name, KEY_WOW64_64KEY, 0);
	status = RegDeleteValueW(hkey, name);
	RegCloseKey(hkey);
	return status;
}

bool IsAutoRunExists(const wchar_t *name)
{
	HKEY hkey = nullptr;
	const LSTATUS status = RegOpenKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
		0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &hkey);
	if (status != ERROR_SUCCESS) {
		return false;
	}
	wchar_t buf[MAX_PATH] = { 0 };
	DWORD cbData;
	bool ret = false;
	const LONG result = RegQueryValueExW(hkey, name, NULL, NULL, reinterpret_cast<LPBYTE>(buf), &cbData);
	if (result != ERROR_SUCCESS) {
		if (result == ERROR_MORE_DATA)
			ret = true;
		if (result == ERROR_FILE_NOT_FOUND)
			ret = false;
	} else {
		ret = true;
	}

	RegCloseKey(hkey);
	return ret;
}
