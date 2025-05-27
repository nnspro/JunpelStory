#pragma once

#include <windows.h>

#include "Direct3D.h"
#include "TextureData.h"
#include "MapData.h"

#define FVF_CUSTOMVERTEX	( D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 )
#define Abs(x) ((x) < 0 ? -(x) : (x))

extern int Compare(tINDEX *a,tINDEX *b);
extern BOOL IsNumeric(char* szData);
extern int gameState;

template <class A, class B> inline A Max(A a, B b)
{
    if (a > b) return a;
    else return b;
}

typedef struct _CustomVertex
{
	D3DXVECTOR3		vPos;
	float			rhw;
	DWORD			dwDiffuse;
	float			tu, tv;
} CUSTOM_VERTEX;

typedef struct
{
	char itmname[16];
	int  itmnum;
	int  checked;
	int  buy;
	int  sell;
	int  att;
	int  level;
}AttItem;

typedef struct
{
	char itmname[16];
	int  itmnum;
	int  checked;
	int  buy;
	int  sell;
	int  hp;
	int  mp;
}HealthItem;

typedef struct
{
	char itmname[16];
	int  itmnum;
	int  checked;
	int  att;
	int  use;
	int  usemp;
}SkillItem;

extern AttItem         *A_Item;
extern HealthItem      *H_Item;
extern SkillItem       *S_Item;

class cMapViewWnd
{
public:
	HWND      h_hndW;
	HINSTANCE h_hinsT;

	LPDIRECT3DVERTEXBUFFER9	g_pVB;
	LPDIRECT3DTEXTURE9		*g_pTileTexture;
	LPDIRECT3DTEXTURE9		*g_pObjTexture;
	
	int map_X,map_Y;
	int MAP_x,MAP_y;
	int map_x,map_y;
	int Map_x,Map_y;

	int show;
	
	BOOL user_life;

	int game_map_stage,game_map_prev;
	int q_npc;
	//퀘스트용함수선언
	char quest, *mss_cl;
	

	//int spr_user_xx,spr_user_yy;
	//int print_map_x,print_map_y;
public:
	ClassMAPDATA			*pMAP;

	CPicData                *m_pUserPic;

	CDirect3D               *m_pDirect3D;
public:
	cMapViewWnd(CDirect3D *);
	~cMapViewWnd();

	BOOL Init();

	void InitPos();
  	void DelData();
	void DelDATA();
	void InitData();
	void InitDATA();
	void DrawQTILE(int x,int y,int tile);
	void DrawMiniQTILE(int x,int y,int tile,int scrollx,int scrolly);
	void DrawObject(int,int,int,int);
	void DrawMiniObject(int,int,int);
	void DrawMiniUserPic(int,int);

	void BackTile();
	void FrontTile();
	void MiniMap_BackTile(int start_x,int start_y);
	void MiniMap_FrontTile(int start_x,int start_y);

	BOOL MapDataLoad();

	void CreateVB();
	void LoadTileTexture();
	void LoadAttTexture();
	void LoadObjTexture();

	void MoveMap();
	//void Thread();
	
};

extern cMapViewWnd *m_pMapViewWnd;

