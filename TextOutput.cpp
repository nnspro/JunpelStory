
#include "Direct3D.h"
#include "TextOutput.h"

CD3dxFont::CD3dxFont()
{
	m_pFont = NULL;
}

CD3dxFont::~CD3dxFont()
{
	SAFE_RELEASE_(m_pFont);
}

bool 
CD3dxFont::CreateFONT(LPDIRECT3DDEVICE9 pD3ddev, char *fontname, int sizefont)
{
	LOGFONT logfont;
		
	logfont.lfWeight = FW_BOLD;
	logfont.lfHeight = sizefont;
	logfont.lfCharSet = HANGEUL_CHARSET;
	logfont.lfOutPrecision = 3;
	logfont.lfClipPrecision = 2;
	logfont.lfQuality = 1;
	logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	//strcpy(logfont.lfFaceName,fontname);

	if(E_FAIL == (D3DXCreateFontIndirect(pD3ddev, (D3DXFONT_DESCA *)&logfont, &m_pFont)))
	{
		return false;
	}
	
	return true;
}
void 
CD3dxFont::DrawTextLine(int left, int top, int right, int bottom, DWORD color, char *str,int MaxCount)
{	
	SetRect(&m_rect, left, top, right, bottom);
	
	//m_pFont->GetDC();
	m_pFont->DrawText(NULL,str, MaxCount, &m_rect, DT_LEFT | DT_WORDBREAK, color);
	
//	m_pFont->DeleteDC();
}
void 
CD3dxFont::DrawText(int left, int top, int right, int bottom, DWORD color, char *str,int MaxCount)
{	
	SetRect(&m_rect, left, top, right, bottom);
	
	//m_pFont->GetDC();
	m_pFont->DrawText(NULL,str, MaxCount, &m_rect, DT_LEFT | DT_NOCLIP, color);
	
//	m_pFont->DeleteDC();
}