#pragma once

#include <windows.h>
#include <windowsx.h>

class CFrameCheck  
{
private:
    LARGE_INTEGER	m_Start;
    LARGE_INTEGER	m_End;
	LARGE_INTEGER	m_Frequency;
	float			m_Timer;
    float			m_FramePerSec;
	
public:
	CFrameCheck();
	virtual ~CFrameCheck();
	
	float GetFPS();
};