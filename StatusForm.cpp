#include "MapOutput.h"
#include "InputManager.h"
#include "Help.h"
#include "MessageBox.h"
#include "DirectSound.h"
#include "StatusForm.h"

int btn_0 = 0,btn_1 = 3,btn_2 = 6;
int btn_click =0;

char ss_sound = 0;

CMessageBox *m_pM_Box_S   = NULL;

cStateMessage::cStateMessage()
{
	m_pDirect3D = NULL;
	m_pIpkData  = NULL;
	m_pEcffect  = NULL;

	m_pChoiceData      = NULL;
	for(int p=0;p<3;p++)
		m_pPicData[p]  = NULL;
	for(int p=0;p<9;p++)
		m_pBtnData[p]  = NULL;

	click_choice = choice_click = state_num = -1;
	info_item = botton_show = scroll_y = 0;
}

cStateMessage::~cStateMessage()
{
	SAFE_DELETE_(m_pD3dxfont);
	SAFE_DELETE_(m_pIpkData);
	SAFE_DELETE_(m_pEcffect);
}

void cStateMessage::Init(CDirect3D *D3D, cSprWnd *spr)
{
	char buf[256];

	m_pDirect3D = D3D;
	m_pSprWnd   = spr;

	m_pItemInfo = new CItemInfomations();
	m_pItemInfo->Init(D3D);

	m_pIpkData = new CIpkData(D3D);
	if(m_pIpkData->Load("itm/itm.ipk") == FALSE)
		exit(0);
	m_pIpkData->LoadPicTexture();

	m_pChoiceData = new CPicData(D3D);
	m_pChoiceData->Load("pic/choice.pic");
	m_pChoiceData->LoadPicTexture();
	
	for(int p=0;p<3;p++)
	{
		sprintf(buf,"pic/att%d.pic",p);
		m_pPicData[p] = new CPicData(D3D);
		m_pPicData[p]->Load(buf);
		m_pPicData[p]->LoadPicTexture();
	}
	for(int p=0;p<9;p++)
	{
		sprintf(buf,"pic/ms_btn_%d.pic",p);
		m_pBtnData[p] = new CPicData(D3D);
		m_pBtnData[p]->Load(buf);
		m_pBtnData[p]->LoadPicTexture();
	}
	m_pEcffect = new cEcffectWnd();
	m_pEcffect->Init(m_pDirect3D);
	
	m_pD3dxfont = NULL;
	m_pD3dxfont = new CD3dxFont();
	if(!m_pD3dxfont->CreateFONT(m_pDirect3D->GetDevice(), (char*)GetStockObject(SYSTEM_FONT), 8))
		return;
}
void
cStateMessage::Render()
{
	char str[256]={'\0',};
	int x,y=250;

	if(state_num == 0)
	{
		x = 255;
		botton_show = 0;
	
		DrawPic(x,y,0);
		for(int p=0;p<5;p++)
		{
			if(m_pSprWnd->User_Itm_Att[p+scroll_y].itemnum>=0)
			{
				m_pIpkData->DrawPic(x+15,305+(p*32),m_pSprWnd->User_Itm_Att[p+scroll_y].itemnum);
				sprintf(str,"%s",A_Item[m_pSprWnd->User_Itm_Att[p+scroll_y].checked].itmname);
				m_pD3dxfont->DrawText(x+45,315+(p*32),0,0,0xff000000,str,strlen(str));
		
				if(click_choice == p)
				{
					m_pD3dxfont->DrawText(x+45,315+(p*32),0,0,0xffff0000,str,strlen(str));
					m_pDirect3D->AlphaBlendEnable(70);
					DrawChoicePic(x,305+(p*32));
					m_pDirect3D->AlphaBlendDisable();
				}
			}
		}
		DrawBtnPic(x+83,274,btn_0);
		DrawBtnPic(x+10,470,btn_1);
		DrawBtnPic(x+26,470,btn_2);
	}
	else if(state_num == 1)
	{
		x = 330;
		botton_show = 1;

		DrawPic(x,y,1);
		for(int p=0;p<5;p++)
		{
			if(m_pSprWnd->User_Itm_Booster[p+scroll_y].itemnum>=0)
			{
				m_pIpkData->DrawPic(x+15,305+(p*32),m_pSprWnd->User_Itm_Booster[p+scroll_y].itemnum);
				sprintf(str,"%s",S_Item[m_pSprWnd->User_Itm_Booster[p+scroll_y].checked].itmname);
				m_pD3dxfont->DrawText(x+45,315+(p*32),0,0,0xff000000,str,strlen(str));
		
				if(click_choice == p)
				{
					m_pD3dxfont->DrawText(x+45,315+(p*32),0,0,0xffff0000,str,strlen(str));
					m_pDirect3D->AlphaBlendEnable(70);
					DrawChoicePic(x,305+(p*32));
					m_pDirect3D->AlphaBlendDisable();
				}
			}
		}
		DrawBtnPic(x+83,274,btn_0);
	}
	else if(state_num == 2)
	{
		x = 440;
		botton_show = 0;

		DrawPic(x,y,2);

		for(int p=0;p<5;p++)
		{
			if(m_pSprWnd->User_Itm_Etc[p+scroll_y].itemnum>=0)
			{
				m_pIpkData->DrawPic(x+15,305+(p*32),m_pSprWnd->User_Itm_Etc[p+scroll_y].itemnum);
				sprintf(str,"(%d)",m_pSprWnd->User_Itm_Etc[p+scroll_y].num);
				m_pD3dxfont->DrawText(x+50,325+(p*32),0,0,0xff000000,str,strlen(str));
				sprintf(str,"%s",H_Item[m_pSprWnd->User_Itm_Etc[p+scroll_y].checked].itmname);
				m_pD3dxfont->DrawText(x+45,315+(p*32),0,0,0xff000000,str,strlen(str));
		
				if(click_choice == p)
				{
					m_pD3dxfont->DrawText(x+45,315+(p*32),0,0,0xffff0000,str,strlen(str));
					m_pDirect3D->AlphaBlendEnable(70);
					DrawChoicePic(x,305+(p*32));
					m_pDirect3D->AlphaBlendDisable();
				}
			}
		}
		DrawBtnPic(x+83,274,btn_0);
		DrawBtnPic(x+10,470,btn_1);
		DrawBtnPic(x+26,470,btn_2);
	}
	else if(state_num == 3)
	{
		x = 503;
		botton_show = 0;

		DrawPic(x,y,2);
		for(int p=0;p<5;p++)
		{
			if(m_pSprWnd->User_Itm_Etc[p+scroll_y].itemnum>=0)
			{
				m_pIpkData->DrawPic(x+15,305+(p*32),m_pSprWnd->User_Itm_Etc[p+scroll_y].itemnum);
				sprintf(str,"(%d)",m_pSprWnd->User_Itm_Etc[p+scroll_y].num);
				m_pD3dxfont->DrawText(x+50,325+(p*32),0,0,0xff000000,str,strlen(str));
				sprintf(str,"%s",H_Item[m_pSprWnd->User_Itm_Etc[p+scroll_y].checked].itmname);
				m_pD3dxfont->DrawText(x+45,315+(p*32),0,0,0xff000000,str,strlen(str));
				if(click_choice == p)
				{
					m_pD3dxfont->DrawText(x+45,315+(p*32),0,0,0xffff0000,str,strlen(str));
					m_pDirect3D->AlphaBlendEnable(70);
					DrawChoicePic(x,305+(p*32));
					m_pDirect3D->AlphaBlendDisable();
				}
			}
		}
		DrawBtnPic(x+83,274,btn_0);
		DrawBtnPic(x+10,470,btn_1);
		DrawBtnPic(x+26,470,btn_2);
	}
	else if(state_num == 4)
	{
		x = 563;
		botton_show = 0;

		DrawPic(563,y,2);
		for(int p=0;p<5;p++)
		{
			if(m_pSprWnd->User_Itm_Etc[p+scroll_y].itemnum>=0)
			{
				m_pIpkData->DrawPic(x+15,305+(p*32),m_pSprWnd->User_Itm_Etc[p+scroll_y].itemnum);
				sprintf(str,"(%d)",m_pSprWnd->User_Itm_Etc[p+scroll_y].num);
				m_pD3dxfont->DrawText(x+50,325+(p*32),0,0,0xff000000,str,strlen(str));
				sprintf(str,"%s",H_Item[m_pSprWnd->User_Itm_Etc[p+scroll_y].checked].itmname);
				m_pD3dxfont->DrawText(x+45,315+(p*32),0,0,0xff000000,str,strlen(str));
				if(click_choice == p)
				{
					m_pD3dxfont->DrawText(x+45,315+(p*32),0,0,0xffff0000,str,strlen(str));
					m_pDirect3D->AlphaBlendEnable(70);
					DrawChoicePic(x,305+(p*32));
					m_pDirect3D->AlphaBlendDisable();
				}
			}
		}
		DrawBtnPic(x+83,274,btn_0);
		DrawBtnPic(x+10,470,btn_1);
		DrawBtnPic(x+26,470,btn_2);
	}
	else if(state_num == 5)
	{
		x = 623;
		botton_show = 0;

		DrawPic(623,y,2);
		for(int p=0;p<5;p++)
		{
			if(m_pSprWnd->User_Itm_Etc[p+scroll_y].itemnum>=0)
			{
				m_pIpkData->DrawPic(x+15,305+(p*32),m_pSprWnd->User_Itm_Etc[p+scroll_y].itemnum);
				sprintf(str,"(%d)",m_pSprWnd->User_Itm_Etc[p+scroll_y].num);
				m_pD3dxfont->DrawText(x+50,325+(p*32),0,0,0xff000000,str,strlen(str));
				sprintf(str,"%s",H_Item[m_pSprWnd->User_Itm_Etc[p+scroll_y].checked].itmname);
				m_pD3dxfont->DrawText(x+45,315+(p*32),0,0,0xff000000,str,strlen(str));
				if(click_choice == p)
				{
					m_pD3dxfont->DrawText(x+45,315+(p*32),0,0,0xffff0000,str,strlen(str));
					m_pDirect3D->AlphaBlendEnable(70);
					DrawChoicePic(x,305+(p*32));
					m_pDirect3D->AlphaBlendDisable();
				}
			}
		}
		DrawBtnPic(x+83,274,btn_0);
		DrawBtnPic(x+10,470,btn_1);
		DrawBtnPic(x+26,470,btn_2);
	}

	if(info_item == 1)
	{
		m_pStateMss->m_pItemInfo->Render();
	}
	
	if(m_pM_Box_S)
	{
		int a =	m_pM_Box_S->Render();
		if(a == -10)
		{
			SAFE_DELETE_(m_pM_Box_S);
		}
		else if(a>0)
		{
			m_pEcffect->CreateItem(m_pSprWnd->spr_x,m_pSprWnd->spr_y,
				m_pSprWnd->User_Itm_Etc[choice_click+scroll_y].itemnum,a,
					m_pSprWnd->User_Itm_Etc[choice_click+scroll_y].checked,-3);
			
			m_pStateMss->m_pSprWnd->User_Itm_Etc[choice_click+scroll_y].num-=a;
			if(m_pStateMss->m_pSprWnd->User_Itm_Etc[choice_click+scroll_y].num<=0)
				m_pStateMss->m_pSprWnd->User_Itm_Etc[choice_click+scroll_y].itemnum = -1;
			
			choice_click = -1;
			SAFE_DELETE_(m_pM_Box_S);
		}
	}

	if(state_num >= 0)MousePosition(x,y);
}
void
cStateMessage::DrawChoicePic(int x,int y)
{

    //	Sprite¿¡ »ç¿ëµÉ ½¦ÀÌ´õ ¼³Á¤
	m_pDirect3D->GetDevice()->SetTexture( 0, 
									 m_pChoiceData->g_pPicTexture);
			
	CUSTOM_VERTEX*	pV = NULL;

	m_pMapViewWnd->g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

	for( int m=0; m < 4; m++ )
	{
      pV[m].vPos.z = 1.0f;
	  pV[m].rhw = 1.0f;
	  pV[m].dwDiffuse = 0xffffffff;
	}
		

	float	fLeft   = (float)x;
	float	fWidth  = (float)m_pChoiceData->w_Width;
	float	fTop    = (float)y;
	float	fHeight = (float)m_pChoiceData->h_Height;

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
cStateMessage::DrawPic(int x,int y,int num)
{

    //	Sprite¿¡ »ç¿ëµÉ ½¦ÀÌ´õ ¼³Á¤
	m_pDirect3D->GetDevice()->SetTexture( 0, 
									 m_pPicData[num]->g_pPicTexture);
			
	CUSTOM_VERTEX*	pV = NULL;

	m_pMapViewWnd->g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

	for( int m=0; m < 4; m++ )
	{
      pV[m].vPos.z = 1.0f;
	  pV[m].rhw = 1.0f;
	  pV[m].dwDiffuse = 0xffffffff;
	}
		

	float	fLeft   = (float)x;
	float	fWidth  = (float)m_pPicData[num]->w_Width;
	float	fTop    = (float)y;
	float	fHeight = (float)m_pPicData[num]->h_Height;

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
cStateMessage::DrawBtnPic(int x,int y,int num)
{

    //	Sprite¿¡ »ç¿ëµÉ ½¦ÀÌ´õ ¼³Á¤
	m_pDirect3D->GetDevice()->SetTexture( 0, 
									 m_pBtnData[num]->g_pPicTexture);
			
	CUSTOM_VERTEX*	pV = NULL;

	m_pMapViewWnd->g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

	for( int m=0; m < 4; m++ )
	{
      pV[m].vPos.z = 1.0f;
	  pV[m].rhw = 1.0f;
	  pV[m].dwDiffuse = 0xffffffff;
	}
		

	float	fLeft   = (float)x;
	float	fWidth  = (float)m_pBtnData[num]->w_Width;
	float	fTop    = (float)y;
	float	fHeight = (float)m_pBtnData[num]->h_Height;

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
cStateMessage::MousePosition(int x,int y)
{
	//double_click = 0;
	int itemnum,num,itemck;

	btn_0        = 0;
	btn_1        = 3;
	btn_2        = 6; 
	
	info_item = 0;

	if(m_pInputManager->m_MouseXPos >= (x+83) && m_pInputManager->m_MouseYPos >= (y+24)
		&& m_pInputManager->m_MouseXPos <= (x+99) && m_pInputManager->m_MouseYPos <= (y+40))
	{
		btn_0 = 1;
		if(ss_sound != 1)
		{
			m_pDSound.Playing(m_psnd.btn.bff, false);
			ss_sound = 1;
		}
		if(m_pInputManager->m_isLButton == true)
		{
            btn_click = 1;
			btn_0 = 2;
		}
		else
		{
			if(btn_click == 1)
			{
				m_pDSound.Playing(m_psnd.btn0.bff, false);

				info_item = btn_click = 0;
				state_num = -1;
			}
		}
	}else
	if(m_pInputManager->m_MouseXPos >= (x+10) && m_pInputManager->m_MouseYPos >= (y+220)
		&& m_pInputManager->m_MouseXPos <= (x+25) && m_pInputManager->m_MouseYPos <= (y+236))
	{
		if(botton_show == 0)
		{
			if(ss_sound != 2)
			{
				m_pDSound.Playing(m_psnd.btn.bff, false);
				ss_sound = 2;
			}
			btn_1 = 4;
			if(m_pInputManager->m_isLButton == true)
			{
		        btn_click = 1;
				btn_1 = 5;
			}
			else
			{
				if(btn_click == 1)
				{
					btn_click    = 0;
					click_choice = -1;
					scroll_y--;
					if(scroll_y < 0)
					{
						m_pDSound.Playing(m_psnd.no.bff, false);
						scroll_y = 0;
					}else 
						m_pDSound.Playing(m_psnd.btn0.bff, false);
				}
			}
		}
	}else
		//info_item = 1;
		//m_pStateMss->m_pItemInfo->LoadingTXT(shop_x+120,shop_y+182,A_Item[pst[shop_itm_list+4]].itmnum,A_Item[pst[shop_itm_list+4]].checked,itm_view);
	if(m_pInputManager->m_MouseXPos >= (x+26) && m_pInputManager->m_MouseYPos >= (y+220)
		&& m_pInputManager->m_MouseXPos <= (x+41) && m_pInputManager->m_MouseYPos <= (y+236))
	{
		if(botton_show == 0)
		{
			if(ss_sound != 3)
			{
				m_pDSound.Playing(m_psnd.btn.bff, false);
				ss_sound = 3;
			}
			btn_2 = 7;
			if(m_pInputManager->m_isLButton == true)
			{
		        btn_click = 1;
				btn_2 = 8;
			}
			else
			{
				if(btn_click == 1)
				{
					btn_click    = 0;
					click_choice = -1; 
					scroll_y++;
					if(state_num == 0)
					{
						if(scroll_y > 5)
						{
							m_pDSound.Playing(m_psnd.no.bff, false);
							scroll_y = 5;
						}else
							m_pDSound.Playing(m_psnd.btn0.bff, false);
					}
					else if(state_num > 1)
					{
						if(scroll_y > 25)
						{
							m_pDSound.Playing(m_psnd.no.bff, false);
							scroll_y = 25;
						}else
							m_pDSound.Playing(m_psnd.btn0.bff, false);
					}
				}
			}
		}
	}
	else
	{
		if(m_pInputManager->m_isLButton == true)
			btn_click = 0;

		ss_sound = 0;
	}

	if(m_pInputManager->m_MouseXPos >= (x+15) && m_pInputManager->m_MouseXPos <= (x+47))
	{
		if(m_pInputManager->m_MouseYPos >= (y+52) && m_pInputManager->m_MouseYPos <= (y+86))
		{
			info_item = 1;
			if(state_num == 0)
			{
				if(m_pSprWnd->User_Itm_Att[scroll_y].itemnum>=0)
					m_pItemInfo->LoadingTXT(x+100,y+182,A_Item[m_pSprWnd->User_Itm_Att[scroll_y].checked].itmnum,A_Item[m_pSprWnd->User_Itm_Att[scroll_y].checked].checked,state_num-1);
				else info_item = 0;
			}else if(state_num == 1)
			{
				if(m_pSprWnd->User_Itm_Booster[scroll_y].itemnum>=0)
					m_pItemInfo->LoadingTXT(x+100,y+182,S_Item[m_pSprWnd->User_Itm_Booster[scroll_y].checked].itmnum,S_Item[m_pSprWnd->User_Itm_Booster[scroll_y].checked].checked+52,-2);
				else info_item = 0;
			}else if(state_num > 1)
			{
				if(m_pSprWnd->User_Itm_Etc[scroll_y].itemnum>=0)
					m_pItemInfo->LoadingTXT(x+100,y+182,H_Item[m_pSprWnd->User_Itm_Etc[scroll_y].checked].itmnum,H_Item[m_pSprWnd->User_Itm_Etc[scroll_y].checked].checked+8,-3);
				else info_item = 0;
			}
			if(m_pInputManager->m_isLButton == true)
			{
				if(click_choice != 0)
					m_pDSound.Playing(m_psnd.btn1.bff, false);
				click_choice = 0;
			}
			else
			{
                if(click_choice == 0)
				{
					//choice_click = click_choice;
					
					if(m_pInputManager->m_isRButton == true)
					{
						itemnum = m_pSprWnd->Interface_[state_num].itemnum;
						num     = m_pSprWnd->Interface_[state_num].num;
						itemck  = m_pSprWnd->Interface_[state_num].checked;
						
						if(state_num == 0)
						{
							if(m_pSprWnd->User_Itm_Att[click_choice+scroll_y].itemnum > -1)
							{
								if(A_Item[m_pSprWnd->User_Itm_Att[click_choice+scroll_y].checked].level
									> m_pSprWnd->Level)
								{
									click_choice = /*choice_click =*/ -1;
									m_pHelp->CreateHelp("Âø¿ëÇÒ ·¹º§ÀÌ ¾Æ´Õ´Ï´Ù.");
									return;
								}
								m_pDSound.Playing(m_psnd.btn2.bff, false);
							
								m_pSprWnd->Interface_[state_num].itemnum = 
									m_pSprWnd->User_Itm_Att[click_choice+scroll_y].itemnum;
								m_pSprWnd->Interface_[state_num].num     = 
									m_pSprWnd->User_Itm_Att[click_choice+scroll_y].num;
								m_pSprWnd->Interface_[state_num].checked = 
									m_pSprWnd->User_Itm_Att[click_choice+scroll_y].checked;

								m_pSprWnd->User_Itm_Att[click_choice+scroll_y].itemnum
									= itemnum;
								m_pSprWnd->User_Itm_Att[click_choice+scroll_y].num = num;
								m_pSprWnd->User_Itm_Att[click_choice+scroll_y].checked
									= itemck;
					
								m_pSprWnd->mis = 0;
								click_choice = /*choice_click =*/ -1;
							
								if(m_pSprWnd->Interface_[state_num].itemnum == 6)//¼Õ
									m_pDSound.LoadFile("sound\\hand.wav", &m_psnd.mugi.bff);
								else if(m_pSprWnd->Interface_[state_num].itemnum == 4)//ºÎ¸Þ¶û
									m_pDSound.LoadFile("sound\\boom.wav", &m_psnd.mugi.bff);
								else if(m_pSprWnd->Interface_[state_num].itemnum == 2)//Ä®
									m_pDSound.LoadFile("sound\\nife.wav", &m_psnd.mugi.bff);
								else if(m_pSprWnd->Interface_[state_num].itemnum == 0)//ÃÑ
									m_pDSound.LoadFile("sound\\gun.wav", &m_psnd.mugi.bff);
							}
						}else if(state_num == 1)
						{
							if(m_pSprWnd->User_Itm_Booster[click_choice+scroll_y].itemnum > -1)
							{
								m_pDSound.Playing(m_psnd.btn2.bff, false);
							
								m_pSprWnd->Interface_[state_num].itemnum = 
									m_pSprWnd->User_Itm_Booster[click_choice+scroll_y].itemnum;
								m_pSprWnd->Interface_[state_num].num     = 
									m_pSprWnd->User_Itm_Booster[click_choice+scroll_y].num;
								m_pSprWnd->Interface_[state_num].checked = 
									m_pSprWnd->User_Itm_Booster[click_choice+scroll_y].checked;
							}
						}else if(state_num > 1)
						{
							m_pDSound.Playing(m_psnd.btn2.bff, false);
						
							m_pSprWnd->Interface_[state_num].itemnum = 
								m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].itemnum;
							m_pSprWnd->Interface_[state_num].num     = 
								m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].num;
							m_pSprWnd->Interface_[state_num].checked = 
								m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].checked;

							m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].itemnum
								= itemnum;
							m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].num = num;
							m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].checked
								= itemck;
							
							click_choice = /*choice_click =*/ -1;
						}
					}
				}
			}
		}else if(m_pInputManager->m_MouseYPos >= (y+87) && m_pInputManager->m_MouseYPos <= (y+118))
		{
			info_item = 1;
			if(state_num == 0)
			{
				if(m_pSprWnd->User_Itm_Att[scroll_y+1].itemnum>=0)
					m_pItemInfo->LoadingTXT(x+100,y+182,A_Item[m_pSprWnd->User_Itm_Att[scroll_y+1].checked].itmnum,A_Item[m_pSprWnd->User_Itm_Att[scroll_y+1].checked].checked,state_num-1);
				else info_item = 0;
			}else if(state_num == 1)
			{
				if(m_pSprWnd->User_Itm_Booster[scroll_y+1].itemnum>=0)
					m_pItemInfo->LoadingTXT(x+100,y+182,S_Item[m_pSprWnd->User_Itm_Booster[scroll_y+1].checked].itmnum,S_Item[m_pSprWnd->User_Itm_Booster[scroll_y+1].checked].checked+52,-2);
				else info_item = 0;
			}else if(state_num > 1)
			{
				if(m_pSprWnd->User_Itm_Etc[scroll_y+1].itemnum>=0)
					m_pItemInfo->LoadingTXT(x+100,y+182,H_Item[m_pSprWnd->User_Itm_Etc[scroll_y+1].checked].itmnum,H_Item[m_pSprWnd->User_Itm_Etc[scroll_y+1].checked].checked+8,-3);
				else info_item = 0;
			}
			if(m_pInputManager->m_isLButton == true)
			{
				if(click_choice != 1)
					m_pDSound.Playing(m_psnd.btn1.bff, false);
				click_choice = 1;
			}
			else
			{
				if(click_choice == 1)
				{
					//choice_click = click_choice;
					
					if(m_pInputManager->m_isRButton == true)
					{
						itemnum = m_pSprWnd->Interface_[state_num].itemnum;
						num     = m_pSprWnd->Interface_[state_num].num;
						itemck  = m_pSprWnd->Interface_[state_num].checked;

						if(state_num == 0)
						{
							if(m_pSprWnd->User_Itm_Att[click_choice+scroll_y].itemnum > -1)
							{
								if(A_Item[m_pSprWnd->User_Itm_Att[click_choice+scroll_y].checked].level
									> m_pSprWnd->Level)
								{
									click_choice = /*choice_click =*/ -1;
									m_pHelp->CreateHelp("Âø¿ëÇÒ ·¹º§ÀÌ ¾Æ´Õ´Ï´Ù.");
									return;
								}
								m_pDSound.Playing(m_psnd.btn2.bff, false);
							
								m_pSprWnd->Interface_[state_num].itemnum = 
									m_pSprWnd->User_Itm_Att[click_choice+scroll_y].itemnum;
								m_pSprWnd->Interface_[state_num].num     = 
									m_pSprWnd->User_Itm_Att[click_choice+scroll_y].num;
								m_pSprWnd->Interface_[state_num].checked = 
									m_pSprWnd->User_Itm_Att[click_choice+scroll_y].checked;

								m_pSprWnd->User_Itm_Att[click_choice+scroll_y].itemnum
									= itemnum;
								m_pSprWnd->User_Itm_Att[click_choice+scroll_y].num = num;
								m_pSprWnd->User_Itm_Att[click_choice+scroll_y].checked
									= itemck;
					
								m_pSprWnd->mis = 0;
								click_choice = /*choice_click =*/ -1;
							
								if(m_pSprWnd->Interface_[state_num].itemnum == 6)//¼Õ
									m_pDSound.LoadFile("sound\\hand.wav", &m_psnd.mugi.bff);
								else if(m_pSprWnd->Interface_[state_num].itemnum == 4)//ºÎ¸Þ¶û
									m_pDSound.LoadFile("sound\\boom.wav", &m_psnd.mugi.bff);
								else if(m_pSprWnd->Interface_[state_num].itemnum == 2)//Ä®
									m_pDSound.LoadFile("sound\\nife.wav", &m_psnd.mugi.bff);
								else if(m_pSprWnd->Interface_[state_num].itemnum == 0)//ÃÑ
									m_pDSound.LoadFile("sound\\gun.wav", &m_psnd.mugi.bff);
							}
						}else if(state_num == 1)
						{
							if(m_pSprWnd->User_Itm_Booster[click_choice+scroll_y].itemnum > -1)
							{
								m_pDSound.Playing(m_psnd.btn2.bff, false);
							
								m_pSprWnd->Interface_[state_num].itemnum = 
									m_pSprWnd->User_Itm_Booster[click_choice+scroll_y].itemnum;
								m_pSprWnd->Interface_[state_num].num     = 
									m_pSprWnd->User_Itm_Booster[click_choice+scroll_y].num;
								m_pSprWnd->Interface_[state_num].checked = 
									m_pSprWnd->User_Itm_Booster[click_choice+scroll_y].checked;
							}
						}else if(state_num > 1)
						{
							m_pDSound.Playing(m_psnd.btn2.bff, false);
							
							m_pSprWnd->Interface_[state_num].itemnum = 
								m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].itemnum;
							m_pSprWnd->Interface_[state_num].num     = 
								m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].num;
							m_pSprWnd->Interface_[state_num].checked = 
								m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].checked;

							m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].itemnum
								= itemnum;
							m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].num = num;
							m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].checked
								= itemck;
							
							click_choice =/* choice_click =*/ -1;
						}
					}
				}
			}
		}else if(m_pInputManager->m_MouseYPos >= (y+119) && m_pInputManager->m_MouseYPos <= (y+150))
		{
			info_item = 1;
			if(state_num == 0)
			{
				if(m_pSprWnd->User_Itm_Att[scroll_y+2].itemnum>=0)
					m_pItemInfo->LoadingTXT(x+100,y+182,A_Item[m_pSprWnd->User_Itm_Att[scroll_y+2].checked].itmnum,A_Item[m_pSprWnd->User_Itm_Att[scroll_y+2].checked].checked,state_num-1);
				else info_item = 0;
			}else if(state_num == 1)
			{
				if(m_pSprWnd->User_Itm_Booster[scroll_y+2].itemnum>=0)
					m_pItemInfo->LoadingTXT(x+100,y+182,S_Item[m_pSprWnd->User_Itm_Booster[scroll_y+2].checked].itmnum,S_Item[m_pSprWnd->User_Itm_Booster[scroll_y+2].checked].checked+52,-2);
				else info_item = 0;
			}else if(state_num > 1)
			{
				if(m_pSprWnd->User_Itm_Etc[scroll_y+2].itemnum>=0)
					m_pItemInfo->LoadingTXT(x+100,y+182,H_Item[m_pSprWnd->User_Itm_Etc[scroll_y+2].checked].itmnum,H_Item[m_pSprWnd->User_Itm_Etc[scroll_y+2].checked].checked+8,-3);
				else info_item = 0;
			}
			if(m_pInputManager->m_isLButton == true)
			{
				if(click_choice != 2)
					m_pDSound.Playing(m_psnd.btn1.bff, false);
				click_choice = 2;
			}
			else
			{
				if(click_choice == 2)
				{
				//	choice_click = click_choice;
					
					if(m_pInputManager->m_isRButton == true)
					{
						itemnum = m_pSprWnd->Interface_[state_num].itemnum;
						num     = m_pSprWnd->Interface_[state_num].num;
						itemck  = m_pSprWnd->Interface_[state_num].checked;
						
						if(state_num == 0)
						{
							if(m_pSprWnd->User_Itm_Att[click_choice+scroll_y].itemnum > -1)
							{
								if(A_Item[m_pSprWnd->User_Itm_Att[click_choice+scroll_y].checked].level
									> m_pSprWnd->Level)
								{
									click_choice = /*choice_click =*/ -1;
									m_pHelp->CreateHelp("Âø¿ëÇÒ ·¹º§ÀÌ ¾Æ´Õ´Ï´Ù.");
									return;
								}
								m_pDSound.Playing(m_psnd.btn2.bff, false);
							
								m_pSprWnd->Interface_[state_num].itemnum = 
									m_pSprWnd->User_Itm_Att[click_choice+scroll_y].itemnum;
								m_pSprWnd->Interface_[state_num].num     = 
									m_pSprWnd->User_Itm_Att[click_choice+scroll_y].num;
								m_pSprWnd->Interface_[state_num].checked = 
									m_pSprWnd->User_Itm_Att[click_choice+scroll_y].checked;

								m_pSprWnd->User_Itm_Att[click_choice+scroll_y].itemnum
									= itemnum;
								m_pSprWnd->User_Itm_Att[click_choice+scroll_y].num = num;
								m_pSprWnd->User_Itm_Att[click_choice+scroll_y].checked
									= itemck;
					
								m_pSprWnd->mis = 0;
								click_choice = /*choice_click =*/ -1;
							
								if(m_pSprWnd->Interface_[state_num].itemnum == 6)//¼Õ
									m_pDSound.LoadFile("sound\\hand.wav", &m_psnd.mugi.bff);
								else if(m_pSprWnd->Interface_[state_num].itemnum == 4)//ºÎ¸Þ¶û
									m_pDSound.LoadFile("sound\\boom.wav", &m_psnd.mugi.bff);
								else if(m_pSprWnd->Interface_[state_num].itemnum == 2)//Ä®
									m_pDSound.LoadFile("sound\\nife.wav", &m_psnd.mugi.bff);
								else if(m_pSprWnd->Interface_[state_num].itemnum == 0)//ÃÑ
									m_pDSound.LoadFile("sound\\gun.wav", &m_psnd.mugi.bff);
							}
						}else if(state_num == 1)
						{
							if(m_pSprWnd->User_Itm_Booster[click_choice+scroll_y].itemnum > -1)
							{
								m_pDSound.Playing(m_psnd.btn2.bff, false);
							
								m_pSprWnd->Interface_[state_num].itemnum = 
									m_pSprWnd->User_Itm_Booster[click_choice+scroll_y].itemnum;
								m_pSprWnd->Interface_[state_num].num     = 
									m_pSprWnd->User_Itm_Booster[click_choice+scroll_y].num;
								m_pSprWnd->Interface_[state_num].checked = 
									m_pSprWnd->User_Itm_Booster[click_choice+scroll_y].checked;
							}
						}else if(state_num > 1)
						{
							m_pDSound.Playing(m_psnd.btn2.bff, false);
							
							m_pSprWnd->Interface_[state_num].itemnum = 
								m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].itemnum;
							m_pSprWnd->Interface_[state_num].num     = 
								m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].num;
							m_pSprWnd->Interface_[state_num].checked = 
								m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].checked;

							m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].itemnum
								= itemnum;
							m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].num = num;
							m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].checked
								= itemck;
							
							click_choice =/* choice_click =*/ -1;
						}
					}
				}
			}
		}else if(m_pInputManager->m_MouseYPos >= (y+151) && m_pInputManager->m_MouseYPos <= (y+182))
		{
			info_item = 1;
			if(state_num == 0)
			{
				if(m_pSprWnd->User_Itm_Att[scroll_y+3].itemnum>=0)
					m_pItemInfo->LoadingTXT(x+100,y+182,A_Item[m_pSprWnd->User_Itm_Att[scroll_y+3].checked].itmnum,A_Item[m_pSprWnd->User_Itm_Att[scroll_y+3].checked].checked,state_num-1);
				else info_item = 0;
			}else if(state_num == 1)
			{
				if(m_pSprWnd->User_Itm_Booster[scroll_y+3].itemnum>=0)
					m_pItemInfo->LoadingTXT(x+100,y+182,S_Item[m_pSprWnd->User_Itm_Booster[scroll_y+3].checked].itmnum,S_Item[m_pSprWnd->User_Itm_Booster[scroll_y+3].checked].checked+52,-2);
				else info_item = 0;
			}else if(state_num > 1)
			{
				if(m_pSprWnd->User_Itm_Etc[scroll_y+3].itemnum>=0)
					m_pItemInfo->LoadingTXT(x+100,y+182,H_Item[m_pSprWnd->User_Itm_Etc[scroll_y+3].checked].itmnum,H_Item[m_pSprWnd->User_Itm_Etc[scroll_y+3].checked].checked+8,-3);
				else info_item = 0;
			}
			if(m_pInputManager->m_isLButton == true)
			{
				if(click_choice != 3)
					m_pDSound.Playing(m_psnd.btn1.bff, false);
				click_choice = 3;
			}
			else
			{
				if(click_choice == 3)
				{
					//choice_click = click_choice;
					
					if(m_pInputManager->m_isRButton == true)
					{
						itemnum = m_pSprWnd->Interface_[state_num].itemnum;
						num     = m_pSprWnd->Interface_[state_num].num;
						itemck  = m_pSprWnd->Interface_[state_num].checked;
						
						if(state_num == 0)
						{
							if(m_pSprWnd->User_Itm_Att[click_choice+scroll_y].itemnum > -1)
							{
								if(A_Item[m_pSprWnd->User_Itm_Att[click_choice+scroll_y].checked].level
									> m_pSprWnd->Level)
								{
									click_choice = /*choice_click =*/ -1;
									m_pHelp->CreateHelp("Âø¿ëÇÒ ·¹º§ÀÌ ¾Æ´Õ´Ï´Ù.");
									return;
								}
								m_pDSound.Playing(m_psnd.btn2.bff, false);
							
								m_pSprWnd->Interface_[state_num].itemnum = 
									m_pSprWnd->User_Itm_Att[click_choice+scroll_y].itemnum;
								m_pSprWnd->Interface_[state_num].num     = 
									m_pSprWnd->User_Itm_Att[click_choice+scroll_y].num;
								m_pSprWnd->Interface_[state_num].checked = 
									m_pSprWnd->User_Itm_Att[click_choice+scroll_y].checked;

								m_pSprWnd->User_Itm_Att[click_choice+scroll_y].itemnum
									= itemnum;
								m_pSprWnd->User_Itm_Att[click_choice+scroll_y].num = num;
								m_pSprWnd->User_Itm_Att[click_choice+scroll_y].checked
									= itemck;
					
								m_pSprWnd->mis = 0;
								click_choice = /*choice_click =*/ -1;
							
								if(m_pSprWnd->Interface_[state_num].itemnum == 6)//¼Õ
									m_pDSound.LoadFile("sound\\hand.wav", &m_psnd.mugi.bff);
								else if(m_pSprWnd->Interface_[state_num].itemnum == 4)//ºÎ¸Þ¶û
									m_pDSound.LoadFile("sound\\boom.wav", &m_psnd.mugi.bff);
								else if(m_pSprWnd->Interface_[state_num].itemnum == 2)//Ä®
									m_pDSound.LoadFile("sound\\nife.wav", &m_psnd.mugi.bff);
								else if(m_pSprWnd->Interface_[state_num].itemnum == 0)//ÃÑ
									m_pDSound.LoadFile("sound\\gun.wav", &m_psnd.mugi.bff);
							}
						}else if(state_num == 1)
						{
							if(m_pSprWnd->User_Itm_Booster[click_choice+scroll_y].itemnum > -1)
							{
								m_pDSound.Playing(m_psnd.btn2.bff, false);
							
								m_pSprWnd->Interface_[state_num].itemnum = 
									m_pSprWnd->User_Itm_Booster[click_choice+scroll_y].itemnum;
								m_pSprWnd->Interface_[state_num].num     = 
									m_pSprWnd->User_Itm_Booster[click_choice+scroll_y].num;
								m_pSprWnd->Interface_[state_num].checked = 
									m_pSprWnd->User_Itm_Booster[click_choice+scroll_y].checked;
							}
						}else if(state_num > 1)
						{
							m_pDSound.Playing(m_psnd.btn2.bff, false);
							
							m_pSprWnd->Interface_[state_num].itemnum = 
								m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].itemnum;
							m_pSprWnd->Interface_[state_num].num     = 
								m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].num;
							m_pSprWnd->Interface_[state_num].checked = 
								m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].checked;

							m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].itemnum
								= itemnum;
							m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].num = num;
							m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].checked
								= itemck;
							
							click_choice = /*choice_click =*/ -1;
						}
					}
				}
			}
		}else if(m_pInputManager->m_MouseYPos >= (y+183) && m_pInputManager->m_MouseYPos <= (y+215))
		{
			info_item = 1;
			if(state_num == 0)
			{
				if(m_pSprWnd->User_Itm_Att[scroll_y+4].itemnum>=0)
					m_pItemInfo->LoadingTXT(x+100,y+182,A_Item[m_pSprWnd->User_Itm_Att[scroll_y+4].checked].itmnum,A_Item[m_pSprWnd->User_Itm_Att[scroll_y+4].checked].checked,state_num-1);
				else info_item = 0;
			}else if(state_num == 1)
			{
				if(m_pSprWnd->User_Itm_Booster[scroll_y+4].itemnum>=0)
					m_pItemInfo->LoadingTXT(x+100,y+182,S_Item[m_pSprWnd->User_Itm_Booster[scroll_y+4].checked].itmnum,S_Item[m_pSprWnd->User_Itm_Booster[scroll_y+4].checked].checked+52,-2);
				else info_item = 0;
			}else if(state_num > 1)
			{
				if(m_pSprWnd->User_Itm_Etc[scroll_y+4].itemnum>=0)
					m_pItemInfo->LoadingTXT(x+100,y+182,H_Item[m_pSprWnd->User_Itm_Etc[scroll_y+4].checked].itmnum,H_Item[m_pSprWnd->User_Itm_Etc[scroll_y+4].checked].checked+8,-3);
				else info_item = 0;
			}
			
			if(m_pInputManager->m_isLButton == true)
			{
				if(click_choice != 4)
					m_pDSound.Playing(m_psnd.btn1.bff, false);
				click_choice = 4;
			}
			else
			{
				if(click_choice == 4)
				{
					//choice_click = click_choice;
					
					if(m_pInputManager->m_isRButton == true)
					{
						itemnum = m_pSprWnd->Interface_[state_num].itemnum;
						num     = m_pSprWnd->Interface_[state_num].num;
						itemck  = m_pSprWnd->Interface_[state_num].checked;
						
						if(state_num == 0)
						{
							if(m_pSprWnd->User_Itm_Att[click_choice+scroll_y].itemnum > -1)
							{
								if(A_Item[m_pSprWnd->User_Itm_Att[click_choice+scroll_y].checked].level
									> m_pSprWnd->Level)
								{
									click_choice = /*choice_click =*/ -1;
									m_pHelp->CreateHelp("Âø¿ëÇÒ ·¹º§ÀÌ ¾Æ´Õ´Ï´Ù.");
									return;
								}
								m_pDSound.Playing(m_psnd.btn2.bff, false);
							
								m_pSprWnd->Interface_[state_num].itemnum = 
									m_pSprWnd->User_Itm_Att[click_choice+scroll_y].itemnum;
								m_pSprWnd->Interface_[state_num].num     = 
									m_pSprWnd->User_Itm_Att[click_choice+scroll_y].num;
								m_pSprWnd->Interface_[state_num].checked = 
									m_pSprWnd->User_Itm_Att[click_choice+scroll_y].checked;

								m_pSprWnd->User_Itm_Att[click_choice+scroll_y].itemnum
									= itemnum;
								m_pSprWnd->User_Itm_Att[click_choice+scroll_y].num = num;
								m_pSprWnd->User_Itm_Att[click_choice+scroll_y].checked
									= itemck;
					
								m_pSprWnd->mis = 0;
								click_choice = /*choice_click =*/ -1;
							
								if(m_pSprWnd->Interface_[state_num].itemnum == 6)//¼Õ
									m_pDSound.LoadFile("sound\\hand.wav", &m_psnd.mugi.bff);
								else if(m_pSprWnd->Interface_[state_num].itemnum == 4)//ºÎ¸Þ¶û
									m_pDSound.LoadFile("sound\\boom.wav", &m_psnd.mugi.bff);
								else if(m_pSprWnd->Interface_[state_num].itemnum == 2)//Ä®
									m_pDSound.LoadFile("sound\\nife.wav", &m_psnd.mugi.bff);
								else if(m_pSprWnd->Interface_[state_num].itemnum == 0)//ÃÑ
									m_pDSound.LoadFile("sound\\gun.wav", &m_psnd.mugi.bff);
							}
						}else if(state_num == 1)
						{
							if(m_pSprWnd->User_Itm_Booster[click_choice+scroll_y].itemnum > -1)
							{
								m_pDSound.Playing(m_psnd.btn2.bff, false);
							
								m_pSprWnd->Interface_[state_num].itemnum = 
									m_pSprWnd->User_Itm_Booster[click_choice+scroll_y].itemnum;
								m_pSprWnd->Interface_[state_num].num     = 
									m_pSprWnd->User_Itm_Booster[click_choice+scroll_y].num;
								m_pSprWnd->Interface_[state_num].checked = 
									m_pSprWnd->User_Itm_Booster[click_choice+scroll_y].checked;

							}
						}else if(state_num > 1)
						{
							m_pDSound.Playing(m_psnd.btn2.bff, false);
						
							m_pSprWnd->Interface_[state_num].itemnum = 
								m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].itemnum;
							m_pSprWnd->Interface_[state_num].num     = 
								m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].num;
							m_pSprWnd->Interface_[state_num].checked = 
								m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].checked;

							m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].itemnum
								= itemnum;
							m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].num = num;
							m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].checked
								= itemck;
							
							click_choice = /*choice_click =*/ -1;
						}
					}
				}
			}
		}
	}



	if(click_choice > -1)
	{
		if(m_pInputManager->m_isLButton == false)
		{
			if(m_pInputManager->m_MouseXPos < x || m_pInputManager->m_MouseYPos < y
				|| m_pInputManager->m_MouseXPos > (x+100))
			{
				if(state_num == 0)
				{
					if(m_pSprWnd->User_Itm_Att[click_choice+scroll_y].itemnum >= 0)
					{
						m_pDSound.Playing(m_psnd.out.bff, false);
						
						m_pEcffect->CreateItem(m_pSprWnd->spr_x,m_pSprWnd->spr_y,
							m_pSprWnd->User_Itm_Att[click_choice+scroll_y].itemnum,
								m_pSprWnd->User_Itm_Att[click_choice+scroll_y].num,
									m_pSprWnd->User_Itm_Att[click_choice+scroll_y].checked,-1);
					
						m_pSprWnd->User_Itm_Att[click_choice+scroll_y].itemnum = -1;
					}
				}else if(state_num > 1)
				{
					if(m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].itemnum >= 0)
					{
						char s[256];
						if(!m_pM_Box_S)
						{
							m_pDSound.Playing(m_psnd.out.bff, false);
							
							m_pM_Box_S = new CMessageBox();
							m_pM_Box_S->Init(m_pDirect3D,300,230,0,1);	
							CopyMemory(m_pM_Box_S->M_Box.Box_Text[0],"¾ó¸¶Å­ ¹ö¸±±î¿ä?",16);
							sprintf(s,"(ÇöÀç %d°³º¸À¯)",m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].num);
							CopyMemory(m_pM_Box_S->M_Box.Box_Text[1],s,strlen(s));
							m_pM_Box_S->M_Box.max = m_pSprWnd->User_Itm_Etc[click_choice+scroll_y].num;
						}
					}
				}
				choice_click = click_choice;                
				click_choice = -1;
			}
		}
	}
}