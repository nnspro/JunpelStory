#include "Object.h"
#include "MapOutput.h"
#include "InputManager.h"
#include "MiniMap.h"
#include "MessageBox.h"
#include "Help.h"
#include "Missail.h"
#include "StatusForm.h"
#include "DirectSound.h"
#include "UnitManager.h"

int move_x = 0,move_y = 0,count = 0;
int jump =0,sliding = 0,Kick = 0,KickPlaying = 0;
int pos_cnt = 0,sit = 0,undek = 0;
int hitting = 4,damage = 0,hitmon = -11,hitmon_1 = -11;
int xX,yY;
//FILE *fp11;
//HWND hhh;
//ClassMAPDATA			*pMAP;
//int y_ = 264;

CMessageBox *m_pMss_Box = NULL;

cUnitManager::cUnitManager()
{
	m_pDirect3D     = NULL;
	
	m_pSprDataWnd   = NULL;
	m_pUserSpr      = NULL;
	m_pNpc          = NULL; 
	m_pMonster      = NULL; 
	m_pBoss         = NULL;
	m_pMagic        = NULL;
	//fp11=fopen("a.txt","wt");
}

cUnitManager::~cUnitManager()
{
//	fclose(fp11);
	SAFE_DELETE_(m_pSprDataWnd);
	SAFE_DELETE_(m_pUserSpr);
	SAFE_DELETE_(m_pNpc);
	SAFE_DELETE_(m_pMonster);
	
	if(m_pMagic)
		SAFE_DELETE_(m_pMagic);

	if(m_pBoss)
		SAFE_DELETE_(m_pBoss);
	SAFE_DELETE_(m_pD3dxfont);
}

void cUnitManager::Init(/*HWND h,*/CDirect3D *D3D)
{
	m_pDirect3D = D3D;
	//hhh=h;

	rn = KeyState = 0;

	_COUNTER = 0;

	//frame = 0.0f;

	m_pSprDataWnd    = new cSprDataWnd();
	m_pSprDataWnd->Init(D3D);
	m_pSprDataWnd->SprLoad(0);
	m_pSprDataWnd->LoadSprTexture();

	m_pUserSpr       = new cSprWnd();
	m_pUserSpr->Init();
	
	m_pNpc           = new cNpc(m_pDirect3D);
	m_pNpc->Init();

	/*m_pMiniMap = new cMiniMap();
	m_pMiniMap->Init(m_pDirect3D);*/

	m_pMonster = new cMonsterWnd();

	m_pDSound.LoadFile("sound\\mongun.wav",  &m_psnd.mongun.bff);
	m_pDSound.LoadFile("sound\\monhit.wav",  &m_psnd.hitmon.bff);
	m_pDSound.LoadFile("sound\\mondie.wav",  &m_psnd.mondie.bff);
	
	if(!m_pMonster->Init(m_pDirect3D))
	//	SAFE_DELETE_(m_pMonster);
	{
		m_pBoss = new cBossWnd();
		if(!m_pBoss->Init(m_pDirect3D))
		{
			SAFE_DELETE_(m_pBoss);
		}
		else
		{
			m_pDSound.LoadFile("sound\\bossgun.wav",  &m_psnd.mongun.bff);
			m_pDSound.LoadFile("sound\\bosshit.wav",  &m_psnd.hitmon.bff);
			m_pDSound.LoadFile("sound\\bossdie.wav",  &m_psnd.mondie.bff);
		}
	}

	m_pMissail = new cMissailWnd();
	m_pMissail->Init(m_pDirect3D,m_pMonster,m_pBoss,m_pUserSpr);
	
	m_pD3dxfont = NULL;
	m_pD3dxfont = new CD3dxFont();
	if(!m_pD3dxfont->CreateFONT(m_pDirect3D->GetDevice(), (char*)GetStockObject(SYSTEM_FONT), 70))
		return;
	
	if(!m_pMapViewWnd->MapDataLoad())PostQuitMessage(0);

}
void cUnitManager::InitBoss()
{
	m_pBoss = new cBossWnd();
	if(!m_pBoss->Init(m_pDirect3D))
		SAFE_DELETE_(m_pBoss);
	
	m_pMissail->m_pBss = m_pBoss;
}
void cUnitManager::AllInit()
{
	move_x = move_y = count = 
	jump = sliding = Kick = KickPlaying = 
	pos_cnt = sit = undek = damage = 0;
	
	hitting = 4;
	hitmon = -11;

	if(m_pBoss)
		SAFE_DELETE_(m_pBoss);
}

