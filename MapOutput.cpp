#include "MapOutput.h"
#include "MiniMap.h"
#include "Object.h"
#include "Tile.h"

#include <commctrl.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

LPCTSTR		StageHeader		= "새이름프로그램 2D맵 데이터.";

float frame__ = 0.0f;

int Compare(tINDEX *a,tINDEX *b)
{
	return (a->Value-b->Value);
}
//숫잔지 문자인지 판별
BOOL IsNumeric(char* szData)
{
    int nLen = strlen(szData);
    if ( nLen <= 0 )
        return FALSE;

    for ( int ii = 0; ii < nLen; ii++ )
    {
        if ( *(szData + ii) < '0' || *(szData + ii) > '9' )
            return FALSE;
    }
    return TRUE;
}


cMapViewWnd::cMapViewWnd(CDirect3D *D3D)
{
	pMAP      = NULL;
	m_pUserPic = NULL;

	m_pDirect3D = D3D;

	q_npc = show           = -1;
	
	quest = game_map_stage = 0;
	game_map_prev          = 0;

	user_life = FALSE;
}

cMapViewWnd::~cMapViewWnd()
{
	SAFE_DELETE_(pMAP);
	SAFE_RELEASE_(g_pVB);
	SAFE_DELETE_(m_pUserPic);
	for(int hh=0;hh<=m_pTileWnd->tilenum;hh++)
		SAFE_RELEASE_(g_pTileTexture[hh]);

	for(int hh=0;hh<=m_pObjectWnd->objnum;hh++)
		SAFE_RELEASE_(g_pObjTexture[hh]);
}
void 
cMapViewWnd::DelData()
{
	for(int hh=0;hh<=m_pTileWnd->tilenum;hh++)
		SAFE_RELEASE_(g_pTileTexture[hh]);

	for(int hh=0;hh<=m_pObjectWnd->objnum;hh++)
		SAFE_RELEASE_(g_pObjTexture[hh]);
}
void 
cMapViewWnd::InitData()
{
	g_pTileTexture = new LPDIRECT3DTEXTURE9 [m_pTileWnd->tilenum+1];
	g_pObjTexture = new LPDIRECT3DTEXTURE9 [m_pObjectWnd->objnum+1];
}

BOOL
cMapViewWnd::Init()
{
	pMAP      = new ClassMAPDATA();
	
	InitPos();

	g_pVB = NULL;
	
	InitData();
	
	CreateVB();

	m_pUserPic = new CPicData(m_pDirect3D);
	m_pUserPic->Load("pic/minimap_user.pic");
	m_pUserPic->LoadPicTexture();

	return TRUE;
}

void 
cMapViewWnd::InitPos()
{
	pMAP->TPW		= m_pTileWnd->w_Width;
	pMAP->TPH		= m_pTileWnd->h_Height;

	map_X = map_Y = 0;
}

