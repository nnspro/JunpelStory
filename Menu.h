#pragma once

#include "Direct3D.h"
#include "TextureData.h"
#include "Sprite.h"

class cMENU
{
public:
	cMENU();
	~cMENU();

	void Init(CDirect3D*,cSprWnd*);
	void DrawPic(int,int,CPicData *);
	void MousePosition();

	int Render();
public:
	char menu__;

	int menu_btn_clk,menu_btn_clk1,menu_btn_clk2;
	int menu_btn_ck;

public:

	CDirect3D			*m_pDirect3d;

	cSprWnd             *m_pSprWnd;

	CPicData            *m_pPicData;	
	CPicData            *m_pBtnPic[9];	
};

