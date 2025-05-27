#include "MapOutput.h"
#include "Help.h"
#include "StatusForm.h"
#include "Missail.h"
#include "DirectSound.h"
#include "Monster.h"

#include <stdio.h>
#include <mmsystem.h>
#include <time.h>

#pragma comment		(lib, "winmm.lib")

cMonsterWnd::cMonsterWnd()
{
	m_pDirect3D  = NULL;
	//m_pMonSprWnd = NULL;

	Mon = NULL;

	//m_pD3dxfont  = NULL;
	//m_pEcffect   = NULL;

	//m_pD3dfont   = NULL;
	
	srand( (unsigned)time(NULL));
}

cMonsterWnd::~cMonsterWnd()
{
	/*if(Mon)
	{
		for (int i=0;i<monTotalnum;i++) 
		{
			SAFE_DELETE_(m_pMonSprWnd[i]);
		}
		SAFE_DELETE_(m_pMonSprWnd);
	}*/
	if(Mon)
	{
		for (int i=0;i<mondatATotalnum;i++) 
		{
			if(Mon[i].m_pMonSprWnd)
			{
				SAFE_DELETE_(Mon[i].m_pMonSprWnd);
			}
		}
		SAFE_DELETE_(Mon);
	}
	SAFE_DELETE_(m_pD3dxfont);
}

BOOL
cMonsterWnd::Init(CDirect3D   *D3D)
{
	m_pDirect3D = D3D;
	
	mondatATotalnum = monTotalnum = 0;

	m_pD3dxfont = NULL;
	m_pD3dxfont = new CD3dxFont();
	if(!m_pD3dxfont->CreateFONT(m_pDirect3D->GetDevice(), "돋움체", 70))
		return FALSE;

	if(!InitData())return FALSE;

	return TRUE;
}

