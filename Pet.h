#pragma once

#include <windows.h>
#include <stdio.h>

#include "Direct3D.h"
#include "SpriteData.h"
//#include "Effect.h"
//#include "TextOutput.h"

#define  PET_STAND        0

class cPetWnd
{
public:
	//int  enugy,str,dex,money,bonus,speed;
	int  pet_x,pet_y,user_y;//,boss_map_x,boss_map_y;
	int  COUNTER;

	char aninum;//move_way,
	/*char attrib,attack;
	char boss_move_check;
	
	char turbo,turbotime;*/

public:
	CDirect3D   *m_pDirect3D;
	cSprDataWnd *m_pPetSprWnd;
	
public:
	cPetWnd();
	~cPetWnd();	

	void Init(CDirect3D *);
	
	void PutImage(int,int,int);
	void Render();
	void PetMove();
		
	void InitPet();
	/*void ResetMonster(int);

	
	int HitItem(RECT);*/
};

