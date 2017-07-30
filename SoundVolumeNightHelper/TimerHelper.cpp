#include "stdafx.h"
#include "TimerHelper.h"


TimerHelper::TimerHelper() : _hWnd(NULL), _result(NULL), _nIDEvent(NULL)
{
}

TimerHelper::TimerHelper(HWND hWnd, UINT_PTR nIDEvent) : _hWnd(hWnd), _result(NULL), _nIDEvent(nIDEvent)
{
}


TimerHelper::~TimerHelper()
{
	if (_result != 0) {
		stop();
	}
}

void TimerHelper::setHWND(HWND hWnd)
{
	_hWnd = hWnd;
}

void TimerHelper::setIDEvent(UINT_PTR nIDEvent)
{
	_nIDEvent = nIDEvent;
}

bool TimerHelper::start(UINT_PTR nIDEvent, UINT uElapse)
{
	_nIDEvent = nIDEvent;
	_result = SetTimer(_hWnd,             // handle to main window 
		_nIDEvent,            // timer identifier 
		uElapse,                 // interval  in milliseconds
		(TIMERPROC)NULL);     // no timer callback 
	return _result != 0;
}

bool TimerHelper::start(UINT uElapse)
{
	_result = SetTimer(_hWnd,             // handle to main window 
		_nIDEvent,            // timer identifier 
		uElapse,                 // interval  in milliseconds
		(TIMERPROC)NULL);     // no timer callback 
	return _result != 0;
}

bool TimerHelper::stop()
{
	const bool result = KillTimer(_hWnd, _nIDEvent);
	if (result) {
		// Destroy succeed
		_result = NULL;
		return result;
	}
	else return result;
}
