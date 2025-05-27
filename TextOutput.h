#pragma once

#include <d3dx9.h>
#include <d3d9.h>

class CD3dxFont  
{
private:
	ID3DXFont*	m_pFont;
//	LPD3DXFONT	m_pFont;
	RECT		m_rect;
	
public:
	CD3dxFont();
	virtual ~CD3dxFont();
	
	bool	CreateFONT(LPDIRECT3DDEVICE9, char *, int);
	void	DrawTextLine(int, int, int, int, DWORD, char *,int);
	void	DrawText(int, int, int, int, DWORD, char *,int);
	
};