//#include "stdafx.h"

#include "InputManager.h"

CInputManager::CInputManager(HINSTANCE h_inst,HWND hh,LPDIRECT3DDEVICE9 pD3ddev)
{
	Inst   = h_inst;
	m_hWnd = hh; 

	m_pDirectInput = NULL;
	m_pDirectInput = new CDirectInput();
	m_pDirectInput->InitDirectInput(h_inst,hh);

	m_pD3ddev = pD3ddev;

	InputInit();
}

CInputManager::~CInputManager(void)
{
}


void
CInputManager::InputInit()
{
	m_isEnterKey    = false;
	m_isUCtrlKey    = false;
	m_isDCtrlKey    = false;
	m_isUAltKey     = false;
	m_isDAltKey     = false;
	m_isUShiftKey   = false;
	m_isDShiftKey   = false;
	m_isUSpaceKey   = false;
	m_isDSpaceKey   = false;
	m_isEnterKey    = false;
	m_isUpKey       = false;
	m_isDownKey     = false;
	m_isLeftKey     = false;
	m_isRightKey    = false;
	
	m_isUF1          = false;
	m_isUF2          = false;
	m_isUF3          = false;
	m_isUF4          = false;
	m_isDF1          = false;
	m_isDF2          = false;
	m_isDF3          = false;
	m_isDF4          = false;

	m_isLButton = false;
	m_isRButton = false;

	m_MouseXPos = 0L;
	m_MouseYPos = 0L;

	m_isPrevLButton = false;

	m_PrevMouseXPos = 0L;
	m_PrevMouseYPos = 0L;

	m_XMove = 0.0f;
	m_YMove = 0.0f;
	m_WheelMove = 0.0f;

	m_isUseLButton = false;
}

void
CInputManager::UpdateInputState()
{
	m_isPrevLButton = m_isLButton;

	m_PrevMouseXPos = m_MouseXPos;
	m_PrevMouseYPos = m_MouseYPos;
	
	m_isDCtrlKey    = false;
	m_isDAltKey     = false;
	m_isDShiftKey   = false;
	m_isDSpaceKey   = false;
	m_isEnterKey    = false;
	m_isUpKey       = false;
	m_isDownKey     = false;
	m_isLeftKey     = false;
	m_isRightKey    = false;

	m_isDF1          = false;
	m_isDF2          = false;
	m_isDF3          = false;
	m_isDF4          = false;

	m_isLButton = false;
	m_isRButton = false;

	m_XMove = 0.0f;
	m_YMove = 0.0f;
	m_WheelMove = 0.0f;

	m_isUseLButton = false;
}

void
CInputManager::FrameMove()
{
	UpdateInputState();

	POINT ptCursor;

	GetCursorPos(&ptCursor);
	ScreenToClient(m_hWnd, &ptCursor);

	if(ptCursor.x >= 5L && ptCursor.x <= 800 - 5L)
		m_MouseXPos = ptCursor.x;
	else if(ptCursor.x < 5L)
		m_MouseXPos = 5L;
	else if(ptCursor.x > 800 - 5L)
		m_MouseXPos = 800 - 5L;

	if(ptCursor.y >= 5L && ptCursor.y <= 600 - 5L)
		m_MouseYPos = ptCursor.y;
	else if(ptCursor.y < 5L)
		m_MouseYPos = 5L;
	else if(ptCursor.y > 600 - 5L)
		m_MouseYPos = 600 - 5L;

	m_pD3ddev->SetCursorPosition(m_MouseXPos, m_MouseYPos, 0L);

	if(!m_pDirectInput->KeyProcess(m_KeyState))
		return;
	if(!m_pDirectInput->MouseProcess(&m_MouseState))
		return;

	if(m_MouseState.rgbButtons[0])
		m_isLButton = true;
	//else m_isLButton = false;

	if(m_MouseState.rgbButtons[1])
		m_isRButton = true;

	m_XMove += (float)m_MouseState.lX;
	m_YMove += (float)m_MouseState.lY;
	m_WheelMove += (float)m_MouseState.lZ;
	
	if(KEYDOWN(DIK_LALT))
		m_isDAltKey = true;
	if(KEYDOWN(DIK_LCONTROL))
		m_isDCtrlKey = true;
	//else //if(KEYUP(DIK_LCONTROL))
		//m_isUCtrlKey = true;
	if(KEYDOWN(DIK_LSHIFT))
		m_isDShiftKey = true;
	if(KEYDOWN(DIK_SPACE))
		m_isDSpaceKey = true;
	if(KEYDOWN(DIK_RETURN))
		m_isEnterKey = true;
	if(KEYDOWN(DIK_UP))
		m_isUpKey = true;
	if(KEYDOWN(DIK_DOWN))
		m_isDownKey = true;
	if(KEYDOWN(DIK_LEFT))
		m_isLeftKey = true;
	if(KEYDOWN(DIK_RIGHT))
		m_isRightKey = true;
	if(KEYDOWN(DIK_F1))
		m_isDF1 = true;
	if(KEYDOWN(DIK_F2))
		m_isDF2 = true;
	if(KEYDOWN(DIK_F3))
		m_isDF3 = true;
	if(KEYDOWN(DIK_F4))
		m_isDF4 = true;
}
