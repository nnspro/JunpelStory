#pragma once

#include "Direct3D.h"
#include "TextureData.h"
//#include "InputManager.h"
#include "TextOutput.h"

typedef struct
{
	int  box_x,box_y;
	int  send;
	int  max; 
	char box_btn;
	char box_input;
	char Box_Text[2][30];
}BOX;

class CMessageBox
{
public:
	CMessageBox(void);
	~CMessageBox(void);

	void Init(CDirect3D*,int,int,int,int);
	void DrawPic(int,int,CPicData *);
	void MousePosition();

	int Render();
public:
	BOX                 M_Box;

	char                number[3];
public:

	CDirect3D			*m_pDirect3d;
	CPicData            *m_pPicData;	
	CPicData            *m_pBtnPic[6];	

	CD3dxFont			*m_pD3dxfont;
};

