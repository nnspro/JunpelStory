#include "MapOutput.h"
#include "Help.h"
#include "StatusForm.h"
#include "DirectSound.h"
#include "Effect.h"

cEcffectWnd::cEcffectWnd()
{
	m_pDirect3D              = NULL;
	for(int p=0;p<2;p++)
		m_pEcffectSprWnd[p] = NULL;

	Ecffect_Buffer = Ecffect_Money = Ecffect_Item = NULL; 
}

cEcffectWnd::~cEcffectWnd()
{
	for(int p=0;p<2;p++)
		SAFE_DELETE_(m_pEcffectSprWnd[p]);

	SAFE_DELETE_(Ecffect_Buffer);
	SAFE_DELETE_(Ecffect_Money);
	SAFE_DELETE_(Ecffect_Item);
}

void 
cEcffectWnd::Init(CDirect3D   *D3D)
{
	m_pDirect3D = D3D;
	
	m_pEcffectSprWnd[0] = new cSprDataWnd();
	m_pEcffectSprWnd[0]->Init(m_pDirect3D);
	m_pEcffectSprWnd[0]->SprLoad(1001);
	m_pEcffectSprWnd[0]->LoadSprTexture();

	m_pEcffectSprWnd[1] = new cSprDataWnd();
	m_pEcffectSprWnd[1]->Init(m_pDirect3D);
	m_pEcffectSprWnd[1]->SprLoad(1002);
	m_pEcffectSprWnd[1]->LoadSprTexture();
	
	Ecffect_Temp[0].count  = 0;
	Ecffect_Temp[0].aninum = 0;
	
	Ecffect_Temp[1].count  = 0;
	Ecffect_Temp[1].aninum = 0;
}

void
cEcffectWnd::CreateHit(int x,int y,int ani)
{
	Ecffect *pNewEcffect = new Ecffect;
	memcpy(pNewEcffect,&Ecffect_Temp[0],sizeof(Ecffect));
	pNewEcffect->eff_x  = x;
	pNewEcffect->eff_y  = y;
	pNewEcffect->aninum = ani;
	AddEcffect(Ecffect_Buffer, pNewEcffect);
}

void
cEcffectWnd::CreateMoney(int x,int y,int ani,int hm)
{
	Ecffect *pNewEcffect = new Ecffect;
	memcpy(pNewEcffect,&Ecffect_Temp[1],sizeof(Ecffect));
	pNewEcffect->eff_x    = x;
	pNewEcffect->eff_y    = y;
	pNewEcffect->map_eff_x= x/8;
	pNewEcffect->map_eff_y= y/8;
	pNewEcffect->aninum   = ani;
	pNewEcffect->how_much = hm;
	AddEcffect(Ecffect_Money, pNewEcffect);
}
void
cEcffectWnd::CreateItem(int x,int y,int ani,int hm,int cc,int vv)
{
	Ecffect *pNewEcffect = new Ecffect;
	memcpy(pNewEcffect,&Ecffect_Temp[1],sizeof(Ecffect));
	pNewEcffect->eff_x    = x;
	pNewEcffect->eff_y    = y;
	pNewEcffect->map_eff_x= x/8;
	pNewEcffect->map_eff_y= y/8;
	pNewEcffect->aninum   = ani;
	pNewEcffect->count    = hm;
	pNewEcffect->speed    = cc;
	pNewEcffect->how_much = vv;
	AddEcffect(Ecffect_Item, pNewEcffect);
}
void 
cEcffectWnd::AddEcffect(Ecffect *&pEcffect, Ecffect *pAdd)
{
	pAdd->pNext = pEcffect;
	pEcffect = pAdd;
}

