#include "MapOutput.h"
#include "Help.h"
#include "StatusForm.h"
#include "DirectSound.h"
#include "Missail.h"

cMissailWnd      *m_pMissail = NULL;

cMissailWnd::cMissailWnd()
{
	m_pDirect3D      = NULL;
	m_pMissailSprWnd = NULL;
	Missail_Buffer   = NULL; 
}

cMissailWnd::~cMissailWnd()
{
	SAFE_DELETE_(m_pMissailSprWnd);
	SAFE_DELETE_(Missail_Buffer);
}

void 
cMissailWnd::Init(CDirect3D *D3D,cMonsterWnd *Mon,cBossWnd *bb,cSprWnd *spr)
{
	m_pDirect3D = D3D;
	
	m_pMon  = Mon;
	m_pBss  = bb;
	m_pUser = spr;

	m_pMissailSprWnd = new cSprDataWnd();
	m_pMissailSprWnd->Init(m_pDirect3D);
	m_pMissailSprWnd->SprLoad(1003);
	m_pMissailSprWnd->LoadSprTexture();
}

void
cMissailWnd::CreateMissail(int x,int y,int ani,int type,int view,int cc,int nn)
{
	Missail *pNewMissail    = new Missail;
	
	pNewMissail->msl_x      = x;
	pNewMissail->msl_y      = y;
	pNewMissail->map_msl_x  = x/8;
	pNewMissail->map_msl_y  = y/8;
	pNewMissail->aninum     = ani;
	pNewMissail->type       = type;
	pNewMissail->view       = view;
	pNewMissail->who        = cc;
	pNewMissail->monnum     = nn;

	pNewMissail->count = pNewMissail->curframe = 0;
	
	if(cc == 0)
		m_pUser->mis++;

	AddMissail(Missail_Buffer, pNewMissail);
}


void 
cMissailWnd::AddMissail(Missail *&pMissail, Missail *pAdd)
{
	pAdd->pNext = pMissail;
	pMissail = pAdd;
}

