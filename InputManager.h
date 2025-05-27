#pragma once

#include "DirectInput.h"
#include <d3dx9.h>
#include <d3d9.h>

#define KEYDOWN(key)	(((m_KeyState[key]) & 0x80) ? true : false)
#define KEYUP(key)		(((m_KeyState[key]) & 0x80) ? false : true)

class CInputManager
{
private:
	LPDIRECT3DDEVICE9   m_pD3ddev;
	CDirectInput		*m_pDirectInput;
	
	BYTE				m_KeyState[256];
	
	HINSTANCE       Inst;
	HWND            m_hWnd; 

public:
	DIMOUSESTATE	m_MouseState;

	bool			m_isUCtrlKey;
	bool			m_isDCtrlKey;
	bool			m_isUAltKey;
	bool			m_isDAltKey;
	bool			m_isUShiftKey;
	bool			m_isDShiftKey;
	bool			m_isUSpaceKey;
	bool			m_isDSpaceKey;
	bool			m_isEnterKey;
	bool			m_isUpKey;
	bool			m_isDownKey;
	bool			m_isLeftKey;
	bool			m_isRightKey;

	bool            m_isUF1;
	bool            m_isUF2;
	bool            m_isUF3;
	bool            m_isUF4;
	bool            m_isDF1;
	bool            m_isDF2;
	bool            m_isDF3;
	bool            m_isDF4;

	bool			m_isLButton;
	bool			m_isRButton;

	LONG			m_MouseXPos;
	LONG			m_MouseYPos;
	
	bool			m_isPrevLButton;

	LONG			m_PrevMouseXPos;
	LONG			m_PrevMouseYPos;

	float			m_XMove;
	float			m_YMove;
	float			m_WheelMove;

	bool			m_isUseLButton;

public:
	CInputManager(HINSTANCE,HWND,LPDIRECT3DDEVICE9);
	~CInputManager(void);

	void InputInit();
	void UpdateInputState();
	void FrameMove();
};
extern CInputManager *m_pInputManager;