void cUnitManager::FrameCheck(/*float m_frame*/)
{
	int index;
	tINDEX List[1000];

	//frame+=m_frame;
 
	//뒷타일
	m_pMapViewWnd->BackTile();
	//전경타일
	m_pMapViewWnd->FrontTile();

	//m_pMiniMap->Mini_Map_User_x = m_pUserSpr->spr_x;
	//m_pMiniMap->Mini_Map_User_y = m_pUserSpr->spr_y;
	//y_ = 264;

	//Animation();
	
	if(KeyState < 112)
	{	
		m_pUserSpr->Hptimes++;
		m_pUserSpr->Mptimes++;

		if(m_pUserSpr->Hptimes >= 500)
		{
			m_pUserSpr->Hptimes = 0;
			m_pUserSpr->Hp+=(8+m_pUserSpr->Hana/3);
			if(m_pUserSpr->Hp > m_pUserSpr->totalHp)
				m_pUserSpr->Hp = m_pUserSpr->totalHp;
		}
		if(m_pUserSpr->Mptimes >= 500)
		{
			m_pUserSpr->Mptimes = 0;
			m_pUserSpr->Mp+=(8+m_pUserSpr->Mana/3);
			if(m_pUserSpr->Mp > m_pUserSpr->totalMp)
				m_pUserSpr->Mp = m_pUserSpr->totalMp;
		}
	}

	if(move_x == 0)
		xX = m_pUserSpr->spr_x + m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].XPos
		- m_pMapViewWnd->map_x;
	else
		xX = 408 + m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].XPos;
	
	if(move_y == 0)
		yY = m_pUserSpr->spr_y + m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].YPos
		- m_pMapViewWnd->map_y;
	else
		yY =  m_pUserSpr->y_ + m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].YPos;
	// 건물이나 캐릭터 모두를 같이 소트한다..
	int all;

	if(m_pMonster->mondatATotalnum > 0)
	{
		all = m_pObjectWnd->ObjCount+m_pMonster->mondatATotalnum/*m_pNpc->npcnum+*/+1;
	
		for (int i=0;i<all;i++)
		{
			List[i].Index=i;
			if (i<m_pObjectWnd->ObjCount)		//  i가 Object 갯수를 넘으면.. Npc의 값을..
				List[i].Value=m_pObjectWnd->aOBJ[i].y -m_pMapViewWnd->map_y ; 
			else if (i>=m_pObjectWnd->ObjCount && i<(m_pMonster->mondatATotalnum+m_pObjectWnd->ObjCount))	        //  i가 Npc갯수를 넘으면.. 캐릭터의 값을..
				List[i].Value=m_pMonster->Mon[i-(m_pObjectWnd->ObjCount)].mon_y-m_pMapViewWnd->map_y;
			else 
				List[i].Value=m_pUserSpr->spr_y;
		}
	}
	else if(m_pBoss)
	{
		all = m_pObjectWnd->ObjCount+2;
	
		for (int i=0;i<all;i++)
		{
			List[i].Index=i;
			if (i<m_pObjectWnd->ObjCount)		//  i가 Object 갯수를 넘으면.. Npc의 값을..
				List[i].Value=m_pObjectWnd->aOBJ[i].y -m_pMapViewWnd->map_y ; 
			else if (i>=m_pObjectWnd->ObjCount && i<(1+m_pObjectWnd->ObjCount))	        //  i가 Npc갯수를 넘으면.. 캐릭터의 값을..
				List[i].Value=m_pBoss->boss_y-m_pMapViewWnd->map_y;
			else 
				List[i].Value=m_pUserSpr->spr_y;
		}
	}else
	{
		all = m_pObjectWnd->ObjCount+1;
		for (int i=0;i<all;i++)
		{
			List[i].Index=i;
			if (i<m_pObjectWnd->ObjCount)		//  i가 Object 갯수를 넘으면.. Npc의 값을..
				List[i].Value=m_pObjectWnd->aOBJ[i].y -m_pMapViewWnd->map_y ; 
			else 
				List[i].Value=m_pUserSpr->spr_y;
		}
	}
	// 퀵소트..
	qsort((void *)List,all,sizeof(tINDEX),(int(*)(const void *,const void *))Compare);

	if(m_pMonster->mondatATotalnum > 0)
	{
		for (int i=0;i<all;i++)
		{
			index=List[i].Index;
			if (index < m_pObjectWnd->ObjCount) // Object라면..
			{
				int xx  =m_pObjectWnd->aOBJ[index].x-m_pMapViewWnd->map_x;
				int yy  =m_pObjectWnd->aOBJ[index].y-m_pMapViewWnd->map_y;
				int num =m_pObjectWnd->aOBJ[index].num;
				if (yy > -150/*m_pObjectWnd->h_Height[m_pObjectWnd->pAni[num].pFrame[m_pObjectWnd->pAni[num].CurFrame].Sprite]*/ && yy < 600 &&
					xx > -100/*m_pObjectWnd->w_Width[m_pObjectWnd->pAni[num].pFrame[m_pObjectWnd->pAni[num].CurFrame].Sprite]*/ && xx < 800  )
					m_pMapViewWnd->DrawObject(xx,yy,num,index);
			}
			else if(index>=m_pObjectWnd->ObjCount && index < (m_pMonster->mondatATotalnum+m_pObjectWnd->ObjCount))//monster라면..
			{
				//int monnum     = m_pMonster->Mon[index-(m_pObjectWnd->ObjCount)].number;	
				int animations = m_pMonster->Mon[index-(m_pObjectWnd->ObjCount)].AniNumber;
				int n_x        = m_pMonster->Mon[index-(m_pObjectWnd->ObjCount)].mon_x +
									m_pMonster->Mon[index-(m_pObjectWnd->ObjCount)].m_pMonSprWnd->pAni[animations].pFrame[m_pMonster->Mon[index-(m_pObjectWnd->ObjCount)].m_pMonSprWnd->pAni[animations].CurFrame].XPos
										- m_pMapViewWnd->map_x;
				int	n_y        = m_pMonster->Mon[index-(m_pObjectWnd->ObjCount)].mon_y + 
									m_pMonster->Mon[index-(m_pObjectWnd->ObjCount)].m_pMonSprWnd->pAni[animations].pFrame[m_pMonster->Mon[index-(m_pObjectWnd->ObjCount)].m_pMonSprWnd->pAni[animations].CurFrame].YPos
										- m_pMapViewWnd->map_y;

				if(n_x >= -50 && n_x <= 850 &&
					n_y >= -50 && n_y <= 650)
						m_pMonster->PutImage(n_x,n_y,/*monnum,*/animations,index-(m_pObjectWnd->ObjCount));
			
				m_pMonster->EmyMove(m_pUserSpr->spr_x,m_pUserSpr->spr_y,index-(m_pObjectWnd->ObjCount));//위에것과 바뀌면 이동처리가 이상해진다.
			}
			else 
			{// 캐릭터라면..
				//index=index-pOBJECT->ObjCount+1;	//	액터의 번호가 1번부터 시작이다..
				//m_pNpc->Render();
				if(m_pUserSpr->Turbo == 0 && KeyState != 113)
					DrawSpr(xX,yY,m_pUserSpr->AniCount);

				if(KeyState != 112 && KeyState != 113)
				{
					if(m_pUserSpr->TurboDelayTime > 29)
					{
						RECT rc;
						rc.left   = xX + m_pSprDataWnd->CrashBox[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].Sprite][0].left;
						rc.right  = xX + m_pSprDataWnd->CrashBox[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].Sprite][0].right;
						rc.top    = yY + m_pSprDataWnd->CrashBox[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].Sprite][0].top;
						rc.bottom = yY + m_pSprDataWnd->CrashBox[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].Sprite][0].bottom;
				
						hitmon = m_pMonster->HitMonster(rc);
						if(hitmon != -11)
							KeyState = 111;
					}else
					{
						m_pUserSpr->TurboDelayTime--;
						m_pUserSpr->Turbo=1-m_pUserSpr->Turbo;
						if(m_pUserSpr->TurboDelayTime < 0)
						{
							m_pUserSpr->TurboDelayTime = 30;
							m_pUserSpr->Turbo = 0;
						}
					}
				}
			}
		}
	}else if(m_pBoss)
	{
		for (int i=0;i<all;i++)
		{
			index=List[i].Index;
			if (index < m_pObjectWnd->ObjCount) // Object라면..
			{
				int xx  =m_pObjectWnd->aOBJ[index].x-m_pMapViewWnd->map_x;
				int yy  =m_pObjectWnd->aOBJ[index].y-m_pMapViewWnd->map_y;
				int num =m_pObjectWnd->aOBJ[index].num;
				if (yy > -150/*m_pObjectWnd->h_Height[m_pObjectWnd->pAni[num].pFrame[m_pObjectWnd->pAni[num].CurFrame].Sprite]*/ && yy < 600 &&
					xx > -100/*m_pObjectWnd->w_Width[m_pObjectWnd->pAni[num].pFrame[m_pObjectWnd->pAni[num].CurFrame].Sprite]*/ && xx < 800  )
						m_pMapViewWnd->DrawObject(xx,yy,num,index);
			}
			else if(index>=m_pObjectWnd->ObjCount && index < (1+m_pObjectWnd->ObjCount))//monster라면..
			{
				//int monnum     = m_pMonster->Mon[index-(m_pObjectWnd->ObjCount)].number;	
				int animations = m_pBoss->aninum;
				int n_x        = m_pBoss->boss_x + m_pBoss->m_pBossSprWnd->pAni[animations].pFrame[m_pBoss->m_pBossSprWnd->pAni[animations].CurFrame].XPos
										- m_pMapViewWnd->map_x;
				int	n_y        = m_pBoss->boss_y + m_pBoss->m_pBossSprWnd->pAni[animations].pFrame[m_pBoss->m_pBossSprWnd->pAni[animations].CurFrame].YPos
										- m_pMapViewWnd->map_y;

				if(n_x >= -50 && n_x <= 850 &&
					n_y >= -50 && n_y <= 650)
				{
					if(m_pBoss->turbo == 0)
						m_pBoss->PutImage(n_x,n_y,/*monnum,*/animations);
				}
			
				m_pBoss->BossMove(m_pUserSpr->spr_x,m_pUserSpr->spr_y);//위에것과 바뀌면 이동처리가 이상해진다.
			}
			else 
			{// 캐릭터라면..
				//index=index-pOBJECT->ObjCount+1;	//	액터의 번호가 1번부터 시작이다..
				//m_pNpc->Render();
				if(m_pUserSpr->Turbo == 0 && KeyState != 113)
					DrawSpr(xX,yY,m_pUserSpr->AniCount);

				if(KeyState != 112 && KeyState != 113)
				{
					if(m_pUserSpr->TurboDelayTime > 29)
					{
						RECT rc;
						rc.left   = xX + m_pSprDataWnd->CrashBox[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].Sprite][0].left;
						rc.right  = xX + m_pSprDataWnd->CrashBox[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].Sprite][0].right;
						rc.top    = yY + m_pSprDataWnd->CrashBox[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].Sprite][0].top;
						rc.bottom = yY + m_pSprDataWnd->CrashBox[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].Sprite][0].bottom;
				
						hitmon = m_pBoss->HitBoss(rc);
						if(hitmon != -11)
							KeyState = 111;
					}else
					{
						m_pUserSpr->TurboDelayTime--;
						m_pUserSpr->Turbo=1-m_pUserSpr->Turbo;
						if(m_pUserSpr->TurboDelayTime < 0)
						{
							m_pUserSpr->TurboDelayTime = 30;
							m_pUserSpr->Turbo = 0;
						}
					}
				}
			}
		}
	}else
	{
		for (int i=0;i<all;i++)
		{
			index=List[i].Index;
			if (index < m_pObjectWnd->ObjCount) // Object라면..
			{
				int xx  =m_pObjectWnd->aOBJ[index].x-m_pMapViewWnd->map_x;
				int yy  =m_pObjectWnd->aOBJ[index].y-m_pMapViewWnd->map_y;
				int num =m_pObjectWnd->aOBJ[index].num;
				if (yy > -150/*m_pObjectWnd->h_Height[m_pObjectWnd->pAni[num].pFrame[m_pObjectWnd->pAni[num].CurFrame].Sprite]*/ && yy < 600 &&
					xx > -100/*m_pObjectWnd->w_Width[m_pObjectWnd->pAni[num].pFrame[m_pObjectWnd->pAni[num].CurFrame].Sprite]*/ && xx < 800  )
						m_pMapViewWnd->DrawObject(xx,yy,num,index);
			}
			else 
			{// 캐릭터라면..
				//index=index-pOBJECT->ObjCount+1;	//	액터의 번호가 1번부터 시작이다..
				m_pNpc->Render();
				if(m_pUserSpr->Turbo == 0 && KeyState != 113)
					DrawSpr(xX,yY,m_pUserSpr->AniCount);
				
				//if(KeyState == 112 && KeyState == 113)
				//{
					if(m_pUserSpr->TurboDelayTime < 29)
					{
						m_pUserSpr->TurboDelayTime--;
						m_pUserSpr->Turbo=1-m_pUserSpr->Turbo;
						if(m_pUserSpr->TurboDelayTime < 0)
						{
							m_pUserSpr->TurboDelayTime = 30;
							m_pUserSpr->Turbo = 0;
						}
					}
				//}
			}
		}
	}
   
    if(m_pMonster)//히트Effect
		m_pMonster->Render();
	else if(m_pBoss)
		m_pBoss->Render();

	if(m_pMissail)//미사일출력 루틴
	{
	
		RECT rc;

		rc.left   = xX + m_pSprDataWnd->CrashBox[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].Sprite][0].left;
		rc.right  = xX + m_pSprDataWnd->CrashBox[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].Sprite][0].right;
		rc.top    = yY + m_pSprDataWnd->CrashBox[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].Sprite][0].top;
		rc.bottom = yY + m_pSprDataWnd->CrashBox[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].Sprite][0].bottom;
			
		m_pMissail->Render();
		hitmon_1 = m_pMissail->HitChk(rc);

		if(KeyState != 112 && KeyState != 113)
		{
			if(m_pUserSpr->TurboDelayTime > 29)
			{
				if(hitmon_1 != -11)
				{
					KeyState = 111;
				}
			}
		}
	}
	//가시에 부딪히면 에너지를 깍는다ㅣ.
	if(KeyState != 112 && KeyState != 113)
	{
		if(m_pUserSpr->TurboDelayTime > 29)
		{
			if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x,m_pMapViewWnd->MAP_y+1) == 5)
					KeyState = 114;
		}
	}
			
	Animation();//좌표보정위로 올라가면 유닛이동처리가 이상해진다.

	//부메랑과Missail처리루틴.
	//부메랑은 한번 Missail은 두번 나가단.
	/*if(m_pUserSpr->mis > 0)
	{
		m_pUserSpr->mis++;
		if(m_pUserSpr->mis >= 20)m_pUserSpr->mis=0;
	}*/
	
	if(KeyState != 110 && KeyState != 113) 
		_COUNTER++;

	if(KeyState != 113)
	m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame=(_COUNTER/m_pSprDataWnd->pAni[m_pUserSpr->AniCount].Delay)%m_pSprDataWnd->pAni[m_pUserSpr->AniCount].FrameNum;
	
	if(KeyState == 112)
	{
		if(m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame >= m_pSprDataWnd->pAni[m_pUserSpr->AniCount].FrameNum-1)
            KeyState = 113;
	}else if(KeyState == 113)
	{
		if(m_pMss_Box->Render() == -20)
		{
			SAFE_DELETE_(m_pMss_Box);
			m_pMapViewWnd->user_life = FALSE;
			m_pUserSpr->Hp = m_pUserSpr->totalHp/2;
			if(m_pUserSpr->Level > 10)
			{
				m_pUserSpr->Kyunghum =m_pUserSpr->Kyunghum*30/100;
			}
			gameState = 800;
		}
	}
	
	//Pet 구현....
	if(m_pUserSpr->m_pPet)
		m_pUserSpr->m_pPet->Render();

	if(m_pNpc->m_pTalkBox->btn_ck == 0 && KeyState < 111)KeyFrame();
	else if(m_pNpc->m_pTalkBox->btn_ck == 1)m_pNpc->m_pTalkBox->Render();
			
	//속도 마법 표현
	if(m_pMagic)
	{
		BOOL j = m_pMagic->Render();
		if(j == FALSE)
			SAFE_DELETE_(m_pMagic);
	}
	//전경타일
	//m_pMapViewWnd->FrontTile();
	//m_pMiniMap->Render();
}
void
cUnitManager::DrawSpr(int x,int y,int i)
{
    //	Sprite에 사용될 쉐이더 설정
m_pDirect3D->GetDevice()->SetTexture( 0, 
									 m_pSprDataWnd->g_pSprTexture[m_pSprDataWnd->pAni[i].pFrame[m_pSprDataWnd->pAni[i].CurFrame].Sprite] );
			
	CUSTOM_VERTEX*	pV = NULL;

	m_pMapViewWnd->g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

	for( int m=0; m < 4; m++ )
	{
      pV[m].vPos.z = 1.0f;
	  pV[m].rhw = 1.0f;
	  pV[m].dwDiffuse = 0xffffffff;
	}
		

	float	fLeft   = (float)x;
	float	fWidth  = (float)m_pSprDataWnd->w_Width[m_pSprDataWnd->pAni[i].pFrame[m_pSprDataWnd->pAni[i].CurFrame].Sprite];
	float	fTop    = (float)y;
	float	fHeight = (float)m_pSprDataWnd->h_Height[m_pSprDataWnd->pAni[i].pFrame[m_pSprDataWnd->pAni[i].CurFrame].Sprite];

	pV[0].vPos.x = (int)(fLeft) - 0.5f;
	pV[0].vPos.y = (int)(fTop + fHeight) - 0.5f;
	pV[1].vPos.x = (int)(fLeft) - 0.5f;
	pV[1].vPos.y = (int)(fTop) - 0.5f;
	pV[2].vPos.x = (int)(fLeft + fWidth) - 0.5f;
	pV[2].vPos.y = (int)(fTop + fHeight) - 0.5f;
	pV[3].vPos.x = (int)(fLeft + fWidth) - 0.5f;
	pV[3].vPos.y = (int)(fTop) - 0.5f;

	pV[0].tu = 0.0f;	pV[0].tv = 1.0f;
	pV[1].tu = 0.0f;	pV[1].tv = 0.0f;
	pV[2].tu = 1.0f;	pV[2].tv = 1.0f;
	pV[3].tu = 1.0f;	pV[3].tv = 0.0f;
		
	m_pMapViewWnd->g_pVB->Unlock();

	m_pDirect3D->GetDevice()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
}
void
cUnitManager::KeyFrame()
{
	if(KeyState == -10)return;

	//앉기
	if(m_pInputManager->m_isDownKey == true)
	{
		if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x,m_pMapViewWnd->MAP_y) == 3)
		{
			if(jump > 0)jump = 0;
			KeyState = 14;
		}
		else if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x,m_pMapViewWnd->MAP_y) == 4)
			KeyState = 13;
		else if(KeyState < 4 || KeyState > 9)//앉아서공격중이아닐때....
			KeyState = 3;
	}
		//점프
	if(m_pInputManager->m_isDAltKey == true)
	{
		if(m_pInputManager->m_isUAltKey == false)
		{
			m_pInputManager->m_isUAltKey = true;
			if(undek <= 1 && KeyState == 3)
				KeyState = 7;//슬라이딩
			else if(KeyState == 110 || KeyState == 11)//사다리 타는중
			{
				if(m_pUserSpr->DropUnit != 0)m_pUserSpr->DropUnit = 0;
				jump = 15;
				KeyState = 4;
			}
			else if(KeyState != 7)
				KeyState = 4;//일반점프
		}
	}else m_pInputManager->m_isUAltKey = false;

	if(KeyState <= 4 || KeyState == 6 || KeyState == 8)
	{//이동
		if(m_pInputManager->m_isLeftKey == true)
		{//왼쪽
			//앉아있을경우
			if(KeyState == 3)
			{
				m_pUserSpr->view = UNIT_STAND + 1;
			}
			else//점프중
				if(KeyState == 4 || KeyState == 6)
					KeyState = 8;
				else if(KeyState == 8)//반대로보기
					m_pUserSpr->view = UNIT_STAND + 1;
				else//그냥이동
					KeyState = 1;
		}
		if(m_pInputManager->m_isRightKey == true)
		{//오른쪽
			if(m_pInputManager->m_isLeftKey == true && KeyState < 2)
			{//키를 동시에 누를경우 멈춘다.
				KeyState = 0;
				goto STOP;
			}
			//앉아있을경우
			if(KeyState == 3)
			{
				m_pUserSpr->view = UNIT_STAND;
			}
			else
			{//점프중
				if(KeyState == 4 || KeyState == 6)
					KeyState = 8;
				else if(KeyState == 8)//점프중 방향바꾸기
					m_pUserSpr->view = UNIT_STAND;
				else//그냥이동
                    KeyState = 2;
			}
		}
		//점프중 키를 놓을면 그냥 떨어진다.
		if(KeyState == 8)
		{
			if(m_pInputManager->m_isLeftKey == false
				&& m_pInputManager->m_isRightKey == false)
			{
				KeyState = 4;
			}
		}
	}
