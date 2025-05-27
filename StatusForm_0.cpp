//#include "stdafx.h"

#include "MapOutput.h"
#include "InputManager.h"
#include "DirectSound.h"
#include "StatusForm_0.h"

#include <stdio.h>

char s_sound = 0;

cState_::cState_()
{
	m_pDirect3d = NULL;
	
	for(int p=0;p<2;p++)
		m_pPicData[p] = NULL;
	
	for(int p=0;p<12;p++)
		m_pBtnPic[p]  = NULL;

	scroll =  0;
	x_sc   =  0;
	send_  = -1;
	
	state_btn_clk  = 0;
	state_btn_clk1 = 0;
	state_btn_clk2 = 3;
	state_btn_clk3 = 6;
	state_ck       = 0;
    plus_btn0      = 9;
	plus_btn1      = 9;
	plus_btn2      = 9;
    plus_btn3      = 9;
	plus_btn4      = 9;
}

cState_::~cState_()
{
	SAFE_DELETE_(m_pD3dxfont);
	for(int p=0;p<2;p++)
		SAFE_DELETE_(m_pPicData[p]);
	
	for(int p=0;p<12;p++)
		SAFE_DELETE_(m_pBtnPic[p]);
}


void
cState_::Init(CDirect3D *pDirect3d,cSprWnd *spr)
{
	char sr[256];

	m_pDirect3d = pDirect3d;
	m_pSprWnd   = spr;

	for(int p=0;p<2;p++)
	{
		sprintf(sr,"pic/state_%d.pic",p);
		m_pPicData[p] = new CPicData(pDirect3d);
		m_pPicData[p]->Load(sr);
		m_pPicData[p]->LoadPicTexture();
	}

	for(int p=0;p<12;p++)
	{
		sprintf(sr,"pic/state_btn_%d.pic",p);
		m_pBtnPic[p] = new CPicData(pDirect3d);
		m_pBtnPic[p]->Load(sr);
		m_pBtnPic[p]->LoadPicTexture();
	}
	m_pD3dxfont = NULL;
	m_pD3dxfont = new CD3dxFont();
	if(!m_pD3dxfont->CreateFONT(m_pDirect3d->GetDevice(), (char*)GetStockObject(SYSTEM_FONT), 16))
		return;
}

