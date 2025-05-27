#include <stdio.h>

#include "TextureData.h"

char *REGQTL={"새이름프로그램 2D그림 데이터.."};

BOOL 
CPicData::Load(char *name)
{
	char Id[31];
	int ver;
	
	ZeroMemory(Id,31);

	FILE *PicFile;

	if(!(PicFile = fopen(name,"rb")))return FALSE;

	fread(Id,  30, 1, PicFile);
	if(strcmp(Id,REGQTL)!=0)
		return FALSE;
	fread(&ver, sizeof(int), 1, PicFile);
	if(ver!=-78987)
		return FALSE;
	fread(&ver, sizeof(int), 1, PicFile);
	if(ver != 365887)
		return FALSE;
	

	DWORD	dwBPP;
	BYTE    ImageType;
    BYTE    ImageDescriptor;

	fread(&w_Width,  sizeof(DWORD), 1, PicFile);
	fread(&h_Height, sizeof(DWORD), 1, PicFile);
	
	RGBAPicData = new DWORD  [w_Width*h_Height];

	fread(&dwBPP,    sizeof(DWORD), 1, PicFile);
	fread(&ImageDescriptor,  sizeof(BYTE), 1, PicFile);
	fread(&ImageType,  sizeof(BYTE), 1, PicFile);
	
	for( DWORD y=0; y < h_Height; y++ )
    {
        DWORD dwOffset = y * w_Width;

        if( 0 == ( ImageDescriptor & 0x0010 ) )
            dwOffset = (h_Height-y-1)*w_Width;

        for( DWORD x=0; x<w_Width; x )
        {
            if( ImageType == 10 )
            {
                BYTE PacketInfo = getc( PicFile );
                WORD PacketType = 0x80 & PacketInfo;
                WORD PixelCount = ( 0x007f & PacketInfo ) + 1;

                if( PacketType )
                {
                    DWORD b = getc( PicFile );
                    DWORD g = getc( PicFile );
                    DWORD r = getc( PicFile );
                    DWORD a = 0xff;
                    if( dwBPP == 32 )
                        a = getc( PicFile );

                    while( PixelCount-- )
                    {
						RGBAPicData[dwOffset+x] = (a<<24L | r<<16L | g<<8L | b);
                        x++;
                    }
                }
                else
                {
                    while( PixelCount-- )
                    {
                        BYTE b = getc( PicFile );
                        BYTE g = getc( PicFile );
                        BYTE r = getc( PicFile );
                        BYTE a = 0xff;
                        if( dwBPP == 32 )
                            a = getc( PicFile );

						RGBAPicData[dwOffset+x] = (a<<24L | r<<16L | g<<8L | b);
                        x++;
                    }
                }
            }
            else
            {
                BYTE b = getc( PicFile );
                BYTE g = getc( PicFile );
                BYTE r = getc( PicFile );
                BYTE a = 0xff;
                if( dwBPP == 32 )
                    a = getc( PicFile );

				RGBAPicData[dwOffset+x] = (a<<24L | r<<16L | g<<8L | b);
                x++;
            }
        }
    }
    fclose(PicFile);

return TRUE;
}
void CPicData::LoadPicTexture()
{
	//SAFE_RELEASE_(g_pTexture);
	if ( FAILED( m_pDirect3D->GetDevice()->CreateTexture(w_Width, h_Height, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &g_pPicTexture, NULL) ) )
		{
	     return;
		}

	//	텍스쳐에 락을 걸어 TGA 데이터 직접 저장
	D3DLOCKED_RECT d3dlr;

	g_pPicTexture->LockRect( 0, &d3dlr, 0, 0 );
	DWORD*	pDst	= (DWORD *)d3dlr.pBits;
	int		DPitch	= d3dlr.Pitch/4;

	for ( DWORD y=0; y < h_Height; ++y )
	{
		for ( DWORD x=0; x < w_Width; ++x )
		{
			pDst[y*DPitch+x] = RGBAPicData[ y*w_Width+x ];
		}
	}
	g_pPicTexture->UnlockRect(0);
}
CPicData::CPicData(CDirect3D *D3D)
{
	m_pDirect3D    = D3D;
	
	g_pPicTexture  = NULL;
	RGBAPicData    = NULL;
}

CPicData::~CPicData()
{
	SAFE_RELEASE_(g_pPicTexture);
	
	SAFE_DELETE_ARRAY_(RGBAPicData);
}