//#include "stdafx.h"

#include "TextInput.h"

int inputline = 0,maxline = 0;
char View_Str_1[12];
char Jang_ID[20];

char				m_StrIn[256];		// 입력을 받는 문자열

BOOL CChatManager::Init(CDirect3D *pDirect3d,int fontsize)
{
	m_bInput = FALSE;
	m_bCursor = FALSE;
	m_Color = 0xFFFFFFFF;
	m_CurPosX = 0;
	m_NameLen = 0;
	m_pFontCom = NULL;
	m_OutCount = 0;
	m_hEdit = NULL;
	m_hWnd = NULL;
	m_ChatBuf = NULL;
	
	memset(&m_RectIn, 0, sizeof(RECT));
	memset(&m_RectOut, 0, sizeof(RECT));
	memset(m_StrIn, 0, sizeof(m_StrIn));
	memset(m_StrOut, 0, sizeof(m_StrOut));
	memset(m_Name, 0, sizeof(m_Name));
	
	g_pAppChat = this; // 이 클래스의 주소 입력
	
	m_pDirect3d     = NULL;
	m_pDirect3d = pDirect3d;

	m_pD3dxfont = NULL;
	m_pD3dxfont = new CD3dxFont();
	if(!m_pD3dxfont->CreateFONT(m_pDirect3d->GetDevice(), (char*)GetStockObject(SYSTEM_FONT), fontsize))
		return FALSE;


	return TRUE;
}

BOOL CChatManager::CleanUp()
{
	SAFE_DELETE_(m_pD3dxfont);
	
	return TRUE;
}

BOOL CChatManager::LinkDevice(/*ID3DXFont* pFont,*/ HWND hWnd, HINSTANCE hInst, int line)
{
///	if (pFont == NULL)
//		return FALSE;

//	m_pFontCom = pFont;

	m_hEdit = CreateWindow("edit", NULL, WS_CHILD, 10, 10, 600, 20, hWnd,
		(HMENU) ID_EDIT, hInst, NULL);

	SendMessage(m_hEdit, EM_LIMITTEXT, (WPARAM)line, 0);

	OldEditProc = (WNDPROC) SetWindowLong(m_hEdit, GWL_WNDPROC, (LONG) EditSubProc);
	m_hWnd = hWnd;
	maxline = line;

	return TRUE;
}

BOOL CChatManager::RestoreDevice(ID3DXFont* pFont)
{
	if (pFont == NULL)
		return FALSE;

	m_pFontCom = pFont;

	return TRUE;
}

BOOL CChatManager::SetColor(D3DCOLOR Color)
{
	m_Color = Color;
	return TRUE;
}

BOOL CChatManager::SetInPos(int x, int y)
{
	m_RectIn.left = x;
	m_RectIn.top = y;
	m_RectIn.right = x + 800;
	m_RectIn.bottom = y + 20;

	return TRUE;
}

BOOL CChatManager::SetOutPos(int x, int y)
{
	m_RectOut.left = x;
	m_RectOut.top = y;
	m_RectOut.right = x + 800;
	m_RectOut.bottom = y + 20;

	return TRUE;
}

BOOL CChatManager::AddOutStr(char* Str)
{
	memcpy(&m_StrOut[0], &m_StrOut[0], 256);
//	m_OutCount = m_OutCount;


	memset(&m_StrOut[0], 0, 256);
	memcpy(&m_StrOut[0], Str, strlen(Str));
//	m_OutCount = 10;

	return TRUE;
}

BOOL CChatManager::InputCancel()
{
	m_bInput = FALSE;
	m_CurPosX = 0;
	memset(m_StrIn, 0, 256);
	SetWindowText(m_hEdit, "");
	SetFocus(m_hWnd);

	return TRUE;
}

