//#include "stdafx.h"

#include "MapOutput.h"
#include "InputManager.h"
#include "TextInput.h"
#include "DirectSound.h"
#include "MessageBox.h"

#include <stdio.h>

int clk = 0,clk1 = 3;
int clk_ck = 0;

char sssound = 0;

CChatManager Input_number;

CMessageBox::CMessageBox(void)
{
	m_pDirect3d = NULL;
	m_pPicData  = NULL;
	for(int p=0;p<6;p++)
		m_pBtnPic[p] = NULL;

	ZeroMemory(number,3);
}

CMessageBox::~CMessageBox(void)
{
	SAFE_DELETE_(m_pD3dxfont);
	SAFE_DELETE_(m_pPicData);
	for(int p=0;p<6;p++)
		SAFE_DELETE_(m_pBtnPic[p]);
}


void
CMessageBox::Init(CDirect3D *pDirect3d,int x,int y,int btn,int read)
{
	char sr[256];

	m_pDirect3d = pDirect3d;
	
	m_pPicData = new CPicData(pDirect3d);
	m_pPicData->Load("pic/mss_box.pic");
	m_pPicData->LoadPicTexture();

	for(int p=0;p<6;p++)
	{
		sprintf(sr,"pic/mss_box_btn_%d.pic",p);
		m_pBtnPic[p] = new CPicData(pDirect3d);
		m_pBtnPic[p]->Load(sr);
		m_pBtnPic[p]->LoadPicTexture();
	}

	m_pD3dxfont = NULL;
	m_pD3dxfont = new CD3dxFont();
	if(!m_pD3dxfont->CreateFONT(m_pDirect3d->GetDevice(), (char*)GetStockObject(SYSTEM_FONT), 16))
		return;

	M_Box.box_x     = x;
	M_Box.box_y     = y;
	M_Box.box_btn   = btn;
	M_Box.send      = -50;
	M_Box.box_input = read;
	M_Box.max       = -1;

	if(read == 1)
	{
		Input_number.Init(m_pDirect3d,18);
		Input_number.LinkDevice(m_pMapViewWnd->h_hndW, m_pMapViewWnd->h_hinsT, 4);
		Input_number.SetColor(D3DCOLOR_XRGB( 255, 255, 255));
		Input_number.SetInPos(x+140,  y+60);
		Input_number.SetOutPos(x+140, y+60);
		Input_number.InputStart();
	}

	for(int p=0;p<2;p++)
		ZeroMemory(M_Box.Box_Text[p],30);	
}

void
CMessageBox::DrawPic(int x,int y,CPicData *m_pPic)
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
CMessageBox::Render()
{
	MousePosition();
	
	DrawPic(M_Box.box_x,M_Box.box_y,m_pPicData);
	if(M_Box.box_btn == 0)
		DrawPic(M_Box.box_x+100,M_Box.box_y+95,m_pBtnPic[clk]);
	else if(M_Box.box_btn == 1)
	{
		DrawPic(M_Box.box_x+150,M_Box.box_y+95,m_pBtnPic[clk]);
		DrawPic(M_Box.box_x+200,M_Box.box_y+95,m_pBtnPic[clk1]);
	}

	for(int p=0;p<2;p++)
	{
		if(strlen(M_Box.Box_Text[p])>0)
			m_pD3dxfont->DrawText(M_Box.box_x+10,(p*16)+(M_Box.box_y+50),0,0,0xffffffff,
				M_Box.Box_Text[p],strlen(M_Box.Box_Text[p]));
	}

	if(M_Box.box_input == 1)
		Input_number.Render(FALSE);

	return M_Box.send;
}

void CMessageBox::MousePosition()
{//45,24
	clk  = 0;
	clk1 = 3;

	if(M_Box.box_btn == 0)
	{
		if(m_pInputManager->m_MouseXPos >= (M_Box.box_x+100) && m_pInputManager->m_MouseYPos >= (M_Box.box_y+95)
				&& m_pInputManager->m_MouseXPos <= (M_Box.box_x+145) && m_pInputManager->m_MouseYPos <= (M_Box.box_y+119))
		{
			clk = 1;
			if(sssound != 1)
			{
				m_pDSound.Playing(m_psnd.btn.bff, false);
				sssound = 1;
			}
			if(m_pInputManager->m_isLButton == true)
			{
				clk    = 2;
				clk_ck = 1;
			}else if(m_pInputManager->m_isLButton == false)
			{
				if(clk_ck == 1)
				{
					m_pDSound.Playing(m_psnd.btn0.bff, false);
				
					clk_ck = 0;
					if(M_Box.box_input == 0)
						M_Box.send = -20;
					else if(M_Box.box_input == 1)
					{
						M_Box.send = -10;
                        Input_number.InputEnd();
						strcpy(number, Input_number.m_StrOut);
						if(strlen(number)>0)
						{
							if(M_Box.max!=-1)
							{
								if(M_Box.max<atoi(number))
									return;
							}
							//if(isdigit(token))     
							if(IsNumeric(number) == TRUE)
								M_Box.send = atoi(number);
						}
					}
				}
			}
		}else sssound = 0;
	}else 
	if(M_Box.box_btn == 1)
	{
		if(m_pInputManager->m_MouseXPos >= (M_Box.box_x+150) && m_pInputManager->m_MouseYPos >= (M_Box.box_y+95)
				&& m_pInputManager->m_MouseXPos <= (M_Box.box_x+195) && m_pInputManager->m_MouseYPos <= (M_Box.box_y+119))
		{
			clk = 1;
			if(sssound != 2)
			{
				m_pDSound.Playing(m_psnd.btn.bff, false);
				sssound = 2;
			}
			if(m_pInputManager->m_isLButton == true)
			{
				clk    = 2;
				clk_ck = 1;
			}else if(m_pInputManager->m_isLButton == false)
			{
				if(clk_ck == 1)
				{
					m_pDSound.Playing(m_psnd.btn0.bff, false);
					
					clk_ck = 0;
					if(M_Box.box_input == 0)
						M_Box.send = -20;
					else if(M_Box.box_input == 1)
					{
						M_Box.send = -10;
						Input_number.InputEnd();
						strcpy(number, Input_number.m_StrOut);
						if(strlen(number)>0)
						{
							if(M_Box.max!=-1)
							{
								if(M_Box.max<atoi(number))
									return;
							}    
							if(IsNumeric(number) ==TRUE)
								M_Box.send = atoi(number);		
						}
					}
				}
			}
		}
		else
			if(m_pInputManager->m_MouseXPos >= (M_Box.box_x+200) && m_pInputManager->m_MouseYPos >= (M_Box.box_y+95)
				&& m_pInputManager->m_MouseXPos <= (M_Box.box_x+245) && m_pInputManager->m_MouseYPos <= (M_Box.box_y+119))
		{
			clk1 = 4;
			if(sssound != 3)
			{
				m_pDSound.Playing(m_psnd.btn.bff, false);
				sssound = 3;
			}
			if(m_pInputManager->m_isLButton == true)
			{
				clk1    = 5;
				clk_ck = 2;
			}else if(m_pInputManager->m_isLButton == false)
			{
				if(clk_ck == 2)
				{
					m_pDSound.Playing(m_psnd.btn0.bff, false);
					
					clk_ck = 0;
					M_Box.send = -10;
					
					if(M_Box.box_input == 1)
						Input_number.InputEnd();
				}
			}
		}else sssound = 0;
	}
}