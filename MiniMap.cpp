#include "MapOutput.h"
#include "Object.h"
#include "InputManager.h"
#include "DirectSound.h"
#include "MiniMap.h"

int btn_roll_s = 0,btn_roll_l = 1;
int btn_click_s = 0,btn_click_l = 0;
char sus = 0;

cMiniMap::cMiniMap()
{
	m_pDirect3D = NULL;

	for(int p = 0;p < 3;p++)
		m_pPicData[p] = NULL;

	for(int p = 0;p < 8;p++)
		m_pBtnPic[p] = NULL;
	
	m_pBackPic /*= m_pUserPic*/ = NULL;

	//Mini_Map_User_x = 6;
	//Mini_Map_User_y = 84;
    
	Mini_Map_x = Mini_Map_y = 0.0f;

	Mini_Map_State = 1;

	quest_num = 0;
}

cMiniMap::~cMiniMap()
{
	SAFE_DELETE_(m_pD3dxBigfont);
	SAFE_DELETE_(m_pD3dxSmallfont);
	for(int p=0;p<3;p++)
		SAFE_DELETE_(m_pPicData[p]);
	SAFE_DELETE_(m_pBackPic);
}

void cMiniMap::Init(CDirect3D *D3D)
{
	m_pDirect3D = D3D;
	char buf[256];

	m_pPicData[0]= new CPicData(D3D);
	m_pPicData[0]->Load("pic/minimap_small.pic");
	m_pPicData[0]->LoadPicTexture();

	m_pPicData[1]= new CPicData(D3D);
	m_pPicData[1]->Load("pic/minimap_middle.pic");
	m_pPicData[1]->LoadPicTexture();

	m_pPicData[2]= new CPicData(D3D);
	m_pPicData[2]->Load("pic/minimap_large.pic");
	m_pPicData[2]->LoadPicTexture();
	
	m_pBackPic = new CPicData(D3D);
	m_pBackPic->Load("pic/minimap_back.pic");
	m_pBackPic->LoadPicTexture();

/*	m_pUserPic = new CPicData(D3D);
	m_pUserPic->Load("pic/minimap_user.pic");
	m_pUserPic->LoadPicTexture();*/

	for(int p=0;p<8;p++)
	{
		sprintf(buf,"pic/mini_btn_%d.pic",p);
		m_pBtnPic[p] = new CPicData(D3D);
		m_pBtnPic[p]->Load(buf);
		m_pBtnPic[p]->LoadPicTexture();
	}
	m_pD3dxBigfont = NULL;
	m_pD3dxBigfont = new CD3dxFont();
	if(!m_pD3dxBigfont->CreateFONT(D3D->GetDevice(), (char*)GetStockObject(SYSTEM_FONT), 28))
        return;
	m_pD3dxSmallfont = NULL;
	m_pD3dxSmallfont = new CD3dxFont();
	if(!m_pD3dxSmallfont->CreateFONT(D3D->GetDevice(), (char*)GetStockObject(SYSTEM_FONT), 16))
		return;
}

void
cMiniMap::LoadVillige()
{
	FILE *vlg;
	char vil[30];

	ZeroMemory(villige,15);

	sprintf(vil,"txt/minimap/%d.txt",m_pMapViewWnd->game_map_stage);
	vlg = fopen(vil,"rt");
	fscanf(vlg,"%s",villige);
	fclose(vlg);
	if(m_pMapViewWnd->quest != 0)
	{
	/*	for (int i=0;i<quest_num;i++) 
		{
			if(QUEST[i])
			{
				SAFE_DELETE_ARRAY_(QUEST[i]);
			}
		}
		SAFE_DELETE_ARRAY_(QUEST);
*/
		sprintf(vil,"txt/quest/quest_%d.txt",m_pMapViewWnd->game_map_stage);
		vlg = fopen(vil,"rt");

		fscanf(vlg,"%d",&quest_num);
        QUEST = new char *[quest_num];
		int nn = 0;
		for(int p=0;p<quest_num;p++)
		{
			fscanf(vlg,"%d",&nn);
			QUEST[p] = new char [nn];

			fgets(QUEST[p],nn,vlg);
		}
		fclose(vlg);
	}
}

