#pragma once

#include <windows.h>
#include <stdio.h>

#include "Direct3D.h"
#include "Monster.h"
#include "Boss.h"
#include "Sprite.h"
#include "SpriteData.h"

//#define  EFFECT_STAND     0

struct Missail
{
	Missail *pNext;

	int msl_x;
	int msl_y;
	int map_msl_x;
	int map_msl_y;

	char type;
	char view;
	char who;

	int aninum;
	int count;
	
	int curframe;

	int monnum;
	
	Missail():pNext(NULL),msl_x(0),msl_y(0),map_msl_x(0),map_msl_y(0),
		type(0),aninum(0),view(0),count(0),curframe(0),monnum(0)
	{}
};
class cMissailWnd
{
public:
	Missail     *Missail_Buffer; 
	
public:
	CDirect3D   *m_pDirect3D;
	cSprDataWnd *m_pMissailSprWnd;

	cSprWnd     *m_pUser;
	cBossWnd    *m_pBss;
	cMonsterWnd *m_pMon;
public:
	cMissailWnd();
	~cMissailWnd();	

	
	void Init(CDirect3D *,cMonsterWnd *,cBossWnd *,cSprWnd *);
	void CreateMissail(int,int,int,int,int,int,int);
	void AddMissail(Missail *&, Missail *);
	void DeleteMissail(Missail *&, Missail *);
	void AllMissailDelete();
	void PutImage(int,int,int,int);
	void Render();
	
	int HitChk(RECT);
};
extern cMissailWnd      *m_pMissail;
