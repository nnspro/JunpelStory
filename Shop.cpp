#include "MapOutput.h"
#include "InputManager.h"
#include "StatusForm.h"
#include "Help.h"
#include "MessageBox.h"
#include "DirectSound.h"
#include "Shop.h"

int ld;
char sound_s = 0;

CMessageBox *m_pM_Box   = NULL;
CMessageBox *m_pM_Box_1 = NULL;
CMessageBox *m_pM_Box_2 = NULL;

cShop::cShop()
{
	m_pDirect3D    = NULL;
	
	m_pShopPicData = NULL;
	m_pShopChoice  = NULL;

	//m_pStateMss->m_pItemInfo    = NULL;

	for(int p=0;p<15;p++)
		m_pShopBtnData[p] = NULL;

	shop_x   = 300;
	shop_y   = 200;
	
	btn_0 = 0;
	btn_1 = 3;
	btn_2 = 0;
	btn_3 = 3;
	btn_4 = 6;
	btn_5 = 9;
	btn_6 = 12;

	ret = shop_user_pos = shop_itm_pos = btn_clk = -1;

	shop_user_list = shop_itm_list = listnum = itm_view = 0;

	info_item = 0;
}

cShop::~cShop()
{
	SAFE_DELETE_(m_pD3dxfont);

	SAFE_DELETE_(m_pShopPicData);
	SAFE_DELETE_(m_pShopChoice);
	for(int p=0;p<15;p++)
		SAFE_DELETE_(m_pShopBtnData[p]);
}