STOP:
	//공격
	if(m_pInputManager->m_isDCtrlKey == true)
	{
		if(m_pInputManager->m_isUCtrlKey == false && KeyState != 110)
		{
			m_pInputManager->m_isUCtrlKey = true;
			//공격중이아닐때....
			if(KeyState != 5 && KeyState != 6)
			{//앉아서공격
				if(KeyState == 3)
					KeyState = 5;
				//점프공격
				if(KeyState == 4 && KickPlaying == 0)
				{
					KeyState = 6;
					Kick = 0;
					KickPlaying = 1;
				}
				else if(KeyState == 8 && KickPlaying == 0)
				{//점프중이동하면서공격
					KeyState = 9;
					Kick = 0;
					KickPlaying = 1;
				}
				else if(KeyState < 3 || KeyState > 9)//서서공격
					KeyState = 10;
			}
		}
	}else m_pInputManager->m_isUCtrlKey = false;
	//위로
	if(m_pInputManager->m_isUpKey == true)
	{
		if(m_pInputManager->m_isDownKey == true)
		{//키를 동시에 누를경우 멈춘다.
			KeyState = 0;
			return;
		}

		if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x,m_pMapViewWnd->MAP_y-1) == 3)
			KeyState = 11;
		else if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x,m_pMapViewWnd->MAP_y-1) == 4)
		{
			if(jump > 0)jump = 0;
			KeyState = 12;
		}
		//else KeyState = 0;
	}

	//아템먹기
	if(m_pInputManager->m_isDShiftKey == true)
	{
		if(m_pInputManager->m_isUShiftKey == false)
		{
			RECT rc;
			
			m_pInputManager->m_isUShiftKey = true;
			
			rc.left   = xX + m_pSprDataWnd->CrashBox[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].Sprite][0].left;
			rc.right  = xX + m_pSprDataWnd->CrashBox[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].Sprite][0].right;
			rc.top    = yY + m_pSprDataWnd->CrashBox[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].Sprite][0].top;
			rc.bottom = yY + m_pSprDataWnd->CrashBox[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].Sprite][0].bottom;
			
			int itm = m_pMonster->HitItem(rc);
			if(itm != -1)
			{
				m_pDSound.Playing(m_psnd.eat.bff, false);
				m_pUserSpr->Money+=itm;
			}

		}
	}else m_pInputManager->m_isUShiftKey = false;
	//스킬사용
	if(m_pInputManager->m_isDSpaceKey == true)
	{
		if(m_pInputManager->m_isUSpaceKey == false)
		{
			m_pInputManager->m_isUSpaceKey = true;

			if(m_pUserSpr->Interface_[1].itemnum > -1)
			{
				if(S_Item[m_pUserSpr->Interface_[1].checked].att == 1)
				{
					if(KeyState < 3 || KeyState > 9)
					{
						if((m_pUserSpr->Mp-S_Item[m_pUserSpr->Interface_[1].checked].usemp) >= 0)
						{
							KeyState = -10;
							m_pUserSpr->Mp-=S_Item[m_pUserSpr->Interface_[1].checked].usemp;
						}else
						{
							m_pDSound.Playing(m_psnd.no.bff, false);
							m_pHelp->CreateHelp("mp가 모자랍니다.");
						}
					}
				}else if(S_Item[m_pUserSpr->Interface_[1].checked].att == 0)
				{
					if(!m_pMagic)
					{
						if((m_pUserSpr->Mp-S_Item[m_pUserSpr->Interface_[1].checked].usemp) >= 0)
						{
							m_pDSound.Playing(m_psnd.gay.bff,false);
							m_pUserSpr->Mp-=S_Item[m_pUserSpr->Interface_[1].checked].usemp;
							
							m_pMagic = new cMoveMagic();
							m_pMagic->Init(m_pDirect3D);
						}else
						{
							m_pDSound.Playing(m_psnd.no.bff, false);
							m_pHelp->CreateHelp("mp가 모자랍니다.");
						}
					}
				}
			}
		}
	}else m_pInputManager->m_isUSpaceKey = false;
}