BOOL 
cMonsterWnd::InitData()
{
	char bf[256];
	int  temp;
	
	FILE *monfp;

//	MonsterData *pNewEcffect = new MonsterData;

	if(Mon)
	{
		for (int i=0;i<mondatATotalnum;i++) 
		{
			if(Mon[i].m_pMonSprWnd)
			{
				SAFE_DELETE_(Mon[i].m_pMonSprWnd);
			}
		}
		SAFE_DELETE_(Mon);
	}
	sprintf(bf,"txt/monster/mon_%d.txt",m_pMapViewWnd->game_map_stage);
	if(!(monfp = fopen(bf,"rt")))
	{
		mondatATotalnum = 0;
		return FALSE;
	}

	/*SAFE_DELETE_(m_pEcffect);
	
	fscanf(monfp,"%d",&monTotalnum);
	
	m_pMonSprWnd = new cSprDataWnd *[monTotalnum];
	for(int p=0;p<monTotalnum;p++)
	{
		m_pMonSprWnd[p] = new cSprDataWnd();
		m_pMonSprWnd[p]->Init(m_pDirect3D);
		fscanf(monfp,"%d",&temp);
		m_pMonSprWnd[p]->SprLoad(temp);
		m_pMonSprWnd[p]->LoadSprTexture();
	}*/
	
	fscanf(monfp,"%d",&mondatATotalnum);
	
	Mon = new MonsterData[mondatATotalnum];
	for(int p=0;p<mondatATotalnum;p++)
	{
		//fscanf(monfp,"%d",&Mon[p].level);
		fscanf(monfp,"%d",&temp);//Mon[p].number);
		//=====================================================
				Mon[p].m_pMonSprWnd = new cSprDataWnd();
				Mon[p].m_pMonSprWnd->Init(m_pDirect3D);
				Mon[p].m_pMonSprWnd->SprLoad(temp);
				Mon[p].m_pMonSprWnd->LoadSprTexture();
         //====================================================
		//fscanf(monfp,"%d",&Mon[p].AniNumber);
		fscanf(monfp,"%d",&Mon[p].speed);
		fscanf(monfp,"%d",&Mon[p].attrib);
		fscanf(monfp,"%d",&Mon[p].attack);
		fscanf(monfp,"%d",&Mon[p].mon_x);
		fscanf(monfp,"%d",&Mon[p].mon_y);
		fscanf(monfp,"%d",&Mon[p].str);
		fscanf(monfp,"%d",&Mon[p].dex);
		fscanf(monfp,"%d",&Mon[p].bonus);
		fscanf(monfp,"%d",&Mon[p].money);
		fscanf(monfp,"%d",&Mon[p].enugy);
		fscanf(monfp,"%d",&Mon[p].reset);

		Mon[p].mon_hit_txt_x =
			Mon[p].mon_hit_txt_y = 
				Mon[p].hitting = Mon[p].move_check = -1;
		Mon[p].move_sem   =  1;
		Mon[p].AniNumber = Mon[p]._COUNTER   =  0;
		
		Mon[p].mon_X      =  Mon[p].mon_x/8;
		Mon[p].mon_Y      =  Mon[p].mon_y/8;

		ZeroMemory(Mon[p].damagetxt,5);
	}
	fclose(monfp);


/*	AddEcffect(Mon, pNewEcffect);
	m_pEcffect = new cEcffectWnd();
	m_pEcffect->Init(m_pDirect3D);*/

	return TRUE;

}
/*void 
cMonsterWnd::AddMonster(MonsterData *&pEcffect, MonsterData *pAdd)
{
	pAdd->pNext = pEcffect;
	pEcffect = pAdd;
}

void
cMonsterWnd::DeleteMonster(MonsterData *&pEcffect, MonsterData *pDelEcffect)
{
	if(pEcffect == pDelEcffect)
	{
		pEcffect = pEcffect->pNext;
	}
	else
	{
		MonsterData *pTmp = pEcffect;

		while(pTmp->pNext)
		{
			if(pTmp->pNext == pDelEcffect)
			{
				pTmp->pNext = pDelEcffect->pNext;
				break;
			}

			pTmp = pTmp->pNext;
		}
	}

	SAFE_DELETE_(pDelEcffect);
}*/
void
cMonsterWnd::ResetMonster(int num)
{
	char bf[256];
	int  temp,t;

	FILE *monfp;

	sprintf(bf,"txt/monster/mon_%d.txt",m_pMapViewWnd->game_map_stage);
	if(!(monfp = fopen(bf,"rt")))return;

	/*fscanf(monfp,"%d",&t);
	for(int p=0;p<t;p++)
	{
		fscanf(monfp,"%d",&temp);
	}
*/	
	fscanf(monfp,"%d",&t);
	
	for(int p=0;p<t;p++)
	{
		if(p == num)
		
		{
			//fscanf(monfp,"%d",&Mon[p].level);
			fscanf(monfp,"%d",&temp);//Mon[p].number);
			//fscanf(monfp,"%d",&Mon[p].AniNumber);
			fscanf(monfp,"%d",&Mon[p].speed);
			fscanf(monfp,"%d",&Mon[p].attrib);
			fscanf(monfp,"%d",&Mon[p].attack);
			fscanf(monfp,"%d",&Mon[p].mon_x);
			fscanf(monfp,"%d",&Mon[p].mon_y);
			fscanf(monfp,"%d",&Mon[p].str);
			fscanf(monfp,"%d",&Mon[p].dex);
			fscanf(monfp,"%d",&Mon[p].bonus);
			fscanf(monfp,"%d",&Mon[p].money);
			fscanf(monfp,"%d",&Mon[p].enugy);
			fscanf(monfp,"%d",&Mon[p].reset);

			Mon[p].mon_hit_txt_x =
				Mon[p].mon_hit_txt_y = 
					Mon[p].hitting = Mon[p].move_check = -1;
			Mon[p].move_sem   =  1;
			Mon[p].AniNumber = Mon[p]._COUNTER   =  0;
		
			Mon[p].mon_X      =  Mon[p].mon_x/8;
			Mon[p].mon_Y      =  Mon[p].mon_y/8;
			
			ZeroMemory(Mon[p].damagetxt,5);
			break;
		}else
		{
			//fscanf(monfp,"%d",&temp);
			fscanf(monfp,"%d",&temp);
			fscanf(monfp,"%d",&temp);
			fscanf(monfp,"%d",&temp);
			fscanf(monfp,"%d",&temp);
			fscanf(monfp,"%d",&temp);
			fscanf(monfp,"%d",&temp);
			fscanf(monfp,"%d",&temp);
			fscanf(monfp,"%d",&temp);
			fscanf(monfp,"%d",&temp);
			fscanf(monfp,"%d",&temp);
			fscanf(monfp,"%d",&temp);
			fscanf(monfp,"%d",&temp);
		}
	}
	fclose(monfp);
}

