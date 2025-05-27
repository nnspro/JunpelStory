#pragma once

#include "Direct3D.h"
#include "FrameCheck.h"
#include "BGManager.h"
#include "Interface.h"
#include "Cursor.h"
#include "d3dfont.h"
#include "Title.h"

#define     JPS_TITLE       200
#define     JPS_INIT        300
#define     JPS_LOOP        400
#define     JPS_GAME        500
#define     JPS_NEXTLOADING 600
#define     JPS_PREVLOADING 700
#define     JPS_GOTOVILLIGE 800

class CGameManager
{
public:
	CGameManager();
	~CGameManager();

	BOOL Init_JunePle_Game(HINSTANCE,HWND);

	void Render();
	void GameState();
	void InitFrame();
public:
	float				m_framerate;
	float				m_currentframe;
	float				m_framecount;
	float				m_totalframe;
	int					m_totalcount;
	//int                 gameState;
public:
	CDirect3D        *m_pDirect3D;
	CFrameCheck		 *m_pFrameCheck;
	cScreenManager   *m_pScreenManager;
	CInterface       *m_pInterface;
	CD3DFont         *m_p3dFont;
 	cMouseCursor     *m_pCursor;
	cTitle           *m_pTitle;
};
