#include "Object.h"

#include <stdio.h>

RECT Crect;
int Pos=0,SX,SY;

char *REGqtl={"ªı¿Ã∏ß«¡∑Œ±◊∑• 2DObject µ•¿Ã≈Õ.."};

cObjectWnd::cObjectWnd()
{
	w_Width  = NULL;
	h_Height = NULL;

	aninum = objnum = stage = 0;

	ObjCount	=	0;
	aOBJ        =   NULL;
}

cObjectWnd::~cObjectWnd()
{
	if(pRGBAObjData)
	{
		for (int i=0;i<=objnum;i++)
		{
			SAFE_DELETE_ARRAY_(pRGBAObjData[i]);
		}
		SAFE_DELETE_ARRAY_(pRGBAObjData);
	}
}

void cObjectWnd::Init()
{
	pRGBAObjData      = NULL;
	
	//if(!(ObjLoad())return FALSE;
}

BOOL
cObjectWnd::ObjLoad()
{
	char Id[35],ID[15];
	char buf[256];
	int ver;

	ZeroMemory(Id,35);
	ZeroMemory(ID,15);

	FILE *ObjFile;

	sprintf(buf,"objects/obj_%d.msp",stage);
	if(!(ObjFile = fopen(buf,"rb")))return FALSE;

	fread(Id,  34, 1, ObjFile);
	if(strcmp(Id,REGqtl)!=0)
		return FALSE;
	
	fread(&ver, sizeof(int), 1, ObjFile);
	if(ver!=-98798)
		return FALSE;
	fread(&ver, sizeof(int), 1, ObjFile);
	if(ver != 980898)
		return FALSE;
	
	fread(ID, 1, 15, ObjFile);
	if (strcmp(ID,"——ÁÈ„’")!=0)
		return FALSE;

	fread(&aninum, 1, sizeof(int), ObjFile);

	pAni = new cAnimation [aninum];
	
	for(int i=0;i<aninum;i++)
	{
		fread(&pAni[i].FrameNum,1,sizeof(int),ObjFile);
		pAni[i].pFrame = new cFrame [pAni[i].FrameNum];
		for(int j=0;j<pAni[i].FrameNum;j++)
		{
			fread(&pAni[i].pFrame[j].Sprite,1,sizeof(int),ObjFile);	
			fread(&pAni[i].pFrame[j].XPos  ,1,sizeof(int),ObjFile);
			fread(&pAni[i].pFrame[j].YPos  ,1,sizeof(int),ObjFile);
		}
		fread(&pAni[i].Sort,1,sizeof(POINT),ObjFile);

	//pAni[i].frame_ = 0.0f;
		pAni[i].CurFrame = 0;
	}

	DWORD	dwBPP;
	BYTE    ImageType;
    BYTE    ImageDescriptor;

	fread(&objnum, sizeof(int), 1, ObjFile);

	pRGBAObjData = new DWORD*  [objnum+1];
	w_Width   = new DWORD   [objnum+1];
	h_Height  = new DWORD   [objnum+1];

for(int tt = 0; tt <= objnum; tt++)
{
	fread(&w_Width[tt],  sizeof(DWORD), 1, ObjFile);
	fread(&h_Height[tt], sizeof(DWORD), 1, ObjFile);
	
	pRGBAObjData[tt] = new DWORD  [w_Width[tt]*h_Height[tt]];
	
	fread(&dwBPP,    sizeof(DWORD), 1, ObjFile);
	fread(&ImageDescriptor,  sizeof(BYTE), 1, ObjFile);
	fread(&ImageType,  sizeof(BYTE), 1, ObjFile);
	
	for( DWORD y=0; y < h_Height[tt]; y++ )
    {
        DWORD dwOffset = y * w_Width[tt];

        if( 0 == ( ImageDescriptor & 0x0010 ) )
            dwOffset = (h_Height[tt]-y-1)*w_Width[tt];

        for( DWORD x=0; x<w_Width[tt]; x )
        {
            if( ImageType == 10 )
            {
                BYTE PacketInfo = getc( ObjFile );
                WORD PacketType = 0x80 & PacketInfo;
                WORD PixelCount = ( 0x007f & PacketInfo ) + 1;

                if( PacketType )
                {
                    DWORD b = getc( ObjFile );
                    DWORD g = getc( ObjFile );
                    DWORD r = getc( ObjFile );
                    DWORD a = 0xff;
                    if( dwBPP == 32 )
                        a = getc( ObjFile );

                    while( PixelCount-- )
                    {
						pRGBAObjData[tt][dwOffset+x] = (a<<24L | r<<16L | g<<8L | b);
                        x++;
                    }
                }
                else
                {
                    while( PixelCount-- )
                    {
                        BYTE b = getc( ObjFile );
                        BYTE g = getc( ObjFile );
                        BYTE r = getc( ObjFile );
                        BYTE a = 0xff;
                        if( dwBPP == 32 )
                            a = getc( ObjFile );

						pRGBAObjData[tt][dwOffset+x] = (a<<24L | r<<16L | g<<8L | b);
                        x++;
                    }
                }
            }
            else
            {
                BYTE b = getc( ObjFile );
                BYTE g = getc( ObjFile );
                BYTE r = getc( ObjFile );
                BYTE a = 0xff;
                if( dwBPP == 32 )
                    a = getc( ObjFile );

				pRGBAObjData[tt][dwOffset+x] = (a<<24L | r<<16L | g<<8L | b);
                x++;
            }
        }
    }
}
    fclose(ObjFile);

	return TRUE;
}

BOOL 
cObjectWnd::Load(FILE *fp)
{
int i;

	fread(&ObjCount,1,sizeof(int),fp);
	
	aOBJ = new ClassOBJECT [ObjCount];

	for(i=0;i<ObjCount;i++)
	{
		fread(&aOBJ[i].y	,1,sizeof(int),fp);
		fread(&aOBJ[i].num	,1,sizeof(int),fp);
		fread(&aOBJ[i].x	,1,sizeof(int),fp);
		
		aOBJ[i].DELAY_ = 0;
	}

return TRUE;
}