void
cMissailWnd::DeleteMissail(Missail *&pMissail, Missail *pDelMissail)
{
	if(pMissail == pDelMissail)
	{
		pMissail = pMissail->pNext;
	}
	else
	{
		Missail *pTmp = pMissail;

		while(pTmp->pNext)
		{
			if(pTmp->pNext == pDelMissail)
			{
				pTmp->pNext = pDelMissail->pNext;
				break;
			}

			pTmp = pTmp->pNext;
		}
	}

	SAFE_DELETE_(pDelMissail);
}
void 
cMissailWnd::AllMissailDelete()
{	
	Missail *pMissail    = NULL;
	Missail *pDelMissail = NULL;

	pMissail =  Missail_Buffer;
	while(pMissail)
	{
		pDelMissail = pMissail;
		pMissail = pMissail->pNext;
		DeleteMissail(Missail_Buffer, pDelMissail);
	}	
}
void 
cMissailWnd::PutImage(int X,int Y,int A,int C)
{
	CUSTOM_VERTEX*	pV = NULL;
 
	m_pDirect3D->GetDevice()->SetTexture( 0, 
									 m_pMissailSprWnd->g_pSprTexture[m_pMissailSprWnd->pAni[A].pFrame[C].Sprite] );

	m_pMapViewWnd->g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

	for( int m=0; m < 4; m++ )
	{
      pV[m].vPos.z = 1.0f;
	  pV[m].rhw = 1.0f;
	  pV[m].dwDiffuse = 0xffffffff;
	}
		

	float	fLeft   = (float)X;
	float	fWidth  = (float)m_pMissailSprWnd->w_Width[m_pMissailSprWnd->pAni[A].pFrame[C].Sprite];
	float	fTop    = (float)Y;
	float	fHeight = (float)m_pMissailSprWnd->h_Height[m_pMissailSprWnd->pAni[A].pFrame[C].Sprite];

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
int
cMissailWnd::HitChk(RECT ro)
{	
	Missail *pMissail    = NULL;
	Missail *pDelMissail = NULL;
	
	pMissail = Missail_Buffer;
	while(pMissail)
	{
		int XX,YY;
		int ck = 0;
		RECT rc;

		XX = pMissail->msl_x+m_pMissailSprWnd->pAni[pMissail->aninum].pFrame[pMissail->curframe].XPos
								- m_pMapViewWnd->map_x;
		YY = pMissail->msl_y+m_pMissailSprWnd->pAni[pMissail->aninum].pFrame[pMissail->curframe].YPos
								- m_pMapViewWnd->map_y;

		rc.left   = XX + m_pMissailSprWnd->CrashBox[m_pMissailSprWnd->pAni[pMissail->aninum].pFrame[pMissail->curframe].Sprite][0].left;
		rc.right  = XX + m_pMissailSprWnd->CrashBox[m_pMissailSprWnd->pAni[pMissail->aninum].pFrame[pMissail->curframe].Sprite][0].right;
		rc.top    = YY + m_pMissailSprWnd->CrashBox[m_pMissailSprWnd->pAni[pMissail->aninum].pFrame[pMissail->curframe].Sprite][0].top;
		rc.bottom = YY + m_pMissailSprWnd->CrashBox[m_pMissailSprWnd->pAni[pMissail->aninum].pFrame[pMissail->curframe].Sprite][0].bottom;
				
		if(pMissail->who == 0)
		{
			if(pMissail->monnum == 0)
			{
				if(m_pMon->mondatATotalnum > 0)
				{
					ck = m_pMon->AttackMonster(rc,pMissail->view,
						(m_pUser->Str/3)+A_Item[m_pUser->Interface_[0].checked].att);
				}else if(m_pBss)
				{
					ck = m_pBss->AttackBoss(rc,pMissail->view,
						(m_pUser->Str/3)+A_Item[m_pUser->Interface_[0].checked].att);
				}
			}else if(pMissail->monnum == 1)
			{
				if(m_pMon->mondatATotalnum > 0)
				{
					ck = m_pMon->AttackMonster(rc,pMissail->view,
						m_pUser->Int+S_Item[m_pUser->Interface_[1].checked].use);
				}else if(m_pBss)
				{
					ck = m_pBss->AttackBoss(rc,pMissail->view,
						m_pUser->Int+S_Item[m_pUser->Interface_[1].checked].use);
				}
			}
			
			if(ck == -12 || ck == -13)
			{
				if(pMissail->aninum!=2)
				{
					m_pUser->mis=0;
					
					pDelMissail = pMissail;
					pMissail = pMissail->pNext;
					DeleteMissail(Missail_Buffer, pDelMissail);
					continue;
				}
			}else
				if(ck > 0)
				{
					m_pUser->Kyunghum+=ck;
					if(m_pUser->Kyunghum >= m_pUser->totalKyunghum)
					{
						if(m_pUser->Level <= 150)
						{
							m_pDSound.Playing(m_psnd.lvup.bff, false);
							m_pStateMss->m_pEcffect->CreateHit(m_pUser->spr_x,m_pUser->spr_y,1);
							char sr[30];
							sprintf(sr,"레벨업하였습니다(%d->%d).",m_pUser->Level,m_pUser->Level+1);
							m_pHelp->CreateHelp(sr);
							sprintf(sr,"축하합니다.");
							m_pHelp->CreateHelp(sr);

							m_pUser->Kyunghum      = 0;
							m_pUser->Level          ++;
				
							m_pUser->totalHp+=6;
							m_pUser->totalMp+=3;
							m_pUser->Hp = m_pUser->totalHp;
							m_pUser->Mp = m_pUser->totalMp;
				
							m_pUser->stackPoint+=5;
				
							int l = m_pUser->Level+1;
							m_pUser->totalKyunghum = (int)(pow((float)m_pUser->Level,2) * (pow((float)l,2)-13*l+82));
						}
					}
					
					if(pMissail->aninum!=2)
					{
						m_pUser->mis=0;
						
						pDelMissail = pMissail;
						pMissail = pMissail->pNext;
						DeleteMissail(Missail_Buffer, pDelMissail);
						continue;
					}
				}
		}else if(pMissail->who == 1)
		{
			//int ck = m_pMon->HitMonster(rc);
			
			if(rc.right >= ro.left && rc.bottom >= ro.top
					&& rc.left <= ro.right && rc.top <= ro.bottom)
			{
				int ck = pMissail->monnum;

				pDelMissail = pMissail;
				pMissail = pMissail->pNext;
				DeleteMissail(Missail_Buffer, pDelMissail);
			
				return ck;
			}
		}
		pMissail = pMissail->pNext;
	}
return -11;
}
void 
cMissailWnd::Render()
{
	Missail *pMissail    = NULL;
	Missail *pDelMissail = NULL;
	int XX,YY;

	pMissail = Missail_Buffer;
	while(pMissail)
	{
		pMissail->count++;
		if(pMissail->type == 0)//앞으로 곧장 나아가는 미사일
		{
			if(pMissail->view == 0)
				pMissail->msl_x+=32;
			else if(pMissail->view == 1)
				pMissail->msl_x-=32;
						
			pMissail->map_msl_x=pMissail->msl_x/8;

			pMissail->curframe
				= (pMissail->count/m_pMissailSprWnd->pAni[pMissail->aninum].Delay)%m_pMissailSprWnd->pAni[pMissail->aninum].FrameNum;
		
			XX = pMissail->msl_x+m_pMissailSprWnd->pAni[pMissail->aninum].pFrame[ pMissail->curframe].XPos
									- m_pMapViewWnd->map_x;
			YY = pMissail->msl_y+m_pMissailSprWnd->pAni[pMissail->aninum].pFrame[ pMissail->curframe].YPos
									- m_pMapViewWnd->map_y;

			PutImage(XX,YY,pMissail->aninum,pMissail->curframe);
			if(pMissail->who == 0 && pMissail->monnum != 1)
			{
				if(m_pMapViewWnd->pMAP->GetAttr(pMissail->map_msl_x,pMissail->map_msl_y) != 0)
				{
					m_pUser->mis=0;
				
					pDelMissail = pMissail;
					pMissail = pMissail->pNext;
					DeleteMissail(Missail_Buffer, pDelMissail);
					continue;
				}
			}
			if(XX <= 0 || XX >= 800 ||
				YY <= 0 || YY >= 600)
			{
				if(pMissail->who == 0)m_pUser->mis=0;
				
				pDelMissail = pMissail;
				pMissail = pMissail->pNext;
				DeleteMissail(Missail_Buffer, pDelMissail);
				continue;
			}
		}else
			if(pMissail->type == 1)//사용자 미사일(부메랑)
		{
			if(pMissail->count < 10)
			{
				if(pMissail->view == 0)
					pMissail->msl_x+=32;
				else if(pMissail->view == 1)
					pMissail->msl_x-=32;
			}else
			{
				if(pMissail->msl_x > m_pUser->spr_x)
					pMissail->msl_x-=32;
				else if(pMissail->msl_x < m_pUser->spr_x)
					pMissail->msl_x+=32;

				if(pMissail->msl_y > (m_pUser->spr_y-42))
					pMissail->msl_y-=32;
				else if(pMissail->msl_y < (m_pUser->spr_y-42))
					pMissail->msl_y+=32;
			}

			pMissail->curframe
				= (pMissail->count/m_pMissailSprWnd->pAni[pMissail->aninum].Delay)%m_pMissailSprWnd->pAni[pMissail->aninum].FrameNum;
		
			XX = pMissail->msl_x+m_pMissailSprWnd->pAni[pMissail->aninum].pFrame[ pMissail->curframe].XPos
									- m_pMapViewWnd->map_x;
			YY = pMissail->msl_y+m_pMissailSprWnd->pAni[pMissail->aninum].pFrame[ pMissail->curframe].YPos
									- m_pMapViewWnd->map_y;

			PutImage(XX,YY,pMissail->aninum,pMissail->curframe);
	//		Sleep(200);
			if(pMissail->msl_x <= m_pUser->spr_x && pMissail->msl_x >= (m_pUser->spr_x-50))
           //     && pMissail->msl_y <= m_pUser->spr_y && pMissail->msl_y >= (m_pUser->spr_y+50))
			{
				m_pUser->mis=0;
				
				pDelMissail = pMissail;
				pMissail = pMissail->pNext;
				DeleteMissail(Missail_Buffer, pDelMissail);
				continue;
			}
		}else 
		if(pMissail->type == 2)//대각선 미사일
		{
			if(pMissail->view == 0)
				pMissail->msl_x+=16;
			else if(pMissail->view == 1)
				pMissail->msl_x-=16;
			
			pMissail->msl_y+=16;

			pMissail->map_msl_x=pMissail->msl_x/8;
			pMissail->map_msl_y=pMissail->msl_y/8;

			pMissail->curframe
				= (pMissail->count/m_pMissailSprWnd->pAni[pMissail->aninum].Delay)%m_pMissailSprWnd->pAni[pMissail->aninum].FrameNum;
		
			XX = pMissail->msl_x+m_pMissailSprWnd->pAni[pMissail->aninum].pFrame[pMissail->curframe].XPos
									- m_pMapViewWnd->map_x;
			YY = pMissail->msl_y+m_pMissailSprWnd->pAni[pMissail->aninum].pFrame[pMissail->curframe].YPos
									- m_pMapViewWnd->map_y;

			PutImage(XX,YY,pMissail->aninum,pMissail->curframe);
			if(m_pMapViewWnd->pMAP->GetAttr(pMissail->map_msl_x,pMissail->map_msl_y) != 0)
			{
				pDelMissail = pMissail;
				pMissail = pMissail->pNext;
				DeleteMissail(Missail_Buffer, pDelMissail);
				continue;
			}
			if(XX <= 0 || XX >= 800 ||
				YY <= 0 || YY >= 600)
			{
				pDelMissail = pMissail;
				pMissail = pMissail->pNext;
				DeleteMissail(Missail_Buffer, pDelMissail);
				continue;
			}
		}
		pMissail = pMissail->pNext;
	}
}
