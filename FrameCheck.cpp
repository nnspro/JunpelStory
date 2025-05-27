//#include "StdAfx.h"
#include "FrameCheck.h"

CFrameCheck::CFrameCheck()
{ 
	m_Timer = 0.0f;
	m_FramePerSec = 0.0f;
	QueryPerformanceFrequency(&m_Frequency);
}

CFrameCheck::~CFrameCheck()
{
	
}

float CFrameCheck::GetFPS() 
{ 
	QueryPerformanceCounter(&m_End);
	
	m_Timer = (m_End.LowPart-m_Start.LowPart) / (float)m_Frequency.LowPart;
	m_FramePerSec = 1 / m_Timer;
	m_Start = m_End;
	
	return m_FramePerSec; 
}