#include <stdio.h>

#include "MapOutput.h"
#include "IPackData.h"

char *REGIPK={"���̸����α׷� 2D������ ������.."};

AttItem         *A_Item       =     NULL;
HealthItem      *H_Item       =     NULL;
SkillItem       *S_Item       =     NULL;

BOOL 
CIpkData::Load(char *name)
{
	char Id[33];
	int ver;
	
	ZeroMemory(Id,33);

	FILE *PicFile,*NameFile;

	if(!(PicFile = fopen(name,"rb")))return FALSE;

	fread(Id,  32, 1, PicFile);
	if(strcmp(Id,REGIPK)!=0)
		return FALSE;
	fread(&ver, sizeof(int), 1, PicFile);
	if(ver!=-184473)
		return FALSE;
	fread(&ver, sizeof(int), 1, PicFile);
	if(ver != 7948998)
		return FALSE;
	
	while(1)
	{
		fread(&ver,sizeof(int),1,PicFile);
		if(ver == 9999)break;
	
		if(ver == -11)//���ݿ������...
		{
			NameFile = fopen("txt/item/itmname_1.txt","rt");

			fread(&ver,sizeof(int),1,PicFile);//�Ѱ���
		
			A_Item = new AttItem[ver];
		
			for(int p=0;p<ver;p++)
			{
				A_Item[p].checked = p;//��������ġ
				fread(&A_Item[p].itmnum,sizeof(int),1,PicFile);//������Sprite��ġ
				fread(&A_Item[p].buy,sizeof(int),1,PicFile);//�����ۻ�°���
				fread(&A_Item[p].sell,sizeof(int),1,PicFile);//�������Ĵ°���
				fread(&A_Item[p].att,sizeof(int),1,PicFile);//�����۰��ݷ�
				fread(&A_Item[p].level,sizeof(int),1,PicFile);//���������뷹��
				
				fscanf(NameFile,"%s",A_Item[p].itmname);//�������̸�
			}

			fclose(NameFile);
		}else if(ver == -33)//ȸ����
		{
			
			NameFile = fopen("txt/item/itmname_2.txt","rt");

			fread(&ver,sizeof(int),1,PicFile);//�Ѱ���
			
			H_Item = new HealthItem[ver];
		
			for(int p=0;p<ver;p++)
			{
				H_Item[p].checked = p;//��������ġ
				fread(&H_Item[p].itmnum,sizeof(int),1,PicFile);//������Sprite��ġ
				fread(&H_Item[p].buy,sizeof(int),1,PicFile);//�����ۻ�°���
				fread(&H_Item[p].sell,sizeof(int),1,PicFile);//�������Ĵ°���
				fread(&H_Item[p].hp,sizeof(int),1,PicFile);//������hpȸ����
				fread(&H_Item[p].mp,sizeof(int),1,PicFile);//������mpȸ����
			
				fscanf(NameFile,"%s",H_Item[p].itmname);//�������̸�
			}

			fclose(NameFile);
		}else if(ver == -44)//ȸ����
		{
			
			NameFile = fopen("txt/item/itmname_3.txt","rt");

			fread(&ver,sizeof(int),1,PicFile);//�Ѱ���
			
			S_Item = new SkillItem[ver];
		
			for(int p=0;p<ver;p++)
			{
				S_Item[p].checked = p;//��ų��ġ
				fread(&S_Item[p].itmnum,sizeof(int),1,PicFile);//��ųSprite��ġ
				fread(&S_Item[p].att,sizeof(int),1,PicFile);//��ų�Ӽ�
				fread(&S_Item[p].use,sizeof(int),1,PicFile);//��ų����
				fread(&S_Item[p].usemp,sizeof(int),1,PicFile);//���mp

				fscanf(NameFile,"%s",S_Item[p].itmname);//��ų�̸�
			}

			fclose(NameFile);
		}
	}

	fread(&ipknum, sizeof(int), 1, PicFile);

	DWORD	dwBPP;
	BYTE    ImageType;
	BYTE    ImageDescriptor;
	
	RGBAIpkData = new DWORD* [ipknum+1];
	w_Width     = new DWORD  [ipknum+1];
	h_Height    = new DWORD  [ipknum+1];

	g_pIpkTexture = new LPDIRECT3DTEXTURE9 [ipknum+1];

	for(int tt=0;tt<=ipknum;tt++)
	{
		
		fread(&w_Width[tt],  sizeof(DWORD), 1, PicFile);
		fread(&h_Height[tt], sizeof(DWORD), 1, PicFile);
	
		RGBAIpkData[tt] = new DWORD  [w_Width[tt]*h_Height[tt]];

		fread(&dwBPP,    sizeof(DWORD), 1, PicFile);
		fread(&ImageDescriptor,  sizeof(BYTE), 1, PicFile);
		fread(&ImageType,  sizeof(BYTE), 1, PicFile);
	
		for( DWORD y=0; y < h_Height[tt]; y++ )
		{
			DWORD dwOffset = y * w_Width[tt];

			if( 0 == ( ImageDescriptor & 0x0010 ) )
				dwOffset = (h_Height[tt]-y-1)*w_Width[tt];

			for( DWORD x=0; x<w_Width[tt]; x )
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
							RGBAIpkData[tt][dwOffset+x] = (a<<24L | r<<16L | g<<8L | b);
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

							RGBAIpkData[tt][dwOffset+x] = (a<<24L | r<<16L | g<<8L | b);
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

					RGBAIpkData[tt][dwOffset+x] = (a<<24L | r<<16L | g<<8L | b);	
					x++;
				}
			}
        }
    }

	fclose(PicFile);