void
cMapViewWnd::DelDATA()
{
	if (pMAP->DATA)
	{
		DelData();

		for(int i=0;i<pMAP->HEIGHT;i++) SAFE_DELETE_ARRAY_(pMAP->DATA[i]);
		SAFE_DELETE_ARRAY_(pMAP->DATA);
	
		for(int i=0;i<(pMAP->HEIGHT*4);i++) SAFE_DELETE_ARRAY_(pMAP->ATT[i]);
		SAFE_DELETE_ARRAY_(pMAP->ATT);
	}

}
void
cMapViewWnd::InitDATA()
{
int i;

	pMAP->DATA=new int *[pMAP->HEIGHT];
	pMAP->ATT =new int *[pMAP->HEIGHT*4];

	for (i=0;i<pMAP->HEIGHT;i++)
	{
		pMAP->DATA[i]=new int [pMAP->WIDTH];
		for (int j=0;j<pMAP->WIDTH;j++)	pMAP->DATA[i][j]=999;
	}
	for (i=0;i<(pMAP->HEIGHT*4);i++)
	{
		pMAP->ATT[i]=new int [pMAP->WIDTH*4];
		for (int j=0;j<(pMAP->WIDTH*4);j++)	pMAP->ATT[i][j]=0;
	}
}
void 
cMapViewWnd::DrawQTILE(int x,int y,int tile)
{
	CUSTOM_VERTEX*	pV;
    
	//if (tile==999) return;	
	//if ( (NULL == g_pVB))
	//	return;

		m_pDirect3D->GetDevice()->SetTexture( 0,g_pTileTexture[tile] );
			
		pV = NULL;

		g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

		for( int i=0; i < 4; i++ )
	    {
    	  pV[i].vPos.z = 1.0f;
	      pV[i].rhw = 1.0f;
	      pV[i].dwDiffuse = 0xffffffff;
		}
								
		float	fLeft   = (float)x-(float)map_X;
		float	fWidth  = (float)m_pTileWnd->w_Width;
		float	fTop    = (float)y-(float)map_Y;
		float	fHeight = (float)m_pTileWnd->h_Height;

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
				
		g_pVB->Unlock();

		m_pDirect3D->GetDevice()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
}
void 
cMapViewWnd::DrawMiniQTILE(int x,int y,int tile,int scrollx,int scrolly)
{
	CUSTOM_VERTEX*	pV;
    
	//if (tile==999) return;	
	//if ( (NULL == g_pVB))
	//	return;

		m_pDirect3D->GetDevice()->SetTexture( 0,g_pTileTexture[tile] );
			
		pV = NULL;

		g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

		for( int i=0; i < 4; i++ )
	    {
    	  pV[i].vPos.z = 1.0f;
	      pV[i].rhw = 1.0f;
	      pV[i].dwDiffuse = 0xffffffff;
		}
		float	fLeft,fTop;
								
		if(scrollx == 0)fLeft   = (float)(x-(map_X/8));
		else fLeft   = (float)x;
		float	fWidth  = (float)m_pTileWnd->w_Width/8;
		if(scrolly == 0)fTop    = (float)(y-(map_Y/8));
		else	fTop    = (float)y;
		float	fHeight = (float)m_pTileWnd->h_Height/8;

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
				
		g_pVB->Unlock();

		m_pDirect3D->GetDevice()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
}
void
cMapViewWnd::DrawMiniObject(int x,int y,int i)
{
    //	Sprite에 사용될 쉐이더 설정
	m_pDirect3D->GetDevice()->SetTexture( 0, 
									 g_pObjTexture[m_pObjectWnd->pAni[i].pFrame[m_pObjectWnd->pAni[i].CurFrame].Sprite] );
			
	CUSTOM_VERTEX*	pV = NULL;

	g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

	for( int m=0; m < 4; m++ )
	{
      pV[m].vPos.z = 1.0f;
	  pV[m].rhw = 1.0f;
	  pV[m].dwDiffuse = 0xffffffff;
	}
		

	float	fLeft   = (float)x;
	float	fWidth  = (float)(m_pObjectWnd->w_Width[m_pObjectWnd->pAni[i].pFrame[m_pObjectWnd->pAni[i].CurFrame].Sprite]/8);
	float	fTop    = (float)y;
	float	fHeight = (float)(m_pObjectWnd->h_Height[m_pObjectWnd->pAni[i].pFrame[m_pObjectWnd->pAni[i].CurFrame].Sprite]/8);

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
		
	g_pVB->Unlock();

	m_pDirect3D->GetDevice()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
}
void
cMapViewWnd::DrawMiniUserPic(int x,int y)
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
		

	float   fLeft   = (float)x;
	float	fWidth  = (float)m_pUserPic->w_Width;
	float	fTop    = (float)y;
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
}
void
cMapViewWnd::DrawObject(int x,int y,int i,int d)
{
    //	Sprite에 사용될 쉐이더 설정
	m_pDirect3D->GetDevice()->SetTexture( 0, 
									 g_pObjTexture[m_pObjectWnd->pAni[i].pFrame[m_pObjectWnd->pAni[i].CurFrame].Sprite] );
			
	CUSTOM_VERTEX*	pV = NULL;

	g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

	for( int m=0; m < 4; m++ )
	{
      pV[m].vPos.z = 1.0f;
	  pV[m].rhw = 1.0f;
	  pV[m].dwDiffuse = 0xffffffff;
	}
	

	m_pObjectWnd->aOBJ[d].DELAY_++;
	m_pObjectWnd->pAni[i].CurFrame = (m_pObjectWnd->aOBJ[d].DELAY_/5)%m_pObjectWnd->pAni[i].FrameNum;

	float	fLeft   = (float)x;
	float	fWidth  = (float)m_pObjectWnd->w_Width[m_pObjectWnd->pAni[i].pFrame[m_pObjectWnd->pAni[i].CurFrame].Sprite];
	float	fTop    = (float)y;
	float	fHeight = (float)m_pObjectWnd->h_Height[m_pObjectWnd->pAni[i].pFrame[m_pObjectWnd->pAni[i].CurFrame].Sprite];

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

	g_pVB->Unlock();

	m_pDirect3D->GetDevice()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
}
void
cMapViewWnd::BackTile()
{
	int x=-32,y=-32,tx,ty,tile;
	
	if (!pMAP->DATA)	return;
	
	for(ty=(Map_y-1);ty<(Map_y+20);ty++)
	{
		for(tx=(Map_x-1);tx<(Map_x+26);tx++)
		{
			if(tx >= 0 && ty >= 0)
			{
				tile  =pMAP->GetTile(tx,ty);
				if(tile >= 0 && tile <= m_pTileWnd->tilenum)
					DrawQTILE(x,y,tile);
			}
			x+=32;
		}
		y+=32;x=-32;
	}
}

