#include "MapOutput.h"
#include "StatusForm.h"
#include "DirectSound.h"
#include "Pet.h"

cPetWnd::cPetWnd()
{
	m_pDirect3D   = NULL;
	
	m_pPetSprWnd = NULL;
}

cPetWnd::~cPetWnd()
{
	SAFE_DELETE_(m_pPetSprWnd);
}

void
cPetWnd::Init(CDirect3D   *D3D)
{
	m_pDirect3D = D3D;
	
	m_pPetSprWnd = new cSprDataWnd();
	m_pPetSprWnd->Init(m_pDirect3D);
	m_pPetSprWnd->SprLoad(123);
	m_pPetSprWnd->LoadSprTexture();

	COUNTER = 0;
}

void
cPetWnd::InitPet()
{
	if(m_pStateMss->m_pSprWnd->view == 0)
	{
		aninum = 1;
		pet_x = m_pStateMss->m_pSprWnd->spr_x - 50;
	}else if(m_pStateMss->m_pSprWnd->view == 1)
	{
		aninum = 0;
		pet_x = m_pStateMss->m_pSprWnd->spr_x + 50;
	}
	
	user_y = m_pStateMss->m_pSprWnd->spr_y;
	
	if(pet_x < 15)
		pet_x = 15;
	/*else if(pet_x > ((m_pMapViewWnd->pMAP->WIDTH*4)-10))
		pet_x = ((m_pMapViewWnd->pMAP->WIDTH*4)-10);*/

	pet_y = user_y - 80;
	//if(pet_y < 0)pet_y = 0;
}

void 
cPetWnd::PutImage(int monX,int monY,int monA)
{
	CUSTOM_VERTEX*	pV = NULL;
 
	m_pDirect3D->GetDevice()->SetTexture( 0, 
									 m_pPetSprWnd->g_pSprTexture[m_pPetSprWnd->pAni[monA].pFrame[m_pPetSprWnd->pAni[monA].CurFrame].Sprite] );

	m_pMapViewWnd->g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

	for( int m=0; m < 4; m++ )
	{
      pV[m].vPos.z = 1.0f;
	  pV[m].rhw = 1.0f;
	  pV[m].dwDiffuse = 0xffffffff;
	}
		

	//m_pMonSprWnd[monN]->pAni[monA].CurFrame = Mon[n].CurFrame;
	float	fLeft   = (float)monX;//m_pMonSprWnd[monN]->pAni[monA].CurFrame
	float	fWidth  = (float)m_pPetSprWnd->w_Width[m_pPetSprWnd->pAni[monA].pFrame[m_pPetSprWnd->pAni[monA].CurFrame].Sprite];
	float	fTop    = (float)monY;
	float	fHeight = (float)m_pPetSprWnd->h_Height[m_pPetSprWnd->pAni[monA].pFrame[m_pPetSprWnd->pAni[monA].CurFrame].Sprite];

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
cPetWnd::Render()
{
	int P_x,P_y;

	if((rand()%100)==80)
		m_pDSound.Playing(m_psnd.pet.bff,false);

	PetMove();
	
	P_x = pet_x + m_pPetSprWnd->pAni[aninum].pFrame[m_pPetSprWnd->pAni[aninum].CurFrame].XPos
			- m_pMapViewWnd->map_x;
	P_y = pet_y + m_pPetSprWnd->pAni[aninum].pFrame[m_pPetSprWnd->pAni[aninum].CurFrame].YPos
			- m_pMapViewWnd->map_y;

	PutImage(P_x,P_y,aninum);
}

void 
cPetWnd::PetMove()
{

	if(m_pStateMss->m_pSprWnd->spr_x-150>pet_x)
	{
		if(aninum != 1)
		{
			aninum = 1;
			COUNTER = 0;
		}
		pet_x+=7;
	}
	else if(m_pStateMss->m_pSprWnd->spr_x+150<pet_x)
	{
		if(aninum != 0)
		{
			aninum = 0;
			COUNTER = 0;
		}
		pet_x-=7;
	}
	
	if(m_pStateMss->m_pSprWnd->spr_y>user_y)
	{
		pet_y+=(m_pStateMss->m_pSprWnd->spr_y-user_y);
		user_y = m_pStateMss->m_pSprWnd->spr_y;
	}
	else if(m_pStateMss->m_pSprWnd->spr_y<user_y)
	{
		pet_y-=(user_y-m_pStateMss->m_pSprWnd->spr_y);
		user_y = m_pStateMss->m_pSprWnd->spr_y;
	}
	
	COUNTER++;
	m_pPetSprWnd->pAni[aninum].CurFrame=(COUNTER/m_pPetSprWnd->pAni[aninum].Delay)%m_pPetSprWnd->pAni[aninum].FrameNum;

}

/*
int
cBossWnd::HitItem(RECT UserXY)
{
	return m_pStateMss->m_pEcffect->ItemChk(UserXY);
}*/