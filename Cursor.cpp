#include "MapOutput.h"
#include "InputManager.h"
#include "Cursor.h"

cMouseCursor::cMouseCursor()
{
	m_pDirect3D = NULL;
	m_pPicData  = NULL;

	//show = -1;
}

cMouseCursor::~cMouseCursor()
{
	SAFE_DELETE_(m_pPicData);
}

void cMouseCursor::Init(CDirect3D *D3D)
{
	m_pDirect3D = D3D;

	m_pPicData = new CPicData(D3D);
	m_pPicData->Load("cursor/akman.cur");
	m_pPicData->LoadPicTexture();
}
void
cMouseCursor::Render()
{
	float x_cur,y_cur;
	
	x_cur =(float)m_pInputManager->m_MouseXPos;
	y_cur = (float)m_pInputManager->m_MouseYPos;

	DrawPic(x_cur,y_cur);
}
void
cMouseCursor::DrawPic(float x,float y)
{
    //	Sprite에 사용될 쉐이더 설정
m_pDirect3D->GetDevice()->SetTexture( 0, 
									 m_pPicData->g_pPicTexture);
			
	CUSTOM_VERTEX*	pV = NULL;

	m_pMapViewWnd->g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

	for( int m=0; m < 4; m++ )
	{
      pV[m].vPos.z = 1.0f;
	  pV[m].rhw = 1.0f;
	  pV[m].dwDiffuse = 0xffffffff;
	}
		

	float	fLeft   = x;
	float	fWidth  = 32.0f;
	float	fTop    = y;
	float	fHeight = 32.0f;

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