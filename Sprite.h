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
	//��Ʈ�� ǥ���Լ�
	int         Hit_Spr_x,Hit_Spr_y;
	//���Ͻ���
	int         DropUnit; 
	//������ ����ð�
	int         Hptimes,Mptimes;

	char        damagetxt[5];
	//���۸��.......
	char        Turbo;
	int         TurboDelayTime;
	//Missail����
	int         mis;
	//����
	BOOL        SHOP;
	//�������� ����. Boss��,�ڵ� ��ũ���ǵ���......
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