void
cUnitManager::Animation()
{
	//char msg[100];
	int size_x = 8,size_y = 8;
	//적과 닿았을때
	if(KeyState == 111 || KeyState == 114)
	{
		if(hitting == 4)
		{
			m_pDSound.Playing(m_psnd.hituser.bff, false);
			
			m_pUserSpr->Hptimes = 0;
			m_pUserSpr->Mptimes = 0;
			
			if(KeyState == 111)
			{
				if(m_pUserSpr->DropUnit < 60)
				{
					if(m_pMonster->mondatATotalnum > 0)
					{
						if(hitmon !=-11)
							damage = (m_pMonster->Mon[hitmon].str-(m_pUserSpr->Dex/2))+rand()%3;
						if(hitmon_1 !=-11)
							damage = (m_pMonster->Mon[hitmon_1].str-(m_pUserSpr->Dex/2))+rand()%3;
					}else if(m_pBoss)
					{
						if(hitmon !=-11)
							damage = (m_pBoss->str-(m_pUserSpr->Dex/2))+rand()%3;
						if(hitmon_1 !=-11)
							damage = (m_pBoss->str-(m_pUserSpr->Dex/2))+rand()%3;
					}
				}else
					damage = m_pUserSpr->DropUnit/3;
			}else if(KeyState == 114)
				damage = 15;
			
			m_pUserSpr->Hp-=damage;
			m_pUserSpr->DropUnit = 0;
			
			sprintf(m_pUserSpr->damagetxt,"%d",damage);
			
			m_pUserSpr->Hit_Spr_x = m_pUserSpr->spr_x;
			m_pUserSpr->Hit_Spr_y = m_pUserSpr->spr_y;
		}
        if(jump > 0)jump = 0;
		if(KickPlaying ==1)KickPlaying = 0;
		
		if(m_pUserSpr->AniCount == UNIT_UP_WALK)
			hitting                    =  0;
		

		if(m_pUserSpr->Hp <= 0)
		{
			m_pDSound.Playing(m_psnd.userdie.bff, false);
			
			m_pUserSpr->Hp             =    0;
			
			m_pMapViewWnd->user_life   = TRUE;  
			
			hitting                    =    4;
			KeyState                   =  112;
			m_pNpc->m_pTalkBox->btn_ck =    0;

			m_pMss_Box = new CMessageBox();
			m_pMss_Box->Init(m_pDirect3D,300,100,0,0);
			if(m_pMapViewWnd->quest == 0)
			{
				CopyMemory(m_pMss_Box->M_Box.Box_Text[0],"모험에 실패했습니다.",20);
				CopyMemory(m_pMss_Box->M_Box.Box_Text[1],"확인을 눌러 마을로 가세요.",26);
			}else
			{
				//m_pMapViewWnd->quest = 0;
				CopyMemory(m_pMss_Box->M_Box.Box_Text[0],"퀘스트를 실패했습니다.",22);
				CopyMemory(m_pMss_Box->M_Box.Box_Text[1],"퀘스트 시작점으로 이동합니다.",29);
			}
						
			goto DIE;
		}

		if(damage > 0 && hitting > -5)
		{
			m_pUserSpr->Hit_Spr_y-=10;
			int Spr_X = m_pUserSpr->Hit_Spr_x - m_pMapViewWnd->map_x;
			int Spr_Y = m_pUserSpr->Hit_Spr_y - m_pMapViewWnd->map_y;

			m_pD3dxfont->DrawText(Spr_X,Spr_Y,0,0,
				D3DCOLOR_XRGB(0, 0, 255),m_pUserSpr->damagetxt,strlen(m_pUserSpr->damagetxt));
		}

		if(hitting >= 0)
		{
			//UserUp(8);
			
			hitting--;
			if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x,m_pMapViewWnd->MAP_y-10) == 2 || hitting == 0)
					hitting = -1;
	
			if(m_pUserSpr->view == UNIT_STAND)
			{
				if(m_pUserSpr->AniCount != UNIT_HIT)
				{
					m_pSprDataWnd->pAni[UNIT_HIT].CurFrame = 0;
					_COUNTER = UNIT_STAND;
				}
				m_pUserSpr->AniCount = UNIT_HIT;
		
				if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x-4,m_pMapViewWnd->MAP_y) != 8 &&
					 m_pMapViewWnd->MAP_x >= 4)
						UserLeft(8);
			}
			else if(m_pUserSpr->view == (UNIT_STAND + 1))
			{
				if(m_pUserSpr->AniCount != (UNIT_HIT + 1))
				{
					m_pSprDataWnd->pAni[UNIT_HIT + 1].CurFrame = 0;
					_COUNTER = UNIT_STAND;
				}
				m_pUserSpr->AniCount = UNIT_HIT + 1;
				if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x+4,m_pMapViewWnd->MAP_y) != 8 &&
					m_pMapViewWnd->MAP_x <= (m_pMapViewWnd->pMAP->WIDTH*4)-4)
						UserRight(8);
			}
			UserUp(8);
		}else if(hitting < 0)
		{
			//UserDown(8);
				
			m_pUserSpr->DropUnit++;
			hitting--;
			if(hitting <= -15)
			{
				if(m_pUserSpr->view == UNIT_STAND)
				{
					if(m_pUserSpr->AniCount != UNIT_DOWN)
					{
						m_pSprDataWnd->pAni[UNIT_DOWN].CurFrame = 0;
						_COUNTER = UNIT_STAND;
					}
					m_pUserSpr->AniCount = UNIT_DOWN;
				}
				else if(m_pUserSpr->view == (UNIT_STAND + 1))
				{
					if(m_pUserSpr->AniCount != (UNIT_DOWN + 2))
					{
						m_pSprDataWnd->pAni[UNIT_DOWN + 2].CurFrame = 0;
						_COUNTER = UNIT_STAND;
					}
					m_pUserSpr->AniCount = UNIT_DOWN + 2;
				}
			}	
			if(m_pUserSpr->view == UNIT_STAND)
			{
				if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x-4,m_pMapViewWnd->MAP_y) != 8 &&
					 m_pMapViewWnd->MAP_x >= 4)
						UserLeft(8);
			}
			else if(m_pUserSpr->view == (UNIT_STAND + 1))
			{
				if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x+4,m_pMapViewWnd->MAP_y) != 8 &&
					 m_pMapViewWnd->MAP_x <= (m_pMapViewWnd->pMAP->WIDTH*4)-4)
						UserRight(8);
			}
			UserDown(8);
		}
	
		if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x,m_pMapViewWnd->MAP_y) != 0
			&& m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x,m_pMapViewWnd->MAP_y) != 3
			&& m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x,m_pMapViewWnd->MAP_y) < 9)
			{
				if(damage > 0)
					m_pUserSpr->TurboDelayTime = 25;
				hitting                    =  4;
				KeyState                   =  0;

				if(m_pUserSpr->DropUnit >= 60)KeyState = 111;
				else m_pUserSpr->DropUnit = 0;
			}
	}else if(KeyState == 112)
	{
DIE:
		if(m_pUserSpr->AniCount != (UNIT_DIE))
		{
			m_pSprDataWnd->pAni[UNIT_DIE].CurFrame = 0;
			_COUNTER = UNIT_STAND;
		}
		m_pUserSpr->AniCount = UNIT_DIE;
	}
	
	//점프
	if(KeyState == 4 || KeyState == 6 || 
		KeyState == 8 || KeyState == 9)
	{
		//if(KeyState == 4)
		//{
			if(jump < 14)
			{	
				jump++;
				/*m_pUserSpr->spr_y-=8;
				m_pMapViewWnd->MAP_y = m_pUserSpr->spr_y/32;*/
				
			//	y_ = 256;
				UserUp(8);
	//			Beep(500,1000);
//Sleep(200);
				if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x,m_pMapViewWnd->MAP_y-10) == 2 || jump == 13)
					jump = 16;
				/*if(m_pMapViewWnd->MAP_x <= 0 || m_pMapViewWnd->MAP_x >= (m_pMapViewWnd->pMAP->WIDTH*4)-4)
				{
					if(KeyState == 8)KeyState = 4;
					else if(KeyState == 9)KeyState = 6;
				}*/
			/*	if(KeyState == 8)
				{
					if(m_pUserSpr->view == UNIT_STAND)
					{
						if(m_pMapViewWnd->MAP_x >= (m_pMapViewWnd->pMAP->WIDTH*4)-4)
							KeyState = 4;
					}else if(m_pUserSpr->view == (UNIT_STAND+1))
					{
						if(m_pMapViewWnd->MAP_x <= 0)
							KeyState = 4;
					}
				}else
				if(KeyState == 9)
				{
					if(m_pUserSpr->view == UNIT_STAND)
					{
						if(m_pMapViewWnd->MAP_x >= (m_pMapViewWnd->pMAP->WIDTH*4)-4)
							KeyState = 6;
					}else if(m_pUserSpr->view == (UNIT_STAND+1))
					{
						if(m_pMapViewWnd->MAP_x <= 0)
							KeyState = 6;
					}
				}*/

				if(KeyState == 4 || KeyState == 8)
				{
					if(m_pUserSpr->view == UNIT_STAND)
					{
						if(m_pUserSpr->AniCount != UNIT_UP)
						{
							m_pDSound.Playing(m_psnd.jump.bff, false);
							
							m_pSprDataWnd->pAni[UNIT_UP].CurFrame = 0;
							_COUNTER = UNIT_STAND;
						}
						m_pUserSpr->AniCount = UNIT_UP;
						if(KeyState == 8)
						{
							if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x+4,m_pMapViewWnd->MAP_y) != 8
								&& m_pMapViewWnd->MAP_x <= (m_pMapViewWnd->pMAP->WIDTH*4)-4)/* ||
								m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x+4,m_pMapViewWnd->MAP_y) != 1)*/
								UserRight(8);
						}
					}
					else if(m_pUserSpr->view == (UNIT_STAND + 1))
					{
						if(m_pUserSpr->AniCount != (UNIT_UP + 2))
						{
							m_pDSound.Playing(m_psnd.jump.bff, false);
							
							m_pSprDataWnd->pAni[UNIT_UP + 2].CurFrame = 0;
							_COUNTER = UNIT_STAND;
						}
						m_pUserSpr->AniCount = UNIT_UP + 2;
						if(KeyState == 8)
						{
							if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x-4,m_pMapViewWnd->MAP_y) != 8
								&& m_pMapViewWnd->MAP_x >= 4)/* ||
								m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x-4,m_pMapViewWnd->MAP_y) != 1)*/
							UserLeft(8);
						}
					}
				}
				else if(KeyState == 6 || KeyState == 9)
				{
					if(m_pUserSpr->Interface_[0].itemnum != 6 && m_pUserSpr->Interface_[0].itemnum != 2)
					{
						Kick = 0;
						KickPlaying = 0;

                        goto BUMERANG;
					}
					else
					{
						Kick++;
						if(Kick >= 7)
						{
							Kick = 0;
							KeyState = 4;
							goto JUMPOUT;
						}
						if(m_pUserSpr->view == UNIT_STAND)
						{
							if(m_pUserSpr->AniCount != UNIT_JUMP_ATTACK)
							{
								m_pSprDataWnd->pAni[UNIT_JUMP_ATTACK].CurFrame = 0;
								_COUNTER = UNIT_STAND;
							}
							m_pUserSpr->AniCount = UNIT_JUMP_ATTACK;
							if(KeyState == 9){
								if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x+4,m_pMapViewWnd->MAP_y) != 8
									&& m_pMapViewWnd->MAP_x <= (m_pMapViewWnd->pMAP->WIDTH*4)-4)/* ||
									m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x+4,m_pMapViewWnd->MAP_y) != 1)*/
									UserRight(8);
							}
						}
						else if(m_pUserSpr->view == (UNIT_STAND + 1))
						{
							if(m_pUserSpr->AniCount != (UNIT_JUMP_ATTACK + 1))
							{
								m_pSprDataWnd->pAni[UNIT_JUMP_ATTACK + 1].CurFrame = 0;
								_COUNTER = UNIT_STAND;
							}
							m_pUserSpr->AniCount = UNIT_JUMP_ATTACK + 1;
							if(KeyState == 9){
								if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x-4,m_pMapViewWnd->MAP_y) != 8
									&& m_pMapViewWnd->MAP_x >= 4)/* ||
									m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x-4,m_pMapViewWnd->MAP_y) != 1)*/
									UserLeft(8);
							}
						}
					}
				}
			}
			else if(jump > 14)
			{
				/*m_pUserSpr->spr_y+=8;
				m_pMapViewWnd->MAP_y = m_pUserSpr->spr_y/32;*/
				
DOWN:
				m_pUserSpr->DropUnit++;
				UserDown(8);
			
				/*if(KeyState == 8)
				{
					if(m_pUserSpr->view == UNIT_STAND)
					{
						if(m_pMapViewWnd->MAP_x >= (m_pMapViewWnd->pMAP->WIDTH*4)-4)
							KeyState = 4;
					}else if(m_pUserSpr->view == (UNIT_STAND+1))
					{
						if(m_pMapViewWnd->MAP_x <= 0)
							KeyState = 4;
					}
				}else
				if(KeyState == 9)
				{
					if(m_pUserSpr->view == UNIT_STAND)
					{
						if(m_pMapViewWnd->MAP_x >= (m_pMapViewWnd->pMAP->WIDTH*4)-4)
							KeyState = 6;
					}else if(m_pUserSpr->view == (UNIT_STAND+1))
					{
						if(m_pMapViewWnd->MAP_x <= 0)
							KeyState = 6;
					}
				}*/
				if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x,m_pMapViewWnd->MAP_y) != 0
					&& m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x,m_pMapViewWnd->MAP_y) != 3
					&& m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x,m_pMapViewWnd->MAP_y) < 9)
					/*jump--;
					if(jump == 4)*/
					{
						jump        = 0;
					
						if(KeyState == 8)
						{
							if(m_pUserSpr->view == 0)KeyState = 2;
							else KeyState = 1;
						}
						else if(KeyState == 9)
						{
							if(m_pUserSpr->view == 0)KeyState = 2;
							else KeyState = 1;
						}
						else KeyState = 0;
	
						Kick        = 0;
						KickPlaying = 0;
						
						if(m_pUserSpr->DropUnit >= 60)KeyState = 111;
						else m_pUserSpr->DropUnit = 0;
						
						goto JUMPOUT;
					} 
					else if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x,m_pMapViewWnd->MAP_y) == 6)
						gameState = 700;
					else if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x,m_pMapViewWnd->MAP_y) == 7)
						gameState = 600;
					//UserDown(8);
				
					if(KeyState == 4 || KeyState == 8)
					{
						if(m_pUserSpr->view == UNIT_STAND)
						{
							if(m_pUserSpr->AniCount != UNIT_DOWN)
							{
								m_pSprDataWnd->pAni[UNIT_DOWN].CurFrame = 0;
								_COUNTER = UNIT_STAND;
							}
							m_pUserSpr->AniCount = UNIT_DOWN;
							if(KeyState == 8){
								if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x+4,m_pMapViewWnd->MAP_y) != 8
									&& m_pMapViewWnd->MAP_x <= (m_pMapViewWnd->pMAP->WIDTH*4)-4)/* ||
									m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x+4,m_pMapViewWnd->MAP_y) != 1)*/
									UserRight(8);
							}
						}
						else if(m_pUserSpr->view == (UNIT_STAND + 1))
						{
							if(m_pUserSpr->AniCount != (UNIT_DOWN + 2))
							{
								m_pSprDataWnd->pAni[UNIT_DOWN + 2].CurFrame = 0;
								_COUNTER = UNIT_STAND;
							}
							m_pUserSpr->AniCount = UNIT_DOWN + 2;
							if(KeyState == 8){
								if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x-4,m_pMapViewWnd->MAP_y) != 8
									&& m_pMapViewWnd->MAP_x >= 4)/* ||
									m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x-4,m_pMapViewWnd->MAP_y) != 1)*/
									UserLeft(8);
							}
						}
					}
					else if(KeyState == 6 || KeyState == 9)
					{
						if(m_pUserSpr->Interface_[0].itemnum != 6 && m_pUserSpr->Interface_[0].itemnum != 2)
						{
							Kick = 0;
							KickPlaying = 0;

							goto BUMERANG;
						}
						else
						{
							Kick++;
							AttackCheck();
							if(Kick >= 7)
							{
								Kick = 0;
								KeyState = 4;
								goto JUMPOUT;
							}
							if(m_pUserSpr->view == UNIT_STAND)
							{
								if(m_pUserSpr->AniCount != UNIT_JUMP_ATTACK)
								{
									m_pSprDataWnd->pAni[UNIT_JUMP_ATTACK].CurFrame = 0;
									_COUNTER = UNIT_STAND;
								}
								m_pUserSpr->AniCount = UNIT_JUMP_ATTACK;
								if(KeyState == 9){
									if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x+4,m_pMapViewWnd->MAP_y) != 8
										&& m_pMapViewWnd->MAP_x <= (m_pMapViewWnd->pMAP->WIDTH*4)-4)/* ||
										m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x+4,m_pMapViewWnd->MAP_y) != 1)*/
									UserRight(8);
								}
							}
							else if(m_pUserSpr->view == (UNIT_STAND + 1))
							{
								if(m_pUserSpr->AniCount != (UNIT_JUMP_ATTACK + 1))
								{
									m_pSprDataWnd->pAni[UNIT_JUMP_ATTACK + 1].CurFrame = 0;
									_COUNTER = UNIT_STAND;
								}
								m_pUserSpr->AniCount = UNIT_JUMP_ATTACK + 1;
								if(KeyState == 9){
									if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x-4,m_pMapViewWnd->MAP_y) != 8
										&& m_pMapViewWnd->MAP_x >= 4)// ||
										//m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x-4,m_pMapViewWnd->MAP_y) != 1)
										UserLeft(8);
								}
							}
						}
					}
				}
	//	}
	//	if(KeyState == 8)KeyState = 4;
	//	else if(KeyState == 9)KeyState = 6;

		return;
	}
