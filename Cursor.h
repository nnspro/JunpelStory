#pragma once

#include <windows.h>
#include <stdio.h>

#include "Direct3D.h"
#include "TextureData.h"

class cMouseCursor
{
public:
	cMouseCursor();
	~cMouseCursor();

	void   Init(CDirect3D*);
	void   Render();
	void   DrawPic(float,float);
	
public:
public:
	CDirect3D        *m_pDirect3D;

	CPicData         *m_pPicData;	
};
