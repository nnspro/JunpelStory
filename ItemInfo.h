#pragma once

#include <windows.h>
#include <stdio.h>

#include "Direct3D.h"
#include "TextureData.h"
#include "TextOutput.h"

class CItemInfomations
{
public:
	CItemInfomations();
	~CItemInfomations();

	void   Init(CDirect3D*);
	void   Render();
	void   DrawPic(int,int,int,int);
	void   LoadingTXT(int,int,int,int,int);
public:
	int    i_info_x,i_info_y;
	int    i_info_scale_x,i_info_scale_y;
	int    i_itm,i_itm_num;
	int    i_info_txt;

	char   **TT;
	char   item_view;

public:
	CD3dxFont		 *m_pD3dxfont;
	CD3dxFont		 *m_pD3dxfont_1;

	CDirect3D        *m_pDirect3D;
	CPicData         *m_pPicData;	
};
