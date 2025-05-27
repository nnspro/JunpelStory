#pragma once

#include <windows.h>

#include "Direct3D.h"

class CPicData
{
public:
	CDirect3D               *m_pDirect3D;

	DWORD			        *RGBAPicData;
	LPDIRECT3DTEXTURE9		g_pPicTexture;
public:
	DWORD            w_Width,h_Height;

public:
	CPicData(CDirect3D*);

	~CPicData();
	
	BOOL Load(char *);

	void LoadPicTexture();

};
