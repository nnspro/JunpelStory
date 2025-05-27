#include "MapOutput.h"
#include "StatusForm.h"
#include "MoveMagic.h"

cMoveMagic::cMoveMagic()
{
	m_pDirect3D = NULL;
	m_pPicData  = NULL;
}

cMoveMagic::~cMoveMagic()
{
	SAFE_DELETE_(m_pPicData);
}

void cMoveMagic::Init(CDirect3D *D3D)
{
	few = 0;
	n = m =  S_Item[m_pStateMss->m_pSprWnd->Interface_[1].checked].use + (m_pStateMss->m_pSprWnd->Int*100);
	
	m_pDirect3D = D3D;

	m_pPicData = new CPicData(D3D);
	m_pPicData->Load("pic/mp.pic");
	m_pPicData->LoadPicTexture();
}
BOOL
cMoveMagic::Render()
{
	if(few < 2)
		m_pStateMss->m_pIpkData->DrawPic(750,450,53);
	m--;
	int u = 50.0f/((float)n/(float)m);
	DrawPic(735,470,u,m_pPicData);
	if(m < 200)
	{
		few++;
		if(few == 4)
			few = 0;
	}
	if(m <= 0)
		return FALSE;

	return TRUE;
}

void
cMoveMagic::DrawPic(int x,int y,int xsize, CPicData *Pic)
{

	//LoadAttTexture(item);
    //	Sprite에 사용될 쉐이더 설정
m_pDirect3D->GetDevice()->SetTexture( 0, 
									 Pic->g_pPicTexture);
			
	CUSTOM_VERTEX*	pV = NULL;

	m_pMapViewWnd->g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

	for( int m=0; m < 4; m++ )
	{
      pV[m].vPos.z = 1.0f;
	  pV[m].rhw = 1.0f;
	  pV[m].dwDiffuse = 0xffffffff;
	}
		

	float	fLeft   = (float)x;
	float	fWidth  = (float)xsize;
	float	fTop    = (float)y;
	float	fHeight = 4.0;

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