JUMPOUT:
	
	if(KeyState == 10)
	{	//공격 
BUMERANG:
		if(m_pUserSpr->view == UNIT_STAND)
		{
			if(m_pUserSpr->Interface_[0].itemnum == 6)
			{
				if(m_pUserSpr->AniCount != UNIT_ATTACK)
				{
					m_pDSound.Playing(m_psnd.mugi.bff, false);
					
					m_pUserSpr->AniCount = UNIT_ATTACK;
					m_pSprDataWnd->pAni[UNIT_ATTACK].CurFrame = 0;
					//count=
					_COUNTER = UNIT_STAND;	
				}
			}
			else if(m_pUserSpr->Interface_[0].itemnum == 4)
			{
				if(m_pUserSpr->mis == 0)
				{
					if(m_pUserSpr->AniCount != UNIT_BUMERANG)
					{
						m_pUserSpr->AniCount = UNIT_BUMERANG;
						m_pSprDataWnd->pAni[UNIT_BUMERANG].CurFrame = 0;
						//count=
						_COUNTER = UNIT_STAND;
					}
				}
			}
			else if(m_pUserSpr->Interface_[0].itemnum == 2)
			{
				if(m_pUserSpr->AniCount != UNIT_NIFE)
				{
					m_pDSound.Playing(m_psnd.mugi.bff, false);
					
					m_pUserSpr->AniCount = UNIT_NIFE;
					m_pSprDataWnd->pAni[UNIT_NIFE].CurFrame = 0;
					//count=
					_COUNTER = UNIT_STAND;
				}
			}
			else if(m_pUserSpr->Interface_[0].itemnum == 0)
			{
				if(m_pUserSpr->mis == 0)
				{
					if(m_pUserSpr->AniCount != UNIT_GUN)
					{
						m_pDSound.Playing(m_psnd.mugi.bff, false);
						
						m_pUserSpr->AniCount = UNIT_GUN;
						m_pSprDataWnd->pAni[UNIT_GUN].CurFrame = 0;
						//count=
						_COUNTER = UNIT_STAND;
					
					//	m_pUserSpr->mis = 5;
						m_pMissail->CreateMissail(m_pUserSpr->spr_x+25,m_pUserSpr->spr_y-42,1,0,m_pUserSpr->view,0,0);
					}
				}
			}

			m_pUserSpr->view = UNIT_STAND;

			//count++;
			if(m_pUserSpr->AniCount != (UNIT_GUN) && m_pUserSpr->AniCount != (UNIT_BUMERANG))
					AttackCheck();
			else if(m_pUserSpr->AniCount == (UNIT_BUMERANG))
			{
				if(m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame == 3)
				{
					if(m_pUserSpr->mis == 0)
					{
						m_pDSound.Playing(m_psnd.mugi.bff, false);
				
						m_pMissail->CreateMissail(m_pUserSpr->spr_x+25,m_pUserSpr->spr_y-42,2,1,m_pUserSpr->view,0,0);
					}
				}
			}

			if(m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame >= m_pSprDataWnd->pAni[m_pUserSpr->AniCount].FrameNum-1)
			{
				if(KeyState != 6 && KeyState != 9)
					KeyState = 0;
				else if(KeyState == 6)
					KeyState = 4;
				else if(KeyState == 9)
					KeyState = 8;

				//count = 0;
				goto Out;
			}
			/*else
			{
				if(m_pUserSpr->Interface_[0].itemnum == 6)
					m_pUserSpr->AniCount = UNIT_ATTACK;
				else if(m_pUserSpr->Interface_[0].itemnum == 4)
					m_pUserSpr->AniCount = UNIT_BUMERANG;
				else if(m_pUserSpr->Interface_[0].itemnum == 2)
					m_pUserSpr->AniCount = UNIT_NIFE;
				else if(m_pUserSpr->Interface_[0].itemnum == 0)
					m_pUserSpr->AniCount = UNIT_GUN;
			}*/
		}
		else if(m_pUserSpr->view == (UNIT_STAND + 1))
		{	
			if(m_pUserSpr->Interface_[0].itemnum == 6)
			{
				if(m_pUserSpr->AniCount != (UNIT_ATTACK + 1))
				{
					m_pDSound.Playing(m_psnd.mugi.bff, false);
					
					m_pUserSpr->AniCount = UNIT_ATTACK+1;
					m_pSprDataWnd->pAni[UNIT_ATTACK + 1].CurFrame = 0;
					//count=
					_COUNTER = UNIT_STAND;	
				}
		//		AttackCheck();
			}
			else if(m_pUserSpr->Interface_[0].itemnum == 4)
			{
				if(m_pUserSpr->mis == 0)
				{
					if(m_pUserSpr->AniCount != (UNIT_BUMERANG + 1))
					{
						m_pUserSpr->AniCount = UNIT_BUMERANG+1;
						m_pSprDataWnd->pAni[UNIT_BUMERANG + 1].CurFrame = 0;
						//count=
						_COUNTER = UNIT_STAND;
					}
				}
			}
			else if(m_pUserSpr->Interface_[0].itemnum == 2)
			{
				if(m_pUserSpr->AniCount != (UNIT_NIFE + 1))
				{
					m_pDSound.Playing(m_psnd.mugi.bff, false);
					
					m_pUserSpr->AniCount = UNIT_NIFE+1;
					m_pSprDataWnd->pAni[UNIT_NIFE + 1].CurFrame = 0;
					//count=
					_COUNTER = UNIT_STAND;
				}
			}
			else if(m_pUserSpr->Interface_[0].itemnum == 0)
			{
				if(m_pUserSpr->AniCount != (UNIT_GUN + 1))
				{
					if(m_pUserSpr->mis == 0)
					{
						m_pDSound.Playing(m_psnd.mugi.bff, false);
						
						m_pUserSpr->AniCount = UNIT_GUN+1;
						m_pSprDataWnd->pAni[UNIT_GUN + 1].CurFrame = 0;
						/*count=*/_COUNTER = UNIT_STAND;
					
						//m_pUserSpr->mis = 1;
					
						m_pMissail->CreateMissail(m_pUserSpr->spr_x-25,m_pUserSpr->spr_y-42,0,0,m_pUserSpr->view,0,0);
					}
				}
			}

			m_pUserSpr->view = UNIT_STAND + 1;

			//count++;
			if(m_pUserSpr->AniCount != (UNIT_GUN+1) && m_pUserSpr->AniCount != (UNIT_BUMERANG+1))
			{
					AttackCheck();
			}
			else if(m_pUserSpr->AniCount == (UNIT_BUMERANG+1))
			{
				if(m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame == 3)
				{
					if(m_pUserSpr->mis == 0)
					{
						m_pDSound.Playing(m_psnd.mugi.bff, false);
						
						m_pMissail->CreateMissail(m_pUserSpr->spr_x-25,m_pUserSpr->spr_y-42,2,1,m_pUserSpr->view,0,0);
					}
				}
			}	
			
			if(m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame >= m_pSprDataWnd->pAni[m_pUserSpr->AniCount].FrameNum-1)
			{
				
				if(KeyState != 6 && KeyState != 9)
					KeyState = 0;
				else if(KeyState == 6)
					KeyState = 4;
				else if(KeyState == 9)
					KeyState = 8;

				//count    = 0;
				goto Out;
			}
			/*else
			{
				if(m_pUserSpr->Interface_[0].itemnum == 6)
					m_pUserSpr->AniCount = UNIT_ATTACK+1;
				else if(m_pUserSpr->Interface_[0].itemnum == 4)
					m_pUserSpr->AniCount = UNIT_BUMERANG+1;
				else if(m_pUserSpr->Interface_[0].itemnum == 2)
					m_pUserSpr->AniCount = UNIT_NIFE+1;
				else if(m_pUserSpr->Interface_[0].itemnum == 0)
					m_pUserSpr->AniCount = UNIT_GUN+1;
			}*/
		}
		return;
	}
	else if(KeyState == -10)
	{
		if(m_pUserSpr->view == UNIT_STAND)
		{
			m_pDSound.Playing(m_psnd.holkick.bff, false);
			if(m_pUserSpr->AniCount != UNIT_SKILL)
			{		
				m_pUserSpr->AniCount = UNIT_SKILL;
				m_pSprDataWnd->pAni[UNIT_SKILL].CurFrame = 0;
				_COUNTER = UNIT_STAND;
			}		
			if(m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame >= m_pSprDataWnd->pAni[m_pUserSpr->AniCount].FrameNum-1)
			{
				m_pDSound.Playing(m_psnd.sliding.bff, false);
				m_pMissail->CreateMissail(m_pUserSpr->spr_x+25,m_pUserSpr->spr_y-42,10,0,m_pUserSpr->view,0,1);
					
				KeyState = 0;
				goto Out;
			}
		}else if(m_pUserSpr->view == (UNIT_STAND+1))
		{
			m_pDSound.Playing(m_psnd.holkick.bff, false);
			if(m_pUserSpr->AniCount != UNIT_SKILL+1)
			{		
				m_pUserSpr->AniCount = UNIT_SKILL+1;
				m_pSprDataWnd->pAni[UNIT_SKILL+1].CurFrame = 0;
				_COUNTER = UNIT_STAND;
			}		
			if(m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame >= m_pSprDataWnd->pAni[m_pUserSpr->AniCount].FrameNum-1)
			{
				m_pDSound.Playing(m_psnd.sliding.bff, false);
				m_pMissail->CreateMissail(m_pUserSpr->spr_x+25,m_pUserSpr->spr_y-42,11,0,m_pUserSpr->view,0,1);
					
				KeyState = 0;
				goto Out;
			}
		}
		
		return;
	}
	//움직임 처리
	else if(KeyState == 3 || KeyState == 5 || KeyState == 7)//)
	{
		//앉기공격
		if(KeyState == 5)
		{
			if(m_pUserSpr->view == UNIT_STAND)
			{
				if(m_pUserSpr->AniCount != UNIT_HOLL_ATTACK)
				{
					m_pDSound.Playing(m_psnd.holkick.bff, false);
					
					m_pSprDataWnd->pAni[UNIT_HOLL_ATTACK].CurFrame = 0;
					_COUNTER = UNIT_STAND;
				}
				m_pUserSpr->view = UNIT_STAND;

				count++;
				if(count > m_pSprDataWnd->pAni[UNIT_HOLL_ATTACK].FrameNum)
				{
					AttackCheck();
					KeyState = 0;
					count    = 0;
					goto Out;
				}
				else
					m_pUserSpr->AniCount = UNIT_HOLL_ATTACK;
			}
			else if(m_pUserSpr->view == (UNIT_STAND + 1))
			{
				if(m_pUserSpr->AniCount != (UNIT_HOLL_ATTACK + 1))
				{
					m_pDSound.Playing(m_psnd.holkick.bff, false);
					
					m_pSprDataWnd->pAni[UNIT_HOLL_ATTACK + 1].CurFrame = 0;
					_COUNTER = UNIT_STAND;
				}
				m_pUserSpr->view = UNIT_STAND + 1;

				count++;
				if(count > m_pSprDataWnd->pAni[UNIT_HOLL_ATTACK + 1].FrameNum)
				{
					AttackCheck();
					KeyState = 0;
					count    = 0;
					goto Out;
				}
				else
				m_pUserSpr->AniCount = UNIT_HOLL_ATTACK + 1;
			}
		}
		else if(KeyState == 3)
		{
			//앉기
			if(m_pUserSpr->view == UNIT_STAND)
			{
				if(m_pUserSpr->AniCount != UNIT_SITDOWN)
				{
					m_pSprDataWnd->pAni[UNIT_SITDOWN].CurFrame = 0;
//				frame = 0.0f;
					_COUNTER = UNIT_STAND;
				}
				m_pUserSpr->AniCount = UNIT_SITDOWN;
			}
			else if(m_pUserSpr->view == (UNIT_STAND + 1))
			{
				if(m_pUserSpr->AniCount != (UNIT_SITDOWN + 1))
				{
					m_pSprDataWnd->pAni[UNIT_SITDOWN + 1].CurFrame = 0;
//					frame = 0.0f;
					_COUNTER = UNIT_STAND;
				}
				m_pUserSpr->AniCount = UNIT_SITDOWN + 1;
			}
			KeyState = 0;
		}
		else if(KeyState == 7)
		{
			//언덕길이면 하지 않는다.
			if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x+1,m_pMapViewWnd->MAP_y-2) == 9 ||
				m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x+1,m_pMapViewWnd->MAP_y-2) == 10)
			{
				KeyState = 0;
				return;
			}
			//슬라이딩
			sliding++;
			if(sliding > 5)
			{
				sliding = 0;
				KeyState = 0;
				return;
			}
			
			if(m_pUserSpr->view == UNIT_STAND)
			{
				if(m_pUserSpr->AniCount != UNIT_SLIDE)
				{
					m_pDSound.Playing(m_psnd.sliding.bff, false);
					
					m_pSprDataWnd->pAni[UNIT_SLIDE].CurFrame = 0;
					_COUNTER = UNIT_STAND;
				}
				m_pUserSpr->AniCount = UNIT_SLIDE;
			
				if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x+4,m_pMapViewWnd->MAP_y-1) == 8
					|| m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x+4,m_pMapViewWnd->MAP_y) == 0 
					|| m_pMapViewWnd->MAP_x >= (m_pMapViewWnd->pMAP->WIDTH*4)-4)
				{
					sliding = 0;
					KeyState = 0;
					return;
				}
				UserRight(16);
			}
			else if(m_pUserSpr->view == (UNIT_STAND + 1))
			{
				if(m_pUserSpr->AniCount != (UNIT_SLIDE + 1))
				{
					m_pDSound.Playing(m_psnd.sliding.bff, false);
					
					m_pSprDataWnd->pAni[UNIT_SLIDE + 1].CurFrame = 0;
					_COUNTER = UNIT_STAND;
				}
				m_pUserSpr->AniCount = UNIT_SLIDE + 1;
				
				if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x-4,m_pMapViewWnd->MAP_y-1) == 8
					|| m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x-4,m_pMapViewWnd->MAP_y) == 0 
					|| m_pMapViewWnd->MAP_x <= 4)
				{
					sliding = 0;
					KeyState = 0;
					return;
				}
				UserLeft(16);
			}
		}
		return;
	}
   else if(KeyState == 11)//)
	{
		if(pos_cnt == 24)
		{
			if(move_x == 1)
			{
				m_pMapViewWnd->map_x-=8;
				m_pMapViewWnd->map_X-=8;//m_pMapViewWnd->map_x%32;
			}
			m_pUserSpr->spr_x-=8;
		
			m_pMiniMap->Mini_Map_User_x = m_pMapViewWnd->MAP_x = m_pUserSpr->spr_x/8;
			pos_cnt=m_pUserSpr->spr_x%32;
		}
		else if(pos_cnt == 8)
		{
			if(move_x == 1)
			{
				m_pMapViewWnd->map_x+=8;
				m_pMapViewWnd->map_X+=8;//m_pMapViewWnd->map_x%32;
			}
			m_pUserSpr->spr_x+=8;
		
			m_pMiniMap->Mini_Map_User_x = m_pMapViewWnd->MAP_x = m_pUserSpr->spr_x/8;
			pos_cnt=m_pUserSpr->spr_x%32;
		}
		else if(pos_cnt == 0)
		{
			if(move_x == 1)
			{
				m_pMapViewWnd->map_x+=16;
				m_pMapViewWnd->map_X+=16;//m_pMapViewWnd->map_x%32;
			}
			m_pUserSpr->spr_x+=16;
		
			m_pMiniMap->Mini_Map_User_x = m_pMapViewWnd->MAP_x = m_pUserSpr->spr_x/8;
			pos_cnt=m_pUserSpr->spr_x%32;
		}
			
			
		if(m_pUserSpr->AniCount != UNIT_UP_WALK )
		{
			_COUNTER = 0;
		}

		m_pUserSpr->view = UNIT_STAND;
		m_pUserSpr->AniCount = UNIT_UP_WALK ;
		
		UserUp(8);
				
		_COUNTER++;
		KeyState = 110;
	}
	else if(KeyState == 12)
	{
		if(m_pUserSpr->AniCount != UNIT_DOWN_WALK )
		{
			_COUNTER = 0;
		}
		m_pUserSpr->view = UNIT_STAND;
		m_pUserSpr->AniCount = UNIT_DOWN_WALK ;
		
		if(count == 0)
		{
			if(move_y == 1)
			{
				m_pMapViewWnd->map_y-=32;
				m_pMapViewWnd->map_Y-=32;//m_pMapViewWnd->map_y%32;
			}
			m_pUserSpr->spr_y-=32;	
			
			m_pMiniMap->Mini_Map_User_y = m_pMapViewWnd->MAP_y = m_pUserSpr->spr_y/8;
		}
		
		count++;
		if(count > m_pSprDataWnd->pAni[m_pUserSpr->AniCount].FrameNum)
		{
			count    = 0;
			KeyState = 0;
			goto Out;
		}
	}
	else if(KeyState == 13)
	{
		if(pos_cnt == 24)
		{
			if(move_x == 1)
			{
				m_pMapViewWnd->map_x-=8;
				m_pMapViewWnd->map_X-=8;//m_pMapViewWnd->map_x%32;
			}
            m_pUserSpr->spr_x-=8;
			
			m_pMiniMap->Mini_Map_User_x = m_pMapViewWnd->MAP_x = m_pUserSpr->spr_x/8;
			pos_cnt=m_pUserSpr->spr_x%32;
		}
		else if(pos_cnt == 8)
		{
			if(move_x == 1)
			{
				m_pMapViewWnd->map_x+=8;
				m_pMapViewWnd->map_X+=8;//m_pMapViewWnd->map_x%32;
			}
			m_pUserSpr->spr_x+=8;
		
			m_pMiniMap->Mini_Map_User_x = m_pMapViewWnd->MAP_x = m_pUserSpr->spr_x/8;
			pos_cnt=m_pUserSpr->spr_x%32;
		}
		else if(pos_cnt == 0)
		{
			if(move_x == 1)
			{
				m_pMapViewWnd->map_x+=16;
				m_pMapViewWnd->map_X+=16;//m_pMapViewWnd->map_x%32;
			}
			m_pUserSpr->spr_x+=16;
			
			m_pMiniMap->Mini_Map_User_x = m_pMapViewWnd->MAP_x = m_pUserSpr->spr_x/8;
			pos_cnt=m_pUserSpr->spr_x%32;
		}

		if(m_pUserSpr->AniCount != UNIT_DOWN_WALK )
		{
			_COUNTER = 0;
		}
		m_pUserSpr->view = UNIT_STAND;
		m_pUserSpr->AniCount = UNIT_DOWN_WALK ;
		
		count++;
		if(count > m_pSprDataWnd->pAni[m_pUserSpr->AniCount].FrameNum)
		{
			count = 0;
			KeyState = 14;
		
			if(move_y == 1)
			{
				m_pMapViewWnd->map_y+=32;
				m_pMapViewWnd->map_Y+=32;//m_pMapViewWnd->map_y%32;
			}
			m_pUserSpr->spr_y+=32;	
			m_pMiniMap->Mini_Map_User_y = m_pMapViewWnd->MAP_y = m_pUserSpr->spr_y/8;
			goto Out;
		}
	} 
	else if(KeyState == 14)//)
	{
		if(m_pUserSpr->AniCount != UNIT_UP_WALK )
		{
			_COUNTER = 0;
		}

		m_pUserSpr->view = UNIT_STAND;
		m_pUserSpr->AniCount = UNIT_UP_WALK ;
		
		UserDown(8);
				
		_COUNTER++;
		KeyState = 110;
	}
	else if(KeyState == 2)//)
	{
		//오른쪽
		if(m_pMagic)
		{
			if(m_pUserSpr->AniCount != UNIT_RUN)
			{
				m_pUserSpr->AniCount = UNIT_RUN;
				_COUNTER = 0;
			}
		}else
		{
			if(m_pUserSpr->AniCount != UNIT_WALK)
			{
				m_pUserSpr->AniCount = UNIT_WALK;
				_COUNTER = 0;
			}
		}
		m_pUserSpr->view = UNIT_STAND;
		
		undek = KeyState = 0;

AGAINR:
		if(m_pMapViewWnd->MAP_x >= ((m_pMapViewWnd->pMAP->WIDTH*4)-4))
			return;
		
		if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x+1,m_pMapViewWnd->MAP_y) == 6)
			gameState = 700;
		else if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x+1,m_pMapViewWnd->MAP_y) == 7)
			gameState = 600;
		else if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x,m_pMapViewWnd->MAP_y-1) == 9)
			undek = 2;//오르막
		else if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x+1,m_pMapViewWnd->MAP_y) == 10)
			undek = 1;//내리막
		else if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x,m_pMapViewWnd->MAP_y) == 0)
		{
			KeyState = 4;
			jump     = 15;
			goto DOWN;
		}else if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x+5,m_pMapViewWnd->MAP_y) == 8)// ||
			//m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x-2,m_pMapViewWnd->MAP_y-4) == 1)
			return;
	
		UserRight(size_x);
		
		if(undek == 1) 
			UserDown(8);
		else if(undek == 2)
			UserUp(8);
		
		if(m_pMagic)
		{
			if(rn == 1)
			{
				rn = 0;
				return ;
			}else 
			{
				rn++;
				goto AGAINR;
			}
		}
		//sprintf((char *)msg,"%d,%d,%d\0",m_pMapViewWnd->MAP_x,m_pUserSpr->spr_x,m_pUserSpr->spr_y);
		//SetWindowText(hhh,msg); 

		return;

	}else if(KeyState == 1)//m_pUserSpr->AniCount == (UNIT_WALK + 1))
	{
		//왼쪽
		if(m_pMagic)
		{
			if(m_pUserSpr->AniCount != (UNIT_RUN+1))
			{
				m_pUserSpr->AniCount = UNIT_RUN + 1;
				_COUNTER = 0;
				
			}
		}else
		{
			if(m_pUserSpr->AniCount != (UNIT_WALK+1))
			{
				m_pUserSpr->AniCount = UNIT_WALK + 1;
				_COUNTER = 0;
			}
		}
		m_pUserSpr->view = UNIT_STAND + 1;
		undek = KeyState = 0;
AGAINL:		
		if(m_pMapViewWnd->MAP_x <= 4)
			return;

		if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x-1,m_pMapViewWnd->MAP_y) == 6)
			gameState = 700;
		else if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x-1,m_pMapViewWnd->MAP_y) == 7)
			gameState = 600;
		else if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x-1,m_pMapViewWnd->MAP_y) == 9)
			undek = 1;//내리막
		else if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x,m_pMapViewWnd->MAP_y-1) == 10)
			undek = 2;//오르막
		else if(undek == 0 && m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x,m_pMapViewWnd->MAP_y) == 0)
		{
			KeyState = 4;
			jump     = 15;
			goto DOWN;
		}else if(m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x-5,m_pMapViewWnd->MAP_y) == 8)// ||
		//	m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x-2,m_pMapViewWnd->MAP_y-4) == 1)
			return;
       
		UserLeft(size_x);
		
		if(undek == 1) 
			UserDown(8);
		else if(undek == 2)
			UserUp(8);
		
		if(m_pMagic)
		{
			if(rn == 1)
			{
				rn = 0;
				return ;
			}else 
			{
				rn++;
				goto AGAINL;
			}
		}
		//sprintf((char *)msg,"%d,%d,%d\0",m_pMapViewWnd->MAP_x,m_pUserSpr->spr_x,m_pUserSpr->spr_y);
		//SetWindowText(hhh,msg); 

		return;
	}

