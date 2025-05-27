#pragma once

#include <windows.h>
#include <stdio.h>

#include "Direct3D.h"
#include "SpriteData.h"
#include "DialogPopup.h"

typedef struct
{
	BOOL  talk_two;

	int npc_x;
	int npc_y;
	int mss_num;
	int spr_num;
	int btn_ck;
}NpcData;

class cNpc
{
public:
	//int npctxtnum;
	int npcnum;

	NpcData     *npcData;
public:
	CDirect3D   *m_pDirect3D;
	cSprDataWnd *m_pSprWnd;
	CTalkBox    *m_pTalkBox; 
public:
	cNpc(CDirect3D*);
	~cNpc();	

	void Init();
	void LoadNpcTxt();
	void DrawPic(int x, int y, int i);

	void Render();	
	
	void Mouseposition();
};