void
cState_::DrawPic(int x,int y,CPicData *m_pPic)
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
cState_::Render()
{
	char str[256];

	MousePosition();

	if(state_btn_clk >= 1)
	{
		if(state_btn_clk == 1)
		{
			if(scroll <= 15)
			{
				scroll++;
				x_sc+=8;
			}
		}else if(state_btn_clk == 2)
		{
			if(scroll > 0)
			{
				scroll--;
				x_sc-=8;
			}else if(scroll == 0)state_btn_clk = 0;
		}
		DrawPic(130+x_sc,417,m_pPicData[1]);
		DrawPic(132+x_sc,462,m_pBtnPic[state_btn_clk2]);
		
		sprintf(str,"%d",(m_pSprWnd->Str/3)+A_Item[m_pSprWnd->Interface_[0].checked].att);
		m_pD3dxfont->DrawText(200+x_sc,425,0,0,0xffffffff,str,strlen(str));
		sprintf(str,"%d",m_pSprWnd->Dex);
		m_pD3dxfont->DrawText(200+x_sc,445,0,0,0xffffffff,str,strlen(str));
	}
	DrawPic(130,225,m_pPicData[0]);

	if(state_btn_clk == 0)
		DrawPic(240,462,m_pBtnPic[state_btn_clk1]);//->
	
	DrawPic(240,227,m_pBtnPic[state_btn_clk3]);
	
	if(m_pSprWnd->stackPoint > 0)
	{
		DrawPic(240,358,m_pBtnPic[plus_btn0]);//+
		DrawPic(240,375,m_pBtnPic[plus_btn1]);
		DrawPic(240,392,m_pBtnPic[plus_btn2]);//+
		DrawPic(240,409,m_pBtnPic[plus_btn3]);
		DrawPic(240,426,m_pBtnPic[plus_btn4]);
	}
	m_pD3dxfont->DrawText(182,243,0,0,0xffffffff,m_pSprWnd->Name,strlen(m_pSprWnd->Name));
	sprintf(str,"%d",m_pSprWnd->Level);
	m_pD3dxfont->DrawText(182,263,0,0,0xffffffff,str,strlen(str));
	sprintf(str,"%d/%d",m_pSprWnd->Hp,m_pSprWnd->totalHp);
	m_pD3dxfont->DrawText(182,290,0,0,0xffffffff,str,strlen(str));
	sprintf(str,"%d/%d",m_pSprWnd->Mp,m_pSprWnd->totalMp);
	m_pD3dxfont->DrawText(182,309,0,0,0xffffffff,str,strlen(str));
	sprintf(str,"%d",m_pSprWnd->Kyunghum,m_pSprWnd->totalKyunghum);
	m_pD3dxfont->DrawText(187,330,0,0,0xffffffff,str,strlen(str));
	sprintf(str,"/%d",m_pSprWnd->totalKyunghum);
	m_pD3dxfont->DrawText(182,340,0,0,0xffffffff,str,strlen(str));
	sprintf(str,"%d",m_pSprWnd->Str);
	m_pD3dxfont->DrawText(182,356,0,0,0xffffffff,str,strlen(str));
	sprintf(str,"%d",m_pSprWnd->Dex);
	m_pD3dxfont->DrawText(182,374,0,0,0xffffffff,str,strlen(str));
	sprintf(str,"%d",m_pSprWnd->Int);
	m_pD3dxfont->DrawText(182,391,0,0,0xffffffff,str,strlen(str));
	sprintf(str,"%d",m_pSprWnd->Hana);
	m_pD3dxfont->DrawText(182,408,0,0,0xffffffff,str,strlen(str));
	sprintf(str,"%d",m_pSprWnd->Mana);
	m_pD3dxfont->DrawText(182,426,0,0,0xffffffff,str,strlen(str));
	sprintf(str,"%d",m_pSprWnd->stackPoint);
	m_pD3dxfont->DrawText(143,457,0,0,0xffffffff,str,strlen(str));

	if(send_ != -1)return send_;

	return -50;
}