Out:
	if(KeyState == 0)
	{
		if(m_pUserSpr->view == UNIT_STAND)
		{
			if(undek == 0 && m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x+1,m_pMapViewWnd->MAP_y) == 0)
			{
				//if(pos_cnt == 8)
				//{
					if(m_pUserSpr->AniCount != UNIT_DANGER)
					{
						m_pSprDataWnd->pAni[UNIT_DANGER].CurFrame = 0;
						_COUNTER = UNIT_STAND;
					}
					m_pUserSpr->AniCount = UNIT_DANGER;
				//}
			}else{
				if(m_pUserSpr->AniCount != UNIT_STAND)
				{
					m_pSprDataWnd->pAni[UNIT_STAND].CurFrame = 0;
					_COUNTER = UNIT_STAND;
				}
				m_pUserSpr->AniCount = UNIT_STAND;
			}
		}
		else if(m_pUserSpr->view == (UNIT_STAND + 1))
		{
			if(undek == 0 && m_pMapViewWnd->pMAP->GetAttr(m_pMapViewWnd->MAP_x-1,m_pMapViewWnd->MAP_y) == 0)
			{
				//if(pos_cnt == 24)
				//{
					if(m_pUserSpr->AniCount != (UNIT_DANGER + 1))
					{
						m_pSprDataWnd->pAni[UNIT_DANGER + 1].CurFrame = 0;
						_COUNTER = UNIT_STAND;
					}
					m_pUserSpr->AniCount = UNIT_DANGER + 1;
				//}
			}else{
				if(m_pUserSpr->AniCount != (UNIT_STAND + 1))
				{
					m_pSprDataWnd->pAni[UNIT_STAND + 1].CurFrame = 0;
					_COUNTER = UNIT_STAND;
				}
				m_pUserSpr->AniCount = UNIT_STAND + 1;
			}
		}
	}
}

