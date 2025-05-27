#pragma once

#include <windows.h>
#include <stdio.h>

#include "Direct3D.h"
#include "TextureData.h"

typedef struct {	
	int total;
	int actions;
	int endlineminus;
	int endlineplus;
}BackINFO;
typedef struct{
	int back_x;
	int back_y;
	int speed;
}BackInfo;//, *LPBackInfo;

class cScreenManager
{
public:
	cScreenManager();
	~cScreenManager();

	void   Init(CDirect3D*);
	void   Render();
	void   DrawBbPic(int,int,int);
	void   DrawPic(int,int,int,int);
	
//	void   UnitMove(int news);
public:
	BackINFO *B_INFO;
	BackInfo **B_Info;

	//int show;
public:
	CDirect3D        *m_pDirect3D;

	CPicData         *m_pPicData;
	CPicData         **BackPic;
	
};
