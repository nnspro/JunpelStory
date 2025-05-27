#pragma once

#include <windows.h>
#include <stdio.h>

#include "Direct3D.h"
#include "SpriteData.h"

#define  EFFECT_STAND     0

struct Ecffect
{
	Ecffect *pNext;

	int eff_x;
	int eff_y;
	int map_eff_x;
	int map_eff_y;

	int count;

	int aninum;
	int times;
	int speed;

	int how_much;
	Ecffect():pNext(NULL),eff_x(0),eff_y(0),map_eff_x(0),map_eff_y(0),
		count(0),aninum(0),times(0),how_much(0),speed(0)
	{}
};/*
struct ITEM
{
	ITEM *pNext;

	int checked;
	int num;
	int itmnum;
	
	int c_x;
	int c_y;
	int map_c_x;
	int map_c_y;
	
	int times;
	
	ITEM():pNext(NULL),checked(0),num(0),itmnum(-1),
		c_x(0),c_y(0),map_c_x(0),map_c_y(0),times(0)
	{}
};
*/
class cEcffectWnd
{
public:
	Ecffect     *Ecffect_Buffer; 
	Ecffect     *Ecffect_Money; 
	Ecffect     *Ecffect_Item; 
	Ecffect     Ecffect_Temp[2];
public:
	CDirect3D   *m_pDirect3D;
	cSprDataWnd *m_pEcffectSprWnd[2];

public:
	cEcffectWnd();
	~cEcffectWnd();	

	
	void Init(CDirect3D *);
	void CreateHit(int,int,int);
	void CreateMoney(int,int,int,int);
	void CreateItem(int,int,int,int,int,int);
	void AddEcffect(Ecffect *&, Ecffect *);
	void DeleteEcffect(Ecffect *&, Ecffect *);
	void AllEffectDelete();
	void PutImage(int,int,int,int);
	void Render();
	
	int ItemChk(RECT);
	
};

