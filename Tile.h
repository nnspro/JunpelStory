#pragma once

#include <windows.h>
#include <stdio.h>


class cTileWnd
{
public:
	CDirect3D           *m_pDirect3D;
public:
	DWORD       h_Height,w_Width;

	int         tilenum;
	
	DWORD**	pRGBATileData;

public:
	cTileWnd();
	~cTileWnd();

	BOOL Init();
	BOOL LoadTILEDATA();
};

extern cTileWnd *m_pTileWnd;
