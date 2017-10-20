#ifndef __CTIMER_H__
#define __CTIMER_H__

#include "DLLMain.h"

//class timer cred: v3n

class CTimer
{
public:
	CTimer(void){ dwStop = 0; }
	~CTimer(void) { }
	int Start(float fSec) { dwStop = (DWORD)(fSec * 1000) + GetTickCount(); return dwStop; }
	void Stop(void) { dwStop = 0; }
	float TimeLeft(void) { if (Running()) return (((float)(dwStop - GetTickCount()) / 1000));  return 0; }
	bool Running(void) { return (dwStop > GetTickCount()); }
protected:
	DWORD dwStop;
};
extern CTimer g_Timer;
#endif