void
cEcffectWnd::DeleteEcffect(Ecffect *&pEcffect, Ecffect *pDelEcffect)
{
	if(pEcffect == pDelEcffect)
	{
		pEcffect = pEcffect->pNext;
	}
	else
	{
		Ecffect *pTmp = pEcffect;

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
}
void 
cEcffectWnd::AllEffectDelete()
{	
	Ecffect *pEcffect    = NULL;
	Ecffect *pDelEcffect = NULL;

	pEcffect = Ecffect_Money;
	while(pEcffect)
	{
		pDelEcffect = pEcffect;
		pEcffect = pEcffect->pNext;
		DeleteEcffect(Ecffect_Money, pDelEcffect);
			
	//	pEcffect = pEcffect->pNext;
	}
	
	pEcffect = Ecffect_Item;
	while(pEcffect)
	{
		pDelEcffect = pEcffect;
		pEcffect = pEcffect->pNext;
		DeleteEcffect(Ecffect_Item, pDelEcffect);
			
	//	pEcffect = pEcffect->pNext;
	}
}
int 
cEcffectWnd::ItemChk(RECT UserXY)
{
	Ecffect *pEcffect    = NULL;
	Ecffect *pDelEcffect = NULL;

	pEcffect = Ecffect_Money;
	while(pEcffect)
	{
		int fr  = m_pEcffectSprWnd[1]->pAni[pEcffect->aninum].pFrame[m_pEcffectSprWnd[1]->pAni[pEcffect->aninum].CurFrame].Sprite;
		int n_x = pEcffect->eff_x + m_pEcffectSprWnd[1]->pAni[pEcffect->aninum].pFrame[m_pEcffectSprWnd[1]->pAni[pEcffect->aninum].CurFrame].XPos
							- m_pMapViewWnd->map_x;
		int	n_y = pEcffect->eff_y + m_pEcffectSprWnd[1]->pAni[pEcffect->aninum].pFrame[m_pEcffectSprWnd[1]->pAni[pEcffect->aninum].CurFrame].YPos
							- m_pMapViewWnd->map_y;

		if(UserXY.right >= n_x+m_pEcffectSprWnd[1]->CrashBox[fr][0].left && UserXY.bottom >= n_y+m_pEcffectSprWnd[1]->CrashBox[fr][0].top
			&& UserXY.left <= n_x+m_pEcffectSprWnd[1]->CrashBox[fr][0].right && UserXY.top <= n_y+m_pEcffectSprWnd[1]->CrashBox[fr][0].bottom)
		{
			char sr[40];
            int tm = pEcffect->how_much;

			pDelEcffect = pEcffect;
			pEcffect = pEcffect->pNext;
			DeleteEcffect(Ecffect_Money, pDelEcffect);
			
			sprintf(sr,"¾Æ·ç¸¦ È¹µæÇÏ¿´½À´Ï´Ù(+%d)",tm);
			m_pHelp->CreateHelp(sr);

			return tm;
		}
		pEcffect = pEcffect->pNext;
	}
	
	pEcffect = Ecffect_Item;
	while(pEcffect)
	{
		char sr[256];
		//int fr  = m_pEcffectSprWnd[1]->pAni[pEcffect->aninum].pFrame[m_pEcffectSprWnd[1]->pAni[pEcffect->aninum].CurFrame].Sprite;
		int n_x = pEcffect->eff_x- m_pMapViewWnd->map_x;
		int	n_y = pEcffect->eff_y- m_pMapViewWnd->map_y;

		if(UserXY.right >= n_x && UserXY.bottom >= n_y
			&& UserXY.left <= (n_x+16) && UserXY.top <= (n_y+16))
		{
			m_pDSound.Playing(m_psnd.eat.bff, false);
			
			if(pEcffect->how_much == -1)
			{
				for(int p=0;p<10;p++)		
				{
					if(m_pStateMss->m_pSprWnd->User_Itm_Att[p].itemnum == -1)
					{
						m_pStateMss->m_pSprWnd->User_Itm_Att[p].itemnum = A_Item[pEcffect->speed].itmnum;
						m_pStateMss->m_pSprWnd->User_Itm_Att[p].num     = 1;
						m_pStateMss->m_pSprWnd->User_Itm_Att[p].checked = A_Item[pEcffect->speed].checked;
	
						sprintf(sr,"¾ÆÀÌÅÛ È¹µæ(%s)",A_Item[pEcffect->speed].itmname);
						m_pHelp->CreateHelp(sr);

						break;
					}
				}
			}
			else if(pEcffect->how_much == -3)
			{
				for(int p=0;p<30;p++)		
				{
					if(m_pStateMss->m_pSprWnd->User_Itm_Etc[p].itemnum == H_Item[pEcffect->speed].itmnum)
					{
						m_pStateMss->m_pSprWnd->User_Itm_Etc[p].itemnum = H_Item[pEcffect->speed].itmnum;
						m_pStateMss->m_pSprWnd->User_Itm_Etc[p].num    += pEcffect->count;
						m_pStateMss->m_pSprWnd->User_Itm_Etc[p].checked = H_Item[pEcffect->speed].checked;
	
						sprintf(sr,"¾ÆÀÌÅÛ È¹µæ(%s)",H_Item[pEcffect->speed].itmname);
						m_pHelp->CreateHelp(sr);

						break;
					}
						
					if(m_pStateMss->m_pSprWnd->User_Itm_Etc[p].itemnum == -1)
					{
						m_pStateMss->m_pSprWnd->User_Itm_Etc[p].itemnum = H_Item[pEcffect->speed].itmnum;
						m_pStateMss->m_pSprWnd->User_Itm_Etc[p].num     = pEcffect->count;
						m_pStateMss->m_pSprWnd->User_Itm_Etc[p].checked = H_Item[pEcffect->speed].checked;

						sprintf(sr,"¾ÆÀÌÅÛ È¹µæ(%s)",H_Item[pEcffect->speed].itmname);
						m_pHelp->CreateHelp(sr);

						break;
					}
				}
			}
			pDelEcffect = pEcffect;
			pEcffect = pEcffect->pNext;
			DeleteEcffect(Ecffect_Item, pDelEcffect);
			
			return -1;
		}
		pEcffect = pEcffect->pNext;
	}
	return -1;
}

void 
cEcffectWnd::PutImage(int X,int Y,int A,int W)
{
	CUSTOM_VERTEX*	pV = NULL;
 
	m_pDirect3D->GetDevice()->SetTexture( 0, 
									 m_pEcffectSprWnd[W]->g_pSprTexture[m_pEcffectSprWnd[W]->pAni[A].pFrame[m_pEcffectSprWnd[W]->pAni[A].CurFrame].Sprite] );

	m_pMapViewWnd->g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

	for( int m=0; m < 4; m++ )
	{
      pV[m].vPos.z = 1.0f;
	  pV[m].rhw = 1.0f;
	  pV[m].dwDiffuse = 0xffffffff;
	}
		

	float	fLeft   = (float)X;
	float	fWidth  = (float)m_pEcffectSprWnd[W]->w_Width[m_pEcffectSprWnd[W]->pAni[A].pFrame[m_pEcffectSprWnd[W]->pAni[A].CurFrame].Sprite];
	float	fTop    = (float)Y;
	float	fHeight = (float)m_pEcffectSprWnd[W]->h_Height[m_pEcffectSprWnd[W]->pAni[A].pFrame[m_pEcffectSprWnd[W]->pAni[A].CurFrame].Sprite];

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
cEcffectWnd::Render()
{
	Ecffect *pEcffect    = NULL;
	Ecffect *pDelEcffect = NULL;
	int XX,YY;

	pEcffect = Ecffect_Buffer;
	while(pEcffect)
	{
		pEcffect->count++;
        m_pEcffectSprWnd[0]->pAni[pEcffect->aninum].CurFrame
			= (pEcffect->count/m_pEcffectSprWnd[0]->pAni[pEcffect->aninum].Delay)%m_pEcffectSprWnd[0]->pAni[pEcffect->aninum].FrameNum;
		
		XX = pEcffect->eff_x+m_pEcffectSprWnd[0]->pAni[pEcffect->aninum].pFrame[ m_pEcffectSprWnd[0]->pAni[pEcffect->aninum].CurFrame].XPos
									- m_pMapViewWnd->map_x;
		YY = pEcffect->eff_y+m_pEcffectSprWnd[0]->pAni[pEcffect->aninum].pFrame[ m_pEcffectSprWnd[0]->pAni[pEcffect->aninum].CurFrame].YPos
									- m_pMapViewWnd->map_y;

		PutImage(XX,YY,pEcffect->aninum,0);
				
		if(m_pEcffectSprWnd[0]->pAni[pEcffect->aninum].CurFrame >= 
			m_pEcffectSprWnd[0]->pAni[pEcffect->aninum].FrameNum-1)
		{
			pDelEcffect = pEcffect;
			pEcffect = pEcffect->pNext;
			DeleteEcffect(Ecffect_Buffer, pDelEcffect);
			continue;
		}
		pEcffect = pEcffect->pNext;
	}

	pEcffect = Ecffect_Money;
	while(pEcffect)
	{
		pEcffect->count++;
		pEcffect->times++;
		
		m_pEcffectSprWnd[1]->pAni[pEcffect->aninum].CurFrame
			= (pEcffect->count/m_pEcffectSprWnd[0]->pAni[pEcffect->aninum].Delay)%m_pEcffectSprWnd[1]->pAni[pEcffect->aninum].FrameNum;
		
		if(!m_pStateMss->m_pSprWnd->m_pPet)
		{
			if(pEcffect->times < 10)
			{
			//	pEcffect->speed++;
				pEcffect->eff_y-=8;//speedmoney[pEcffect->speed];
			}
			else
			{
				if(m_pMapViewWnd->pMAP->GetAttr(pEcffect->map_eff_x,pEcffect->map_eff_y+1) == 0
				|| m_pMapViewWnd->pMAP->GetAttr(pEcffect->map_eff_x,pEcffect->map_eff_y+1) == 3)
				{
			/*		pEcffect->speed--;
					if(pEcffect->speed<0)
						pEcffect->speed = 0;*/	

					pEcffect->eff_y+=8;//speedmoney[pEcffect->speed];
					pEcffect->map_eff_x=pEcffect->eff_x/8;
					pEcffect->map_eff_y=pEcffect->eff_y/8;
				}	
			}
		}else
		{
			if(m_pStateMss->m_pSprWnd->m_pPet->pet_x > pEcffect->eff_x)
				pEcffect->eff_x+=8;
			else if(m_pStateMss->m_pSprWnd->m_pPet->pet_x < pEcffect->eff_x)
				pEcffect->eff_x-=8;
			
			if(m_pStateMss->m_pSprWnd->m_pPet->pet_y > pEcffect->eff_y)
				pEcffect->eff_y+=8;
			else if(m_pStateMss->m_pSprWnd->m_pPet->pet_y < pEcffect->eff_y)
				pEcffect->eff_y-=8;
		}

		XX = pEcffect->eff_x+m_pEcffectSprWnd[1]->pAni[pEcffect->aninum].pFrame[ m_pEcffectSprWnd[1]->pAni[pEcffect->aninum].CurFrame].XPos
									- m_pMapViewWnd->map_x;
		YY = pEcffect->eff_y+m_pEcffectSprWnd[1]->pAni[pEcffect->aninum].pFrame[ m_pEcffectSprWnd[1]->pAni[pEcffect->aninum].CurFrame].YPos
									- m_pMapViewWnd->map_y;

		PutImage(XX,YY,pEcffect->aninum,1);
		
		if(!m_pStateMss->m_pSprWnd->m_pPet)
		{		
			if( pEcffect->times >= 1000)
			{
				pDelEcffect = pEcffect;
				pEcffect = pEcffect->pNext;
				DeleteEcffect(Ecffect_Money, pDelEcffect);
				continue;
			}
		}else
		{
			if(m_pStateMss->m_pSprWnd->m_pPet->pet_x-30 <= pEcffect->eff_x && m_pStateMss->m_pSprWnd->m_pPet->pet_x+30 >= pEcffect->eff_x
				&& m_pStateMss->m_pSprWnd->m_pPet->pet_y-30 <= pEcffect->eff_y && m_pStateMss->m_pSprWnd->m_pPet->pet_y+30 >= pEcffect->eff_y)
			{
				char sr[40];
			     int tm = pEcffect->how_much;

				pDelEcffect = pEcffect;
				pEcffect = pEcffect->pNext;
				DeleteEcffect(Ecffect_Money, pDelEcffect);
			
				sprintf(sr,"¾Æ·ç¸¦ È¹µæÇÏ¿´½À´Ï´Ù(+%d)",tm);
				m_pHelp->CreateHelp(sr);
					
				m_pStateMss->m_pSprWnd->Money +=tm;
				
				continue;
			}
		}
		pEcffect = pEcffect->pNext;
	}

	pEcffect = Ecffect_Item;
	while(pEcffect)
	{
		pEcffect->times++;
		if(pEcffect->times < 10)
		{
		//	pEcffect->speed++;
			pEcffect->eff_y-=8;//speedmoney[pEcffect->speed];
		}
		else
		{
			if(m_pMapViewWnd->pMAP->GetAttr(pEcffect->map_eff_x,pEcffect->map_eff_y+4) == 0
			|| m_pMapViewWnd->pMAP->GetAttr(pEcffect->map_eff_x,pEcffect->map_eff_y+4) == 3)
			{
				pEcffect->eff_y+=8;//speedmoney[pEcffect->speed];
				pEcffect->map_eff_x=pEcffect->eff_x/8;
				pEcffect->map_eff_y=pEcffect->eff_y/8;
			}	
		}

		XX = pEcffect->eff_x- m_pMapViewWnd->map_x;
		YY = pEcffect->eff_y- m_pMapViewWnd->map_y;

		//PutImage(XX,YY,pEcffect->aninum,1);
		m_pStateMss->m_pIpkData->DrawPic(XX,YY,pEcffect->aninum);

		if( pEcffect->times >= 1000)
		{
			pDelEcffect = pEcffect;
			pEcffect = pEcffect->pNext;
			DeleteEcffect(Ecffect_Item, pDelEcffect);
			continue;
		}
		pEcffect = pEcffect->pNext;
	}
}
