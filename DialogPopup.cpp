#include "MapOutput.h"
#include "InputManager.h"
#include "DialogPopup.h"
#include "StatusForm.h"
#include "Shop.h"
#include "Pet.h"
#include "DirectSound.h"
#include "Help.h"

int txt_ = 0,*colortxt_;
int btn_show = 0,btn_main_show = 0;
//FILE *bb;
int btn0 = 0,btn1 = 3,btn2 = 6,btn3 = 9,btn4 = 12; 
int talk_btn_click = 0;
int npc_num = 0;

char soundS = 0,soundSS = 0;

cShop *m_pShop = NULL;

CTalkBox::CTalkBox()
{
	m_pDirect3D = NULL;
	m_pPicData  = NULL;

	talk_npc_txt_num = btn_ck = colornum = talknum = 0;

	for(int p=0;p<15;p++)
		m_pBtnData[p] = NULL;

	talk_box_x = 280;
	talk_box_y = 150;

	npctxtnum = -2;

	btn_ck = 0;
//	bb=fopen("a.txt","wt");
}

CTalkBox::~CTalkBox()
{

/*	FILE *save;
	char bb[256];

	sprintf(bb,"txt/npc/mssck/mss_%d.txt",m_pMapViewWnd->game_map_stage);
	save = fopen(bb,"wt");
	for(int p=0;p<npcnum;p++)
		fprintf(save,"%d\n",mss_cl[p]);
	fclose(save);
*/
	//fclose(bb);
	SAFE_DELETE_(m_pD3dxfont);
	SAFE_DELETE_(m_pPicData);
	for(int p=0;p<15;p++)
		SAFE_DELETE_(m_pBtnData[p]);
}