void
cMiniMap::Render()
{   
	int index,o_x=m_pMapViewWnd->map_x,o_y=m_pMapViewWnd->map_y;
	tINDEX List[1000];

	//MousePosition();

	if(Mini_Map_State !=0)
	{
		m_pDirect3D->AlphaBlendEnable(170);
		DrawBackPic();		
		m_pDirect3D->AlphaBlendDisable();

		if(m_pMapViewWnd->quest == 0)
		{
			m_pMapViewWnd->MiniMap_BackTile((int)Mini_Map_x+13,(int)Mini_Map_y+35);
        	
			int all=m_pObjectWnd->ObjCount;
			for (int i=0;i<all;i++)
			{
				List[i].Index=i;
				List[i].Value=m_pObjectWnd->aOBJ[i].y - o_y ; 
			}
			// Äü¼ÒÆ®..
			qsort((void *)List,all,sizeof(tINDEX),(int(*)(const void *,const void *))Compare);
		
			if(o_x>=(m_pMapViewWnd->pMAP->WIDTH*32)-1312)
			{
				o_x=(m_pMapViewWnd->pMAP->WIDTH*32)-1312;
			}
			if(o_y>=(m_pMapViewWnd->pMAP->HEIGHT*32)-1120)
			{
				o_y=(m_pMapViewWnd->pMAP->HEIGHT*32)-1120;
			}
	
			for (int i=0;i<all;i++)
			{
				index=List[i].Index;
		
				//if(o_x >= m_pMapViewWnd->map_x)
				int xx  =m_pObjectWnd->aOBJ[index].x-o_x;
				int yy  =m_pObjectWnd->aOBJ[index].y-o_y;
				int num =m_pObjectWnd->aOBJ[index].num;
				if (yy > -200 && yy < 1100 &&
						xx > -100 && xx < 1200  )
							m_pMapViewWnd->DrawMiniObject((int)((Mini_Map_x+15)+(xx/8)),(int)((Mini_Map_x+36)+(yy/8)),num);
			}

			m_pMapViewWnd->MiniMap_FrontTile((int)Mini_Map_x+13,(int)Mini_Map_y+35);
		}else
		{
			for (int i=0;i<quest_num;i++)
				m_pD3dxSmallfont->DrawText((int)Mini_Map_x+15,(int)(Mini_Map_y+38.0f)+(i*20),0,0,0xffffffff,
					QUEST[i],strlen(QUEST[i]));
		}
	}
	DrawPic();
	if(Mini_Map_State == 2)
		m_pD3dxBigfont->DrawText((int)Mini_Map_x+15,(int)Mini_Map_y+190,0,0,0xffffffff,villige,strlen(villige));
	
	
	MousePosition();
	BtnDraw();

}/*
void
cMiniMap::DrawUserPic()
{
	m_pDirect3D->GetDevice()->SetTexture( 0, 
									 m_pUserPic->g_pPicTexture);
			
	CUSTOM_VERTEX*	pV = NULL;

	m_pMapViewWnd->g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

	for( int m=0; m < 4; m++ )
	{
      pV[m].vPos.z = 1.0f;
	  pV[m].rhw = 1.0f;
	  pV[m].dwDiffuse = 0xffffffff;
	}
		

	float   fLeft = (Mini_Map_x+(Mini_Map_User_x/2));
	float	fWidth  = (float)m_pUserPic->w_Width;
	float	fTop    = ((Mini_Map_y+60.0f)+(Mini_Map_User_y/2));
	float	fHeight = (float)m_pUserPic->h_Height;

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
}*/
void
cMiniMap::DrawBackPic()
{
	m_pDirect3D->GetDevice()->SetTexture( 0, 
									 m_pBackPic->g_pPicTexture);
			
	CUSTOM_VERTEX*	pV = NULL;

	m_pMapViewWnd->g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

	for( int m=0; m < 4; m++ )
	{
      pV[m].vPos.z = 1.0f;
	  pV[m].rhw = 1.0f;
	  pV[m].dwDiffuse = 0xffffffff;
	}
		

	float	fLeft   = Mini_Map_x+13.0f;
	float	fWidth  = 161.0f;
	float	fTop    = Mini_Map_y+33.0f;
	float	fHeight = 141.0f;

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
cMiniMap::DrawPic()
{
	m_pDirect3D->GetDevice()->SetTexture( 0, 
									 m_pPicData[Mini_Map_State]->g_pPicTexture);
			
	CUSTOM_VERTEX*	pV = NULL;

	m_pMapViewWnd->g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

	for( int m=0; m < 4; m++ )
	{
      pV[m].vPos.z = 1.0f;
	  pV[m].rhw = 1.0f;
	  pV[m].dwDiffuse = 0xffffffff;
	}
		

	float	fLeft   = Mini_Map_x;
	float	fWidth  = (float)m_pPicData[Mini_Map_State]->w_Width;
	float	fTop    = Mini_Map_y;
	float	fHeight = (float)m_pPicData[Mini_Map_State]->h_Height;

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
cMiniMap::DrawBtnPic(int x,int y,int num)
{
	m_pDirect3D->GetDevice()->SetTexture( 0, 
									 m_pBtnPic[num]->g_pPicTexture);
			
	CUSTOM_VERTEX*	pV = NULL;

	m_pMapViewWnd->g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

	for( int m=0; m < 4; m++ )
	{
      pV[m].vPos.z = 1.0f;
	  pV[m].rhw = 1.0f;
	  pV[m].dwDiffuse = 0xffffffff;
	}
		

	float	fLeft   = Mini_Map_x+x;
	float	fWidth  = (float)m_pBtnPic[num]->w_Width;
	float	fTop    = Mini_Map_y+y;
	float	fHeight = (float)m_pBtnPic[num]->h_Height;

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
cMiniMap::BtnDraw()
{
	DrawBtnPic(120,10,btn_roll_s);
	DrawBtnPic(138,10,btn_roll_l);
}
void
cMiniMap::MousePosition()
{
	//POINT oldpos,newpos;
	//::GetCursorPos(&oldpos);

	btn_roll_s = 0;
	btn_roll_l = 1;
	if(Mini_Map_State==0)
		btn_roll_s = 6;
	else if(Mini_Map_State==2)
		btn_roll_l = 7;

	//187,181m_pInputManager->m_MouseXPos, m_pInputManager->m_MouseYPos
	if(m_pInputManager->m_MouseXPos >= (Mini_Map_x+120.0f) && m_pInputManager->m_MouseYPos >= (Mini_Map_y+10.0f)
			&& m_pInputManager->m_MouseXPos <= (Mini_Map_x+136.0f) && m_pInputManager->m_MouseYPos <= (Mini_Map_y+26.0f))
	{
		if(sus != 1)
		{
			m_pDSound.Playing(m_psnd.btn.bff, false);
			sus = 1;
		}
		if(Mini_Map_State > 0 && btn_click_l == 0)
		{
			btn_roll_s = 2;
			if(m_pInputManager->m_isLButton == true)
			{
				btn_roll_s  = 4;
				btn_click_s = 1;
			}else if(m_pInputManager->m_isLButton == false)
			{
				if(btn_click_s == 1)
				{
					m_pDSound.Playing(m_psnd.btn0.bff, false);
					btn_click_s = 0;
					Mini_Map_State--;
				}
				//if(Mini_Map_State <= 0)Mini_Map_State = 0;
				//Sleep(00);
			}
		}
	}
	else if(m_pInputManager->m_MouseXPos >= (Mini_Map_x+138.0f) && m_pInputManager->m_MouseYPos >= (Mini_Map_y+10.0f)
			&& m_pInputManager->m_MouseXPos <= (Mini_Map_x+154.0f) && m_pInputManager->m_MouseYPos <= (Mini_Map_y+26.0f))
	{
		if(sus != 2)
		{
			m_pDSound.Playing(m_psnd.btn.bff, false);
			sus = 2;
		}
		if(Mini_Map_State < 2 && btn_click_s == 0)
		{
			btn_roll_l = 3;
			if(m_pInputManager->m_isLButton == true)
			{
				btn_roll_l  = 5;
				btn_click_l = 1;
			}else
			{
				if(btn_click_l == 1)
				{
					m_pDSound.Playing(m_psnd.btn0.bff, false);
					btn_click_l = 0;
					Mini_Map_State++;
				}
				//if(Mini_Map_State >= 2)Mini_Map_State = 2;
				//Sleep(200);
			}
		}
	}else sus = 0;
	
	/*if(m_pInputManager->m_MouseXPos >= Mini_Map_x && m_pInputManager->m_MouseYPos >= Mini_Map_y
			&& m_pInputManager->m_MouseXPos <= (Mini_Map_x+187.0f) && m_pInputManager->m_MouseYPos <= (Mini_Map_y+181.0f))
	{

		if(btn_click_s == 1)
		{
			btn_click_s = 0;
			return;
		}
		else if(btn_click_l == 1)
		{
			btn_click_l = 0;
			return;
		}

		if(m_pInputManager->m_isLButton == true)
		{
			Mini_Map_x+=(float)m_pInputManager->m_MouseState.lX;
			Mini_Map_y+=(float)m_pInputManager->m_MouseState.lY;
		}
	}*/

}