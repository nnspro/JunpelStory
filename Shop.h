#pragma once

#include <windows.h>
#include <stdio.h>

#include "Direct3D.h"
#include "TextureData.h"
#include "TextOutput.h"
//#include "ItemInfo.h"

class cShop
{
public:
	cShop();
	~cShop();

	void   Init(CDirect3D*,int,int);
	void   DrawPic(int,int,CPicData*);
	void   MousePosition();
	
	int    Render();
	
public:
	int    shop_x,shop_y;
	int    itm_view;
	int    listnum,*pst;

	int    btn_0,btn_1,btn_2,btn_3,btn_4,btn_5,btn_6;
	int    btn_clk,shop_itm_list,shop_user_list;
	int    shop_itm_pos,shop_user_pos;
	int    ret;

	char   info_item;
public:
	CDirect3D        *m_pDirect3D;

	CPicData         *m_pShopPicData;	
	CPicData         *m_pShopBtnData[15];	
	CPicData         *m_pShopChoice;

	//CItemInfomations *m_pItemInfo;

	CD3dxFont		 *m_pD3dxfont;
};
