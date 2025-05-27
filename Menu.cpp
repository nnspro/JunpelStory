//#include "stdafx.h"

#include "MapOutput.h"
#include "InputManager.h"
#include "MessageBox.h"
#include "DirectSound.h"
#include "StatusForm_0.h"

#include "Menu.h"

#include <stdio.h>

CMessageBox *m_pBox   = NULL;
cState_     *m_pState = NULL;

char ssound = 0;

cMENU::cMENU()
{
	m_pDirect3d = NULL;
	m_pPicData  = NULL;
	m_pState    = NULL;
	
	menu__ = 0;
	
	for(int p=0;p<9;p++)
		m_pBtnPic[p] = NULL;

	menu_btn_clk  = 0;
	menu_btn_clk1 = 3;
	menu_btn_clk2 = 6;
	menu_btn_ck   = 0;

}

cMENU::~cMENU()
{
	SAFE_DELETE_(m_pPicData);
	for(int p=0;p<9;p++)
		SAFE_DELETE_(m_pBtnPic[p]);
}


void
cMENU::Init(CDirect3D *pDirect3d,cSprWnd *spr)
{
	char sr[256];

	m_pDirect3d = pDirect3d;
	m_pSprWnd   = spr;

	m_pPicData = new CPicData(pDirect3d);
	m_pPicData->Load("pic/menu.pic");
	m_pPicData->LoadPicTexture();

	for(int p=0;p<6;p++)
	{
		sprintf(sr,"pic/menu_btn_%d.pic",p);
		m_pBtnPic[p] = new CPicData(pDirect3d);
		m_pBtnPic[p]->Load(sr);
		m_pBtnPic[p]->LoadPicTexture();
	}

	for(int p=6;p<9;p++)
	{
		sprintf(sr,"pic/ms_btn_%d.pic",p-6);
		m_pBtnPic[p] = new CPicData(pDirect3d);
		m_pBtnPic[p]->Load(sr);
		m_pBtnPic[p]->LoadPicTexture();
	}
}

void
cMENU::DrawPic(int x,int y,CPicData *m_pPic)
{
    //	Sprite에 사용될 쉐이더 설정
m_pDirect3d->GetDevice()->SetTexture( 0, m_pPic->g_pPicTexture);
			
	CUSTOM_VERTEX*	pV = NULL;

	m_pMapViewWnd->g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

	for( int m=0; m < 4; m++ )
	{
      pV[m].vPos.z = 1.0f;
	  pV[m].rhw = 1.0f;
	  pV[m].dwDiffuse = 0xffffffff;
	}
		

	float	fLeft   = (float)x;
	float	fWidth  = (float)m_pPic->w_Width;
	float	fTop    = (float)y;
	float	fHeight = (float)m_pPic->h_Height;

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

	m_pDirect3d->GetDevice()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
}

int
cMENU::Render()
{
	if(!m_pBox)
	{
		MousePosition();
	
		DrawPic(0,380,m_pPicData);
		DrawPic(2,412,m_pBtnPic[menu_btn_clk]);
		DrawPic(2,445,m_pBtnPic[menu_btn_clk1]);
		DrawPic(110,382,m_pBtnPic[menu_btn_clk2]);
	}
	else
	{
		if(m_pState)
			SAFE_DELETE_(m_pState);
		
		int a = m_pBox->Render();
		if(a != -50)
		{
			SAFE_DELETE_(m_pBox);
			return a;
		}
	}

	if(m_pState)
	{
		int a = m_pState->Render();
		if(a != -50)
		{
			SAFE_DELETE_(m_pState);
			return -50;
		}
	}

	if(menu__ != 0)
		return menu__;

	return -50;
}

void cMENU::MousePosition()
{
	menu_btn_clk  = 0;
	menu_btn_clk1 = 3;
	menu_btn_clk2 = 6;

	if(m_pInputManager->m_MouseXPos >= 2 && m_pInputManager->m_MouseYPos >= 412
			&& m_pInputManager->m_MouseXPos <= 126 && m_pInputManager->m_MouseYPos <= 444)
	{
		menu_btn_clk = 1;
		if(ssound != 1)
		{
			m_pDSound.Playing(m_psnd.btn.bff, false);
			ssound = 1;
		}
		if(m_pInputManager->m_isLButton == true)
		{
			menu_btn_clk = 2;
			menu_btn_ck = 1;
		}else if(m_pInputManager->m_isLButton == false)
		{
			if(menu_btn_ck == 1)
			{
				menu_btn_ck = 0;
				if(!m_pState)
				{
					m_pDSound.Playing(m_psnd.btn0.bff, false);
				
					m_pState = new cState_();
					m_pState->Init(m_pDirect3d,m_pSprWnd);
				}
			}
		}
	}else 
		if(m_pInputManager->m_MouseXPos >= 2 && m_pInputManager->m_MouseYPos >= 445
			&& m_pInputManager->m_MouseXPos <= 126 && m_pInputManager->m_MouseYPos <= 477)
		{
			menu_btn_clk1 = 4;
			if(ssound != 2)
			{
				m_pDSound.Playing(m_psnd.btn.bff, false);
				ssound = 2;
			}
			if(m_pInputManager->m_isLButton == true)
			{
				menu_btn_clk1 = 5;
				menu_btn_ck = 2;
			}else if(m_pInputManager->m_isLButton == false)
			{
				if(menu_btn_ck == 2)
				{
					menu_btn_ck = 0;
					if(!m_pBox)
					{
						m_pDSound.Playing(m_psnd.btn0.bff, false);
						
						m_pBox = new CMessageBox();
						m_pBox->Init(m_pDirect3d,300,100,1,0);
						CopyMemory(m_pBox->M_Box.Box_Text[0],"정말로 모험을 포기합니까?",25);
					}
				}
			}
		}else 
		if(m_pInputManager->m_MouseXPos >= 110 && m_pInputManager->m_MouseYPos >= 382
			&& m_pInputManager->m_MouseXPos <= 126 && m_pInputManager->m_MouseYPos <= 398)
		{
			menu_btn_clk2 = 7;
			if(ssound != 3)
			{
				m_pDSound.Playing(m_psnd.btn.bff, false);
				ssound = 3;
			}
			if(m_pInputManager->m_isLButton == true)
			{
				menu_btn_clk2 = 8;
				menu_btn_ck = 3;
			}else if(m_pInputManager->m_isLButton == false)
			{
				if(menu_btn_ck == 3)
				{
					m_pDSound.Playing(m_psnd.btn0.bff, false);
					
					menu_btn_ck = 0;
					menu__ = -10;
				}
			}
		}else ssound = 0;
}