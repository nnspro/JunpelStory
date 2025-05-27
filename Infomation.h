#pragma once

#include <windows.h>
#include <stdio.h>

#include "Direct3D.h"
#include "TextureData.h"
#include "TextOutput.h"

class CInfomations
{
public:
	CInfomations();
	~CInfomations();

	void   Init(CDirect3D*);
	void   Render();
	void   DrawPic(int,int,int,int);
	//void   LoadingTXT(int);
public:
	int Rendering;
	int Info_x,Info_y;
	int Info_p;

public:
	CD3dxFont		 *m_pD3dxfont;

	CDirect3D        *m_pDirect3D;
	CPicData         *m_pPicData;	
};
