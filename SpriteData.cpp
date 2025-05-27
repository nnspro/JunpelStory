#include "SpriteData.h"

#include <stdio.h>

char *regqtl={"새이름프로그램 2DSprite 데이터.."};

cSprDataWnd::cSprDataWnd()
{
	w_Width  = NULL;
	h_Height = NULL;

	aninum = sprnum = 0;
}

cSprDataWnd::~cSprDataWnd()
{
	if(pRGBASprData)
	{
		for (int i=0;i<=sprnum;i++)
		{
			SAFE_RELEASE_(g_pSprTexture[i]);

			SAFE_DELETE_ARRAY_(pRGBASprData[i]);
		}
		SAFE_DELETE_ARRAY_(pRGBASprData);
	}
}

void cSprDataWnd::Init(CDirect3D *D3D)
{
	pRGBASprData      = NULL;
	g_pSprTexture     = NULL;
	
	m_pDirect3D       = D3D;

	//view = AniCount = 0;

	//if(!(ObjLoad())return FALSE;
}

BOOL
cSprDataWnd::SprLoad(int num)
{
	char Id[38],ID[15];
	char buf[256];
	int ver;

	ZeroMemory(Id,38);
	ZeroMemory(ID,15);

	FILE *SprFile;

	sprintf(buf,"sprite/spr_%d.asp",num);
	if(!(SprFile = fopen(buf,"rb")))return FALSE;

	fread(Id,  36, 1, SprFile);
	if(strcmp(Id,regqtl)!=0)
		return FALSE;
	
	fread(&ver, sizeof(int), 1, SprFile);
	if(ver!=-999878)
		return FALSE;
	fread(&ver, sizeof(int), 1, SprFile);
	if(ver != 997865)
		return FALSE;
	
	DWORD	dwBPP;
	BYTE    ImageType;
    BYTE    ImageDescriptor;

	fread(&sprnum, sizeof(int), 1, SprFile);

	pRGBASprData = new DWORD*  [sprnum+1];
	w_Width      = new DWORD   [sprnum+1];
	h_Height     = new DWORD   [sprnum+1];

	g_pSprTexture = new LPDIRECT3DTEXTURE9 [sprnum+1];

for(int tt = 0; tt <= sprnum; tt++)
{
	fread(&w_Width[tt],  sizeof(DWORD), 1, SprFile);
	fread(&h_Height[tt], sizeof(DWORD), 1, SprFile);
	
	pRGBASprData[tt] = new DWORD  [w_Width[tt]*h_Height[tt]];
	
	fread(&dwBPP,    sizeof(DWORD), 1, SprFile);
	fread(&ImageDescriptor,  sizeof(BYTE), 1, SprFile);
	fread(&ImageType,  sizeof(BYTE), 1, SprFile);
	
	for( DWORD y=0; y < h_Height[tt]; y++ )
    {
        DWORD dwOffset = y * w_Width[tt];

        if( 0 == ( ImageDescriptor & 0x0010 ) )
            dwOffset = (h_Height[tt]-y-1)*w_Width[tt];

        for( DWORD x=0; x<w_Width[tt]; x )
        {
            if( ImageType == 10 )
            {
                BYTE PacketInfo = getc( SprFile );
                WORD PacketType = 0x80 & PacketInfo;
                WORD PixelCount = ( 0x007f & PacketInfo ) + 1;

                if( PacketType )
                {
                    DWORD b = getc( SprFile );
                    DWORD g = getc( SprFile );
                    DWORD r = getc( SprFile );
                    DWORD a = 0xff;
                    if( dwBPP == 32 )
                        a = getc( SprFile );

                    while( PixelCount-- )
                    {
						pRGBASprData[tt][dwOffset+x] = (a<<24L | r<<16L | g<<8L | b);
                        x++;
                    }
                }
                else
                {
                    while( PixelCount-- )
                    {
                        BYTE b = getc( SprFile );
                        BYTE g = getc( SprFile );
                        BYTE r = getc( SprFile );
                        BYTE a = 0xff;
                        if( dwBPP == 32 )
                            a = getc( SprFile );

						pRGBASprData[tt][dwOffset+x] = (a<<24L | r<<16L | g<<8L | b);
                        x++;
                    }
                }
            }
            else
            {
                BYTE b = getc( SprFile );
                BYTE g = getc( SprFile );
                BYTE r = getc( SprFile );
                BYTE a = 0xff;
                if( dwBPP == 32 )
                    a = getc( SprFile );

				pRGBASprData[tt][dwOffset+x] = (a<<24L | r<<16L | g<<8L | b);
                x++;
            }
        }
    }
}
	fread(ID, 1, 15, SprFile);
	if (strcmp(ID,"金五植")!=0)
		return FALSE;

	fread(&aninum, 1, sizeof(int), SprFile);

	pAni = new cAnimation [aninum];
	
	for(int i=0;i<aninum;i++)
	{
		fread(&pAni[i].FrameNum,1,sizeof(int),SprFile);
		pAni[i].pFrame = new cFrame [pAni[i].FrameNum];
		for(int j=0;j<pAni[i].FrameNum;j++)
		{
			fread(&pAni[i].pFrame[j].XPos  ,1,sizeof(int),SprFile);
			fread(&pAni[i].pFrame[j].YPos  ,1,sizeof(int),SprFile);
			fread(&pAni[i].pFrame[j].Sprite,1,sizeof(int),SprFile);	
		}
		//pAni[i].frame_ = 0.0f;
		pAni[i].CurFrame = 0;

		/*if(i <= 1)
			pAni[i].Delay = 7;
		else if(i == 40)
			pAni[i].Delay = 2;
		else
			pAni[i].Delay = 1;*/
	}

	CrashBox = new RECT*[sprnum+1];
	hitbox   = new   int[sprnum+1];

	for(int i=0;i<=sprnum;i++)
	{
		fread(&hitbox[i],1,sizeof(int),SprFile);
		CrashBox[i] = new RECT[hitbox[i]];

		for(int j=0;j<hitbox[i];j++)
		{	
			fread(&CrashBox[i][j].right ,1,sizeof(int),SprFile);
			fread(&CrashBox[i][j].bottom,1,sizeof(int),SprFile);
			fread(&CrashBox[i][j].top   ,1,sizeof(int),SprFile);
			fread(&CrashBox[i][j].left  ,1,sizeof(int),SprFile);	
		}
	}

    fclose(SprFile);

	sprintf(buf,"txt/sprdelay/delay_%d.txt",num);
	if(!(SprFile = fopen(buf,"rt")))return FALSE;
	for(int i=0;i<aninum;i++)
	{
		fscanf(SprFile,"%d",&pAni[i].Delay);
	}
	fclose(SprFile);

	return TRUE;
}
void cSprDataWnd::LoadSprTexture()
{
	//SAFE_RELEASE_(g_pTexture);
	for(int pp=0;pp<=sprnum;++pp)
	{
		if ( FAILED( m_pDirect3D->GetDevice()->CreateTexture(w_Width[pp], h_Height[pp], 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &g_pSprTexture[pp], NULL) ) )
		{
	     return;
		}

		//	텍스쳐에 락을 걸어 TGA 데이터 직접 저장
		D3DLOCKED_RECT d3dlr;

		g_pSprTexture[pp]->LockRect( 0, &d3dlr, 0, 0 );
		DWORD*	pDst	= (DWORD *)d3dlr.pBits;
		int		DPitch	= d3dlr.Pitch/4;

		for ( DWORD y=0; y < h_Height[pp]; ++y )
		{
			for ( DWORD x=0; x < w_Width[pp]; ++x )
			{
				pDst[y*DPitch+x] = pRGBASprData[pp][ y*w_Width[pp]+x ];
			}
		}

		g_pSprTexture[pp]->UnlockRect(0);
	}
}