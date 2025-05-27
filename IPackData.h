#pragma once

#include <windows.h>

#include "Direct3D.h"

class CIpkData
{
public:
	CDirect3D               *m_pDirect3D;

	DWORD			        **RGBAIpkData;
	LPDIRECT3DTEXTURE9		*g_pIpkTexture;
public:
	int             ipknum;
	DWORD           *w_Width,*h_Height;

public:
	CIpkData(CDirect3D*);

	~CIpkData();
	
	BOOL Load(char *);

	void LoadPicTexture();
	void DrawPic(int x,int y,int num);
};
