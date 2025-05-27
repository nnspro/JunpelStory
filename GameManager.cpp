#include "DirectSound.h"
#include "Object.h"
#include "Tile.h"
#include "MapOutput.h"

#include "MiniMap.h"

#include "InputManager.h"
#include "UnitManager.h"
#include "Help.h"

#include "Missail.h"

#include "Mp3.h"

#include "GameManager.h"

cTileWnd      *m_pTileWnd      = NULL;
cObjectWnd    *m_pObjectWnd    = NULL;
cMapViewWnd   *m_pMapViewWnd   = NULL;
cUnitManager  *m_pUnitManager  = NULL;
CInputManager *m_pInputManager = NULL;
cMiniMap      *m_pMiniMap      = NULL; 
CMySound      *m_pMp3          = NULL;

YK_JinDsound        m_pDSound;
sound               m_psnd;

float m_f_frame = 0.0f;

//HWND hhh;

int                 gameState;
//FILE *fp2;

CGameManager::CGameManager()
{
	m_pDirect3D      = NULL;
	m_pFrameCheck    = NULL;
	m_pScreenManager = NULL;
	m_pInterface     = NULL;
	m_p3dFont        = NULL; 
	m_pTitle         = NULL;

	m_framecount	= 0.0f;
	m_totalframe	= 0.0f;
	m_totalcount	= 0;

	gameState   = JPS_GAME;

	//fp2 = fopen("a,txt","wt");
}

CGameManager::~CGameManager()
{
	SAFE_DELETE_(m_pDirect3D);
	
	SAFE_DELETE_(m_pFrameCheck);
	SAFE_DELETE_(m_pInputManager);
	SAFE_DELETE_(m_pMiniMap);

	SAFE_DELETE_(m_pUnitManager);
	SAFE_DELETE_(m_pMapViewWnd);
	SAFE_DELETE_(m_pTileWnd);
	SAFE_DELETE_(m_pObjectWnd);
	SAFE_DELETE_(m_pScreenManager);
	
	SAFE_DELETE_(m_pInterface);
	SAFE_DELETE_(m_p3dFont);
	SAFE_DELETE_(m_pCursor);
	SAFE_DELETE_(m_pHelp);
	
	SAFE_DELETE_(m_pMp3);
	
	if(m_psnd.jump.bff != NULL) {
		m_psnd.jump.bff->Release();
		m_psnd.jump.bff = NULL;
	}
	if(m_psnd.hitmon.bff != NULL) {
		m_psnd.hitmon.bff->Release();
		m_psnd.hitmon.bff = NULL;
	}
	if(m_psnd.hituser.bff != NULL) {
		m_psnd.hituser.bff->Release();
		m_psnd.hituser.bff = NULL;
	}
	if(m_psnd.holkick.bff != NULL) {
		m_psnd.holkick.bff->Release();
		m_psnd.holkick.bff = NULL;
	}
	if(m_psnd.mugi.bff != NULL) {
		m_psnd.mugi.bff->Release();
		m_psnd.mugi.bff = NULL;
	}
	if(m_psnd.sliding.bff != NULL) {
		m_psnd.sliding.bff->Release();
		m_psnd.sliding.bff = NULL;
	}
	if(m_psnd.mondie.bff != NULL) {
		m_psnd.mondie.bff->Release();
		m_psnd.mondie.bff = NULL;
	}
	if(m_psnd.userdie.bff != NULL) {
		m_psnd.userdie.bff->Release();
		m_psnd.userdie.bff = NULL;
	}
	if(m_psnd.eat.bff != NULL) {
		m_psnd.eat.bff->Release();
		m_psnd.eat.bff = NULL;
	}
	if(m_psnd.mongun.bff != NULL) {
		m_psnd.mongun.bff->Release();
		m_psnd.mongun.bff = NULL;
	}
	if(m_psnd.eat_e.bff != NULL) {
		m_psnd.eat_e.bff->Release();
		m_psnd.eat_e.bff = NULL;
	}
	if(m_psnd.out.bff != NULL) {
		m_psnd.out.bff->Release();
		m_psnd.out.bff = NULL;
	}
	if(m_psnd.btn0.bff != NULL) {
		m_psnd.btn0.bff->Release();
		m_psnd.btn0.bff = NULL;
	}
	if(m_psnd.btn1.bff != NULL) {
		m_psnd.btn1.bff->Release();
		m_psnd.btn1.bff = NULL;
	}
	if(m_psnd.btn2.bff != NULL) {
		m_psnd.btn2.bff->Release();
		m_psnd.btn2.bff = NULL;
	}
	if(m_psnd.no.bff != NULL) {
		m_psnd.no.bff->Release();
		m_psnd.no.bff = NULL;
	}
	if(m_psnd.open.bff != NULL) {
		m_psnd.open.bff->Release();
		m_psnd.open.bff = NULL;
	}
	if(m_psnd.gay.bff != NULL) {
		m_psnd.gay.bff->Release();
		m_psnd.gay.bff = NULL;
	}
	if(m_psnd.pet.bff != NULL) {
		m_psnd.pet.bff->Release();
		m_psnd.pet.bff = NULL;
	}
	m_pDSound.Destroy();

	ShowCursor(TRUE);
	//fclose(fp2);
}