void CTalkBox::Init(CDirect3D *D3D)
{
	char bb[256]={0,};

	m_pDirect3D = D3D;

	m_pPicData = new CPicData(D3D);
	m_pPicData->Load("pic/talkbox.pic");
	m_pPicData->LoadPicTexture();
	
	for(int p=0;p<15;p++)
	{
		sprintf(bb,"pic/tk_bn_%d.pic",p);
		m_pBtnData[p] = new CPicData(D3D);
		m_pBtnData[p]->Load(bb);
		m_pBtnData[p]->LoadPicTexture();
	}

	m_pD3dxfont = NULL;
	m_pD3dxfont = new CD3dxFont();
	if(!m_pD3dxfont->CreateFONT(m_pDirect3D->GetDevice(), "돋움체", 18))
		return;
}
void 
CTalkBox::LoadTxt(int n)
{
	char tbuf[256]={0,};
	FILE *txtfile;

	m_pDSound.Playing(m_psnd.open.bff, false);
	
	SAFE_DELETE_ARRAY_(colortxt_);

	txt_ = 0;
	npc_num = n;

	m_pMapViewWnd->quest = 0;

	sprintf(tbuf,"txt/npc/talk/talk%d_%d.txt",m_pMapViewWnd->mss_cl[n],talk_npc_txt_num);
REDATA:
	txtfile = fopen(tbuf,"rt");

	fscanf(txtfile,"%d",&talknum);

	if(talknum < 0)
	{
		if(talknum != -555)
		{
			fclose(txtfile);

			m_pShop = new cShop();
			m_pShop->Init(m_pDirect3D,talknum,talk_npc_txt_num);
			
			return;
		}else
		{
			int a,cked,cked1;
			
			ZeroMemory(tbuf,256);

			fscanf(txtfile,"%d",&a);
			fclose(txtfile);

			sprintf(tbuf,"txt/npc/ckecked/%d.txt",a);
			txtfile = fopen(tbuf,"rt");
			fscanf(txtfile,"%d",&cked);
			
			if(cked == 100)//Interface검사
			{
				fscanf(txtfile,"%d",&a);
				fscanf(txtfile,"%d",&cked);
				fscanf(txtfile,"%d",&cked1);
				if(m_pStateMss->m_pSprWnd->Interface_[a].itemnum == cked)
					fscanf(txtfile,"%d",&cked1);
				fclose(txtfile);

				sprintf(tbuf,"txt/npc/talk/talk0_%d.txt",cked1);
				
				talk_npc_txt_num = cked1;
				
				goto REDATA;
			//	txtfile = fopen(tbuf,"rt");
				
			//	fscanf(txtfile,"%d",&talknum);
			}
			else if(cked == 101)//레벨검사
			{
				fscanf(txtfile,"%d",&a);
				fscanf(txtfile,"%d",&cked);
				if(m_pStateMss->m_pSprWnd->Level >= a)
					fscanf(txtfile,"%d",&cked);
				fclose(txtfile);

				sprintf(tbuf,"txt/npc/talk/talk0_%d.txt",cked);
				
				talk_npc_txt_num = cked;
				
				goto REDATA;
				//txtfile = fopen(tbuf,"rt");
				
				//fscanf(txtfile,"%d",&talknum);
			}else if(cked == 102)//돈검사
			{
				fscanf(txtfile,"%d",&a);
				fscanf(txtfile,"%d",&cked);
				if(m_pStateMss->m_pSprWnd->Money >= a)
					fscanf(txtfile,"%d",&cked);
				fclose(txtfile);

				sprintf(tbuf,"txt/npc/talk/talk0_%d.txt",cked);
				
				talk_npc_txt_num = cked;
				
				goto REDATA;
				//txtfile = fopen(tbuf,"rt");
				
				//fscanf(txtfile,"%d",&talknum);
			}
		}
	}

	fscanf(txtfile,"%d",&colornum);
	
	TkData.text      = new char  [talknum];

	if(colornum != 0)
	{
		TkData.colortext = new char* [colornum];
		TkData.color_pos = new int   [colornum];
		TkData.color     = new int   [colornum];
		TkData.color_x   = new int   [colornum];
		TkData.color_y   = new int   [colornum];
		TkData.numtext   = new int   [colornum];

		colortxt_        = new int   [colornum];
	

		for(int p=0;p<colornum;p++)
		{
			colortxt_[p] = 0;

			fscanf(txtfile,"%d",&TkData.numtext[p]);
		
			TkData.colortext[p] = new char [TkData.numtext[p]];

			for(int pp=0;pp<TkData.numtext[p];pp++)
				TkData.colortext[p][pp] = fgetc(txtfile);
		
			fscanf(txtfile,"%d",&TkData.color_pos[p]);
			fscanf(txtfile,"%d",&TkData.color[p]);
			fscanf(txtfile,"%d",&TkData.color_x[p]);
			fscanf(txtfile,"%d",&TkData.color_y[p]);
		}
	}

	for(int p=0;p<talknum;p++)
		TkData.text[p] = fgetc(txtfile);

	fscanf(txtfile,"%d",&btn_main_show);
	fscanf(txtfile,"%d",&btn_show);

	fscanf(txtfile,"%d",&TkData.ms);
	fscanf(txtfile,"%d",&TkData.ms_ck);

	if(TkData.ms >= 100)
	{
		Q_bss = new QUEST_BONUS[TkData.ms_ck];
		for(int p=0;p<TkData.ms_ck;p++)
		{
			fscanf(txtfile,"%d",&Q_bss[p].input);
			fscanf(txtfile,"%d",&Q_bss[p].what);
			fscanf(txtfile,"%d",&Q_bss[p].bs);
		}
	}

	fclose(txtfile);
}
void
CTalkBox::Render()
{
	if(talknum > 0)
	{
		int f = 0;
		char puttxt[256]={0,},putcoltxt[256]={0,};

		DrawPic();

		if(btn_main_show == 0)
			DrawBtnPic(talk_box_x+180,talk_box_y+225,btn0);
		else if(btn_main_show == 1)
		{
			DrawBtnPic(talk_box_x+108,talk_box_y+225,btn3);
			DrawBtnPic(talk_box_x+180,talk_box_y+225,btn4);
		}

		if(btn_show == 1)
			DrawBtnPic(talk_box_x+10,talk_box_y+170,btn2);
		else if(btn_show == 2)
		{
			DrawBtnPic(talk_box_x+10,talk_box_y+170,btn1);
			DrawBtnPic(talk_box_x+82,talk_box_y+170,btn2);
		}
		else if(btn_show == 3)
			DrawBtnPic(talk_box_x+10,talk_box_y+170,btn1);

		strncpy(puttxt,TkData.text,txt_);
		m_pD3dxfont->DrawText(talk_box_x+10,talk_box_y+10,525,360,0xff000000,puttxt,txt_);
	
		for(int p=0;p<colornum;p++)
		{
			if(TkData.color_pos[p] <= txt_)
			{
				strncpy(putcoltxt,TkData.colortext[p],colortxt_[p]);
				if(TkData.color[p] == 1)
					m_pD3dxfont->DrawText((talk_box_x+10)+TkData.color_x[p],(talk_box_y+10)+TkData.color_y[p],
						525,360,0xff0000ff,putcoltxt,colortxt_[p]);
				else 
					if(TkData.color[p] == 2)
					m_pD3dxfont->DrawText((talk_box_x+10)+TkData.color_x[p],(talk_box_y+10)+TkData.color_y[p],
						525,360,0xff00ff00,putcoltxt,colortxt_[p]);
				else 
					if(TkData.color[p] == 3)
					m_pD3dxfont->DrawText((talk_box_x+10)+TkData.color_x[p],(talk_box_y+10)+TkData.color_y[p],
						525,360,0xffff0000,putcoltxt,colortxt_[p]);

				//f = strlen(TkData.colortext[p]);
					if(colortxt_[p] < TkData.numtext[p])
						colortxt_[p]++;
			}
		}
		if(txt_ < talknum)txt_++;

		MousePositions();
	}else
	{
		if(m_pShop)
		{
			int a = m_pShop->Render();
			if(a == -30)
			{
				btn_ck  = 0;
				txt_    = 0;
				npctxtnum = talknum = -2;
				
				SAFE_DELETE_(m_pShop);
			}
		}
	}
}
void
CTalkBox::DrawPic()
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
		

	float	fLeft   = (float)talk_box_x;//280.0f;
	float	fWidth  = (float)m_pPicData->w_Width;
	float	fTop    = (float)talk_box_y;//150.0f;
	float	fHeight = (float)m_pPicData->h_Height;

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
CTalkBox::DrawBtnPic(int x,int y,int num)
{
    //	Sprite에 사용될 쉐이더 설정
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
CTalkBox::MousePositions()
{
//	char n[256];

	btn0 =  0;
	btn1 =  3;
	btn2 =  6;
	btn3 =  9;
	btn4 = 12;

//	sprintf(n,"%d,%d",m_pInputManager->m_MouseXPos,m_pInputManager->m_MouseYPos);
//	m_pD3dxfont.DrawText(10,200,525,360,0xffff0000,n,10);

	if(btn_main_show == 0)
	{
		if(m_pInputManager->m_MouseXPos >= (talk_box_x+180) && m_pInputManager->m_MouseYPos >= (talk_box_y+225)
			&& m_pInputManager->m_MouseXPos <= (talk_box_x+250) && m_pInputManager->m_MouseYPos <= (talk_box_y+247))
		{
			btn0 = 1;
			if(soundS != 1)
			{
				m_pDSound.Playing(m_psnd.btn.bff, false);
				soundS = 1;
			}
			if(m_pInputManager->m_isLButton == true)
			{
				btn0 = 2;
				talk_btn_click = 1;
			}
			else if(m_pInputManager->m_isLButton == false)
			{
				if(talk_btn_click == 1)
				{
					m_pDSound.Playing(m_psnd.btn0.bff, false);
					talk_btn_click = 0;
					btn_ck = 0;
					txt_   = 0;	
					
					CheckQ(npc_num);
				}
			}
		}
	}else if(btn_main_show == 1)
	{
		if(m_pInputManager->m_MouseYPos >= (talk_box_y+225) && m_pInputManager->m_MouseYPos <= (talk_box_y+247))
		{
			if(m_pInputManager->m_MouseXPos >= (talk_box_x+108) && m_pInputManager->m_MouseXPos <= (talk_box_x+178))
			{	
				btn3 = 10;
				if(soundS != 2)
				{
					m_pDSound.Playing(m_psnd.btn.bff, false);
					soundS = 2;
				}
				if(m_pInputManager->m_isLButton == true)
				{
					btn3 = 11;
					talk_btn_click = 1;
				}
				else if(m_pInputManager->m_isLButton == false)
				{
					if(talk_btn_click == 1)
					{
						m_pDSound.Playing(m_psnd.btn0.bff, false);
						talk_btn_click = 0;
						btn_ck = 0;
						txt_   = 0;
						npctxtnum = -1;		
					}
				}
			}else 
				if(m_pInputManager->m_MouseXPos >= (talk_box_x+180) && m_pInputManager->m_MouseXPos <= (talk_box_x+250))
			{	
				btn4 = 13;
				if(soundS != 3)
				{
					m_pDSound.Playing(m_psnd.btn.bff, false);
					soundS = 3;
				}
				if(m_pInputManager->m_isLButton == true)
				{
					btn4 = 14;
					talk_btn_click = 1;
				}
				else if(m_pInputManager->m_isLButton == false)
				{
					if(talk_btn_click == 1)
					{
						m_pDSound.Playing(m_psnd.btn0.bff, false);
						talk_btn_click = 0;
						btn_ck = 0;
						txt_   = 0;		

						CheckQ(npc_num);
					}
				}
			}
		}
	}else soundS = 0;

	if(btn_show == 1)
	{//360,342
		if(m_pInputManager->m_MouseXPos >= (talk_box_x+10) && m_pInputManager->m_MouseYPos >= (talk_box_y+170)
			&& m_pInputManager->m_MouseXPos <= (talk_box_x+80) && m_pInputManager->m_MouseYPos <= (talk_box_y+192))
		{
			btn2 = 7;
			if(soundSS != 1)
			{
				m_pDSound.Playing(m_psnd.btn1.bff, false);
				soundSS = 1;
			}
			if(m_pInputManager->m_isLButton == true)
			{
				btn2 = 8;
				talk_btn_click = 1;
			}else if(m_pInputManager->m_isLButton == false)
			{
				if(talk_btn_click == 1)
				{
					m_pDSound.Playing(m_psnd.btn2.bff, false);
					talk_btn_click = 0;
					talk_npc_txt_num++;
					LoadTxt(npc_num);
				}
			}
		}
	}else
		if(btn_show == 2)
		{
			if(m_pInputManager->m_MouseXPos >= (talk_box_x+10) && m_pInputManager->m_MouseYPos >= (talk_box_y+170)
				&& m_pInputManager->m_MouseXPos <= (talk_box_x+80) && m_pInputManager->m_MouseYPos <= (talk_box_y+192))
			{
				btn1 = 4;
				if(soundSS != 1)
				{
					m_pDSound.Playing(m_psnd.btn1.bff, false);
					soundSS = 1;
				}
				if(m_pInputManager->m_isLButton == true)
				{
					btn1 = 5;
					talk_btn_click = 1;
				}else 
					if(m_pInputManager->m_isLButton == false)
				{
					if(talk_btn_click == 1)
					{
						m_pDSound.Playing(m_psnd.btn2.bff, false);
						talk_btn_click = 0;
						talk_npc_txt_num--;
						LoadTxt(npc_num);
					}
				}
			}else
			if(m_pInputManager->m_MouseXPos >= (talk_box_x+82) && m_pInputManager->m_MouseYPos >= (talk_box_y+170)
				&& m_pInputManager->m_MouseXPos <= (talk_box_x+152) && m_pInputManager->m_MouseYPos <= (talk_box_y+192))
			{
				btn2 = 7;
				if(soundSS != 2)
				{
					m_pDSound.Playing(m_psnd.btn1.bff, false);
					soundSS = 2;
				}
				if(m_pInputManager->m_isLButton == true)
				{
					btn2 = 8;
					talk_btn_click = 1;
				}else 
					if(m_pInputManager->m_isLButton == false)
				{
					if(talk_btn_click == 1)
					{
						m_pDSound.Playing(m_psnd.btn2.bff, false);
						talk_btn_click = 0;
						talk_npc_txt_num++;
						LoadTxt(npc_num);
					}
				}
			}
		}else 
			if(btn_show == 3)
			{
				if(m_pInputManager->m_MouseXPos >= (talk_box_x+10) && m_pInputManager->m_MouseYPos >= (talk_box_y+170)
					&& m_pInputManager->m_MouseXPos <= (talk_box_x+80) && m_pInputManager->m_MouseYPos <= (talk_box_y+192))
				{
					btn1 = 4;
					if(soundSS != 3)
					{
						m_pDSound.Playing(m_psnd.btn1.bff, false);
						soundSS = 3;
					}
					if(m_pInputManager->m_isLButton == true)
					{
						btn1 = 5;
						talk_btn_click = 2;
					}else
						if(m_pInputManager->m_isLButton == false)
					{
						if(talk_btn_click == 2)
						{
							m_pDSound.Playing(m_psnd.btn2.bff, false);
							talk_btn_click = 0;
							talk_npc_txt_num--;
							LoadTxt(npc_num);
						}
					}
				}
			}
			
			if(m_pInputManager->m_MouseXPos >= talk_box_x && m_pInputManager->m_MouseYPos >= talk_box_y
				&& m_pInputManager->m_MouseXPos <= (talk_box_x+256) && m_pInputManager->m_MouseYPos <= (talk_box_y+256))
			{
				if(talk_btn_click >= 1)
					return;
			
				if(m_pInputManager->m_isLButton == true)
				{
					talk_box_x+=m_pInputManager->m_MouseState.lX;
					talk_box_y+=m_pInputManager->m_MouseState.lY;
				}
			}
}

void CTalkBox::CheckQ(int n)
{
	char bb[30];

	npctxtnum = -1;
	
	if(TkData.ms == 100)
	{//퀘스트 수행후 보상
		for(int p=0;p<TkData.ms_ck;p++)
		{
			if(Q_bss[p].input == 100)
			{
				m_pStateMss->m_pSprWnd->Interface_[Q_bss[p].what].itemnum = Q_bss[p].bs;
				m_pHelp->CreateHelp("아이템을 받았습니다.");
			}else if(Q_bss[p].input == 101)
			{
				m_pStateMss->m_pSprWnd->Money+=Q_bss[p].what;
				sprintf(bb,"아루를 받았습니다.(+%d)",Q_bss[p].what);
				m_pHelp->CreateHelp(bb);
			}else if(Q_bss[p].input == 102)
			{
				/*if(Q_bss[p].what == 0)
				{
					for(int j=0;j<10;j++)
					{
						if(m_pStateMss->m_pSprWnd->User_Itm_Att[j].itemnum == -1)
						{
							m_pStateMss->m_pSprWnd->User_Itm_Att[j].itemnum = Q_bss[p].bs;
							break;
						}
					}
				}else*/ if(Q_bss[p].what == 1)
				{
					for(int j=0;j<5;j++)
					{
						if(m_pStateMss->m_pSprWnd->User_Itm_Booster[j].itemnum == -1)
						{
							m_pStateMss->m_pSprWnd->User_Itm_Booster[j].itemnum = S_Item[Q_bss[p].bs].itmnum;
							m_pStateMss->m_pSprWnd->User_Itm_Booster[j].checked = S_Item[Q_bss[p].bs].checked;
							break;
						}
					}
				}/*else if(Q_bss[p].what == 2)
				{
					for(int j=0;j<30;j++)
					{
						if(m_pStateMss->m_pSprWnd->User_Itm_Etc[j].itemnum == Q_bss[p].bs)
						{
							if((m_pStateMss->m_pSprWnd->User_Itm_Etc[j].num+1)<=200)
							{
								m_pStateMss->m_pSprWnd->User_Itm_Etc[j].itemnum = H_Item[pst[shop_itm_pos]].itmnum;
								m_pStateMss->m_pSprWnd->User_Itm_Etc[j].num    += a;
								m_pStateMss->m_pSprWnd->User_Itm_Etc[j].checked = H_Item[pst[shop_itm_pos]].checked;

								sprintf(ss,"구매했습니다.(%s)",H_Item[pst[shop_itm_pos]].itmname);
								m_pHelp->CreateHelp(ss);

								break;
							}
						}
						
						if(m_pStateMss->m_pSprWnd->User_Itm_Etc[p].itemnum == -1)
						{
						if(m_pStateMss->m_pSprWnd->User_Itm_Etc[j].itemnum == -1)
						{
							m_pStateMss->m_pSprWnd->User_Itm_Etc[j].itemnum = Q_bss[p].bs;
							break;
						}
					}
				}*/
				m_pHelp->CreateHelp("아이템을 받았습니다.");
			}
		}
		m_pDSound.Playing(m_psnd.gay.bff, false);
		m_pMapViewWnd->mss_cl[n] = 2;
		
		sprintf(bb,"txt/npc/mssck/mss_%d.txt",m_pMapViewWnd->game_map_stage);
		FILE *save = fopen(bb,"wt");
		for(int p=0;p<(m_pMapViewWnd->q_npc&0x000fff);p++)
		{
			fprintf(save,"%d\n",m_pMapViewWnd->mss_cl[p]);
		}
		fclose(save);
	}
	else if(TkData.ms == 99)
	{//퀘스트 스테이지로...
		m_pMapViewWnd->quest = 1;
		m_pMapViewWnd->q_npc&=0x000fff;
		m_pMapViewWnd->q_npc|=n<<12;
		m_pMapViewWnd->game_map_stage = TkData.ms_ck;
		gameState = 600;
	}else if(TkData.ms == 98)
	{//마을 가기 스테이지
		m_pMapViewWnd->quest = 0;
		m_pMapViewWnd->game_map_stage = TkData.ms_ck;
		gameState = 600;
	}else if(TkData.ms == 97)
	{//Pet구입
		m_pDSound.Playing(m_psnd.gay.bff, false);
		m_pStateMss->m_pSprWnd->Money-=TkData.ms_ck;
	
		m_pStateMss->m_pSprWnd->Interface_[7].itemnum = 58;
		
		m_pStateMss->m_pSprWnd->m_pPet = new cPetWnd();
		m_pStateMss->m_pSprWnd->m_pPet->Init(m_pDirect3D);
		m_pStateMss->m_pSprWnd->m_pPet->InitPet();
	}else if(TkData.ms <= 50)
	{//텔레포트
		m_pStateMss->m_pSprWnd->Money-=TkData.ms_ck;
		
		m_pMapViewWnd->quest = 0;
		m_pMapViewWnd->game_map_stage = TkData.ms;
		gameState = 600;
	}
}