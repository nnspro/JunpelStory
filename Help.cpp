#include "Help.h"

int fonttime[5];
int help_num = 0;

cHelp *m_pHelp = NULL;

cHelp::cHelp(CDirect3D *pDirect3d)
{
	m_pDirect3d = pDirect3d;

	m_Color = 0xFFFFFFFF;
}

cHelp::~cHelp(void)
{
	SAFE_DELETE_(m_pD3dfont);
}

void cHelp::Init()
{
	for(int p=0;p<5;p++)
	{
		help_delay_time[p] =  0;
		fonttime[p]        = 20;
		ZeroMemory(Help_Message[p], 30);
	}
	m_pD3dfont = NULL;
	m_pD3dfont = new CD3dxFont();
	if(!m_pD3dfont->CreateFONT(m_pDirect3d->GetDevice(), "돋음", 15))
		return;
}

void cHelp::CreateHelp(char *text)
{
	for(int d=4;d>=1;d--)
	{
		ZeroMemory(Help_Message[d], 30);
		strcpy(Help_Message[d], Help_Message[d-1]);
		help_delay_time[d] =  help_delay_time[d-1];
		fonttime[d]        =  fonttime[d-1];
	}
	ZeroMemory(Help_Message[0], 30);
	strcpy(Help_Message[0],text);
	help_delay_time[0] =  0;
	fonttime[0]        = 20;
	//help_num++;
	//if(help_num > 9)help_num = 0;
}
void cHelp::Help_View()
{
	D3DCOLOR Color;
	DWORD Alpha;

	for(int p=0;p<5;p++)
	{
		if(strlen(Help_Message[p])>0)
		{
			help_delay_time[p] ++;
			if (help_delay_time[p] >= 10)
			{
				help_delay_time[p]=0;
				if (fonttime[p] != 0)
					fonttime[p]--;
			}

			if (fonttime[p] == 0)
			{
				fonttime[p] = 20;
				ZeroMemory(Help_Message[p],30);
			}
		
			Color = (m_Color & 0x00FFFFFF); //알파값을 0으로 
			if (fonttime[p] > 5)
				Alpha = 0xFF000000;
			else
				Alpha = (fonttime[p] + 2) << 29;
		
			Color |= (D3DCOLOR) Alpha;
			m_pD3dfont->DrawText(600, (p*17)+400, 0, 0, Color, Help_Message[p], strlen(Help_Message[p]));
		}
	}
}

