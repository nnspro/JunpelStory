#pragma once

#include <windows.h>
#include <stdio.h>

#include "Direct3D.h"

class cFrame
{
public:
	int Sprite,XPos,YPos;
};

class cAnimation
{
public:
	int		FrameNum;
	int     CurFrame;
	int     Delay;

	POINT   Sort;
	cFrame	*pFrame;

};

class ClassOBJECT
{
public:
	int		x,y,num;
	
	int     DELAY_;
};

class cObjectWnd
{
public:
	
public:
	
	int         objnum,aninum,ObjCount;
	int         stage;
	
	DWORD       *h_Height,*w_Width;
	
	DWORD**	    pRGBAObjData;
	
	cAnimation  *pAni;
	ClassOBJECT	*aOBJ;

public:
	cObjectWnd();
	~cObjectWnd();	

	void Init();
	BOOL ObjLoad();
	
	BOOL Load(FILE *fp);
	
};

extern cObjectWnd *m_pObjectWnd;
