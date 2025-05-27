#pragma once

#include <windows.h>
#include <stdio.h>

#include "Direct3D.h"
#include "Object.h"

class cSprDataWnd
{
public:
	int         sprnum,aninum,*hitbox;

	RECT		**CrashBox;
	
	DWORD       *h_Height,*w_Width;
	DWORD**	    pRGBASprData;

public:
	CDirect3D   *m_pDirect3D;
	
	cAnimation  *pAni;
	LPDIRECT3DTEXTURE9		*g_pSprTexture;
	
public:
	cSprDataWnd();
	~cSprDataWnd();	

	void Init(CDirect3D*);
	void LoadSprTexture(); 

	BOOL SprLoad(int num);
};

