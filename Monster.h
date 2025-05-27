#pragma once

#include <windows.h>
#include <stdio.h>

#include "Direct3D.h"
#include "SpriteData.h"
//#include "Effect.h"
#include "TextOutput.h"

#define  MON_STAND        0
//#define  MON_STAND_1      2
#define  MON_HIT          2
#define  MON_DIE          4

typedef struct// MonsterData
{
	//MonsterData *pNext;

	cSprDataWnd *m_pMonSprWnd;
	
	//int number;//Monster 이름 번호
	int AniNumber;//Monster 에니메이션 번호
	int speed;//이동속도

	int attrib;//움직임?
	int attack;//선공이냐?아니냐?
	
	int mon_x;//Monster 좌표
	int mon_y;

	int mon_X;//Monster Tile좌표
	int mon_Y;
			  //Monster 히트량 표시	
	int  mon_hit_txt_x;
	int  mon_hit_txt_y;
	char damagetxt[5]; 

	int str;//공격력
	int dex;//방어력

	//int level;//Monster레벨
	int bonus;//경험치

	int money;//주는 돈
	int enugy;//Monster 에너지

	int reset;//리셋시간
	//=======================
	int  move_check;//움직임체크
	int  move_sem;//움직임계산
	int  _COUNTER;//에니메이션위치
	
	char hitting;//공격당했을시 뒤로 빠짐.
	
/*	MonsterData():pNext(NULL),AniNumber(0),speed(0),attrib(0),attack(0),
		mon_x(0),mon_y(0),mon_X(0),mon_Y(0),mon_hit_txt_x(0),mon_hit_txt_Y(0),
		str(0),dex(0),bonus(0),money(0),enugy(0),reset(0),
		move_check(0),move_sem(0),_COUNTER(0)
	{}*/
}MonsterData;

class cMonsterWnd
{
public:
	int monTotalnum,mondatATotalnum;
public:
	CDirect3D   *m_pDirect3D;
	//cSprDataWnd **m_pMonSprWnd;
			
	MonsterData *Mon;
	
	CD3dxFont	*m_pD3dxfont;
public:
	cMonsterWnd();
	~cMonsterWnd();	

	BOOL Init(CDirect3D *);
	BOOL InitData();

	void PutImage(int,int,int,int);
	void Render();
	void EmyMove(int,int,int);
	
	void ResetMonster(int);

//	void AddMonster(MonsterData *&, MonsterData *);
//	void DeleteMNonster(MonsterData *&, MonsterData *);

	int HitMonster(RECT);
	int AttackMonster(RECT,int,int);
	int HitItem(RECT);
};

