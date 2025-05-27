#include "Direct3D.h"

#include "DirectInput.h"


CDirectInput::CDirectInput()
{
	m_pDi = NULL;
	m_pDidevKey = NULL;
	m_pDidevMouse = NULL;
}

CDirectInput::~CDirectInput()
{
	Release();
}

bool CDirectInput::InitDirectInput(HINSTANCE hInst, HWND hWnd)
{
	if(FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDi, NULL)))
	{
		return false;
	}
	
	if(FAILED(m_pDi->CreateDevice(GUID_SysKeyboard, &m_pDidevKey, NULL)))
		return false;
	if(FAILED(m_pDidevKey->SetDataFormat(&c_dfDIKeyboard)))
		return false;
	if(FAILED(m_pDidevKey->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY)))
		return false;
	while(m_pDidevKey->Acquire() == DIERR_INPUTLOST);
	
	if(FAILED(m_pDi->CreateDevice(GUID_SysMouse, &m_pDidevMouse, NULL)))
		return false;
	if(FAILED(m_pDidevMouse->SetDataFormat(&c_dfDIMouse)))
		return false;
	if(FAILED(m_pDidevMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		return false;
	while(m_pDidevMouse->Acquire() == DIERR_INPUTLOST);
	
	return true;
}

void CDirectInput::Release()
{
	if(m_pDi)
	{
		if(m_pDidevKey)
		{
			m_pDidevKey->Unacquire();
			SAFE_RELEASE_(m_pDidevKey);
		}
		if(m_pDidevMouse)
		{
			m_pDidevMouse->Unacquire();
			SAFE_RELEASE_(m_pDidevMouse);
		}
		
		SAFE_RELEASE_(m_pDi);
	}
}

bool
CDirectInput::KeyProcess(BYTE key)
{
	ZeroMemory(m_KeyState, sizeof(m_KeyState));
	
	m_hr = m_pDidevKey->GetDeviceState(sizeof(m_KeyState), m_KeyState);
	
	if(FAILED(m_hr))
	{
		m_hr = m_pDidevKey->Acquire();
		while(m_hr == DIERR_INPUTLOST)
			m_pDidevKey->Acquire();
		
		return false;
	}
	
	if(m_KeyState[key] & 0x80)
	{
		return true;
	}
	
	return false;
}

bool
CDirectInput::KeyProcess(BYTE *key)
{
	ZeroMemory(key, 256);
	
	m_hr = m_pDidevKey->GetDeviceState(256, key);
	
	if(FAILED(m_hr))
	{
		m_hr = m_pDidevKey->Acquire();
		while(m_hr == DIERR_INPUTLOST)
			m_pDidevKey->Acquire();
		
		return false;
	}
	
	return true;
}

bool CDirectInput::MouseProcess(DIMOUSESTATE *MouseState)
{
	ZeroMemory(MouseState, sizeof(DIMOUSESTATE));
	
	m_hr = m_pDidevMouse->GetDeviceState(sizeof(DIMOUSESTATE), MouseState);
	
	if(FAILED(m_hr))
	{
		m_hr = m_pDidevMouse->Acquire();
		while(m_hr == DIERR_INPUTLOST)
			m_pDidevMouse->Acquire();
		
		return false;
	}
	
	return true;
}

void CDirectInput::DeviceAcquire()
{
	if(m_pDidevKey)
		m_pDidevKey->Acquire();
	if(m_pDidevMouse)
		m_pDidevMouse->Acquire();
}

void CDirectInput::DeviceUnacquire()
{
	if(m_pDidevKey)
		m_pDidevKey->Unacquire();
	if(m_pDidevMouse)
		m_pDidevMouse->Acquire();
}