void
cMapViewWnd::FrontTile()
{
	int x=-32,y=-32,tx,ty,tile;
	
	if (!pMAP->DATA)	return;
	
	for(ty=(Map_y-1);ty<(Map_y+20);ty++)
	{
		for(tx=(Map_x-1);tx<(Map_x+26);tx++)
		{
			if(tx >= 0 && ty >= 0)
			{
				tile =pMAP->GetFTile(tx,ty);

				if(tile != 0)
					DrawQTILE(x,y,tile);
			}
			x+=32;
		}
		y+=32;x=-32;
	}
}
void
cMapViewWnd::MiniMap_BackTile(int start_x,int start_y)
{
	int x=start_x-4,y=start_y-4,scx=0,scy=0;
	int tx,ty,tile,mp_x=Map_x,mp_y=Map_y;
	
	if (!pMAP->DATA)	return;
	if (mp_x >= (pMAP->WIDTH-41))
	{
		mp_x = (pMAP->WIDTH-41);
		scx = 1;
	}
	if (mp_y >= (pMAP->HEIGHT-35))
	{
		mp_y = (pMAP->HEIGHT-35);
		scy = 1;
	}
	for(ty=(mp_y-1);ty<(mp_y+35);ty++)
	{
		for(tx=(mp_x-1);tx<(mp_x+41);tx++)
		{
			if(tx >= 0 && ty >= 0)
			{
				tile  =pMAP->GetTile(tx,ty);
				if(tile >= 0 && tile <= m_pTileWnd->tilenum)
					DrawMiniQTILE(x,y,tile,scx,scy);
			}
			x+=4;
		}
		y+=4;x=start_x-4;
	}
}

void
cMapViewWnd::MiniMap_FrontTile(int start_x,int start_y)
{
	
	int x=start_x-4,y=start_y-4,scx=0,scy=0;
	int tx,ty,tile,mp_x=Map_x,mp_y=Map_y;
	
	if (!pMAP->DATA)	return;
	if (mp_x >= (pMAP->WIDTH-41))
	{
		mp_x = (pMAP->WIDTH-41);
		scx = 1;
	}
	if (mp_y >= (pMAP->HEIGHT-35))
	{
		mp_y = (pMAP->HEIGHT-35);
		scy = 1;
	}
	for(ty=(mp_y-1);ty<(mp_y+35);ty++)
	{
		for(tx=(mp_x-1);tx<(mp_x+41);tx++)
		{
			if(tx >= 0 && ty >= 0)
			{
				tile  =pMAP->GetFTile(tx,ty);
				if(tile != 0)
					DrawMiniQTILE(x,y,tile,scx,scy);
				if(tx == ((m_pMiniMap->Mini_Map_User_x*8)/32) &&
					ty == ((m_pMiniMap->Mini_Map_User_y*8)/32))
					DrawMiniUserPic(x,y-8);
			}
			x+=4;
		}
		y+=4;x=start_x-4;
	}
}
BOOL
cMapViewWnd::MapDataLoad()
{
	FILE *fp;
	int a,b;//,c;
	char SH[30],bb[256];

	ZeroMemory(SH,30);

	sprintf(bb,"maps/map_%d.mmf",game_map_stage);
	if(!(fp=fopen(bb,"rb")))return FALSE;

	fread(SH,27,1,fp);
	if (strcmp(SH,StageHeader)!=0)
	{
		return FALSE;
	}
	fread(&a,1,sizeof(int),fp);
	if (a!=5421)
	{
		fclose(fp);
		return FALSE;
	}
	fread(&b,1,sizeof(int),fp);
	if (b!=9867)
	{
		fclose(fp);
		return FALSE;
	}
	DelDATA();

	fread(&pMAP->HEIGHT,1,sizeof(int),fp);
	fread(&pMAP->WIDTH, 1,sizeof(int),fp);

	pMAP->WWIDTH  = m_pTileWnd->w_Width *pMAP->WIDTH ;
	pMAP->WHEIGHT = m_pTileWnd->h_Height*pMAP->HEIGHT;

	InitDATA();
	
	for (int i=0;i<(pMAP->HEIGHT*4);i++)
		//for(int ii=0;ii<();ii++)
			fread(pMAP->ATT[i],pMAP->WIDTH*4,sizeof(int),fp);
	for (int i=0;i<pMAP->HEIGHT;i++) fread(pMAP->DATA[i],pMAP->WIDTH,sizeof(int),fp);
	
	fread(&m_pObjectWnd->stage,1,sizeof(int),fp);

	if(!(m_pTileWnd->LoadTILEDATA()))return FALSE;
   
	//_pObjectWnd->DelData();
	m_pObjectWnd->Load(fp);
	m_pObjectWnd->ObjLoad();

	fclose(fp);
	
	InitData();
	
	LoadTileTexture();
	LoadObjTexture();
	
	return TRUE;
}

