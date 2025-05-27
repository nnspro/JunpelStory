//#include "stdafx.h"

#include "Direct3D.h"

CDirect3D::CDirect3D()
{
	m_pD3d = NULL;
	m_pD3ddev = NULL;
}

CDirect3D::~CDirect3D()
{
	Release();
}

bool CDirect3D::Init3D(HWND hWnd, int width, int height,  bool winmode)
{
	if(NULL == (m_pD3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		return false;
	}
	
	D3DDISPLAYMODE d3ddm;
	if(FAILED(m_pD3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))	
	{
		return false;
	}
	
	D3DPRESENT_PARAMETERS d3dpp;	
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	if(winmode)
	{
		d3dpp.Windowed = true;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = d3ddm.Format;
		d3dpp.EnableAutoDepthStencil = true;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	}
	else if(!winmode)
	{		
		d3dpp.Windowed = false;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = d3ddm.Format;
		d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
		d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		d3dpp.BackBufferCount = 1;
		d3dpp.FullScreen_RefreshRateInHz = d3ddm.RefreshRate;//나중에는 60hz로 하자..
		d3dpp.EnableAutoDepthStencil = true;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		//전체화면에서 화면 주사율에 상관없이 렌더링하기... 이놈이 속도가 더 빠르다..
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp./*FullScreen_*/PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	
	if(FAILED(m_pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_ADAPTERGROUP_DEVICE , &d3dpp, &m_pD3ddev)))
	{
		return false;
	}
	
	m_pD3ddev->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
	m_pD3ddev->SetRenderState( D3DRS_ZWRITEENABLE, 0x00000000L );

//	m_pD3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
//	m_pD3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	m_pD3ddev->SetRenderState( D3DRS_LIGHTING, FALSE );

	m_pD3ddev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	m_pD3ddev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3ddev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pD3ddev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	
	m_pD3ddev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3ddev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//	시계 방향으로 출력
	m_pD3ddev->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

	return true;
}

void
CDirect3D::Release()
{
	if(m_pD3d)
	{
		SAFE_RELEASE_(m_pD3ddev);
		SAFE_RELEASE_(m_pD3d);
	}
}


bool
CDirect3D::SetUpVertexBuffer(LPDIRECT3DVERTEXBUFFER9 &pVB, void *pVertex, DWORD customfvf, UINT sizealloc)
{
	void *pVBuffer = NULL;
	
	if(FAILED(m_pD3ddev->CreateVertexBuffer(sizealloc, 0, customfvf, D3DPOOL_MANAGED, &pVB, NULL)))
	{
		return false;
	}
	if(FAILED(pVB->Lock(0, sizealloc, (void**)&pVBuffer, D3DLOCK_DISCARD)))
	{
		return false;
	}
	
	memcpy(pVBuffer, pVertex, sizealloc);
	
	pVB->Unlock();
	
	return true;
}

bool
CDirect3D::SetUpIndexBuffer(LPDIRECT3DINDEXBUFFER9 &pIB, void *pIndex, UINT sizealloc)
{
	void *pIBuffer = NULL;
	
	if(FAILED(m_pD3ddev->CreateIndexBuffer(sizealloc, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &pIB, NULL)))
	{
		return false;
	}
	if(FAILED(pIB->Lock(0, sizealloc, (void**)&pIBuffer, D3DLOCK_DISCARD)))
	{
		return false;
	}
	
	memcpy(pIBuffer, pIndex, sizealloc);	
	
	pIB->Unlock();
	
	return true;
}

bool
CDirect3D::SetUpTexture(LPDIRECT3DTEXTURE9 &pTexture, char *texturename)
{
	if(FAILED(D3DXCreateTextureFromFile(m_pD3ddev, texturename, &pTexture)))
	{
		return false;
	}
	
	return true;
}

bool
CDirect3D::SetUpTexture(LPDIRECT3DTEXTURE9 &pTexture, char *texturename, DWORD colorkey)
{
	if(FAILED(D3DXCreateTextureFromFileEx(m_pD3ddev, texturename, 
		D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR,
		D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, colorkey, NULL, NULL, &pTexture)))
	{
		return false;
	}
	
	return true;
}

void
CDirect3D::ColorKeyEnable()
{	
	//m_pD3ddev->SetRenderState(D3DRS_COLORVERTEX,TRUE);

   	m_pD3ddev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pD3ddev->SetRenderState(D3DRS_ALPHAREF,0x00);
	m_pD3ddev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

void
CDirect3D::ColorKeyDisable()
{
	m_pD3ddev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void
CDirect3D::AlphaBlendEnable(int alpha)
{
	m_pD3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	
	m_pD3ddev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(alpha,255,255,255));
	
	m_pD3ddev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	m_pD3ddev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR );
	m_pD3ddev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE );
	
	m_pD3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); 
	m_pD3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	//m_pD3ddev->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
}

void
CDirect3D::AlphaBlendDisable()
{
	m_pD3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void
CDirect3D::SetTextureBase(int stage, LPDIRECT3DTEXTURE9 &pTexture)
{
	m_pD3ddev->SetTexture(stage, pTexture);
	m_pD3ddev->SetTextureStageState(stage, D3DTSS_TEXCOORDINDEX, 0);
	m_pD3ddev->SetTextureStageState(stage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3ddev->SetTextureStageState(stage, D3DTSS_COLOROP, D3DTOP_MODULATE);
	
	m_pD3ddev->SetSamplerState(stage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3ddev->SetSamplerState(stage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}

void 
CDirect3D::SetTextureBaseForDiffuse(int stage, LPDIRECT3DTEXTURE9 &pTexture)
{
	m_pD3ddev->SetTexture(stage, pTexture);
	m_pD3ddev->SetTextureStageState(stage, D3DTSS_TEXCOORDINDEX, 0);
	m_pD3ddev->SetTextureStageState(stage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3ddev->SetTextureStageState(stage, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pD3ddev->SetTextureStageState(stage, D3DTSS_COLOROP, D3DTOP_MODULATE);
	
	m_pD3ddev->SetSamplerState(stage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3ddev->SetSamplerState(stage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
}

void 
CDirect3D::SetTextureLightMap(int stage, LPDIRECT3DTEXTURE9 &pTexture)
{
	m_pD3ddev->SetTexture(stage, pTexture);
	m_pD3ddev->SetTextureStageState(stage, D3DTSS_TEXCOORDINDEX, 0);
	m_pD3ddev->SetTextureStageState(stage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3ddev->SetTextureStageState(stage, D3DTSS_COLORARG2, D3DTA_CURRENT);
	m_pD3ddev->SetTextureStageState(stage, D3DTSS_COLOROP, D3DTOP_ADD);
	
	m_pD3ddev->SetSamplerState(stage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3ddev->SetSamplerState(stage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}

void 
CDirect3D::SetTextureDetailMap(int stage, LPDIRECT3DTEXTURE9 &pTexture)
{
	m_pD3ddev->SetTexture(stage, pTexture);
	m_pD3ddev->SetTextureStageState(stage, D3DTSS_TEXCOORDINDEX, 0);
	m_pD3ddev->SetTextureStageState(stage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3ddev->SetTextureStageState(stage, D3DTSS_COLORARG2, D3DTA_CURRENT);
	m_pD3ddev->SetTextureStageState(stage, D3DTSS_COLOROP, D3DTOP_ADDSIGNED);
	
	m_pD3ddev->SetSamplerState(stage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3ddev->SetSamplerState(stage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}

LPDIRECT3DDEVICE9
CDirect3D::GetDevice()
{
	return m_pD3ddev;
}
