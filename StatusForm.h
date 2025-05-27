#pragma once

#include <windows.h>
#include <stdio.h>

#include "Direct3D.h"
#include "TextureData.h"
#include "Sprite.h"
#include "IPackData.h"
#include "TextOutput.h"
#include "Effect.h"
#include "ItemInfo.h"

class cStateMessage
{
public:
	cStateMessage();
	~cStateMessage();

	void   Init(CDirect3D *,cSprWnd *);
	void   Render();
	
	void   DrawPic(int x,int y,int num);
	void   DrawBtnPic(int x,int y,int num);
    void   DrawChoicePic(int x,int y);

	void   MousePosition(int ,int);
public:
	int state_num;
	int scroll_y;
	int choice_click,click_choice;
	int botton_show;

	char info_item;
public:
	CDirect3D        *m_pDirect3D;

	CD3dxFont	     *m_pD3dxfont;

	CIpkData         *m_pIpkData;
	cSprWnd          *m_pSprWnd;

	cEcffectWnd *m_pEcffect; 
	
	CItemInfomations *m_pItemInfo;
	
	CPicData         *m_pPicData[3];	
	CPicData         *m_pChoiceData;	
	CPicData         *m_pBtnData[9];	
};

extern cStateMessage    *m_pStateMss;