/*===========================
	Desc:
		버텍스 버퍼 생성
===========================*/
void cMapViewWnd::CreateVB()
{
	m_pDirect3D->GetDevice()->CreateVertexBuffer( 4*sizeof(CUSTOM_VERTEX),
									D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
									FVF_CUSTOMVERTEX,
									D3DPOOL_DEFAULT, &g_pVB, NULL );
}




/*===========================
	Desc:
		텍스쳐 생성
		생성된 텍스쳐에 락을 걸어 RGBA 데이터를 직접 저장한다.
===========================*/
void cMapViewWnd::LoadTileTexture()
{
	//SAFE_RELEASE_(g_pTexture);
	for(int pp=0;pp<=m_pTileWnd->tilenum;++pp)
	{
		if ( FAILED( m_pDirect3D->GetDevice()->CreateTexture(m_pTileWnd->w_Width, m_pTileWnd->h_Height, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &g_pTileTexture[pp], NULL) ) )
		{
	     return;
		}

		//	텍스쳐에 락을 걸어 TGA 데이터 직접 저장
		D3DLOCKED_RECT d3dlr;

		g_pTileTexture[pp]->LockRect( 0, &d3dlr, 0, 0 );
		DWORD*	pDst	= (DWORD *)d3dlr.pBits;
		int		DPitch	= d3dlr.Pitch/4;

		for ( DWORD y=0; y < m_pTileWnd->h_Height; ++y )
		{
			for ( DWORD x=0; x < m_pTileWnd->w_Width; ++x )
			{
				pDst[y*DPitch+x] = m_pTileWnd->pRGBATileData[pp][ y*m_pTileWnd->w_Width+x ];
			}
		}

		g_pTileTexture[pp]->UnlockRect(0);
	}
}
void cMapViewWnd::LoadObjTexture()
{
	//SAFE_RELEASE_(g_pAttTexture);
	for(int pp=0;pp<=m_pObjectWnd->objnum;++pp)
	{
		if ( FAILED( m_pDirect3D->GetDevice()->CreateTexture(m_pObjectWnd->w_Width[pp], m_pObjectWnd->h_Height[pp], 
			0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &g_pObjTexture[pp], NULL) ) )
		{
		     return;
		}

		//	텍스쳐에 락을 걸어 TGA 데이터 직접 저장
		D3DLOCKED_RECT d3dlr;

		g_pObjTexture[pp]->LockRect( 0, &d3dlr, 0, 0 );
		DWORD*	pDst	= (DWORD *)d3dlr.pBits;
		int		DPitch	= d3dlr.Pitch/4;

		for ( DWORD y=0; y < m_pObjectWnd->h_Height[pp]; ++y )
		{
			for ( DWORD x=0; x < m_pObjectWnd->w_Width[pp]; ++x )
			{
				pDst[y*DPitch+x] = m_pObjectWnd->pRGBAObjData[pp][ y*m_pObjectWnd->w_Width[pp]+x ];
			}
		}
        
		g_pObjTexture[pp]->UnlockRect(0);
	}
}