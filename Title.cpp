#include "MapOutput.h"
#include "DirectSound.h"
#include "InputManager.h"
#include "Title.h"

cTitle::cTitle()
{
	m_pDirect3D = NULL;
	m_pPicData  = NULL;

	//show = -1;
}

cTitle::~cTitle()
{
	SAFE_DELETE_(m_pPicData);

	for(int p=0;p<3;p++)
		SAFE_DELETE_(BackPic[p]);
}

void cTitle::Init(CDirect3D *D3D)
{
	char buf[256];

	m_pDirect3D = D3D;

	m_pPicData = new CPicData(D3D);
	m_pPicData->Load("pic/titleback.pic");
	m_pPicData->LoadPicTexture();

	BackPic[0] = new CPicData(D3D);
    BackPic[0]->Load("pic/title.pic");
	BackPic[0]->LoadPicTexture();

	BackPic[1] = new CPicData(D3D);
    BackPic[1]->Load("pic/d_start.pic");
	BackPic[1]->LoadPicTexture();

	BackPic[2] = new CPicData(D3D);
    BackPic[2]->Load("pic/b_start.pic");
	BackPic[2]->LoadPicTexture();
	
	clk  = 1;
	ssnd = clk1 = 0;
}

BOOL
cTitle::Render()
{
	MousePos();
	
	if(clk == 3)
		return TRUE;
	
	DrawPic(0,0,800,600);

	DrawBbPic(130,20,0);
	DrawBbPic(600,500,clk);

	return FALSE;
}
void
cTitle::DrawBbPic(int x,int y,int num)
{
    //	Sprite에 사용될 쉐이더 설정
m_pDirect3D->GetDevice()->SetTexture( 0, 
									 BackPic[num]->g_pPicTexture);
			
	CUSTOM_VERTEX*	pV = NULL;

	m_pMapViewWnd->g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

	for( int m=0; m < 4; m++ )
	{
      pV[m].vPos.z = 1.0f;
	  pV[m].rhw = 1.0f;
	  pV[m].dwDiffuse = 0xffffffff;
	}
		

	float	fLeft   = (float)x;
	float	fWidth  = (float)BackPic[num]->w_Width;
	float	fTop    = (float)y;
	float	fHeight = (float)BackPic[num]->h_Height;

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
cTitle::DrawPic(int x,int y,int Zoom_x,int Zoom_y)
{
    //	Sprite에 사용될 쉐이더 설정
	m_pDirect3D->GetDevice()->SetTexture( 0, m_pPicData->g_pPicTexture);
			
	CUSTOM_VERTEX*	pV = NULL;

	m_pMapViewWnd->g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

	for( int m=0; m < 4; m++ )
	{
      pV[m].vPos.z = 1.0f;
	  pV[m].rhw = 1.0f;
	  pV[m].dwDiffuse = 0xffffffff;
	}
		

	float	fLeft   = (float)x;
	float	fWidth  = (float)Zoom_x;
	float	fTop    = (float)y;
	float	fHeight = (float)Zoom_y;

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
cTitle::MousePos()
{
	clk = 1;

	if(m_pInputManager->m_MouseXPos >= 600 && m_pInputManager->m_MouseYPos >= 500
			&& m_pInputManager->m_MouseXPos <= 728 && m_pInputManager->m_MouseYPos <= 532)
	{

		clk = 2;
		if(ssnd != 1)
		{
			m_pDSound.Playing(m_psnd.btn.bff, false);
			ssnd = 1;
		}

		if(m_pInputManager->m_isLButton == true)
		{
			clk1 = 1;
		}else if(m_pInputManager->m_isLButton == false)
		{
			if(clk1 == 1)
			{
				m_pDSound.Playing(m_psnd.btn0.bff, false);
				clk1 = 0;
				clk = 3;
			}
		}
	}else ssnd = 0;
}