#pragma once

#include <windows.h>
#include <stdio.h>

#include "Direct3D.h"

#include "Npc.h"
#include "Monster.h"
#include "Boss.h"

#include "MoveMagic.h"

#include "SpriteData.h"
#include "Sprite.h"

#define     UNIT_STAND         0
#define     UNIT_WALK          2
#define     UNIT_ATTACK        4
#define     UNIT_HIT           6
#define     UNIT_DIE           8
#define     UNIT_UP            9
#define     UNIT_DOWN         10
#define     UNIT_JUMP_ATTACK  13
#define     UNIT_SITDOWN      15
#define     UNIT_HOLL_ATTACK  17
#define     UNIT_SLIDE        19
#define     UNIT_UP_WALK      21
#define     UNIT_RUN          22 
#define     UNIT_SKILL        24
#define     UNIT_BUSTER       26
#define     UNIT_GUN          28
#define     UNIT_NIFE         30 
#define     UNIT_BUMERANG     32
#define     UNIT_DANGER       34
#define     UNIT_POOL         36
#define     UNIT_SWMING       38
#define     UNIT_DOWN_WALK    40

class cUnitManager
{
public:
	cUnitManager();
	~cUnitManager();

	void   Init(/*HWND,*/CDirect3D*);
	void   AllInit();
	void   FrameCheck(/*float*/);
	void   DrawSpr(int,int,int);
	void   KeyFrame();
	void   Animation();
	void   InitBoss();
	//BOOL Init_JunePle_Game(HWND);

	void   UserUp(int);
	void   UserDown(int);
	void   UserLeft(int);
	void   UserRight(int);

	void   AttackCheck();
	//키 입력 변수
	char   KeyState;

public:
	int _COUNTER;
	
	char rn;
	//float frame;
public:
	CDirect3D        *m_pDirect3D;
	
	cSprDataWnd      *m_pSprDataWnd;
	cSprWnd          *m_pUserSpr;
	cNpc             *m_pNpc;
	cMonsterWnd      *m_pMonster;
	cMoveMagic       *m_pMagic;

	cBossWnd         *m_pBoss;

	CD3dxFont		 *m_pD3dxfont;
};
