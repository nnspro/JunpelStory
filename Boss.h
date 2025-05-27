#pragma once

#include <windows.h>
#include <stdio.h>

#include "Direct3D.h"
#include "SpriteData.h"
//#include "Effect.h"
#include "TextOutput.h"

#define  BOSS_STAND        0
#define  BOSS_HIT          2
#define  BOSS_DIE          4
#define  BOSS_ATTACK       6
#define  BOSS_ETC          8

class cBossWnd
{
public:
	int  enugy,str,dex,money,bonus,speed;
	int  boss_x,boss_y,boss_map_x,boss_map_y;
	int  COUNTER;

	char attrib,attack;
	char boss_move_check;
	char move_way,aninum;

	char turbo,turbotime;

public:
	CDirect3D   *m_pDirect3D;
	cSprDataWnd *m_pBossSprWnd;
	
	CD3dxFont	*m_pD3dxfont;
public:
	cBossWnd();
	~cBossWnd();	

	BOOL Init(CDirect3D *);
	BOOL InitData();

	void PutImage(int,int,int);
	void Render();
	void BossMove(int,int);
	
	int  HitBoss(RECT);
	int  AttackBoss(RECT,int,int);
	
	/*void ResetMonster(int);

	
	int HitItem(RECT);*/
};

