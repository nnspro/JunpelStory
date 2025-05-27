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
	
	//int number;//Monster �̸� ��ȣ
	int AniNumber;//Monster ���ϸ��̼� ��ȣ
	int speed;//�̵��ӵ�

	int attrib;//������?
	int attack;//�����̳�?�ƴϳ�?
	
	int mon_x;//Monster ��ǥ
	int mon_y;

	int mon_X;//Monster Tile��ǥ
	int mon_Y;
			  //Monster ��Ʈ�� ǥ��	
	int  mon_hit_txt_x;
	int  mon_hit_txt_y;
	char damagetxt[5]; 

	int str;//���ݷ�
	int dex;//����

	//int level;//Monster����
	int bonus;//����ġ

	int money;//�ִ� ��
	int enugy;//Monster ������

	int reset;//���½ð�
	//=======================
	int  move_check;//������üũ
	int  move_sem;//�����Ӱ��
	int  _COUNTER;//���ϸ��̼���ġ
	
	char hitting;//���ݴ������� �ڷ� ����.
	
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