void
cState_::MousePosition()
{
	state_btn_clk1 = 0;
	state_btn_clk2 = 3;
    state_btn_clk3 = 6;
	
	if(m_pInputManager->m_MouseXPos >= 240 && m_pInputManager->m_MouseYPos >= 227
			&& m_pInputManager->m_MouseXPos <= 256 && m_pInputManager->m_MouseYPos <= 243)
	{
		state_btn_clk3 = 7;
		if(s_sound != 1)
		{
			m_pDSound.Playing(m_psnd.btn.bff, false);
			s_sound = 1;
		}
		if(m_pInputManager->m_isLButton == true)
		{
			state_btn_clk3 = 8;
			state_ck = 1;
		}else if(m_pInputManager->m_isLButton == false)
		{
			if(state_ck == 1)
			{
				m_pDSound.Playing(m_psnd.btn0.bff, false);
	
				state_ck =   0;
				send_    = -10;
			}
		}
	}else 
		if(m_pInputManager->m_MouseXPos >= 240 && m_pInputManager->m_MouseYPos >= 462
			&& m_pInputManager->m_MouseXPos <= 256 && m_pInputManager->m_MouseYPos <= 478)
		{
			if(state_btn_clk == 0)
			{
				if(s_sound != 2)
				{
					m_pDSound.Playing(m_psnd.btn.bff, false);
					s_sound = 2;
				}
				state_btn_clk1 = 1;
				if(m_pInputManager->m_isLButton == true)
				{
					state_btn_clk1 = 2;
					state_ck = 2;
				}else if(m_pInputManager->m_isLButton == false)
				{
					if(state_ck == 2)
					{
						m_pDSound.Playing(m_psnd.btn0.bff, false);
						
						state_ck = 0;
						state_btn_clk = 1;
					}
				}
			}
		}else 
		if(m_pInputManager->m_MouseXPos >= 260 && m_pInputManager->m_MouseYPos >= 462
			&& m_pInputManager->m_MouseXPos <= 276 && m_pInputManager->m_MouseYPos <= 478)
		{
			if(state_btn_clk == 1)
			{
				if(s_sound != 3)
				{
					m_pDSound.Playing(m_psnd.btn.bff, false);
					s_sound = 3;
				}
				state_btn_clk2 = 4;
				if(m_pInputManager->m_isLButton == true)
				{
					state_btn_clk2 = 5;
					state_ck = 3;
				}else if(m_pInputManager->m_isLButton == false)
				{
					if(state_ck == 3)
					{
						m_pDSound.Playing(m_psnd.btn0.bff, false);
						
						state_ck = 0;
						state_btn_clk = 2;
					}
				}
			}
		}else s_sound = 0;
	if(m_pSprWnd->stackPoint > 0)
	{
		plus_btn0=
		plus_btn1=
		plus_btn2=
		plus_btn3=
		plus_btn4=9;
		
		if(m_pInputManager->m_MouseXPos >= 240 && m_pInputManager->m_MouseYPos >= 358
			&& m_pInputManager->m_MouseXPos <= 256 && m_pInputManager->m_MouseYPos <= 374)
		{	
			plus_btn0=10;
			if(m_pInputManager->m_isLButton == true)
			{
				plus_btn0= 11; 
				state_ck =  4;
			}else if(m_pInputManager->m_isLButton == false)
			{
				if(state_ck == 4)
				{
					state_ck  = 0;
					plus_btn0 = 9;

					m_pSprWnd->Str++;
					m_pSprWnd->stackPoint--;
				}
			}
		}else if(m_pInputManager->m_MouseXPos >= 240 && m_pInputManager->m_MouseYPos >= 375
			&& m_pInputManager->m_MouseXPos <= 256 && m_pInputManager->m_MouseYPos <= 391)
		{	
			plus_btn1=10;
			if(m_pInputManager->m_isLButton == true)
			{
				plus_btn1= 11; 
				state_ck =  5;
			}else if(m_pInputManager->m_isLButton == false)
			{
				if(state_ck == 5)
				{
					state_ck  = 0;
					plus_btn2 = 9;

					m_pSprWnd->Dex++;
					m_pSprWnd->stackPoint--;
				}
			}
		}else if(m_pInputManager->m_MouseXPos >= 240 && m_pInputManager->m_MouseYPos >= 392
			&& m_pInputManager->m_MouseXPos <= 256 && m_pInputManager->m_MouseYPos <= 408)
		{	
			plus_btn2=10;
			if(m_pInputManager->m_isLButton == true)
			{
				plus_btn2= 11; 
				state_ck =  6;
			}else if(m_pInputManager->m_isLButton == false)
			{
				if(state_ck == 6)
				{
					state_ck  = 0;
					plus_btn2 = 9;

					m_pSprWnd->Int++;
					m_pSprWnd->stackPoint--;
				}
			}
		}else if(m_pInputManager->m_MouseXPos >= 240 && m_pInputManager->m_MouseYPos >= 409
			&& m_pInputManager->m_MouseXPos <= 256 && m_pInputManager->m_MouseYPos <= 424)
		{	
			plus_btn3=10;
			if(m_pInputManager->m_isLButton == true)
			{
				plus_btn3= 11; 
				state_ck =  7;
			}else if(m_pInputManager->m_isLButton == false)
			{
				if(state_ck == 7)
				{
					state_ck  = 0;
					plus_btn3 = 9;

					m_pSprWnd->Hana++;
					m_pSprWnd->totalHp+=10;
					m_pSprWnd->stackPoint--;
				}
			}
		}
		else if(m_pInputManager->m_MouseXPos >= 240 && m_pInputManager->m_MouseYPos >= 425
			&& m_pInputManager->m_MouseXPos <= 256 && m_pInputManager->m_MouseYPos <= 441)
		{	
			plus_btn4=10;
			if(m_pInputManager->m_isLButton == true)
			{
				plus_btn4= 11; 
				state_ck =  8;
			}else if(m_pInputManager->m_isLButton == false)
			{
				if(state_ck == 8)
				{
					state_ck  = 0;
					plus_btn4 = 9;

					m_pSprWnd->Mana++;
					m_pSprWnd->totalMp+=10;
					m_pSprWnd->stackPoint--;
				}
			}
		}
	}
}