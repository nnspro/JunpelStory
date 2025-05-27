#pragma once

#include <windows.h>
#include <stdio.h>

#include "Direct3D.h"
#include "TextOutput.h"
#include "TextureData.h"

class cMiniMap
{
public:
	cMiniMap();
	~cMiniMap();

	void   Init(CDirect3D*);
	void   Render();
	void   DrawPic();
	void   DrawBackPic();
	//void   DrawUserPic();
	void   DrawBtnPic(int,int,int);

	void   BtnDraw();
	void   MousePosition();

	void   LoadVillige();
public:
	float Mini_Map_x,Mini_Map_y; 
	int   Mini_Map_State;
	int   Mini_Map_User_x,Mini_Map_User_y;

	char villige[15];
	char **QUEST;
	char quest_num;
public:
	CDirect3D        *m_pDirect3D;
	CPicData         *m_pPicData[3];
	CPicData         *m_pBackPic;
	//CPicData         *m_pUserPic;
	CPicData         *m_pBtnPic[8];
	
	CD3dxFont		 *m_pD3dxBigfont,*m_pD3dxSmallfont;
};

extern cMiniMap *m_pMiniMap;