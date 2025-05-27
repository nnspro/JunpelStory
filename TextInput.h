#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include <mmsystem.h>

#include "Direct3D.h"
#include "TextOutput.h"

#define ID_EDIT		101101 // ����Ʈ �ڽ� ���̵� 

class CChatManager
{
private:
	int					m_CurPosX;			// Cursor ��ġ
	char*				m_ChatBuf;			// ���ø����̼ǰ� ����
	char				m_Name[15];			// �̸� 
	int					m_NameLen;			// �̸��� ����

	CD3dxFont			*m_pD3dxfont;
	CDirect3D			*m_pDirect3d;

	ID3DXFont*			m_pFontCom;			// ��Ʈ �� 
	HWND				m_hEdit;			// ����Ʈ �ڽ� �ڵ�
	HWND				m_hWnd;				// ������ �ڵ�

	BOOL				m_bInput;			// �Է� ����ΰ�?
	BOOL				m_bCursor;			// Cursor�� ���ִ���?

	RECT				m_RectOut;		// ����� ��ġ
	RECT				m_RectIn;			// �Է� ���� ��ġ

	D3DCOLOR			m_Color;			// ��Ʈ ����
	int					m_OutCount;		// ���ڿ� ��� ���� ��

	BOOL Init();							// �ʱ�ȭ
	BOOL CleanUp();							// ���� �غ�
	BOOL SetStrIn();						// ����Ʈ �ڽ��� ������ ���ڿ� ������ ����

public:
//	CChatManager() ;//{ Init(CDirect3D *pDirect3d); }				// ������
	~CChatManager() { CleanUp(); }			// �Ҹ���
	
	BOOL Init(CDirect3D *pDirect3d,int fontsize);
	BOOL LinkDevice(HWND hWnd, HINSTANCE hInst,int line);	// ��ġ ����
	BOOL RestoreDevice(ID3DXFont* pFont);	// ��Ʈ �� ����
	
	BOOL AddOutStr(char* Str);				// ��� ���ڿ� �߰�
	BOOL InputStart();						// �Է� ��� ���� 
	BOOL InputCancel();						// �Է� ��� ���
	void InputEnd();						// �Է��� ������ ��� ���ڿ��� �߰�

	BOOL SetColor(D3DCOLOR Color);			// ��� ���� ����
	BOOL SetOutPos(int x, int y);	// ��� ��ġ ����
	BOOL SetInPos(int x, int y);			// �Է� ��ġ ����

	BOOL SetMyName(char* Name);				// �̸� ����
	BOOL SetChatBuf(char* Buf);				// ���� ���� ����

	BOOL Render(BOOL Pass_yn);              // ������
	
	char				m_StrOut[256];  	// ����� ���ڿ�
	
}; // CChatManager
static WNDPROC OldEditProc = NULL;	// ���� ����Ʈ �ڽ��� Proc
static CChatManager* g_pAppChat = NULL; // �� Ŭ������ �ּҸ� ����
// ����Ŭ���� Proc
static LRESULT CALLBACK EditSubProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