void 
cMonsterWnd::PutImage(int monX,int monY,int monA,int n)
{
	CUSTOM_VERTEX*	pV = NULL;
 
	if(Mon[n].move_check == 6)return;
	
	m_pDirect3D->GetDevice()->SetTexture( 0, 
									 Mon[n].m_pMonSprWnd->g_pSprTexture[Mon[n].m_pMonSprWnd->pAni[monA].pFrame[Mon[n].m_pMonSprWnd->pAni[monA].CurFrame].Sprite] );

	m_pMapViewWnd->g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

	for( int m=0; m < 4; m++ )
	{
      pV[m].vPos.z = 1.0f;
	  pV[m].rhw = 1.0f;
	  pV[m].dwDiffuse = 0xffffffff;
	}
		

	//m_pMonSprWnd[monN]->pAni[monA].CurFrame = Mon[n].CurFrame;
	float	fLeft   = (float)monX;//m_pMonSprWnd[monN]->pAni[monA].CurFrame
	float	fWidth  = (float)Mon[n].m_pMonSprWnd->w_Width[Mon[n].m_pMonSprWnd->pAni[monA].pFrame[Mon[n].m_pMonSprWnd->pAni[monA].CurFrame].Sprite];
	float	fTop    = (float)monY;
	float	fHeight = (float)Mon[n].m_pMonSprWnd->h_Height[Mon[n].m_pMonSprWnd->pAni[monA].pFrame[Mon[n].m_pMonSprWnd->pAni[monA].CurFrame].Sprite];

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
cMonsterWnd::Render()
{
	if(m_pStateMss->m_pEcffect)
		m_pStateMss->m_pEcffect->Render();
}

void 
cMonsterWnd::EmyMove(int userx,int usery,int n)
{	

	if(Mon[n].move_check != 6)
	{
		if(Mon[n].attack == 0 || Mon[n].attack == 3)	
		{
			if(Mon[n].move_check == -1)
			{
				Mon[n].move_check = rand()%3;
				if(Mon[n].move_check > 0)
				{
					Mon[n].move_sem   = rand()%15+5;
					Mon[n]._COUNTER   = 0;
				}
			}
		}else if(Mon[n].attack == 1 || Mon[n].attack == 4)
		{
			if(usery == Mon[n].mon_y)
			{
				if(userx > Mon[n].mon_x)
				{
					if(Mon[n].move_check == -1)
					{
						Mon[n].move_check = 2;
						Mon[n].move_sem   = 5;
						Mon[n]._COUNTER   = 0;
					}
				}else 
					if(userx < Mon[n].mon_x)
				{
					if(Mon[n].move_check == -1)
					{
						Mon[n].move_check = 1;
						Mon[n].move_sem   = 5;
						Mon[n]._COUNTER   = 0;
					}
				}else 
					if(userx == Mon[n].mon_x)
				{
					if(Mon[n].move_check == -1)
					{
						Mon[n].move_check = rand()%2+1;
						Mon[n].move_sem   = 5;
						Mon[n]._COUNTER   = 0;
					}
				}
			}else
			{
				if(Mon[n].move_check == -1)
				{
					Mon[n].move_check = rand()%3;
					if(Mon[n].move_check > 0)
					{
						Mon[n].move_sem   = rand()%15+5;
						Mon[n]._COUNTER   = 0;
					}
				}
			}
		}else if(Mon[n].attack == 2)
		{
			if(Mon[n].move_check != 1111 && Mon[n].AniNumber != MON_DIE)
			{
				Mon[n].move_check = -1;	

				if(userx < Mon[n].mon_x)
					Mon[n].move_check = 1;
				else if(userx > Mon[n].mon_x)
					Mon[n].move_check = 2;
			}
		}
	}
	//===============================================================================
	if(Mon[n].attrib == 0 || Mon[n].attrib == 4)
	{
		if(Mon[n].move_check == 1)
		{
			if(m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X-1,Mon[n].mon_Y) == 9)
			{
				Mon[n].mon_y+=Mon[n].speed;
				Mon[n].mon_Y = Mon[n].mon_y/8;//내리막
			}else if(m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X,Mon[n].mon_Y-1) == 10)
			{
				//Mon[n].mon_x-= 2;
				Mon[n].mon_y-=Mon[n].speed;
				Mon[n].mon_Y = Mon[n].mon_y/8;//오르막
			}else
				if(m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X-2,Mon[n].mon_Y) == 0 ||
					m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X-2,Mon[n].mon_Y-4) == 1 ||
					m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X-2,Mon[n].mon_Y-4) == 8 ||
					Mon[n].mon_X <= 0)
			{
				Mon[n].move_check = -1;
				Mon[n].move_sem   =  1;
			}
		}else if(Mon[n].move_check == 2)
		{	
			if(m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X,Mon[n].mon_Y-1) == 9)
			{
				//Mon[n].mon_x+= 2;
				Mon[n].mon_y-=Mon[n].speed;
				Mon[n].mon_Y = Mon[n].mon_y/8;//오르막
			}else if(m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X+1,Mon[n].mon_Y) == 10)
			{
				//Mon[n].mon_x+= 2;
				Mon[n].mon_y+=Mon[n].speed;
				Mon[n].mon_Y = Mon[n].mon_y/8;//내리막
			}
			else 
				if(m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X+2,Mon[n].mon_Y) == 0 ||
					m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X+2,Mon[n].mon_Y-4) == 1 ||
					m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X+2,Mon[n].mon_Y-4) == 8 ||
					Mon[n].mon_X >= ((m_pMapViewWnd->pMAP->WIDTH*4)-4))
			{
				Mon[n].move_check = -1;
				Mon[n].move_sem   =  1;
			}
		}
	}else
	if(Mon[n].attrib == 1)
	{
		if(Mon[n].move_check == 1)
		{
			if(m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X-2,Mon[n].mon_Y) >= 8/* &&
				m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X-3,Mon[n].mon_Y) <= 10*/ ||
					m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X-2,Mon[n].mon_Y) == 1 ||
						Mon[n].mon_X <= 0)
			{
				Mon[n].move_check = -1;
				Mon[n].move_sem   =  1;
			}
		}else if(Mon[n].move_check == 2)
		{	
			if(m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X+2,Mon[n].mon_Y) >= 8/* &&
				m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X+3,Mon[n].mon_Y) <= 10*/ || 
					m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X+2,Mon[n].mon_Y) == 1 ||
						Mon[n].mon_X >= ((m_pMapViewWnd->pMAP->WIDTH*4)-4))
			{
				Mon[n].move_check = -1;
				Mon[n].move_sem   =  1;
			}
		}
	}
	