BOOL CGameManager::Init_JunePle_Game(HINSTANCE h_Inst, HWND hWnd)
{
//	hhh=hWnd;
	bool win_mode = false;
	if(IDYES == MessageBox(NULL,"창모드로 할까요?","화면모드선택",MB_YESNO))
		win_mode = true;

	m_pDirect3D      = new CDirect3D();
	if(m_pDirect3D->Init3D(hWnd,800,600,win_mode) == FALSE)return FALSE;

	m_pDSound.Init(hWnd);
	m_pDSound.LoadFile("sound\\jump.wav",    &m_psnd.jump.bff);
	m_pDSound.LoadFile("sound\\holkick.wav", &m_psnd.holkick.bff);
	m_pDSound.LoadFile("sound\\sliding.wav", &m_psnd.sliding.bff);
	m_pDSound.LoadFile("sound\\userhit.wav", &m_psnd.hituser.bff);
	m_pDSound.LoadFile("sound\\userdie.wav", &m_psnd.userdie.bff);
	m_pDSound.LoadFile("sound\\eat_e.wav",   &m_psnd.eat_e.bff);
	m_pDSound.LoadFile("sound\\eat.wav",     &m_psnd.eat.bff);
	m_pDSound.LoadFile("sound\\levelup.wav", &m_psnd.lvup.bff);
	m_pDSound.LoadFile("sound\\btn.wav",     &m_psnd.btn.bff);
	m_pDSound.LoadFile("sound\\btn0.wav",    &m_psnd.btn0.bff);
	m_pDSound.LoadFile("sound\\btn1.wav",    &m_psnd.btn1.bff);
	m_pDSound.LoadFile("sound\\btn2.wav",    &m_psnd.btn2.bff);
	m_pDSound.LoadFile("sound\\out.wav",     &m_psnd.out.bff);
	m_pDSound.LoadFile("sound\\no.wav",      &m_psnd.no.bff);
	m_pDSound.LoadFile("sound\\open.wav",    &m_psnd.open.bff);
	m_pDSound.LoadFile("sound\\gay.wav",     &m_psnd.gay.bff);
	m_pDSound.LoadFile("sound\\wait.wav",    &m_psnd.pet.bff);
		
	m_pDSound.Playing(m_psnd.pet.bff,false);

	m_pFrameCheck    = new CFrameCheck();
	//m_pSorWnd      = new cSprWnd();
	m_pInputManager  = new CInputManager(h_Inst,hWnd,m_pDirect3D->GetDevice());

	m_pTileWnd       = new cTileWnd(); 
	m_pTileWnd->Init();
	
	m_pObjectWnd     = new cObjectWnd();
	m_pObjectWnd->Init();
	
	m_pMapViewWnd    = new cMapViewWnd(m_pDirect3D);
	m_pMapViewWnd->Init();
	m_pMapViewWnd->h_hndW  = hWnd;
	m_pMapViewWnd->h_hinsT = h_Inst;

	m_pMiniMap       = new cMiniMap();
	m_pMiniMap->Init(m_pDirect3D);
	
	m_pUnitManager   = new cUnitManager();
	m_pUnitManager->Init(/*hWnd,*/m_pDirect3D);

	m_pMiniMap->LoadVillige();
	
	m_pScreenManager = new cScreenManager();
	m_pScreenManager->Init(m_pDirect3D);

	m_pInterface     = new CInterface();
	m_pInterface->Init(m_pDirect3D,m_pUnitManager->m_pUserSpr);

	m_p3dFont        = new CD3DFont( _T("Arial"), 10);
	if(FAILED(m_p3dFont->InitDeviceObjects(m_pDirect3D->GetDevice())))
		return false;
	m_p3dFont->RestoreDeviceObjects();

	m_pCursor        = new cMouseCursor();
	m_pCursor->Init(m_pDirect3D);

	m_pHelp = new cHelp(m_pDirect3D);
	m_pHelp->Init();

	/*	POINT ptCursor;
	ptCursor.x = 400;
	ptCursor.y = 300;
	ClientToScreen(hWnd, &ptCursor);
	SetCursorPos(ptCursor.x, ptCursor.y);
	*/
	ShowCursor(FALSE);

	return TRUE;
}

