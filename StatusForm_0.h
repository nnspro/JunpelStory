#pragma once

#include "Direct3D.h"
#include "TextureData.h"
#include "Sprite.h"
#include "TextOutput.h"

class cState_
{
public:
	cState_();
	~cState_();

	void Init(CDirect3D*,cSprWnd*);
	void DrawPic(int,int,CPicData *);
	void MousePosition();

	int Render();
public:
	int scroll,x_sc,send_;

	int state_btn_clk,state_btn_clk1,state_btn_clk2,state_btn_clk3;
	int state_ck;
	int plus_btn0,plus_btn1,plus_btn2,plus_btn3,plus_btn4;

public:

	CDirect3D			*m_pDirect3d;
	
	cSprWnd             *m_pSprWnd;
	CD3dxFont	    	*m_pD3dxfont;

	CPicData            *m_pPicData[2];	
	CPicData            *m_pBtnPic[12];	
};

