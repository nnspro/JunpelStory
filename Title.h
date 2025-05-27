#pragma once

#include <windows.h>
#include <stdio.h>

#include "Direct3D.h"
#include "TextureData.h"

class cTitle
{
public:
	cTitle();
	~cTitle();

	void   Init(CDirect3D*);
	void   DrawBbPic(int,int,int);
	void   DrawPic(int,int,int,int);
	void   MousePos();
	
	BOOL   Render();
public:
	CDirect3D        *m_pDirect3D;

	CPicData         *m_pPicData;
	CPicData         *BackPic[3];

private:
	char clk,clk1,ssnd;
	
};