void CChatManager::InputEnd()
{
	if (m_StrIn[0] != NULL)
	{
		memcpy(&m_StrOut[0], m_StrIn, sizeof(m_StrIn));
	
		m_bInput = FALSE;
		m_CurPosX = 0;
		memset(m_StrIn, 0, 256);
		SetWindowText(m_hEdit, "");
		SetFocus(m_hWnd);
	}

}

BOOL CChatManager::InputStart()
{
	m_bInput = TRUE;
	ZeroMemory(m_StrIn, 256);
	SetWindowText(m_hEdit, "");

	return TRUE;
}

BOOL CChatManager::Render(BOOL Pass_yn)
{
	char temp[20];

	if (m_bInput == TRUE)
	{
		SetStrIn();
	
		if(Pass_yn == TRUE)
		{
			inputline = strlen(m_StrIn);
			ZeroMemory(temp, sizeof(temp));
			for(int i=0; i<inputline; i++)
				*(temp+i)   = '*';
			
			if (m_bCursor == TRUE)
			{
				temp[m_CurPosX] = '|';
				temp[m_CurPosX + 1] = '\0';
			}

			m_pD3dxfont->DrawText(m_RectOut.left,m_RectOut.top,m_RectOut.right,
				m_RectOut.bottom,m_Color,temp, maxline);
			
			temp[m_CurPosX] = '\0';
		}else{
			inputline = strlen(m_StrIn);
			
			if (m_bCursor == TRUE)
			{
				m_StrIn[m_CurPosX] = '|';
				m_StrIn[m_CurPosX + 1] = '\0';
			}
				
			m_pD3dxfont->DrawText(m_RectIn.left,m_RectIn.top,m_RectIn.right,
				m_RectIn.bottom,m_Color,m_StrIn, maxline );
		}
		m_StrIn[m_CurPosX] = '\0';
		SetFocus(m_hEdit);
	}/*else
	{
		if(Pass_yn == TRUE)
		{
			int len = strlen(&m_StrOut[0]);
			ZeroMemory(temp, sizeof(temp));
			for(int i=0; i<len; i++)
				*(temp+i)   = '*';
			
			m_pD3dxfont->DrawText(m_RectOut.left,m_RectOut.top,m_RectOut.right,
				m_RectOut.bottom,m_Color,temp, maxline);
		}else
			m_pD3dxfont->DrawText(m_RectOut.left,m_RectOut.top,m_RectOut.right,
				m_RectOut.bottom,m_Color,&m_StrOut[0], maxline);
	}
/*	DWORD NowTick = timeGetTime();
	static DWORD LastTick = NowTick;

	if (NowTick > LastTick + 1000)
	{
		LastTick = NowTick;
		if (m_OutCount != 0)m_OutCount--;
	}
	*/
	return TRUE;
}

BOOL CChatManager::SetStrIn()
{
	DWORD NowTick = timeGetTime();
	static DWORD LastTick = NowTick;

	GetWindowText(m_hEdit, m_StrIn, maxline);
	
	m_CurPosX = GetWindowTextLength(m_hEdit);
	
	if (NowTick > LastTick + 500)
	{
		LastTick = NowTick;
		m_bCursor ^= 1;
	}

	return TRUE;
}

BOOL CChatManager::SetMyName(char* Name)
{
	m_NameLen = strlen(Name);

	if (m_NameLen > 40)
		return FALSE;

	memset(m_Name, 0, sizeof(m_Name));
	memcpy(m_Name, Name, m_NameLen);

	return TRUE;
}

BOOL CChatManager::SetChatBuf(char* Buf)
{
	m_ChatBuf = Buf;
	return TRUE;
}

LRESULT CALLBACK EditSubProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_CHAR:
		if (wParam == VK_RETURN)
			return 0;
		else if (wParam == VK_ESCAPE)
			return 0;
		else if (wParam == VK_TAB)
			return 0;

		if(strlen(m_StrIn) >= maxline)return 0;

		break;
	}

	return CallWindowProc(OldEditProc, hWnd, iMessage, wParam, lParam);
}