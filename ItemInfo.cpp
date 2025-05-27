#include "MapOutput.h"
#include "InputManager.h"
#include "StatusForm.h"
//#include "DirectSound.h"
#include "ItemInfo.h"

CItemInfomations::CItemInfomations()
{
	m_pDirect3D = NULL;
	m_pPicData  = NULL;

	TT = NULL;

	i_info_txt = 
		i_info_x = i_info_y = 
			i_info_scale_x = i_info_scale_y = 0;
	i_itm = -1;
}

CItemInfomations::~CItemInfomations()
{
	SAFE_DELETE_(m_pD3dxfont);
	SAFE_DELETE_(m_pD3dxfont_1);
	SAFE_DELETE_(m_pPicData);
}

void CItemInfomations::Init(CDirect3D *D3D)
{
	m_pDirect3D = D3D;

	m_pPicData = new CPicData(D3D);
	m_pPicData->Load("pic/mp.pic");
	m_pPicData->LoadPicTexture();
	
	m_pD3dxfont = NULL;
	m_pD3dxfont = new CD3dxFont();
	if(!m_pD3dxfont->CreateFONT(m_pDirect3D->GetDevice(), (char*)GetStockObject(SYSTEM_FONT), 17))
		return;
	m_pD3dxfont_1 = NULL;
	m_pD3dxfont_1 = new CD3dxFont();
	if(!m_pD3dxfont_1->CreateFONT(m_pDirect3D->GetDevice(), (char*)GetStockObject(SYSTEM_FONT), 25))
		return;
}

void 
CItemInfomations::LoadingTXT(int x,int y,int itm,int num,int vv)
{
	if(i_itm_num != num)
	{
	//	m_pDSound.Playing(m_psnd.btn.bff, false);
					
		item_view  = vv;

		i_itm     = itm;
		i_itm_num = num;
	
		FILE *item_read;
		char ss[256];

		if(vv == -1)
			sprintf(ss,"txt/item/att/%d.txt",num);
		if(vv == -2)
			sprintf(ss,"txt/item/skill/%d.txt",num-52);
		else if(vv == -3)
			sprintf(ss,"txt/item/enugy/%d.txt",num-8);

		item_read = fopen(ss,"rt");
		fscanf(item_read,"%d",&i_info_scale_x);
		fscanf(item_read,"%d",&i_info_scale_y);
	
		if(TT)
		{
			for(int p=0;p<i_info_txt;p++)
				SAFE_DELETE_ARRAY_(TT[p]);
		
			SAFE_DELETE_ARRAY_(TT);
		}

		fscanf(item_read,"%d",&i_info_txt);
		
		TT = new char*[i_info_txt];
		int ttt = 0;
		for(int p=0;p<i_info_txt;p++)
		{
			fscanf(item_read,"%d",&ttt);
			TT[p] = new char[ttt];
			fgets(TT[p],ttt,item_read);
		}

		fclose(item_read);
	}
	i_info_x  = x;
	i_info_y  = y;
	
	if((x+i_info_scale_x)>800)
		i_info_x  = 800-i_info_scale_x;
	if((y+i_info_scale_y)>500)
		i_info_y  = 500-i_info_scale_y;	
}

void
CItemInfomations::Render()
{
	char ss[30];

	m_pDirect3D->AlphaBlendEnable(150);
	DrawPic(i_info_x,i_info_y,i_info_scale_x,i_info_scale_y);
	m_pDirect3D->AlphaBlendDisable();
	
	m_pStateMss->m_pIpkData->DrawPic(i_info_x+5,i_info_y+5,i_itm);

	if(item_view == -1)
	{
		m_pD3dxfont_1->DrawText(i_info_x+45,i_info_y+5,0,0,0xffffffff,A_Item[i_itm_num].itmname,strlen(A_Item[i_itm_num].itmname));

		sprintf(ss,"착용레벨 : %d",A_Item[i_itm_num].level);
		if(A_Item[i_itm_num].level <= m_pStateMss->m_pSprWnd->Level)
			m_pD3dxfont->DrawText(i_info_x+45,i_info_y+45,0,0,0xffffffff,ss,strlen(ss));
		else
			m_pD3dxfont->DrawText(i_info_x+45,i_info_y+45,0,0,0xffff0000,ss,strlen(ss));
		sprintf(ss,"공격력   : %d",A_Item[i_itm_num].att);
		m_pD3dxfont->DrawText(i_info_x+45,i_info_y+63,0,0,0xffffffff,ss,strlen(ss));
	}else if(item_view == -2)
	{
		m_pD3dxfont_1->DrawText(i_info_x+45,i_info_y+5,0,0,0xffffffff,S_Item[i_itm_num-52].itmname,strlen(S_Item[i_itm_num-52].itmname));

		int m = S_Item[i_itm_num-52].att;
		if(m == 0)
			sprintf(ss,"이동속도 스킬");
		else if(m == 1)
			sprintf(ss,"공격 스킬");
		m_pD3dxfont->DrawText(i_info_x+45,i_info_y+35,0,0,0xffffffff,ss,strlen(ss));

		if(m == 0)
			sprintf(ss,"기본지속시간: 1분");
		else if(m == 1)
			sprintf(ss,"기본공격력 : %d",S_Item[i_itm_num-52].use);
        m_pD3dxfont->DrawText(i_info_x+45,i_info_y+53,0,0,0xffffffff,ss,strlen(ss));
		
		sprintf(ss,"소요 MP : %d",S_Item[i_itm_num-52].usemp);
		m_pD3dxfont->DrawText(i_info_x+45,i_info_y+71,0,0,0xffffffff,ss,strlen(ss));
	}else if(item_view == -3)
	{
		m_pD3dxfont_1->DrawText(i_info_x+45,i_info_y+5,0,0,0xffffffff,H_Item[i_itm_num-8].itmname,strlen(H_Item[i_itm_num-8].itmname));

		sprintf(ss,"HP회복력 : %d",H_Item[i_itm_num-8].hp);
		m_pD3dxfont->DrawText(i_info_x+40,i_info_y+45,0,0,0xffffffff,ss,strlen(ss));
		sprintf(ss,"MP회복력 : %d",H_Item[i_itm_num-8].mp);
		m_pD3dxfont->DrawText(i_info_x+40,i_info_y+63,0,0,0xffffffff,ss,strlen(ss));
	}
	for(int p=0;p<i_info_txt;p++)
			m_pD3dxfont->DrawText(i_info_x+10,(i_info_y+90)+(p*20),0,0,0xffffffff,
				TT[p],strlen(TT[p]));
}

void
CItemInfomations::DrawPic(int x,int y,int size_x,int size_y)
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
		

	float	fLeft   = (float)x;
	float	fWidth  = (float)size_x;
	float	fTop    = (float)y;
	float	fHeight = (float)size_y;

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