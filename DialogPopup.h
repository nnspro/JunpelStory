#pragma once

#include <windows.h>
#include <stdio.h>

#include "Direct3D.h"
#include "TextureData.h"
#include "TextOutput.h"

typedef struct
{
	char *text;
	char **colortext;
	int  *color;
	int  *color_pos;
	int  *color_x,*color_y;
	int  *numtext;

	char ms;
	int  ms_ck;
}TalkData;

typedef struct
{
	char input;
	int  what;
	int  bs;
}QUEST_BONUS;

class CTalkBox
{
public:
	CTalkBox();
	~CTalkBox();

	void   Init(CDirect3D*);
	void   Render();
	void   DrawPic();
	void   DrawBtnPic(int,int,int);
	void   LoadTxt(int);
	void   MousePositions();
	void   CheckQ(int);
public:
	int talknum,colornum;
	int talk_box_x,talk_box_y;
	int btn_ck,talk_npc_txt_num;
	int npctxtnum;

	//char *mss_cl;

	TalkData    TkData;
	QUEST_BONUS *Q_bss;
public:
	CD3dxFont		 *m_pD3dxfont;

	CDirect3D        *m_pDirect3D;
	CPicData         *m_pPicData;	
	CPicData         *m_pBtnData[15];
};
