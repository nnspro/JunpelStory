#include "MapOutput.h"
#include "InputManager.h"
#include "Npc.h"

#include <stdio.h>

//FILE *fii;
char bb[256];
FILE *npcfile;

cNpc::cNpc(CDirect3D *D3D)
{
	m_pDirect3D = NULL;
	m_pDirect3D = D3D;

	m_pSprWnd   = NULL;
	m_pTalkBox  = NULL;

	//npctxtnum = -1;//fii = fopen("a.txt","wt");
}

cNpc::~cNpc()
{
	SAFE_DELETE_(m_pSprWnd);
	
	if(npcnum > 0)
	{
		sprintf(bb,"txt/npc/mssck/mss_%d.txt",m_pMapViewWnd->game_map_stage);
		npcfile = fopen(bb,"wt");
	
		for(int p=0;p<npcnum;p++)
		{
			fprintf(npcfile,"%d\n",m_pMapViewWnd->mss_cl[p]);
		}
		fclose(npcfile);
	}
	
	SAFE_DELETE_(m_pTalkBox);
	//fclose(fii);
}

void cNpc::Init()
{
	if(m_pSprWnd)SAFE_DELETE_(m_pSprWnd);
	if(m_pTalkBox)SAFE_DELETE_(m_pTalkBox);
	

	m_pSprWnd  = new cSprDataWnd();
	m_pSprWnd->Init(m_pDirect3D);
	m_pSprWnd->SprLoad(1000);
	m_pSprWnd->LoadSprTexture();

	m_pTalkBox = new CTalkBox();
	m_pTalkBox->Init(m_pDirect3D);
	
	LoadNpcTxt();

	sprintf(bb,"txt/npc/mssck/mss_%d.txt",m_pMapViewWnd->game_map_stage);
	if(!(npcfile = fopen(bb,"rt")))
	{
		npcnum = 0;
		
		return;
	}

	m_pMapViewWnd->q_npc&=0xfff000;
	m_pMapViewWnd->q_npc|=npcnum;
	m_pMapViewWnd->mss_cl = new char[npcnum];
	
	for(int p=0;p<npcnum;p++)
	{
		fscanf(npcfile,"%d",&m_pMapViewWnd->mss_cl[p]);
	}
	fclose(npcfile);	
}

void
cNpc::LoadNpcTxt()
{
	sprintf(bb,"txt/npc/pos/%d.txt",m_pMapViewWnd->game_map_stage);
	if(!(npcfile = fopen(bb,"rt")))
	{
		npcnum = 0;

		return;
	}
	fscanf(npcfile,"%d",&npcnum);

	npcData            = new NpcData[npcnum];
	
	for(int p=0;p<npcnum;p++)
	{
		fscanf(npcfile,"%d",&npcData[p].npc_x);
		fscanf(npcfile,"%d",&npcData[p].npc_y);
		fscanf(npcfile,"%d",&npcData[p].mss_num);
		fscanf(npcfile,"%d",&npcData[p].spr_num);
	/*	int a;
		fscanf(npcfile,"%d",&a);
		if(a == 1)
			npcData[p].talk_two = TRUE;
		else
			npcData[p].talk_two = FALSE;
		
	//	m_pTalkBox->mss_cl[p] = 0;*/
	}
	fclose(npcfile);
} 

void
cNpc::DrawPic(int x, int y, int i)
{
    //	Sprite에 사용될 쉐이더 설정
m_pDirect3D->GetDevice()->SetTexture( 0, 
									 m_pSprWnd->g_pSprTexture[m_pSprWnd->pAni[i].pFrame[m_pSprWnd->pAni[i].CurFrame].Sprite] );
			
	CUSTOM_VERTEX*	pV = NULL;

	m_pMapViewWnd->g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

	for( int m=0; m < 4; m++ )
	{
      pV[m].vPos.z = 1.0f;
	  pV[m].rhw = 1.0f;
	  pV[m].dwDiffuse = 0xffffffff;
	}
		

	float	fLeft   = (float)x;
	float	fWidth  = (float)m_pSprWnd->w_Width[m_pSprWnd->pAni[i].pFrame[m_pSprWnd->pAni[i].CurFrame].Sprite];
	float	fTop    = (float)y;
	float	fHeight = (float)m_pSprWnd->h_Height[m_pSprWnd->pAni[i].pFrame[m_pSprWnd->pAni[i].CurFrame].Sprite];

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
cNpc::Render()
{
	int n_x=0,n_y=0;

	if(npcnum == 0)return;

	for(int p=0;p<npcnum;p++)
	{
		n_x = npcData[p].npc_x + m_pSprWnd->pAni[0].pFrame[m_pSprWnd->pAni[0].CurFrame].XPos
		- m_pMapViewWnd->map_x;
		n_y = npcData[p].npc_y + m_pSprWnd->pAni[0].pFrame[m_pSprWnd->pAni[0].CurFrame].YPos
		- m_pMapViewWnd->map_y;
		if(n_x >= -50 && n_x <= 800 &&
			n_y >= -80 && n_y <= 600)
			DrawPic(n_x,n_y,npcData[p].spr_num);
	}
	if(m_pTalkBox->btn_ck == 0)
		Mouseposition();
	//else
	//	m_pTalkBox->Render();
}
void
cNpc::Mouseposition()
{
	for(int p=0;p<npcnum;p++)
	{
		int xx = npcData[p].npc_x + m_pSprWnd->pAni[0].pFrame[m_pSprWnd->pAni[0].CurFrame].XPos 
			- m_pMapViewWnd->map_x;
		int yy = npcData[p].npc_y + m_pSprWnd->pAni[0].pFrame[m_pSprWnd->pAni[0].CurFrame].YPos 
			- m_pMapViewWnd->map_y;

		/*fprintf(fii,"%d,%d,%d,%d\n",
			m_pSprWnd->CrashBox[p].left,m_pSprWnd->CrashBox[p].top,
			m_pSprWnd->CrashBox[p].right,m_pSprWnd->CrashBox[p].bottom);*/

		if(m_pInputManager->m_MouseXPos >= xx+m_pSprWnd->CrashBox[p][0].left && m_pInputManager->m_MouseYPos >= yy+m_pSprWnd->CrashBox[p][0].top
			&& m_pInputManager->m_MouseXPos <= xx+m_pSprWnd->CrashBox[p][0].right && m_pInputManager->m_MouseYPos <= yy+m_pSprWnd->CrashBox[p][0].bottom)
		{
			if(m_pInputManager->m_isLButton == true)
			{
				//npcData[p].
				if(m_pTalkBox->npctxtnum != npcData[p].mss_num)
				{
					m_pTalkBox->talk_npc_txt_num = npcData[p].mss_num;
					m_pTalkBox->LoadTxt(p);
					//if(npcData[p].talk_two == TRUE)
					//	m_pTalkBox->mss_cl[p] = 1;
				}
				m_pTalkBox->npctxtnum = npcData[p].mss_num;
				m_pTalkBox->btn_ck = 1;
			}
		}
	}
}