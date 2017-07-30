#pragma once
#include <windows.h>


class TimerHelper
{
public:
	TimerHelper();
	TimerHelper(HWND hWnd, UINT_PTR nIDEvent);
	virtual ~TimerHelper();

	void setHWND(HWND hWnd);
	void setIDEvent(UINT_PTR nIDEvent);
	bool start(UINT_PTR nIDEvent, UINT uElapse);
	bool start(UINT uElapse);
	bool stop();
private:
	HWND _hWnd;
	UINT_PTR _result;
	UINT_PTR _nIDEvent;
};

