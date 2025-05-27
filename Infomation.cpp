#include "MapOutput.h"
#include "InputManager.h"
#include "Infomation.h"

int total_size_x=0,total_size_y=0,info_num=0;
char **buffer;

CInfomations::CInfomations()
{
	m_pDirect3D = NULL;
	m_pPicData  = NULL;

	Rendering = -1;
	Info_p = Info_x = Info_y = 0;
}

CInfomations::~CInfomations()
{
	SAFE_DELETE_(m_pD3dxfont);
	SAFE_DELETE_(m_pPicData);
}

void CInfomations::Init(CDirect3D *D3D)
{
	m_pDirect3D = D3D;

	m_pPicData = new CPicData(D3D);
	m_pPicData->Load("pic/info.pic");
	m_pPicData->LoadPicTexture();
	
	m_pD3dxfont = NULL;
	m_pD3dxfont = new CD3dxFont();
	if(!m_pD3dxfont->CreateFONT(m_pDirect3D->GetDevice(), (char*)GetStockObject(SYSTEM_FONT), 16))
		return;
}
void
CInfomations::Render()
{
	if(Rendering >= 0)
	{
		if(Info_p == 0)
		{
			int nn,i;
			char buf[256];
			FILE *info;
			
			for (i=0;i<info_num;i++) 
			{
				if(buffer[i])
				{
					SAFE_DELETE_ARRAY_(buffer[i]);
				}
			}
			SAFE_DELETE_ARRAY_(buffer);

			sprintf(buf,"txt/interface/%d.txt",Rendering);
			info = fopen(buf,"rt");

			fscanf(info,"%d",&info_num);
	        buffer = new char *[info_num];
			//for(int p = 0;p < num;p++)
			//	buffer[num] = new char [50];
			fscanf(info,"%d",&Info_x);
			fscanf(info,"%d",&Info_y);
			fscanf(info,"%d",&total_size_x);
			fscanf(info,"%d",&total_size_y);
			//while(NULL != fgets(buffer[num], 50, info))
			for(int p=0;p<info_num;p++)
			{
				fscanf(info,"%d",&nn);
				buffer[p] = new char [nn];

				fgets(buffer[p],nn,info);
			/*buffer[p][strlen(buffer[p]) - 1] = 0;//패치 내용 읽기
				int a = strlen(buffer[p]);
				//total_size_y = Max(a,total_size_y);

			//	num++;*/
			}
			fclose(info);
		}
		m_pDirect3D->AlphaBlendEnable(150);
		DrawPic(Info_x,Info_y,total_size_x,total_size_y);
		m_pDirect3D->AlphaBlendDisable();
		
		for(int pp=0;pp<info_num;pp++)
			m_pD3dxfont->DrawText(Info_x+10,(Info_y+10)+(pp*20),0,0,0xffffffff,
				buffer[pp],strlen(buffer[pp]));

		Info_p=1;
	}
}
void
CInfomations::DrawPic(int x,int y,int size_x,int size_y)
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