return TRUE;
}
void CIpkData::LoadPicTexture()
{
	//SAFE_RELEASE_(g_pTexture);
	for(int tt=0;tt<=ipknum;tt++)
	{
		if ( FAILED( m_pDirect3D->GetDevice()->CreateTexture(w_Width[tt], h_Height[tt], 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &g_pIpkTexture[tt], NULL) ) )
			{
		     return;
			}

		//	�ؽ��Ŀ� ���� �ɾ� TGA ������ ���� ����
		D3DLOCKED_RECT d3dlr;

		g_pIpkTexture[tt]->LockRect( 0, &d3dlr, 0, 0 );
		DWORD*	pDst	= (DWORD *)d3dlr.pBits;
		int		DPitch	= d3dlr.Pitch/4;

		for ( DWORD y=0; y < h_Height[tt]; ++y )
		{
			for ( DWORD x=0; x < w_Width[tt]; ++x )
			{
				pDst[y*DPitch+x] = RGBAIpkData[tt][ y*w_Width[tt]+x ];
			}
		}
		g_pIpkTexture[tt]->UnlockRect(0);
	}
}
void
CIpkData::DrawPic(int x,int y,int num)
{

    //	Sprite�� ���� ���̴� ����
	m_pDirect3D->GetDevice()->SetTexture( 0, g_pIpkTexture[num]);
			
	CUSTOM_VERTEX*	pV = NULL;

	m_pMapViewWnd->g_pVB->Lock( 0, 4*sizeof(CUSTOM_VERTEX), (void**)&pV, D3DLOCK_DISCARD );

	for( int m=0; m < 4; m++ )
	{
      pV[m].vPos.z = 1.0f;
	  pV[m].rhw = 1.0f;
	  pV[m].dwDiffuse = 0xffffffff;
	}
		

	float	fLeft   = (float)x;
	float	fWidth  = (float)w_Width[num];
	float	fTop    = (float)y;
	float	fHeight = (float)h_Height[num];

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
CIpkData::CIpkData(CDirect3D *D3D)
{
	m_pDirect3D   = D3D;
	
	RGBAIpkData   = NULL;
	g_pIpkTexture = NULL;

	ipknum        = 0;
}

CIpkData::~CIpkData()
{
	if(RGBAIpkData)
	{
		for (int i=0;i<=ipknum;i++)
		{
			SAFE_RELEASE_(g_pIpkTexture[i]);

			SAFE_DELETE_ARRAY_(RGBAIpkData[i]);
		}
		SAFE_DELETE_ARRAY_(RGBAIpkData);
	}
}