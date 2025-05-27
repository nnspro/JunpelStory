#pragma once

#include "Direct3D.h"
#include "TextOutput.h"

class cHelp
{
private:
	CDirect3D			*m_pDirect3d;

	CD3dxFont			*m_pD3dfont;
	D3DCOLOR            m_Color;

public:
	char Help_Message[5][30];
	int  help_delay_time[5]; 
public:
	cHelp(CDirect3D *);
	~cHelp(void);

	void Init();
	void Help_View();
	void CreateHelp(char *);
	
	

};

extern cHelp *m_pHelp;