void CGameManager::Render()
{
	char msg[100]={0,};
	
	if( NULL == m_pDirect3D->GetDevice())
        return;

	GameState();
	
	if(gameState == JPS_INIT)
	{
		m_f_frame += (50.0f/m_framerate);
		if(m_f_frame > 3.0f)
		{
			m_pInputManager->FrameMove();
			//	화면을 지운다.
			m_pDirect3D->GetDevice()->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,0,0,255), 1.0f, 0 );
			m_pDirect3D->GetDevice()->SetFVF( FVF_CUSTOMVERTEX );
			m_pDirect3D->GetDevice()->SetStreamSource( 0, m_pMapViewWnd->g_pVB, 0, sizeof(CUSTOM_VERTEX) );
			m_pDirect3D->ColorKeyEnable();
			//	렌더링 시작
			m_pDirect3D->GetDevice()->BeginScene();
			
			if(m_pTitle->Render() == TRUE)
			{
				SAFE_DELETE_(m_pTitle);
				
				char ms[256];
				sprintf(ms,"music\\%d.mp3",rand()%2);
				if(m_pMp3)
					SAFE_DELETE_(m_pMp3);
				m_pMp3 = new CMySound(ms);
				m_pMp3->Play();

				gameState = JPS_LOOP;
			}
		
			//마우스Cursor
			m_pCursor->Render();
			
			//렌더링종료
			m_pDirect3D->GetDevice()->EndScene();
			m_pDirect3D->ColorKeyDisable();
			//	화면으로 출력한다.
			m_pDirect3D->GetDevice()->Present( NULL, NULL, NULL, NULL );
		}
	}
	else if(gameState == JPS_LOOP)
	{
		m_f_frame += (50.0f/m_framerate);
		if(m_f_frame > 3.0f)
		{
			m_pInputManager->FrameMove();
			//	화면을 지운다.
			m_pDirect3D->GetDevice()->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,0,0,255), 1.0f, 0 );
		
			m_pDirect3D->GetDevice()->SetFVF( FVF_CUSTOMVERTEX );
			m_pDirect3D->GetDevice()->SetStreamSource( 0, m_pMapViewWnd->g_pVB, 0, sizeof(CUSTOM_VERTEX) );
		
			m_pDirect3D->ColorKeyEnable();

			//	렌더링 시작
			m_pDirect3D->GetDevice()->BeginScene();
			
			//배경
			m_pScreenManager->Render();

			//게임화면
			m_pUnitManager->FrameCheck(/*60.0f/m_framerate*/);
 		    
			//MiniMap
			m_pMiniMap->Render();
			//npc
			//m_pNpc->Render();
			//인테페이스
			m_pInterface->Render();
			//마우스Cursor
			m_pCursor->Render();
			
			//렌더링종료
			m_pDirect3D->GetDevice()->EndScene();
			m_pDirect3D->ColorKeyDisable();

			//알림출력
			m_pHelp->Help_View();
			//프레임을 나타냅니다......
			sprintf(msg,"%6.1f fps\0",m_framecount);
			m_p3dFont->DrawText(700,0,0xffff0000,msg);
		
			m_f_frame = 0.0f;
			
			//	화면으로 출력한다.
			m_pDirect3D->GetDevice()->Present( NULL, NULL, NULL, NULL );
		}
	}
	
 }

