#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <Windows.h>

inline WORD absS(WORD value)
{
	if (value < 0)
		return -1 * value;
	return value;
}

std::wstring LoadStringFromResource(
	__in UINT stringID,
	__in_opt HINSTANCE instance = NULL)
{
	wchar_t * pBuf = nullptr;

	const int len = LoadStringW(
		instance,
		stringID,
		reinterpret_cast< wchar_t * >(&pBuf),
		0);

	if (len)
		return std::wstring(pBuf, len);
	else
		return std::wstring();
}