//움직임처리
	if(Mon[n].hitting < 0)
	{
		if(Mon[n].move_check == 2)
		{
			if(Mon[n].attrib == 4)
			{
				if(usery <= Mon[n].mon_y+50 && usery >= Mon[n].mon_y-150
					&& userx >= Mon[n].mon_x && userx <= Mon[n].mon_x+300)
				{
					if(Mon[n].AniNumber != 6)
					{
						m_pDSound.Playing(m_psnd.mongun.bff, false);
						
						Mon[n]._COUNTER   = 0;
						Mon[n].AniNumber  = 6;
					}
				}else
				{
					if(m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X+2,Mon[n].mon_Y) != 0 ||
						m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X+2,Mon[n].mon_Y-4) != 1 ||
							m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X+2,Mon[n].mon_Y-4) != 8 ||
								Mon[n].mon_X < ((m_pMapViewWnd->pMAP->WIDTH*4)-4))
						{
							Mon[n].mon_x+= Mon[n].speed;
							Mon[n].mon_X = Mon[n].mon_x/8;
						}
				}
			}else
			if(Mon[n].attrib == 0)
			{
				if(m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X+2,Mon[n].mon_Y) != 0 ||
					m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X+2,Mon[n].mon_Y-4) != 1 ||
					m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X+2,Mon[n].mon_Y-4) != 8 ||
					Mon[n].mon_X < ((m_pMapViewWnd->pMAP->WIDTH*4)-4))
				{
					Mon[n].mon_x+= Mon[n].speed;
					Mon[n].mon_X = Mon[n].mon_x/8;
				}
			}else if(Mon[n].attrib == 1)
			{
				if(m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X+2,Mon[n].mon_Y) < 8 &&
					m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X+2,Mon[n].mon_Y) != 1 ||
						Mon[n].mon_X < ((m_pMapViewWnd->pMAP->WIDTH*4)-4))
				{
					Mon[n].mon_x+= Mon[n].speed;
					Mon[n].mon_X = Mon[n].mon_x/8;
				}
			}else if(Mon[n].attrib == 5)
			{	
				if(usery <= Mon[n].mon_y+160 && usery >= Mon[n].mon_y 
					&& userx >= Mon[n].mon_x && userx <= Mon[n].mon_x+160)
				{
					if(Mon[n].AniNumber != 6)
					{
						m_pDSound.Playing(m_psnd.mongun.bff, false);
						
						Mon[n]._COUNTER   = 0;
						Mon[n].AniNumber  = 6;
					}
				}else  {
					if(Mon[n].AniNumber != MON_STAND+1)
					{
						Mon[n]._COUNTER  = 0;
						Mon[n].AniNumber = MON_STAND+1;
					}
				}
			}else if(Mon[n].attrib == 6)
			{	
				if(usery <= Mon[n].mon_y+200 && usery >= Mon[n].mon_y-200
					&& userx >= Mon[n].mon_x && userx <= Mon[n].mon_x+400)
				{
					if(Mon[n].AniNumber != 6)
					{
						m_pDSound.Playing(m_psnd.mongun.bff, false);
						
						Mon[n]._COUNTER   = 0;
						Mon[n].AniNumber  = 6;
					}
				}else  {
					if(Mon[n].AniNumber != MON_STAND+1)
					{
						Mon[n]._COUNTER  = 0;
						Mon[n].AniNumber = MON_STAND+1;
					}
				}
			}else if(Mon[n].attrib == 7)
			{	
				if(usery <= Mon[n].mon_y && usery >= Mon[n].mon_y-50
					&& userx >= Mon[n].mon_x && userx <= Mon[n].mon_x+130)
				{
					if(Mon[n].AniNumber != 6)
					{
						m_pDSound.Playing(m_psnd.mongun.bff, false);
						
						Mon[n]._COUNTER   = 0;
						Mon[n].AniNumber  = 6;
					}
				}else  {
					if(Mon[n].AniNumber != MON_STAND+1)
					{
						
						if(Mon[n].m_pMonSprWnd->pAni[Mon[n].AniNumber].CurFrame
							>= Mon[n].m_pMonSprWnd->pAni[Mon[n].AniNumber].FrameNum-1)
						{
							Mon[n].AniNumber = MON_STAND+1;

							Mon[n]._COUNTER = 0;
						}
					}
				}
			}//점프하는 Monster처리...
			if(Mon[n].attack == 3 || Mon[n].attack == 4)
			{
				if(usery >= Mon[n].mon_y-100 && usery <= Mon[n].mon_y-30
					&& userx >= Mon[n].mon_x && userx <= Mon[n].mon_x+100)
				{
					if(Mon[n].AniNumber != 6)
					{
						Mon[n]._COUNTER  = 0;
                        Mon[n].AniNumber  = 6;
					}
				}else {
					if(Mon[n].AniNumber != MON_STAND+1)
					{
						Mon[n]._COUNTER  = 0;
						Mon[n].AniNumber = MON_STAND+1;
					}
				}
			}
			
			if(Mon[n].AniNumber != MON_STAND+1 && Mon[n].AniNumber != 6)
			{
				Mon[n]._COUNTER  = 0;
				Mon[n].AniNumber = MON_STAND+1;
			}

			Mon[n].move_sem--;
		}else 
			if(Mon[n].move_check == 1)
		{
			if(Mon[n].attrib == 4)
			{
				if(usery <= Mon[n].mon_y+50 && usery >= Mon[n].mon_y-150
					&& userx <= Mon[n].mon_x && userx >= Mon[n].mon_x-300)
				{
					if(Mon[n].AniNumber != 5)
					{
						m_pDSound.Playing(m_psnd.mongun.bff, false);
						
						Mon[n]._COUNTER   = 0;
						Mon[n].AniNumber  = 5;
					}
				}else
				{
					if(m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X-2,Mon[n].mon_Y) != 0 ||
						m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X-2,Mon[n].mon_Y-4) != 1 ||
							m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X-2,Mon[n].mon_Y-4) != 8 ||
								Mon[n].mon_X > 0)
						{
							Mon[n].mon_x-= Mon[n].speed;
							Mon[n].mon_X = Mon[n].mon_x/8;
						}
				}
			}else
			if(Mon[n].attrib == 0)
			{
				if(m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X-2,Mon[n].mon_Y) != 0 ||
					m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X-2,Mon[n].mon_Y-4) != 1 ||
					m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X-2,Mon[n].mon_Y-4) != 8 ||
					Mon[n].mon_X > 0)
				{
					Mon[n].mon_x-= Mon[n].speed;
					Mon[n].mon_X = Mon[n].mon_x/8;
				}
			}else if(Mon[n].attrib == 1)
			{
				if(m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X-2,Mon[n].mon_Y) < 8 &&
					m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X-2,Mon[n].mon_Y) != 1 ||
						Mon[n].mon_X > 0)
				{
					Mon[n].mon_x-= Mon[n].speed;
					Mon[n].mon_X = Mon[n].mon_x/8;
				}
			}else if(Mon[n].attrib == 5)
			{	
				if(usery <= Mon[n].mon_y+160 && usery >= Mon[n].mon_y
					&& userx <= Mon[n].mon_x && userx >= Mon[n].mon_x-160)
				{
					if(Mon[n].AniNumber != 5)
					{
						m_pDSound.Playing(m_psnd.mongun.bff, false);
						
						Mon[n]._COUNTER   = 0;
						Mon[n].AniNumber  = 5;
					}
				}else {
					if(Mon[n].AniNumber != MON_STAND)
					{
						Mon[n]._COUNTER   = 0;
						Mon[n].AniNumber = MON_STAND;
					}
				}
			}else if(Mon[n].attrib == 6)
			{
				if(usery <= Mon[n].mon_y+200 && usery >= Mon[n].mon_y-200
					&& userx <= Mon[n].mon_x && userx >= Mon[n].mon_x-400)
				{
					if(Mon[n].AniNumber != 5)
					{
						m_pDSound.Playing(m_psnd.mongun.bff, false);
						
						Mon[n]._COUNTER   = 0;
						Mon[n].AniNumber  = 5;
					}
				}else {
					if(Mon[n].AniNumber != MON_STAND)
					{
						Mon[n]._COUNTER   = 0;
						Mon[n].AniNumber = MON_STAND;
					}
				}
			}else if(Mon[n].attrib == 7)
			{	
				if(usery <= Mon[n].mon_y && usery >= Mon[n].mon_y-50
					&& userx <= Mon[n].mon_x && userx >= Mon[n].mon_x-130)
				{
					if(Mon[n].AniNumber != 5)
					{
						m_pDSound.Playing(m_psnd.mongun.bff, false);
						
						Mon[n]._COUNTER   = 0;
						Mon[n].AniNumber  = 5;
					}
				}else  {
					if(Mon[n].AniNumber != MON_STAND)
					{
						
						if(Mon[n].m_pMonSprWnd->pAni[Mon[n].AniNumber].CurFrame
							>= Mon[n].m_pMonSprWnd->pAni[Mon[n].AniNumber].FrameNum-1)
						{
							Mon[n].AniNumber = MON_STAND+1;

							Mon[n]._COUNTER = 0;
						}
					}
				}
			}
			//점프하는 Monster처리...
			if(Mon[n].attack == 3 || Mon[n].attack == 4)
			{
				if(usery >= Mon[n].mon_y-100 && usery <= Mon[n].mon_y-30 
					&& userx <= Mon[n].mon_x && userx >= Mon[n].mon_x-100)
				{
					if(Mon[n].AniNumber != 5)
					{
						Mon[n]._COUNTER   = 0;
						Mon[n].AniNumber  = 5;
					}
				}else{
					if(Mon[n].AniNumber != MON_STAND)
					{
						Mon[n]._COUNTER   = 0;
						Mon[n].AniNumber = MON_STAND;
					}
				}
			}
			
			if(Mon[n].AniNumber != MON_STAND && Mon[n].AniNumber != 5)
			{
				Mon[n]._COUNTER   = 0;
				Mon[n].AniNumber = MON_STAND;
			}
			
			Mon[n].move_sem--;
		}
		
		//Mon[n].move_sem--;
		//Mon[n]._COUNTER++;
	}
	else if(Mon[n].hitting > 0)
	{
		Mon[n].mon_hit_txt_y-=10;
	
		m_pD3dxfont->DrawText(Mon[n].mon_hit_txt_x,Mon[n].mon_hit_txt_y,0,0,
			D3DCOLOR_XRGB(255, 0, 0),Mon[n].damagetxt,strlen(Mon[n].damagetxt));

		if(Mon[n].move_check == 1)
		{
			if(Mon[n].attrib == 0 || Mon[n].attrib == 4)
			{
				Mon[n].mon_x-= Mon[n].speed;
				Mon[n].mon_X = Mon[n].mon_x/8;
				
				if(m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X-2,Mon[n].mon_Y) == 0 ||
					m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X-2,Mon[n].mon_Y-4) == 1 ||
						m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X-2,Mon[n].mon_Y-4) >= 8 ||
							Mon[n].mon_X <= 0)
				{
					Mon[n].mon_x+= Mon[n].speed;
					Mon[n].mon_X = Mon[n].mon_x/8;
				}
			}else if(Mon[n].attrib == 1)
			{
				Mon[n].mon_x-= Mon[n].speed;
				Mon[n].mon_X = Mon[n].mon_x/8;
				
				if(m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X-2,Mon[n].mon_Y) >= 8 &&
					m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X-2,Mon[n].mon_Y) == 1 ||
						Mon[n].mon_X <= 0)
				{
					Mon[n].mon_x+= Mon[n].speed;
					Mon[n].mon_X = Mon[n].mon_x/8;
				}
			}else
			{
				Mon[n].mon_x-=Mon[n].speed;
				Mon[n].mon_X = Mon[n].mon_x/8;
			}

			if(Mon[n].AniNumber != MON_HIT+1)
				Mon[n].AniNumber = MON_HIT+1;
		}else if(Mon[n].move_check == 2)
		{	
			if(Mon[n].attrib == 0 || Mon[n].attrib == 4)
			{
				Mon[n].mon_x+= Mon[n].speed;
				Mon[n].mon_X = Mon[n].mon_x/8;
				
				if(m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X+2,Mon[n].mon_Y) == 0 ||
					m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X+2,Mon[n].mon_Y-4) == 1 ||
						m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X+1,Mon[n].mon_Y-4) >= 8 ||
							Mon[n].mon_X >= ((m_pMapViewWnd->pMAP->WIDTH*4)-4))
				{
					Mon[n].mon_x-= Mon[n].speed;
					Mon[n].mon_X = Mon[n].mon_x/8;
				}
			}else if(Mon[n].attrib == 1)
			{
				Mon[n].mon_x+= Mon[n].speed;
				Mon[n].mon_X = Mon[n].mon_x/8;
				
				if(m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X+2,Mon[n].mon_Y) >= 8 &&
					m_pMapViewWnd->pMAP->GetAttr(Mon[n].mon_X+2,Mon[n].mon_Y) == 1 ||
						Mon[n].mon_X >= ((m_pMapViewWnd->pMAP->WIDTH*4)-4))
				{
					Mon[n].mon_x-= Mon[n].speed;
					Mon[n].mon_X = Mon[n].mon_x/8;
				}
			}else
			{
				Mon[n].mon_x+=Mon[n].speed;
				Mon[n].mon_X = Mon[n].mon_x/8;
			}	
			if(Mon[n].AniNumber != MON_HIT)
				Mon[n].AniNumber = MON_HIT;
		}
		Mon[n].hitting--;
		if(Mon[n].hitting <= 0)
		{
			Mon[n].hitting  = -1;
			Mon[n].move_sem = 1;
			Mon[n].move_check=3-Mon[n].move_check;
		}
	}

	if(Mon[n].attrib < 5)
	{
		if(Mon[n].move_sem <= 0 || Mon[n].move_check == 0)
		{
			Mon[n].move_check = -1;
			Mon[n].move_sem   =  1;
		}
	}

	Mon[n]._COUNTER++;
	Mon[n].m_pMonSprWnd->pAni[Mon[n].AniNumber].CurFrame=(Mon[n]._COUNTER/Mon[n].m_pMonSprWnd->pAni[Mon[n].AniNumber].Delay)%Mon[n].m_pMonSprWnd->pAni[Mon[n].AniNumber].FrameNum;

    if(Mon[n].attrib >= 4 && Mon[n].AniNumber > 4)
	{
		if(Mon[n].m_pMonSprWnd->pAni[Mon[n].AniNumber].CurFrame == 2)
		{
			if(Mon[n].move_check == 1)
			{
				Mon[n].move_check = 1111;
				if(Mon[n].attrib == 5)
					m_pMissail->CreateMissail(Mon[n].mon_x-20,Mon[n].mon_y,3,2,1,1,n);
				else if(Mon[n].attrib == 6)
					m_pMissail->CreateMissail(Mon[n].mon_x-20,Mon[n].mon_y-20,7,0,1,1,n);
				else if(Mon[n].attrib == 4)
					m_pMissail->CreateMissail(Mon[n].mon_x-20,Mon[n].mon_y-20,8,0,1,1,n);
			}
			else if(Mon[n].move_check == 2)
			{
				Mon[n].move_check = 1111;
				if(Mon[n].attrib == 5)
					m_pMissail->CreateMissail(Mon[n].mon_x+20,Mon[n].mon_y,4,2,0,1,n);
				else if(Mon[n].attrib == 6)
					m_pMissail->CreateMissail(Mon[n].mon_x+20,Mon[n].mon_y-20,7,0,0,1,n);
				else if(Mon[n].attrib == 4)
					m_pMissail->CreateMissail(Mon[n].mon_x-20,Mon[n].mon_y-20,9,0,0,1,n);
			}
		}else if(Mon[n].m_pMonSprWnd->pAni[Mon[n].AniNumber].CurFrame >= Mon[n].m_pMonSprWnd->pAni[Mon[n].AniNumber].FrameNum-1)
			Mon[n].move_check = -1;
		
	}

	if(Mon[n].AniNumber == MON_DIE)
	{
		if(Mon[n].m_pMonSprWnd->pAni[Mon[n].AniNumber].CurFrame
			>= Mon[n].m_pMonSprWnd->pAni[Mon[n].AniNumber].FrameNum-1)
			{
				Mon[n].AniNumber  = MON_STAND;
				Mon[n].move_check = 6;
				Mon[n].hitting    = 0;
			}
	
			//Mon[n]._COUNTER++;
	}
	if(Mon[n].move_check == 6)
	{
		Mon[n].reset--;
		if(Mon[n].reset <= 0)
			ResetMonster(n);
	}

}
int
cMonsterWnd::HitMonster(RECT UserXY)
{
	for(int p=0;p<mondatATotalnum;p++)
	{
		if(Mon[p].hitting < 6 && Mon[p].move_check != 5 && Mon[p].move_check != 6)
		{
			int animations = Mon[p].AniNumber;
			int fr         = Mon[p].m_pMonSprWnd->pAni[animations].pFrame[Mon[p].m_pMonSprWnd->pAni[animations].CurFrame].Sprite;
        
			int n_x        = Mon[p].mon_x + Mon[p].m_pMonSprWnd->pAni[animations].pFrame[Mon[p].m_pMonSprWnd->pAni[animations].CurFrame].XPos
									- m_pMapViewWnd->map_x;
			int	n_y        = Mon[p].mon_y + Mon[p].m_pMonSprWnd->pAni[animations].pFrame[Mon[p].m_pMonSprWnd->pAni[animations].CurFrame].YPos
								- m_pMapViewWnd->map_y;
		
			if(Mon[p].m_pMonSprWnd->hitbox[fr] == 2)
			{
				if(UserXY.right >= n_x+Mon[p].m_pMonSprWnd->CrashBox[fr][1].left && UserXY.bottom >= n_y+Mon[p].m_pMonSprWnd->CrashBox[fr][1].top
					&& UserXY.left <= n_x+Mon[p].m_pMonSprWnd->CrashBox[fr][1].right && UserXY.top <= n_y+Mon[p].m_pMonSprWnd->CrashBox[fr][1].bottom)
				{
					return p;
				}
			}
		}	
	}
	return -11;
}
int
cMonsterWnd::AttackMonster(RECT UserXY,int view,int str)
{
	for(int p=0;p<mondatATotalnum;p++)
	{
		if(Mon[p].hitting < 6 && Mon[p].move_check != 5 && Mon[p].move_check != 6)
		{
			int animations = Mon[p].AniNumber;
			int fr         = Mon[p].m_pMonSprWnd->pAni[animations].pFrame[Mon[p].m_pMonSprWnd->pAni[animations].CurFrame].Sprite;
			
			int n_x        = Mon[p].mon_x + Mon[p].m_pMonSprWnd->pAni[animations].pFrame[Mon[p].m_pMonSprWnd->pAni[animations].CurFrame].XPos
								- m_pMapViewWnd->map_x;
			int	n_y        = Mon[p].mon_y + Mon[p].m_pMonSprWnd->pAni[animations].pFrame[Mon[p].m_pMonSprWnd->pAni[animations].CurFrame].YPos
							- m_pMapViewWnd->map_y;
		
			if(UserXY.right >= n_x+Mon[p].m_pMonSprWnd->CrashBox[fr][0].left && UserXY.bottom >= n_y+Mon[p].m_pMonSprWnd->CrashBox[fr][0].top
					&& UserXY.left <= n_x+Mon[p].m_pMonSprWnd->CrashBox[fr][0].right && UserXY.top <= n_y+Mon[p].m_pMonSprWnd->CrashBox[fr][0].bottom)
				{
					m_pDSound.Playing(m_psnd.hitmon.bff, false);
					//공격자의 최종 최소 공격력 - 공격 대상의 방어력/2 * 아머 타입별 데미지 적용율) ~ 
					//(공격자 최종 최대 공격력 - 공격 대상의 방어력/2 * 아머 타입별 데미지 적용율) 
					int damage = (str-Mon[p].dex)+rand()%3;
					if(damage <= 0)
						return -13;

					if(Mon[p].attrib < 5)
					{
						if(Mon[p].attack == 3 || Mon[p].attack == 4)
							Mon[p].attack = 4;
						else Mon[p].attack = 1;
					}
					sprintf(Mon[p].damagetxt,"%d",damage);
					Mon[p].mon_hit_txt_x = Mon[p].mon_x-m_pMapViewWnd->map_x;
					Mon[p].mon_hit_txt_y = n_y;//-m_pMapViewWnd->map_y;

					m_pStateMss->m_pEcffect->CreateHit(Mon[p].mon_x,Mon[p].mon_y,0);
										
					Mon[p].enugy-=damage;
					
					Mon[p].hitting   = 6;
					Mon[p].move_sem  = 2;

					if(view == 0)
						Mon[p].move_check = 2;
					else if(view == 1)
						Mon[p].move_check = 1;
						
					Mon[p]._COUNTER  = 0;

					//Monster 저세상.
					if(Mon[p].enugy <= 0){
						m_pDSound.Playing(m_psnd.mondie.bff, false);
						char sr[40];
						
						Mon[p].hitting    = -1;
						Mon[p].move_check =  5;
						//Mon[p].move_sem   =  3;
						Mon[p].AniNumber  =  MON_DIE;
						
						int Alu = 0;
						if(Mon[p].money >= 100)Alu = 1;

						m_pStateMss->m_pEcffect->CreateMoney(Mon[p].mon_x,Mon[p].mon_y,Alu,Mon[p].money);
						sprintf(sr,"경험치를 얻었습니다(+%d)",Mon[p].bonus);
						m_pHelp->CreateHelp(sr);

						return Mon[p].bonus;
					}
					else return -12;
				}
		}
	}
	return -11;
}

int
cMonsterWnd::HitItem(RECT UserXY)
{
	return m_pStateMss->m_pEcffect->ItemChk(UserXY);
}