void
cUnitManager::UserUp(int size)
{
	move_y = 0;
	
	m_pUserSpr->spr_y-=size;
	m_pMiniMap->Mini_Map_User_y = m_pMapViewWnd->MAP_y = m_pUserSpr->spr_y/8;
	
	if(m_pMapViewWnd->MAP_y >= 40 && m_pMapViewWnd->MAP_y <= ((m_pMapViewWnd->pMAP->HEIGHT*4)-40))
	{
		m_pMapViewWnd->map_y-=size;
		m_pMapViewWnd->map_Y=m_pMapViewWnd->map_y%32;
			
	//	if(m_pMapViewWnd->map_Y <= -1)
	//	{
		//	m_pMapViewWnd->map_Y = 3;
			m_pMapViewWnd->Map_y= m_pMapViewWnd->map_y/32;
	//	}			
		
		move_y = 1;
	}
}
void
cUnitManager::UserDown(int size)
{
	move_y = 0;
	
	if(m_pMapViewWnd->MAP_y >= 40 && m_pMapViewWnd->MAP_y <= ((m_pMapViewWnd->pMAP->HEIGHT*4)-40))
	{
		m_pMapViewWnd->map_y+=size;
		m_pMapViewWnd->map_Y=m_pMapViewWnd->map_y%32;
			
		//if(m_pMapViewWnd->map_Y >= 5)
		//{
		//	m_pMapViewWnd->map_Y = 1;
			m_pMapViewWnd->Map_y= m_pMapViewWnd->map_y/32;
		//}			
		
		move_y = 1;
	}
	
	m_pUserSpr->spr_y+=size;
	m_pMiniMap->Mini_Map_User_y = m_pMapViewWnd->MAP_y = m_pUserSpr->spr_y/8;
}
void
cUnitManager::UserLeft(int size)
{	
	move_x = 0;
	
	if(m_pMapViewWnd->MAP_x >= 52 && m_pMapViewWnd->MAP_x <= ((m_pMapViewWnd->pMAP->WIDTH*4)-52))
	{
	
		m_pMapViewWnd->map_x-=size;
      	m_pMapViewWnd->map_X=m_pMapViewWnd->map_x%32;
			
		//if(m_pMapViewWnd->map_X == 0)
		//{
		//	m_pMapViewWnd->map_X = 3;
			m_pMapViewWnd->Map_x=m_pMapViewWnd->map_x/32;
		//}
		
	  m_pMapViewWnd->show = 0;
	  move_x = 1;			
	}
	m_pUserSpr->spr_x-=size;
	pos_cnt = m_pUserSpr->spr_x%32;
	
	m_pMiniMap->Mini_Map_User_x = m_pMapViewWnd->MAP_x = m_pUserSpr->spr_x/8;
}
void
cUnitManager::UserRight(int size)
{
	m_pUserSpr->spr_x+=size;
	pos_cnt = m_pUserSpr->spr_x%32;
	m_pMiniMap->Mini_Map_User_x = m_pMapViewWnd->MAP_x = m_pUserSpr->spr_x/8;
	
	move_x = 0;
	
	if(m_pMapViewWnd->MAP_x >= 52 && m_pMapViewWnd->MAP_x <= ((m_pMapViewWnd->pMAP->WIDTH*4)-52))
	{
		m_pMapViewWnd->map_x+=size;
		m_pMapViewWnd->map_X=m_pMapViewWnd->map_x%32;
			
		//if(m_pMapViewWnd->map_X >= 5)
		//{
		//	m_pMapViewWnd->map_X = 1;
			m_pMapViewWnd->Map_x= m_pMapViewWnd->map_x/32;
		//}			
		m_pMapViewWnd->show = move_x = 1;
	}
}
void
cUnitManager::AttackCheck()
{
	RECT rc;
	int  ck=0;
		
	
	if(m_pSprDataWnd->hitbox[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].Sprite] == 2)
	{
		rc.left   = xX + m_pSprDataWnd->CrashBox[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].Sprite][1].left;
		rc.right  = xX + m_pSprDataWnd->CrashBox[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].Sprite][1].right;
		rc.top    = yY + m_pSprDataWnd->CrashBox[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].Sprite][1].top;
		rc.bottom = yY + m_pSprDataWnd->CrashBox[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].pFrame[m_pSprDataWnd->pAni[m_pUserSpr->AniCount].CurFrame].Sprite][1].bottom;
		
		if(m_pMonster->mondatATotalnum > 0)
		{
			ck = m_pMonster->AttackMonster(rc,m_pUserSpr->view,
				(m_pUserSpr->Str/3)+A_Item[m_pUserSpr->Interface_[0].checked].att);
		}else if(m_pBoss)
		{
			ck = m_pBoss->AttackBoss(rc,m_pUserSpr->view,
				(m_pUserSpr->Str/3)+A_Item[m_pUserSpr->Interface_[0].checked].att);
		}
		
		if(ck >	0)
		{
			m_pUserSpr->Kyunghum+=ck;
			if(m_pUserSpr->Kyunghum >= m_pUserSpr->totalKyunghum)
			{
				if(m_pUserSpr->Level <= 150)
				{
					m_pDSound.Playing(m_psnd.lvup.bff, false);
					
					m_pStateMss->m_pEcffect->CreateHit(m_pUserSpr->spr_x,m_pUserSpr->spr_y,1);
					
					char sr[30];
					sprintf(sr,"레벨업하였습니다(%d->%d).",m_pUserSpr->Level,m_pUserSpr->Level+1);
					m_pHelp->CreateHelp(sr);
					sprintf(sr,"축하합니다.");
					m_pHelp->CreateHelp(sr);

					m_pUserSpr->Kyunghum      = 0;
					m_pUserSpr->Level          ++;
				
					m_pUserSpr->totalHp+=6;
					m_pUserSpr->totalMp+=3;
					m_pUserSpr->Hp = m_pUserSpr->totalHp;
					m_pUserSpr->Mp = m_pUserSpr->totalMp;
				
					m_pUserSpr->stackPoint+=5;
				
					int l = m_pUserSpr->Level+1;
					m_pUserSpr->totalKyunghum = (int)(pow((float)m_pUserSpr->Level,2) * (pow((float)l,2)-13*l+82));
				}
			}
		}
	}
}