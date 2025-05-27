#pragma once

#include <windows.h>
#include <stdio.h>

#include "Direct3D.h"
#include "TextureData.h"
#include "Sprite.h"
#include "Infomation.h"
#include "StatusForm.h"
#include "TextOutput.h"

class CInterface
{
public:
	CInterface();
	~CInterface();

	void   Init(CDirect3D*,cSprWnd *);
	void   Render();
	void   DrawPic(int,int,int,CPicData*);
	
	void   MousePosition();
	void   DelEff();
public:
	int checked;
	
	char info_view;
	//char itm_info_view;
public:
	CDirect3D        *m_pDirect3D;

	cSprWnd          *m_pSprWnd;
	//cStateMessage    *m_pStateMss;
	CInfomations     *m_pInfo;  
	
	CPicData         *m_pHeadPic;
	CPicData         *m_pPicData;
	CPicData         *m_pHpPic;
	CPicData         *m_pMpPic;
	CPicData         *m_pPointPic;
	
	CD3dxFont		 *m_pD3dxfont;
};
