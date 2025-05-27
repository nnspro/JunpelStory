#include "MapOutput.h"
#include "Help.h"
#include "StatusForm.h"
#include "Missail.h"
#include "DirectSound.h"
#include "Boss.h"

#include <stdio.h>
#include <mmsystem.h>
#include <time.h>

int  mmmmm = 0,boss_hit_x=0,boss_hit_y=0;
int  hittime = 0;

char damagetxt[5];

cBossWnd::cBossWnd()
{
	m_pDirect3D   = NULL;
	
	m_pBossSprWnd = NULL;

	srand( (unsigned)time(NULL));
}

cBossWnd::~cBossWnd()
{
	SAFE_DELETE_(m_pD3dxfont);
	SAFE_DELETE_(m_pBossSprWnd);
}

BOOL 
cBossWnd::Init(CDirect3D   *D3D)
{
	m_pDirect3D = D3D;
	
	m_pD3dxfont = NULL;
	m_pD3dxfont = new CD3dxFont();
	if(!m_pD3dxfont->CreateFONT(m_pDirect3D->GetDevice(), "돋움체", 70))
		return FALSE;

	if(!InitData())return FALSE;

	return TRUE;
}

BOOL
cBossWnd::InitData()
{
	char bf[256];
	int  temp;
	
	FILE *bossfp;

	if(m_pBossSprWnd)
		SAFE_DELETE_(m_pBossSprWnd);

	sprintf(bf,"txt/boss/boss_%d.txt",m_pMapViewWnd->game_map_stage);
	if(!(bossfp = fopen(bf,"rt")))
		return FALSE;
	
	fscanf(bossfp,"%d",&temp);
	
	m_pBossSprWnd = new cSprDataWnd();
	m_pBossSprWnd->Init(m_pDirect3D);
	m_pBossSprWnd->SprLoad(temp);
	m_pBossSprWnd->LoadSprTexture();
	
	fscanf(bossfp,"%d",&enugy);
	fscanf(bossfp,"%d",&str);
	fscanf(bossfp,"%d",&dex);
	fscanf(bossfp,"%d",&attrib);
	fscanf(bossfp,"%d",&attack);
	fscanf(bossfp,"%d",&boss_x);
	fscanf(bossfp,"%d",&boss_y);
	fscanf(bossfp,"%d",&bonus);
	fscanf(bossfp,"%d",&speed);
	
	COUNTER = 0;
		
	boss_map_x = boss_x/8;
	boss_map_y = boss_y/8;

	aninum = move_way =  0;
	boss_move_check   = -1;
	
	turbo     = 0;
	turbotime = -1;

	ZeroMemory(damagetxt,5);
	
	fclose(bossfp);

	return TRUE;

}
void 
cBossWnd::PutImage(int monX,int monY,int monA)
{
	CUSTOM_VERTEX*	pV = NULL;
 
	m_pDirect3D->GetDevice()->SetTexture( 0, 
									 m_pBossSprWnd->g_pSprTexture[m_pBossSprWnd->pAni[monA].pFrame[m_pBossSprWnd->pAni[monA].CurFrame].Sprite] );

	m_pMapViewWnd->g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

	for( int m=0; m < 4; m++ )
	{
      pV[m].vPos.z = 1.0f;
	  pV[m].rhw = 1.0f;
	  pV[m].dwDiffuse = 0xffffffff;
	}
		

	//m_pMonSprWnd[monN]->pAni[monA].CurFrame = Mon[n].CurFrame;
	float	fLeft   = (float)monX;//m_pMonSprWnd[monN]->pAni[monA].CurFrame
	float	fWidth  = (float)m_pBossSprWnd->w_Width[m_pBossSprWnd->pAni[monA].pFrame[m_pBossSprWnd->pAni[monA].CurFrame].Sprite];
	float	fTop    = (float)monY;
	float	fHeight = (float)m_pBossSprWnd->h_Height[m_pBossSprWnd->pAni[monA].pFrame[m_pBossSprWnd->pAni[monA].CurFrame].Sprite];

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
cBossWnd::Render()
{
	if(m_pStateMss->m_pEcffect)
		m_pStateMss->m_pEcffect->Render();
}

void 
cBossWnd::BossMove(int userx,int usery)
{
	if(boss_move_check == 2)
	{
		if(attrib == 0)//사용자와 거리를 유지하면서 빠지면서 공격.
		{
			if(userx <= boss_x-350)
				boss_move_check = -1;
		}else if(attrib == 1)//사용자 유닛 근처로 이동후 공격
		{
			if(userx >= boss_x+100 && userx <= boss_x+110)
				boss_move_check = -2;
			else if(userx <= boss_x-115)// && userx >= boss_x-200)
				boss_move_check = 1;
			else if(userx > boss_x+115)// && userx <= boss_x+200)
				boss_move_check = 2;
		}
	}else
		if(boss_move_check == 1)
	{
		if(attrib == 0)//사용자와 거리를 유지하면서 빠지면서 공격.
		{
			if(userx >= boss_x+350)
				boss_move_check = -2;
		}else if(attrib == 1)//사용자 유닛 근처로 이동후 공격
		{
			if(userx <= boss_x-100 && userx >= boss_x-110)
				boss_move_check = -1;
			else if(userx <= boss_x-115)// && userx >= boss_x-200)
				boss_move_check = 1;
			else if(userx > boss_x+115)// && userx <= boss_x+200)
				boss_move_check = 2;
		}
	}else
        if(boss_move_check == -2)
	{
		if(aninum != BOSS_ETC+1)
		{
			aninum = BOSS_ETC+1;
			COUNTER = 0;
		}
		if(m_pBossSprWnd->pAni[aninum].CurFrame
			>= m_pBossSprWnd->pAni[aninum].FrameNum-1)
				boss_move_check = 0;
	}else
		if(boss_move_check == -1)
	{
		if(aninum != BOSS_ETC)
		{
			aninum = BOSS_ETC;
			COUNTER = 0;
		}
		if(m_pBossSprWnd->pAni[aninum].CurFrame
			>= m_pBossSprWnd->pAni[aninum].FrameNum-1)
				boss_move_check = 0;
	}else
		if(boss_move_check == 0)
	{
		if(userx <= boss_x)
		{
			if(attrib == 0)
			{
				if(m_pBossSprWnd->pAni[aninum].CurFrame == 0 && mmmmm == 0)
				{
					m_pMissail->CreateMissail(boss_x-50,boss_y-72,5,0,1,1,0);
					mmmmm = 1;
				}else mmmmm = 0;

				if(m_pBossSprWnd->pAni[aninum].CurFrame
				>= m_pBossSprWnd->pAni[aninum].FrameNum-1)
				{
					if(userx <= boss_x && userx >= boss_x-200)
					{
						boss_move_check = 1;
					}else if(userx > boss_x && userx <= boss_x+200)
					{
						boss_move_check = 2;
					}
				}
			}else if(attrib == 1)
			{
				if(m_pBossSprWnd->pAni[aninum].CurFrame
					>= m_pBossSprWnd->pAni[aninum].FrameNum-1)
				{
					if(userx <= boss_x-115)// && userx >= boss_x-200)
					{
						boss_move_check = 1;
					}else if(userx > boss_x+115)// && userx <= boss_x+50)
					{
						boss_move_check = 2;
					}
				}
			}

			m_pDSound.Playing(m_psnd.mongun.bff, false);
			
			if(aninum != BOSS_ATTACK)
			{
				aninum = BOSS_ATTACK;
				COUNTER = 0;
			}
		}else if(userx > boss_x)
		{
			if(attrib == 0)
			{
				if(m_pBossSprWnd->pAni[aninum].CurFrame == 0 && mmmmm == 0)
				{
					m_pMissail->CreateMissail(boss_x+50,boss_y-72,6,0,0,1,0);
					mmmmm = 1;
				}else mmmmm = 0;

				if(m_pBossSprWnd->pAni[aninum].CurFrame
					>= m_pBossSprWnd->pAni[aninum].FrameNum-1)
				{
					if(userx <= boss_x && userx >= boss_x-200)
					{
						boss_move_check = 1;
					}else if(userx > boss_x && userx <= boss_x+200)
					{
						boss_move_check = 2;
					}
				}
			}else if(attrib == 1)
			{
				if(m_pBossSprWnd->pAni[aninum].CurFrame
					>= m_pBossSprWnd->pAni[aninum].FrameNum-1)
				{
					if(userx <= boss_x-115)// && userx >= boss_x-200)
					{
						boss_move_check = 1;
					}else if(userx > boss_x+115)// && userx <= boss_x+200)
					{
						boss_move_check = 2;
					}
				}
			}

			m_pDSound.Playing(m_psnd.mongun.bff, false);
			
			if(aninum != BOSS_ATTACK+1)
			{
				aninum = BOSS_ATTACK+1;
				COUNTER = 0;
			}
		}
	}

	if(boss_move_check == 1)
	{
		boss_x-=speed;
		boss_map_x = boss_x/8;

		if(boss_map_x <= 4)
			boss_move_check = 2;

		if(aninum != BOSS_STAND)
		{
			aninum = BOSS_STAND;
			COUNTER = 0;
		}
	}else
		if(boss_move_check == 2)
	{
		boss_x+=speed;
		boss_map_x = boss_x/8;
		
		if(boss_map_x >= ((m_pMapViewWnd->pMAP->WIDTH*4)-8))
			boss_move_check = 1;

		if(aninum != BOSS_STAND+1)
		{
			aninum = BOSS_STAND+1;
			COUNTER = 0;
		}
	}
	
	if(boss_move_check == 5)
	{
		if(userx <= boss_x)
		{
			if(aninum != BOSS_HIT)
			{
				aninum = BOSS_HIT;
				COUNTER = 0;
			}
			boss_move_check = 6;
		}else if(userx > boss_x)
		{
			if(aninum != BOSS_HIT+1)
			{
				aninum = BOSS_HIT+1;
				COUNTER = 0;
			}
			boss_move_check = 7;
		}
		
	}else if(boss_move_check == 6)
	{
		boss_hit_y-=10;
   
		m_pD3dxfont->DrawText(boss_hit_x,boss_hit_y,0,0,
			D3DCOLOR_XRGB(255, 0, 0),damagetxt,strlen(damagetxt));

		hittime++;
		if(hittime > 8)
		{
			hittime = 0;
			boss_move_check = 1;
			turbotime = 30;
		}
	}else if(boss_move_check == 7)
	{
		boss_hit_y-=10;
   
		m_pD3dxfont->DrawText(boss_hit_x,boss_hit_y,0,0,
			D3DCOLOR_XRGB(255, 0, 0),damagetxt,strlen(damagetxt));

		hittime++;
		if(hittime > 8)
		{
			hittime = 0;
			boss_move_check = 2;
			turbotime = 30;
			
		}
	}else if(boss_move_check == 8)
	{
		if(userx <= boss_x)
		{
			if(aninum != BOSS_DIE)
			{
				aninum = BOSS_DIE;
				COUNTER = 0;
			}
			boss_move_check = 9;
		}else if(userx > boss_x)
		{
			if(aninum != BOSS_DIE+1)
			{
				aninum = BOSS_DIE+1;
				COUNTER = 0;
			}
			boss_move_check = 9;
		}
	}else if(boss_move_check == 9)
	{
		if(m_pBossSprWnd->pAni[aninum].CurFrame
			>= m_pBossSprWnd->pAni[aninum].FrameNum-1)
		{
			gameState = 800;
			m_pMapViewWnd->mss_cl[(m_pMapViewWnd->q_npc&0xfff000)>>12] = 1;
			char bb[256];
			sprintf(bb,"txt/npc/mssck/mss_%d.txt",m_pMapViewWnd->game_map_stage-2);
			FILE *save = fopen(bb,"wt");
			for(int p=0;p<(m_pMapViewWnd->q_npc&0x000fff);p++)
			{
				fprintf(save,"%d\n",m_pMapViewWnd->mss_cl[p]);
			}
			fclose(save);
		}
	}/*else if(boss_move_check == 10)
	{
		if(m_pBossSprWnd->pAni[aninum].CurFrame
			>= m_pBossSprWnd->pAni[aninum].FrameNum-1)
		{
			gameState = 800;
			m_pMapViewWnd->mss_cl[(m_pMapViewWnd->q_npc&0xfff000)>>12] = 1;
			char bb[256];
			sprintf(bb,"txt/npc/mssck/mss_%d.txt",m_pMapViewWnd->game_map_stage-2);
			FILE *save = fopen(bb,"wt");
			for(int p=0;p<(m_pMapViewWnd->q_npc&0x000fff);p++)
			{
				fprintf(save,"%d\n",m_pMapViewWnd->mss_cl[p]);
			}
			fclose(save);
		}
	}*/

	if(turbotime >= 0)
	{
		turbotime--;
		turbo = 1-turbo;
		if(turbotime < 0)
		{
			turbotime = -1;
			turbo = 0;
		}
	}

	COUNTER++;
	m_pBossSprWnd->pAni[aninum].CurFrame=(COUNTER/m_pBossSprWnd->pAni[aninum].Delay)%m_pBossSprWnd->pAni[aninum].FrameNum;

}

int
cBossWnd::HitBoss(RECT UserXY)
{
	if(boss_move_check < 5 && turbotime < 0)
	{
		int animations = aninum;
		int fr         = m_pBossSprWnd->pAni[animations].pFrame[m_pBossSprWnd->pAni[animations].CurFrame].Sprite;
       
		int n_x        = boss_x + m_pBossSprWnd->pAni[animations].pFrame[m_pBossSprWnd->pAni[animations].CurFrame].XPos
								- m_pMapViewWnd->map_x;
		int	n_y        = boss_y + m_pBossSprWnd->pAni[animations].pFrame[m_pBossSprWnd->pAni[animations].CurFrame].YPos
								- m_pMapViewWnd->map_y;
			
		if(m_pBossSprWnd->hitbox[fr] == 2)
		{
			if(UserXY.right >= n_x+m_pBossSprWnd->CrashBox[fr][1].left && UserXY.bottom >= n_y+m_pBossSprWnd->CrashBox[fr][1].top
				&& UserXY.left <= n_x+m_pBossSprWnd->CrashBox[fr][1].right && UserXY.top <= n_y+m_pBossSprWnd->CrashBox[fr][1].bottom)
			{
				return 1;
			}
		}
	}
	return -11;
}
int
cBossWnd::AttackBoss(RECT UserXY,int view,int str)
{
	if(boss_move_check < 5 && turbotime < 0)
	{
		int animations = aninum;
		int fr         = m_pBossSprWnd->pAni[animations].pFrame[m_pBossSprWnd->pAni[animations].CurFrame].Sprite;
		
		int n_x        = boss_x + m_pBossSprWnd->pAni[animations].pFrame[m_pBossSprWnd->pAni[animations].CurFrame].XPos
								- m_pMapViewWnd->map_x;
		int	n_y        = boss_y + m_pBossSprWnd->pAni[animations].pFrame[m_pBossSprWnd->pAni[animations].CurFrame].YPos
								- m_pMapViewWnd->map_y;
		
		if(UserXY.right >= n_x+m_pBossSprWnd->CrashBox[fr][0].left && UserXY.bottom >= n_y+m_pBossSprWnd->CrashBox[fr][0].top
				&& UserXY.left <= n_x+m_pBossSprWnd->CrashBox[fr][0].right && UserXY.top <= n_y+m_pBossSprWnd->CrashBox[fr][0].bottom)
			{	
				m_pDSound.Playing(m_psnd.hitmon.bff, false);
				//공격자의 최종 최소 공격력 - 공격 대상의 방어력/2 * 아머 타입별 데미지 적용율) ~ 
				//(공격자 최종 최대 공격력 - 공격 대상의 방어력/2 * 아머 타입별 데미지 적용율) 
				int damage = (str-dex)+rand()%3;
				if(damage <= 0)
					return -13;

				sprintf(damagetxt,"%d",damage);
				m_pStateMss->m_pEcffect->CreateHit(boss_x,boss_y,0);
										
				enugy-=damage;
				
				boss_hit_x = boss_x - m_pMapViewWnd->map_x;
				boss_hit_y = n_y; 

				boss_move_check = 5;
				//Monster 저세상.
				if(enugy <= 0)
				{
					m_pDSound.Playing(m_psnd.mondie.bff, false);
					char sr[40];
						
					boss_move_check = 8;

					//aninum  =  MON_DIE;
						
					sprintf(sr,"경험치를 얻었습니다(+%d)",bonus);
					m_pHelp->CreateHelp(sr);

					return bonus;
				}else
					return -12;
			}
	}
	return -11;
}
/*
int
cBossWnd::HitItem(RECT UserXY)
{
	return m_pStateMss->m_pEcffect->ItemChk(UserXY);
}*/