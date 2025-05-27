#include "MapOutput.h"
#include "BGManager.h"

int file;

cScreenManager::cScreenManager()
{
	m_pDirect3D = NULL;
	m_pPicData  = NULL;

	//show = -1;
}

cScreenManager::~cScreenManager()
{
	//SAFE_DELETE_(m_pPicData);
}

void cScreenManager::Init(CDirect3D *D3D)
{
	char buf[256];

	m_pDirect3D = D3D;

	FILE *screen;

	sprintf(buf,"txt/stage/stage_%d.txt",m_pMapViewWnd->game_map_stage);
	screen = fopen(buf,"rt");
	
	fscanf(screen,"%s",buf);

	m_pPicData = new CPicData(D3D);
	m_pPicData->Load(buf);
	m_pPicData->LoadPicTexture();
	
	fscanf(screen,"%d",&file);
	
	B_INFO  = new BackINFO [file];
	B_Info  = new BackInfo*[file];
	BackPic = new CPicData*[file];

	for(int p=0;p<file;p++)
	{
		fscanf(screen,"%d",&B_INFO[p].total);
		fscanf(screen,"%d",&B_INFO[p].actions);
		fscanf(screen,"%d",&B_INFO[p].endlineminus);
		fscanf(screen,"%d",&B_INFO[p].endlineplus);

		B_Info[p]  = new BackInfo[B_INFO[p].total];
		
		for(int pp=0;pp<B_INFO[p].total;pp++)
		{
			fscanf(screen,"%d",&B_Info[p][pp].back_x);
			fscanf(screen,"%d",&B_Info[p][pp].back_y);
			fscanf(screen,"%d",&B_Info[p][pp].speed);
		}
		fscanf(screen,"%s",buf);
		
		BackPic[p] = new CPicData(D3D);
        BackPic[p]->Load(buf);
		BackPic[p]->LoadPicTexture();
	}
	fclose(screen);
}
void
cScreenManager::Render()
{
	DrawPic(0,0,800,600);
	
	for(int p=0;p<file;p++)
	{
		if(B_INFO[p].actions == 1)
		{
			for(int pp=0;pp<B_INFO[p].total;pp++)
			{
				DrawBbPic(B_Info[p][pp].back_x,B_Info[p][pp].back_y,p);
			
				if(B_Info[p][pp].back_x < B_INFO[p].endlineminus)B_Info[p][pp].back_x = 800;
				B_Info[p][pp].back_x -= B_Info[p][pp].speed;
			}
		}
		else if(B_INFO[p].actions == 2)
		{
			for(int pp=0;pp<B_INFO[p].total;pp++)
			{
				DrawBbPic(B_Info[p][pp].back_x,B_Info[p][pp].back_y,p);
				if(m_pMapViewWnd->show == 1)
				{
					
					if(B_Info[p][pp].back_x <= B_INFO[p].endlineminus)B_Info[p][pp].back_x = 800;
					B_Info[p][pp].back_x -= B_Info[p][pp].speed;
				}else if(m_pMapViewWnd->show == 0)
				{
					
					if(B_Info[p][pp].back_x >= B_INFO[p].endlineplus)B_Info[p][pp].back_x = B_INFO[p].endlineminus;
					B_Info[p][pp].back_x += B_Info[p][pp].speed;
				}
			}
		}else if(B_INFO[p].actions == 3)
		{
			for(int pp=0;pp<B_INFO[p].total;pp++)
			{
				DrawBbPic(B_Info[p][pp].back_x,B_Info[p][pp].back_y,p);
			}
		}
	}
	m_pMapViewWnd->show = -1;
}
void
cScreenManager::DrawBbPic(int x,int y,int num)
{
    //	Sprite에 사용될 쉐이더 설정
m_pDirect3D->GetDevice()->SetTexture( 0, 
									 BackPic[num]->g_pPicTexture);
			
	CUSTOM_VERTEX*	pV = NULL;

	m_pMapViewWnd->g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

	for( int m=0; m < 4; m++ )
	{
      pV[m].vPos.z = 1.0f;
	  pV[m].rhw = 1.0f;
	  pV[m].dwDiffuse = 0xffffffff;
	}
		

	float	fLeft   = (float)x;
	float	fWidth  = (float)BackPic[num]->w_Width;
	float	fTop    = (float)y;
	float	fHeight = (float)BackPic[num]->h_Height;

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
cScreenManager::DrawPic(int x,int y,int Zoom_x,int Zoom_y)
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
	float	fWidth  = (float)Zoom_x;
	float	fTop    = (float)y;
	float	fHeight = (float)Zoom_y;

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