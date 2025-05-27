#include "MapOutput.h"
#include "InputManager.h"
#include "Menu.h"
#include "DirectSound.h"
#include "Interface.h"

int click = 0,head = 0,view = 0,sound = 0;

cMENU            *m_pMenu     = NULL;
cStateMessage    *m_pStateMss = NULL;

CInterface::CInterface()
{
	m_pDirect3D = NULL;
	
	m_pInfo     = NULL;
		
	m_pHeadPic  = NULL; 
	m_pPicData  = NULL;
	m_pHpPic    = NULL;
	m_pMpPic    = NULL;
	m_pPointPic = NULL;

	/*itm_info_view =*/ info_view = checked = 0;
}

CInterface::~CInterface()
{
	SAFE_DELETE_(m_pD3dxfont);
	SAFE_DELETE_(m_pStateMss);
	SAFE_DELETE_(m_pInfo);
	SAFE_DELETE_(m_pHeadPic);
	SAFE_DELETE_(m_pPicData);
	SAFE_DELETE_(m_pHpPic);
	SAFE_DELETE_(m_pMpPic);
	SAFE_DELETE_(m_pPointPic);
}

void CInterface::Init(CDirect3D *D3D,cSprWnd *spr)
{
	m_pDirect3D = D3D;

	m_pSprWnd   = spr;

	m_pStateMss = new cStateMessage();
	m_pStateMss->Init(D3D, spr);

	m_pInfo     = new CInfomations();
	m_pInfo->Init(D3D);

	m_pPicData  = new CPicData(D3D);
	m_pPicData->Load("pic/interface.pic");
	m_pPicData->LoadPicTexture();
	
	m_pHpPic    = new CPicData(D3D);
	m_pHpPic->Load("pic/hp.pic");
	m_pHpPic->LoadPicTexture();
	
	m_pMpPic    = new CPicData(D3D);
	m_pMpPic->Load("pic/mp.pic");
	m_pMpPic->LoadPicTexture();
	
	m_pPointPic = new CPicData(D3D);
	m_pPointPic->Load("pic/point.pic");
	m_pPointPic->LoadPicTexture();

	m_pHeadPic = new CPicData(D3D);
	m_pHeadPic->Load("pic/Head.pic");
	m_pHeadPic->LoadPicTexture();
	

	if(spr->Interface_[0].itemnum == 6)//손
		m_pDSound.LoadFile("sound\\hand.wav", &m_psnd.mugi.bff);
	else if(spr->Interface_[0].itemnum == 4)//부메랑
		m_pDSound.LoadFile("sound\\boom.wav", &m_psnd.mugi.bff);
	else if(spr->Interface_[0].itemnum == 2)//칼
		m_pDSound.LoadFile("sound\\nife.wav", &m_psnd.mugi.bff);
	else if(spr->Interface_[0].itemnum == 0)//총
		m_pDSound.LoadFile("sound\\gun.wav", &m_psnd.mugi.bff);

	if(spr->Interface_[7].itemnum == 58)
	{
		spr->m_pPet = new cPetWnd();
		spr->m_pPet->Init(D3D);
		spr->m_pPet->InitPet();
	}

	m_pD3dxfont = NULL;
	m_pD3dxfont = new CD3dxFont();
	if(!m_pD3dxfont->CreateFONT(m_pDirect3D->GetDevice(), (char*)GetStockObject(SYSTEM_FONT), 16))
		return;
}
void
CInterface::Render()
{
	char str[256];

	int hp = 114.0f/((float)m_pSprWnd->totalHp/(float)m_pSprWnd->Hp);
	int mp = 114.0f/((float)m_pSprWnd->totalMp/(float)m_pSprWnd->Mp);
	int po = 114.0f/((float)m_pSprWnd->totalKyunghum/(float)m_pSprWnd->Kyunghum);

	DrawPic(162,504,hp,m_pHpPic);
	DrawPic(162,524,mp,m_pMpPic);
	DrawPic(162,543,po,m_pPointPic);
	DrawPic(0,500,800,m_pPicData);

	if(head == 1 || m_pSprWnd->stackPoint > 0)
	{
		if(m_pSprWnd->stackPoint > 0 && head == 0)
		{
			view++;
			if(view >= 5 && view <= 10)
			{
				DrawPic(1,507,64,m_pHeadPic);
				if(view == 10)view = 0;
			}
		}else
			if(head == 1)
				DrawPic(1,507,64,m_pHeadPic);
	}

	if(m_pSprWnd->Interface_[0].itemnum >= 0)
		m_pStateMss->m_pIpkData->DrawPic(324,518,m_pSprWnd->Interface_[0].itemnum+1);
	if(m_pSprWnd->Interface_[1].itemnum >= 0)
		m_pStateMss->m_pIpkData->DrawPic(399,518,m_pSprWnd->Interface_[1].itemnum);
	if(m_pSprWnd->Interface_[2].itemnum >= 0)
	{
		m_pStateMss->m_pIpkData->DrawPic(510,518,m_pSprWnd->Interface_[2].itemnum+1);
		sprintf(str, "%d", m_pSprWnd->Interface_[2].num);
		m_pD3dxfont->DrawText(510,518,0,0,D3DCOLOR_XRGB(255, 255, 255), str, strlen(str));
	}
	if(m_pSprWnd->Interface_[3].itemnum >= 0)
	{
		m_pStateMss->m_pIpkData->DrawPic(570,518,m_pSprWnd->Interface_[3].itemnum+1);
		sprintf(str, "%d", m_pSprWnd->Interface_[3].num);
		m_pD3dxfont->DrawText(570,518,0,0,D3DCOLOR_XRGB(255, 255, 255), str, strlen(str));
	}
	if(m_pSprWnd->Interface_[4].itemnum >= 0)
	{
		m_pStateMss->m_pIpkData->DrawPic(630,518,m_pSprWnd->Interface_[4].itemnum+1);
		sprintf(str, "%d", m_pSprWnd->Interface_[4].num);
		m_pD3dxfont->DrawText(630,518,0,0,D3DCOLOR_XRGB(255, 255, 255), str, strlen(str));
	}
	if(m_pSprWnd->Interface_[5].itemnum >= 0)
	{
		m_pStateMss->m_pIpkData->DrawPic(690,518,m_pSprWnd->Interface_[5].itemnum+1);
		sprintf(str, "%d", m_pSprWnd->Interface_[5].num);
		m_pD3dxfont->DrawText(690,518,0,0,D3DCOLOR_XRGB(255, 255, 255), str, strlen(str));
	}
	if(m_pSprWnd->Interface_[6].itemnum >= 0)
		m_pStateMss->m_pIpkData->DrawPic(755,520,m_pSprWnd->Interface_[6].itemnum);
	if(m_pSprWnd->Interface_[7].itemnum >= 0)
		m_pStateMss->m_pIpkData->DrawPic(755,540,m_pSprWnd->Interface_[7].itemnum);

    if(!m_pMapViewWnd->user_life && m_pSprWnd->SHOP == FALSE)MousePosition();

	if(m_pSprWnd->SHOP == FALSE)m_pStateMss->Render();
	
	if(info_view == 1)
		m_pInfo->Render();
	else if(info_view == 2)
		m_pStateMss->m_pItemInfo->Render();

	if(m_pMenu)
	{
		int a = m_pMenu->Render();
		if(a == -20)
		{
			SAFE_DELETE_(m_pMenu);
			PostQuitMessage(0);
		}else if(a == -10)
			SAFE_DELETE_(m_pMenu);
	}

	sprintf(str, "%d", m_pSprWnd->Level);
	m_pD3dxfont->DrawText(62,529,0,0,D3DCOLOR_XRGB(255, 255, 0), str, strlen(str));
	sprintf(str, "%d", m_pSprWnd->Hp);
	m_pD3dxfont->DrawText(170,504,0,0,D3DCOLOR_XRGB(255, 255, 255), str, strlen(str));
	sprintf(str, "%d", m_pSprWnd->Mp);
	m_pD3dxfont->DrawText(170,524,0,0,D3DCOLOR_XRGB(255, 255, 255), str, strlen(str));
	sprintf(str, "%d-%d", m_pSprWnd->Kyunghum,m_pSprWnd->totalKyunghum);
	m_pD3dxfont->DrawText(170,543,0,0,D3DCOLOR_XRGB(255, 255, 255), str, strlen(str));
	sprintf(str, "%d(아루)", m_pSprWnd->Money);
	m_pD3dxfont->DrawText(40,550,0,0,D3DCOLOR_XRGB(255, 255, 255), str, strlen(str));
}
void 
CInterface::MousePosition()
{
	info_view     = 0;
	head          = 0;
	//itm_info_view = 0;

	//무기315,10,368,58
    if(m_pInputManager->m_MouseXPos >= 315 && m_pInputManager->m_MouseYPos >= 510
			&& m_pInputManager->m_MouseXPos <= 368 && m_pInputManager->m_MouseYPos <= 558)
	{
		info_view = 2;
		m_pStateMss->m_pItemInfo->LoadingTXT(320,400,m_pSprWnd->Interface_[0].itemnum,m_pSprWnd->Interface_[0].checked,-1);
		if(sound != 1)
		{
			m_pDSound.Playing(m_psnd.btn.bff, false);
			sound = 1;
		}

		if(m_pInputManager->m_isLButton == true)
			click = 1;
		else
		{
			if(click == 1)
			{
				m_pDSound.Playing(m_psnd.btn0.bff, false);
				
				m_pStateMss->click_choice = /*m_pStateMss->choice_click =*/ -1;
				m_pStateMss->scroll_y = 0;
				m_pStateMss->state_num = 0;
				click = 0;
			}
		}
	}else
		if(m_pInputManager->m_MouseXPos >= 385 && m_pInputManager->m_MouseYPos >= 510
			&& m_pInputManager->m_MouseXPos <= 440 && m_pInputManager->m_MouseYPos <= 558)
	{
		info_view = 2;
		if(sound != 2)
		{
			m_pDSound.Playing(m_psnd.btn.bff, false);
			sound = 2;
		}
		if(m_pSprWnd->Interface_[1].itemnum >= 0)
		{
			m_pStateMss->m_pItemInfo->LoadingTXT(390,400,m_pSprWnd->Interface_[1].itemnum,m_pSprWnd->Interface_[1].checked+52,-2);

		}else info_view = 1;

		m_pInfo->Info_p    = 0;
		if(m_pInfo->Rendering == 5)m_pInfo->Info_p = 1;
		m_pInfo->Rendering = 5;
		
		if(m_pInputManager->m_isLButton == true)
			click = 2;
		else
		{
			if(m_pInputManager->m_isRButton == true)
			{
				if(m_pSprWnd->Interface_[1].itemnum >= 0)
				{
					m_pDSound.Playing(m_psnd.out.bff, false);
					
					m_pSprWnd->Interface_[1].itemnum = -1;
				}
			}else
			{
				if(click == 2)
				{
					m_pDSound.Playing(m_psnd.btn0.bff, false);
				
					m_pStateMss->click_choice = /*m_pStateMss->choice_click =*/ -1;
					m_pStateMss->scroll_y = 0;
					m_pStateMss->state_num = 1;
					click = 0;
				}
			}
		}
	}else
		if(m_pInputManager->m_MouseXPos >= 496 && m_pInputManager->m_MouseYPos >= 510
			&& m_pInputManager->m_MouseXPos <= 550 && m_pInputManager->m_MouseYPos <= 558)
	{
		info_view = 2;
		if(sound != 3)
		{
			m_pDSound.Playing(m_psnd.btn.bff, false);
			sound = 3;
		}
		if(m_pSprWnd->Interface_[2].itemnum >= 0)
		{
			m_pStateMss->m_pItemInfo->LoadingTXT(501,400,m_pSprWnd->Interface_[2].itemnum,m_pSprWnd->Interface_[2].checked+8,-3);

		}else info_view = 1;

		m_pInfo->Info_p    = 0;
		if(m_pInfo->Rendering == 6)m_pInfo->Info_p = 1;
		m_pInfo->Rendering = 6;

		if(m_pInputManager->m_isLButton == true)
			click = 3;
		else
		{
			if(m_pInputManager->m_isRButton == true)
			{
				if(m_pSprWnd->Interface_[2].itemnum >= 0)
				{
					m_pDSound.Playing(m_psnd.out.bff, false);
					
					for(int p=0;p<30;p++)
					{
						if(m_pSprWnd->User_Itm_Etc[p].itemnum == -1)
						{
							m_pSprWnd->User_Itm_Etc[p].itemnum = 
								m_pSprWnd->Interface_[2].itemnum;
							m_pSprWnd->User_Itm_Etc[p].checked = 
								m_pSprWnd->Interface_[2].checked;
							m_pSprWnd->User_Itm_Etc[p].num = 
								m_pSprWnd->Interface_[2].num;

							m_pSprWnd->Interface_[2].itemnum = -1;
							break;
						}
					}
				}
			}else
			{
				if(click == 3)
				{
					m_pDSound.Playing(m_psnd.btn0.bff, false);
					
					m_pStateMss->click_choice = /*m_pStateMss->choice_click =*/ -1;
					m_pStateMss->scroll_y = 0;
					m_pStateMss->state_num = 2;
					click = 0;
				}
			}
		}
	}else
		if(m_pInputManager->m_MouseXPos >= 560 && m_pInputManager->m_MouseYPos >= 510
			&& m_pInputManager->m_MouseXPos <= 614 && m_pInputManager->m_MouseYPos <= 558)
	{
		info_view = 2;
		if(sound != 4)
		{
			m_pDSound.Playing(m_psnd.btn.bff, false);
			sound = 4;
		}
		if(m_pSprWnd->Interface_[3].itemnum >= 0)
		{
			m_pStateMss->m_pItemInfo->LoadingTXT(565,400,m_pSprWnd->Interface_[3].itemnum,m_pSprWnd->Interface_[3].checked+8,-3);

		}else info_view = 1;

		m_pInfo->Info_p    = 0;
		if(m_pInfo->Rendering == 7)m_pInfo->Info_p = 1;
		m_pInfo->Rendering = 7;
		
		if(m_pInputManager->m_isLButton == true)
			click = 4;
		else
		{
			if(m_pInputManager->m_isRButton == true)
			{
				if(m_pSprWnd->Interface_[3].itemnum >= 0)
				{
					m_pDSound.Playing(m_psnd.out.bff, false);
					
					for(int p=0;p<30;p++)
					{
						if(m_pSprWnd->User_Itm_Etc[p].itemnum == -1)
						{
							m_pSprWnd->User_Itm_Etc[p].itemnum = 
								m_pSprWnd->Interface_[3].itemnum;
							m_pSprWnd->User_Itm_Etc[p].checked = 
								m_pSprWnd->Interface_[3].checked;
							m_pSprWnd->User_Itm_Etc[p].num = 
								m_pSprWnd->Interface_[3].num;

							m_pSprWnd->Interface_[3].itemnum = -1;
							break;
						}
					}
				}
			}else
			{
				if(click == 4)
				{
					m_pDSound.Playing(m_psnd.btn0.bff, false);
				
					m_pStateMss->click_choice = /*m_pStateMss->choice_click =*/ -1;
					m_pStateMss->scroll_y = 0;
					m_pStateMss->state_num = 3;
					click = 0;
				}
			}
		}
	}else
		if(m_pInputManager->m_MouseXPos >= 617 && m_pInputManager->m_MouseYPos >= 510
			&& m_pInputManager->m_MouseXPos <= 674 && m_pInputManager->m_MouseYPos <= 558)
	{
		info_view = 2;
		if(sound != 5)
		{
			m_pDSound.Playing(m_psnd.btn.bff, false);
			sound = 5;
		}
		if(m_pSprWnd->Interface_[4].itemnum >= 0)
		{
			m_pStateMss->m_pItemInfo->LoadingTXT(565,400,m_pSprWnd->Interface_[4].itemnum,m_pSprWnd->Interface_[4].checked+8,-3);

		}else info_view = 1;

		m_pInfo->Info_p    = 0;
		if(m_pInfo->Rendering == 8)m_pInfo->Info_p = 1;
		m_pInfo->Rendering = 8;
		
		if(m_pInputManager->m_isLButton == true)
			click = 5;
		else
		{
			if(m_pInputManager->m_isRButton == true)
			{
				if(m_pSprWnd->Interface_[4].itemnum >= 0)
				{
					m_pDSound.Playing(m_psnd.out.bff, false);
					
					for(int p=0;p<30;p++)
					{
						if(m_pSprWnd->User_Itm_Etc[p].itemnum == -1)
						{
							m_pSprWnd->User_Itm_Etc[p].itemnum = 
								m_pSprWnd->Interface_[4].itemnum;
							m_pSprWnd->User_Itm_Etc[p].checked = 
								m_pSprWnd->Interface_[4].checked;
							m_pSprWnd->User_Itm_Etc[p].num = 
								m_pSprWnd->Interface_[4].num;

							m_pSprWnd->Interface_[4].itemnum = -1;
							break;
						}
					}
				}
			}else
			{
				if(click == 5)
				{
					m_pDSound.Playing(m_psnd.btn0.bff, false);
				
					m_pStateMss->click_choice = /*m_pStateMss->choice_click =*/ -1;
					m_pStateMss->scroll_y = 0;
					m_pStateMss->state_num = 4;
					click = 0;
				}
			}
		}
	}else
		if(m_pInputManager->m_MouseXPos >= 679 && m_pInputManager->m_MouseYPos >= 510
			&& m_pInputManager->m_MouseXPos <= 730 && m_pInputManager->m_MouseYPos <= 558)
	{
		info_view = 2;
		if(sound != 6)
		{
			m_pDSound.Playing(m_psnd.btn.bff, false);
			sound = 6;
		}
		if(m_pSprWnd->Interface_[5].itemnum >= 0)
		{
			m_pStateMss->m_pItemInfo->LoadingTXT(565,400,m_pSprWnd->Interface_[5].itemnum,m_pSprWnd->Interface_[5].checked+8,-3);

		}else info_view = 1;

		m_pInfo->Info_p    = 0;
		if(m_pInfo->Rendering == 9)m_pInfo->Info_p = 1;
		m_pInfo->Rendering = 9;
	
		if(m_pInputManager->m_isLButton == true)
			click = 6;
		else
		{
			if(m_pInputManager->m_isRButton == true)
			{
				if(m_pSprWnd->Interface_[5].itemnum >= 0)
				{
					m_pDSound.Playing(m_psnd.out.bff, false);
					
					for(int p=0;p<30;p++)
					{
						if(m_pSprWnd->User_Itm_Etc[p].itemnum == -1)
						{
							m_pSprWnd->User_Itm_Etc[p].itemnum = 
								m_pSprWnd->Interface_[5].itemnum;
							m_pSprWnd->User_Itm_Etc[p].checked = 
								m_pSprWnd->Interface_[5].checked;
							m_pSprWnd->User_Itm_Etc[p].num = 
								m_pSprWnd->Interface_[5].num;
							
							m_pSprWnd->Interface_[5].itemnum = -1;
							break;
						}
					}
				}
			}else
			{
				if(click == 6)
				{
					m_pDSound.Playing(m_psnd.btn0.bff, false);
				
					m_pStateMss->click_choice = /*m_pStateMss->choice_click =*/ -1;
					m_pStateMss->scroll_y = 0;
					m_pStateMss->state_num = 5;
					click = 0;
				}
			}
		}
	}else//5,507,47,249
		if(m_pInputManager->m_MouseXPos >= 5 && m_pInputManager->m_MouseYPos >= 507
			&& m_pInputManager->m_MouseXPos <= 47 && m_pInputManager->m_MouseYPos <= 549)
	{
		if(!m_pMenu)
		{
			head              = 1;
			info_view          = 1;
			m_pInfo->Info_p    = 0;
			if(m_pInfo->Rendering == 0)m_pInfo->Info_p = 1;
			m_pInfo->Rendering = 0;

			if(sound != 7)
			{
				m_pDSound.Playing(m_psnd.btn.bff, false);
				sound = 7;
			}

			if(m_pInputManager->m_isLButton == true)
				click = 7;
			else
			{
				if(click == 7)
				{
					m_pDSound.Playing(m_psnd.btn0.bff, false);
					
					m_pMenu = new cMENU();
					m_pMenu->Init(m_pDirect3D,m_pSprWnd);
					click = 0; 
					view = 0;
				}
			}
		}
	}else sound = 0;
	//아이템 키 조작..............................................
	if(m_pInputManager->m_isDF1 == true)
	{
		m_pInputManager->m_isUF1 = true;
	}else if(m_pInputManager->m_isUF1 == true)
	{
		if(m_pSprWnd->Interface_[2].itemnum >= 0)
		{
			if(m_pSprWnd->Interface_[2].itemnum < 10000)
			{
				m_pDSound.Playing(m_psnd.eat_e.bff, false);
				
				m_pSprWnd->Interface_[2].num--;
				if(m_pSprWnd->Interface_[2].num<=0)
					m_pSprWnd->Interface_[2].itemnum = -1;

				m_pSprWnd->Hp+=H_Item[m_pSprWnd->Interface_[2].checked].hp;
				m_pSprWnd->Mp+=H_Item[m_pSprWnd->Interface_[2].checked].mp;
	
				if(m_pSprWnd->Hp>m_pSprWnd->totalHp)m_pSprWnd->Hp=m_pSprWnd->totalHp;
				if(m_pSprWnd->Mp>m_pSprWnd->totalMp)m_pSprWnd->Mp=m_pSprWnd->totalMp;
			}
		}
		m_pInputManager->m_isUF1 = false;
	}
	
	if(m_pInputManager->m_isDF2 == true)
	{
		m_pInputManager->m_isUF2 = true;
	}else if(m_pInputManager->m_isUF2 == true)
	{
		if(m_pSprWnd->Interface_[3].itemnum >= 0)
		{
			if(m_pSprWnd->Interface_[3].itemnum < 10000)
			{
				m_pDSound.Playing(m_psnd.eat_e.bff, false);
				
				m_pSprWnd->Interface_[3].num--;
				if(m_pSprWnd->Interface_[3].num<=0)
					m_pSprWnd->Interface_[3].itemnum = -1;

				m_pSprWnd->Hp+=H_Item[m_pSprWnd->Interface_[3].checked].hp;
				m_pSprWnd->Mp+=H_Item[m_pSprWnd->Interface_[3].checked].mp;
	
				if(m_pSprWnd->Hp>m_pSprWnd->totalHp)m_pSprWnd->Hp=m_pSprWnd->totalHp;
				if(m_pSprWnd->Mp>m_pSprWnd->totalMp)m_pSprWnd->Mp=m_pSprWnd->totalMp;
			}
		}
		m_pInputManager->m_isUF2 = false;
	}

	if(m_pInputManager->m_isDF3 == true)
	{
		m_pInputManager->m_isUF3 = true;
	}else if(m_pInputManager->m_isUF3 == true)
	{
		if(m_pSprWnd->Interface_[4].itemnum >= 0)
		{
			if(m_pSprWnd->Interface_[4].itemnum < 10000)
			{
				m_pDSound.Playing(m_psnd.eat_e.bff, false);
				
				m_pSprWnd->Interface_[4].num--;
				if(m_pSprWnd->Interface_[4].num<=0)
					m_pSprWnd->Interface_[4].itemnum = -1;

				m_pSprWnd->Hp+=H_Item[m_pSprWnd->Interface_[4].checked].hp;
				m_pSprWnd->Mp+=H_Item[m_pSprWnd->Interface_[4].checked].mp;
	
				if(m_pSprWnd->Hp>m_pSprWnd->totalHp)m_pSprWnd->Hp=m_pSprWnd->totalHp;
				if(m_pSprWnd->Mp>m_pSprWnd->totalMp)m_pSprWnd->Mp=m_pSprWnd->totalMp;
			}
		}
		m_pInputManager->m_isUF3 = false;
	}
	
	if(m_pInputManager->m_isDF4 == true)
	{
		m_pInputManager->m_isUF4 = true;
	}else if(m_pInputManager->m_isUF4 == true)
	{
		if(m_pSprWnd->Interface_[5].itemnum >= 0)
		{
			if(m_pSprWnd->Interface_[5].itemnum < 10000)
			{
				m_pDSound.Playing(m_psnd.eat_e.bff, false);
				
				m_pSprWnd->Interface_[5].num--;
				if(m_pSprWnd->Interface_[5].num<=0)
					m_pSprWnd->Interface_[5].itemnum = -1;

				m_pSprWnd->Hp+=H_Item[m_pSprWnd->Interface_[5].checked].hp;
				m_pSprWnd->Mp+=H_Item[m_pSprWnd->Interface_[5].checked].mp;
	
				if(m_pSprWnd->Hp>m_pSprWnd->totalHp)m_pSprWnd->Hp=m_pSprWnd->totalHp;
				if(m_pSprWnd->Mp>m_pSprWnd->totalMp)m_pSprWnd->Mp=m_pSprWnd->totalMp;
			}
		}
		m_pInputManager->m_isUF4 = false;
	}
}
void
CInterface::DrawPic(int x,int y,int xsize, CPicData *Pic)
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
	float	fHeight = (float)Pic->h_Height;

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
CInterface::DelEff()
{
	m_pStateMss->m_pEcffect->AllEffectDelete();
}