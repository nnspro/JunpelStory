#pragma once

#include <windows.h>
#include <stdio.h>

#include "Direct3D.h"
#include "TextureData.h"

class cMoveMagic
{
public:
	cMoveMagic();
	~cMoveMagic();

	void   Init(CDirect3D *);
	void   DrawPic(int,int,int,CPicData *);

	BOOL   Render();
	
private:
	int m,n;
	
	char few;
public:
	CDirect3D        *m_pDirect3D;

	CPicData         *m_pPicData;	
};
