#pragma once

#include <d3dx9.h>
#include <d3d9.h>

#define SAFE_DELETE_(p)			{if(p) {delete (p);     (p)=NULL; }}
#define SAFE_DELETE_ARRAY_(p)	{if(p) {delete[] (p);   (p)=NULL; }}
#define SAFE_RELEASE_(p)		{if(p) {(p)->Release(); (p)=NULL; }}

typedef struct tagINDEX
{
	int		Value;
	int		Index;
} tINDEX;

class CDirect3D 
{
private:
	LPDIRECT3D9			m_pD3d;
	LPDIRECT3DDEVICE9	m_pD3ddev;
	
private:
	void				Release();
	
public:
	CDirect3D();
	virtual ~CDirect3D();

	bool				Init3D(HWND, int, int, bool);
	bool				SetUpVertexBuffer(LPDIRECT3DVERTEXBUFFER9 &, void *, DWORD, UINT);
	bool				SetUpIndexBuffer(LPDIRECT3DINDEXBUFFER9 &, void *, UINT);
	bool				SetUpTexture(LPDIRECT3DTEXTURE9 &, char *);
	bool				SetUpTexture(LPDIRECT3DTEXTURE9 &, char *, DWORD);
	void				ColorKeyEnable();
	void				ColorKeyDisable();
	void				AlphaBlendEnable(int);
	void				AlphaBlendDisable();
	void				SetTextureBase(int, LPDIRECT3DTEXTURE9 &);
	void				SetTextureBaseForDiffuse(int, LPDIRECT3DTEXTURE9 &);
	void				SetTextureLightMap(int, LPDIRECT3DTEXTURE9 &);
	void				SetTextureDetailMap(int, LPDIRECT3DTEXTURE9 &);
	LPDIRECT3DDEVICE9	GetDevice();
};