void CGameManager::GameState()
{
	if(gameState == JPS_TITLE)
	{
		InitFrame();
			
		m_pTitle = new cTitle();
		m_pTitle->Init(m_pDirect3D);
		
		gameState = JPS_INIT;
	}
	else if(gameState == JPS_GAME)
	{
		InitFrame();
	
		m_pMp3 = new CMySound("music\\2.mp3");
		m_pMp3->Play();

		m_pDSound.LoadFile("sound\\pet.wav",&m_psnd.pet.bff);
		
		gameState = JPS_TITLE;
	}
	/*else if(gameState == JPS_INIT)
	{
		InitFrame();
		gameState = JPS_LOOP;
	}*/
	else if(gameState == JPS_NEXTLOADING || 
				gameState == JPS_PREVLOADING ||
					gameState == JPS_GOTOVILLIGE)
	{
		char ms[256];
		
		InitFrame();
	
		sprintf(ms,"music\\%d.mp3",rand()%2);
		if(m_pMp3)
			SAFE_DELETE_(m_pMp3);
		m_pMp3 = new CMySound(ms);
		m_pMp3->Play();

		if(gameState == JPS_PREVLOADING)
		{
			m_pMapViewWnd->game_map_prev=1;
			m_pMapViewWnd->game_map_stage--;
		}
		else if(gameState == JPS_NEXTLOADING)
		{
			m_pMapViewWnd->game_map_prev=0;
			if(m_pMapViewWnd->quest == 0)
				m_pMapViewWnd->game_map_stage++;
			else {
				m_pMapViewWnd->quest++;
				if(m_pMapViewWnd->quest==3)
					m_pMapViewWnd->game_map_stage++;
			}

		}
		else if(gameState == JPS_GOTOVILLIGE)
		{
			m_pMapViewWnd->game_map_prev  = 0;
			if(m_pMapViewWnd->quest == 0)
				m_pMapViewWnd->game_map_stage = 0;
			else {
				m_pMapViewWnd->game_map_stage-=(m_pMapViewWnd->quest-1);
				m_pMapViewWnd->quest = 0;
			}
		}
		

		m_pMapViewWnd->InitPos();
		m_pMapViewWnd->MapDataLoad();
		
		m_pMiniMap->LoadVillige();

		SAFE_DELETE_(m_pScreenManager);
		m_pScreenManager = new cScreenManager();
		m_pScreenManager->Init(m_pDirect3D);
		
		m_pUnitManager->KeyState = m_pUnitManager->_COUNTER = 0;
		m_pUnitManager->AllInit();//초기화
		m_pUnitManager->m_pUserSpr->Init();//유닛 제정의
		if(!m_pUnitManager->m_pMonster->InitData())//Monster 재정의
			m_pUnitManager->InitBoss();
		
		m_pUnitManager->m_pNpc->Init();//npc재정의
		//모든Effect와 아이템 제거
		m_pInterface->DelEff();
		m_pMissail->AllMissailDelete();
		//Pet...
		if(m_pInterface->m_pSprWnd->m_pPet)
			m_pInterface->m_pSprWnd->m_pPet->InitPet();

		gameState = JPS_LOOP;
	}
	else if(gameState == JPS_LOOP)
	{	
		m_framerate = m_pFrameCheck->GetFPS();

		++m_totalcount;
		m_totalframe += m_framerate;
		m_framecount += (1000.0f / m_framerate);
		if(m_framecount >= 1000.0f)
		{
			m_currentframe = m_totalframe / (float)m_totalcount;
			m_framecount -= 1000.0f;
			m_totalframe = 0.0f;
			m_totalcount = 0;
		}
		
		if(!m_pMp3->PlayEndTime())
		{
			m_pMp3->Home();
			m_pMp3->Play();
		}
	}
}

void CGameManager::InitFrame()
{	
	POINT ptCursor;
	ptCursor.x = 400;
	ptCursor.y = 300;
	ClientToScreen(m_pMapViewWnd->h_hndW, &ptCursor);
	SetCursorPos(ptCursor.x, ptCursor.y);

	m_framecount = 1000.0f;
	m_totalframe = 0.0f;
	m_f_frame    = 0.0f;
	m_totalcount = 0;

	m_framerate = m_pFrameCheck->GetFPS();
}
