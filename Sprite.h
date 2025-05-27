#pragma once

#include <windows.h>
#include <stdio.h>

#include "Pet.h"
#include "Direct3D.h"

typedef struct
{
	int num;
	int itemnum;

	int checked;

	//int attrib;
	//int 
}ITEMUser;

class cSprWnd
{
public:
	char        Name[13];

	int         spr_x,spr_y;
	int         AniCount;
	int         view,y_;
	int         Hp,totalHp;
	int         Mp,totalMp;
	int         Kyunghum,totalKyunghum;
	int         Level,Money;
	int         Str,Dex,Int,Mana,Hana;
	int         stackPoint;
	//히트량 표시함수
	int         Hit_Spr_x,Hit_Spr_y;
	//낙하시점
	int         DropUnit; 
	//에너지 차기시간
	int         Hptimes,Mptimes;

	char        damagetxt[5];
	//슈퍼모드.......
	char        Turbo;
	int         TurboDelayTime;
	//Missail여부
	int         mis;
	//가게
	BOOL        SHOP;
	//스테이지 구분. Boss판,자동 스크롤판등등등......
	char        stageSelect;
public:
	ITEMUser Interface_[8];

	ITEMUser User_Itm_Att[10];
	ITEMUser User_Itm_Etc[30];
	ITEMUser User_Itm_Booster[5];
public:
	cSprWnd();
	~cSprWnd();	

	void Init();

    cPetWnd          *m_pPet;	
};

