#pragma once

#pragma comment (lib, "dinput8.lib")
#include <dinput.h>

class CDirectInput  
{
private:
	LPDIRECTINPUT8			m_pDi;
	LPDIRECTINPUTDEVICE8	m_pDidevKey;
	LPDIRECTINPUTDEVICE8	m_pDidevMouse;
	BYTE					m_KeyState[256];
	HRESULT					m_hr;
	
private:
	void	Release();
	
public:
	CDirectInput();
	virtual ~CDirectInput();
	
	bool	InitDirectInput(HINSTANCE, HWND);
	bool	KeyProcess(BYTE);
	bool	KeyProcess(BYTE *);
	bool	MouseProcess(DIMOUSESTATE *);
	void	DeviceAcquire();
	void	DeviceUnacquire();
};