void cShop::Init(CDirect3D *D3D, int v,int l)
{
	char ss[256];

	m_pDirect3D = D3D;

	itm_view = v;

	m_pShopPicData = new CPicData(D3D);
	m_pShopPicData->Load("pic/shop.pic");
	m_pShopPicData->LoadPicTexture();

	m_pShopChoice = new CPicData(D3D);
	m_pShopChoice->Load("pic/shop_choice.pic");
	m_pShopChoice->LoadPicTexture();
	
	for(int p=0;p<15;p++)
	{
		sprintf(ss,"pic/shop_btn_%d.pic",p);
		
		m_pShopBtnData[p] = new CPicData(D3D);
		m_pShopBtnData[p]->Load(ss);
		m_pShopBtnData[p]->LoadPicTexture();
	}

	FILE *shoplist;
	sprintf(ss,"txt/npc/shop/shop_%d.txt",l);
	shoplist = fopen(ss,"rt");
	fscanf(shoplist,"%d",&listnum);

	pst = new int[listnum];

	for(int p =0;p<listnum;p++)
	{
		fscanf(shoplist,"%d",&pst[p]);
	}

	fclose(shoplist);

	m_pD3dxfont = NULL;
	m_pD3dxfont = new CD3dxFont();
	if(!m_pD3dxfont->CreateFONT(D3D->GetDevice(), (char*)GetStockObject(SYSTEM_FONT), 16))
		return;

	m_pStateMss->m_pSprWnd->SHOP = TRUE;

}
int
cShop::Render()
{
	char ss[256]={"\0",};
	//float x_cur,y_cur;
	
	//x_cur =(float)m_pInputManager->m_MouseXPos;
	//y_cur = (float)m_pInputManager->m_MouseYPos;

	if(!m_pM_Box && !m_pM_Box_1 && !m_pM_Box_2)
		MousePosition();

	DrawPic(shop_x,shop_y,m_pShopPicData);
	//Button
	DrawPic(shop_x+111,shop_y+222,m_pShopBtnData[btn_0]);
	DrawPic(shop_x+111,shop_y+239,m_pShopBtnData[btn_1]);
	DrawPic(shop_x+237,shop_y+222,m_pShopBtnData[btn_2]);
	DrawPic(shop_x+237,shop_y+239,m_pShopBtnData[btn_3]);

	
	DrawPic(shop_x+128,shop_y+222,m_pShopBtnData[btn_4]);
	DrawPic(shop_x+2  ,shop_y+222,m_pShopBtnData[btn_5]);
	DrawPic(shop_x+66 ,shop_y+222,m_pShopBtnData[btn_6]);
	//DrawPic(shop_x+237,shop_y+237,m_pShopBtnData[btn_5]);

	if(itm_view == -1)//무기
	{
		//가게....
		for(int p=shop_itm_list;p<(shop_itm_list+5);p++)
		{
			m_pStateMss->m_pIpkData->DrawPic(shop_x+8,(shop_y+23)+((p-shop_itm_list)*40),A_Item[pst[p]].itmnum+1);
			m_pD3dxfont->DrawText(shop_x+46,(shop_y+22)+((p-shop_itm_list)*40),0,0,0xff000000,
				A_Item[pst[p]].itmname,strlen(A_Item[pst[p]].itmname));

			sprintf(ss,"%d아루",A_Item[pst[p]].buy);
			m_pD3dxfont->DrawText(shop_x+46,(shop_y+40)+((p-shop_itm_list)*40),0,0,0xff000000,
				ss,strlen(ss));

			if(p == shop_itm_pos)
			{
				m_pD3dxfont->DrawText(shop_x+46,(shop_y+22)+((p-shop_itm_list)*40),0,0,0xffff0000,
					A_Item[pst[p]].itmname,strlen(A_Item[pst[p]].itmname));

				m_pD3dxfont->DrawText(shop_x+46,(shop_y+40)+((p-shop_itm_list)*40),0,0,0xffff0000,
						ss,strlen(ss));
				m_pDirect3D->AlphaBlendEnable(80);
				DrawPic(shop_x+8,(shop_y+22)+((p-shop_itm_list)*40),m_pShopChoice);
				m_pDirect3D->AlphaBlendDisable();
			}
		}
		//사용자....
		for(int p=shop_user_list;p<(shop_user_list+5);p++)
		{
			if(m_pStateMss->m_pSprWnd->User_Itm_Att[p].itemnum >= 0)
			{
				m_pStateMss->m_pIpkData->DrawPic(shop_x+136,(shop_y+23)+((p-shop_user_list)*40),m_pStateMss->m_pSprWnd->User_Itm_Att[p].itemnum+1);
				m_pD3dxfont->DrawText(shop_x+173,(shop_y+22)+((p-shop_user_list)*40),0,0,0xff000000,
					A_Item[m_pStateMss->m_pSprWnd->User_Itm_Att[p].checked].itmname,strlen(A_Item[m_pStateMss->m_pSprWnd->User_Itm_Att[p].checked].itmname));

				sprintf(ss,"%d아루",A_Item[m_pStateMss->m_pSprWnd->User_Itm_Att[p].checked].sell);
				m_pD3dxfont->DrawText(shop_x+173,(shop_y+40)+((p-shop_user_list)*40),0,0,0xff000000,
					ss,strlen(ss));

				if(p == shop_user_pos)
				{
					m_pD3dxfont->DrawText(shop_x+173,(shop_y+22)+((p-shop_user_list)*40),0,0,0xff00ff00,
						A_Item[m_pStateMss->m_pSprWnd->User_Itm_Att[p].checked].itmname,strlen(A_Item[m_pStateMss->m_pSprWnd->User_Itm_Att[p].checked].itmname));

					m_pD3dxfont->DrawText(shop_x+173,(shop_y+40)+((p-shop_user_list)*40),0,0,0xff00ff00,
						ss,strlen(ss));
					m_pDirect3D->AlphaBlendEnable(80);
					DrawPic(shop_x+135,(shop_y+22)+((p-shop_user_list)*40),m_pShopChoice);
					m_pDirect3D->AlphaBlendDisable();
				}
			}
		}
	}else if(itm_view == -3)//회복약
	{	//가게....
		for(int p=shop_itm_list;p<(shop_itm_list+5);p++)
		{
			m_pStateMss->m_pIpkData->DrawPic(shop_x+8,(shop_y+23)+((p-shop_itm_list)*40),H_Item[pst[p]].itmnum+1);
			m_pD3dxfont->DrawText(shop_x+46,(shop_y+22)+((p-shop_itm_list)*40),0,0,0xff000000,
				H_Item[pst[p]].itmname,strlen(H_Item[pst[p]].itmname));

			sprintf(ss,"%d아루",H_Item[pst[p]].buy);
			m_pD3dxfont->DrawText(shop_x+46,(shop_y+40)+((p-shop_itm_list)*40),0,0,0xff000000,
				ss,strlen(ss));

			if(p == shop_itm_pos)
			{
				m_pD3dxfont->DrawText(shop_x+46,(shop_y+22)+((p-shop_itm_list)*40),0,0,0xffff0000,
					H_Item[pst[p]].itmname,strlen(H_Item[pst[p]].itmname));

				m_pD3dxfont->DrawText(shop_x+46,(shop_y+40)+((p-shop_itm_list)*40),0,0,0xffff0000,
						ss,strlen(ss));
				m_pDirect3D->AlphaBlendEnable(80);
				DrawPic(shop_x+8,(shop_y+22)+((p-shop_itm_list)*40),m_pShopChoice);
				m_pDirect3D->AlphaBlendDisable();
			}
		}
		//사용자....
		for(int p=shop_user_list;p<(shop_user_list+5);p++)
		{
			if(m_pStateMss->m_pSprWnd->User_Itm_Etc[p].itemnum >= 0)
			{
				m_pStateMss->m_pIpkData->DrawPic(shop_x+136,(shop_y+23)+((p-shop_user_list)*40),m_pStateMss->m_pSprWnd->User_Itm_Etc[p].itemnum+1);
				m_pD3dxfont->DrawText(shop_x+173,(shop_y+22)+((p-shop_user_list)*40),0,0,0xff000000,
					H_Item[m_pStateMss->m_pSprWnd->User_Itm_Etc[p].checked].itmname,strlen(H_Item[m_pStateMss->m_pSprWnd->User_Itm_Etc[p].checked].itmname));
				sprintf(ss,"%d",m_pStateMss->m_pSprWnd->User_Itm_Etc[p].num);
				m_pD3dxfont->DrawText(shop_x+140,(shop_y+40)+((p-shop_user_list)*40),0,0,0xff000000,
					ss,strlen(ss));
				sprintf(ss,"%d아루",H_Item[m_pStateMss->m_pSprWnd->User_Itm_Etc[p].checked].sell);
				m_pD3dxfont->DrawText(shop_x+173,(shop_y+40)+((p-shop_user_list)*40),0,0,0xff000000,
					ss,strlen(ss));

				if(p == shop_user_pos)
				{
					m_pD3dxfont->DrawText(shop_x+173,(shop_y+22)+((p-shop_user_list)*40),0,0,0xff00ff00,
						H_Item[m_pStateMss->m_pSprWnd->User_Itm_Etc[p].checked].itmname,strlen(H_Item[m_pStateMss->m_pSprWnd->User_Itm_Etc[p].checked].itmname));

					m_pD3dxfont->DrawText(shop_x+173,(shop_y+40)+((p-shop_user_list)*40),0,0,0xff00ff00,
						ss,strlen(ss));
					m_pDirect3D->AlphaBlendEnable(80);
					DrawPic(shop_x+135,(shop_y+22)+((p-shop_user_list)*40),m_pShopChoice);
					m_pDirect3D->AlphaBlendDisable();
				}
			}
		}
	}
	sprintf(ss,"%d",m_pStateMss->m_pSprWnd->Money);
	m_pD3dxfont->DrawText(shop_x+185,shop_y+2,0,0,0xffffffff,
		ss,strlen(ss));

	if(info_item == 1)
	{
		m_pStateMss->m_pItemInfo->Render();
	}

	if(m_pM_Box_2)
	{
		int a =	m_pM_Box_2->Render();
		if(a == -10)
		{
			SAFE_DELETE_(m_pM_Box_2);
		}
		else if(a == -20)
		{
			if(itm_view == -1)
			{
				m_pDSound.Playing(m_psnd.out.bff, false);
				
				m_pStateMss->m_pSprWnd->Money+=A_Item[m_pStateMss->m_pSprWnd->User_Itm_Att[shop_user_pos].checked].sell;
				m_pStateMss->m_pSprWnd->User_Itm_Att[shop_user_pos].itemnum = -1;
				SAFE_DELETE_(m_pM_Box_2);
				
				sprintf(ss,"판매했습니다.(%s)",A_Item[m_pStateMss->m_pSprWnd->User_Itm_Att[shop_user_pos].checked].itmname);
				m_pHelp->CreateHelp(ss);
				
				shop_user_pos = -1;
			}
		}else if(a>0)
		{
			if(itm_view == -3)
			{
				m_pDSound.Playing(m_psnd.out.bff, false);
				
				m_pStateMss->m_pSprWnd->Money+=(H_Item[m_pStateMss->m_pSprWnd->User_Itm_Etc[shop_user_pos].checked].sell*a);
				m_pStateMss->m_pSprWnd->User_Itm_Etc[shop_user_pos].num-=a;
				if(m_pStateMss->m_pSprWnd->User_Itm_Etc[shop_user_pos].num<=0)
					m_pStateMss->m_pSprWnd->User_Itm_Etc[shop_user_pos].itemnum = -1;
				SAFE_DELETE_(m_pM_Box_2);
				
				sprintf(ss,"판매했습니다.(%s)",H_Item[m_pStateMss->m_pSprWnd->User_Itm_Etc[shop_user_pos].checked].itmname);
				m_pHelp->CreateHelp(ss);
				
				shop_user_pos = -1;
			}
		}
	}

	if(m_pM_Box_1)
	{
		int a = m_pM_Box_1->Render();
		if(a == -20)
			SAFE_DELETE_(m_pM_Box_1);
	}

	if(m_pM_Box)
	{
		int a =	m_pM_Box->Render();
		if(a == -10)
		{
			SAFE_DELETE_(m_pM_Box);
		}
		else if(a == -20)
		{
			if(itm_view == -1)
			{
				if(m_pStateMss->m_pSprWnd->Money>=A_Item[pst[shop_itm_pos]].buy)
				{
					for(int p=0;p<10;p++)
					{
						if(m_pStateMss->m_pSprWnd->User_Itm_Att[p].itemnum == -1)
						{
							m_pDSound.Playing(m_psnd.eat.bff, false);
							
							m_pStateMss->m_pSprWnd->Money-=A_Item[pst[shop_itm_pos]].buy;
							m_pStateMss->m_pSprWnd->User_Itm_Att[p].itemnum = A_Item[pst[shop_itm_pos]].itmnum;
							m_pStateMss->m_pSprWnd->User_Itm_Att[p].num     = 1;
							m_pStateMss->m_pSprWnd->User_Itm_Att[p].checked = A_Item[pst[shop_itm_pos]].checked;
							
							sprintf(ss,"구매했습니다.(%s)",A_Item[pst[shop_itm_pos]].itmname);
							m_pHelp->CreateHelp(ss);

							break;
						}
					}
					SAFE_DELETE_(m_pM_Box);
				}else
				{
					m_pDSound.Playing(m_psnd.no.bff, false);
					
					SAFE_DELETE_(m_pM_Box);
					m_pM_Box_1 = new CMessageBox();
					m_pM_Box_1->Init(m_pDirect3D,300,230,0,0);	
				
					sprintf(ss,"아루가 부족합니다.(%d)",A_Item[pst[shop_itm_pos]].buy-m_pStateMss->m_pSprWnd->Money);
					m_pHelp->CreateHelp(ss);

					CopyMemory(m_pM_Box_1->M_Box.Box_Text[0],"아루가 부족합니다.",18);
				}
			}
		}else if(a>0)
		{
			if(itm_view == -3)
			{
				if(m_pStateMss->m_pSprWnd->Money>=(a*H_Item[pst[shop_itm_pos]].buy))
				{
					for(int p=0;p<30;p++)
					{
						if(m_pStateMss->m_pSprWnd->User_Itm_Etc[p].itemnum == H_Item[pst[shop_itm_pos]].itmnum)
						{
							if((m_pStateMss->m_pSprWnd->User_Itm_Etc[p].num+a)<=200)
							{
								m_pDSound.Playing(m_psnd.eat.bff, false);
								
								m_pStateMss->m_pSprWnd->Money-=(a*H_Item[pst[shop_itm_pos]].buy);
								m_pStateMss->m_pSprWnd->User_Itm_Etc[p].itemnum = H_Item[pst[shop_itm_pos]].itmnum;
								m_pStateMss->m_pSprWnd->User_Itm_Etc[p].num    += a;
								m_pStateMss->m_pSprWnd->User_Itm_Etc[p].checked = H_Item[pst[shop_itm_pos]].checked;

								sprintf(ss,"구매했습니다.(%s)",H_Item[pst[shop_itm_pos]].itmname);
								m_pHelp->CreateHelp(ss);

								break;
							}
						}
						
						if(m_pStateMss->m_pSprWnd->User_Itm_Etc[p].itemnum == -1)
						{
							m_pDSound.Playing(m_psnd.eat.bff, false);
							
							m_pStateMss->m_pSprWnd->Money-=(a*H_Item[pst[shop_itm_pos]].buy);
							m_pStateMss->m_pSprWnd->User_Itm_Etc[p].itemnum = H_Item[pst[shop_itm_pos]].itmnum;
							m_pStateMss->m_pSprWnd->User_Itm_Etc[p].num     = a;
							m_pStateMss->m_pSprWnd->User_Itm_Etc[p].checked = H_Item[pst[shop_itm_pos]].checked;

							sprintf(ss,"구매했습니다.(%s)",H_Item[pst[shop_itm_pos]].itmname);
							m_pHelp->CreateHelp(ss);

							break;
						}
					}
					SAFE_DELETE_(m_pM_Box);
				}else
				{
					m_pDSound.Playing(m_psnd.no.bff, false);
					
					SAFE_DELETE_(m_pM_Box);
					m_pM_Box_1 = new CMessageBox();
					m_pM_Box_1->Init(m_pDirect3D,300,230,0,0);	
				
					sprintf(ss,"아루가 부족합니다.(%d)",H_Item[pst[shop_itm_pos]].buy-m_pStateMss->m_pSprWnd->Money);
					m_pHelp->CreateHelp(ss);

					CopyMemory(m_pM_Box_1->M_Box.Box_Text[0],"아루가 부족합니다.",18);
				}
			}
		}
	}
	return ret;
}
void
cShop::DrawPic(int x,int y,CPicData *Pic)
{
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
	float	fWidth  = (float)Pic->w_Width;
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
cShop::MousePosition()
{
	btn_0 = 0;
	btn_1 = 3;
	btn_2 = 0;
	btn_3 = 3;
	btn_4 = 6;
	btn_5 = 9;
	btn_6 = 12;

	info_item = 0;

	if(m_pInputManager->m_MouseXPos >= (shop_x+111) && m_pInputManager->m_MouseXPos <= (shop_x+127))
	{
		if(m_pInputManager->m_MouseYPos >= (shop_y+222) && m_pInputManager->m_MouseYPos <= (shop_y+237))
		{
			if(listnum>5)
			{
				btn_0 = 1;
				if(sound_s != 1)
				{
					m_pDSound.Playing(m_psnd.btn.bff, false);
					sound_s = 1;
				}
				if(m_pInputManager->m_isLButton == true)
				{
					btn_0   = 2;
					btn_clk = 0;

					shop_itm_pos = -1;
				}
				else
				{
					if(btn_clk == 0)
					{
						
						
						shop_itm_list--;
						if(shop_itm_list < 0)
						{
							m_pDSound.Playing(m_psnd.no.bff, false);
							shop_itm_list = 0;
						}else
							m_pDSound.Playing(m_psnd.btn0.bff, false);
						
						btn_clk = -1;
					}
				}
			}
		}else if(m_pInputManager->m_MouseYPos >= (shop_y+239) && m_pInputManager->m_MouseYPos <= (shop_y+255))
		{
			if(listnum>5)
			{
				btn_1 = 4;
				if(sound_s != 2)
				{
					m_pDSound.Playing(m_psnd.btn.bff, false);
					sound_s = 2;
				}
				if(m_pInputManager->m_isLButton == true)
				{
					btn_1   = 5;
					btn_clk = 1;
					
					shop_itm_pos = -1;
				}
				else
				{
					if(btn_clk == 1)
					{
						shop_itm_list++;
						if(shop_itm_list > listnum-5)
						{
							m_pDSound.Playing(m_psnd.no.bff, false);
							shop_itm_list = listnum-5;
						}else 
							m_pDSound.Playing(m_psnd.btn0.bff, false);
						btn_clk = -1;
					}
		
				}
			}
		}
	}else
		if(m_pInputManager->m_MouseXPos >= (shop_x+237) && m_pInputManager->m_MouseXPos <= (shop_x+253))
	{
		if(m_pInputManager->m_MouseYPos >= (shop_y+222) && m_pInputManager->m_MouseYPos <= (shop_y+237))
		{
			btn_2 = 1;
			if(sound_s != 3)
			{
				m_pDSound.Playing(m_psnd.btn.bff, false);
				sound_s = 3;
			}
			if(m_pInputManager->m_isLButton == true)
			{
				btn_2   = 2;
				btn_clk = 2;
			}
			else
			{
				if(btn_clk == 2)
				{
					btn_clk = -1;
					shop_user_list--;
                    if(shop_user_list<0)
					{
						m_pDSound.Playing(m_psnd.no.bff, false);
						shop_user_list = 0;
					}else
						m_pDSound.Playing(m_psnd.btn0.bff, false);
				}
			}
		}else if(m_pInputManager->m_MouseYPos >= (shop_y+239) && m_pInputManager->m_MouseYPos <= (shop_y+255))
		{
			btn_3 = 4;
			if(sound_s != 4)
			{
				m_pDSound.Playing(m_psnd.btn.bff, false);
				sound_s = 4;
			}
			if(m_pInputManager->m_isLButton == true)
			{
				btn_3   = 5;
				btn_clk = 3;
			}
			else
			{
				if(btn_clk == 3)
				{
					btn_clk = -1;
					shop_user_list++;
					if(itm_view == -1)
					{
						if(shop_user_list>5)
						{
							m_pDSound.Playing(m_psnd.no.bff, false);
							shop_user_list = 5;
						}else
							m_pDSound.Playing(m_psnd.btn0.bff, false);
					}else if(itm_view == -3)
					{
						if(shop_user_list>25)
						{
							m_pDSound.Playing(m_psnd.no.bff, false);
							shop_user_list = 25;
						}else
							m_pDSound.Playing(m_psnd.btn0.bff, false);
					}
				}
			}
		}
	}else 
 //128,2,66
	if(m_pInputManager->m_MouseYPos >= (shop_y+222) && m_pInputManager->m_MouseYPos <= (shop_y+255))
	{
		if(m_pInputManager->m_MouseXPos >= (shop_x+128) && m_pInputManager->m_MouseXPos <= (shop_x+192))
		{
			if(shop_user_pos>=0)
			{
				btn_4 = 7;
				if(sound_s != 5)
				{
					m_pDSound.Playing(m_psnd.btn.bff, false);
					sound_s = 5;
				}
				if(m_pInputManager->m_isLButton == true)
				{
					btn_4   = 8;
					btn_clk = 4;
				}else
				{
					if(btn_clk == 4)
					{
						m_pDSound.Playing(m_psnd.btn0.bff, false);
						btn_clk = -1;
						if(!m_pM_Box_2)
						{
							m_pM_Box_2 = new CMessageBox();
							if(itm_view == -1)
							{
								m_pM_Box_2->Init(m_pDirect3D,300,230,1,0);	
								CopyMemory(m_pM_Box_2->M_Box.Box_Text[0],"이것을 판매합니까?",18);
							}else if(itm_view == -3)
							{
								char s[256];
								m_pM_Box_2->Init(m_pDirect3D,300,230,1,1);	
								CopyMemory(m_pM_Box_2->M_Box.Box_Text[0],"얼마큼 판매합니까?",18);
								sprintf(s,"(현재 %d개보유)",m_pStateMss->m_pSprWnd->User_Itm_Etc[shop_user_pos].num);
								CopyMemory(m_pM_Box_2->M_Box.Box_Text[1],s,strlen(s));
								m_pM_Box_2->M_Box.max = m_pStateMss->m_pSprWnd->User_Itm_Etc[shop_user_pos].num;
							}
						}
					}
				}
			}
		}else
			if(m_pInputManager->m_MouseXPos >= (shop_x+2) && m_pInputManager->m_MouseXPos <= (shop_x+66))
			{
				if(shop_itm_pos>=0)
				{
					btn_5 = 10;
					if(sound_s != 6)
					{
						m_pDSound.Playing(m_psnd.btn.bff, false);
						sound_s = 6;
					}
					if(m_pInputManager->m_isLButton == true)
					{
						btn_5   = 11;
						btn_clk = 5;
					}else
					{
						if(btn_clk == 5)
						{
							m_pDSound.Playing(m_psnd.btn0.bff, false);
							btn_clk = -1;

							if(!m_pM_Box)
							{
								m_pM_Box = new CMessageBox();
								if(itm_view == -1)
								{
									m_pM_Box->Init(m_pDirect3D,300,230,1,0);	
									CopyMemory(m_pM_Box->M_Box.Box_Text[0],"이것을 구입합니까?",18);
								}else if(itm_view == -3)
								{
									m_pM_Box->Init(m_pDirect3D,300,230,1,1);	
									CopyMemory(m_pM_Box->M_Box.Box_Text[0],"얼마큼 구입합니까?",18);
									CopyMemory(m_pM_Box->M_Box.Box_Text[1],"(1 ~ 100개까지만)",17);
									m_pM_Box->M_Box.max = 100;
								}
							}
						}
					}
				}
		}else
			if(m_pInputManager->m_MouseXPos >= (shop_x+66) && m_pInputManager->m_MouseXPos <= (shop_x+110))
			{
				btn_6 = 13;
				if(sound_s != 7)
				{
					m_pDSound.Playing(m_psnd.btn.bff, false);
					sound_s = 7;
				}
				if(m_pInputManager->m_isLButton == true)
				{
					btn_6   = 14;
					btn_clk = 6;
				}else
				{
					if(btn_clk == 6)
					{
						m_pDSound.Playing(m_psnd.btn0.bff, false);
						m_pStateMss->m_pSprWnd->SHOP = FALSE;
						
						btn_clk = -1;
						ret = -30;
					}
				}
		}
	}else sound_s = 0;

	if(m_pInputManager->m_MouseXPos >= (shop_x+8) && m_pInputManager->m_MouseXPos <= (shop_x+119))
	{
		if(m_pInputManager->m_MouseYPos >= (shop_y+22) && m_pInputManager->m_MouseYPos <= (shop_y+52))
		{
			info_item = 1;
			if(itm_view == -1)
				m_pStateMss->m_pItemInfo->LoadingTXT(shop_x+120,shop_y+22,A_Item[pst[shop_itm_list]].itmnum,A_Item[pst[shop_itm_list]].checked,itm_view);
			else if(itm_view == -3)
				m_pStateMss->m_pItemInfo->LoadingTXT(shop_x+120,shop_y+22,H_Item[pst[shop_itm_list]].itmnum,H_Item[pst[shop_itm_list]].checked+8,itm_view);
			
			if(m_pInputManager->m_isLButton == true)
			{
				if(btn_clk != 14)
					m_pDSound.Playing(m_psnd.btn1.bff, false);
				btn_clk = 14;
			}
			else
				if(btn_clk == 14)
				{
					shop_itm_pos = shop_itm_list;
					shop_user_pos = btn_clk = -1;
				}
		}else
		if(m_pInputManager->m_MouseYPos >= (shop_y+62) && m_pInputManager->m_MouseYPos <= (shop_y+92))
		{
			info_item = 1;
			if(itm_view == -1)
				m_pStateMss->m_pItemInfo->LoadingTXT(shop_x+120,shop_y+62,A_Item[pst[shop_itm_list+1]].itmnum,A_Item[pst[shop_itm_list+1]].checked,itm_view);
			else if(itm_view == -3)
				m_pStateMss->m_pItemInfo->LoadingTXT(shop_x+120,shop_y+62,H_Item[pst[shop_itm_list+1]].itmnum,H_Item[pst[shop_itm_list+1]].checked+8,itm_view);
			
			if(m_pInputManager->m_isLButton == true)
			{
				if(btn_clk != 15)
					m_pDSound.Playing(m_psnd.btn1.bff, false);
				btn_clk = 15;
			}
			else
				if(btn_clk == 15)
				{
					shop_itm_pos = shop_itm_list+1;
					shop_user_pos = btn_clk = -1;
				}
		}else
		if(m_pInputManager->m_MouseYPos >= (shop_y+102) && m_pInputManager->m_MouseYPos <= (shop_y+132))
		{
			info_item = 1;
			if(itm_view == -1)
				m_pStateMss->m_pItemInfo->LoadingTXT(shop_x+120,shop_y+102,A_Item[pst[shop_itm_list+2]].itmnum,A_Item[pst[shop_itm_list+2]].checked,itm_view);
			else if(itm_view == -3)
				m_pStateMss->m_pItemInfo->LoadingTXT(shop_x+120,shop_y+102,H_Item[pst[shop_itm_list+2]].itmnum,H_Item[pst[shop_itm_list+2]].checked+8,itm_view);
			
			if(m_pInputManager->m_isLButton == true)
			{
				if(btn_clk != 16)
					m_pDSound.Playing(m_psnd.btn1.bff, false);
				btn_clk = 16;
			}
			else
				if(btn_clk == 16)
				{
					shop_itm_pos = shop_itm_list+2;
					shop_user_pos = btn_clk = -1;
				}
		}else
		if(m_pInputManager->m_MouseYPos >= (shop_y+142) && m_pInputManager->m_MouseYPos <= (shop_y+172))
		{
			info_item = 1;
			if(itm_view == -1)
				m_pStateMss->m_pItemInfo->LoadingTXT(shop_x+120,shop_y+142,A_Item[pst[shop_itm_list+3]].itmnum,A_Item[pst[shop_itm_list+3]].checked,itm_view);
			else if(itm_view == -3)
				m_pStateMss->m_pItemInfo->LoadingTXT(shop_x+120,shop_y+142,H_Item[pst[shop_itm_list+3]].itmnum,H_Item[pst[shop_itm_list+3]].checked+8,itm_view);
			
			if(m_pInputManager->m_isLButton == true)
			{
				if(btn_clk != 17)
					m_pDSound.Playing(m_psnd.btn1.bff, false);
				btn_clk = 17;
			}
			else
				if(btn_clk == 17)
				{
					shop_itm_pos = shop_itm_list+3;
					shop_user_pos = btn_clk = -1;
				}
		}else
		if(m_pInputManager->m_MouseYPos >= (shop_y+182) && m_pInputManager->m_MouseYPos <= (shop_y+212))
		{
		info_item = 1;
			if(itm_view == -1)
				m_pStateMss->m_pItemInfo->LoadingTXT(shop_x+120,shop_y+182,A_Item[pst[shop_itm_list+4]].itmnum,A_Item[pst[shop_itm_list+4]].checked,itm_view);
			else if(itm_view == -3)
				m_pStateMss->m_pItemInfo->LoadingTXT(shop_x+120,shop_y+182,H_Item[pst[shop_itm_list+4]].itmnum,H_Item[pst[shop_itm_list+4]].checked+8,itm_view);
			
			if(m_pInputManager->m_isLButton == true)
			{
				if(btn_clk != 18)
					m_pDSound.Playing(m_psnd.btn1.bff, false);
				btn_clk = 18;
			}
			else
				if(btn_clk == 18)
				{
					shop_itm_pos = shop_itm_list+4;
					shop_user_pos = btn_clk = -1;
				}
		}
	}else//사용자.......................
	if(m_pInputManager->m_MouseXPos >= (shop_x+135) && m_pInputManager->m_MouseXPos <= (shop_x+246))
	{
		if(m_pInputManager->m_MouseYPos >= (shop_y+22) && m_pInputManager->m_MouseYPos <= (shop_y+52))
		{
			if(m_pInputManager->m_isLButton == true)
			{
				if(btn_clk != 19)
					m_pDSound.Playing(m_psnd.btn1.bff, false);
				btn_clk = 19;
			}
			else
				if(btn_clk == 19)
				{				
					shop_user_pos = shop_user_list;
					if(itm_view == -1)
					{
						if(m_pStateMss->m_pSprWnd->User_Itm_Att[shop_user_pos].itemnum < 0)
						shop_user_pos = -1;
					}else if(itm_view == -3)
					{
						if(m_pStateMss->m_pSprWnd->User_Itm_Etc[shop_user_pos].itemnum < 0)
						shop_user_pos = -1;
					}
					shop_itm_pos = btn_clk = -1;
				}
		}else
		if(m_pInputManager->m_MouseYPos >= (shop_y+62) && m_pInputManager->m_MouseYPos <= (shop_y+92))
		{
			if(m_pInputManager->m_isLButton == true)
			{
				if(btn_clk != 20)
					m_pDSound.Playing(m_psnd.btn1.bff, false);
				btn_clk = 20;
			}
			else
				if(btn_clk == 20)
				{
					shop_user_pos = shop_user_list+1;
					if(itm_view == -1)
					{
						if(m_pStateMss->m_pSprWnd->User_Itm_Att[shop_user_pos].itemnum < 0)
						shop_user_pos = -1;
					}else if(itm_view == -3)
					{
						if(m_pStateMss->m_pSprWnd->User_Itm_Etc[shop_user_pos].itemnum < 0)
						shop_user_pos = -1;
					}
					shop_itm_pos = btn_clk = -1;
				}
		}else
		if(m_pInputManager->m_MouseYPos >= (shop_y+102) && m_pInputManager->m_MouseYPos <= (shop_y+132))
		{
			if(m_pInputManager->m_isLButton == true)
			{
				if(btn_clk != 21)
					m_pDSound.Playing(m_psnd.btn1.bff, false);
				btn_clk = 21;
			}
			else
				if(btn_clk == 21)
				{
					shop_user_pos = shop_user_list+2;
					if(itm_view == -1)
					{
						if(m_pStateMss->m_pSprWnd->User_Itm_Att[shop_user_pos].itemnum < 0)
						shop_user_pos = -1;
					}else if(itm_view == -3)
					{
						if(m_pStateMss->m_pSprWnd->User_Itm_Etc[shop_user_pos].itemnum < 0)
						shop_user_pos = -1;
					}
					shop_itm_pos = btn_clk = -1;
				}
		}else
		if(m_pInputManager->m_MouseYPos >= (shop_y+142) && m_pInputManager->m_MouseYPos <= (shop_y+172))
		{
			if(m_pInputManager->m_isLButton == true)
			{
				if(btn_clk != 22)
					m_pDSound.Playing(m_psnd.btn1.bff, false);
				btn_clk = 22;
			}
			else
				if(btn_clk == 22)
				{
					shop_user_pos = shop_user_list+3;
					if(itm_view == -1)
					{
						if(m_pStateMss->m_pSprWnd->User_Itm_Att[shop_user_pos].itemnum < 0)
						shop_user_pos = -1;
					}else if(itm_view == -3)
					{
						if(m_pStateMss->m_pSprWnd->User_Itm_Etc[shop_user_pos].itemnum < 0)
						shop_user_pos = -1;
					}
					shop_itm_pos = btn_clk = -1;
				}
		}else
		if(m_pInputManager->m_MouseYPos >= (shop_y+182) && m_pInputManager->m_MouseYPos <= (shop_y+212))
		{
			if(m_pInputManager->m_isLButton == true)
			{
				if(btn_clk != 23)
					m_pDSound.Playing(m_psnd.btn1.bff, false);
				btn_clk = 23;
			}
			else
				if(btn_clk == 23)
				{
					shop_user_pos = shop_user_list+4;
					if(itm_view == -1)
					{
						if(m_pStateMss->m_pSprWnd->User_Itm_Att[shop_user_pos].itemnum < 0)
						shop_user_pos = -1;
					}else if(itm_view == -3)
					{
						if(m_pStateMss->m_pSprWnd->User_Itm_Etc[shop_user_pos].itemnum < 0)
						shop_user_pos = -1;
					}
					shop_itm_pos = btn_clk = -1;
				}
		}
	}
	if(m_pInputManager->m_isRButton == true)
	{
		m_pDSound.Playing(m_psnd.btn2.bff, false);
		if(shop_itm_pos >= 0)
		{
			if(!m_pM_Box)
			{
				m_pM_Box = new CMessageBox();
				if(itm_view == -1)
				{
					m_pM_Box->Init(m_pDirect3D,300,230,1,0);	
					CopyMemory(m_pM_Box->M_Box.Box_Text[0],"이것을 구입합니까?",18);
				}else if(itm_view == -3)
				{
					m_pM_Box->Init(m_pDirect3D,300,230,1,1);	
					CopyMemory(m_pM_Box->M_Box.Box_Text[0],"얼마큼 구입합니까?",18);
					CopyMemory(m_pM_Box->M_Box.Box_Text[1],"(1 ~ 100개까지만)",17);
					m_pM_Box->M_Box.max = 100;
				}
			}
		}else if(shop_user_pos >= 0)
		{
			if(!m_pM_Box_2)
			{
				m_pM_Box_2 = new CMessageBox();
				if(itm_view == -1)
				{
					m_pM_Box_2->Init(m_pDirect3D,300,230,1,0);	
					CopyMemory(m_pM_Box_2->M_Box.Box_Text[0],"이것을 판매합니까?",18);
				}else if(itm_view == -3)
				{
					char s[256];
					m_pM_Box_2->Init(m_pDirect3D,300,230,1,1);	
					CopyMemory(m_pM_Box_2->M_Box.Box_Text[0],"얼마큼 판매합니까?",18);
					sprintf(s,"(현재 %d개보유)",m_pStateMss->m_pSprWnd->User_Itm_Etc[shop_user_pos].num);
					CopyMemory(m_pM_Box_2->M_Box.Box_Text[1],s,strlen(s));
					m_pM_Box_2->M_Box.max = m_pStateMss->m_pSprWnd->User_Itm_Etc[shop_user_pos].num;
				}
			}
		}
	}
}