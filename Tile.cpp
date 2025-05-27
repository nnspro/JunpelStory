#include "Object.h"
#include "Tile.h"

#include <commctrl.h>

char *regQTL={"새이름프로그램 2DTile 데이터.."};

cTileWnd::cTileWnd()
{
	h_Height = w_Width = 0;
	tilenum = 0;
}
BOOL
cTileWnd::Init()
{
	pRGBATileData      = NULL;

	//if(LoadTILEDATA()==FALSE)return FALSE;

	return TRUE;
}

cTileWnd::~cTileWnd()
{
	if(pRGBATileData)
	{
		for (int i=0;i<=tilenum;i++)
		{
			SAFE_DELETE_ARRAY_(pRGBATileData[i]);
		}
		SAFE_DELETE_ARRAY_(pRGBATileData);
	}
}
BOOL
cTileWnd::LoadTILEDATA()
{
	char Id[31];
	char buf[256];
	int ver;

	ZeroMemory(Id,31);

	FILE *TileFile;

	sprintf(buf,"tiles/tile_%d.ttl",m_pObjectWnd->stage);
	if(!(TileFile = fopen(buf,"rb")))return FALSE;

	fread(Id,  30, 1, TileFile);
	if(strcmp(Id,regQTL)!=0)
		return FALSE;
	
	fread(&ver, sizeof(int), 1, TileFile);
	if(ver!=-5555)
		return FALSE;
	fread(&ver, sizeof(int), 1, TileFile);
	if(ver != 9999)
		return FALSE;
	fread(&tilenum, sizeof(int), 1, TileFile);

	DWORD	dwBPP;
	BYTE    ImageType;
    BYTE    ImageDescriptor;

	fread(&w_Width,  sizeof(DWORD), 1, TileFile);
	fread(&h_Height, sizeof(DWORD), 1, TileFile);
	
	pRGBATileData = new DWORD*  [tilenum+1];

for(int tt = 0; tt <= tilenum; tt++)
{
	pRGBATileData[tt] = new DWORD  [w_Width*h_Height];

	fread(&dwBPP,    sizeof(DWORD), 1, TileFile);
	fread(&ImageDescriptor,  sizeof(BYTE), 1, TileFile);
	fread(&ImageType,  sizeof(BYTE), 1, TileFile);
	
	for( DWORD y=0; y < h_Height; y++ )
    {
        DWORD dwOffset = y * w_Width;

        if( 0 == ( ImageDescriptor & 0x0010 ) )
            dwOffset = (h_Height-y-1)*w_Width;

        for( DWORD x=0; x<w_Width; x )
        {
            if( ImageType == 10 )
            {
                BYTE PacketInfo = getc( TileFile );
                WORD PacketType = 0x80 & PacketInfo;
                WORD PixelCount = ( 0x007f & PacketInfo ) + 1;

                if( PacketType )
                {
                    DWORD b = getc( TileFile );
                    DWORD g = getc( TileFile );
                    DWORD r = getc( TileFile );
                    DWORD a = 0xff;
                    if( dwBPP == 32 )
                        a = getc( TileFile );

                    while( PixelCount-- )
                    {
						pRGBATileData[tt][dwOffset+x] = (a<<24L | r<<16L | g<<8L | b);
                        x++;
                    }
                }
                else
                {
                    while( PixelCount-- )
                    {
                        BYTE b = getc( TileFile );
                        BYTE g = getc( TileFile );
                        BYTE r = getc( TileFile );
                        BYTE a = 0xff;
                        if( dwBPP == 32 )
                            a = getc( TileFile );

						pRGBATileData[tt][dwOffset+x] = (a<<24L | r<<16L | g<<8L | b);
                        x++;
                    }
                }
            }
            else
            {
                BYTE b = getc( TileFile );
                BYTE g = getc( TileFile );
                BYTE r = getc( TileFile );
                BYTE a = 0xff;
                if( dwBPP == 32 )
                    a = getc( TileFile );

				pRGBATileData[tt][dwOffset+x] = (a<<24L | r<<16L | g<<8L | b);
                x++;
            }
        }
    }
}
    fclose(TileFile);

	return TRUE;
}
